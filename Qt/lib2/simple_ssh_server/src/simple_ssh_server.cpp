#include "simplesshserver.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <string>
#include <vector>
#include <libssh/libssh.h>
#include <libssh/server.h>

#ifdef Q_OS_WIN
#include <windows.h>
#ifndef PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE
#define PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE 0x00020016
#endif
#else
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <pty.h>      // LIBS += -lutil
#include <sys/wait.h>
#endif

SimpleSshServer::SimpleSshServer(quint16 port, QObject *parent)
    : QThread(parent), m_port(port), m_keyPath("./ssh_host_rsa_key"), m_sshBind(nullptr), m_running(false) {
    ssh_init();
}

SimpleSshServer::~SimpleSshServer() {
    stopServer();
    ssh_finalize();
}

bool SimpleSshServer::startServer() {
    if (!ensureHostKeyExists()) return false;

    m_sshBind = ssh_bind_new();
    if (!m_sshBind) return false;

    ssh_bind_options_set(m_sshBind, SSH_BIND_OPTIONS_BINDPORT, &m_port);
    QByteArray localKey = QDir::current().absoluteFilePath(m_keyPath).toLocal8Bit();
    ssh_bind_options_set(m_sshBind, SSH_BIND_OPTIONS_RSAKEY, localKey.constData());

    if (ssh_bind_listen(m_sshBind) < 0) {
        qCritical() << "SSH Не удалось открыть порт:" << ssh_get_error(m_sshBind);
        ssh_bind_free(m_sshBind);
        m_sshBind = nullptr;
        return false;
    }

    m_running = true;
    this->start();
    return true;
}

void SimpleSshServer::stopServer() {
    if (!m_running) return;
    m_running = false;

    // СИЛОВОЕ ЗАВЕРШЕНИЕ ПОТОКА:
    // Так как accept заблокирован на уровне ядра, мы принудительно гасим поток ОС.
    // Это на 100% выгружает поток из памяти без зависаний.
    if (this->isRunning()) {
        this->terminate();
        this->wait(); // Дожидаемся полной выгрузки дескрипторов потока
    }

    if (m_sshBind) {
        ssh_bind_free(m_sshBind);
        m_sshBind = nullptr;
    }
    qInfo() << "SSH сервер полностью остановлен.";
}

bool SimpleSshServer::ensureHostKeyExists() {
    if (QFile::exists(m_keyPath)) return true;
    ssh_key key = nullptr;
    if (ssh_pki_generate(SSH_KEYTYPE_RSA, 2048, &key) != SSH_OK) return false;
    QByteArray localKey = m_keyPath.toLocal8Bit();
    int rc = ssh_pki_export_privkey_file(key, "", nullptr, nullptr, localKey.constData());
    ssh_key_free(key);
    return (rc == SSH_OK);
}

void SimpleSshServer::run() {
    while (m_running) {
        ssh_session session = ssh_new();
        if (!session) continue;

        if (!m_running || !m_sshBind) {
            ssh_free(session);
            break;
        }

        int rc = ssh_bind_accept(m_sshBind, session);
        if (rc == SSH_OK && m_running) {
            handleClient(session);
        } else {
            ssh_free(session);
        }
    }
}

void SimpleSshServer::handleClient(ssh_session session) {
    if (ssh_handle_key_exchange(session) != SSH_OK) {
        ssh_disconnect(session); ssh_free(session); return;
    }

    ssh_message message;
    bool authenticated = false;

    while (!authenticated && m_running) {
        message = ssh_message_get(session);
        if (!message) break;

        if (ssh_message_type(message) == SSH_REQUEST_AUTH && message && ssh_message_subtype(message) == SSH_AUTH_METHOD_PASSWORD) {
            const char* rawUser = ssh_message_auth_user(message);
            std::string user = rawUser ? rawUser : "";

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
            const char* rawPassword = ssh_message_auth_password(message);
            std::string password = rawPassword ? rawPassword : "";
#pragma GCC diagnostic pop

            if (user == "admin" && password == "my_secure_password") {
                authenticated = true;
                ssh_message_auth_reply_success(message, 0);
            } else {
                ssh_message_reply_default(message);
            }
        } else {
            if (message) ssh_message_reply_default(message);
        }
        if (message) ssh_message_free(message);
    }

    if (!authenticated) { ssh_disconnect(session); ssh_free(session); return; }

    ssh_channel channel = nullptr;
    while (m_running) {
        message = ssh_message_get(session);
        if (!message) break;
        if (ssh_message_type(message) == SSH_REQUEST_CHANNEL_OPEN && ssh_message_subtype(message) == SSH_CHANNEL_SESSION) {
            channel = ssh_message_channel_request_open_reply_accept(message);
            ssh_message_free(message);
            break;
        }
        ssh_message_reply_default(message);
        ssh_message_free(message);
    }

    if (channel) {
        while (m_running) {
            message = ssh_message_get(session);
            if (!message) break;
            if (ssh_message_type(message) == SSH_REQUEST_CHANNEL &&
                (ssh_message_subtype(message) == SSH_CHANNEL_REQUEST_PTY || ssh_message_subtype(message) == SSH_CHANNEL_REQUEST_SHELL)) {
                ssh_message_channel_request_reply_success(message);
                ssh_message_free(message);

                ssh_set_blocking(session, 0);
                ssh_channel_set_blocking(channel, 0);

                runConsole(channel);
                break;
            }
            ssh_message_reply_default(message);
            ssh_message_free(message);
        }
    }
    ssh_disconnect(session);
    ssh_free(session);
}

void SimpleSshServer::runConsole(ssh_channel channel) {
    std::vector<char> buffer;
    buffer.resize(4096);

#ifdef Q_OS_WIN
    HANDLE hInPipeRead = NULL, hInPipeWrite = NULL;
    HANDLE hOutPipeRead = NULL, hOutPipeWrite = NULL;

    CreatePipe(&hInPipeRead, &hInPipeWrite, NULL, 0);
    CreatePipe(&hOutPipeRead, &hOutPipeWrite, NULL, 0);

    DWORD pipeMode = PIPE_NOWAIT;
    SetNamedPipeHandleState(hOutPipeRead, &pipeMode, NULL, NULL);

    HPCON hPC = NULL;
    COORD size = {80, 24};
    HRESULT hr = CreatePseudoConsole(size, hInPipeRead, hOutPipeWrite, 0, &hPC);
    if (FAILED(hr)) {
        CloseHandle(hInPipeRead); CloseHandle(hInPipeWrite);
        CloseHandle(hOutPipeRead); CloseHandle(hOutPipeWrite);
        return;
    }

    STARTUPINFOEXW siEx = {0};
    siEx.StartupInfo.cb = sizeof(STARTUPINFOEXW);
    siEx.StartupInfo.dwFlags = STARTF_USESTDHANDLES;

    SIZE_T attrSize = 0;
    InitializeProcThreadAttributeList(NULL, 1, 0, &attrSize);
    siEx.lpAttributeList = (PPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, attrSize);
    InitializeProcThreadAttributeList(siEx.lpAttributeList, 1, 0, &attrSize);
    UpdateProcThreadAttribute(siEx.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE, hPC, sizeof(HPCON), NULL, NULL);

    PROCESS_INFORMATION pi = {0};
    wchar_t cmdPath[] = L"cmd.exe /Q";
    CreateProcessW(NULL, cmdPath, NULL, NULL, TRUE, EXTENDED_STARTUPINFO_PRESENT, NULL, NULL, &siEx.StartupInfo, &pi);

    CloseHandle(hInPipeRead);
    CloseHandle(hOutPipeWrite);

    while (m_running) {
        DWORD bytesFromOs = 0;
        if (ReadFile(hOutPipeRead, buffer.data(), buffer.size(), &bytesFromOs, NULL) && bytesFromOs > 0) {
            ssh_channel_write(channel, buffer.data(), bytesFromOs);
        }

        DWORD exitCode;
        if (GetExitCodeProcess(pi.hProcess, &exitCode) && exitCode != STILL_ACTIVE) {
            break;
        }

        int bytesFromNet = ssh_channel_read_nonblocking(channel, buffer.data(), buffer.size(), 0);
        if (bytesFromNet > 0) {
            DWORD bytesWritten = 0;
            WriteFile(hInPipeWrite, buffer.data(), bytesFromNet, &bytesWritten, NULL);
        } else if (bytesFromNet < 0 || ssh_channel_is_eof(channel)) {
            break;
        }

        QThread::msleep(10);
    }

    TerminateProcess(pi.hProcess, 0);
    CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
    ClosePseudoConsole(hPC);
    CloseHandle(hInPipeWrite); CloseHandle(hOutPipeRead);
    DeleteProcThreadAttributeList(siEx.lpAttributeList);
    HeapFree(GetProcessHeap(), 0, siEx.lpAttributeList);

#else
    int masterFd = -1;
    pid_t pid = forkpty(&masterFd, nullptr, nullptr, nullptr);

    if (pid < 0) return;

    if (pid == 0) {
        execl("/bin/sh", "/bin/sh", "-i", nullptr);
        std::exit(1);
    }

    int flags = fcntl(masterFd, F_GETFL, 0);
    fcntl(masterFd, F_SETFL, flags | O_NONBLOCK);

    struct pollfd fds;
    fds.fd = masterFd;
    fds.events = POLLIN;

    while (m_running) {
        int ret = poll(&fds, 1, 10);

        if (ret > 0 && (fds.revents & POLLIN)) {
            ssize_t bytesFromOs = ::read(masterFd, buffer.data(), buffer.size());
            if (bytesFromOs > 0) {
                ssh_channel_write(channel, buffer.data(), bytesFromOs);
            } else if (bytesFromOs < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
                break;
            }
        }

        int status;
        if (waitpid(pid, &status, WNOHANG) > 0) {
            break;
        }

        int bytesFromNet = ssh_channel_read_nonblocking(channel, buffer.data(), buffer.size(), 0);
        if (bytesFromNet > 0) {
            ssize_t nw = ::write(masterFd, buffer.data(), bytesFromNet);
            Q_UNUSED(nw);
        } else if (bytesFromNet < 0 || ssh_channel_is_eof(channel)) {
            break;
        }

        QThread::msleep(10);
    }

    ::close(masterFd);
    kill(pid, SIGTERM);
    waitpid(pid, nullptr, 0);
#endif
}

//--------------------------------------------------------------------------------
#include <QTextStream>
#include <QString>
#include <QFile>

#include <unistd.h>
#include <execinfo.h>
#include <signal.h>
//--------------------------------------------------------------------------------
QString get_signame(int signum)
{
    QString temp;
    switch(signum)
    {
    case SIGHUP:    temp = "SIGHUP      1 Hangup (POSIX).";break;
    case SIGINT:    temp = "SIGINT	2 Interrupt (ANSI).";break;
    case SIGQUIT:   temp = "SIGQUIT	3 Quit (POSIX).";break;
    case SIGILL:    temp = "SIGILL	4 Illegal instruction (ANSI).";break;
    case SIGTRAP:   temp = "SIGTRAP	5 Trace trap (POSIX).";break;
    case SIGABRT:   temp = "SIGABRT	6 Abort (ANSI).";break;
    case SIGBUS:    temp = "SIGBUS	7 BUS error (4.2 BSD).";break;
    case SIGFPE:    temp = "SIGFPE	8 Floating-point exception (ANSI).";break;
    case SIGKILL:   temp = "SIGKILL	9 Kill, unblockable (POSIX).";break;
    case SIGUSR1:   temp = "SIGUSR1     10 User-defined signal 1 (POSIX).";break;
    case SIGSEGV:   temp = "SIGSEGV	11 Segmentation violation (ANSI).";break;
    case SIGUSR2:   temp = "SIGUSR2	12 User-defined signal 2 (POSIX).";break;
    case SIGPIPE:   temp = "SIGPIPE	13 Broken pipe (POSIX).";break;
    case SIGALRM:   temp = "SIGALRM	14 Alarm clock (POSIX).";break;
    case SIGTERM:   temp = "SIGTERM	15 Termination (ANSI).";break;
    case SIGSTKFLT: temp = "SIGSTKFLT   16 Stack fault.";break;
    case SIGCHLD:   temp = "SIGCHLD	17 Child status has changed (POSIX).";break;
    case SIGCONT:   temp = "SIGCONT	18 Continue (POSIX).";break;
    case SIGSTOP:   temp = "SIGSTOP	19 Stop, unblockable (POSIX).";break;
    case SIGTSTP:   temp = "SIGTSTP	20 Keyboard stop (POSIX).";break;
    case SIGTTIN:   temp = "SIGTTIN	21 Background read from tty (POSIX).";break;
    case SIGTTOU:   temp = "SIGTTOU	22 Background write to tty (POSIX).";break;
    case SIGURG:    temp = "SIGURG	23 Urgent condition on socket (4.2 BSD).";break;
    case SIGXCPU:   temp = "SIGXCPU	24 CPU limit exceeded (4.2 BSD).";break;
    case SIGXFSZ:   temp = "SIGXFSZ	25 File size limit exceeded (4.2 BSD).";break;
    case SIGVTALRM: temp = "SIGVTALRM   26 Virtual alarm clock (4.2 BSD).";break;
    case SIGPROF:   temp = "SIGPROF	27 Profiling alarm clock (4.2 BSD).";break;
    case SIGWINCH:  temp = "SIGWINCH    28 Window size change (4.3 BSD, Sun).";break;
    case SIGIO:     temp = "SIGIO	29 I/O now possible (4.2 BSD).";break;
    case SIGPWR:    temp = "SIGPWR	30 Power failure restart (System V).";break;
    case SIGSYS:    temp = "SIGSYS	31 Bad system call.";break;
    case _NSIG:     temp = "_NSIG	65 Biggest signal number + 1 (including real-time signals).";    break;
    default:        temp  = QString("unknown signum: %1").arg(signum);break;
    }
    return temp;
}
//--------------------------------------------------------------------------------
static inline void printStackTrace(QFile *pFile,
                                   int signalNumber,
                                   const char* signalName,
                                   unsigned int max_frames = 50)
{
    QTextStream out(pFile);
    out.setGenerateByteOrderMark(false);
    if (signalName)
        out << QString("Caught signal %1 (%2)\n").arg(QString::number(signalNumber)).arg(signalName);
    else
        out << QString("Caught signal %1\n").arg(QString::number(signalNumber));
    out.flush();
    // storage array for stack trace address data
    void* addrlist[max_frames+1];
    // retrieve current stack addresses
    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));
    if (addrlen == 0)
    {
        out << "Stack address length is empty.\n";
        return;
    }
    // create readable strings to each frame.
    backtrace_symbols_fd(addrlist, addrlen, pFile->handle());
    /*
     backtrace_symbols uses malloc. Its better to use backtrace_symbols_fd.
     */
    char** symbollist = backtrace_symbols(addrlist, addrlen);
    // print the stack trace.
    for (int i = 4; i < addrlen; i++)
    {
        out << QString("%1\n").arg(symbollist[i]);
    }
    free(symbollist);
}
//--------------------------------------------------------------------------------
void signalHandler(int signum)
{
    // associate each signal with a signal name string.
    const char* name = NULL;
    switch(signum)
    {
    case SIGHUP:    name = "SIGHUP";    break;
    case SIGINT:    name = "SIGINT";    break;
    case SIGQUIT:   name = "SIGQUIT";   break;
    case SIGILL:    name = "SIGILL";    break;
    case SIGTRAP:   name = "SIGTRAP";   break;
    case SIGABRT:   name = "SIGABRT";   break;
    case SIGBUS:    name = "SIGBUS";    break;
    case SIGFPE:    name = "SIGFPE";    break;
    case SIGKILL:   name = "SIGKILL";   break;
    case SIGUSR1:   name = "SIGUSR1";   break;
    case SIGSEGV:   name = "SIGSEGV";   break;
    case SIGUSR2:   name = "SIGUSR2";   break;
    case SIGPIPE:   name = "SIGPIPE";   break;
    case SIGALRM:   name = "SIGALRM";   break;
    case SIGTERM:   name = "SIGTERM";   break;
    case SIGSTKFLT: name = "SIGSTKFLT"; break;
    case SIGCHLD:   name = "SIGCHLD";   break;
    case SIGCONT:   name = "SIGCONT";   break;
    case SIGSTOP:   name = "SIGSTOP";   break;
    case SIGTSTP:   name = "SIGTSTP";   break;
    case SIGTTIN:   name = "SIGTTIN";   break;
    case SIGTTOU:   name = "SIGTTOU";   break;
    case SIGURG:    name = "SIGURG";    break;
    case SIGXCPU:   name = "SIGXCPU";   break;
    case SIGXFSZ:   name = "SIGXFSZ";   break;
    case SIGVTALRM: name = "SIGVTALRM"; break;
    case SIGPROF:   name = "SIGPROF";   break;
    case SIGWINCH:  name = "SIGWINCH";  break;
    case SIGIO:     name = "SIGIO";     break;
    case SIGPWR:    name = "SIGPWR";    break;
    case SIGSYS:    name = "SIGSYS";    break;
    case _NSIG:     name = "_NSIG";     break;
    default:  break;
    }
    // Dump a stack trace to a file.
    QFile stackTraceFile;
    stackTraceFile.setFileName("xxx");
    if (stackTraceFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        printStackTrace(&stackTraceFile, signum, name);
        stackTraceFile.close();
    }
    if (name)
        fprintf(stderr, "Caught signal %d\n", signum);
    else
        fprintf(stderr, "Caught signal %d (%s)\n", signum, name);

    // If you caught one of the above signals, it is likely you just
    // want to quit your program right now.

    exit(signum);
}
//--------------------------------------------------------------------------------
void posix_death_signal(int signum)
{
    const int max_deep = 100;
    void  *traces[max_deep];

    qDebug() << get_signame(signum);
    int ntprs = backtrace(traces, max_deep);
    backtrace_symbols_fd(traces, ntprs, STDERR_FILENO);

    signal(signum, SIG_DFL);
    exit(signum);
}
//--------------------------------------------------------------------------------
void set_signals(void)
{
#if 0
    signal(SIGABRT, signalHandler);
#else
    for(int sign = 1; sign <= 31; sign ++)
    {
        //signal(sign, posix_death_signal);
        signal(sign, signalHandler);
    }
    signal(_NSIG, signalHandler);
#endif
}
//--------------------------------------------------------------------------------

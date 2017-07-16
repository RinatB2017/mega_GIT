#include "qtelnet.h"

#include <iostream>
#include <iomanip>
#include <netdb.h>
#include <cstring>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <poll.h>


using namespace std;

int qtelnet::telnet_connect(qtelnet &tracker,
                            const char *host,
                            const char *port)
{
    if (tracker.connected)
    {
        cerr << "ERROR: qtelnet tracker state is connected\n";
        return -1;
    }

    struct addrinfo hints;
    struct addrinfo *ai;
    int sock;
    int rs;
    telnet_t *telnet;
    pthread_t thread;

    /* look up server host */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((rs = getaddrinfo(host, port, &hints, &ai)) != 0)
    {
        cerr << "getaddrinfo() failed for " << host << ": " << gai_strerror(rs);
        return 1;
    }

    /* create socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        cerr << "socket() failed: " << strerror(errno) << "\n";
        return 2;
    }

    /* connect */
    if (connect(sock, ai->ai_addr, ai->ai_addrlen) == -1)
    {
        cerr << "connect() failed: " << strerror(errno) << "\n";
        return 3;
    }

    /* free address lookup info */
    freeaddrinfo(ai);

    /* initialize telnet box */
    telnet_telopt_t telopts[] = {
        { TELNET_TELOPT_ECHO,		TELNET_WONT, TELNET_DONT },
        { TELNET_TELOPT_TTYPE,		TELNET_WILL, TELNET_DONT },
        { TELNET_TELOPT_COMPRESS2,	TELNET_WONT, TELNET_DO   },
        { TELNET_TELOPT_MSSP,		TELNET_WONT, TELNET_DO   },
        { -1, 0, 0 }
    };
    telnet = telnet_init(telopts, telnet_event_handler, 0, &tracker);

    tracker.sockfd = sock;
    tracker.telnet = telnet;
    tracker.connected = 1;

    if (!tracker.worker_running)
    {
        rs = pthread_create(&thread, NULL, &worker, &tracker);
        if (rs)
        {
            cerr << "pthread_create() failed: " << strerror(errno) << "\n";
            return 4;
        }
        tracker.workerThread = thread;
    }
    else
    {
        cerr << "ERROR: qtelnet tracker has already track a worker thread\n";
        return 4;
    }

    return 0;
}

void qtelnet::telnet_disconnect(qtelnet &tracker)
{
    if (tracker.sockfd != -1)
    {
        close(tracker.sockfd);
        tracker.sockfd = -1;
    }
    if (tracker.telnet != NULL)
    {
        telnet_free(tracker.telnet);
        tracker.telnet = NULL;
    }

    // Stop worker thread
    if (tracker.worker_running)
    {
        tracker.connected = 0;
        pthread_join(tracker.workerThread, NULL);
    }
}

void qtelnet::send_text(qtelnet &tracker, const char *text, int len)
{
    static char crlf[] = { '\r', '\n' };

    cout << "Sending: '" << text << "'\n";
    telnet_send(tracker.telnet, text, len);
    telnet_send(tracker.telnet, crlf, 2);
}

qtelnet::qtelnet()
{
    struct termios tios;

    sockfd = -1;
    orig_tios = new termios;
    telnet = NULL;
    do_echo = 1;
    connected = 0;
    workerThread = -1;
    worker_running = 0;
    data_recv = NULL;

    /* get current terminal settings, set raw mode, make sure
     * to restore terminal settings */
    tcgetattr(STDOUT_FILENO, orig_tios);
    tios = *orig_tios;
    cfmakeraw(&tios);
    tcsetattr(STDOUT_FILENO, TCSADRAIN, &tios);
}

qtelnet::~qtelnet()
{
    // Stop worker thread in case telnet_disconnect is not called
    connected = 0;
    if (worker_running)
    {
        pthread_join(workerThread, NULL);
    }

    if (orig_tios != NULL)
    {
        tcsetattr(STDOUT_FILENO, TCSADRAIN, orig_tios);
        delete orig_tios;
    }
}

void qtelnet::set_data_recv_callback(const qtelnet::data_recv_callback_t callback)
{
    this->data_recv = callback;
}

void qtelnet::set_data_recv_bundle(void *bundle)
{
    data_recv_bundle = bundle;
}


/*
 * =================================================
 * Private members
 * =================================================
*/

void qtelnet::telnet_event_handler(telnet_t *telnet,
                                   telnet_event_t *event,
                                   void *user_data)
{
    qtelnet *tracker = (qtelnet*) user_data;
    char buffer[1024];

    switch (event->type)
    {
    case TELNET_EV_DATA:
        /* data received */
        strncpy(buffer, event->data.buffer, event->data.size);
        buffer[event->data.size] = 0;
        cout << buffer << "\r\n";
        cout << flush;
        if (tracker->data_recv != NULL)
        {
            tracker->data_recv(event->data.buffer, event->data.size, tracker->data_recv_bundle);
        }
        break;

    case TELNET_EV_SEND:
        /* data must be sent */
        send_data(tracker, event->data.buffer, event->data.size);
        break;

    case TELNET_EV_WILL:
        /* request to enable remote feature (or receipt) */
        /* we'll agree to turn off our echo if server wants us to stop */
        if (event->neg.telopt == TELNET_TELOPT_ECHO)
            tracker->do_echo = 0;
        break;

    case TELNET_EV_WONT:
        /* notification of disabling remote feature (or receipt) */
        if (event->neg.telopt == TELNET_TELOPT_ECHO)
            tracker->do_echo = 1;
        break;

    case TELNET_EV_DO:
        /* request to enable local feature (or receipt) */
        break;

    case TELNET_EV_DONT:
        /* demand to disable local feature (or receipt) */
        break;

    case TELNET_EV_TTYPE:
        /* respond to TTYPE commands */
        /* respond with our terminal type, if requested */
        if (event->ttype.cmd == TELNET_TTYPE_SEND) {
            telnet_ttype_is(telnet, getenv("TERM"));
        }
        break;

    case TELNET_EV_SUBNEGOTIATION:
        /* respond to particular subnegotiations */
        break;

    case TELNET_EV_ERROR:
        /* error */
        cerr << "ERROR: " << event->error.msg << "\n";
        break;

    default:
        /* ignore */
        break;
    }
}

int qtelnet::send_data(qtelnet *tracker, const char *buffer, size_t size)
{
    int rs;
    size_t len = size;

    /* send data */
    while (len > 0)
    {
        if ((rs = send(tracker->sockfd, buffer, len, 0)) == -1)
        {
            cerr << "send() failed: " << strerror(errno) << "\n";
            return -1;
        }
        else if (rs == 0)
        {
            cerr << "send() unexpectedly returned 0\n";
            return -1;
        }

        /* update pointer and size to see if we've got more to send */
        buffer += rs;
        len -= rs;
    }

    return (size - len);
}

void *qtelnet::worker(void *data)
{
    if (!data) pthread_exit(NULL);

    qtelnet *tracker = (qtelnet*) data;
    tracker->worker_running = 1;

    /* initialize poll descriptors */
    struct pollfd pfd[2];
    char buffer[1024];
    int rs;

    // TODO Use select instead of poll
    memset(pfd, 0, sizeof(pfd));
    pfd[0].fd = STDIN_FILENO;
    pfd[0].events = POLLIN;
    pfd[1].fd = tracker->sockfd;
    pfd[1].events = POLLIN;

    /* loop while both connections are open */
    while (tracker->connected && poll(pfd, 2, 2000) != -1)
    {
        /* read from stdin */
        /*
        if (pfd[0].revents & POLLIN)
        {
            if ((rs = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
            {
                _input(buffer, rs);
            }
            else if (rs == 0)
            {
                break;
            }
            else
            {
                fprintf(stderr, "recv(server) failed: %s\n",
                        strerror(errno));
                exit(1);
            }
        }
        */

        /* read from client */
        if (pfd[1].revents & POLLIN)
        {
            if ((rs = recv(tracker->sockfd, buffer, sizeof(buffer), 0)) > 0)
            {
                telnet_recv(tracker->telnet, buffer, rs);
            }
            else if (rs == 0)
            {
                break;
            }
            else
            {
                cerr << "recv(client) failed: " << strerror(errno) << "\n";
                pthread_exit(NULL);
            }
        }
    }

    cout << "Worker stopped\r\n";
    tracker->worker_running = 0;
    pthread_exit(NULL);
}

//--------------------------------------------------------------------------------
#include <QString>
#include <unistd.h>
#include <execinfo.h>
#include <signal.h>
//--------------------------------------------------------------------------------
QString get_signame(int signum)
{
    QString temp;
    switch(signum)
    {
    case SIGHUP:    temp = "SIGHUP    1 Hangup (POSIX).";break;
    case SIGINT:    temp = "SIGINT	  2 Interrupt (ANSI).";break;
    case SIGQUIT:   temp = "SIGQUIT	  3 Quit (POSIX).";break;
    case SIGILL:    temp = "SIGILL	  4 Illegal instruction (ANSI).";break;
    case SIGTRAP:   temp = "SIGTRAP	  5 Trace trap (POSIX).";break;
    case SIGABRT:   temp = "SIGABRT	  6 Abort (ANSI).";break;
    case SIGBUS:    temp = "SIGBUS	  7 BUS error (4.2 BSD).";break;
    case SIGFPE:    temp = "SIGFPE	  8 Floating-point exception (ANSI).";break;
    case SIGKILL:   temp = "SIGKILL	  9 Kill, unblockable (POSIX).";break;
    case SIGUSR1:   temp = "SIGUSR1   10 User-defined signal 1 (POSIX).";break;
    case SIGSEGV:   temp = "SIGSEGV	  11 Segmentation violation (ANSI).";break;
    case SIGUSR2:   temp = "SIGUSR2	  12 User-defined signal 2 (POSIX).";break;
    case SIGPIPE:   temp = "SIGPIPE	  13 Broken pipe (POSIX).";break;
    case SIGALRM:   temp = "SIGALRM	  14 Alarm clock (POSIX).";break;
    case SIGTERM:   temp = "SIGTERM	  15 Termination (ANSI).";break;
    case SIGSTKFLT: temp = "SIGSTKFLT 16 Stack fault.";break;
    case SIGCHLD:   temp = "SIGCHLD	  17 Child status has changed (POSIX).";break;
    case SIGCONT:   temp = "SIGCONT	  18 Continue (POSIX).";break;
    case SIGSTOP:   temp = "SIGSTOP	  19 Stop, unblockable (POSIX).";break;
    case SIGTSTP:   temp = "SIGTSTP	  20 Keyboard stop (POSIX).";break;
    case SIGTTIN:   temp = "SIGTTIN	  21 Background read from tty (POSIX).";break;
    case SIGTTOU:   temp = "SIGTTOU	  22 Background write to tty (POSIX).";break;
    case SIGURG:    temp = "SIGURG	  23 Urgent condition on socket (4.2 BSD).";break;
    case SIGXCPU:   temp = "SIGXCPU	  24 CPU limit exceeded (4.2 BSD).";break;
    case SIGXFSZ:   temp = "SIGXFSZ	  25 File size limit exceeded (4.2 BSD).";break;
    case SIGVTALRM: temp = "SIGVTALRM 26 Virtual alarm clock (4.2 BSD).";break;
    case SIGPROF:   temp = "SIGPROF	  27 Profiling alarm clock (4.2 BSD).";break;
    case SIGWINCH:  temp = "SIGWINCH  28 Window size change (4.3 BSD, Sun).";break;
    case SIGIO:     temp = "SIGIO	  29 I/O now possible (4.2 BSD).";break;
    case SIGPWR:    temp = "SIGPWR	  30 Power failure restart (System V).";break;
    case SIGSYS:    temp = "SIGSYS	  31 Bad system call.";break;
    default:        temp  = QString("unknown signum: %1").arg(signum);break;
    }
    return temp;
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
    for(int sign = 1; sign < 31; sign ++)
        signal(sign, posix_death_signal);
}
//--------------------------------------------------------------------------------

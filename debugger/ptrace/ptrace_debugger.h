/*
 * Author: Ali Gholami
 * Date: 2014-02-27
 */
#include <unistd.h>                                                                
#include <stdio.h>                                                                 
#include <stdlib.h>                                                               
#include <sys/wait.h>                                                              
#include <sys/ptrace.h>                                                            
#include <sys/types.h>                                                             
#include <sys/reg.h>                                                               
#include <signal.h>                                                                
#include <sys/user.h>

#ifdef __x86_64__
/* rax for 64 bits instructions */
#define ORIG_REG orig_rax 
#define REG rax
/* syscall names for x86_64 */
char *(syscall_names[314]) = {
  "read","write","open","close","stat","fstat","lstat","poll",
  "lseek","mmap","mprotect","munmap","brk","rt_sigaction","rt_sigprocmask",
  "rt_sigreturn","ioctl","pread","pwrite","readv","writev","access","pipe",
  "select","sched_yield","mremap","msync","mincore","madvise","shmget","shmat",
  "shmctl","dup","dup2","pause","nanosleep","getitimeyr","alarm","setitimer",
  "getpid","sendfile","socket","connect","accept","sendto","recvfrom","sendmsg",
  "recvmsg","shutdown","bind","listen","getsockname","getpeername","socketpair",
  "setsockopt","getsockopt","clone","fork","vfork","execve","_exit","wait4",
  "kill","uname","semget","semop","semctl","shmdt","msgget","msgsnd","msgrcv",
  "msgctl","fcntl","flock","fsync","fdatasync","truncate","ftruncate","getdents",
  "getcwd","chdir","fchdir","rename","mkdir","rmdir","creat","link","unlink",
  "symlink","readlink","chmod","fchmod","chown","fchown","lchown","umask",
  "gettimeofday","getrlimit","getrusage","sysinfo","times","ptrace","getuid",
  "syslog","getgid","setuid","setgid","geteuid","getegid","setpgid","getppid",
  "getpgrp","setsid","setreuid","setregid","getgroups","setgroups","setresuid",
  "getresuid","setresgid","getresgid","getpgid","setfsuid","setfsgid","getsid",
  "capget","capset","rt_sigpending","rt_sigtimedwait","rt_sigqueueinfo","rt_sigsuspend",
  "sigaltstack","utime","mknod","uselib","personality","ustat","statfs","fstatfs",
  "sysfs","getpriority","setpriority","sched_setparam","sched_getparam",
  "sched_setscheduler","sched_getscheduler","sched_get_priority_max",
  "sched_get_priority_min","sched_rr_get_interval","mlock","munlock","mlockall",
  "munlockall","vhangup","modify_ldt","pivot_root","_sysctl","prctl","arch_prctl",
  "adjtimex","setrlimit","chroot","sync","acct","settimeofday","mount","umount",
  "swapon","swapoff","reboot","sethostname","setdomainname","iopl","ioperm",
  "create_module","init_module","delete_module","get_kernel_syms","query_module",
  "quotactl","nfsservctl","getpmsg","putpmsg","afs_syscall","tuxcall","security",
  "gettid","readahead","setxattr","lsetxattr","fsetxattr","getxattr","lgetxattr",
  "fgetxattr","listxattr","llistxattr","flistxattr","removexattr","lremovexattr",
  "fremovexattr","tkill","time","futex","sched_setaffinity","sched_getaffinity",
  "set_thread_area","io_setup","io_destroy","io_getevents","io_submit","io_cancel",
  "get_thread_area","lookup_dcookie","epoll_create","epoll_ctl_old","epoll_wait_old",
  "remap_file_pages","getdents64","set_tid_address","restart_syscall","semtimedop",
  "fadvise64","timer_create","timer_settime","timer_gettime","timer_getoverrun",
  "timer_delete","clock_settime","clock_gettime","clock_getres","clock_nanosleep",
  "exit_group","epoll_wait","epoll_ctl","tgkill","utimes","vserver","mbind",
  "set_mempolicy","get_mempolicy","mq_open","mq_unlink","mq_timedsend","mq_timedreceive",
  "mq_notify","mq_getsetattr","kexec_load","waitid","add_key","request_key","keyctl",
  "ioprioset","ioprio_get","inotify_init","inotify_add_watch","inotify_rm_watch",
  "migrate_pages","openat","mkdirat","mknodat","fchownat","futimesat","newfstatat",
  "unlinkat","renameat","linkat","symlinkat","readlinkat","fchmodat","faccessat",
  "pselect6","ppoll","unshare","set_robust_list","get_robust_list","splice","tee",
  "sync_file_range","vmsplice","move_pages","utimensat","epoll_pwait","signalfd",
  "timerfd_create","eventfd","fallocate","timerfd_settime","timerfd_gettime","accept4",
  "signalfd4","eventfd2","epoll_create1","dup3","pipe2","inotify_init1","preadv",
  "pwritev","rt_tgsigqueueinfo","perf_event_open","recvmmsg","fanotify_init","fanotify_mark",
  "prlimit64","name_to_handle_at","open_by_handle_at","clock_adjtime","syncfs","sendmmsg",
  "setns","getcpu","process_vm_readv","process_vm_writev","kcmp","finit_module",
};
#endif

#ifdef __i386__
/* eax for 32 bits instructions*/
#define ORIG_REG orig_eax 
#define REG eax

/* syscall names for i386 */
char *(syscall_names[256]) = {
    "exit", "fork", "read", "write", "open", "close", "waitpid", "creat",
    "link", "unlink", "execve", "chdir", "time", "mknod", "chmod",
    "lchown", "break", "oldstat", "lseek", "getpid", "mount", "umount",
    "setuid", "getuid", "stime", "ptrace", "alarm", "oldfstat", "pause",
    "utime", "stty", "gtty", "access", "nice", "ftime", "sync", "kill",
    "rename", "mkdir", "rmdir", "dup", "pipe", "times", "prof", "brk",
    "setgid", "getgid", "signal", "geteuid", "getegid", "acct", "umount2",
    "lock", "ioctl", "fcntl", "mpx", "setpgid", "ulimit", "oldolduname",
    "umask", "chroot", "ustat", "dup2", "getppid", "getpgrp", "setsid",
    "sigaction", "sgetmask", "ssetmask", "setreuid", "setregid",
    "sigsuspend", "sigpending", "sethostname", "setrlimit", "getrlimit",
    "getrusage", "gettimeofday", "settimeofday", "getgroups", "setgroups",
    "select", "symlink", "oldlstat", "readlink", "uselib", "swapon",
    "reboot", "readdir", "mmap", "munmap", "truncate", "ftruncate",
    "fchmod", "fchown", "getpriority", "setpriority", "profil", "statfs",
    "fstatfs", "ioperm", "socketcall", "syslog", "setitimer", "getitimer",
    "stat", "lstat", "fstat", "olduname", "iopl", "vhangup", "idle",
    "vm86old", "wait4", "swapoff", "sysinfo", "ipc", "fsync", "sigreturn",
    "clone", "setdomainname", "uname", "modify_ldt", "adjtimex",
    "mprotect", "sigprocmask", "create_module", "init_module",
    "delete_module", "get_kernel_syms", "quotactl", "getpgid", "fchdir",
    "bdflush", "sysfs", "personality", "afs_syscall", "setfsuid",
    "setfsgid", "_llseek", "getdents", "_newselect", "flock", "msync",
    "readv", "writev", "getsid", "fdatasync", "_sysctl", "mlock",
    "munlock", "mlockall", "munlockall", "sched_setparam",
    "sched_getparam", "sched_setscheduler", "sched_getscheduler",
    "sched_yield", "sched_get_priority_max", "sched_get_priority_min",
    "sched_rr_get_interval", "nanosleep", "mremap", "setresuid",
    "getresuid", "vm86", "query_module", "poll", "nfsservctl",
    "setresgid", "getresgid", "prctl","rt_sigreturn","rt_sigaction",
    "rt_sigprocmask", "rt_sigpending", "rt_sigtimedwait",
    "rt_sigqueueinfo", "rt_sigsuspend", "pread", "pwrite", "chown",
    "getcwd", "capget", "capset", "sigaltstack", "sendfile", "getpmsg",
    "putpmsg", "vfork", "ugetrlimit", "mmap2", "truncate64",
    "ftruncate64", "stat64", "lstat64", "fstat64", "lchown32", "getuid32",
    "getgid32", "geteuid32", "getegid32", "setreuid32", "setregid32",
    "getgroups32", "setgroups32", "fchown32", "setresuid32",
    "getresuid32", "setresgid32", "getresgid32", "chown32", "setuid32",
    "setgid32", "setfsuid32", "setfsgid32", "pivot_root", "mincore",
    "madvise", "getdents64", "fcntl64", 0, "security", "gettid",
    "readahead", "setxattr", "lsetxattr", "fsetxattr", "getxattr",
    "lgetxattr", "fgetxattr", "listxattr", "llistxattr", "flistxattr",
    "removexattr", "lremovexattr", "fremovexattr", "tkill", "sendfile64",    
    "futex", "sched_setaffinity", "sched_getaffinity",
  };
#endif

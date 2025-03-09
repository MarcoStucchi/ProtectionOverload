// Require for compilation
#include <sys/stat.h>
#include <errno.h>

// void _exit(int status) { while (1) {} }
// int _close(int file) { return -1; }
// int _write(int file, char *ptr, int len) { return len; }
// int _lseek(int file, int ptr, int dir) { return 0; }
// int _read(int file, char *ptr, int len) { return 0; }
// int _fstat(int file, void *st) { return 0; }
// int _isatty(int file) { return 1; }
// int _kill(int pid, int sig) { errno = EINVAL; return -1; }
// int _getpid() { return 1; }




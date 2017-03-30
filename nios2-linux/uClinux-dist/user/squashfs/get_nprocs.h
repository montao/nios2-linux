#include <sys/sysinfo.h>
#include <unistd.h>
#define get_nprocs() sysconf(_SC_NPROCESSORS_CONF)

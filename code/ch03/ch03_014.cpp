#include <sys/resource.h>

size_t peak_memory_usage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;  // in kilobytes (macOS) or bytes (Linux)
}

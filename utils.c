#include "utils.h"
#include <stdio.h>
#include <sys/time.h>

double get_time_in_seconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec) + (tv.tv_usec / 1000000.0);
}

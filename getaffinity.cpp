#include <sched.h>
#include <unistd.h>

#include <iostream>

int main() {
    cpu_set_t mask;
    CPU_ZERO(&mask);
    sched_getaffinity(0, sizeof(mask), &mask);
    int cpu_count = CPU_COUNT(&mask);

    // -1 = before first range
    // 0 = not in range
    // 1 = range just started
    // 2 = long range
    int range_status = -1;

    for (int cpu = 0; cpu < CPU_SETSIZE; ++cpu) {
        if (CPU_ISSET(cpu, &mask)) {
            if (range_status <= 0) {
                if (range_status >= 0) {
                    std::cout << ",";
                }
                std::cout << cpu;
                range_status = 1;
            } else if (range_status == 1) {
                ++range_status;
            }
        } else if (range_status == 1) {
            range_status = 0;
        } else if (range_status == 2) {
            std::cout << "-" << cpu - 1;
            range_status = 0;
        }
    }
    std::cout << std::endl;

    return 0;
}

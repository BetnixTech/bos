#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    char line[256];

    printf("CPU Info:\n");
    fp = fopen("/proc/cpuinfo", "r");
    if (fp) {
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, "model name") != NULL)
                printf("%s", line);
        }
        fclose(fp);
    }

    printf("\nMemory Info:\n");
    fp = fopen("/proc/meminfo", "r");
    if (fp) {
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, "MemTotal") != NULL || strstr(line, "MemAvailable") != NULL)
                printf("%s", line);
        }
        fclose(fp);
    }

    return 0;
}

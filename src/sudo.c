#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "config.h"
#include "sudo.h"

int check_valid_args(int argc, const char **argv) {
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-u") == 0) {
            return -1;
        }
    }
    return 0;
}

int run_sudo(int argc, const char **argv, uid_t uid, const char *sudo) {
    char **real_argv = (char **) malloc(sizeof(const char *) * (argc + 3));
    if (!real_argv) {
        errno = ENOMEM;
        return -1;
    }
    real_argv[0] = (char *) sudo;
    real_argv[1] = "-u";
    char buf[sizeof("#XXXXXXXX")];
    sprintf(buf, "#%d", uid);
    real_argv[2] = buf;
    for (int i = 1; i < argc; ++i) {
        real_argv[i + 2] = (char *) argv[i];
    }
    real_argv[argc + 2] = NULL;
    if (execv(sudo, real_argv) < 0) {
        int e = errno;
        free(real_argv);
        errno = e;
        return -1;
    }
    return -1;
}

int find_sudo(sudo_list_t *list, char *sudo, int sudo_len) {
    struct stat s;
    for (sudo_list_t *it = list; it; it = it->next) {
        if (stat(it->sudo_path, &s) < 0) {
            if (errno != ENOENT) {
                return -1;
            }
        } else {
            strncpy(sudo, it->sudo_path, sudo_len - 1);
            sudo[sudo_len - 1] = 0;
            return strlen(sudo);
        }
    }
    errno = ENOENT;
    return -1;
}

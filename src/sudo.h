#ifndef SUDONT_SUDO_H
#define SUDONT_SUDO_H

#include <sys/types.h>

#include "config.h"

int check_valid_args(int argc, const char **argv);
int run_sudo(int argc, const char **argv, uid_t uid, const char *sudo);
int find_sudo(sudo_list_t *list, char *sudo, int sudo_len);

#endif

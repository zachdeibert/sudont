#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "policy.h"
#include "config.h"

#ifndef STR
#define _STR(x) # x
#define STR(x) _STR(x)
#endif

int parse_config(config_t *config) {
    memset(config, 0, sizeof(config_t));
    FILE *file = fopen(DEFAULT_CONFIG_FILE, "r");
    int err = EIO;
    if (!file) {
        if (0) {
            io_error:
            fclose(file);
            config_free(*config);
        }
        errno = err;
        return -1;
    }
    char line[MAX_LINE_LENGTH + 1];
    const char *argv[MAX_ARGS];
    while (!feof(file)) {
        fscanf(file, "%" STR(MAX_LINE_LENGTH) "[^\n]\n", line);
        if (strnlen(line, MAX_LINE_LENGTH + 1) >= MAX_LINE_LENGTH) {
            fputs("Lines in config file are too long.\n", stderr);
            goto io_error;
        }
        int argc = 0;
        int is_reading_arg = 0;
        for (char *it = line; *it; ++it) {
            switch (*it) {
                case ' ':
                case '\t':
                case '\r':
                    if (is_reading_arg) {
                        is_reading_arg = 0;
                        *it = 0;
                    }
                    break;
                default:
                    if (!is_reading_arg) {
                        is_reading_arg = 1;
                        if (argc >= MAX_LINE_LENGTH) {
                            fputs("Line in config file has too many arguments.\n", stderr);
                            goto io_error;
                        }
                        argv[argc++] = it;
                    }
                    break;
            }
        }
        if (strcmp(*argv, "sudo") == 0) {
            if (argc == 2) {
                sudo_list_t *node = (sudo_list_t *) malloc(sizeof(sudo_list_t));
                if (!node) {
                    err = ENOMEM;
                    goto io_error;
                }
                node->next = config->sudo;
                char *buf = (char *) malloc(strlen(argv[1]) + 1);
                if (!buf) {
                    free(node);
                    err = ENOMEM;
                    goto io_error;
                }
                strcpy(buf, argv[1]);
                node->sudo_path = buf;
                config->sudo = node;
            } else {
                fputs("'sudo' directive takes exactly one argument\n", stderr);
                goto io_error;
            }
        } else {
            policy_list_t *node = (policy_list_t *) malloc(sizeof(policy_list_t));
            if (!node) {
                err = ENOMEM;
                goto io_error;
            }
            node->next = config->policies;
            if (policy_parse(&node->policy, argc, argv) < 0) {
                err = errno;
                goto io_error;
            }
            config->policies = node;
        }
    }
    fclose(file);
    return 0;
}

void config_free(config_t config) {
    for (sudo_list_t *it = config.sudo; it; ) {
        sudo_list_t *tmp = it->next;
        free(it);
        it = tmp;
    }
    for (policy_list_t *it = config.policies; it; ) {
        policy_list_t *tmp = it->next;
        free(it);
        it = tmp;
    }
}

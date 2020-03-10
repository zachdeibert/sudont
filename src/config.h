#ifndef SUDONT_CONFIG_H
#define SUDONT_CONFIG_H

#include "policy.h"

#define DEFAULT_CONFIG_FILE "/etc/sudonters"

struct _sudo_list_t;
typedef struct _sudo_list_t sudo_list_t;
struct _sudo_list_t {
    const char *sudo_path;
    sudo_list_t *next;
};

struct _policy_list_t;
typedef struct _policy_list_t policy_list_t;
struct _policy_list_t {
    policy_t policy;
    policy_list_t *next;
};

typedef struct {
    sudo_list_t *sudo;
    policy_list_t *policies;
} config_t;

int parse_config(config_t *config);
void config_free(config_t config);

#endif

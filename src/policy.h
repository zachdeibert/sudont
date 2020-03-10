#ifndef SUDONT_POLICY_H
#define SUDONT_POLICY_H

#include "proc-tree.h"

typedef enum {
    pr_undefined = 0,
    pr_deny,
    pr_allow,
    pr_error
} policy_result_t;

typedef void (*policy_action_t)(void *context, proc_tree_t *ps, policy_result_t *result);

typedef struct {
    policy_action_t action;
    void *context;
} policy_t;

int policy_parse(policy_t *policy, int argc, const char **argv);
void policy_eval(policy_t policy, proc_tree_t *ps, policy_result_t *result);

#ifndef CONCAT
#define _CONCAT(a, b) a ## b
#define CONCAT(a, b) _CONCAT(a, b)
#endif
#define POLICY_PROTO(name) \
void name(void *context, proc_tree_t *ps, policy_result_t *result); \
void *CONCAT(name, _init)(int argc, const char **argv)

POLICY_PROTO(policy_all);
POLICY_PROTO(policy_by_user);
POLICY_PROTO(policy_interactive);

#endif

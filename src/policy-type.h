#ifndef SUDONT_POLICY_TYPE_H
#define SUDONT_POLICY_TYPE_H

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

#endif

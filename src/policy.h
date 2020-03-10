#ifndef SUDONT_POLICY_H
#define SUDONT_POLICY_H

#include "proc-tree.h"
#include "policy-type.h"
#include "config.h"

int policy_parse(policy_t *policy, int argc, const char **argv, sudo_list_t *sudo);
void policy_eval(policy_t policy, proc_tree_t *ps, policy_result_t *result);
void policy_free(policy_t policy);

#ifndef CONCAT
#define _CONCAT(a, b) a ## b
#define CONCAT(a, b) _CONCAT(a, b)
#endif
#define POLICY_PROTO(name) \
void name(void *context, proc_tree_t *ps, policy_result_t *result); \
void *CONCAT(name, _init)(int argc, const char **argv, sudo_list_t *sudo)

POLICY_PROTO(policy_all);
POLICY_PROTO(policy_by_user);
POLICY_PROTO(policy_interactive);

#endif

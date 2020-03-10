#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "proc-tree.h"
#include "policy.h"

int policy_parse(policy_t *policy, int argc, const char **argv) {
#define POLICY_TYPE(argv0, name) \
    do { \
        if (strcmp(argv[0], argv0) == 0) { \
            policy->action = name; \
            policy->context = CONCAT(name, _init)(argc, argv); \
            return 0; \
        } \
    } while (0)
    POLICY_TYPE("allow", policy_all);
    POLICY_TYPE("deny", policy_all);
    POLICY_TYPE("allow_user", policy_by_user);
    POLICY_TYPE("deny_user", policy_by_user);
    POLICY_TYPE("allow_interactive", policy_interactive);
    POLICY_TYPE("deny_interactive", policy_interactive);
    errno = EINVAL;
    return -1;
}

void policy_eval(policy_t policy, proc_tree_t *ps, policy_result_t *result) {
    policy.action(policy.context, ps, result);
}

void policy_free(policy_t policy) {
    if (policy.context) {
        free(policy.context);
    }
}

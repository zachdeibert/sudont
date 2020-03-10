#include <stdlib.h>
#include <string.h>
#include "../proc-tree.h"
#include "../policy.h"

typedef struct {
    policy_result_t res;
} context_t;

void policy_all(void *context, proc_tree_t *ps, policy_result_t *result) {
    if (context == NULL) {
        *result = pr_error;
        return;
    }
    context_t *ctx = (context_t *) context;
    *result = ctx->res;
}

void *policy_all_init(int argc, const char **argv, sudo_list_t *sudo) {
    if (argc != 1) {
        return NULL;
    }
    context_t *ctx = (context_t *) malloc(sizeof(context_t));
    if (ctx == NULL) {
        return NULL;
    }
    if (strcmp("deny", *argv) == 0) {
        ctx->res = pr_deny;
    } else if (strcmp("allow", *argv) == 0) {
        ctx->res = pr_allow;
    } else {
        ctx->res = pr_error;
    }
    return ctx;
}

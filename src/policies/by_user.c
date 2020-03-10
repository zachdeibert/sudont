#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include "../proc-tree.h"
#include "../policy.h"

typedef struct {
    policy_result_t res;
    uid_t uid;
} context_t;

void policy_by_user(void *context, proc_tree_t *ps, policy_result_t *result) {
    if (context == NULL) {
        *result = pr_error;
        return;
    }
    context_t *ctx = (context_t *) context;
    while (ps->parent) {
        ps = ps->parent;
    }
    if (ps->uid == ctx->uid) {
        *result = ctx->res;
    }
}

void *policy_by_user_init(int argc, const char **argv, sudo_list_t *sudo) {
    if (argc != 2) {
        return NULL;
    }
    context_t *ctx = (context_t *) malloc(sizeof(context_t));
    if (ctx == NULL) {
        return NULL;
    }
    if (strcmp("deny_user", *argv) == 0) {
        ctx->res = pr_deny;
    } else if (strcmp("allow_user", *argv) == 0) {
        ctx->res = pr_allow;
    } else {
        ctx->res = pr_error;
    }
    struct passwd *pwd = getpwnam(argv[1]);
    if (pwd) {
        ctx->uid = pwd->pw_uid;
    } else {
        free(ctx);
        return NULL;
    }
    return ctx;
}

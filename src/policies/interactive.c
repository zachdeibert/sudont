#include <stdlib.h>
#include <string.h>
#include "../proc-tree.h"
#include "../policy.h"

typedef struct {
    policy_result_t res;
    int num_sudos;
    const char *sudos[0];
} context_t;

void policy_interactive(void *context, proc_tree_t *ps, policy_result_t *result) {
    if (context == NULL || ps == NULL || ps->parent == NULL) {
        *result = pr_error;
        return;
    }
    context_t *ctx = (context_t *) context;
    for (int i = 0; i < ctx->num_sudos; ++i) {
        if (strcmp(ctx->sudos[i], ps->parent->filename) == 0) {
            *result = ctx->res;
            return;
        }
    }
}

void *policy_interactive_init(int argc, const char **argv, sudo_list_t *sudo) {
    if (argc != 1) {
        return NULL;
    }
    int num_sudos = 0;
    int sudo_path_chars = 0;
    for (sudo_list_t *it = sudo; it; it = it->next) {
        ++num_sudos;
        sudo_path_chars += strlen(it->sudo_path) + 1;
    }
    context_t *ctx = (context_t *) malloc(sizeof(context_t) + sizeof(const char *) * num_sudos + sizeof(char) * sudo_path_chars);
    if (ctx == NULL) {
        return NULL;
    }
    if (strcmp("deny_interactive", *argv) == 0) {
        ctx->res = pr_deny;
    } else if (strcmp("allow_interactive", *argv) == 0) {
        ctx->res = pr_allow;
    } else {
        ctx->res = pr_error;
    }
    ctx->num_sudos = num_sudos;
    const char **sudos = ctx->sudos;
    char *char_buf = ctx + sizeof(context_t) + sizeof(const char *) * num_sudos;
    for (sudo_list_t *it = sudo; it; it = it->next) {
        strcpy(char_buf, it->sudo_path);
        *sudos++ = char_buf;
        char_buf += strlen(it->sudo_path) + 1;
    }
    return ctx;
}

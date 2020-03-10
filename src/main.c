#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <unistd.h>
#include "config.h"
#include "proc-tree.h"
#include "proc.h"
#include "policy.h"
#include "sudo.h"

int main(int argc, const char **argv) {
    if (setuid(0) < 0) {
        if (errno == EPERM) {
            fprintf(stderr, "Executable %s needs the setuid bit to work properly\n", *argv);
        } else {
            perror("setuid");
        }
        goto exit;
    }
    config_t config;
    if (parse_config(&config) < 0) {
        perror("parse_config");
        goto exit;
    }
    proc_tree_t *tree;
    if (ps_tree(&tree, config.sudo) < 0) {
        if (errno == ESRCH) {
            fprintf(stderr, "%s can only be run after 'sudo' has been run.\n", *argv);
        } else {
            perror("ps_tree");
        }
        goto exit;
    }
    policy_result_t res = pr_undefined;
    for (policy_list_t *it = config.policies; it; it = it->next) {
        policy_eval(it->policy, tree, &res);
        if (res == pr_error) {
            perror("policy_eval");
            goto free_ps;
        }
    }
    switch (res) {
        case pr_undefined:
            fputs("Error in policy script\n", stderr);
            break;
        case pr_deny:
            fputs("Access denied.\n", stderr);
            break;
        case pr_allow:
            if (check_valid_args(argc, argv) < 0) {
                fputs("sudont: Unrecognized argument -u\n", stderr);
                goto free_ps;
            }
            uid_t uid;
            for (proc_tree_t *it = tree; it; it = it->parent) {
                uid = it->uid;
            }
            ps_tree_free(tree);
            char sudo[PATH_MAX];
            if (find_sudo(config.sudo, sudo, PATH_MAX) < 0) {
                perror("find_sudo");
                goto free_config;
            }
            config_free(config);
            if (run_sudo(argc, argv, uid, sudo) < 0) {
                perror("run_sudo");
                goto exit;
            }
            break;
    }
    free_ps:
    ps_tree_free(tree);
    free_config:
    config_free(config);
    exit:
    return EXIT_FAILURE;
}

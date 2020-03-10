#ifndef SUDONT_PROC_TREE_H
#define SUDONT_PROC_TREE_H

#include <sys/types.h>

struct _proc_tree_t;
typedef struct _proc_tree_t proc_tree_t;
struct _proc_tree_t {
    const char *filename;
    uid_t uid;
    gid_t gid;
    pid_t pid;
    proc_tree_t *parent;
};

#endif

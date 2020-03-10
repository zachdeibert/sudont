#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "proc-tree.h"
#include "config.h"
#include "proc.h"

int ps_scan(pid_t pid, proc_tree_t **tree, sudo_list_t *sudo);

int ps_tree(proc_tree_t **tree, sudo_list_t *sudo) {
    return ps_scan(getpid(), tree, sudo);
}

void ps_tree_free(proc_tree_t *tree) {
    while (tree) {
        proc_tree_t *tmp = tree->parent;
        free((char *) tree->filename);
        free(tree);
        tree = tmp;
    }
}

int ps_scan(pid_t pid, proc_tree_t **tree, sudo_list_t *sudo) {
    *tree = (proc_tree_t *) malloc(sizeof(proc_tree_t));
    if (!*tree) {
        errno = ENOMEM;
        return -1;
    }
    (*tree)->pid = pid;
    char filename[sizeof("/proc/XXXXXX/stat")];
    sprintf(filename, "/proc/%d/exe", pid);
    char buf[PATH_MAX + 1];
    int r = readlink(filename, buf, sizeof(buf));
    if (r < 0) {
        return -1;
    } else if (r >= sizeof(buf) - 1) {
        free(*tree);
        errno = ENOBUFS;
        return -1;
    }
    char *tmp = (char *) malloc(r + 1);
    if (!tmp) {
        free(*tree);
        errno = ENOMEM;
        return -1;
    }
    memcpy(tmp, buf, r);
    tmp[r] = 0;
    (*tree)->filename = tmp;
    sprintf(filename, "/proc/%d/stat", pid);
    FILE *file = fopen(filename, "r");
    if (!file) {
        free(tmp);
        free(*tree);
        errno = EIO;
        return -1;
    }
    struct stat s;
    memset(&s, 0, sizeof(struct stat));
    if (fstat(fileno(file), &s) < 0) {
        int e = errno;
        fclose(file);
        free(tmp);
        free(*tree);
        errno = e;
        return -1;
    }
    (*tree)->uid = s.st_uid;
    (*tree)->gid = s.st_gid;
    pid_t ppid;
    if (fscanf(file, "%*d %*s %*c %d", &ppid) != 1) {
        fclose(file);
        free(tmp);
        free(*tree);
        errno = EIO;
        return -1;
    }
    fclose(file);
    if (ppid == 0) {
        free(tmp);
        free(*tree);
        errno = ESRCH;
        return -1;
    }
    for (sudo_list_t *it = sudo; it; it = it->next) {
        if (strcmp(it->sudo_path, (*tree)->filename) == 0) {
            (*tree)->parent = NULL;
            return 0;
        }
    }
    if (ps_scan(ppid, &(*tree)->parent, sudo) < 0) {
        int e = errno;
        free(tmp);
        free(*tree);
        errno = e;
        return -1;
    }
    return 0;
}

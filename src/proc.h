#ifndef SUDONT_PROC_H
#define SUDONT_PROC_H

#include "proc-tree.h"
#include "config.h"

int ps_tree(proc_tree_t **tree, sudo_list_t *sudo);
void ps_tree_free(proc_tree_t *tree);

#endif

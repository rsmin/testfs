/*
  RCS:          $Header: /afs/cs.cmu.edu/user/tmwong/Cvs/fscachesim/top-down-size-splay.h,v 1.1.1.1 2000/09/21 16:25:41 tmwong Exp $
  Description:  An implementation of top-down splaying with sizes
  Author:       D. Sleator <sleator@cs.cmu.edu>
*/

#ifndef _TOP_DOWN_SIZE_SPLAY_H_
#define _TOP_DOWN_SIZE_SPLAY_H_

typedef struct tree_node Tree;
struct tree_node {
    Tree * left, * right;
    int key;
    int size;   /* maintained to be the number of nodes rooted here */
};

Tree * Splay_splay (int i, Tree *t);

Tree * Splay_insert(int i, Tree *t);

Tree * Splay_delete(int i, Tree *t);

Tree * Splay_find_rank(int r, Tree *t);

void Splay_printtree(Tree * t, int d);

#define node_size(x) (((x)==NULL) ? 0 : ((x)->size))
/* This macro returns the size of a node.  Unlike "x->size",     */
/* it works even if x=NULL.  The test could be avoided by using  */
/* a special version of NULL which was a real node with size 0.  */
 
#endif /* _TOP_DOWN_SIZE_SPLAY_H_ */

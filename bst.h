#ifndef BST_H
#define BST_H

/* 이진 탐색 트리(BST)의 노드 (연결 자료구조) */
typedef struct Node
{
    int data;
    struct Node *left;
    struct Node *right;
} Node;

/*
 * value를 BST에 삽입하고 루트를 반환한다 (트리가 비어 있으면 새 노드가 루트).
 * 기존 노드의 값과 비교할 때마다 *cmp를 1 증가시킨다.
 */
Node *bst_insert(Node *root, int value, long *cmp);

/*
 * BST에서 key를 탐색한다. 찾으면 1, 못 찾으면 0을 반환한다.
 * *cmp에는 방문한 노드 수(= 비교 횟수)가 저장된다.
 */
int bst_search(const Node *root, int key, int *cmp);

/* 트리의 높이를 레벨 수로 반환한다 (빈 트리 = 0, 루트만 있으면 1) */
int bst_height(const Node *root);

/* 트리 전체의 메모리를 해제한다 */
void bst_free(Node *root);

#endif

#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

/* 새 노드를 동적 할당하고 초기화한다 */
static Node *create_node(int value)
{
    Node *n = (Node *)malloc(sizeof(Node));
    if (n == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    n->data = value;
    n->left = NULL;
    n->right = NULL;
    return n;
}

Node *bst_insert(Node *root, int value, long *cmp)
{
    Node *new_node = create_node(value);
    if (root == NULL)
        return new_node; /* 빈 트리: 비교 없이 루트가 된다 */

    Node *cur = root;
    for (;;)
    {
        (*cmp)++; /* value와 cur->data를 비교: 노드 1개 방문 = 비교 1회 */
        if (value < cur->data)
        {
            if (cur->left == NULL)
            {
                cur->left = new_node;
                break;
            }
            cur = cur->left;
        }
        else
        { /* value > cur->data (과제의 데이터는 서로 다른 값이다) */
            if (cur->right == NULL)
            {
                cur->right = new_node;
                break;
            }
            cur = cur->right;
        }
    }
    return root;
}

int bst_search(const Node *root, int key, int *cmp)
{
    const Node *cur = root;
    *cmp = 0;
    while (cur != NULL)
    {
        (*cmp)++; /* 노드 1개 방문 = 비교 1회 */
        if (key == cur->data)
            return 1; /* 탐색 성공 */
        cur = (key < cur->data) ? cur->left : cur->right;
    }
    return 0; /* 더 이동할 노드가 없으면 탐색 실패 */
}

int bst_height(const Node *root)
{
    if (root == NULL)
        return 0;
    int hl = bst_height(root->left);
    int hr = bst_height(root->right);
    return 1 + (hl > hr ? hl : hr);
}

void bst_free(Node *root)
{
    if (root == NULL)
        return;
    /* 자식을 먼저 해제한 뒤 자기 자신을 해제한다 (후위 순회) */
    bst_free(root->left);
    bst_free(root->right);
    free(root);
}

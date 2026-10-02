#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "search.h"

#define DATA_COUNT 100
#define KEY_COUNT  50
#define MAX_VALUE  1000

static int rand_range(int lo, int hi)
{
    return lo + rand() % (hi - lo + 1);
}

/* [0, MAX_VALUE] 범위에서 서로 다른 정수 n개를 발생 순서 그대로 생성한다 */
static void generate_distinct(int arr[], int n)
{
    int used[MAX_VALUE + 1] = {0};
    int count = 0;
    while (count < n) {
        int v = rand_range(0, MAX_VALUE);
        if (used[v])
            continue; /* 이미 나온 값이면 버리고 새로 생성한다 */
        used[v] = 1;
        arr[count++] = v;
    }
}

/* 정수를 천 단위 쉼표가 있는 문자열로 변환한다 (예: 3421 -> "3,421") */
static void format_number(long value, char *out, size_t size)
{
    char raw[32];
    snprintf(raw, sizeof(raw), "%ld", value);

    int start = (raw[0] == '-') ? 1 : 0;
    int len = (int)strlen(raw);
    size_t j = 0;

    if (start)
        out[j++] = '-';
    for (int i = start; i < len && j + 2 < size; i++) {
        if (i > start && (len - i) % 3 == 0)
            out[j++] = ','; /* 오른쪽부터 세 자리마다 쉼표 */
        out[j++] = raw[i];
    }
    out[j] = '\0';
}

int main(int argc, char *argv[])
{
    unsigned seed = (argc > 1) ? (unsigned)strtoul(argv[1], NULL, 10) : 2025u;
    srand(seed);

    int data[DATA_COUNT];
    int keys[KEY_COUNT];

    /* 1. 데이터를 생성하여 배열에 저장한다 */
    generate_distinct(data, DATA_COUNT);

    /* 2. 같은 데이터를 순서대로 BST에 삽입하며 생성 비용(비교 횟수)을 센다 */
    Node *root = NULL;
    long build_cmp = 0;
    for (int i = 0; i < DATA_COUNT; i++)
        root = bst_insert(root, data[i], &build_cmp);

    /* 3. 탐색 대상 50개 생성 (서로 같을 수 있고, 데이터에 없을 수도 있다) */
    for (int i = 0; i < KEY_COUNT; i++)
        keys[i] = rand_range(0, MAX_VALUE);

    printf("=== Generated %d distinct integers (insertion order) ===\n", DATA_COUNT);
    for (int i = 0; i < DATA_COUNT; i++)
        printf("%4d%s", data[i], (i % 10 == 9) ? "\n" : " ");

    printf("\nBST build comparisons : %ld\n", build_cmp);
    printf("BST height (levels)   : %d\n\n", bst_height(root)); /* 구조 분석용 추가 통계 */

    /* 4. 같은 key로 두 가지 탐색을 각각 수행하고 결과를 출력한다 */
    printf("=== %d search keys ===\n\n", KEY_COUNT);

    long seq_total = 0, bst_total = 0;

    for (int i = 0; i < KEY_COUNT; i++) {
        int seq_cmp, bst_cmp;
        int seq_found = sequential_search(data, DATA_COUNT, keys[i], &seq_cmp);
        int bst_found = bst_search(root, keys[i], &bst_cmp);

        if (seq_found != bst_found) { /* 두 탐색의 성공/실패 결과는 반드시 같아야 한다 */
            fprintf(stderr, "Inconsistent result for key %d\n", keys[i]);
            bst_free(root);
            return EXIT_FAILURE;
        }

        printf("Search Key : %d\n\n", keys[i]);
        printf("Sequential Search\n");
        printf("Result      : %s\n", seq_found ? "Found" : "Not Found");
        printf("Comparisons : %d\n\n", seq_cmp);
        printf("BST Search\n");
        printf("Result      : %s\n", bst_found ? "Found" : "Not Found");
        printf("Comparisons : %d\n\n", bst_cmp);

        seq_total += seq_cmp;
        bst_total += bst_cmp;
    }

    double seq_avg = (double)seq_total / KEY_COUNT;
    double bst_avg = (double)bst_total / KEY_COUNT;

    /* 5. 총 비교 횟수와 평균 비교 횟수 출력 (과제 요구 항목만 출력) */
    char seq_total_str[32], bst_total_str[32];
    format_number(seq_total, seq_total_str, sizeof(seq_total_str));
    format_number(bst_total, bst_total_str, sizeof(bst_total_str));

    printf("Number of searches: %d\n\n", KEY_COUNT);

    printf("Sequential Search\n");
    printf("Total comparisons   : %s\n", seq_total_str);
    printf("Average comparisons : %.2f\n\n", seq_avg);

    printf("BST Search\n");
    printf("Total comparisons   : %s\n", bst_total_str);
    printf("Average comparisons : %.2f\n\n", bst_avg);

    bst_free(root);
    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#define SIZE 9 // 數獨大小

int sudoku[SIZE][SIZE] = {
    {5, 8, 7, 1, 3, 6, 9, 2, 4},
    {9, 2, 4, 7, 5, 8, 1, 6, 3},
    {3, 2, 6, 9, 2, 4, 8, 5, 7},
    {2, 4, 3, 6, 1, 9, 7, 8, 5},
    {7, 9, 8, 4, 7, 5, 3, 1, 2},
    {6, 5, 1, 8, 2, 3, 4, 9, 6},
    {1, 6, 2, 3, 8, 7, 5, 4, 9},
    {8, 7, 9, 5, 4, 1, 6, 3, 8},
    {4, 3, 5, 2, 6, 9, 2, 7, 1}
};

void *checkRow(void *arg) {
    int row = *((int *)arg);
    free(arg);
    int check[SIZE] = {0};
    for (int i = 0; i < SIZE; i++) {
        int num = sudoku[row][i];
        if (num < 1 || num > SIZE || check[num - 1]) {
            pthread_exit((void *)0);
        }
        check[num - 1] = 1;
    }
    pthread_exit((void *)1);
}

void *checkColumn(void *arg) {
    int col = *((int *)arg);
    free(arg);
    int check[SIZE] = {0};
    for (int i = 0; i < SIZE; i++) {
        int num = sudoku[i][col];
        if (num < 1 || num > SIZE || check[num - 1]) {
            pthread_exit((void *)0);
        }
        check[num - 1] = 1;
    }
    pthread_exit((void *)1);
}

void *checkSubgrid(void *arg) {
    int startRow = *((int *)arg);
    int startCol = *((int *)(arg + sizeof(int)));
    free(arg);
    int check[SIZE] = {0};
    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > SIZE || check[num - 1]) {
                pthread_exit((void *)0);
            }
            check[num - 1] = 1;
        }
    }
    pthread_exit((void *)1);
}

int main() {
    pthread_t threads[SIZE];
    int *arg;
    void *result_column, *result_row, *result_subgrid;
    // 检查行
    for (int i = 0; i < SIZE; i++) {
        arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&threads[i], NULL, checkRow, arg);
    }
    // 等待行检查完成
    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads[i], &result_row);
        if ((intptr_t)result_row == 0) {
            printf("Invalid solution in row %d\n", i);
            break;
            // return 0; // 退出程序，不再继续检查
        }
    }

    // 检查列
    for (int i = 0; i < SIZE; i++) {
        arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&threads[i], NULL, checkColumn, arg);
    }
    // 等待列检查完成
    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads[i], &result_column);
        if ((intptr_t)result_column == 0) {
            printf("Invalid solution in column %d\n", i);
            break;
            // return 0; // 退出程序
        }
    }
    int indices[SIZE][2] = {{0, 0}, {0, 3}, {0, 6}, {3, 0}, {3, 3}, {3, 6}, {6, 0}, {6, 3}, {6, 6}};

    // 检查子网格
    for (int i = 0; i < SIZE; i++) {
        arg = malloc(2 * sizeof(int));
        arg[0] = indices[i][0];
        arg[1] = indices[i][1];
        pthread_create(&threads[i], NULL, checkSubgrid, arg);
    }
    // 等待子网格检查完成
    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads[i], &result_subgrid);
        if ((intptr_t)result_subgrid == 0) {
            printf("Invalid solution in subgrid %d starting at (%d, %d)\n", i, indices[i][0], indices[i][1]);
            return 0; // 退出程序
        }
    }
    if((intptr_t)result_subgrid && (intptr_t)result_column && (intptr_t)result_row)
    {
        printf("Valid Sudoku solution\n");

    }
    return 0;
}
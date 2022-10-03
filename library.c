#include "library.h"
#include <stdio.h>
#include <stdlib.h>

double** getMatrix(int n, int m){
    double t;
    double **arr = malloc(sizeof(int**) * n);
    for(int i = 0; i < n; i++){
        arr[i] = malloc(sizeof(int*) * m);
    }
    printf("Enter matrix: \n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%lf", &t);
            arr[i][j] = t;
        }
    }
    return arr;
}

void printMatrix(double** arr, int n, int m){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%lf\t", arr[i][j]);
        }
        printf("\n");
    }
}

double** plus(double ** arr1, double ** arr2, int n1, int m1, int n2, int m2){
    if (n1 != n2 || m1 != m2){
        return NULL;
    }
    int n = n1;
    int m = m2;
    double **arr = malloc(sizeof(int**) * n1);
    for(int i = 0; i < n; i++){
        arr[i] = malloc(sizeof(int*) * m2);
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            arr[i][j] = arr1[i][j] + arr2[i][j];
        }
    }
    return arr;
}

double** mulNum(double ** arr1, int n, int m, int a){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            arr1[i][j] = a * arr1[i][j];
        }
    }
    return arr1;
}

double** mulMatrix(double** arr1, double** arr2, int n1, int m1, int n2, int m2){
    if (m1 != n2) {
        printf("Wrong size!\n");
        return NULL;
    }
    double** arr3 = malloc(sizeof(double**) * n1);
    for(int i = 0; i < n1; i++){
        arr3[i] = malloc(sizeof(double*) * m2);
    }
    double a;
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++) {
            a = 0;
            for(int k = 0; k < m1; k++){
                a += arr1[i][k]*arr2[k][j];
            }
            arr3[i][j] = a;
        }
    }
    return arr3;
}

double determinant(double** arr, int width, int height){
    if (width != height){
        printf("Wrong size");
        return 0;
    }
    double arrTmp[width][height];
    double temp;
    double det = 1, arT;
    int count = 0;
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            arrTmp[i][j] = arr[i][j];
        }
    }
    int j;
    for (int i = 0; i < width; i++){
        j = i;
        while (arrTmp[j][i] == 0.0){
            j++;
            if (j == height){
                return 0;
            }
        }
        if (i != j) {
            for (int k = 0; k < width; k++) {
                temp = arrTmp[i][k];
                arrTmp[i][k] = arrTmp[j][k];
                arrTmp[j][k] = temp;
            }
            count++;
        }
        for (int m = i + 1; m < height; m++){
            arT = arrTmp[m][i];
            for (int n = 0; n < width; n++){
                arrTmp[m][n] = arrTmp[m][n] - ( (arrTmp[i][n] / arrTmp[i][i]) * arT );
            }
        }
    }

    for (int l = 0; l < width; l++) {
        det *= arrTmp[l][l];
    }
    if (count % 2 != 0){
        det *= -1;
    }
    return det;
}
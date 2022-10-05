#include <stdio.h>
#include "library.h"

int main() {
    double** arr1;
    double** arr2;
    double** arr3; // сложение двух матриц
    double** arr4; //умножение на число
    double** arr5; //умножение матриц
    double det;//подсчет определителя
    int n1, m1, n2, m2, a;
    printf("Enter size of the first matrix for input: \n");
    scanf("%d %d", &n1, &m1);
    arr1 = getMatrix(n1, m1);
    printf("Enter size of the second matrix for input: \n");
    scanf("%d %d", &n2, &m2);
    arr2 = getMatrix(n2, m2);
    arr3 = plus(arr1, arr2, n1, m1, n2, m2);
    printf("Enter number to multiply Matrix2\n");
    scanf("%d", &a);
    arr4 = mulNum(arr2, n2, m2, a);
    arr5 = mulMatrix(arr1, arr2, n1, m1, n2, m2);
    det = determinant(arr1, n1, m1);
    printf("First matrix\n");
    printMatrix(arr1, n1, m1);
    printf("\n");
    printf("Second matrix\n");
    printMatrix(arr2, n2, m2);
    printf("\n");
    if(arr3 != NULL){
    printf("Matrix1 + Matrix2:\n");
    printMatrix(arr3, n1, m1);
    printf("\n");
    }
    if(arr4 != NULL){
    printf("Matrix1 * int:\n");
    printf("\n");
    printMatrix(arr4, n2, m2);
    printf("\n");
    }
    if(arr5 != NULL){
    printf("Matrix1 * Matrix2: \n");
    printMatrix(arr5, m1, n2);
    printf("\n");
    }
    printf("Determinant of the first matrix is: \n");
    printf("%lf", det);
    return 0;
}

#ifndef PRAC2_LIBRARY_H
#define PRAC2_LIBRARY_H

double** getMatrix(int n, int m);//по заданным размерам вводит матрицу построчно(n - число строк, m - столбцов)

void printMatrix(double** arr, int n, int m);//по размерам матрицы выводит ее(n - число строк, m - столбцов)

double** plus(double ** arr1, double ** arr2, int n1, int m1, int n2, int m2);//суммирует две матрицы с одинаковыми размерами

double** mulNum(double ** arr1, int n, int m, int a);//а - число, на которое умножаем arr1 - матрицу

double** mulMatrix(double** arr1, double** arr2, int n1, int m1, int n2, int m2);//перемножает матрицы arr1 и arr2 соответствующего размера, иначе,
                                                                                 //если размеры матриц неверные, возвращает null

double determinant(double** arr, int width, int height);//считает определитель квадратной матрицы по методу Гаусса, если матрица не квадратная, возвращает 0
//и сообщение об ошибке

#endif //PRAC2_LIBRARY_H
#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#define M 10



char* get_s(void){

    unsigned int n;

    int k = 0;

    char *s = malloc(M);

    if (!s) {

        free(s);

        return NULL;

    }

    while(fgets(s + k, M, stdin)){

        n = strlen(s);

        if(s[n-1] != '\n'){

            k = k + M - 1;

            s = realloc(s, k + M);

            if (!s) {

                free(s);

                return NULL;

            }

        }

        else {

            s[n-1] = '\0';

            return s;

        }

    }

    free(s);

    return NULL;

}



char** inputArray(int * size){

    int mem = 8;

    char*(* arr) = malloc(sizeof(char *) * mem);

    if (!arr) {

    return NULL;

    }

    char * TestGet;

    int num = 0;

    while ( (TestGet = get_s() ) != NULL) {   

    num++;

    if (num > mem){

    mem = mem * 2;

    arr = realloc(arr, sizeof(char *) * mem);

    if (!arr) {

    return NULL;

    }

    }

    arr[num - 1] = TestGet;     

    }

    *size = num;

    return arr;

}

void SortArr(char*(* arrStr), int size){



    for (int i = 0; i <= size - 1; i++) {

        for (int j = (size - 1); j > i; j--) {

            if ( strcmp(arrStr[j - 1], arrStr[j]) > 0) {

                char* temp = arrStr[j - 1];

                arrStr[j - 1] = arrStr[j];

                arrStr[j] = temp;

            }

        }

    }

}



void PrintArray( char*(* arrStr), int sizeOfArray) {

    for (int i = 0; i < sizeOfArray; i++) {

        printf("%s\n", arrStr[i]);

        free(arrStr[i]);

    }

    free( arrStr );

}



int main() {

    int num;

    char*(* arrStr);

    arrStr = inputArray(&num);

    SortArr(arrStr, num);

    PrintArray(arrStr, num);

    return 0;

}
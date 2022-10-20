#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 10

char* getStr(FILE * f1){ //ввод строки
    unsigned int n;
    int k = 0;
    char *s = (char *) malloc(M);
    if (!s) {
        free(s);
        return NULL;
    }
    while(fgets(s + k, M, f1)){
        n = strlen(s);
        if(s[n-1] != '\n'){
            k = k + M - 1;
            s = (char *)realloc(s, k + M);
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

int main(int argc, char ** argv) {
    if (argc != 2){
        printf("Wrong number of parameters!\n");
        return 0;
    }
    FILE * f1;
    char * s1, *s2;
    int count = 0;
    unsigned max = 0, flag = 0;
    f1 = fopen(argv[1], "r");
    if (f1 == NULL){
        printf("No such file or directory\n");
        return 0;
    }
    FILE * f2 = fopen("tmp", "w+");
    while((s1 = getStr(f1)) != NULL){
        if (strlen(s1) > max)
            max = strlen(s1);
        fprintf(f2, "%s\n", s1);
        free(s1);
    }
    free(s1);
    char s3[max];
    max = 0;
    fseek(f1, 0, SEEK_SET);
    while((s1 = getStr(f1)) != NULL){
        fseek (f2, 0, SEEK_SET);
        while((s2 = getStr(f2)) != NULL){
            if(!strcmp(s1, s2)){
                count ++;
            }
            free(s2);
        }
        if (count > max){
            max = count;
            strcpy(s3, s1);
            flag = 1;
        }
        free(s1);
        free(s2);
        count = 0;
    }
    if (flag)
        printf("The answer is: %s \n", s3);
    else
        printf("Empty file!");
    fclose(f1);
    fclose(f2);
    remove("tmp");
    return 0;
}
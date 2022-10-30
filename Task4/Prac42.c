#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 10

char* getStr(FILE * f){ //ввод строки
    unsigned int n;
    int k = 0;
    char *s = (char *) malloc(M);
    if (!s) {
        free(s);
        return NULL;
    }
    while(fgets(s + k, M, f)){
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

int main(int argc, char**argv) {
    if (argc != 3){
        printf("Wrong number of parameters! Needs file name and max string length.\n");
        return 0;
    }
    FILE * f1 = fopen(argv[1], "r");
    char * s1;
    unsigned max = 0;
    while ((s1 = getStr(f1)) != NULL){
        if (strlen(s1) > max)
            max = strlen(s1);
        free(s1);
    }
    free(s1);
    FILE * f2 = fopen("tmpFile", "w+");
    for (int i = 0; i <= max; i++){
        fseek(f1, 0, SEEK_SET);
        while((s1 = getStr(f1)) != NULL){
            if(strlen(s1) == i)
                fprintf(f2, "%s\n", s1);
            free(s1);
        }
    }
    fclose(f1);
    f1 = fopen(argv[1], "w");
    fseek(f2, 0, SEEK_SET);
    while ((s1 = getStr(f2)) != NULL){
        fprintf(f1, "%s\n", s1);
        free(s1);
    }
    free(s1);
    fclose(f1);
    fclose(f2);
    remove("tmpFile");
    return 0;
}
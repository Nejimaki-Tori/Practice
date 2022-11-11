#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define Memory 10

char * getStr(FILE * f){
    char * s = (char *)malloc(Memory);
    if (!s){
        free(s);

        return NULL;
    }
    int k = 0;
    unsigned n;
    while(fgets(s + k, Memory, f)){ //ввод строки
        n = strlen(s);
        if (s[n-1] != '\n'){
            k = k + Memory - 1;
            s = (char *)realloc(s, k + Memory);
            if(!s){
                free(s);

                return NULL;
            }
        } else {
            s[n-1] = '\0';

            return s;
        }
    }
    free(s);

    return NULL;
}

char** strPtr(char const * strCurrent, int * n) { //разбиение строки
    if (!strCurrent){

        return NULL;
    }
    char * s = NULL;
    char** arrCurrent = NULL;
    arrCurrent = (char**)malloc(sizeof(char*) * Memory);
    if (!arrCurrent){

        return NULL;
    }
    int countStr = 0, num = Memory, countArr = 0, flagSpace = 0, mem = Memory, flagQuote = 0, flagLastQuote = 0, tmp = 0, i = 0, flagSpecialSymbol = 0;
    char c;
    while ((c = strCurrent[i]) != '\0'){
        if ( (c != ' ') && (c != '"') && (c != '\n') && (flagQuote == 0) && (c != '&') && (c != '|') && (c != '>') && (c != '<') && (c != ';') && (c != '(') && (c != ')') ) { //ввод "обычного" символа
            if (!s){
                s = (char *)malloc(Memory);
                if(!s){
                    free(s);

                    return NULL;
                }
            }
            if (flagSpecialSymbol) { //проверка на то, если до этого встретился спецсимвол
                s = (char *) realloc(s, num + 2);
                if(!s){
                    free(s);

                    return NULL;
                }
                s[countStr] = '\0';
                countArr++;
                if (countArr > mem) {
                    mem = mem * 2;
                    arrCurrent = (char **) realloc(arrCurrent, sizeof(char *) * mem);
                    if (!arrCurrent){

                        return NULL;
                    }
                }
                arrCurrent[countArr - 1] = s;
                s = NULL;
                s = (char *)malloc(Memory);
                if(!s){
                    free(s);

                    return NULL;
                }
                countStr = 0;
            }
            flagSpecialSymbol = 0;
            flagSpace = 0;
            num = Memory;
            countStr++;
            if(countStr > num){
                num += countStr;
                s = (char *)realloc(s, num);
                if(!s){
                    free(s);

                    return NULL;
                }

            }
            s[countStr - 1] = c;
        } else if (c == ' ' && flagSpace == 0 && flagQuote == 0) {     //первый пробел без кавычки
            if (flagSpecialSymbol) { //проверка на то, если до этого встретился спецсимвол
                s = (char *) realloc(s, num + 2);
                if(!s){
                    free(s);

                    return NULL;
                }
                s[countStr] = '\0';
                countArr++;
                if (countArr > mem) {
                    mem = mem * 2;
                    arrCurrent = (char **) realloc(arrCurrent, sizeof(char *) * mem);
                    if (!arrCurrent){

                        return NULL;
                    }
                }
                arrCurrent[countArr - 1] = s;
                s = NULL;
                countStr = 0;
            } else {
                flagSpace = 1;
                s = (char *) realloc(s, num + 2);
                if(!s){
                    free(s);

                    return NULL;
                }
                s[countStr] = '\0';
                countArr++;
                if (countArr > mem) {
                    mem = mem * 2;
                    arrCurrent = (char **) realloc(arrCurrent, sizeof(char *) * mem);
                    if (!arrCurrent){

                        return NULL;
                    }
                }
                arrCurrent[countArr - 1] = s;
                s = NULL;
                countStr = 0;
            }
        } else if (c == '"' && flagQuote == 0) {      //первая кавычка
            if (flagSpecialSymbol) { //проверка на то, если до этого встретился спецсимвол
                s = (char *) realloc(s, num + 2);
                if(!s){
                    free(s);

                    return NULL;
                }
                s[countStr] = '\0';
                countArr++;
                if (countArr > mem) {
                    mem = mem * 2;
                    arrCurrent = (char **) realloc(arrCurrent, sizeof(char *) * mem);
                    if (!arrCurrent){

                        return NULL;
                    }
                }
                arrCurrent[countArr - 1] = s;
                s = NULL;
                countStr = 0;
                flagSpecialSymbol = 0;
            }
            flagQuote = 1;
        } else if (flagQuote) { //ввод внутри кавычек
            if (c != '"') {
            if (!s){
                s = (char *)malloc(Memory);
                if(!s){
                    free(s);

                    return NULL;
                }
            }
            num = Memory;
            countStr++;
            if(countStr > num){
                num += countStr;
                s = (char *)realloc(s, num);
                if(!s){
                    free(s);

                    return NULL;
                }
            }
            s[countStr - 1] = c;
            } else { //завершение ввода в кавычках
                flagQuote = 0;
                flagLastQuote = 1;
                tmp = countStr;
            }
        } else if (flagLastQuote == 1 && c == ' ') { //установка переноса строки, если после кавычки встретился пробел
            arrCurrent[countArr - 1][tmp] = '\0';
            flagLastQuote = 0;
        } else if (c == '&' || c == '|' || c == '>' || c == '<' || c == ';' || c == '(' || c == ')') { //проверка спецсимволов
            if (flagSpecialSymbol){
                if (s[countStr - 1] == c && (c != ';') && (c != '(') && (c != ')')){ //проверка на повторный спецсимвол
                    countStr++;
                    if(countStr > num){
                        num += countStr;
                        s = (char *)realloc(s, num);
                        if(!s){
                            free(s);

                            return NULL;
                        }
                    }
                    s[countStr - 1] = c;
                    s = (char *) realloc(s, num + 2);
                    if(!s){
                        free(s);

                        return NULL;
                    }
                    s[countStr] = '\0';
                    countArr++;
                    if (countArr > mem) {
                        mem = mem * 2;
                        arrCurrent = (char **) realloc(arrCurrent, sizeof(char *) * mem);
                        if (!arrCurrent){

                            return NULL;
                        }
                    }
                    arrCurrent[countArr - 1] = s;
                    s = NULL;
                    countStr = 0;
                    flagSpecialSymbol = 0;
                } else { //новый спецсимвол
                    s = (char *) realloc(s, num + 2);
                    if(!s){
                        free(s);

                        return NULL;
                    }
                    s[countStr] = '\0';
                    countArr++;
                    if (countArr > mem) {
                        mem = mem * 2;
                        arrCurrent = (char **) realloc(arrCurrent, sizeof(char *) * mem);
                        if (!arrCurrent){

                            return NULL;
                        }
                    }
                    arrCurrent[countArr - 1] = s;
                    s = NULL;
                    countStr = 1;
                    s = (char *)malloc(Memory);
                    if(!s){
                        free(s);

                        return NULL;
                    }
                    s[countStr - 1] = c;
                }
            } else { //впервые встретили спецсимвол
                if (s) {
                    s = (char *) realloc(s, num + 2);
                    s[countStr] = '\0';
                    countArr++;
                    if (countArr > mem) {
                        mem = mem * 2;
                        arrCurrent = (char **) realloc(arrCurrent, sizeof(char *) * mem);
                    }
                    arrCurrent[countArr - 1] = s;
                }
                s = NULL;
                s = (char *)malloc(Memory);
                if(!s){
                    free(s);

                    return NULL;
                }
                num = Memory;
                countStr = 1;
                s[countStr - 1] = c;
                flagSpecialSymbol = 1;
            }
        } else if (c != ' '){ //завершаем ввод "обычного"(без кавычек, спецсимволов и т.д.) слова
            if (s) {
                s = (char *) realloc(s, num + 2);
                if(!s){
                    free(s);

                    return NULL;
                }
                s[countStr] = '\0';
                countArr++;
                if(countArr > mem){
                    mem = mem * 2;
                    arrCurrent = (char**)realloc(arrCurrent, sizeof(char *) * mem);
                    if (!arrCurrent){

                        return NULL;
                    }

                }
                arrCurrent[countArr - 1] = s;
            }
        }
        i++;
    }
    if (s) {//сохранение последнего слова
        s = (char *) realloc(s, num + 2);
        if(!s){
            free(s);

            return NULL;
        }
        s[countStr] = '\0';
        countArr++;
        if(countArr > mem){
            mem = mem * 2;
            arrCurrent = (char**)realloc(arrCurrent, sizeof(char *) * mem);
            if (!arrCurrent){

                return NULL;
            }

        }
        arrCurrent[countArr - 1] = s;
    }
    *n = countArr;
    if(countArr > mem) {
        mem = mem * 2;
        arrCurrent = (char **) realloc(arrCurrent, sizeof(char *) * mem);
        if (!arrCurrent) {

            return NULL;
        }
    }
    arrCurrent[countArr] = NULL;
    return arrCurrent;
}

void delArr(char*(* arrStr), int sizeOfArray) { //удаление массива
        for (int i = 0; i < sizeOfArray; i++) {
            free(arrStr[i]);
        }
        free(arrStr);
}

int main(int argc, char** argv){
    char * str = NULL;
    char** arr = NULL;
    int countArr = 0, pid, status = 0, err = 0;
    FILE * f;
    if (argc == 1){
        f = stdin;
    } else if (argc == 2) {
        f = fopen(argv[1], "r");
        if(!f) {
            printf("Can't open the file.\n");
            return -1;
        }
    } else {
        printf("Wrong parameters!\n");
        return -1;
    }
    while ((str = getStr(f)) != NULL) { //ввод
        arr = strPtr(str, &countArr);
        if (!strcmp(arr[0], "cd")){ //реализация команды cd
            if (countArr == 1){
                chdir(getenv("HOME"));
            } else
                err = chdir(arr[1]);
            if (err){
                printf("Wrong directory!\n");
            }
        } else if(fork() > 0){ //блокирование отца
            pid = wait(&status);
            if (pid == -1)
                return -1;
        } else { //выполнение команды shell
            err = execvp(arr[0], arr);
            if (err)
                execv(arr[0], arr);
            printf("Command < %s > wasn't found.\n", arr[0]);
            return -1;
        }
        err = 0;
        delArr(arr, countArr);
        free(str);
    }
    return 0;
}
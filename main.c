#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#define Memory 10

char * getStr(FILE * f){
    char * s = (char *)malloc(Memory);
    int k = 0;
    unsigned n;
    while(fgets(s + k, Memory, f)){ //ввод строки
        n = strlen(s);
        if (s[n-1] != '\n'){
            k = k + Memory - 1;
            s = (char *)realloc(s, k + Memory);
        } else {
            s[n-1] = '\0';
            if (n > 1) {
                if (s[n - 2] == '\r') {
                    s[n - 2] = '\0';
                }
            }
            return s;
        }
    }
    free(s);

    return NULL;
}

char** strPtr(char const * strCurrent, int * n) { //разбиение строки
    char * s = NULL;
    char** arrCurrent = NULL;
    arrCurrent = (char**)malloc(sizeof(char*) * Memory);
    int countStr = 0, num = Memory, countArr = 0, flagSpace = 0, mem = Memory,
            flagQuote = 0, flagLastQuote = 0, tmp = 0, i = 0, flagSpecialSymbol = 0;
    char c;
    while ((c = strCurrent[i]) != '\0'){
        if ( (c != ' ') && (c != '"') && (c != '\n') && (flagQuote == 0) &&
             (c != '&') && (c != '|') && (c != '>') && (c != '<') && (c != ';') &&
             (c != '(') && (c != ')') )
        { //ввод "обычного" символа
            if (!s){
                s = (char *)malloc(Memory);
            }
            if (flagSpecialSymbol) { //проверка на то, если до этого встретился спецсимвол
                s = (char *) realloc(s, num + 2);
                s[countStr] = '\0';
                countArr++;
                if (countArr > mem) {
                    mem = mem * 2;
                    arrCurrent = (char **) realloc(arrCurrent, sizeof(char *) * mem);
                }
                arrCurrent[countArr - 1] = s;
                s = NULL;
                s = (char *)malloc(Memory);
                countStr = 0;
            }
            flagSpecialSymbol = 0;
            flagSpace = 0;
            num = Memory;
            countStr++;
            if(countStr > num){
                num += countStr;
                s = (char *)realloc(s, num);

            }
            s[countStr - 1] = c;
        } else if (c == ' ' && flagSpace == 0 && flagQuote == 0) { //первый пробел без кавычки
            if (flagSpecialSymbol) { //проверка на то, если до этого встретился спецсимвол
                s = (char *) realloc(s, num + 2);
                s[countStr] = '\0';
                countArr++;
                if (countArr > mem) {
                    mem = mem * 2;
                    arrCurrent = (char **) realloc(arrCurrent, sizeof(char *) * mem);
                }
                arrCurrent[countArr - 1] = s;
                s = NULL;
                countStr = 0;
            } else {
                flagSpace = 1;
                s = (char *) realloc(s, num + 2);
                s[countStr] = '\0';
                countArr++;
                if (countArr > mem) {
                    mem = mem * 2;
                    arrCurrent = (char **) realloc(arrCurrent, sizeof(char *) * mem);
                }
                arrCurrent[countArr - 1] = s;
                s = NULL;
                countStr = 0;
            }
        } else if (c == '"' && flagQuote == 0) { //первая кавычка
            if (flagSpecialSymbol) { //проверка на то, если до этого встретился спецсимвол
                s = (char *) realloc(s, num + 2);
                s[countStr] = '\0';
                countArr++;
                if (countArr > mem) {
                    mem = mem * 2;
                    arrCurrent = (char **) realloc(arrCurrent, sizeof(char *) * mem);
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
                }
                num = Memory;
                countStr++;
                if(countStr > num){
                    num += countStr;
                    s = (char *)realloc(s, num);
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
                    }
                    s[countStr - 1] = c;
                    s = (char *) realloc(s, num + 2);
                    s[countStr] = '\0';
                    countArr++;
                    if (countArr > mem) {
                        mem = mem * 2;
                        arrCurrent = (char **) realloc(arrCurrent, sizeof(char *) * mem);
                    }
                    arrCurrent[countArr - 1] = s;
                    s = NULL;
                    countStr = 0;
                    flagSpecialSymbol = 0;
                } else { //новый спецсимвол
                    s = (char *) realloc(s, num + 2);
                    s[countStr] = '\0';
                    countArr++;
                    if (countArr > mem) {
                        mem = mem * 2;
                        arrCurrent = (char **) realloc(arrCurrent, sizeof(char *) * mem);
                    }
                    arrCurrent[countArr - 1] = s;
                    s = NULL;
                    countStr = 1;
                    s = (char *)malloc(Memory);
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
                num = Memory;
                countStr = 1;
                s[countStr - 1] = c;
                flagSpecialSymbol = 1;
            }
        } else if (c != ' '){ //завершаем ввод "обычного"(без кавычек, спецсимволов и т.д.) слова
            if (s) {
                s = (char *) realloc(s, num + 2);
                s[countStr] = '\0';
                countArr++;
                if(countArr > mem){
                    mem = mem * 2;
                    arrCurrent = (char**)realloc(arrCurrent, sizeof(char *) * mem);

                }
                arrCurrent[countArr - 1] = s;
            }
        }
        i++;
    }
    if (s) {//сохранение последнего слова
        s = (char *) realloc(s, num + 2);
        s[countStr] = '\0';
        countArr++;
        if(countArr > mem){
            mem = mem * 2;
            arrCurrent = (char**)realloc(arrCurrent, sizeof(char *) * mem);

        }
        arrCurrent[countArr - 1] = s;
    }
    *n = countArr;
    if(countArr > mem) {
        mem = mem * 2;
        arrCurrent = (char **) realloc(arrCurrent, sizeof(char *) * mem);
    }
    arrCurrent[countArr] = NULL;
    return arrCurrent;
}

void sigHandler(int s){
    printf("\nShell: ");
    fflush(stdout);
    signal(SIGINT, sigHandler);
}

char*** commandPtr(char ** arr, int countArr, int * cmdInd, char * sign1, char * sign2){ //разбиение на команды конвейера
    char** arrTmp = NULL;
    char*** arrCmd = malloc(sizeof(char **) * Memory);
    int index = 0, indCmd = 0, indexTmp = 0, mem = Memory, memCmd = Memory;
    //printf("tst %s\n", sign1);
    while(index <= countArr){
        if (arr[index]) {
            if(!arrTmp){
                arrTmp = malloc(sizeof(char *) * Memory);
            }
            if (strcmp(arr[index], sign1) != 0 && strcmp(arr[index], sign2) != 0) {
                arrTmp[indexTmp] = arr[index];
                indexTmp++;
                if (indexTmp >= mem){
                    mem *= 2;
                    arrTmp = (char **)realloc(arrTmp, sizeof(char *) * mem);
                }
            } else {
                arrTmp[indexTmp] = NULL;
                arrCmd[indCmd] = arrTmp;
                arrTmp = NULL;
                indCmd++;
                if (indCmd >= memCmd){
                    memCmd *= 2;
                    arrCmd = (char ***)realloc(arrTmp, sizeof(char *) * memCmd);
                }
                indexTmp = 0;
            }
        } else {
            if (indexTmp){
                arrTmp[indexTmp] = NULL;
                arrCmd[indCmd] = arrTmp;
                indCmd++;
            }
        }
        index++;
    }
    arrCmd[indCmd] = NULL;
    *cmdInd = indCmd;
    if (indCmd > 0)
        return arrCmd;
    else {
        free(arrCmd);
        free(arrTmp);
        *cmdInd = 0;
        return NULL;
    }
}

int find(char** arr){ //поиск спецсимволов
    int index = 0;
    while(arr[index]){
        if(!strcmp(arr[index], ">") || !strcmp(arr[index], "<") || !strcmp(arr[index], ">>")){
            return index;
        }
        index++;
    }
    return -1;
}

void redirect(char** files, char* sign, int index){
    int fd;
    if (!strcmp(sign, "<") || !strcmp(sign, ">") || !strcmp(sign, ">>") ){
        if (!strcmp(sign, "<")){
            fd = open(files[index + 1], O_RDONLY, 0664);
            dup2(fd, 0);
            close(fd);
        }
        if (!strcmp(sign, ">")){
            fd = open(files[index + 1], O_WRONLY | O_CREAT | O_TRUNC, 0664);
            dup2(fd, 1);
            close(fd);
        }
        if (!strcmp(sign, ">>")){
            fd = open(files[index + 1], O_APPEND | O_WRONLY | O_CREAT, 0664);
            dup2(fd, 1);
            close(fd);
        }
        if (files[index + 2] && (!strcmp(files[index + 2], ">") || !strcmp(files[index + 2], ">>") || !strcmp(files[index + 2], "<"))){
            sign = files[index + 2];
            if (!strcmp(sign, ">")){
                fd = open(files[index + 3], O_WRONLY | O_CREAT, 0664);
                dup2(fd, 1);
                close(fd);
            } else if (!strcmp(sign, ">>")) {
                fd = open(files[index + 3], O_APPEND | O_WRONLY | O_CREAT, 0664);
                dup2(fd, 1);
                close(fd);
            } else {
                fd = open(files[index + 3], O_RDONLY, 0664);
                dup2(fd, 0);
                close(fd);
            }
        }
    }
}

int wordCount(char** arr){ //подсчет количества слов в массиве
    int count = 0;
    while(arr[count]){
        count++;
    }
    return count;
}

int pipeLine(char*** arr, int indPipe){ //конвейер
    int fd[2], index = 0, ind, err = 0, fd1, fd2, countArr, flagBack = 0, errGlobal = 0, status = 0;
    int tmp = dup(0);
    countArr = wordCount(arr[0]);
    if (arr[0][0] && !strcmp(arr[0][0], "cd")){ //реализация команды cd
        if (countArr == 1){
            chdir(getenv("HOME"));
        } else
            err = chdir(arr[0][1]);
        if (err){
            printf("Wrong directory!\n");
        }
        return 0;
    }
    countArr = wordCount(arr[indPipe-1]);
    if(countArr > 0 && !strcmp(arr[indPipe - 1][countArr - 1], "&")){ //проверка на фоновый режим
        flagBack = 1;
        arr[indPipe - 1][countArr - 1] = NULL;
    }
    while(arr[index]) { //конвейер
        err = pipe(fd);
        if(err){
            printf("Couldn't create the pipe!\n");
            exit(1);
        }
        if (fork() == 0) { //сын
            if(flagBack) { //запуск всех процессов конвейера в фоне
                if (fork() == 0) { //внук
                    if (index == 0) { //блокировка ввода для первой команды
                        fd1 = open("/dev/null", O_RDONLY, 0644);
                        dup2(fd1, 0);
                        close(fd1);
                    }
                    fd2 = open("/dev/null", O_WRONLY, 0644);
                    dup2(fd2, 1);
                    close(fd2);
                    signal(SIGINT, SIG_IGN);
                    ind = find(arr[index]);
                    if (ind > 0) { //перенаправление в файл
                        redirect(arr[index], arr[index][ind], ind);
                        arr[index][ind] = NULL;
                    }
                    if (index != indPipe - 1) {
                        dup2(fd[1], 1);
                    }
                    execvp(arr[index][0], arr[index]);
                    execv(arr[index][0], arr[index]);
                    printf("Command < %s > wasn't found.\n", arr[index][0]);
                    fflush(NULL);
                    exit(1);
                } else { //завершение сына
                    exit(0);
                }
            } else { // "обычный" конвейер
                if (index == 0 || index == indPipe - 1) { //перенаправление ввода-вывода
                    ind = find(arr[index]);
                    if (ind > 0) {
                        redirect(arr[index], arr[index][ind], ind);
                        arr[index][ind] = NULL;
                    }
                }
                if (index != indPipe - 1) {
                    dup2(fd[1], 1);
                }
                signal(SIGINT, SIG_DFL);
                close(fd[0]);
                close(fd[1]);
                execvp(arr[index][0], arr[index]);
                execv(arr[index][0], arr[index]);
                printf("Command < %s > wasn't found.\n", arr[index][0]);
                fflush(NULL);
                exit(1);
            }
        } else { //отец
            dup2(fd[0], 0);
            close(fd[0]);
            close(fd[1]);
        }
        index++;
    }
    dup2(tmp, 0); //сохранение stdin
    close(tmp);
    if (indPipe == 1){
        signal(SIGINT, SIG_IGN);
        wait(&errGlobal); //ожидаем завершения команды
        signal(SIGINT, sigHandler);
    } else {
        while(wait(&status) != -1){ //ожидаем завершения конвейера
            if(status){
                errGlobal = status;
            }
        }
    }
    return errGlobal;
}

char** symbolFind(char** arr, int * ind){
    char** arrSymb = malloc(sizeof(char*) * Memory);
    int count = 0, index = 0, mem = Memory;
    char * signAnd = "&&\0";
    char * signOr = "||\0";
    while(arr[count]){
        if(!strcmp(arr[count], signAnd) || !strcmp(arr[count], signOr)){
            arrSymb[index] = arr[count];
            index++;
        }
        if (index >= mem){
            mem *= 2;
            arrSymb = realloc(arrSymb, sizeof(char*) * mem);
        }
        count++;
    }
    *ind = index;
    arrSymb[index] = NULL;
    return arrSymb;
}

void delArr(char*(* arrStr), int sizeOfArray, char*** arrCmd, int indCmd) { //удаление массивов
    if (arrStr){
        for (int i = 0; i < sizeOfArray; i++) {
            free(arrStr[i]);
        }
    }
    if (arrCmd) {
        for (int i = 0; i < indCmd; i++) {
            free(arrCmd[i]);
        }
        free(arrCmd);
    }
    free(arrStr);
}

int main(int argc, char** argv){
    char * str = NULL;
    char** arr = NULL;
    char*** arrPipe = NULL;
    char*** arrCmd = NULL;
    char*** arrLogic = NULL;
    char * sign, * signCmd, * signAnd, *signOr;
    char ** arrSymb = NULL;
    sign = "|\0";
    signCmd = ";\0";
    signAnd = "&&\0";
    signOr = "||\0";
    int countArr = 0, indPipe = 0, indCmd = 0, indLogic = 0, indSymb = 0, countArrTmp, status = 0, tmp = 0;
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
    printf("Shell: ");
    signal(SIGINT, sigHandler);
    while ((str = getStr(f)) != NULL) { //ввод
        arr = strPtr(str, &countArr);
        arrCmd = commandPtr(arr, countArr, &indCmd, signCmd, signCmd);
        for (int i = 0; i < indCmd; i++){
            countArrTmp = wordCount(arrCmd[i]);
            arrLogic = commandPtr(arrCmd[i], countArrTmp, &indLogic, signAnd, signOr);
            arrSymb = symbolFind(arrCmd[i], &indSymb);
            for(int j = 0; j < indLogic; j++) {
                countArrTmp = wordCount(arrLogic[j]);
                arrPipe = commandPtr(arrLogic[j], countArrTmp, &indPipe, sign, sign);
                if (countArrTmp) {
                    if((j == 0) || (!strcmp(arrSymb[tmp], "&&") && !status) || (!strcmp(arrSymb[tmp], "||") && status)) {
                        status = pipeLine(arrPipe, indPipe); //конвейер
                        fflush(NULL);
                    } else {
                        status = 1;
                    }
                    if(j != 0){
                        tmp++;
                    }
                }
                for(int k = 0; k < indPipe; k++){
                    free(arrPipe[k]);
                }
                free(arrPipe);
            }
            tmp = 0;
            free(arrSymb);
            for(int j = 0; j < indLogic; j++){
                free(arrLogic[j]);
            }
            free(arrLogic);
        }
        printf("Shell: ");
        delArr(arr, countArr, arrCmd, indCmd);
        free(str);
    }
    printf("\n");
    return 0;
}
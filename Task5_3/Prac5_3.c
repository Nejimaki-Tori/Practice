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
        } else if (c == ' ' && flagSpace == 0 && flagQuote == 0) {     //первый пробел без кавычки
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
        } else if (c == '"' && flagQuote == 0) {      //первая кавычка
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
    printf("\n");
    signal(SIGINT, sigHandler);
}

char*** commandPtr(char ** arr, int countArr, int * pipeInd, char * sign){ //разбиение на команды конвейера
    char** arrTmp = NULL;
    char*** arrPipe = malloc(sizeof(char **) * Memory);
    int index = 0, indPipe = 0, indexTmp = 0, mem = Memory, memPipe = Memory;
    while(index <= countArr){
        if (arr[index]) {
            if(!arrTmp){
                arrTmp = malloc(sizeof(char *) * Memory);
            }
            if (strcmp(arr[index], sign) != 0) {
                arrTmp[indexTmp] = arr[index];
                indexTmp++;
                if (indexTmp >= mem){
                    mem *= 2;
                    arrTmp = (char **)realloc(arrTmp, sizeof(char *) * mem);
                }
            } else {
                arrTmp[indexTmp] = NULL;
                arrPipe[indPipe] = arrTmp;
                arrTmp = NULL;
                indPipe++;
                if (indPipe >= memPipe){
                    memPipe *= 2;
                    arrPipe = (char ***)realloc(arrTmp, sizeof(char *) * memPipe);
                }
                indexTmp = 0;
            }
        } else {
            if (indexTmp){
                arrTmp[indexTmp] = NULL;
                arrPipe[indPipe] = arrTmp;
                indPipe++;
            }
        }
        index++;
    }
    arrPipe[indPipe] = NULL;
    *pipeInd = indPipe;
    if (indPipe > 1)
        return arrPipe;
    else {
        free(arrPipe);
        free(arrTmp);
        *pipeInd = 0;
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

void pipeLine(char*** arr, int indPipe){ //конвейер
int fd[2], index = 0, ind, err, fd1, fd2, countArr, flagBack = 0;
int tmp = dup(0);
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
                exit(-1);
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
            exit(-1);
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
    while(wait(NULL) != -1); //ожидаем завершения конвейера
    printf("Shell: ");
}

void command(char** arr, int countArr){ //обычная команда
    int err = 0, ind, fd1, fd2;
    if (arr[0] && !strcmp(arr[0], "cd")){ //реализация команды cd
        if (countArr == 1){
            chdir(getenv("HOME"));
        } else
            err = chdir(arr[1]);
        if (err){
            printf("Wrong directory!\n");
        }
    } else if(( err = fork() ) > 0){ //блокирование отца
        wait(NULL);
        printf("Shell: ");
    } else if (err == 0){ //выполнение команды shell
        ind = find(arr);
        if (ind > 0) {
            redirect(arr, arr[ind], ind);
            arr[ind] = NULL;
        }
        if(countArr > 0 && !strcmp(arr[countArr - 1], "&")){ //выполнение фонового процесса
            if(fork() == 0) { //создание "внука"
                fd1 = open("/dev/null", O_RDONLY, 0644); //блокировка ввода
                fd2 = open("/dev/null", O_WRONLY, 0644); //и вывода
                dup2(fd1, 0);
                dup2(fd2, 1);
                close(fd1);
                close(fd2);
                arr[countArr - 1] = NULL;
                signal(SIGINT, SIG_IGN);
                ind = find(arr);
                if (ind > 0) {
                    redirect(arr, arr[ind], ind);
                    arr[ind] = NULL;
                }
                execvp(arr[0], arr);
                execv(arr[0], arr);
                printf("Command < %s > wasn't found.\n", arr[0]);
                exit(-1);
            } else { //завершение сына
                exit(0);
            }
        } else { //команда shell не фоновая
            signal(SIGINT, SIG_DFL);
            execvp(arr[0], arr);
            execv(arr[0], arr);
            printf("Command < %s > wasn't found.\n", arr[0]);
            exit(-1);
        }
    } else
        printf("Couldn't execute the command!\n");
}

void delArr(char*(* arrStr), int sizeOfArray, char*** arrPipe, int pipeSize) { //удаление массивов
    for (int i = 0; i < sizeOfArray; i++) {
        free(arrStr[i]);
    }
    if (arrPipe) {
        for (int i = 0; i < pipeSize; i++) {
            free(arrPipe[i]);
        }
        free(arrPipe);
    }
    free(arrStr);
}

int main(int argc, char** argv){
    char * str = NULL;
    char** arr = NULL;
    char*** arrPipe = NULL;
    char * sign;
    sign = "|\0";
    int countArr = 0, indPipe = 0;
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
        arrPipe = commandPtr(arr, countArr, &indPipe, sign);
        if (arrPipe) {
            pipeLine(arrPipe, indPipe); //конвейер
        } else {
            command(arr, countArr); //обычная команда
        }
        delArr(arr, countArr, arrPipe, indPipe);
        free(str);
    }
    printf("\n");
    return 0;
}
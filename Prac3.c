#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 10
struct list{
    char * s;
    struct list * next;
};

char* getStr(void){ //ввод строки
    unsigned int n;
    int k = 0;
    char *s = (char *)malloc(M);
    if (!s) {
        free(s);
        return NULL;
    }
    while(fgets(s + k, M, stdin)){
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
struct list * wordSort(struct list * wordList, char * str){ //вставка слов в список сразу в нужном порядке
    char * strInList;
    struct list * newLink;
    struct list * currLink;
    struct list * tempLink;
    if (wordList == NULL){ //вставляем первое слово в пустой список
        wordList = (struct list *)malloc(sizeof(struct list *) * 8);
        wordList->s = str;
        wordList->next = NULL;
        return wordList;
    }
    else{
        strInList = wordList->s;
        if (strcmp(str, strInList) <= 0){ //вставляем слово перед началом списка
            newLink = (struct list *)malloc(sizeof(struct list *) * 8);
            newLink->s = str;
            newLink->next = wordList;
            wordList = newLink;
            return wordList;
        }
        else{
            if (wordList->next == NULL){  //вставляем слово сразу за первым словом в списке
                newLink = (struct list *) malloc(sizeof(struct list *) * 8);
                newLink->s = str;
                newLink->next = NULL;
                wordList->next = newLink;
                return wordList;
            }
            else{
                currLink = wordList;
                tempLink = currLink->next;
                while(1){
                    strInList = tempLink->s;
                    if(strcmp(str, strInList) <= 0){ //вставляем слово в середину списка
                        newLink = (struct list *) malloc(sizeof(struct list *) * 8);
                        newLink->s = str;
                        newLink->next = tempLink;
                        currLink->next = newLink;
                        return wordList;
                    }
                    else{
                        if (tempLink->next == NULL){ //вставляем слово в конец списка
                            newLink = (struct list *) malloc(sizeof(struct list *) * 8);
                            newLink->s = str;
                            newLink->next = NULL;
                            tempLink->next = newLink;
                            return wordList;
                        }
                        else{ //идем дальше по списку
                            currLink = tempLink;
                            tempLink = tempLink->next;
                        }
                    }
                }
            }
        }
    }
}

void printList(struct list * wordList){ //вывод списка
    struct list * tempLink;
    char * strPr;
    tempLink = wordList;
    while(tempLink != NULL){
        strPr = tempLink->s;
        printf("%s\n", strPr);
        tempLink = tempLink->next;
    }
}

void delList(struct list * wordList){ //удаление списка
    struct list * tempLink;
    while (wordList != NULL){
        tempLink = wordList;
        wordList = wordList->next;
        free(tempLink->s);
        free(tempLink);
    }
}

int main(){
    struct list * mainList;
    char * mainString = NULL;
    mainList = NULL;
    int flag = '1';
    while (flag != '0'){
        printf("Введите список до пустой строки: \n");
        mainString = getStr();
        if (mainString == NULL){
            return 0;
        }
        while (mainString[0] != '\0') {
            mainList = wordSort(mainList, mainString);
            mainString = getStr();
            if (mainString == NULL){
                return 0;
            }
        }
        printf("Отсортированный список: \n");
        printList(mainList);
        delList(mainList);
        mainList = NULL;
        free(mainString);
        printf("Ввести новый список? 1 - да, 0 - нет \n");
        flag = getchar();
        getchar();
    }
    return 0;
}
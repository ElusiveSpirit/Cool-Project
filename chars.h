#include "stdio.h"
#include "string.h"

#define STRLEN(s) ({int retval = 0; while (*(s + ++retval)); retval;})
#define MAX_SIZE 256

int getCharArray(char **pInputArray, size_t &size) {
    size = 0;
    char *pArray = NULL;
    char *pBuff = (char *)malloc(MAX_SIZE * sizeof(char));
    if (pBuff == NULL)
        return -1;

    while (fgets(pBuff, MAX_SIZE, stdin)) {

        char *pNewLine = strchr(pBuff, '\n');
        size_t buffSize = 0;
        if (pNewLine != NULL) {
            buffSize = pNewLine - pBuff;
        } else {
            buffSize = STRLEN(pBuff);
        }

        char *pTemp = (char *)realloc(pArray,
                (size + buffSize + 1) * sizeof(char));
        if (pTemp == NULL) {
            if (pArray != NULL)
                free(pArray);
            free(pBuff);
            return -1;
        }
        pArray = pTemp;
        pTemp = (char *)memcpy(pArray + size, pBuff, buffSize);
        size += buffSize;
        if (pTemp == NULL) {
            free(pArray);
            free(pBuff);
            return -1;
        }
        pArray[size] = '\0';

        if (pNewLine != NULL) break;
    }
    free(pBuff);

    if (*pInputArray != NULL)
        free(*pInputArray);
    *pInputArray = pArray;
    return 0;
}

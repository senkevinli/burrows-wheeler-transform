#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "bwt.h"

/* Suffix struct that keeps track of string and starting index. */
struct Suffix {
    const char *string;
    int idx;
};

static void createSuffixArr(const char *str, int len, int *suffixArr);

/**
 * @brief Returns a new string after the Burrows-WHeeler transformation.
 * NOTE: Must be freed after call.
 * 
 * @param str String to be transformed.
 * @return const char* Transformed string.
 */
const char *createBwt(const char *str){
    
    int len = strlen(str);

    char *dup = malloc((len + 1) * sizeof(char));
    strncpy(dup, str, len);
    dup[len] = BWT_EOF;

    int suffixArr[len + 1];
    createSuffixArr(dup, len + 1, suffixArr);

    char *ret = malloc((len + 1) * sizeof(char));

    /* Last column will be the character to the left of suffix array
       starting index. */
    int i;
    for (i = 0; i < len + 1; i++){
        ret[i] = suffixArr[i] == 0 ? BWT_EOF : dup[suffixArr[i] - 1];
    }

    free(dup);

    return ret;
}

/* Comparator for stdlib quicksort. */
static int comparator(const void *p, const void *q){
    const char *pStr = (*(struct Suffix **)p)->string;
    const char *qStr = (*(struct Suffix **)q)->string;

    return strcmp(pStr, qStr);
}

/**
 * @brief Creates the suffix array of the given string.
 * 
 * @param str The string from which to construct a suffix array.
 * @param len The length of the string included the null terminating character.
 * @param suffixArr Will be modified with suffix indices after calling.
 */
static void createSuffixArr(const char *str, int len, int *suffixArr){
    
    int i;
    struct Suffix **temp = malloc(len * sizeof(struct Suffix *));

    /* Construct suffixes. */
    for (i = len - 1; i >= 0; i--){
        char *suffixStr = malloc((len - i) * sizeof(char));
        memcpy(suffixStr, str + i, len - i);

        struct Suffix *s = malloc(sizeof(struct Suffix));
        s->idx = i;
        s->string = suffixStr;

        temp[i] = s;
    }

    /* Sort suffixes. */
    qsort((void *)temp, len, sizeof(struct Suffix *), comparator);

    /* Only the starting indices are relevant. */
    for (i = 0; i < len; i++){
        suffixArr[i] = temp[i]->idx;
    }

    /* Free everything. */
    for (i = 0; i < len; i++){
        free((void *)temp[i]->string);
        free((void *)temp[i]);
    }
    free((void *)temp);

    return;
}
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

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
const char *createBww(const char *str){
    
    int len = strlen(str);
    int suffixArr[len + 1];
    createSuffixArr(str, len + 1, suffixArr);

    return NULL;
}

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

    qsort((void *)temp, len, sizeof(struct Suffix *), comparator);
    for (i = 0; i < len; i++){
        puts(temp[i]->string);
    }
}
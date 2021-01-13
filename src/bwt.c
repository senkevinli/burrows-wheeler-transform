#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "bwt.h"

#define CHARS 256
/* Suffix struct that keeps track of string and starting index. */
struct Suffix {
    const char *string;
    int idx;
};

/* Helper functions. */
static void naiveSuffixArr(const char *str, int len, int *suffixArr);
static void skewSuffixArr(const char *str, int len, int *suffixArr);

static void countSort(const char *src, int len, char *dest, int range);

/**
 * @brief Returns a new string after the Burrows-WHeeler transformation.
 * NOTE: Must be freed after call.
 * 
 * @param str String to be transformed.
 * @return const char* Transformed string.
 */
const char *createBwt(const char *str) {
    
    int len = strlen(str);

    char *dup = calloc((len + 2), sizeof(char));
    strncpy(dup, str, len);
    dup[len] = BWT_EOF;

    int suffixArr[len + 1];
    naiveSuffixArr(dup, len + 1, suffixArr);

    char *ret = calloc((len + 2), sizeof(char));

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
static int comparator(const void *p, const void *q) {
    const char *pStr = (*(struct Suffix **)p)->string;
    const char *qStr = (*(struct Suffix **)q)->string;

    return strcmp(pStr, qStr);
}

/**
 * @brief Creates the suffix array of the given string. Uses naive algorithm,
 * O(n^2logn) time complexity
 * 
 * @param str The string from which to construct a suffix array.
 * @param len The length of the string included the null terminating character.
 * @param suffixArr Will be modified with suffix indices after calling.
 */
static void naiveSuffixArr(const char *str, int len, int *suffixArr) {
    
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

/**
 * @brief Counting sort for strings -- character values between 0 to 255 inclusive.
 * Sort is stable and is O(n) time complexity.
 * 
 * @param src The string to sort.
 * @param len Length of the string.
 * @param dest Will be updated with the new sorted string.
 * @param range Range of values.
 */
static void countSort(const char *src, int len, char *dest, int range)
{

    int counts[range];
    memset(counts, 0, sizeof(counts));

    /* Getting frequencies. */
    int i;
    for (i = 0; i < len; i++)
        counts[src[i]]++;

    /* Accumulation from left to right. */
    int acc = 0;
    for (i = 0; i < range; i++) {
        int prev = counts[i];
        counts[i] += acc;
        acc += prev;
    }

    /* Shifting over one position to get the start. */
    int prev = 0;
    for (i = 0; i < range; i++) {
        int orig = counts[i];
        counts[i] = prev;
        prev = orig;
    }

    /* Constructing output array. */
    for (i = 0; i < len; i++) {
        int idx = counts[src[i]]++;
        dest[idx] = src[i];
    }
}

/**
 * @brief Creates the suffix array of the given string. Uses the skew algorithm,
 * O(n) time complexity
 * 
 * @param str The string from which to construct a suffix array.
 * @param len The length of the string included the null terminating character.
 * @param suffixArr Will be modified with suffix indices after calling.
 */
static void skewSuffixArr(const char *str, int len, int *suffixArr) {
    return;
}


/**
 * @brief Takes a BWT string and transforms it back to the original string.
 * BWT must include the EOF.
 * NOTE: Returned string must be freed after call.
 * 
 * @param str The BWT string to transform back.
 * @return const char* The original string before transformation.
 */
const char *inverseBwt(const char *str) {
    
    /* Get the first column via count sort. */
    int len = strlen(str);
    char *firstCol = malloc((len + 1) * sizeof(char));
    countSort(str, len, firstCol, CHARS);

    /* For tracking the ranks of each char, char corresponds to index. */
    int mapping[CHARS];
    memset(mapping, 0, sizeof(mapping));

    /* Parallel array for tracking ranks. */
    int *ranks = malloc(len * sizeof(int));
    int i;
    for (i = 0; i < len; i++)
        ranks[i] = mapping[str[i]]++;

    /* Reuse mapping array for calculating the starting row in the first column. */
    memset(mapping, 0, sizeof(mapping));
    for (i = 0; i < len; i++)
        /* Keep track of the first occurence only. */
        if (mapping[firstCol[i]] == 0)
            mapping[firstCol[i]] = i;


    /* String construction using FL relation. */
    char *ret = malloc((len + 1) * sizeof(char));
    int retIdx = len - 1;
    ret[retIdx--] = '\0';
    int rowIdx = 0;

    /* Stop once we return to EOF. */
    while (str[rowIdx] != BWT_EOF) {
        char character = str[rowIdx];
        ret[retIdx--] = character;
        rowIdx = mapping[character] + ranks[rowIdx];
    }

    free((void *)ranks);
    free((void *)firstCol);

    /* If assertion fails, string cannot be inverted. */
    //assert(strlen(ret) == len - 1);
    return ret;
}
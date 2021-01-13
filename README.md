# Burrows-Wheeler-Transform
![Tests](https://github.com/senkevinli/burrows-wheeler-transform/workflows/Tests/badge.svg)
> C Implementation of the Burrows-Wheeler-Transform.

## Task
> Vagheesh Narasimhan

With petabytes of data being generated of genetic sequences (think about how many people have got a covid test - this involves essentially sequencing the DNA in your swab and checking if one particular substring (that of the Sars-Cov-2 genome) exists in that sample. The sequencing process is pure molecular biology, but searching for a substring of 100 DNA bases of a specific sequence amidst 100s of thousands of random sequences (which occurs in your nasal swab) is a pure CS problem.

The way we currently deal with, compression, searching and indexing of DNA sequences is to use something called the [burrows wheeler transform](https://en.wikipedia.org/wiki/Burrows–Wheeler_transform).

I want you to read the wiki page. But also get an appreciation for why this transform is 
1. Linear in time to compute
2. Reversible/invertible
3. Results in significant compression over the initial input data.

I also want you to understand simple terms:
1. lexicographical order
2. suffix array

Our end goal is to implement an alignment (also known as search in lay parlance) of a substring on a long reference sequence - first introduced [here](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC2705234/).

The BWT algorithm however is old, almost 25 years old, which is close to dinosaur territory in CS. Despite much trying however, there has not been much of an advance in thinking about the theory of such algorithms that led to improvements. But lately, a new paper has brought up the idea of a generalized version of the BWT (https://arxiv.org/pdf/1902.01280.pdf). Suggesting that there is a whole class of BWT like algorithms that could be useful for different purposes.

**The goal for this week is very simple. Implement a C/C++ version of the BWT, suffix array and and another function to compute it's inverse on a simple string.**

## Implementation

### Creation

#### Lexicographical Ordering
Complete ordering of symbols, can be alphabetical if dealing with characters.  

#### Cyclically Rotated Matrix
The Burrows Wheeler Transformed string is constructed through the assemblage of characters in the last column of the (sorted) cyclically rotated matrix.

Here is the cyclically rotated matrix for `banana` with the EOF appended (more on this later, for now the EOF just denotes
the ending of the string and is lexicographically smaller than all other characters):
```
banana$
anana$b
nana$ba
ana$ban
na$bana
a$banan
$banana
```
Sorted (ascending) cyclically rotated matrix:
```
$banana
a$banan
ana$ban
anana$b
banana$
na$bana
nana$ba
```
Since the matrix is sorted (by a character's right context -- everything to the right of the character in question), it is likely that the last column will have similar characters grouped together if the string has recurring substring patterns.
For example, take the string `banana`. The transformed string will be: `annb$aa` with the `$` as a special EOF character. It is evident in the transformed string that similar characters, specifically `a` and `n` are grouped together due to the recurring string pattern of `ana` in `banana`.

#### Suffix Array
The suffix array is a data structure that contains the sorted suffixes of a string.  

Again, for `banana`, all the suffixes would be:  
```
a
n
ana
nana
anana
banana  
```
After sorting (ascending):
```
a  
ana  
anana  
banana  
na
nana
```
We only need to keep track of the starting index not the entire substring. So the suffix array becomes:
```
[5, 3, 1, 0, 4, 2] # each number corresponds to the starting index of the suffix.
```

What's interesing about the suffix array is that the layout of our sorted cyclically rotated matrix can
be in the exact same order of the suffix array if we include the special EOF character in our suffix array.
Because the EOF is the smallest lexicographical character, everything after the EOF when sorting becomes irrelevant
in our cyclically rotated matrix. With the EOF, sorting the cyclically rotated matrix devolves into the creation
of a suffix array for the ordering.

Once we have the suffix array, we can use the indices in conjunction with the original string to compute the
Burrows Wheeler Transformed string by taking the value at suffix array index `i` and subtracting one to get the left
index of the appropriate character (with one edge case for the EOF).

#### Summary
Steps for computing the Burrow's Wheeler Transform.
1. Create the suffix array (can be completed in linear time with the Skew algorithm).
2. Create transformed string by referencing the original string (linear time).


### Inversion

The existence of an inverse function for the Burrow's Wheeler Transform is what makes the BWT an algorithm for compression.
I believe (?) that the inverse function only works if there is an EOF character present in the string to be decoded.

#### FL Mapping and Ranks

The last column gives us enough information to construct the first column as all characters are present and the first column
is guaranteed to be in sorted order.  

First and last columns for: `annb$aa`
```
F:     L:
$      a
a      n
a      n
a      b
b      $
n      a
n      a
```

Since we know that the rotations are cyclic, we can deduce from the first and the last columns that the characters are adjacent,
specifically: characters in `L` on the same row precede `F`. So in the case that all letters are unique, such as the encoded string of `d$abc`, it's simple to find the order by tracing/matching the characters in `L` and `F` with each other and constructing the original string that way. In the case that there are some repeat letters, the FL Mapping shows us that within the same characters, the order of the characters in F and L are preserved.  

For example:
```
# We assign a unique marker to each 'a'.
F:     L:
$      a(1)
a(1)   n
a(2)   n
a(3)   b
b      $
n      a(2)
n      a(3)
```
This works because both are being sorted by their right context in both columns.  

Now we have a way to distinguish between distinct characters and characters that are the same. We can now traverse the FL mappings
by starting from the last column index 0. This will result in two traversals and remain in O(N).

#### Summary
For inverting the Burrows Wheeler Transform:
1. Sort the `L` column to create the `F` column. Can be accomplished in O(N + K) with a count sort where K is all the range
of values that the string can take on.
2. Find rankings in the `L` column and map them to rows in the `F` column. Can be done in two passes.
3. Traverse the FM mapping to reconstruct the original string. O(N) time complexity.

# Burrows-Wheeler-Transform
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
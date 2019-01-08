# Dynamic-Trie
gcc initialize.c -o initialize;./initialize</br>
gcc binary.c -o binary
time ./binary<input1

## Input format:
<pre>
i word
s word
d word
i #word_file
s #word_file
d #word_file
e
</pre>

word_file must be line separated</br>
## Description
The first 2 bytes of TrieBinary is used to store the size of the Trie: end</br>
Every next 52 bytes is a node. Each node has 26 children of 2 bytes each.</br>
If the child is end of string it is indicated by making its value negative.</br>

hexdump -d TrieBinary</br>
command to display binary files</br>
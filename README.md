# computer-science
Computer science studies helpers.   
Have fun :)

## FIAR-Minimax
A C implementation of *Four-In-a-Row* (CLI), user versus computer, and the *minimax* algorithm. The computer steps are being calculated and chosen by the algorithm.
The player may enter the level of difficulty, which implies the depth of the minimax tree.

### Further Reading:  
  - [The minimax algorithm](https://en.wikipedia.org/wiki/Minimax)

## MaxDHeap
A simple, non-generic and minimalist implementation of the heap sort algorithm
using **d-ary maximum heap**, to sort an array of integers.

"A d-ary heap is like a binary heap, but (with one possible exception) non-leaf
nodes have d children instead of 2 children." (problem 6.2, "Introduction to Algorithms",
by Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, and Clifford Stein)

### Indexes Explained
Let *A[0..n-1]* be the heap array (the heap size is *n*).  
In d-ary heap, the index of the **k'th son of node A[i]** is:
<center><img src="http://i.imgur.com/zNXzJpJ.png" width="35%"></center>

(skipping the children of the previous *i-1* nodes)

---
The index of the **parent of A[i]** is:
<center><img src="http://i.imgur.com/eh9s80O.png" width="14%"></center>

**Proof**:   
Let *p* be the index of the parent of *A[i]*. Then:
<center><img src="http://i.imgur.com/DeDspZn.png" width="48%"></center>
---
The **first leaf** of the heap is in index:
<center><img src="http://i.imgur.com/wi5mLto.png" width="17%"></center>

**Proof**:  
The index of the first son of A[i] is:
<center><img src="http://i.imgur.com/KVKRvPp.png" width="80%"></center>

Which means that it's **out of the array bounds**.  
In addition, for:
<center><img src="http://i.imgur.com/VgOvuQM.png" width="23%"></center>

The first son index is:
<center><img src="http://i.imgur.com/KTxiTIH.png" width="90%"></center>

Which means it's **within the array bounds**.

Because heap is an almost-full binary tree, we derive that ***i* is the index of the first leaf**.
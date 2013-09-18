#C++ Sorting Algorithm implementation

A collection of Sorting Algorithm implemented in C++11. See the `develop` branch for latest version.



##Insertion Sort
|http://en.wikipedia.org/wiki/Insertion_sort|stable|
|----|---|
|Worst case performance              |O(n2) comparisons, swaps|
|Best case performance               |O(n) comparisons, O(1) swaps|
|Average case performance            |O(n2) comparisons, swaps|
|Worst case space complexity         |O(n) total, O(1)|

###Merge Sort
|http://en.wikipedia.org/wiki/Mergesort|stable|
|----|---
|Worst case performance              |O(n log n)
|Best case performance               |O(n log n) typical, O(n) natural variant
|Average case performance            |O(n log n)
|Worst case space complexity         |O(n) auxiliary


###Heap Sort
|http://en.wikipedia.org/wiki/Heapsort|unstable|
|----|---
|Worst case performance              |O(n log n)
|Best case performance               |Omega(n), O(n log n)
|Average case performance            |O(n log n)
|Worst case space complexity         |O(n) total, O(1) auxiliary

###Quick Sort
|http://en.wikipedia.org/wiki/Quicksort|unstable|
|----|---
|Worst case performance              |O(n2) (extremely rare)
|Best case performance               |O(n log n)
|Average case performance            |O(n log n)
|Worst case space complexity         |O(n) auxiliary (naive) O(log n) auxiliary (Sedgewick 1978)

###Bubble Sort
|http://en.wikipedia.org/wiki/Bubble_sort|stable|
|----|---
|Worst case performance              |O(n^2)
|Best case performance               |O(n)
|Average case performance            |O(n^2)
|Space                               |inplace

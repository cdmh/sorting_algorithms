#C++ Sorting Algorithm implementation

A collection of Sorting Algorithm implemented in C++11. See the `develop` branch for latest version.

## Testing
Tested on g++ 4.8.1 on Ubuntu 13.04 Server and Microsoft Visual Studio 2013RC

###Bubble Sort
A slow a unuseful sorting algorithm for all but the simplest data sets, or for learning about sorting algorithms.

|http://en.wikipedia.org/wiki/Bubble_sort|stable|
|----|---
|Worst case performance              |O(n^2)
|Best case performance               |O(n)
|Average case performance            |O(n^2)
|Space                               |O(1)

###Cocktail Sort
aka Bubble Sort, cocktail shaker sort, ripple sort, shuffle sort, shuttle sort or happy hour sort

A Bubble sort variant which is only marginally more performant. Sorting is performed in both directions on each pass of the array.

|http://en.wikipedia.org/wiki/Cocktail_sort|stable|
|----|---
|Worst case performance              |O(n^2)
|Best case performance               |O(n)
|Average case performance            |O(n^2)
|Space                               |O(1)


###Heap Sort
|http://en.wikipedia.org/wiki/Heapsort|unstable|
|----|---
|Worst case performance              |O(n log n)
|Best case performance               |Omega(n), O(n log n)
|Average case performance            |O(n log n)
|Worst case space complexity         |O(n) total, O(1) auxiliary

##Insertion Sort
An adapted Bubble Sort that alternates between forward and backward passes.

|http://en.wikipedia.org/wiki/Insertion_sort|stable|
|----|---|
|Worst case performance              |O(n2) comparisons, swaps|
|Best case performance               |O(n) comparisons, O(1) swaps|
|Average case performance            |O(n2) comparisons, swaps|
|Worst case space complexity         |O(n) total, O(1)|

##Introsort
Introsort is a hybrid sorting algorithm that uses Quick Sort to a recursion depth of O(log n) and then switches to a Heap Sort. Heap Sort is implemented using the standard C++ functions `make_heap` and `sort_heap`, both of which require Random Access data iterators. For maximum flexibility, this implementation of Introsort does not switch to Heap Sort if the data iterators are not Random Access, and can therefore operator on container with any iterator category.

|http://en.wikipedia.org/wiki/Introsort|unstable|
|----|---|
|Worst case performance              |O(n log n)|
|Average case performance            |O(n log n)|

###Merge Sort
A divide and conquer recursive sorting algorithm which respects the original ordering of elements in the data set with identical keys. Two merge sort algorithms are included; 'merge_sort' an inplace sort that sorts a data set within itself, and a 'merge_sort_copy' which produces a secondary data set conatining the sorted result and leave the original unchanged.

|http://en.wikipedia.org/wiki/Mergesort|stable|
|----|---
|Worst case performance              |O(n log n)
|Best case performance               |O(n log n) typical, O(n) natural variant
|Average case performance            |O(n log n)
|Worst case space complexity         |O(n) auxiliary

###Min Max Sort
An adapted Selection Sort that finds and repositions the Min and Max elements on each pass.

||unstable|
|----|---
|Worst case performance              |O(n^2)
|Best case performance               |O(n)
|Average case performance            |O(n^2)
|Space                               |O(1)

###Quick Sort
A recursive sorting algorithm that works well with a cache, but is unstable; that is elements with identical keys may appear in a different order in the result than they do in the original data set.

|http://en.wikipedia.org/wiki/Quicksort|unstable|
|----|---
|Worst case performance              |O(n2) (extremely rare)
|Best case performance               |O(n log n)
|Average case performance            |O(n log n)
|Worst case space complexity         |O(n) auxiliary (naive) O(log n) auxiliary (Sedgewick 1978)

###Selection Sort
An inplace comparison sorting algorithm that is slow for large data sets, but it is a simple algorithm and has reasonable performance for inplace algorithms.

|http://en.wikipedia.org/wiki/Selection_sort|stable|
|----|---
|Worst case performance              |O(n^2)
|Best case performance               |O(n^2)
|Average case performance            |O(n^2)
|Space                               |O(1)

#include "sort.h"

namespace cdmh {

// Heap Sort
//     Worst case performance       O(n log n)
//     Best case performance        Omega(n), O(n log n)
//     Average case performance	    O(n log n)
//     Worst case space complexity  O(n) total, O(1) auxiliary
// http://en.wikipedia.org/wiki/Heapsort

template<typename It, typename Pred>
void heap_sort(It begin, It end, Pred pred)
{
    std::make_heap(begin, end, pred);
    std::sort_heap(begin, end, pred);
}

template<typename C, typename Pred=std::less, bool isContainer=detail::is_container<C>::value>
void heap_sort(C &container, Pred pred)
{
    static_assert(isContainer, "'container' must be a container");
    heap_sort(begin(container), end(container), pred);
}

}   // namespace cdmh

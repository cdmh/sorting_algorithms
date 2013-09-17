#include "sort.h"

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


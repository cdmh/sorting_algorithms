#include "sort.h"

namespace cdmh {

// Insertion Sort
//    Worst case performance            О(n2) comparisons, swaps
//    Best case performance             O(n) comparisons, O(1) swaps
//    Average case performance          О(n2) comparisons, swaps
//    Worst case space complexity       О(n) total, O(1)
// http://en.wikipedia.org/wiki/Insertion_sort

template<typename It, typename Pred>
void insertion_sort(It begin, It end, Pred pred)
{
    if (begin == end)
        return;

    for (auto it=begin; ++it != end; )
    {
        auto it2 = it;
        auto value = std::move(*it);

        if (pred(value, *begin))
        {
            std::move_backward(begin, it, ++it2);
            *begin = std::move(value);
        }
        else
        {
            for (auto first = it2; pred(value, *--first); it2=first)
                *it2 = std::move(*first);
            *it2 = std::move(value);
        }
    }
}

template<typename C, typename Pred=std::less, bool=detail::is_container<C>::value>
void insertion_sort(C &container, Pred pred)
{
    insertion_sort(begin(container), end(container), pred);
}

}   // namespace cdmh

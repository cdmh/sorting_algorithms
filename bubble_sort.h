#include "sort.h"

namespace cdmh {

// Bubble Sort
//     Worst case performance   O(n^2)
//     Best case performance    O(n)
//     Average case performance O(n^2)
//     Space                    inplace
// http://en.wikipedia.org/wiki/Bubble_sort

template<typename It, typename Pred=std::less>
void bubble_sort(It begin, It end, Pred pred)
{
    if (std::distance(begin, end) <= 1)
        return;
    
    bool finished = false;
    auto ite = detail::advance(end, -1);
    while (!finished)
    {
        finished = true;
        for (auto it=begin; it!=ite; ++it)
        {
            auto next = detail::advance(it, 1);
            if (pred(*next, *it))
            {
                std::swap(*it, *next);
                finished = false;
            }
            detail::advance(ite, -1);
        }
    }
}

template<typename C, typename Pred=std::less, bool isContainer=detail::is_container<C>::value>
void bubble_sort(C &container, Pred pred)
{
    bubble_sort(container.begin(), container.end(), pred);
}

}   // namespace cdmh

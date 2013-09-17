#include "sort.h"

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

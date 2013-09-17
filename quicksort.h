#include "sort.h"

namespace cdmh {

namespace detail {

template <typename It, typename Distance>
It advance(It it, Distance n)
{
    std::advance(it, n);
    return it;
}

template<typename T>
T median(T const &t1, T const &t2, T const &t3)
{
    if (t1 < t2)
    {
        if (t2 < t3)
            return t2;
        else if (t1 < t3)
            return t3;
        else
            return t1;
    }
    else if (t1 < t3)
        return t1;
    else if (t2 < t3)
        return t3;
    else
        return t2;
}

}   // namespace detail

template<typename It, typename Pred=std::less>
void quicksort(It begin, It end, Pred pred)
{
    auto const size = std::distance(begin, end);
    if (size <= 1)
        return;

    using value_t = typename std::iterator_traits<It>::value_type;
    using namespace std::placeholders;

    auto const not_pred = [&pred](value_t const &first, value_t const &second) { return !pred(second, first); };
    auto const pivot    = detail::median(*begin, *detail::advance(begin, size / 2), *detail::advance(end, -1));
    It   const split1   = std::partition(begin, end, std::bind(pred, _1, pivot));
    It   const split2   = std::partition(split1, end, std::bind(not_pred, _1, pivot));
    quicksort(begin, split1, pred);
    quicksort(split2, end, pred);
}

template<typename C, typename Pred=std::less, bool isContainer=detail::is_container<C>::value>
void quicksort(C &container, Pred pred)
{
    quicksort(container.begin(), container.end(), pred);
}

}   // namespace cdmh

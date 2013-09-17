#pragma once

#include <list>
#include <deque>
#include <vector>

namespace cdmh {
namespace detail {

template <typename Container>
struct is_container : std::false_type { };

template <typename... Ts> struct is_container<std::list<Ts...> >   : std::true_type { };
template <typename... Ts> struct is_container<std::deque<Ts...> >  : std::true_type { };
template <typename... Ts> struct is_container<std::vector<Ts...> > : std::true_type { };

template <typename It, typename Distance>
It advance(It it, Distance n)
{
    std::advance(it, n);
    return it;
}

}   // namespace detail
}   // namespace cdmh

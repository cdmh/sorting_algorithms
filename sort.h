#pragma once

#include <list>
#include <deque>
#include <vector>
namespace detail {

template <typename Container>
struct is_container : std::false_type { };

template <typename... Ts> struct is_container<std::list<Ts...> >   : std::true_type { };
template <typename... Ts> struct is_container<std::deque<Ts...> > : std::true_type { };
template <typename... Ts> struct is_container<std::vector<Ts...> > : std::true_type { };

}   // namespace detail

// Copyright (c) 2013 Craig Henderson
// https://github.com/cdmh/sorting_algorithms
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

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
inline It advance(It it, Distance n)
{
    std::advance(it, n);
    return it;
}

template<typename T>
inline T median(T const &t1, T const &t2, T const &t3)
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
}   // namespace cdmh

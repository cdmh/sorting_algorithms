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

#include "sort.h"

namespace cdmh {

namespace detail {

template<typename It, typename Pred>
std::pair<It,It> quicksort_splits(It begin, It end, Pred pred)
{
    using value_t = typename std::iterator_traits<It>::value_type;
    using namespace std::placeholders;

    auto const not_pred = [&pred](value_t const &first, value_t const &second) { return !pred(second, first); };
    auto const pivot    = detail::median(*begin, *detail::advance(begin, std::distance(begin, end) / 2), *detail::advance(end, -1));
    auto const split    = std::partition(begin, end, std::bind(pred, _1, pivot));
    return std::make_pair(split, std::partition(split, end, std::bind(not_pred, _1, pivot)));
}

}   // namespace detail {

// Quick Sort
//     Worst case performance       O(n2) (extremely rare)
//     Best case performance        O(n log n)
//     Average case performance     O(n log n)
//     Worst case space complexity  O(n) auxiliary (naive) O(log n) auxiliary (Sedgewick 1978)
// http://en.wikipedia.org/wiki/Quicksort

template<typename It, typename Pred=std::less>
void quicksort(It begin, It end, Pred pred)
{
    if (std::distance(begin, end) > 1)
    {
        auto splits = detail::quicksort_splits(begin, end, pred);
        quicksort(begin, splits.first, pred);
        quicksort(splits.second, end, pred);
    }
}

template<typename C, typename Pred=std::less, bool isContainer=detail::is_container<C>::value>
void quicksort(C &container, Pred pred)
{
    quicksort(container.begin(), container.end(), pred);
}

}   // namespace cdmh

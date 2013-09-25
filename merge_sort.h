// Copyright (c) 2013 Craig Henderson
// https://github.com/cdmh/sorting_algorithms

#include "sort.h"
#include <cassert>
#include <algorithm>        // inplace_merge

namespace cdmh {

// Merge Sort
//  Worst case performance      O(n log n)
//  Best case performance       O(n log n) typical, O(n) natural variant
//  Average case performance    O(n log n)
//  Worst case space complexity O(n) auxiliary
// http://en.wikipedia.org/wiki/Mergesort

// inplace merge sort
template<typename It, typename Pred>
void merge_sort(It begin, It end, Pred pred)
{
    auto const size = std::distance(begin, end);
    if (size <= 1)
        return;

    It middle = begin;
    std::advance(middle, size / 2);

    merge_sort(begin, middle, pred);
    merge_sort(middle, end, pred);
    std::inplace_merge(begin, middle, end, pred);
}

// inplace merge sort
template<typename C, typename Pred=std::less<typename C::value_type>, bool isContainer=detail::is_container<C>::value>
void merge_sort(C &container, Pred pred)
{
    static_assert(isContainer, "'container' must be a container");
    merge_sort(begin(container), end(container), pred);
}

// merge sort to non-overlapping output iterator
template<typename It, typename Out, typename Pred>
void merge_sort(It begin, It end, Out result, Pred pred)
{
    auto const size = std::distance(begin, end);
    if (size == 0)
        return;
    else if (size == 1)
    {
        *result++ = std::move(*begin);
        return;
    }

    It middle = begin;
    std::advance(middle, size / 2);

    std::vector<typename std::iterator_traits<It>::value_type> left, right;
    merge_sort(begin, middle, std::back_inserter(left), pred);
    merge_sort(middle, end, std::back_inserter(right), pred);
    std::merge(left.begin(), left.end(), right.begin(), right.end(), result, pred);
}

// merge sort to secondary container
template<typename C1, typename C2, typename Pred=std::less<typename C1::value_type>, bool isContainer=detail::is_container<C1>::value && detail::is_container<C2>::value, typename=std::enable_if<std::is_same<typename C1::value_type, typename C2::value_type>::value>>
void merge_sort(C1 const &container, C2 &result, Pred pred)
{
    assert(result.empty());
    static_assert(isContainer, "'container' and 'result' must be containers");
    merge_sort(begin(container), end(container), std::back_inserter(result), pred);
}

}   // namespace cdmh

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

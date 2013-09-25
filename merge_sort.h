// Copyright (c) 2013 Craig Henderson
// https://github.com/cdmh/sorting_algorithms

#include "sort.h"
#include <cassert>
#include <vector>
#include <algorithm>        // inplace_merge

namespace cdmh {

// Merge Sort
//  Worst case performance      O(n log n)
//  Best case performance       O(n log n) typical, O(n) natural variant
//  Average case performance    O(n log n)
//  Worst case space complexity O(n) auxiliary
// http://en.wikipedia.org/wiki/Mergesort

// inplace merge sort
template<typename It, typename Pred=std::less<typename std::iterator_traits<It>::value_type>>
void merge_sort(It begin, It end, Pred pred=Pred())
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

// merge sort to non-overlapping output iterator
template<typename It, typename Out, typename Pred=std::less<typename std::iterator_traits<It>::value_type>>
void merge_sort_copy(It begin, It end, Out result, Pred pred=Pred())
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
    merge_sort_copy(begin, middle, std::back_inserter(left), pred);
    merge_sort_copy(middle, end, std::back_inserter(right), pred);
    std::merge(left.begin(), left.end(), right.begin(), right.end(), result, pred);
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

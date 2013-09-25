// Copyright (c) 2013 Craig Henderson
// https://github.com/cdmh/sorting_algorithms

#pragma once

#include "sort.h"

namespace cdmh {

// Heap Sort
//     Worst case performance       O(n log n)
//     Best case performance        Omega(n), O(n log n)
//     Average case performance	    O(n log n)
//     Worst case space complexity  O(n) total, O(1) auxiliary
// http://en.wikipedia.org/wiki/Heapsort

template<typename It, typename Pred>
void heap_sort(It begin, It end, Pred pred)
{
    std::make_heap(begin, end, pred);
    std::sort_heap(begin, end, pred);
}

template<typename C, typename Pred=std::less<typename C::value_type>, bool isContainer=detail::is_container<C>::value>
void heap_sort(C &container, Pred pred)
{
    static_assert(isContainer, "'container' must be a container");
    heap_sort(begin(container), end(container), pred);
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

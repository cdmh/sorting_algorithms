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

#include "sort.h"

namespace cdmh {

// Selection Sort
//      Worst case performance      О(n^2)
//      Best case performance       О(n^2)
//      Average case performance    О(n^2)
//      Worst case space complexity О(n) total, O(1) auxiliary
// http://en.wikipedia.org/wiki/Selection_sort

template<typename It, typename Pred=std::less<typename std::iterator_traits<It>::value_type>>
void selection_sort(It begin, It end, Pred pred)
{
    for (auto it=begin; it!= end; ++it)
        std::swap(*std::min_element(it, end, pred), *it);
}

template<typename C, typename Pred=std::less<typename C::value_type>, bool isContainer=detail::is_container<C>::value>
void selection_sort(C &container, Pred pred)
{
    selection_sort(container.begin(), container.end(), pred);
}

}   // namespace cdmh

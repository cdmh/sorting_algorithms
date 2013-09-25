// Copyright (c) 2013 Craig Henderson
// https://github.com/cdmh/sorting_algorithms

#include "sort.h"

namespace cdmh {

// Bubble Sort
//     Worst case performance       O(n^2)
//     Best case performance        O(n)
//     Average case performance     O(n^2)
//     Wost case space complexity   O(1)
// http://en.wikipedia.org/wiki/Bubble_sort

template<typename It, typename Pred=std::less<typename std::iterator_traits<It>::value_type>>
void bubble_sort(It begin, It end, Pred pred)
{
    if (std::distance(begin, end) <= 1)
        return;
    
    auto ite      = end;
    bool finished = false;
    while (!finished)
    {
        finished = true;
        std::advance(ite, -1);
        for (auto it=begin; it!=ite; ++it)
        {
            auto next = detail::advance(it, 1);
            if (pred(*next, *it))
            {
                std::swap(*it, *next);
                finished = false;
            }
        }
    }
}

template<typename C, typename Pred=std::less<typename C::value_type>, bool isContainer=detail::is_container<C>::value>
void bubble_sort(C &container, Pred pred)
{
    bubble_sort(container.begin(), container.end(), pred);
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

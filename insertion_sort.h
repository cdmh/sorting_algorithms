// Copyright (c) 2013 Craig Henderson
// https://github.com/cdmh/sorting_algorithms

#include "sort.h"

namespace cdmh {

// Insertion Sort
//    Worst case performance            О(n2) comparisons, swaps
//    Best case performance             O(n) comparisons, O(1) swaps
//    Average case performance          О(n2) comparisons, swaps
//    Worst case space complexity       О(n) total, O(1)
// http://en.wikipedia.org/wiki/Insertion_sort

template<typename It, typename Pred=std::less<typename std::iterator_traits<It>::value_type>>
inline void insertion_sort(It begin, It end, Pred pred=Pred())
{
    if (begin == end)
        return;

    for (auto it=begin; ++it != end; )
    {
        auto it2 = it;
        auto value = std::move(*it);

        if (pred(value, *begin))
        {
            std::move_backward(begin, it, ++it2);
            *begin = std::move(value);
        }
        else
        {
            for (auto first = it2; pred(value, *--first); it2=first)
                *it2 = std::move(*first);
            *it2 = std::move(value);
        }
    }
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

// Copyright (c) 2013 Craig Henderson
// https://github.com/cdmh/sorting_algorithms

#include "sort.h"
#include "quicksort.detail.h"

namespace cdmh {

// Quick Sort
//     Worst case performance       O(n2) (extremely rare)
//     Best case performance        O(n log n)
//     Average case performance     O(n log n)
//     Worst case space complexity  O(n) auxiliary (naive) O(log n) auxiliary (Sedgewick 1978)
// http://en.wikipedia.org/wiki/Quicksort

template<typename It, typename Pred=std::less<typename std::iterator_traits<It>::value_type>>
void quicksort(It begin, It end, Pred pred=Pred())
{
    if (std::distance(begin, end) > 1)
    {
        auto splits = detail::quicksort_splits(begin, end, pred);
        quicksort(begin, splits.first, pred);
        quicksort(splits.second, end, pred);
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

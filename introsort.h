// Copyright (c) 2013 Craig Henderson
// https://github.com/cdmh/sorting_algorithms

#include "sort.h"
#include "quicksort.h"
#include "heap_sort.h"
#include <cmath>    // floor, log

namespace cdmh {

namespace detail {

// the heap sort uses std::make_heap and std::sort_heap, both of which only work with random access iterators.
// this functor allows us to use heap_sort or not depending on the iterator type
template<typename It, bool IsRandomIterator=std::is_same<typename std::iterator_traits<It>::iterator_category, std::random_access_iterator_tag>::value>
struct heap_sort_if_we_can
{
    template<typename... Args>
    bool operator()(Args...)
    {
        return false;
    }
};

template<typename It>
struct heap_sort_if_we_can<It, true>
{
    template<typename Pred>
    bool operator()(It begin, It end, Pred pred)
    {
        heap_sort(begin, end, pred);
        return true;
    }
};


// detail introsort implementation takes a depth count
template<typename It, typename Pred>
void introsort(It begin, It end, Pred pred, size_t depth)
{
    auto const size = std::distance(begin, end);
    if (size <= 1)
        return;

    // if we've reached the depth limit, then attempt a heap sort.
    // if heap sort is not available to us, then continue with quicksort algorithm
    if (--depth == 0  &&  heap_sort_if_we_can<It>()(begin, end, pred))
        return;

    auto splits = detail::quicksort_splits(begin, end, pred);
    introsort(begin, splits.first, pred, depth);
    introsort(splits.second, end, pred, depth);
}

}   // namespace detail

// Introsort
//     Worst case performance       O(n log n)
//     Average case performance     O(n log n)
// http://en.wikipedia.org/wiki/Introsort

template<typename It, typename Pred=std::less<typename std::iterator_traits<It>::value_type>>
void introsort(It begin, It end, Pred pred)
{
    if (std::distance(begin, end) > 1)
        detail::introsort(begin, end, pred, (size_t)floor(log(std::distance(begin,end)/log(2.0))));
}

template<typename C, typename Pred=std::less<typename C::value_type>, bool isContainer=detail::is_container<C>::value>
void introsort(C &container, Pred pred)
{
    introsort(container.begin(), container.end(), pred);
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

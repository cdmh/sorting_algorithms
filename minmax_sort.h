// Copyright (c) 2013 Craig Henderson
// https://github.com/cdmh/sorting_algorithms

#include "sort.h"

namespace cdmh {

// MinMax Sort

template<typename It, typename Pred=std::less<typename std::iterator_traits<It>::value_type>>
inline void minmax_sort(It begin, It end, Pred pred=Pred())
{
    auto count = std::distance(begin, end);
    for (auto it=begin, ite=end; count > 1; ++it, count-=2)
    {
        auto minmax = std::minmax_element(it, ite, pred);
        std::advance(ite,-1);

        if (minmax.second == it  &&  minmax.first == ite)
            std::swap(*it, *ite);
        else if (minmax.second == it)
        {
            std::swap(*minmax.second, *ite);
            std::swap(*minmax.first, *it);
        }
        else
        {
            std::swap(*minmax.first, *it);
            std::swap(*minmax.second, *ite);
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

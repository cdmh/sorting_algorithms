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

#include "insertion_sort.h"
#include "merge_sort.h"
#include "heap_sort.h"
#include "quicksort.h"
#include "bubble_sort.h"

#include <algorithm>    // is_sorted
#include <functional>   // greater
#include <vector>
#include <list>
#include <deque>
#include <cassert>
#include <string>
#include <iostream>

template<typename Sort, typename It, typename Pred>
void run_iterator_sort(It it, It ite, Sort sort, Pred pred)
{
    sort(it, ite, pred);
    std::cout << "--> ";
    for (auto each=it; each!=ite; ++each)
        std::cout << *each << " ";
    std::cout << '\n';
    assert(std::is_sorted(it, ite, pred));
}

template<typename Sort, typename C, typename Pred>
void run_container_inplace_sort(Sort sort, C container, Pred pred)
{
    sort(container, pred);
    std::cout << "--> ";
    for (auto each : container)
        std::cout << each << " ";
    std::cout << '\n';
    assert(std::is_sorted(begin(container), end(container), pred));
}

template<typename Sort, typename C, typename Pred>
void run_container_to_container_sort(Sort sort, C container, Pred pred)
{
    C result;
    sort(container, result, pred);

    std::cout << "--> ";
    for (auto each : result)
        std::cout << each << " ";
    std::cout << '\n';

    assert(result.size() == container.size());
    assert(std::is_sorted(begin(result), end(result), pred));
}

template<typename C, typename Pred=std::less<typename C::value_type>>
void test_sorts(C container, Pred pred=Pred())
{
    // we have to specify all the template parameters because otherwise there
    // are name conflicts without the function parameters being specified. In
    // normal usage, the template parameters are deduced automatically;
    //     cdmh::insertion_sort(container, pred);
    //     cdmh::insertion_sort(container.begin(), container.end(), std::greater<typename C::value_type>());
    //     cdmh::merge_sort(container, pred);
    //     ... etc

    // std::sort requires random access iterators, so list & deque are not supported
    std::cout << "std::sort " << container.size() << " elements\n";
    [&pred](std::vector<typename C::value_type> container) {
        run_iterator_sort(container.begin(), container.end(), std::sort<std::vector<typename C::value_type>::iterator, decltype(pred)>, pred);
    }(std::vector<typename C::value_type>(container.begin(), container.end()));

    // std::stable_sort requires random access iterators, so list & deque are not supported
    std::cout << "std::stable_sort " << container.size() << " elements\n";
    [&pred](std::vector<typename C::value_type> container) {
        run_iterator_sort(container.begin(), container.end(), std::stable_sort<std::vector<typename C::value_type>::iterator, decltype(pred)>, pred);
    }(std::vector<typename C::value_type>(container.begin(), container.end()));

    // the heap sort currently uses std::make_heap and std::sort_heap which both require
    // a random access iterator, so only vector is tested here
    std::cout << "Heap Sort " << container.size() << " elements\n";
    run_container_inplace_sort(cdmh::heap_sort<C, Pred, true>, container, pred);

    std::cout << "Merge Sort " << container.size() << " elements\n";
    run_container_to_container_sort(cdmh::merge_sort<std::vector<typename C::value_type>, std::vector<typename C::value_type>, Pred, true>, std::vector<typename C::value_type>(container.begin(), container.end()), pred);
    run_container_to_container_sort(cdmh::merge_sort<std::list  <typename C::value_type>, std::list  <typename C::value_type>, Pred, true>, std::list  <typename C::value_type>(container.begin(), container.end()), pred);
    run_container_to_container_sort(cdmh::merge_sort<std::deque <typename C::value_type>, std::deque <typename C::value_type>, Pred, true>, std::deque <typename C::value_type>(container.begin(), container.end()), pred);

    std::cout << "Inplace Merge Sort " << container.size() << " elements\n";
    run_container_inplace_sort(cdmh::merge_sort<std::vector<typename C::value_type>, Pred, true>, std::vector<typename C::value_type>(container.begin(), container.end()), pred);
    run_container_inplace_sort(cdmh::merge_sort<std::list  <typename C::value_type>, Pred, true>, std::list  <typename C::value_type>(container.begin(), container.end()), pred);
    run_container_inplace_sort(cdmh::merge_sort<std::deque <typename C::value_type>, Pred, true>, std::deque <typename C::value_type>(container.begin(), container.end()), pred);

    std::cout << "Insertion Sort " << container.size() << " elements\n";
    run_container_inplace_sort(cdmh::insertion_sort<std::vector<typename C::value_type>, Pred, true>, std::vector<typename C::value_type>(container.begin(), container.end()), pred);
    run_container_inplace_sort(cdmh::insertion_sort<std::list  <typename C::value_type>, Pred, true>, std::list  <typename C::value_type>(container.begin(), container.end()), pred);
    run_container_inplace_sort(cdmh::insertion_sort<std::deque <typename C::value_type>, Pred, true>, std::deque <typename C::value_type>(container.begin(), container.end()), pred);

    std::cout << "Quicksort " << container.size() << " elements \n";
    run_container_inplace_sort(cdmh::quicksort<std::vector<typename C::value_type>, Pred, true>, std::vector<typename C::value_type>(container.begin(), container.end()), pred);
    run_container_inplace_sort(cdmh::quicksort<std::list  <typename C::value_type>, Pred, true>, std::list  <typename C::value_type>(container.begin(), container.end()), pred);
    run_container_inplace_sort(cdmh::quicksort<std::deque <typename C::value_type>, Pred, true>, std::deque <typename C::value_type>(container.begin(), container.end()), pred);

    std::cout << "Bubble Sort " << container.size() << " elements \n";
    run_container_inplace_sort(cdmh::bubble_sort<std::vector<typename C::value_type>, Pred, true>, std::vector<typename C::value_type>(container.begin(), container.end()), pred);
    run_container_inplace_sort(cdmh::bubble_sort<std::list  <typename C::value_type>, Pred, true>, std::list  <typename C::value_type>(container.begin(), container.end()), pred);
    run_container_inplace_sort(cdmh::bubble_sort<std::deque <typename C::value_type>, Pred, true>, std::deque <typename C::value_type>(container.begin(), container.end()), pred);
}

template<typename C>
void test_all_sorts(C container)
{
    // test sort ascending
    test_sorts(container, std::less<typename C::value_type>());

    // test sort descending
    test_sorts(container, std::greater<typename C::value_type>());
}

int main()
{
    test_all_sorts(std::vector<std::string>{ "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog" });
    test_all_sorts(std::vector<std::string>{ "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog" });
    test_all_sorts(std::vector<int>{});
    test_all_sorts(std::vector<int>{ 3 });
    test_all_sorts(std::vector<int>{ 3, 4, 5, 9, 8, 2, 1, 7, 6 });
    test_all_sorts(std::vector<int>{ 60, 10, 410, 20, 40, 50, 60, 10, 40, 30, 40, 50, 60, 10, 40, 50, 6 });
    test_all_sorts(std::vector<int>{ 10, 20, 30, 40, 510, 20, 50, 60, 10, 60, 130, 10, 20, 30, 40, 50, 60, 140, 10, 20, 30, 40, 50, 60, 1 });

    // shows stability of the sort by comparing doubles as ints
    test_sorts(
        std::vector<double>{ 1.2, 1.1, 0.4, 0.1, 0.9, 3.1, 3.6, 9.4, 9.8, 9.6, 3.2 },
        [](double const &first, double const &second) {
            return int(first)<int(second);
        });

    std::vector<int> v{ 10, 20, 30, 40, 510, 20, 50, 60, 10, 60, 130, 10, 20, 30, 40, 50, 60, 140, 10, 20, 30, 40, 50, 60, 1};
    std::sort(v.begin(), v.end());
}

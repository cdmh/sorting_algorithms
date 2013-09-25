// Copyright (c) 2013 Craig Henderson
// https://github.com/cdmh/sorting_algorithms

#include "bubble_sort.h"
#include "cocktail_sort.h"
#include "heap_sort.h"
#include "insertion_sort.h"
#include "introsort.h"
#include "merge_sort.h"
#include "minmax_sort.h"
#include "quicksort.h"
#include "selection_sort.h"

#include <algorithm>    // is_sorted
#include <functional>   // greater
#include <cassert>
#include <string>
#include <list>
#include <deque>
#include <iostream>

namespace cdmh {
namespace test {

// test the calling interface for each sort algorithm
void test_interfaces(void)
{
    std::vector<int> v;
    for (auto i=0; i<1000; ++i)
        v.push_back(i);

    std::random_shuffle(v.begin(), v.end());
    cdmh::bubble_sort(v.begin(), v.end());
    assert(std::is_sorted(v.begin(), v.end()));
    cdmh::bubble_sort(v.begin(), v.end(), std::greater<int>());
    assert(std::is_sorted(v.begin(), v.end(), std::greater<int>()));

    std::random_shuffle(v.begin(), v.end());
    cdmh::cocktail_sort(v.begin(), v.end());
    assert(std::is_sorted(v.begin(), v.end()));
    cdmh::cocktail_sort(v.begin(), v.end(), std::greater<int>());
    assert(std::is_sorted(v.begin(), v.end(), std::greater<int>()));

    std::random_shuffle(v.begin(), v.end());
    cdmh::heap_sort(v.begin(), v.end());
    assert(std::is_sorted(v.begin(), v.end()));
    cdmh::heap_sort(v.begin(), v.end(), std::greater<int>());
    assert(std::is_sorted(v.begin(), v.end(), std::greater<int>()));

    std::random_shuffle(v.begin(), v.end());
    cdmh::insertion_sort(v.begin(), v.end());
    assert(std::is_sorted(v.begin(), v.end()));
    cdmh::insertion_sort(v.begin(), v.end(), std::greater<int>());
    assert(std::is_sorted(v.begin(), v.end(), std::greater<int>()));

    std::random_shuffle(v.begin(), v.end());
    cdmh::introsort(v.begin(), v.end());
    assert(std::is_sorted(v.begin(), v.end()));
    cdmh::introsort(v.begin(), v.end(), std::greater<int>());
    assert(std::is_sorted(v.begin(), v.end(), std::greater<int>()));

    std::random_shuffle(v.begin(), v.end());
    cdmh::merge_sort(v.begin(), v.end());
    assert(std::is_sorted(v.begin(), v.end()));
    cdmh::merge_sort(v.begin(), v.end(), std::greater<int>());
    assert(std::is_sorted(v.begin(), v.end(), std::greater<int>()));

    std::vector<int> result;
    std::random_shuffle(v.begin(), v.end());
    cdmh::merge_sort_copy(v.begin(), v.end(), std::back_inserter(result));
    assert(std::is_sorted(result.begin(), result.end()));
    result.clear();
    cdmh::merge_sort_copy(v.begin(), v.end(), std::back_inserter(result), std::greater<int>());
    assert(std::is_sorted(result.begin(), result.end(), std::greater<int>()));

    std::random_shuffle(v.begin(), v.end());
    cdmh::minmax_sort(v.begin(), v.end());
    assert(std::is_sorted(v.begin(), v.end()));
    cdmh::minmax_sort(v.begin(), v.end(), std::greater<int>());
    assert(std::is_sorted(v.begin(), v.end(), std::greater<int>()));

    std::random_shuffle(v.begin(), v.end());
    cdmh::quicksort(v.begin(), v.end());
    assert(std::is_sorted(v.begin(), v.end()));
    cdmh::quicksort(v.begin(), v.end(), std::greater<int>());
    assert(std::is_sorted(v.begin(), v.end(), std::greater<int>()));

    std::random_shuffle(v.begin(), v.end());
    cdmh::selection_sort(v.begin(), v.end());
    assert(std::is_sorted(v.begin(), v.end()));
    cdmh::selection_sort(v.begin(), v.end(), std::greater<int>());
    assert(std::is_sorted(v.begin(), v.end(), std::greater<int>()));
}

template<typename Sort, typename It, typename Pred>
void sort_between_iterators(It it, It ite, Sort sort, Pred pred)
{
    sort(it, ite, pred);
    std::cout << "--> ";
    for (auto each=it; each!=ite; ++each)
        std::cout << *each << " ";
    std::cout << '\n';
    assert(std::is_sorted(it, ite, pred));
}

template<typename Sort, typename C, typename Pred>
void sort_container(Sort sort, C container, Pred pred)
{
    sort(container.begin(), container.end(), pred);
    std::cout << "--> ";
    for (auto each : container)
        std::cout << each << " ";
    std::cout << '\n';
    assert(std::is_sorted(begin(container), end(container), pred));
}

template<typename Sort, typename C, typename Pred>
void sort_container_copy(Sort sort, C container, Pred pred)
{
    C result;
    sort(container.begin(), container.end(), std::back_inserter(result), pred);

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
        sort_between_iterators(
            container.begin(),
            container.end(),
            std::sort<
                typename std::vector<typename C::value_type>::iterator,
                decltype(pred)>,
            pred);
    }(std::vector<typename C::value_type>(container.begin(), container.end()));

    // std::stable_sort requires random access iterators, so list & deque are not supported
    std::cout << "std::stable_sort " << container.size() << " elements\n";
    [&pred](std::vector<typename C::value_type> container) {
        sort_between_iterators(
            container.begin(),
            container.end(),
            std::stable_sort<
                typename std::vector<typename C::value_type>::iterator,
                decltype(pred)>,
            pred);
    }(std::vector<typename C::value_type>(container.begin(), container.end()));

    std::cout << "Bubble Sort " << container.size() << " elements \n";
    sort_container(
        cdmh::bubble_sort<std::vector<typename C::value_type>::iterator, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::bubble_sort<std::list<typename C::value_type>::iterator, Pred>,
        std::list<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::bubble_sort<std::deque<typename C::value_type>::iterator, Pred>,
        std::deque<typename C::value_type>(container.begin(), container.end()),
        pred);

    std::cout << "Cocktail Sort " << container.size() << " elements \n";
    sort_container(
        cdmh::cocktail_sort<std::vector<typename C::value_type>::iterator, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::cocktail_sort<std::list<typename C::value_type>::iterator, Pred>,
        std::list<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::cocktail_sort<std::deque<typename C::value_type>::iterator, Pred>,
        std::deque<typename C::value_type>(container.begin(), container.end()),
        pred);

    // the heap sort currently uses std::make_heap and std::sort_heap which both require
    // a random access iterator, so only vector is tested here
    std::cout << "Heap Sort " << container.size() << " elements\n";
    sort_container(
        cdmh::heap_sort<std::vector<typename C::value_type>::iterator, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);

    std::cout << "Insertion Sort " << container.size() << " elements\n";
    sort_container(
        cdmh::insertion_sort<std::vector<typename C::value_type>::iterator, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::insertion_sort<std::list<typename C::value_type>::iterator, Pred>,
        std::list<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::insertion_sort<std::deque<typename C::value_type>::iterator, Pred>,
        std::deque<typename C::value_type>(container.begin(), container.end()),
        pred);

    std::cout << "Introsort " << container.size() << " elements \n";
    sort_container(
        cdmh::introsort<std::vector<typename C::value_type>::iterator, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::introsort<std::list<typename C::value_type>::iterator, Pred>,
        std::list<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::introsort<std::deque<typename C::value_type>::iterator, Pred>,
        std::deque<typename C::value_type>(container.begin(), container.end()),
        pred);

    std::cout << "Merge Sort (copy)" << container.size() << " elements\n";
    sort_container_copy(
        cdmh::merge_sort_copy<
            std::vector<typename C::value_type>::iterator,
            std::back_insert_iterator<std::vector<typename C::value_type>>, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container_copy(
        cdmh::merge_sort_copy<
            std::list<typename C::value_type>::iterator,
            std::back_insert_iterator<std::list<typename C::value_type>>, Pred>,
        std::list<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container_copy(
        cdmh::merge_sort_copy<
            std::deque<typename C::value_type>::iterator,
            std::back_insert_iterator<std::deque<typename C::value_type>>, Pred>,
        std::deque<typename C::value_type>(container.begin(), container.end()),
        pred);

    std::cout << "Merge Sort (inplace)" << container.size() << " elements\n";
    sort_container(
        cdmh::merge_sort<std::vector<typename C::value_type>::iterator, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::merge_sort<std::list<typename C::value_type>::iterator, Pred>,
        std::list<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::merge_sort<std::deque<typename C::value_type>::iterator, Pred>,
        std::deque<typename C::value_type>(container.begin(), container.end()),
        pred);

    std::cout << "MinMax sort " << container.size() << " elements \n";
    sort_container(
        cdmh::minmax_sort<std::vector<typename C::value_type>::iterator, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::minmax_sort<std::list<typename C::value_type>::iterator, Pred>,
        std::list<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::minmax_sort<std::deque<typename C::value_type>::iterator, Pred>,
        std::deque<typename C::value_type>(container.begin(), container.end()),
        pred);

    std::cout << "Quicksort " << container.size() << " elements \n";
    sort_container(
        cdmh::quicksort<std::vector<typename C::value_type>::iterator, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::quicksort<std::list<typename C::value_type>::iterator, Pred>,
        std::list<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::quicksort<std::deque<typename C::value_type>::iterator, Pred>,
        std::deque<typename C::value_type>(container.begin(), container.end()),
        pred);

    std::cout << "Selection sort " << container.size() << " elements \n";
    sort_container(
        cdmh::selection_sort<std::vector<typename C::value_type>::iterator, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::selection_sort<std::list<typename C::value_type>::iterator, Pred>,
        std::list<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::selection_sort<std::deque<typename C::value_type>::iterator, Pred>,
        std::deque<typename C::value_type>(container.begin(), container.end()),
        pred);
}

template<typename C>
void test_all_sorts(C container)
{
    // test sort ascending
    test_sorts(container, std::less<typename C::value_type>());

    // test sort descending
    test_sorts(container, std::greater<typename C::value_type>());
}

}   // namespace test
}   // namespace cdmh

int main()
{
    using cdmh::test::test_sorts;
    using cdmh::test::test_all_sorts;

    cdmh::test::test_interfaces();
    test_all_sorts(std::vector<int>{ 1, 2, 3, 4, 51, 2, 5, 6, 1, 6, 13, 1, 2, 3, 4, 5, 6, 14, 1, 2, 3, 4, 5, 6, 1 });
    test_all_sorts(std::vector<std::string>{ "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog" });
    test_all_sorts(std::vector<std::string>{ "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog" });
    test_all_sorts(std::vector<int>{});
    test_all_sorts(std::vector<int>{ 3 });
    test_all_sorts(std::vector<int>{ 3, 4, 5, 9, 8, 2, 1, 7, 6 });
    test_all_sorts(std::vector<int>{ 60, 10, 410, 40, 50, 60, 10, 40, 30, 40, 50, 60, 10, 40, 50, 6 });
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

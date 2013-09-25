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

namespace utils {

template<typename C, typename Pred>
C sorted(C container, Pred pred)
{
    std::vector<typename C::value_type> vector(container.begin(), container.end());
    std::sort(vector.begin(), vector.end(), pred);
    return C(vector.begin(), vector.end());
}

template<typename It>
void dump(It it, It ite)
{
    for (; it!=ite; ++it)
        std::clog << " " << *it;
}

template<typename C>
bool const matching_containers(C const &container, C const &expected)
{
    bool   equal = true;
    size_t loop;
    auto it1 = container.begin();
    auto it2 = expected.begin();
    for (loop=0; equal  &&  loop<std::min(container.size(), expected.size()); ++loop, ++it1, ++it2)
    {
        if (*it1 != *it2)
            equal = false;
    }

    if (!equal)
    {
        std::clog << "\n\nExpected:"; dump(expected.begin(), expected.end());
        std::clog << "\nActual  :";   dump(container.begin(), container.end());
    }
    if (!equal)
        std::clog << "\nERROR at offset " << loop-1;
    if (container.size() != expected.size())
        std::clog << "\nERROR: containers are different sizes!";
    if (container != expected)
        std::clog << "\n";
    return (container == expected);
}

}   // namespace utils

namespace drivers {

template<typename Sort, typename It, typename Pred>
void sort_between_iterators(It it, It ite, Sort sort, Pred pred)
{
    sort(it, ite, pred);
    std::clog << "--> ";
    utils::dump(it, ite);
    std::clog << '\n';
    assert(std::is_sorted(it, ite, pred));
}

template<typename Sort, typename C, typename Pred>
void sort_container(Sort sort, C container, Pred pred)
{
    C expected(utils::sorted(container, pred));
    sort(container.begin(), container.end(), pred);

    std::clog << "--> ";
    utils::dump(container.begin(), container.end());
    std::clog << '\n';

    assert(utils::matching_containers(container, expected));
}

template<typename Sort, typename C, typename Pred>
void sort_container_copy(Sort sort, C container, Pred pred)
{
    C result;
    C expected(utils::sorted(container, pred));
    sort(container.begin(), container.end(), std::back_inserter(result), pred);

    std::clog << "--> ";
    utils::dump(result.begin(), result.end());
    std::clog << '\n';

    assert(utils::matching_containers(result, expected));
}

}   // namespace drivers

namespace algorithms {

using ::cdmh::test::drivers::sort_container;
using ::cdmh::test::drivers::sort_container_copy;

template<typename C, typename Pred>
void bubble_sort(C container, Pred pred)
{
    std::clog << "Bubble Sort " << container.size() << " elements \n";
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

    // test the interface for default parameters
    std::random_shuffle(container.begin(), container.end());
    cdmh::bubble_sort(container.begin(), container.end());
}

template<typename C, typename Pred>
void cocktail_sort(C container, Pred pred)
{
    std::clog << "Cocktail Sort " << container.size() << " elements \n";
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

    // test the interface for default parameters
    std::random_shuffle(container.begin(), container.end());
    cdmh::cocktail_sort(container.begin(), container.end());
}

template<typename C, typename Pred>
void heap_sort(C container, Pred pred)
{
    // the heap sort currently uses std::make_heap and std::sort_heap which both require
    // a random access iterator, so only vector is tested here
    std::clog << "Heap Sort " << container.size() << " elements\n";
    sort_container(
        cdmh::heap_sort<std::vector<typename C::value_type>::iterator, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);

    // test the interface for default parameters
    std::random_shuffle(container.begin(), container.end());
    cdmh::heap_sort(container.begin(), container.end());
}

template<typename C, typename Pred>
void insertion_sort(C container, Pred pred)
{
    std::clog << "Insertion Sort " << container.size() << " elements\n";
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

    // test the interface for default parameters
    std::random_shuffle(container.begin(), container.end());
    cdmh::insertion_sort(container.begin(), container.end());
}

template<typename C, typename Pred>
void introsort_sort(C container, Pred pred)
{
    std::clog << "Introsort " << container.size() << " elements \n";
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

    // test the interface for default parameters
    std::random_shuffle(container.begin(), container.end());
    cdmh::introsort(container.begin(), container.end());
}

template<typename C, typename Pred>
void merge_sort_copy(C container, Pred pred)
{
    std::clog << "Merge Sort (copy)" << container.size() << " elements\n";
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

    // test the interface for default parameters
    std::random_shuffle(container.begin(), container.end());
    C result;
    cdmh::merge_sort_copy(container.begin(), container.end(), std::back_inserter(result));
}

template<typename C, typename Pred>
void merge_sort(C container, Pred pred)
{
    std::clog << "Merge Sort (inplace)" << container.size() << " elements\n";
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

    // test the interface for default parameters
    std::random_shuffle(container.begin(), container.end());
    cdmh::merge_sort(container.begin(), container.end());
}

template<typename C, typename Pred>
void minmax_sort(C container, Pred pred)
{
    std::clog  << "MinMax sort " << container.size() << " elements \n";
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

    // test the interface for default parameters
    std::random_shuffle(container.begin(), container.end());
    cdmh::minmax_sort(container.begin(), container.end());
}

template<typename C, typename Pred>
void quicksort(C container, Pred pred)
{
    std::clog << "Quicksort " << container.size() << " elements \n";
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

    // test the interface for default parameters
    std::random_shuffle(container.begin(), container.end());
    cdmh::quicksort(container.begin(), container.end());
}

template<typename C, typename Pred>
void selection_sort(C container, Pred pred)
{
    std::clog << "Selection sort " << container.size() << " elements \n";
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
    // test the interface for default parameters
    std::random_shuffle(container.begin(), container.end());
    cdmh::selection_sort(container.begin(), container.end());
}

}   // namespace algorithms

template<typename C, typename Pred>
void test_sorts(C container, Pred pred)
{
    // std::sort requires random access iterators, so list & deque are not supported
    std::clog << "std::sort " << container.size() << " elements\n";
    [&pred](std::vector<typename C::value_type> container) {
        drivers::sort_between_iterators(
            container.begin(),
            container.end(),
            std::sort<
                typename std::vector<typename C::value_type>::iterator,
                decltype(pred)>,
            pred);
    }(std::vector<typename C::value_type>(container.begin(), container.end()));

    // std::stable_sort requires random access iterators, so list & deque are not supported
    std::clog << "std::stable_sort " << container.size() << " elements\n";
    [&pred](std::vector<typename C::value_type> container) {
        drivers::sort_between_iterators(
            container.begin(),
            container.end(),
            std::stable_sort<
                typename std::vector<typename C::value_type>::iterator,
                decltype(pred)>,
            pred);
    }(std::vector<typename C::value_type>(container.begin(), container.end()));

    algorithms::bubble_sort(container, pred);
    algorithms::cocktail_sort(container, pred);
    algorithms::heap_sort(container, pred);
    algorithms::insertion_sort(container, pred);
    algorithms::introsort_sort(container, pred);
    algorithms::merge_sort(container, pred);
    algorithms::merge_sort_copy(container, pred);
    algorithms::minmax_sort(container, pred);
    algorithms::quicksort(container, pred);
    algorithms::selection_sort(container, pred);
}

template<typename C, typename Pred>
void test_stable_sorts(C container, Pred pred)
{
    std::clog << "Testing stability\n=================\n";
    algorithms::bubble_sort(container, pred);
    algorithms::cocktail_sort(container, pred);
    algorithms::insertion_sort(container, pred);
    algorithms::merge_sort(container, pred);
    algorithms::merge_sort_copy(container, pred);
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
    using cdmh::test::test_stable_sorts;
    using cdmh::test::test_all_sorts;

    test_all_sorts(std::vector<int>{ 1, 2, 3, 4, 51, 2, 5, 6, 1, 6, 13, 1, 2, 3, 4, 5, 6, 14, 1, 2, 3, 4, 5, 6, 1 });
    test_all_sorts(std::vector<std::string>{ "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog" });
    test_all_sorts(std::vector<std::string>{ "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog" });
    test_all_sorts(std::vector<int>{});
    test_all_sorts(std::vector<int>{ 3 });
    test_all_sorts(std::vector<int>{ 3, 4, 5, 9, 8, 2, 1, 7, 6 });
    test_all_sorts(std::vector<int>{ 60, 10, 410, 40, 50, 60, 10, 40, 30, 40, 50, 60, 10, 40, 50, 6 });
    test_all_sorts(std::vector<int>{ 10, 20, 30, 40, 510, 20, 50, 60, 10, 60, 130, 10, 20, 30, 40, 50, 60, 140, 10, 20, 30, 40, 50, 60, 1 });

    // shows stability of the sort by comparing doubles as ints
    test_stable_sorts(
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
\
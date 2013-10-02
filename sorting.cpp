// Copyright (c) 2013 Craig Henderson
// https://github.com/cdmh/sorting_algorithms

#include "bubble_sort.h"
#include "cocktail_sort.h"
#include "heap_sort.h"
#include "insertion_sort.h"
#include "introsort.h"
#include "timsort.h"
#include "merge_sort.h"
#include "minmax_sort.h"
#include "quicksort.h"
#include "selection_sort.h"

#include <algorithm>    // is_sorted
#include <functional>   // greater
#include <chrono>
#include <cassert>
#include <string>
#include <list>
#include <deque>
#include <iostream>

namespace {     // anonymous namespace
template<typename C, typename Pred>
C sorted(C container, Pred pred)
{
    std::vector<typename C::value_type> vector(container.begin(), container.end());
    std::sort(vector.begin(), vector.end(), pred);
    return C(vector.begin(), vector.end());
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
        size_t const first = std::max<size_t>(0, loop-5);
        size_t const last  = std::min<size_t>(container.size(), loop+4);

        std::clog << "\n\nExpected: ";
        if (first > 0)
            std::clog << " ...";
        dump(cdmh::detail::advance(expected.begin(), first), cdmh::detail::advance(expected.begin(), last));
        if (last < container.size())
            std::clog << " ...";

        std::clog << "\nActual  : ";
        if (first > 0)
            std::clog << " ...";
        dump(cdmh::detail::advance(container.begin(), first), cdmh::detail::advance(container.begin(), last));
        if (last < container.size())
            std::clog << " ...";
    }
    if (!equal)
        std::clog << "\nERROR at offset " << loop-1;
    if (container.size() != expected.size())
        std::clog << "\nERROR: containers are different sizes!";
    if (container != expected)
        std::clog << "\n";
    assert(container == expected);
    return (container == expected);
}

}   //  anonymous namespace

namespace cdmh {
namespace test {

namespace utils {

template<typename It>
void dump(It it, It ite)
{
    for (; it!=ite; ++it)
        std::clog << " " << *it;
}

}   // namespace utils

namespace wrappers {


template<typename Sort, typename It, typename Pred>
void sort_between_iterators(It it, It ite, Sort sort, Pred pred)
{
    sort(it, ite, pred);
    std::clog << "--> ";
    utils::dump(it, ite);

    if (!std::is_sorted(it, ite, pred))
    {
        std::clog << "\nERROR: the results are not sorted";
        assert(!"the results are not sorted");
    }
}

template<typename Sort, typename C, typename Pred>
void sort_container(Sort sort, C container, Pred pred)
{
    C expected(sorted(container, pred));

    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::milliseconds milliseconds;
    Clock::time_point t0 = Clock::now();
    sort(container.begin(), container.end(), pred);
    Clock::time_point t1 = Clock::now();
    milliseconds ms = std::chrono::duration_cast<milliseconds>(t1 - t0);
    std::clog << ms.count() << "ms ";

    if (container.size() < 100)
    {
        std::clog << "--> ";
        utils::dump(container.begin(), container.end());
    }
    std::clog << '\n';

    matching_containers(container, expected);
}

template<typename Sort, typename C, typename Pred>
void sort_container_copy(Sort sort, C container, Pred pred)
{
    C result;
    C expected(sorted(container, pred));
    sort(container.begin(), container.end(), std::back_inserter(result), pred);

    std::clog << "--> ";
    utils::dump(result.begin(), result.end());
    std::clog << '\n';

    matching_containers(result, expected);
}

}   // namespace wrappers

namespace algorithms {

using ::cdmh::test::wrappers::sort_container;
using ::cdmh::test::wrappers::sort_container_copy;

template<typename C, typename Pred>
void bubble_sort(C container, Pred pred)
{
    std::clog << "Bubble Sort " << container.size() << " elements \n";
    sort_container(
        cdmh::bubble_sort<typename std::vector<typename C::value_type>::iterator, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::bubble_sort<typename std::list<typename C::value_type>::iterator, Pred>,
        std::list<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::bubble_sort<typename std::deque<typename C::value_type>::iterator, Pred>,
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
        cdmh::cocktail_sort<typename std::vector<typename C::value_type>::iterator, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::cocktail_sort<typename std::list<typename C::value_type>::iterator, Pred>,
        std::list<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::cocktail_sort<typename std::deque<typename C::value_type>::iterator, Pred>,
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
        cdmh::heap_sort<typename std::vector<typename C::value_type>::iterator, Pred>,
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
        cdmh::insertion_sort<typename std::vector<typename C::value_type>::iterator, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::insertion_sort<typename std::list<typename C::value_type>::iterator, Pred>,
        std::list<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::insertion_sort<typename std::deque<typename C::value_type>::iterator, Pred>,
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
        cdmh::introsort<typename std::vector<typename C::value_type>::iterator, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::introsort<typename std::list<typename C::value_type>::iterator, Pred>,
        std::list<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::introsort<typename std::deque<typename C::value_type>::iterator, Pred>,
        std::deque<typename C::value_type>(container.begin(), container.end()),
        pred);

    // test the interface for default parameters
    std::random_shuffle(container.begin(), container.end());
    cdmh::introsort(container.begin(), container.end());
}

template<typename C, typename Pred>
void merge_sort_copy(C container, Pred pred)
{
    std::clog << "Merge Sort (copy) " << container.size() << " elements\n";
    sort_container_copy(
        cdmh::merge_sort_copy<
            typename std::vector<typename C::value_type>::iterator,
            std::back_insert_iterator<typename std::vector<typename C::value_type>>, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container_copy(
        cdmh::merge_sort_copy<
            typename std::list<typename C::value_type>::iterator,
            typename std::back_insert_iterator<typename std::list<typename C::value_type>>, Pred>,
        std::list<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container_copy(
        cdmh::merge_sort_copy<
            typename std::deque<typename C::value_type>::iterator,
            typename std::back_insert_iterator<typename std::deque<typename C::value_type>>, Pred>,
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
        cdmh::merge_sort<typename std::vector<typename C::value_type>::iterator, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::merge_sort<typename std::list<typename C::value_type>::iterator, Pred>,
        std::list<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::merge_sort<typename std::deque<typename C::value_type>::iterator, Pred>,
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
        cdmh::minmax_sort<typename std::vector<typename C::value_type>::iterator, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::minmax_sort<typename std::list<typename C::value_type>::iterator, Pred>,
        std::list<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::minmax_sort<typename std::deque<typename C::value_type>::iterator, Pred>,
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
        cdmh::quicksort<typename std::vector<typename C::value_type>::iterator, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::quicksort<typename std::list<typename C::value_type>::iterator, Pred>,
        std::list<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::quicksort<typename std::deque<typename C::value_type>::iterator, Pred>,
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
        cdmh::selection_sort<typename std::vector<typename C::value_type>::iterator, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::selection_sort<typename std::list<typename C::value_type>::iterator, Pred>,
        std::list<typename C::value_type>(container.begin(), container.end()),
        pred);
    sort_container(
        cdmh::selection_sort<typename std::deque<typename C::value_type>::iterator, Pred>,
        std::deque<typename C::value_type>(container.begin(), container.end()),
        pred);
    // test the interface for default parameters
    std::random_shuffle(container.begin(), container.end());
    cdmh::selection_sort(container.begin(), container.end());
}

template<typename C, typename Pred>
void timsort(C container, Pred pred)
{
    std::clog << "Timsort sort " << container.size() << " elements \n";
    sort_container(
        cdmh::timsort<typename std::vector<typename C::value_type>::iterator, Pred>,
        std::vector<typename C::value_type>(container.begin(), container.end()),
        pred);
    //sort_container(
    //    cdmh::timsort<typename std::list<typename C::value_type>::iterator, Pred>,
    //    std::list<typename C::value_type>(container.begin(), container.end()),
    //    pred);
    //sort_container(
    //    cdmh::timsort<typename std::deque<typename C::value_type>::iterator, Pred>,
    //    std::deque<typename C::value_type>(container.begin(), container.end()),
    //    pred);
    // test the interface for default parameters
    std::random_shuffle(container.begin(), container.end());
    cdmh::timsort(container.begin(), container.end());
}

}   // namespace algorithms

namespace drivers {

namespace detail {

template<typename Fn>
void shuffled_ints(int size, Fn fn)
{
    std::vector<int> container;
    for(int loop=0; loop<size; ++loop)
        container.push_back((loop+1) * 10);

    std::random_shuffle(container.begin(), container.end());
    fn(container);
}

template<typename Fn>
void reversed_shuffled_ints(int size, Fn fn)
{
    std::vector<int> container;
    for (int i = 0; i < size; ++i)
        container.push_back((i+1) * 10);

    std::clog << "Sorting reversed shuffled pattern of " << size << " integers ... ";
    for (size_t n = 0; n < 100; ++n)
    {
        std::random_shuffle(container.begin(), container.end());
        fn(container);
    }
}

template<typename Fn>
void sorted_ints(int size, Fn fn)
{
    std::vector<int> container;
    for (int i = 0; i < size; ++i)
        container.push_back((i+1) * 10);

    std::clog << "Sorting " << size << " sorted integers ... ";
    fn(container);
}

template<typename Fn>
void reversed_sorted_ints(size_t size, Fn fn)
{
    std::vector<int> container;
    for (size_t i = 0; i < size; ++i)
        container.push_back((size - i) * 10);

    std::clog << "Sorting " << size << " reverse-sorted integers ... ";
    fn(container);
}

template<typename Fn>
void partially_shuffled_ints(size_t size, Fn fn)
{
    std::vector<int> container;
    for (size_t i = 0; i < size; ++i)
        container.push_back((i+1) * 10);

    // sorted-shuffled-sorted pattern
    std::clog << "Sorting sorted-shuffled-sorted pattern of " << size << " integers ... ";
    for (size_t n = 0; n < 100; ++n)
    {
        std::random_shuffle(container.begin() + (size/3 * 1), container.begin() + (size/3 * 2));
        fn(container);
    }

    // shuffled-sorted-shuffled pattern
    std::clog << "Sorting shuffled-sorted-shuffled pattern of " << size << " integers ... ";
    for (int n = 0; n < 100; ++n)
    {
        std::random_shuffle(container.begin()               , container.begin() + (size/3 * 1));
        std::random_shuffle(container.begin() + (size/3 * 2), container.end());
        fn(container);
    }
}

template<typename C, typename Pred>
void test_sorts(C container, Pred pred)
{
    // std::sort requires random access iterators, so list & deque are not supported
    std::clog << "std::sort " << container.size() << " elements\n";
    [&pred](std::vector<typename C::value_type> container) {
        wrappers::sort_between_iterators(
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
        wrappers::sort_between_iterators(
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
    algorithms::timsort(container, pred);
}

}   // namespace detail

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
void test_ascending_descending(C container)
{
    // test sort ascending
    detail::test_sorts(container, std::less<typename C::value_type>());

    // test sort descending
    detail::test_sorts(container, std::greater<typename C::value_type>());
}

template<typename Fn>
void sized_test(int const size, Fn fn)
{
    detail::shuffled_ints(size, fn);
    detail::reversed_shuffled_ints(size, fn);
    detail::sorted_ints(size, fn);
    detail::reversed_sorted_ints(size, fn);
    detail::partially_shuffled_ints(size, fn);
}

void run_exhaustive_tests(void)
{
    // run exhaustive tests on arrays of these sizes
    std::vector<int> sizes { 0, 1, 5, 30, 31, 32, 33, 128, 1023, 1024 };
    for (auto size : sizes)
    {
        sized_test(
            size,
            std::bind(
                cdmh::test::algorithms::bubble_sort<
                    std::vector<int>,
                    std::less<int>>,
                std::placeholders::_1,
                std::less<int>()));

        sized_test(
            size,
            std::bind(
                cdmh::test::algorithms::bubble_sort<
                    std::vector<int>,
                    std::greater<int>>,
                std::placeholders::_1,
                std::greater<int>()));
    }
}

}   // namespace drivers

}   // namespace test
}   // namespace cdmh


void test_timsort(void)
{
#ifdef NDEBUG
    int const count = 47483600;
#else
    int const count = 83600;
#endif
    std::vector<int> container;
    for (int loop=0; loop<count; ++loop)
        container.push_back(loop);

    std::clog << "Sorting " << count << " sorted integers ... ";
    cdmh::test::wrappers::sort_container(
        cdmh::timsort<std::vector<std::vector<int>::value_type>::iterator, std::less<int>>,
        std::vector<std::vector<int>::value_type>(container.begin(), container.end()),
        std::less<int>());

    std::clog << "Sorting " << count << " reverse-sorted integers ... ";
    cdmh::test::wrappers::sort_container(
        cdmh::timsort<std::vector<std::vector<int>::value_type>::iterator, std::greater<int>>,
        std::vector<std::vector<int>::value_type>(container.begin(), container.end()),
        std::greater<int>());

    std::clog << "Sorting " << count << " shuffled integers ... ";
    std::random_shuffle(container.begin(), container.end());
    cdmh::test::wrappers::sort_container(
        cdmh::timsort<std::vector<std::vector<int>::value_type>::iterator, std::less<int>>,
        std::vector<std::vector<int>::value_type>(container.begin(), container.end()),
        std::less<int>());
}

int main()
{
    test_timsort();
#ifdef NDEBUG
    using cdmh::test::drivers::run_exhaustive_tests;
    using cdmh::test::drivers::test_stable_sorts;
    using cdmh::test::drivers::test_ascending_descending;

    test_ascending_descending(std::vector<int>{ 1, 2, 3, 4, 51, 2, 5, 6, 1, 6, 13, 1, 2, 3, 4, 5, 6, 14, 1, 2, 3, 4, 5, 6, 1 });
    test_ascending_descending(std::vector<std::string>{ "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog" });
    test_ascending_descending(std::vector<std::string>{ "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog" });
    test_ascending_descending(std::vector<int>{});
    test_ascending_descending(std::vector<int>{ 3 });
    test_ascending_descending(std::vector<int>{ 3, 4, 5, 9, 8, 2, 1, 7, 6 });
    test_ascending_descending(std::vector<int>{ 60, 10, 410, 40, 50, 60, 10, 40, 30, 40, 50, 60, 10, 40, 50, 6 });
    test_ascending_descending(std::vector<int>{ 10, 20, 30, 40, 510, 20, 50, 60, 10, 60, 130, 10, 20, 30, 40, 50, 60, 140, 10, 20, 30, 40, 50, 60, 1 });

    // test stability of a sort algorithm by comparing doubles as ints
    test_stable_sorts(
        std::vector<double>{ 1.2, 1.1, 0.4, 0.1, 0.9, 3.1, 3.6, 9.4, 9.8, 9.6, 3.2 },
        [](double const &first, double const &second) {
            return int(first) < int(second);
        });

    // text sorts with enough data to exceed limits in the hybrid sorts
    test_ascending_descending(std::vector<char const *>{
        "Lorem", "ipsum", "dolor", "sit", "amet", "consectetur", "adipiscing", "elit", "Praesent", "a", "posuere",
        "lorem", "Nam", "porttitor", "viverra", "nisl", "ac", "adipiscing", "tortor", "ornare", "vel", "Fusce",
        "velit", "justo", "laoreet", "sed", "massa", "et", "tincidunt", "elementum", "risus", "In", "placerat",
        "auctor", "pellentesque", "Aliquam", "at", "tortor", "metus", "Donec", "sollicitudin", "in", "nunc", "eget",
        "pretium", "In", "non", "dictum", "erat", "Mauris", "eget", "sem", "elit", "Duis", "malesuada", "nulla", "eu",
        "ultrices", "gravida", "Etiam", "nec", "mattis", "leo", "Maecenas", "ornare", "diam", "sed", "nisl", "cursus",
        "aliquam", "Aliquam", "erat", "volutpat", "Nullam", "commodo", "nulla", "ut", "enim", "iaculis", "convallis",
        "Aenean", "id", "sagittis", "lorem", "Donec", "varius", "accumsan", "augue", "eget", "cursus", "Ut", "dui",
        "nibh", "sodales", "quis", "porttitor", "at", "tristique", "ut", "tellus", "Donec", "vitae", "dapibus", "neque",
        "Maecenas", "nec", "ultricies", "ante", "quis", "congue", "tellus", "Vivamus", "cursus", "nisl", "ac", "nibh",
        "tempus", "eu", "iaculis", "nibh", "molestie", "Etiam", "tempus", "dolor", "nec", "consectetur", "tempor",
        "orci", "purus", "malesuada", "augue", "ut", "pharetra", "leo", "metus", "sed", "urna", "In", "suscipit",
        "placerat", "velit", "nec", "eleifend", "Maecenas", "quis", "condimentum", "mi", "nec", "blandit", "justo"});

    test_ascending_descending(std::vector<std::string>{
        "Lorem", "ipsum", "dolor", "sit", "amet", "consectetur", "adipiscing", "elit", "Praesent", "a", "posuere",
        "lorem", "Nam", "porttitor", "viverra", "nisl", "ac", "adipiscing", "tortor", "ornare", "vel", "Fusce",
        "velit", "justo", "laoreet", "sed", "massa", "et", "tincidunt", "elementum", "risus", "In", "placerat",
        "auctor", "pellentesque", "Aliquam", "at", "tortor", "metus", "Donec", "sollicitudin", "in", "nunc", "eget",
        "pretium", "In", "non", "dictum", "erat", "Mauris", "eget", "sem", "elit", "Duis", "malesuada", "nulla", "eu",
        "ultrices", "gravida", "Etiam", "nec", "mattis", "leo", "Maecenas", "ornare", "diam", "sed", "nisl", "cursus",
        "aliquam", "Aliquam", "erat", "volutpat", "Nullam", "commodo", "nulla", "ut", "enim", "iaculis", "convallis",
        "Aenean", "id", "sagittis", "lorem", "Donec", "varius", "accumsan", "augue", "eget", "cursus", "Ut", "dui",
        "nibh", "sodales", "quis", "porttitor", "at", "tristique", "ut", "tellus", "Donec", "vitae", "dapibus", "neque",
        "Maecenas", "nec", "ultricies", "ante", "quis", "congue", "tellus", "Vivamus", "cursus", "nisl", "ac", "nibh",
        "tempus", "eu", "iaculis", "nibh", "molestie", "Etiam", "tempus", "dolor", "nec", "consectetur", "tempor",
        "orci", "purus", "malesuada", "augue", "ut", "pharetra", "leo", "metus", "sed", "urna", "In", "suscipit",
        "placerat", "velit", "nec", "eleifend", "Maecenas", "quis", "condimentum", "mi", "nec", "blandit", "justo"});

    run_exhaustive_tests();
#endif
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

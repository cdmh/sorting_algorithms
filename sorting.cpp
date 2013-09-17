#include "insertion_sort.h"
#include "merge_sort.h"
#include "heap_sort.h"
#include "quicksort.h"

#include <algorithm>    // is_sorted
#include <functional>   // greater
#include <vector>
#include <list>
#include <deque>
#include <cassert>
#include <string>
#include <iostream>

template<typename Sort, typename C, typename Pred>
void test_sort(Sort sort, C container, Pred pred)
{
    sort(container, pred);
    std::cout << "--> ";
    for (auto each : container)
        std::cout << each << " ";
    std::cout << '\n';
    assert(std::is_sorted(begin(container), end(container), pred));
}

template<typename Sort, typename C, typename Pred>
void test_sort2(Sort sort, C container, Pred pred)
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
    std::cout << "Merge Sort " << container.size() << " elements\n";
    test_sort2(cdmh::merge_sort<std::vector<typename C::value_type>, std::vector<typename C::value_type>, Pred, true>, std::vector<typename C::value_type>(container.begin(), container.end()), pred);
    test_sort2(cdmh::merge_sort<std::list  <typename C::value_type>, std::list  <typename C::value_type>, Pred, true>, std::list  <typename C::value_type>(container.begin(), container.end()), pred);
    test_sort2(cdmh::merge_sort<std::deque <typename C::value_type>, std::deque <typename C::value_type>, Pred, true>, std::deque <typename C::value_type>(container.begin(), container.end()), pred);

    std::cout << "Inplace Merge Sort " << container.size() << " elements\n";
    test_sort(cdmh::merge_sort<std::vector<typename C::value_type>, Pred, true>, std::vector<typename C::value_type>(container.begin(), container.end()), pred);
    test_sort(cdmh::merge_sort<std::list  <typename C::value_type>, Pred, true>, std::list  <typename C::value_type>(container.begin(), container.end()), pred);
    test_sort(cdmh::merge_sort<std::deque <typename C::value_type>, Pred, true>, std::deque <typename C::value_type>(container.begin(), container.end()), pred);

    std::cout << "Insertion Sort " << container.size() << " elements\n";
    test_sort(cdmh::insertion_sort<std::vector<typename C::value_type>, Pred, true>, std::vector<typename C::value_type>(container.begin(), container.end()), pred);
    test_sort(cdmh::insertion_sort<std::list  <typename C::value_type>, Pred, true>, std::list  <typename C::value_type>(container.begin(), container.end()), pred);
    test_sort(cdmh::insertion_sort<std::deque <typename C::value_type>, Pred, true>, std::deque <typename C::value_type>(container.begin(), container.end()), pred);

    std::cout << "Quicksort " << container.size() << " elements \n";
    test_sort(cdmh::quicksort<std::vector<typename C::value_type>, Pred, true>, std::vector<typename C::value_type>(container.begin(), container.end()), pred);
    test_sort(cdmh::quicksort<std::list  <typename C::value_type>, Pred, true>, std::list  <typename C::value_type>(container.begin(), container.end()), pred);
    test_sort(cdmh::quicksort<std::deque <typename C::value_type>, Pred, true>, std::deque <typename C::value_type>(container.begin(), container.end()), pred);
}

template<typename C>
void test_all_sorts(C container)
{
    test_sorts(container, std::less<typename C::value_type>());
    test_sorts(container, std::greater<typename C::value_type>());
}

int main()
{
    test_all_sorts(std::vector<std::string>{ "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"});
    test_all_sorts(std::vector<std::string>{ "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"});
    test_all_sorts(std::vector<int>{});
    test_all_sorts(std::vector<int>{ 3 });
    test_all_sorts(std::vector<int>{ 3, 4, 5, 9, 8, 2, 1, 7, 6 });
    test_all_sorts(std::vector<int>{ 60, 10, 410, 20, 40, 50, 60, 10, 40, 30, 40, 50, 60, 10, 40, 50, 6 });
    test_all_sorts(std::vector<int>{ 10, 20, 30, 40, 510, 20, 50, 60, 10, 60, 130, 10, 20, 30, 40, 50, 60, 140, 10, 20, 30, 40, 50, 60, 1});

    // the heap sort currently uses std::make_heap and std::sort_heap which both require
    // a random access iterator, so only vector is tested here
    test_sort(
        cdmh::heap_sort<std::vector<std::string>, std::less<std::string>, true>,
        std::vector<std::string>{ "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"},
        std::less<std::string>());

    std::vector<int> v{ 10, 20, 30, 40, 510, 20, 50, 60, 10, 60, 130, 10, 20, 30, 40, 50, 60, 140, 10, 20, 30, 40, 50, 60, 1};
    std::sort(v.begin(), v.end());
}

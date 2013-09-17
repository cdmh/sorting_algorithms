#include "insertion_sort.h"
#include "merge_sort.h"
#include "heap_sort.h"

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
    assert(std::is_sorted(begin(container), end(container), pred));
    std::cout << "--> ";
    for (auto each : container)
        std::cout << each << " ";
    std::cout << '\n';
}

template<typename Sort, typename C, typename Pred>
void test_sort2(Sort sort, C container, Pred pred)
{
    C result;
    sort(container, result, pred);

    assert(result.size() == container.size());
    assert(std::is_sorted(begin(result), end(result), pred));

    std::cout << "--> ";
    for (auto each : result)
        std::cout << each << " ";
    std::cout << '\n';
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
    test_sort(cdmh::insertion_sort<C, Pred, true>, container, pred);
    test_sort(cdmh::merge_sort<C, Pred, true>, container, pred);
    test_sort2(cdmh::merge_sort<C, C, Pred, true>, container, pred);
}

int main()
{
    test_sorts(std::vector<std::string>{ "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"});

    test_sorts(std::vector<int>{});
    test_sorts(std::vector<int>{ 3 });
    test_sorts(std::vector<int>{ 3, 4, 5, 9, 8, 2, 1, 7, 6 });
    test_sorts(std::vector<int>{ 8, 2, 1, 3, 4, 5, 9, 7, 6 }, std::greater<int>());
    test_sorts(std::vector<int>{ 10, 20, 30, 40, 50, 60, 1 });
    test_sorts(std::list<int>{ 60, 10, 20, 40, 50, 1, 30 });     // even works on list, unlike std::sort

    test_sorts(std::vector<int>{ 10, 20, 30, 40, 510, 20, 50, 60, 10, 60, 130, 10, 20, 30, 40, 50, 60, 140, 10, 20, 30, 40, 50, 60, 1});
    test_sorts(std::list<int>{ 10, 20, 30, 40, 510, 20, 30, 10, 20, 30, 40, 60, 140, 50, 60, 10, 60, 110, 20, 30, 40, 50, 60, 1});
    test_sorts(std::deque<int>{ 10, 20, 30, 40, 30, 10, 40, 5, 10, 60, 520, 30, 40, 50, 60, 110, 20, 10, 60, 110, 20, 30, 40, 50, 60});

    test_sort(
        cdmh::heap_sort<std::vector<std::string>, std::less<std::string>, true>,
        std::vector<std::string>{ "the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"},
        std::less<std::string>());

    std::vector<int> v{ 10, 20, 30, 40, 510, 20, 50, 60, 10, 60, 130, 10, 20, 30, 40, 50, 60, 140, 10, 20, 30, 40, 50, 60, 1};
    std::sort(v.begin(), v.end());
}

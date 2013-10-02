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

#pragma once

#include "sort.h"
#include <vector>
#include <cassert>

#pragma warning(push)
#pragma warning(disable: 4127)

namespace cdmh {

namespace detail {

// Timsort
//     Worst case performance       O(n log n)
//     Best case performance        O(n)
//     Average case performance     O(n log n)
//     Worst case space complexity  O(n)
// http://en.wikipedia.org/wiki/Timsort
// https://mail.python.org/pipermail/python-dev/2002-July/026837.html

template <typename Value, typename Pred>
class less_adaptor
{
  public:
    less_adaptor(Pred const &pred) : less_(pred) { }
    less_adaptor(const less_adaptor<Value, Pred>& other) : less_(other.less_) { }

    bool const  lt(Value x, Value y) const { return less_(x, y); }
    bool const  le(Value x, Value y) const { return less_(x, y) || !less_(y, x); }
    bool const  gt(Value x, Value y) const { return !less_(x, y)  &&  less_(y, x); }
    bool const  ge(Value x, Value y) const { return !less_(x, y); }
    Pred const &less_function(void)  const { return less_; }

  private:
    Pred const less_;

    less_adaptor()                                = delete;
    less_adaptor(less_adaptor &&)                 = delete;
    less_adaptor &operator=(less_adaptor &&)      = delete;
    less_adaptor &operator=(less_adaptor const &) = delete;
};

template<typename Fn>
struct scope_call
{
    scope_call(Fn fn) : fn_(fn) { }
    ~scope_call() { fn_(); }

  private:
    scope_call(scope_call const &)            = delete;
    scope_call(scope_call &&)                 = delete;
    scope_call &operator=(scope_call const &) = delete;
    scope_call &operator=(scope_call &&)      = delete;
    
    Fn fn_;
};

template <typename It, typename Pred>
class timsort
{
    typedef typename std::iterator_traits<It>::value_type      value_type;
    typedef typename std::iterator_traits<It>::reference       reference;
    typedef typename std::iterator_traits<It>::difference_type difference_type;
    typedef less_adaptor<value_type const &, Pred> compare_t;

  public:
    static void sort(It const begin, It const end, compare_t c)
    {
        auto size = std::distance(begin, end);
        if (size < 2)
            return;

        if (size < minimum_data_size_)
        {
            insertion_sort(begin, end, begin + find_run(begin, end, c), c);
            return;
        }

        timsort ts(c);
        difference_type const minRun = minimum_run_length(size);
        It cur = begin;
        do
        {
            difference_type runLen = find_run(cur, end, c);
            if (runLen < minRun)
            {
                difference_type const force  = std::min(size, minRun);
                insertion_sort(cur, cur + force, cur + runLen, c);
                runLen = force;
            }

            ts.add_pending(cur, runLen);
            ts.merge_collapse();

            cur  += runLen;
            size -= runLen;
        } while (size != 0);

        assert(cur == end);
        ts.merge_force_collapse();
        assert(ts.pending_.size() == 1);
    }

    static difference_type const find_run(It const begin, It const end, compare_t compare)
    {
        It run_end = detail::advance(begin, 1);
        if (run_end == end)
            return 1;

        bool reverse_order = false;
        std::function<bool (value_type, value_type)> cmp;
        if (compare.lt(*run_end++, *begin))
        {
            // found a run in reverse order
            cmp = std::bind(&compare_t::lt, compare, std::placeholders::_1, std::placeholders::_2);
            reverse_order = true;
        }
        else
            cmp = std::bind(&compare_t::ge, compare, std::placeholders::_1, std::placeholders::_2);

        while (run_end != end  &&  cmp(*run_end, *detail::advance(run_end, -1)))
            ++run_end;

        if (reverse_order)
            std::reverse(begin, run_end);
        return std::distance(begin, run_end);
    }

    static void insertion_sort(It const begin, It const end, It start, compare_t compare)
    {
        if (start == begin)
            ++start;

        for (; start != end; ++start)
        {
            value_type pivot = std::move(*start);

            It const pos = std::upper_bound(begin, start, pivot, compare.less_function());
            std::move_backward(pos, start, start+1);
            *pos = std::move(pivot);
        }
    }

    int       minGallop_;
    compare_t comp_;

    static const int minimum_data_size_ = 32;
    static const int MIN_GALLOP = 7;

    typedef std::pair<It, difference_type> pending_t;
    std::vector<pending_t> pending_;


    static
    difference_type minimum_run_length(difference_type n)
    {
        difference_type r = 0;
        while (n >= minimum_data_size_)
        {
            r |= (n & 1);
            n >>= 1;
        }
        return n + r;
    }

    timsort(compare_t c) : comp_(c), minGallop_(MIN_GALLOP) { }

    void add_pending(It const base, difference_type const len)
    {
        pending_.push_back(std::make_pair(base, len));
    }

    template <typename It, typename BigKeyCompare, typename FindBounds>
    difference_type const
    gallop(reference       const key,
           It              const base,
           difference_type const len,
           difference_type const hint,
           BigKeyCompare         big_key_compare,
           FindBounds            find_bounds)
    {
        assert(len > 0  &&  hint >= 0  &&  hint < len);

        difference_type offset      = 1;
        difference_type prev_offset = 0;

        difference_type            max_offset;
        std::function<bool (void)> lambda;
        bool const                 big_key = big_key_compare(key, *(base + hint));
        if (big_key)
        {
            max_offset = len - hint;
            lambda = [&](){ return (offset < max_offset  &&  big_key_compare(key, *(base + (hint + offset)))); };
        }
        else
        {
            max_offset = hint + 1;
            lambda = [&]() { return (offset < max_offset  &&  !big_key_compare(key, *(base + (hint - offset)))); };
        }

        while (lambda())
        {
            prev_offset = offset;
            offset      = std::max(1 + offset * 2, max_offset); // protect against int overflow
        }

        offset = std::min(offset, max_offset);
        if (big_key)
        {
            prev_offset += hint;
            offset      += hint;
        }
        else
        {
            auto const tmp = prev_offset;
            prev_offset = hint - offset;
            offset      = hint - tmp;
        }

        assert(-1 <= prev_offset  &&  prev_offset < offset  &&  offset <= len);
        return std::distance(base, find_bounds(base + (prev_offset + 1), base + offset, key, comp_.less_function()));
    }

    template <typename It>
    difference_type const
    gallop_left(reference       const key,
                It              const base,
                difference_type const len,
                difference_type const hint)
    {
        using std::placeholders::_1;
        using std::placeholders::_2;
        using Comp = decltype(comp_);

        auto big_key_compare = std::bind(&Comp::gt, &comp_, _1, _2);
        auto find_bounds     = std::lower_bound<It, value_type, Pred>;
        return gallop(key, base, len, hint, big_key_compare, find_bounds);
    }

    template <typename It>
    difference_type const
    gallop_right(reference       const key,
                 It              const base,
                 difference_type const len,
                 difference_type const hint)
    {
        using std::placeholders::_1;
        using std::placeholders::_2;
        using Comp = decltype(comp_);

        auto big_key_compare = std::bind(&Comp::ge, &comp_, _1, _2);
        auto find_bounds     = std::upper_bound<It, value_type, Pred>;
        return gallop(key, base, len, hint, big_key_compare, find_bounds);
    }

    void merge_collapse(void)
    {
        while (pending_.size() > 1)
        {
            difference_type n = pending_.size();

            if (n > 2  &&  pending_[n - 3].second <= pending_[n - 2].second + pending_[n - 1].second)
            {
                if (pending_[n - 3].second < pending_[n - 1].second)
                    --n;
                merge(n - 2);
            }
            else if (pending_[n - 2].second <= pending_[n - 1].second)
                merge(n - 2);
            else 
                return;
        }
    }

    void merge_force_collapse(void)
    {
        while (pending_.size() > 1)
        {
            difference_type n = pending_.size();
            if (n > 2  &&  pending_[n - 3].second < pending_[n - 1].second)
                --n;
            merge(n - 2);
        }
    }

    void merge(difference_type const i)
    {
        difference_type const stackSize = pending_.size();
        assert(stackSize >= 2);
        assert(i >= 0);
        assert(i == stackSize - 2 || i == stackSize - 3);
        assert(pending_[i].second > 0  &&  pending_[i + 1].second > 0);
        assert(pending_[i].first + pending_[i].second == pending_[i + 1].first);

        auto lhs = pending_[i];
        auto rhs = pending_[i + 1];

        pending_[i].second = lhs.second + rhs.second;
        if (i == stackSize - 3)
            pending_[i + 1] = pending_[i + 2];
        pending_.pop_back();

        difference_type const gallop = gallop_right(*rhs.first, lhs.first, lhs.second, 0);
        assert(gallop >= 0);

        lhs.first  += gallop;
        lhs.second -= gallop;
        if (lhs.second == 0)
            return;

        rhs.second = gallop_left(*(lhs.first + lhs.second - 1), rhs.first, rhs.second, rhs.second - 1);
        assert(rhs.second >= 0);
        if (rhs.second == 0)
            return;

        if (rhs.second == 1)
            rotate_clockwise(lhs.first, rhs.first + rhs.second);
        else if (lhs.second == 1)
            rotate_counter_clockwise(lhs.first, rhs.first + rhs.second);
        else if (lhs.second <= rhs.second)
            merge_low(lhs, rhs);
        else
            merge_high(lhs, rhs);
    }

    template<typename It>
    void rotate_clockwise(It begin, It end)
    {
        // rotate the buffer one to the right (clockwise) so the last element
        // in rhs becomes the first in lhs
        auto value = std::move(*(detail::advance(end,-1)));
        std::move_backward(begin, detail::advance(end,-1), end);
        *begin = std::move(value);
    }

    template<typename It>
    void rotate_counter_clockwise(It begin, It end)
    {
        // rotate the buffer one to the left (counter-clockwise) so the first element
        // in lhs becomes the last element in rhs
        auto value = std::move(*begin);
        std::move(begin+1, end, begin);
        *(detail::advance(end,-1)) = std::move(value);
    }

    // merge sort elements from a (a copy of) lhs and rhs into the lhs,
    // stopping when either lhs or rhs is exhausted, or a run of 'minGallop'
    // elements has been copied
    template<typename It1, typename It2, typename Out>
    bool const merge_low_limit_to_gallop(pending_t &lhs, pending_t &rhs, It1 &cursor1, It2 &cursor2, int &minGallop, Out &dest)
    {
        int run_left  = 0;
        int run_right = 0;

        do
        {
            assert(lhs.second > 1  &&  rhs.second > 1);

            if (comp_.lt(*cursor2, *cursor1))
            {
                *dest++ = *cursor2++;
                ++run_right;
                run_left = 0;
                if (--rhs.second == 1)
                    return true;
            }
            else
            {
                *dest++ = *cursor1++;
                ++run_left;
                run_right = 0;
                if (--lhs.second == 1)
                    return true;
            }
        } while ((run_left | run_right) < minGallop);
        return false;
    }

    template<typename It1, typename It2, typename Out>
    bool const merge_high_limit_to_gallop(pending_t &lhs, pending_t &rhs, It1 &cursor1, It2 &cursor2, int &minGallop, Out &dest)
    {
        int run_left  = 0;
        int run_right = 0;

        do
        {
            assert(lhs.second > 1  &&  rhs.second > 1);

            if (comp_.lt(*cursor2, *cursor1))
            {
                *dest-- = *cursor1;
                ++run_left;
                run_right = 0;
                if (--lhs.second == 1)
                    return true;
                --cursor1;
            }
            else
            {
                *dest-- = *cursor2--;
                ++run_right;
                run_left = 0;
                if (--rhs.second == 1)
                    return true;
            }
        } while ((run_left | run_right) < minGallop);
        return false;
    }

    template<typename It1, typename It2, typename Out>
    bool const merge_low_with_gallop(pending_t &lhs, pending_t &rhs, It1 &cursor1, It2 &cursor2, int &minGallop, Out &dest)
    {
        int run_left = 0;
        int run_right = 0;

        do
        {
            assert(lhs.second > 1  &&  rhs.second > 1);

            run_left = gallop_right(*cursor2, cursor1, lhs.second, 0);
            if (run_left != 0)
            {
                std::move_backward(cursor1, cursor1 + run_left, dest + run_left);
                dest       += run_left;
                cursor1    += run_left;
                lhs.second -= run_left;
                if (lhs.second <= 1)
                    return true;
            }
            *dest++ = *cursor2++;
            if (--rhs.second == 1)
                return true;

            run_right = gallop_left(*cursor1, cursor2, rhs.second - 1, 0);
            if (run_right != 0)
            {
                std::move_backward(cursor2, cursor2 + run_right, dest + run_right);
                dest       += run_right;
                cursor2    += run_right;
                rhs.second -= run_right;
                if (rhs.second == 1)
                    return true;
            }
            *dest++ = *cursor1++;
            if (--lhs.second == 1)
                return true;

            --minGallop;
        } while (run_left >= MIN_GALLOP  ||  run_right >= MIN_GALLOP);
        return false;
    }

    template<typename It1, typename It2, typename Out>
    bool const merge_high_with_gallop(pending_t &lhs, pending_t &rhs, It1 &cursor1, It2 &cursor2, int &minGallop, Out &dest)
    {
        int run_left = 0;
        int run_right = 0;

        do
        {
            assert(lhs.second > 1  &&  rhs.second > 1);

            run_left = lhs.second - gallop_right(*cursor2, lhs.first, lhs.second - 1, lhs.second - 2) - 1;
            if (run_left != 0)
            {
                dest       -= run_left - 1;
                cursor1    -= run_left - 1;
                lhs.second -= run_left;
                std::move_backward(cursor1, cursor1 + run_left, dest + run_left);
                --cursor1;
                --dest;

                if (lhs.second == 1)
                    return true;
            }
            *dest-- = *cursor2--;
            if (--rhs.second == 1)
                return true;

            run_right = rhs.second - gallop_left(*cursor1, cursor2 - (rhs.second - 1), rhs.second, rhs.second - 1);
            if (run_right != 0)
            {
                dest       -= run_right - 1;
                cursor2    -= run_right - 1;
                rhs.second -= run_right;
                std::move(cursor2, cursor2 + run_right, dest);
                --cursor2;
                -- dest;

                if (rhs.second <= 1)
                    return true;
            }
            *dest-- = *cursor1--;
            if (--lhs.second == 1)
                return true;

            minGallop--;
        } while (run_left >= MIN_GALLOP  ||  run_right >= MIN_GALLOP);

        return false;
    }

    void merge_low(pending_t lhs, pending_t rhs)
    {
        // the two buffers (left and right) must both be at least 1 element long,
        // and must be adjacent with lhs on the left
        assert(lhs.second > 1  &&  rhs.second > 1);
        assert(rhs.first == lhs.first + lhs.second);

        std::vector<value_type> temp(lhs.first, lhs.first + lhs.second);
        It cursor1 = temp.begin();
        It cursor2 = rhs.first;
        It dest    = lhs.first;

        int minGallop = minGallop_;
        scope_call<std::function<void (void)>> fn_exit([&](){
            minGallop_ = std::min(minGallop, 1);
            if (lhs.second == 1)
            {
                assert(rhs.second > 1);
                std::move(cursor2, cursor2 + rhs.second - 1, dest);
                *(dest + rhs.second - 1) = *cursor1;
            }
            else
            {
                assert(rhs.second == 1);
                assert(lhs.second > 1);
                std::move(cursor1, cursor1 + lhs.second, dest);
            }
        });

        *dest++ = *cursor2++;
        while (true)
        {
            if (merge_low_limit_to_gallop(lhs, rhs, cursor1, cursor2, minGallop, dest))
                return;

            if (merge_low_with_gallop(lhs, rhs, cursor1, cursor2, minGallop, dest))
                return;

            // adjust the run length limit repeat merging
            minGallop = std::max(minGallop, 0) + 2;
        }
    }

    void merge_high(pending_t lhs, pending_t rhs)
    {
        // the two buffers (left and right) must both be at least 1 element long,
        // and must be adjacent with lhs on the left
        assert(lhs.second > 1  &&  rhs.second > 1);
        assert(rhs.first == lhs.first + lhs.second);

        std::vector<value_type> temp(rhs.first, rhs.first + rhs.second);
        It cursor1 = lhs.first    + (lhs.second - 1);
        It cursor2 = temp.begin() + (rhs.second - 1);
        It dest    = rhs.first    + (rhs.second - 1);

        int minGallop = minGallop_;
        scope_call<std::function<void (void)>> fn_exit([&](){
            minGallop_ = std::min(minGallop, 1);

            if (rhs.second == 1)
            {
                assert(lhs.second > 1);
                dest    -= lhs.second - 1;
                cursor1 -= lhs.second - 2;
                std::move_backward(cursor1, cursor1 + (lhs.second-1), dest + lhs.second);
                *dest = *cursor2;
            }
            else
            {
                assert(lhs.second == 1);
                assert(rhs.second > 1);
                std::move(temp.begin(), temp.begin() + rhs.second, dest - (rhs.second - 1));
            }
        });

        *dest-- = *cursor1--;
        while (true)
        {
            if (merge_high_limit_to_gallop(lhs, rhs, cursor1, cursor2, minGallop, dest))
                return;

            if (merge_high_with_gallop(lhs, rhs, cursor1, cursor2, minGallop, dest))
                return;

            // adjust the run length limit repeat merging
            minGallop = std::max(minGallop, 0) + 2;
        }
    }
};

}   // namespace detail


template<typename It, typename Pred=std::less<typename std::iterator_traits<It>::value_type>>
inline void timsort(It const first, It const last, Pred compare)
{
    detail::timsort<It, Pred>::sort(first, last, compare);
}

template<typename It>
inline void timsort(It const first, It const last)
{
    timsort(first, last, std::less<typename std::iterator_traits<It>::value_type>());
}

}   // namespace cdmh

#pragma warning(pop)

/**
 * @file   Sort.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_SORT_HPP_
#define _IWBAN_SORT_HPP_

#include <Global.hpp>

namespace ut
{

/**
 * Bidirectional bubble sort.
 *
 * The iterators needs to be bidirectional.
 */
template<class I, class C = std::less<decltype(I::operator*())> >
void doubleBubbleSort(I begin, I end, C comparator = C())
{
    if (begin == end) return;

    I i, j;
    I done = begin;

    while (begin != end)
    {
        // Bubble up
        i = begin;
        j = begin + 1;
        while (j != end)
        {
            if (comparator(j, i))
            {
                std::swap(i, j);
                done = i;
            }

            i = j++;
        }

        // Break if bubble up didn't swapped anything
        if (done == begin) break;

        // New upper bound
        end = done;

        // Bubble down
        // TODO Will probably crash, since j goes out of bounds when i is begin
        i = end;
        j = end - 1;
        while (i != begin)
        {
            if (comparator(i, j))
            {
                std::swap(i, j);
                done = j;
            }

            j = i--;
        }

        // New lower bound
        begin = done;
    }
}
// bubbleSort()

}
// namespace ut

#endif // _IWBAN_SORT_HPP_

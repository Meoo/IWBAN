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
        j = begin; // begin + 1
        i = j++;   // begin
        while (j != end)
        {
            if (comparator(*j, *i))
            {
                std::swap(*i, *j);
                done = j;
            }

            i = j++;
        }

        // Break if bubble up didn't swapped anything
        if (done == begin) break;

        // New upper bound
        end = done;

        // Bubble down
        // TODO Will probably crash, since j goes out of bounds when i is begin
        j = end; // end - 1
        i = j--; // end
        while (i != begin)
        {
            if (comparator(*j, *i))
            {
                std::swap(*i, *j);
                done = i;
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

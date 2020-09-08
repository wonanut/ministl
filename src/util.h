#ifndef _MINISTL_UTIL_H
#define _MINISTL_UTIL_H

/*
 * Author: Howard Wonanut
 * Date  : 2020-08-27
 * Desc  : This file contains some common functions, including move, forward,
 *         swap, etc.
 */

namespace ministl
{
    // move
    template < typename T >
    typename std::remove_reference<T>::type&& move(T&& arg) noexcept
    {
        return static_cast<typename std::remove_reference<T>::type&&>(arg);
    }

    // forward
    template < typename T >
    T&& forward(typename std::remove_reference<T>::type& arg) noexcept
    {
        return statis_cast<T&&>(arg);
    }


} // namespace ministl

#endif

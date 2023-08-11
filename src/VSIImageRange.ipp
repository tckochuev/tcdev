#ifndef VSIIMAGERANGE_IPP
#define VSIIMAGERANGE_IPP

#include <algorithm>
#include <cassert>

template<typename ImageT>
auto VSImageRange<ImageT>::begin() -> iterator
{
    return static_cast<const VSImageRange*>(this)->begin();
}

template<typename ImageT>
auto VSImageRange<ImageT>::begin() const -> const_iterator
{
    return doBegin();
}

template<typename ImageT>
auto VSImageRange<ImageT>::cbegin() const -> const_iterator
{
    return begin();
}

template<typename ImageT>
auto VSImageRange<ImageT>::end() -> iterator
{
    return static_cast<const VSImageRange*>(this)->end();
}

template<typename ImageT>
auto VSImageRange<ImageT>::end() const -> const_iterator
{
    return doEnd();
}

template<typename ImageT>
auto VSImageRange<ImageT>::cend() const -> const_iterator
{
    return end();
}

template<typename ImageT>
auto VSImageRange<ImageT>::size() const -> size_type
{
    return doSize();
}

template<typename ImageT>
auto VSImageRange<ImageT>::get(size_type i) const-> value_type
{
    assert(i >= 0 && i < size());
    return doGet();
}

template<typename ImageT>
auto VSImageRange<ImageT>::doSize() const-> size_type
{
    return static_cast<size_type>(std::distance(begin(), end()));
}

#endif

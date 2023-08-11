#ifndef VSIMAGEPROVIDERRANGE_IPP
#define VSIMAGEPROVIDERRANGE_IPP

#include "VSImageProviderRange.h"

#include <boost/iterator/transform_iterator.hpp>

template<typename ImageProviderT>
auto VSImageProviderRange<ImageProviderT>::begin() -> iterator
{
    return std::begin(m_providers);
}

template<typename ImageProviderT>
auto VSImageProviderRange<ImageProviderT>::begin() const -> const_iterator
{
    return std::begin(m_providers);
}

template<typename ImageProviderT>
auto VSImageProviderRange<ImageProviderT>::cbegin() const -> const_iterator
{
    return begin();
}

template<typename ImageProviderT>
auto VSImageProviderRange<ImageProviderT>::end() -> iterator
{
    return std::end(m_providers);
}

template<typename ImageProviderT>
auto VSImageProviderRange<ImageProviderT>::end() const -> const_iterator
{
    return std::end(m_providers);
}

template<typename ImageProviderT>
auto VSImageProviderRange<ImageProviderT>::cend() const -> const_iterator
{
    return end();
}

template<typename ImageProviderT>
auto VSImageProviderRange<ImageProviderT>::size() const -> size_type
{
    return m_providers.size();
}

template<typename ImageProviderT>
template<typename InputIterator, typename OutputIterator>
void VSImageProviderRange<ImageProviderT>::set(InputIterator first, InputIterator last, OutputIterator output)
{
    std::move(m_providers.begin(), m_providers.end(), output);
    set(first, last);
}

template<typename ImageProviderT>
template<typename InputIterator>
void VSImageProviderRange<ImageProviderT>::set(InputIterator first, InputIterator last)
{
    m_providers.clear();
    std::move(first, last, std::back_inserter(m_providers));
    updated();
}

template<typename ImageProviderT>
auto VSImageProviderRange<ImageProviderT>::doGet(Super::size_type i) const -> Super::value_type
{
    return *(doBegin() + i);
}

template<typename ImageProviderT>
auto VSImageProviderRange<ImageProviderT>::doBegin() const -> Super::const_iterator
{
    return Super::const_iterator(boost::transform_iterator(begin(), &std::invoke<const_reference>));
}

template<typename ImageProviderT>
auto VSImageProviderRange<ImageProviderT>::doEnd() const -> Super::const_iterator
{
    return Super::const_iterator(boost::transform_iterator(end(), &std::invoke<const_reference>));
}

template<typename ImageProviderT>
auto VSImageProviderRange<ImageProviderT>::doSize() const -> Super::size_type
{
    return static_cast<Super::size_type>(size());
}

#endif

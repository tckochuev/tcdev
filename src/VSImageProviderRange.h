#ifndef VSIMAGEPROVIDERRANGE_H
#define VSIMAGEPROVIDERRANGE_H

#include <functional>
#include <vector>

#include "VSIImageRange.h"

/*ImageProvider is a movable type with operator() -> tc::err::Result<ImageType, std::error_code>*/
template<typename ImageProviderT>
class VSImageProviderRange : public VSImageRange<tc::err::Value<std::invoke_result_t<ImageProviderT>>>
{
public:
    using Super = VSImageRange<tc::err::Value<std::invoke_result_t<ImageProviderT>>>;
    using ImageProvider = ImageProviderT;

private:
    using Container = std::vector<ImageProvider>;

public:
    using value_type = typename Container::value_type;
    using const_reference = typename Container::const_reference;
    using reference = const_reference;
    using const_pointer = typename Container::const_pointer;
    using pointer = const_pointer;
    using size_type = typename Container::size_type;
    using const_iterator = typename Container::const_iterator;
    using iterator = const_iterator;

    VSImageProviderRange() = default;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

    size_type size() const;

    template<typename InputIterator, typename OutputIterator>
    void set(InputIterator first, InputIterator last, OutputIterator output);

    template<typename InputIterator>
    void set(InputIterator first, InputIterator last);

private:
    Super::const_iterator doBegin() const override;
    Super::const_iterator doEnd() const override;
    Super::value_type doGet(Super::size_type i) const override;
    Super::size_type doSize() const override;

    Container m_providers;
};

#include "VSImageProviderRange.ipp"

#endif

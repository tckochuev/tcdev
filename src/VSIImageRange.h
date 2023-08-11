#ifndef VSIIMAGERANGE_H
#define VSIIMAGERANGE_H

#include <iterator>

#include <boost/range/any_range.hpp>

#include "VSError.h"

template<typename ImageT>
class VSImageRange
{
public:
    using Image = ImageT;
    using value_type = tc::err::Result<Image>;
    using const_reference = value_type;
    using reference = const_reference;
    using iterator = boost::range_iterator<boost::any_range<value_type, boost::single_pass_traversal_tag, reference>>::type;
    using const_iterator = iterator;
    using pointer = std::iterator_traits<iterator>::pointer;
    using const_pointer = std::iterator_traits<const_iterator>::const_pointer;
    using size_type = size_t;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

    size_type size() const;

    value_type get(size_type i) const;

    virtual ~VSImageRange() = default;

    /* SIGNAL */ void updated() {}

protected:
    VSImageRange() = default;
    VSImageRange(const VSImageRange&) = default;
    VSImageRange(VSImageRange&&) = default;
    VSImageRange& operator=(const VSImageRange&) = default;
    VSImageRange& operator=(VSImageRange&&) = default;

private:
    virtual const_iterator doBegin() const = 0;
    virtual const_iterator doEnd() const = 0;
    virtual value_type doGet() const = 0;
    virtual size_type doSize() const;
};

#include "VSIImageRange.ipp"

#endif

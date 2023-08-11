#include <iostream>

#include <vector>
#include <set>
#include "VSUtils.h"

#include <boost/range.hpp>
#include <boost/range/any_range.hpp>

#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/any_cast.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/iterator.hpp>
#include <boost/type_erasure/same_type.hpp>
#include <boost/type_erasure/relaxed.hpp>
#include <boost/type_erasure/typeid_of.hpp>

#include <boost/type_erasure/builtin.hpp>
#include <boost/type_erasure/operators.hpp>

template<typename ContainerT>
class ContainerAdaptor : public tc::ReversibleContainerAdaptor<ContainerT>
{
public:
    using Super = tc::ReversibleContainerAdaptor<ContainerT>;
    using Container = typename Super::Container;

    ContainerAdaptor(const Container& c) : m_container(c) {}

private:
    const Container& container() const override {
        return m_container;
    }

    Container m_container;
};

//namespace bte = boost::type_erasure;
//
//using Concept = boost::mpl::vector<
//    bte::copy_constructible<bte::_a>,
//    bte::copy_constructible<bte::_b>,
//    bte::copy_constructible<bte::_c>,
//    bte::addable<bte::_a, bte::_b, bte::_a>,
//    bte::dereferenceable<bte::_c, bte::_a>,
//    bte::ostreamable<std::ostream, bte::_a>,
//    bte::ostreamable<std::ostream, bte::_b>,
//    bte::ostreamable<std::ostream, bte::_c>
//>;
//template<typename Placeholder>
//using Any = bte::any<Concept, Placeholder>;

inline namespace bte
{

template<
    typename Value,
    typename Traversal,
    typename Reference = Value&,
    typename Difference = std::ptrdiff_t
>
using AnyIterator = boost::type_erasure::any<
    boost::mpl::vector<
        boost::type_erasure::iterator<
            Traversal,
            boost::type_erasure::_self,
            Reference,
            Difference
        >,
        boost::type_erasure::same_type<
            typename boost::type_erasure::iterator<
                Traversal,
                boost::type_erasure::_self,
                Reference,
                Difference
            >::value_type,
            std::remove_cv_t<Value>
        >,
        boost::type_erasure::relaxed,
        boost::type_erasure::typeid_<>
    >
>;

}

namespace br
{

template<
    typename Value,
    typename Traversal,
    typename Reference = Value&,
    typename Difference = std::ptrdiff_t,
    typename Buffer = boost::iterators::use_default
>
using AnyIterator = typename boost::range_iterator<
    boost::any_range<
        Value,
        Traversal,
        Reference,
        Difference,
        Buffer
    >
>::type;

}

int main()
{
    std::ostream_iterator<int> ostreamIterator(std::cout, " ");
    ContainerAdaptor<std::vector<int>> adaptor({1, 2, 3, 4, 5});
    for (int i : adaptor) {
        *ostreamIterator++ = i;
    }
    std::endl(std::cout);

    //int i[2] = {0, 1};
    //using Mapping = boost::mpl::map<
    //    boost::mpl::pair<bte::_a, int*>,
    //    boost::mpl::pair<bte::_b, int>,
    //    boost::mpl::pair<bte::_c, int>
    //>;
    //Any<bte::_a> a(i, bte::make_binding<Mapping>());
    //Any<bte::_b> b(1, bte::make_binding<Mapping>());
    //std::cout << *(a + b) << std::endl;

    AnyIterator<const int, boost::random_access_traversal_tag> first(adaptor.begin());
    decltype(first) last;
    last = decltype(first)(adaptor.end());
    std::for_each(first, last, [&ostreamIterator](std::iterator_traits<decltype(first)>::reference value) {
        *ostreamIterator++ = value;
    });
    std::endl(std::cout);

    auto retrieved = boost::type_erasure::any_cast<boost::range_iterator<decltype(adaptor)>::type>(first);
    *ostreamIterator++ = *retrieved;
    std::endl(std::cout);

    std::set<int> s{1, 2, 3};
    std::cout << tc::contains(s, 1) << " " << tc::contains(adaptor, 10) << std::endl;

    return 0;
}
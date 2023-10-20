#include <iostream>

#include <vector>
#include <set>
#include "Utils.h"
//#include "SequencedSet.h"

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

#include <queue>
#include <boost/iterator/indirect_iterator.hpp>
#include <boost/iterator/function_input_iterator.hpp>

#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

//template<typename ContainerT>
//class ContainerAdaptor : public tc::ReversibleContainerAdaptor<ContainerT>
//{
//public:
//    using Super = tc::ReversibleContainerAdaptor<ContainerT>;
//    using Container = typename Super::Container;
//
//    ContainerAdaptor(const Container& c) : m_container(c) {}
//
//private:
//    const Container& container() const override {
//        return m_container;
//    }
//
//    Container m_container;
//};

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

//inline namespace bte
//{
//
//template<
//    typename Value,
//    typename Traversal,
//    typename Reference = Value&,
//    typename Difference = std::ptrdiff_t
//>
//using AnyIterator = boost::type_erasure::any<
//    boost::mpl::vector<
//        boost::type_erasure::iterator<
//            Traversal,
//            boost::type_erasure::_self,
//            Reference,
//            Difference
//        >,
//        boost::type_erasure::same_type<
//            typename boost::type_erasure::iterator<
//                Traversal,
//                boost::type_erasure::_self,
//                Reference,
//                Difference
//            >::value_type,
//            std::remove_cv_t<Value>
//        >,
//        boost::type_erasure::relaxed,
//        boost::type_erasure::typeid_<>
//    >
//>;
//
//}
//
//namespace br
//{
//
//template<
//    typename Value,
//    typename Traversal,
//    typename Reference = Value&,
//    typename Difference = std::ptrdiff_t,
//    typename Buffer = boost::iterators::use_default
//>
//using AnyIterator = typename boost::range_iterator<
//    boost::any_range<
//        Value,
//        Traversal,
//        Reference,
//        Difference,
//        Buffer
//    >
//>::type;
//
//}

struct Presentation
{
    int id;
    std::string title;
    std::string avatarSource;
    std::string dirName;
    std::vector<std::string> slidesFilePaths;
};

namespace boost {
namespace serialization {

template<typename Archive>
void serialize(Archive& ar, Presentation& p, const unsigned int version)
{
    ar& p.id;
    ar& p.title;
    ar& p.avatarSource;
    ar& p.dirName;
    ar & p.slidesFilePaths;
}

} // namespace serialization
} // namespace boost

int main()
{
    //std::ostream_iterator<int> ostreamIterator(std::cout, " ");
    //ContainerAdaptor<std::vector<int>> adaptor({1, 2, 3, 4, 5});
    //for (int i : adaptor) {
    //    *ostreamIterator++ = i;
    //}
    //std::endl(std::cout);

    //int i[2] = {0, 1};
    //using Mapping = boost::mpl::map<
    //    boost::mpl::pair<bte::_a, int*>,
    //    boost::mpl::pair<bte::_b, int>,
    //    boost::mpl::pair<bte::_c, int>
    //>;
    //Any<bte::_a> a(i, bte::make_binding<Mapping>());
    //Any<bte::_b> b(1, bte::make_binding<Mapping>());
    //std::cout << *(a + b) << std::endl;

    //AnyIterator<const int, boost::random_access_traversal_tag> first(adaptor.begin());
    //decltype(first) last;
    //last = decltype(first)(adaptor.end());
    //std::for_each(first, last, [&ostreamIterator](std::iterator_traits<decltype(first)>::reference value) {
    //    *ostreamIterator++ = value;
    //});
    //std::endl(std::cout);

    //auto retrieved = boost::type_erasure::any_cast<boost::range_iterator<decltype(adaptor)>::type>(first);
    //*ostreamIterator++ = *retrieved;
    //std::endl(std::cout);

    //std::set<int> s{1, 2, 3};
    //std::cout << tc::contains(s, 1) << " " << tc::contains(adaptor, 10) << std::endl;

    std::stringstream sstrm;
    boost::archive::text_oarchive oa(sstrm);
    Presentation p{1, "title", "avatarSource", "dirName", {"a", "b", "c"}};
    oa << p;
    std::cout << sstrm.tellg() << std::endl;
    Presentation p2;
    boost::archive::text_iarchive ia(sstrm);
    ia >> p2;

    sstrm.clear();
    sstrm.seekg(0);
    sstrm.seekp(0);
    std::vector<Presentation> v(10);
    oa << v;
    std::vector<Presentation> v2;
    ia >> v2;

    struct A
    {
        A(int i) : i(i) {}
        A(const A& other) {
            i = other.i;
            std::cout << "const A&" << std::endl;
        }
        A(A&& other) noexcept {
            i = other.i;
            std::cout << "A&&" << std::endl;
        }

        int i;
    };

    std::list<A> input({A(1), A(2), A(3), A(4), A(5)});
    std::list<A> output;
    std::move(tc::EraseIterator(input, std::begin(input)), tc::EraseIterator(input, std::end(input)), std::back_inserter(output));
    //tc::container::ISequencedSet<tc::container::Orderable<int>>* s = nullptr;

    return 0;
}
#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/iterator.hpp>
#include <boost/type_erasure/same_type.hpp>
#include <boost/type_erasure/relaxed.hpp>
#include <boost/type_erasure/typeid_of.hpp>
#include <boost/range/any_range.hpp>
#include <boost/type_erasure/any.hpp>

namespace tc
{
namespace bte //for boost type erasure
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

} //bte

inline namespace br // for boost range. Faster, but without ability to retrieve erased type.
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

} //br
}

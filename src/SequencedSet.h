#ifndef SEQUENCED_SET
#define SEQUENCED_SET

#include <type_traits>

#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include "ValueProperties.h"
#include "VSUtils.h"

namespace tc::container
{
namespace detail
{
template<typename ValueProperty, bool RandomAccess>
using Container = boost::multi_index_container<
	typename ValueProperty::Value,
	boost::multi_index::indexed_by<
		std::conditional_t<
			RandomAccess,
			boost::multi_index::random_access<boost::multi_index::tag<tc::tag::Sequence>>,
			boost::multi_index::sequenced<boost::multi_index::tag<tc::tag::Sequence>>
		>,
		std::conditional_t<
			tc::container::isOrderable<ValueProperty>,
			boost::multi_index::ordered_unique<
				boost::multi_index::tag<tc::tag::Set>,
				boost::multi_index::identity<typename ValueProperty::Value>,
				typename ValueProperty::Comparator
			>,
			boost::multi_index::hashed_unique<
				boost::multi_index::tag<tc::tag::Set>,
				boost::multi_index::identity<typename ValueProperty::Value>,
				typename ValueProperty::Hash,
				typename ValueProperty::KeyEqual
			>
		>
	>
>;
}

}

#endif

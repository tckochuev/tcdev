#include "VSContainer.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/random_access_index.hpp>

namespace tc::container::seqset
{

namespace tag
{

template<typename TValue, typename TComparator = std::less<TValue>>
struct Orderable
{
	using Value = TValue;
	using Comparator = TComparator;
};
template<typename ValueProperty>
inline constexpr bool isOrderable = false;
template<typename Value, typename Comparator>
inline constexpr bool isOrderable<Orderable<Value, Comparator>> = true;

template<typename TValue, typename THash = std::hash<TValue>, typename TKeyEqual = std::equal_to<TValue>>
struct HashableAndEquatable
{
	using Value = TValue;
	using Hash = THash;
	using KeyEqual = TKeyEqual;
};
template<typename ValueProperty>
inline constexpr bool isHashableAndEquatable = false;
template<typename Value, typename Hash, typename KeyEqual>
inline constexpr bool isHashableAndEquatable<HashableAndEquatable<Value, Hash, KeyEqual>> = true;

struct Sequence {};
struct Set {};

}

namespace detail
{

template<typename ValueProperty, bool randomAccess>
using Container = boost::multi_index_container<
	typename ValueProperty::Value,
	boost::multi_index::indexed_by<
		std::conditional_t<
			randomAccess,
			boost::multi_index::random_access<boost::multi_index::tag<tc::container::seqset::tag::Sequence>>,
			boost::multi_index::sequenced<boost::multi_index::tag<tc::container::seqset::tag::Sequence>>
		>,
		std::conditional_t<
			tc::container::seqset::tag::isOrderable<ValueProperty>,
			boost::multi_index::ordered_unique<
				boost::multi_index::tag<tc::container::seqset::tag::Set>,
				boost::multi_index::identity<typename ValueProperty::Value>,
				typename ValueProperty::Comparator
			>,
			boost::multi_index::hashed_unique<
				boost::multi_index::tag<tc::container::seqset::tag::Set>,
				boost::multi_index::identity<typename ValueProperty::Value>,
				typename ValueProperty::Hash,
				typename ValueProperty::KeyEqual
			>
		>
	>
>;

}

template<typename Derived, typename ValueProperty, bool randomAccess = true>
class Wrapper :
	public tc::container::seq::Wrapper<
		tc::container::revers::Wrapper<
			Derived,
			detail::Container<ValueProperty, randomAccess>
		>
	>
{
public:
	using value_type = typename Wrapper::value_type;
	using iterator = typename Wrapper::iterator;

	std::pair<iterator, bool> insert(iterator pos, const value_type& v);
	std::pair<iterator, bool> insert(iterator pos, value_type&& v);

	bool replace(iterator pos, const value_type& v);
	bool replace(iterator pos, value_type&& v);
	template<typename Modifier>
	bool modify(iterator pos, Modifier mod);
	template<typename Modifier, typename Rollback>
	bool modify(iterator pos, Modifier mod, Rollback back);

	void relocate(iterator pos, iterator target);
	void relocate(iterator pos, iterator first, iterator last);
	template<typename InputIterator> void rearrange(InputIterator first);

protected
	template<typename T>
	std::pair<iterator, bool> doTryInsert(iterator pos, const value_type& v);

	//template<typename T>

};

}
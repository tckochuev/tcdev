#ifndef SEQUENCED_SET
#define SEQUENCED_SET

#include <type_traits>
#include <list>
#include <variant>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include "Container.h"
#include "AnyIterator.h"
#include "Utils.h"

namespace tc::container
{
namespace detail
{
namespace bstmi
{
namespace tag
{
struct Sequence {};
struct Set {};
} //tag

namespace bstmi = boost::multi_index;

template<bool randomAccess>
using Sequence = std::conditional_t<
	randomAccess,
	bstmi::random_access<bstmi::tag<tag::Sequence>>,
	bstmi::sequenced<bstmi::tag<tag::Sequence>>
>;

template<typename ValueProperty>
using Set = std::conditional_t<
	isOrderable<ValueProperty>,
	bstmi::ordered_unique<
		bstmi::tag<tag::Set>,
		bstmi::identity<typename ValueProperty::Value>,
		typename ValueProperty::Comparator
	>,
	bstmi::hashed_unique<
		bstmi::tag<tag::Set>,
		bstmi::identity<typename ValueProperty::Value>,
		typename ValueProperty::Hash,
		typename ValueProperty::KeyEqual
	>
>;

template<typename ValueProperty, bool randomAccess>
using Container = bstmi::multi_index_container<
	typename ValueProperty::Value,
	bstmi::indexed_by<
		Sequence<randomAccess>,
		Set<ValueProperty>
	>
>;
} //bstmi

template<typename ValueProperty, bool randomAccess>
class ISequencedSetBase : public IReversibleContainerAdaptor<bstmi::Container<ValueProperty, randomAccess>>
{};

template<typename ValueProperty>
class ISequencedSetBase<ValueProperty, true> : public IReversibleContainerAdaptor<bstmi::Container<ValueProperty, true>>
{
public:
	using typename ISequencedSetBase::size_type;

	/*virtual*/ size_type capacity() const {
		return doCapacity();
	}
	///@brief If capacity was >= m, nothing is done, otherwise - the internal capacity is changed so that capacity >= m.
	/*virtual*/ void reserve(size_type m) {
		doReserve(m);
	}
	/*virtual*/ void shrink_to_fit() {
		doShrinkToFit();
	}

protected:
	using ISequencedSetBase::container;

	virtual size_type doCapacity() const {
		return container().capacity();
	}
	virtual void doReserve(size_type m) const {
		return container().reserve(m);
	}
	virtual void doShrinkToFit() {
		return container().shrink_to_fit();
	}
};

} //detail

template<typename ValueProperty, bool randomAccess = true>
class ISequencedSet : public detail::ISequencedSetBase<ValueProperty, randomAccess>
{
public:
	using typename ISequencedSet::value_type;
	using typename ISequencedSet::Container::key_type;
	using typename ISequencedSet::iterator;
	using ISequencedSet::begin;
	using ISequencedSet::end;

	iterator find(const key_type& key) const;
	///@returns A pair consisting of an iterator to the inserted element (or to the element that prevented the insertion) and
	/// a bool value set to true if and only if the insertion took place.
	/*virtual*/ std::pair<iterator, bool> insert(iterator pos, const value_type& value);
	/*virtual*/ std::pair<iterator, bool> insert(iterator pos, value_type&& value);

	///@pre When overriden, must be defined when first != last, and pos not in [first, last],
	/// in default implementation pos not in [first, last).
	/*virtual*/ void move(iterator pos, iterator first, iterator last);
	///@brief same as move(pos, e, std::next(e))
	void move(iterator pos, iterator e);

	template<typename InputIterator>
	/*virtual*/ void rearrange(iterator first, iterator last, InputIterator permutation);
	///@brief Same as rearrange(begin(), end(), permutation).
	template<typename InputIterator>
	void rearrange(InputIterator permutation);

	///@pre e != end.
	/*virtual*/ bool replace(iterator e, const value_type& value);
	/*virtual*/ bool replace(iterator e, value_type&& value);

	///@pre pos != end.
	/*virtual*/ iterator erase(iterator pos);
	///@brief when overriden, must be defined when first != last,
	/// in default implementation it is no-op, when first == last.
	/*virtual*/ iterator erase(iterator first, iterator last);

	///@pre When overriden, must be defined when this container is not empty,
	/// in default implementation it is no-op, when container is empty.
	/*virtual*/ void clear();

	bool keysEqual(const key_type& key1, const key_type& key2) const;

protected:
	using ISequencedSet_ = ISequencedSet;
	using typename ISequencedSet::Container;
	using ISequencedSet::container;
	using Set = detail::bstmi::Set<ValueProperty>;
	using SetIterator = typename Set::iterator;
	//using Buffer = std::list<value_type>;
	using Copy = std::reference_wrapper<const value_type>;
	using Move = std::reference_wrapper<value_type>;
	using CopyOrMove = std::variant<Copy, Move>;

	static const value_type& get(CopyOrMove value);
	template<typename Callable>
	static auto invoke(Callable callable, CopyOrMove argument);

	virtual std::pair<iterator, bool> tryInsert(iterator pos, CopyOrMove value);
	/// @brief Default implementation calls this function, when insert will take place (value is not already in this container).
	virtual iterator doInsert(SetIterator setPos, iterator pos, CopyOrMove value);
	///@pre All elements in buffer are unique and not already in this container (insertion of all elements must take place).
	///virtual void doInsertToSequence(iterator pos, Buffer& buffer);
	virtual void doMove(iterator pos, iterator first, iterator last);
	virtual bool doReplace(iterator e, CopyOrMove value);
	virtual void doClear();

	Set& set();
	const Set& set() const;
};

} //tc::container

#include "SequencedSet.ipp"

#endif

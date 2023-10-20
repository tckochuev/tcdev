#ifndef CONTAINER
#define CONTAINER

#include <type_traits>
#include <functional>

#include "Utils.h"

namespace tc::container
{

template
<
	typename ConstIterator,
	typename Iterator = ConstIterator
>
class IIterable
{
public:
	using const_iterator = ConstIterator;
	using iterator = Iterator;

	/*virtual*/ iterator begin() {
		return doBegin();
	}
	/*virtual*/ const_iterator begin() const {
		return doBegin();
	}
	const_iterator cbegin() const {
		return begin();
	}
	/*virtual*/ iterator end() {
		return doEnd();
	}
	/*virtual*/ const_iterator end() const {
		return doEnd();
	}
	const_iterator cend() const {
		return end();
	}

	virtual ~IIterable() = default;

protected:
	using IIterable_ = IIterable;

	virtual iterator doBegin() = 0;
	virtual const_iterator doBegin() const {
		return const_iterator(const_cast<IIterable*>(this)->begin());
	}
	virtual iterator doEnd() = 0;
	virtual const_iterator doEnd() const {
		return const_iterator(const_cast<IIterable*>(this)->end());
	}
};

template
<
	typename ConstReverseIterator,
	typename ReverseIterator = ConstReverseIterator
>
class IReverseIterable
{
public:
	using const_reverse_iterator = ConstReverseIterator;
	using reverse_iterator = ReverseIterator;

	/*virtual*/ reverse_iterator rbegin() {
		return doRBegin();
	}
	/*virtual*/ const_reverse_iterator rbegin() const {
		return doRBegin();
	}
	const_reverse_iterator crbegin() const {
		return rbegin();
	}
	/*virtual*/ reverse_iterator rend() {
		return doREnd();
	}
	/*virtual*/ const_reverse_iterator rend() const {
		return doREnd();
	}
	const_reverse_iterator crend() const {
		return rend();
	}

	virtual ~IReverseIterable() = default;

protected:
	using IReverseIterable_ = IReverseIterable;

	virtual reverse_iterator doRBegin() = 0;
	virtual const_reverse_iterator doRBegin() const {
		return const_reverse_iterator(const_cast<IReverseIterable*>(this)->rbegin());
	}
	virtual reverse_iterator doREnd() = 0;
	virtual const_reverse_iterator doREnd() const {
		return const_reverse_iterator(const_cast<IReverseIterable*>(this)->rend());
	}
};

template<typename TContainer>
class IContainerTypesHolder
{
	INTERFACE(IContainerTypesHolder);

protected:
	using IContainerTypesHolder_ = IContainerTypesHolder;
	using Container = std::remove_const_t<TContainer>;

public:
	using value_type = typename Container::value_type;
	using reference = typename Container::reference;
	using const_reference = typename Container::const_reference;
	using const_iterator = typename Container::const_iterator;
	using iterator = std::conditional_t<std::is_const_v<TContainer>, const_iterator, typename Container::iterator>;
	using difference_type = typename Container::difference_type;
	using size_type = typename Container::size_type;

	virtual ~IContainerTypesHolder() = default;
};

template<typename ReversibleContainer>
class IReversibleContainerTypesHolder : public virtual IContainerTypesHolder<ReversibleContainer>
{
	INTERFACE(IReversibleContainerTypesHolder);

public:
	using const_reverse_iterator = typename ReversibleContainer::const_reverse_iterator;
	using reverse_iterator = std::conditional_t<
		std::is_const_v<ReversibleContainer>,
		const_reverse_iterator,
		typename ReversibleContainer::reverse_iterator
	>;

protected:
	using IReversibleContainerTypesHolder_ = IReversibleContainerTypesHolder;
};

template<typename TContainer>
class IContainerAdaptor :
	public virtual IContainerTypesHolder<TContainer>,
	public IIterable<
		typename IContainerTypesHolder<TContainer>::const_iterator,
		typename IContainerTypesHolder<TContainer>::iterator
	>
{
public:
	using iterator = typename IContainerAdaptor::iterator;
	using const_iterator = typename IContainerAdaptor::const_iterator;
	using size_type = typename IContainerAdaptor::size_type;

	/*virtual*/ size_type max_size() const {
		return doMaxSize();
	}
	/*virtual*/ size_type size() const {
		return doSize();
	}
	/*virtual*/ bool empty() const {
		return doEmpty();
	}

protected:
	using IContainerAdaptor_ = IContainerAdaptor;
	using typename IContainerAdaptor::Container;

	iterator doBegin() override {
		return container().begin();
	}
	const_iterator doBegin() const override {
		return container().begin();
	}
	iterator doEnd() override {
		return container().end();
	}
	const_iterator doEnd() const override {
		return container().end();
	}
	virtual size_type doMaxSize() const {
		return container().max_size();
	}
	virtual size_type doSize() const {
		return container().size();
	}
	virtual bool doEmpty() const {
		return container().empty();
	}

	Container& container() {
		return const_cast<Container&>(static_cast<const IContainerAdaptor*>(this)->container());
	}
	virtual const Container& container() const = 0;
};

template<typename ReversibleContainer>
class IReversibleContainerAdaptor :
	public IContainerAdaptor<ReversibleContainer>,
	public IReversibleContainerTypesHolder<ReversibleContainer>,
	public IReverseIterable<
		typename IReversibleContainerTypesHolder<ReversibleContainer>::const_reverse_iterator,
		typename IReversibleContainerTypesHolder<ReversibleContainer>::iterator
	>
{
public:
	using reverse_iterator = typename IReversibleContainerAdaptor::reverse_iterator;
	using const_reverse_iterator = typename IReversibleContainerAdaptor::const_reverse_iterator;

protected:
	using IReversibleContainerAdaptor_ = IReversibleContainerAdaptor;
	using IReversibleContainerAdaptor::container;

	reverse_iterator doRBegin() override {
		return container().rbegin();
	};
	const_reverse_iterator doRBegin() const override {
		return container().rbegin();
	}
	reverse_iterator doREnd() override {
		return container().rend();
	}
	const_reverse_iterator doREnd() const override {
		return container().rend();
	}
};

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

}

#endif

#ifndef VSUTILS_H
#define VSUTILS_H

#include <type_traits>

#include <boost/range.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/any_range.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/iterator.hpp>
#include <boost/type_erasure/same_type.hpp>
#include <boost/type_erasure/relaxed.hpp>
#include <boost/type_erasure/typeid_of.hpp>

#define DECLARE_DEFAULT_COPY_MOVE_BY_DEFAULT(ClassName)\
ClassName() = default;\
ClassName(const ClassName&) = default;\
ClassName& operator=(const ClassName&) = default;\
ClassName(ClassName&&) noexcept = default;\
ClassName& operator=(ClassName&&) noexcept = default;

namespace tc
{

namespace detail
{

template<typename Void, template<typename...> typename Target, typename... Args>
struct Detector {
	static inline constexpr bool Value = false;
};

template<template<typename...> typename Target, typename... Args>
struct Detector<std::void_t<Target<Args...>>, Target, Args...>
{
	static inline constexpr bool Value = true;
	using Type = Target<Args...>;
};

}

/**
 * @brief Provides the ability to detect functionality of types at compile time.
 */
template<template<typename...> typename Target, typename... Args>
inline constexpr bool isDetected = typename detail::Detector<void, Target, Args...>::Value;

template<template<typename...> typename Target, typename... Args>
using Detected = typename detail::Detector<void, Target, Args...>::Type;

namespace detail
{

template<typename T, typename Arg>
using memberFunctionFind = decltype(std::declval<T>().find(std::declval<Arg>()));

}

template<typename InputRange, typename Value>
typename boost::range_iterator<std::remove_reference_t<InputRange>>::type
find(InputRange&& range, const Value& value)
{
	if constexpr (isDetected<detail::memberFunctionFind, InputRange&, decltype(value)>) {
		return range.find(value);
	}
	else {
		return boost::find(range, value);
	}
}

template<typename InputRange, typename Value>
bool contains(InputRange&& range, const Value& value) {
	return find(range, value) != boost::end(range);
}

namespace tag
{

struct Sequence {};
struct Set {};

}

template<
	typename ConstIterator,
	typename Iterator = ConstIterator,
	typename = void
>
class IIterable
{
public:
	using const_iterator = ConstIterator;
	using iterator = Iterator;

	virtual iterator begin() = 0;
	virtual const_iterator begin() const = 0;
	const_iterator cbegin() const {
		return begin();
	}
	virtual iterator end() = 0;
	virtual const_iterator end() const = 0;
	const_iterator cend() const {
		return end();
	}

	virtual ~IIterable() = default;
};

template<
	typename ConstIterator,
	typename Iterator
>
class IIterable<ConstIterator, Iterator, std::enable_if_t<std::is_constructible_v<Iterator, ConstIterator&&>>>
{
public:
	using const_iterator = ConstIterator;
	using iterator = Iterator;

	virtual iterator begin() {
		return iterator(static_cast<const IIterable*>(this)->begin());
	};
	virtual const_iterator begin() const = 0;
	const_iterator cbegin() const {
		return begin();
	}
	virtual iterator end() {
		return iterator(static_cast<const IIterable*>(this)->end());
	}
	virtual const_iterator end() const = 0;
	const_iterator cend() const {
		return end();
	}

	virtual ~IIterable() = default;
};

template<
	typename ConstReverseIterator,
	typename ReverseIterator = ConstReverseIterator,
	typename = void
>
class IReverseIterable
{
public:
	using const_reverse_iterator = ConstReverseIterator;
	using reverse_iterator = ReverseIterator;

	virtual reverse_iterator rbegin() = 0;
	virtual const_reverse_iterator rbegin() const = 0;
	const_reverse_iterator crbegin() const {
		return rbegin();
	}
	virtual reverse_iterator rend() = 0;
	virtual const_reverse_iterator rend() const = 0;
	const_reverse_iterator crend() const {
		return rend();
	}

	virtual ~IReverseIterable() = default;
};

template<
	typename ConstReverseIterator,
	typename ReverseIterator
>
class IReverseIterable<ConstReverseIterator, ReverseIterator, std::enable_if_t<std::is_constructible_v<ReverseIterator, ConstReverseIterator&&>>>
{
public:
	using const_reverse_iterator = ConstReverseIterator;
	using reverse_iterator = ReverseIterator;

	virtual reverse_iterator rbegin() {
		return reverse_iterator(static_cast<const IReverseIterable*>(this)->rbegin());
	};
	virtual const_reverse_iterator rbegin() const = 0;
	const_reverse_iterator crbegin() const {
		return rbegin();
	}
	virtual reverse_iterator rend() {
		return reverse_iterator(static_cast<const IReverseIterable*>(this)->rend());
	}
	virtual const_reverse_iterator rend() const = 0;
	const_reverse_iterator crend() const {
		return rend();
	}

	virtual ~IReverseIterable() = default;
};

template<typename Container, bool exposeNonConstIterator = false>
class AbstractContainerTypesHolder
{
public:
	using value_type = typename Container::value_type;
	using reference = typename Container::reference;
	using const_reference = typename Container::const_reference;
	using const_iterator = typename Container::const_iterator;
	using iterator = std::conditional_t<exposeNonConstIterator, typename Container::iterator, const_iterator>;
	using difference_type = typename Container::difference_type;
	using size_type = typename Container::size_type;

	virtual ~AbstractContainerTypesHolder() = default;

protected:
	DECLARE_DEFAULT_COPY_MOVE_BY_DEFAULT(AbstractContainerTypesHolder);
};

template<typename ReversibleContainer, bool exposeNonConstIterator = false>
class AbstractReversibleContainerTypesHolder : public virtual AbstractContainerTypesHolder<ReversibleContainer, exposeNonConstIterator>
{
public:
	using const_reverse_iterator = typename ReversibleContainer::const_reverse_iterator;
	using reverse_iterator = std::conditional_t<exposeNonConstIterator, typename ReversibleContainer::reverse_iterator, const_reverse_iterator>;

	virtual ~AbstractReversibleContainerTypesHolder() = default;

protected:
	DECLARE_DEFAULT_COPY_MOVE_BY_DEFAULT(AbstractReversibleContainerTypesHolder);
};

template<typename TContainer, bool exposeNonConstIterator = false>
class AbstractContainerAdaptor :
	public virtual AbstractContainerTypesHolder<TContainer, exposeNonConstIterator>,
	public IIterable<
		typename TContainer::const_iterator,
		std::conditional_t<exposeNonConstIterator, typename TContainer::iterator, typename TContainer::const_iterator>
	>
{
private:
	using TypesHolder = AbstractContainerTypesHolder<TContainer, exposeNonConstIterator>;

public:
	using iterator = typename TypesHolder::iterator;
	using const_iterator = typename TypesHolder::const_iterator;
	using size_type = typename TypesHolder::size_type;

	iterator begin() override {
		return container().begin();
	}
	const_iterator begin() const override {
		return container().begin();
	}
	iterator end() override {
		return container().end();
	}
	const_iterator end() const override {
		return container().end();
	}
	size_type max_size() const {
		return container().max_size();
	}
	size_type size() const {
		return container().size();
	}
	bool empty() const {
		return container().empty();
	}

	virtual ~AbstractContainerAdaptor() = default;

protected:
	using Container = TContainer;

	Container& container() {
		return const_cast<Container&>(static_cast<const AbstractContainerAdaptor*>(this)->container());
	}
	virtual const Container& container() const = 0;
};

template<typename ReversibleContainer, bool exposeNonConstIterator = false>
class ReversibleContainerAdaptor :
	public AbstractContainerAdaptor<ReversibleContainer, exposeNonConstIterator>,
	public AbstractReversibleContainerTypesHolder<ReversibleContainer, exposeNonConstIterator>,
	public IReverseIterable<
		typename ReversibleContainer::const_reverse_iterator,
		std::conditional_t<
			exposeNonConstIterator,
			typename ReversibleContainer::reverse_iterator,
			typename ReversibleContainer::const_reverse_iterator
		>
	>
{
private:
	using TypesHolder = AbstractReversibleContainerTypesHolder<Container, exposeNonConstIterator>;

public:
	using reverse_iterator = typename TypesHolder::reverse_iterator;
	using const_reverse_iterator = typename TypesHolder::const_reverse_iterator;

	reverse_iterator rbegin() override {
		return container().rbegin();
	};
	const_reverse_iterator rbegin() const override {
		return container().rbegin();
	}
	reverse_iterator rend() override {
		return container().rend();
	}
	const_reverse_iterator rend() const override {
		return container().rend();
	}

	virtual ~ReversibleContainerAdaptor() = default;

protected:
	using AbstractContainerAdaptor<ReversibleContainer, exposeNonConstIterator>::container;
};

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

inline namespace br // for boost range. Much faster, but no possibility to retrieve erased type.
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

} //tc

#endif

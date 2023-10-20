#ifndef UTILS_H
#define UTILS_H

#include <type_traits>
#include <functional>
#include <cassert>

#include <boost/range.hpp>
#include <boost/range/algorithm.hpp>

#include <boost/system/system_error.hpp>

#define DECLARE_DEFAULT_COPY_MOVE_CTORS_BY_DEFAULT(ClassName)\
ClassName() = default;\
ClassName(const ClassName&) = default;\
ClassName(ClassName&&) noexcept = default;

#define DECLARE_COPY_MOVE_ASSIGN_BY_DEFAULT(ClassName)\
ClassName& operator=(const ClassName&) = default;\
ClassName& operator=(ClassName&&) noexcept = default;

#define INTERFACE(ClassName)\
protected:\
DECLARE_DEFAULT_COPY_MOVE_CTORS_BY_DEFAULT(ClassName)\
DECLARE_COPY_MOVE_ASSIGN_BY_DEFAULT(ClassName)

namespace tc
{

namespace syserr = boost::system;

template<typename T1, typename T2, typename Comparator>
inline bool equals(const T1& v1, const T2& v2, Comparator comparator) {
	return !std::invoke(comparator, v1, v2) && !std::invoke(comparator, v2, v1);
}

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

///@brief Shadows(except iterator category - always input iterator) and holds Iterator(it) of Container(container) and calls it = container.erase(it), when incremented.
template<typename Container, typename Iterator = typename Container::iterator>
class EraseIterator
{
public:
	using iterator = Iterator;
	using difference_type = typename std::iterator_traits<iterator>::difference_type;
	using value_type = typename std::iterator_traits<iterator>::value_type;
	using pointer = typename std::iterator_traits<iterator>::pointer;
	using reference = typename std::iterator_traits<iterator>::reference;
	using iterator_category = std::input_iterator_tag;

	EraseIterator(Container& container, iterator it) : container(container), it(it)
	{}

	template<
		typename OtherContainer,
		typename OtherIterator,
		std::enable_if_t<
			std::is_convertible_v<std::reference_wrapper<OtherContainer>, std::reference_wrapper<Container>> &&
			std::is_convertible_v<OtherIterator, Iterator>,
			bool
		> = true
	>
	EraseIterator(const EraseIterator<OtherContainer, OtherIterator>& other) : container(other.container), it(other.it)
	{}

	bool operator==(const EraseIterator& other) const
	{
		assert(it != other.it || &(container.get()) == &(other.container.get()));
		return it == other.it;
	}
	bool operator!=(const EraseIterator& other) const {
		return !(*this == other);
	}
	reference operator*() const {return *it;}
	EraseIterator& operator++()
	{
		it = container.get().erase(it);
		return *this;
	}
	void operator++(int) {++(*this);}

	std::reference_wrapper<Container> container;
	iterator it;
};

} //tc

#endif

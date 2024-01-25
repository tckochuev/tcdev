#ifndef CONTAINER
#define CONTAINER

#include <type_traits>
#include <functional>
#include <cassert>
#include <iterator>

#include <boost/iterator/iterator_facade.hpp>

#include "VSUtils.h"

namespace tc::container
{

//template
//<
//	typename ConstIterator,
//	typename Iterator = ConstIterator
//>
//class IIterable
//{
//public:
//	using const_iterator = ConstIterator;
//	using iterator = Iterator;

//	/*virtual*/ iterator begin() {
//		return doBegin();
//	}
//	/*virtual*/ const_iterator begin() const {
//		return doBegin();
//	}
//	const_iterator cbegin() const {
//		return begin();
//	}
//	/*virtual*/ iterator end() {
//		return doEnd();
//	}
//	/*virtual*/ const_iterator end() const {
//		return doEnd();
//	}
//	const_iterator cend() const {
//		return end();
//	}

//	virtual ~IIterable() = default;

//protected:
//	using IIterable_ = IIterable;

//	virtual iterator doBegin() = 0;
//	virtual const_iterator doBegin() const {
//		return const_iterator(const_cast<IIterable*>(this)->begin());
//	}
//	virtual iterator doEnd() = 0;
//	virtual const_iterator doEnd() const {
//		return const_iterator(const_cast<IIterable*>(this)->end());
//	}
//};

//template
//<
//	typename ConstReverseIterator,
//	typename ReverseIterator = ConstReverseIterator
//>
//class IReverseIterable
//{
//public:
//	using const_reverse_iterator = ConstReverseIterator;
//	using reverse_iterator = ReverseIterator;

//	/*virtual*/ reverse_iterator rbegin() {
//		return doRBegin();
//	}
//	/*virtual*/ const_reverse_iterator rbegin() const {
//		return doRBegin();
//	}
//	const_reverse_iterator crbegin() const {
//		return rbegin();
//	}
//	/*virtual*/ reverse_iterator rend() {
//		return doREnd();
//	}
//	/*virtual*/ const_reverse_iterator rend() const {
//		return doREnd();
//	}
//	const_reverse_iterator crend() const {
//		return rend();
//	}

//	virtual ~IReverseIterable() = default;

//protected:
//	using IReverseIterable_ = IReverseIterable;

//	virtual reverse_iterator doRBegin() = 0;
//	virtual const_reverse_iterator doRBegin() const {
//		return const_reverse_iterator(const_cast<IReverseIterable*>(this)->rbegin());
//	}
//	virtual reverse_iterator doREnd() = 0;
//	virtual const_reverse_iterator doREnd() const {
//		return const_reverse_iterator(const_cast<IReverseIterable*>(this)->rend());
//	}
//};

//template<typename TContainer>
//class IContainerTypesHolder
//{
//	INTERFACE(IContainerTypesHolder);

//protected:
//	using IContainerTypesHolder_ = IContainerTypesHolder;
//	using Container = std::remove_const_t<TContainer>;

//public:
//	using value_type = typename Container::value_type;
//	using reference = typename Container::reference;
//	using const_reference = typename Container::const_reference;
//	using const_iterator = typename Container::const_iterator;
//	using iterator = std::conditional_t<std::is_const_v<TContainer>, const_iterator, typename Container::iterator>;
//	using difference_type = typename Container::difference_type;
//	using size_type = typename Container::size_type;
//};

//template<typename ReversibleContainer>
//class IReversibleContainerTypesHolder : public virtual IContainerTypesHolder<ReversibleContainer>
//{
//	INTERFACE(IReversibleContainerTypesHolder);

//public:
//	using const_reverse_iterator = typename ReversibleContainer::const_reverse_iterator;
//	using reverse_iterator = std::conditional_t<
//		std::is_const_v<ReversibleContainer>,
//		const_reverse_iterator,
//		typename ReversibleContainer::reverse_iterator
//	>;

//protected:
//	using IReversibleContainerTypesHolder_ = IReversibleContainerTypesHolder;
//};

//template<typename TContainer>
//class IContainerAdaptor :
//	public virtual IContainerTypesHolder<TContainer>,
//	public IIterable<
//		typename IContainerTypesHolder<TContainer>::const_iterator,
//		typename IContainerTypesHolder<TContainer>::iterator
//	>
//{
//public:
//	using iterator = typename IContainerAdaptor::iterator;
//	using const_iterator = typename IContainerAdaptor::const_iterator;
//	using size_type = typename IContainerAdaptor::size_type;

//	/*virtual*/ size_type max_size() const {
//		return doMaxSize();
//	}
//	/*virtual*/ size_type size() const {
//		return doSize();
//	}
//	/*virtual*/ bool empty() const {
//		return doEmpty();
//	}

//protected:
//	using IContainerAdaptor_ = IContainerAdaptor;
//	using typename IContainerAdaptor::Container;

//	iterator doBegin() override {
//		return container().begin();
//	}
//	const_iterator doBegin() const override {
//		return container().begin();
//	}
//	iterator doEnd() override {
//		return container().end();
//	}
//	const_iterator doEnd() const override {
//		return container().end();
//	}
//	virtual size_type doMaxSize() const {
//		return container().max_size();
//	}
//	virtual size_type doSize() const {
//		return container().size();
//	}
//	virtual bool doEmpty() const {
//		return container().empty();
//	}

//	Container& container() {
//		return const_cast<Container&>(static_cast<const IContainerAdaptor*>(this)->container());
//	}
//	virtual const Container& container() const = 0;
//};

//template<typename ReversibleContainer>
//class IReversibleContainerAdaptor :
//	public IContainerAdaptor<ReversibleContainer>,
//	public IReversibleContainerTypesHolder<ReversibleContainer>,
//	public IReverseIterable<
//		typename IReversibleContainerTypesHolder<ReversibleContainer>::const_reverse_iterator,
//		typename IReversibleContainerTypesHolder<ReversibleContainer>::iterator
//	>
//{
//public:
//	using reverse_iterator = typename IReversibleContainerAdaptor::reverse_iterator;
//	using const_reverse_iterator = typename IReversibleContainerAdaptor::const_reverse_iterator;

//protected:
//	using IReversibleContainerAdaptor_ = IReversibleContainerAdaptor;
//	using IReversibleContainerAdaptor::container;

//	reverse_iterator doRBegin() override {
//		return container().rbegin();
//	};
//	const_reverse_iterator doRBegin() const override {
//		return container().rbegin();
//	}
//	reverse_iterator doREnd() override {
//		return container().rend();
//	}
//	const_reverse_iterator doREnd() const override {
//		return container().rend();
//	}
//};

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

template<
	typename Size,
	typename ConstIterator,
	typename Iterator = ConstIterator,
	typename Value = typename std::iterator_traits<Iterator>::value_type,
	typename Reference = typename std::iterator_traits<Iterator>::reference,
	typename ConstReference = typename std::iterator_traits<ConstIterator>::reference,
	typename Difference = typename std::iterator_traits<Iterator>::difference_type
>
struct Traits
{
	using value_type = Value;
	using reference = Reference;
	using const_reference = ConstReference;
	using iterator = Iterator;
	using const_iterator = ConstIterator;
	using difference_type = Difference;
	using size_type = Size;
};

template<
	typename Container,
	typename Size = typename Container::size_type,
	typename ConstIterator = typename Container::const_iterator,
	typename Iterator = ConstIterator,
	typename Value = typename std::iterator_traits<Iterator>::value_type,
	typename Reference = typename std::iterator_traits<Iterator>::reference,
	typename ConstReference = typename std::iterator_traits<ConstIterator>::reference,
	typename Difference = typename std::iterator_traits<Iterator>::difference_type
>
struct DefaultTraits :
	Traits<Size, ConstIterator, Iterator, Value, Reference, ConstReference, Difference>
{};

template<
	typename TDerived,
	typename TContainer,
	typename TTraits = DefaultTraits<TContainer>
>
class Wrapper : public TTraits
{
public:
	using Derived = TDerived;
	using Container = TContainer;
	using Traits = TTraits;
	using iterator = typename Wrapper::iterator;
	using const_iterator = typename Wrapper::const_iterator;
	using size_type = typename Wrapper::size_type;

	iterator begin() {
		return Accessor::begin(derived());
	}
	const_iterator begin() const {
		return Accessor::begin(derived());
	}
	const_iterator cbegin() const {
		return begin();
	}
	iterator end() {
		return Accessor::end(derived());
	}
	const_iterator end() const {
		return Accessor::end(derived());
	}
	const_iterator cend() const {
		return end();
	}
	size_type max_size() const {
		return Accessor::max_size(derived());
	}
	size_type size() const {
		return Accessor::size(derived());
	}
	bool empty() const {
		return Accessor::empty(derived());
	}
	void swap(Derived& other) {
		return Accessor::swap(derived(), other);
	}

protected:
	Wrapper() = default;
	Wrapper(const Wrapper&) = default;
	Wrapper(Wrapper&&) noexcept = default;
	Wrapper& operator=(const Wrapper&) = default;
	Wrapper& operator=(Wrapper&&) noexcept = default;

	template<typename... Args>
	struct Enabler {
		static inline constexpr bool value = true;
	};
	template<typename T>
	struct Enabler<T> {
		static inline constexpr bool value = !std::is_base_of_v<Wrapper, T>;
	};
	template<
		typename... Args,
		typename = std::enable_if_t<Enabler<std::decay_t<Args>...>::value>
	>
	Wrapper(Args&&... args) : m_container(std::forward<Args>(args)...) {}

	iterator doBegin() {
		return m_container.begin();
	}
	const_iterator doBeginConst() const {
		return m_container.begin();
	}
	iterator doEnd() {
		return m_container.end();
	}
	const_iterator doEndConst() const {
		return m_container.end();
	}
	size_type doMaxSize() const {
		return m_container.max_size();
	}
	size_type doSize() const {
		return m_container.size();
	}
	bool doEmpty() const {
		return m_container.empty();
	}
	void doSwap(Derived& other) {
		m_container.swap(static_cast<Wrapper&>(other).m_container);
	}
	bool doEqual(const Derived& other) const {
		return m_container == static_cast<const Wrapper&>(other).m_container;
	}
	Derived& derived() {
		return static_cast<Derived&>(*this);
	}
	const Derived& derived() const{
		return static_cast<const Derived&>(*this);
	}

	Container m_container;
private:
	struct Accessor : Derived
	{
		static iterator begin(Derived& d) {
			return (d.*&Accessor::doBegin)();
		}
		static const_iterator begin(const Derived& d) {
			return (d.*&Accessor::doBeginConst)();
		}
		static iterator end(Derived& d) {
			return (d.*&Accessor::doEnd)();
		}
		static const_iterator end(const Derived& d) {
			return (d.*&Accessor::doEndConst)();
		}
		static size_type max_size(const Derived& d) {
			return (d.*&Accessor::doMaxSize)();
		}
		static size_type size(const Derived& d) {
			return (d.*&Accessor::doSize)();
		}
		static bool empty(const Derived& d) {
			return (d.*&Accessor::doEmpty)();
		}
		static void swap(Derived& d, Derived& other) {
			return (d.*&Accessor::doSwap)(other);
		}
		static bool equal(const Derived& d, const Derived& other) {
			return (d.*&Accessor::doEqual)(other);
		}
	};
	template<typename D, typename C, typename CT>
	friend bool operator==(const Wrapper<D, C, CT>&, const Wrapper<D, C, CT>&);
};

template<
	typename D,
	typename C,
	typename CT
>
bool operator==(const Wrapper<D, C, CT>& lhs, const Wrapper<D, C, CT>& rhs)
{
	return Wrapper<D, C, CT>::Accessor::equal(lhs.derived(), rhs.derived());
}

template<
	typename D,
	typename C,
	typename CT
>
bool operator!=(
	const Wrapper<D, C, CT>& lhs,
	const Wrapper<D, C, CT>& rhs
)
{
	return !(lhs == rhs);
}

template<
	typename D,
	typename C,
	typename CT
>
void swap(
	Wrapper<D, C, CT>& lhs,
	Wrapper<D, C, CT>& rhs
)
{
	return lhs.swap(static_cast<D&>(rhs));
}

namespace revers
{

template<
	typename Size,
	typename ConstIterator,
	typename ConstReverseIterator,
	typename Iterator = ConstIterator,
	typename ReverseIterator = ConstReverseIterator,
	typename Value = typename std::iterator_traits<Iterator>::value_type,
	typename Reference = typename std::iterator_traits<Iterator>::reference,
	typename ConstReference = typename std::iterator_traits<ConstIterator>::reference,
	typename Difference = typename std::iterator_traits<Iterator>::difference_type
>
struct Traits :
	tc::container::Traits<
		Size, ConstIterator, Iterator, Value, Reference, ConstReference, Difference
	>
{
	using reverse_iterator = ReverseIterator;
	using const_reverse_iterator = ConstReverseIterator;
};

template<
	typename Container,
	typename Size = typename Container::size_type,
	typename ConstIterator = typename Container::const_iterator,
	typename ConstReverseIterator = typename Container::const_reverse_iterator,
	typename Iterator = ConstIterator,
	typename ReverseIterator = ConstReverseIterator,
	typename Value = typename std::iterator_traits<Iterator>::value_type,
	typename Reference = typename std::iterator_traits<Iterator>::reference,
	typename ConstReference = typename std::iterator_traits<ConstIterator>::reference,
	typename Difference = typename std::iterator_traits<Iterator>::difference_type
>
struct DefaultTraits :
	Traits<
		Size, ConstIterator, ConstReverseIterator, Iterator, ReverseIterator,
		Value, Reference, ConstReference, Difference
	>
{};

template<
	typename Derived,
	typename Container,
	typename Traits = DefaultTraits<Container>
>
class Wrapper :
	public tc::container::Wrapper<
		Derived,
		Container,
		Traits
	>
{
public:
	using reverse_iterator = typename Wrapper::reverse_iterator;
	using const_reverse_iterator = typename Wrapper::const_reverse_iterator;

	reverse_iterator rbegin() {
		return Accessor::rbegin(derived());
	}
	const_reverse_iterator rbegin() const {
		return Accessor::rbegin(derived());
	}
	const_reverse_iterator crbegin() const {
		return rbegin();
	}
	reverse_iterator rend() {
		return Accessor::rend(derived());
	}
	const_reverse_iterator rend() const {
		return Accessor::rend(derived());
	}
	const_reverse_iterator crend() const {
		return rend();
	}

protected:
	using Super = tc::container::Wrapper<Derived, Container, Traits>;
	using Super::m_container;
	using Super::derived;

	using Super::Super;
	Wrapper() = default;
	Wrapper(const Wrapper&) = default;
	Wrapper(Wrapper&&) noexcept = default;
	Wrapper& operator=(const Wrapper&) = default;
	Wrapper& operator=(Wrapper&&) noexcept = default;

	reverse_iterator doRBegin() {
		return m_container.rbegin();
	};
	const_reverse_iterator doRBeginConst() const {
		return m_container.rbegin();
	}
	reverse_iterator doREnd() {
		return m_container.rend();
	}
	const_reverse_iterator doREndConst() const {
		return m_container.rend();
	}

private:
	struct Accessor : Derived
	{
		static reverse_iterator rbegin(Derived& d) {
			return (d.*&Accessor::doRBegin)();
		}
		static const_reverse_iterator rbegin(const Derived& d) {
			return (d.*&Accessor::doRBeginConst)();
		}
		static reverse_iterator rend(Derived& d) {
			return (d.*&Accessor::doREnd)();
		}
		static const_reverse_iterator rend(const Derived& d) {
			return (d.*&Accessor::doREndConst)();
		}
	};
};

}

template<typename Value, typename State = ptrdiff_t>
class SameValueIterator :
	public boost::iterator_facade<SameValueIterator<Value, State>, Value, std::random_access_iterator_tag>
{
public:
	template<typename V, typename S>
	SameValueIterator(V&& value, S&& state) : value(std::forward<V>(value)), state(std::forward<S>(state))
	{}

private:
	typename SameValueIterator::reference dereference() const {
		return value;
	}
	bool equal(const SameValueIterator& other) const {
		return state == other.state;
	}
	void increment() {
		++state;
	}
	void decrement() {
		--state;
	}
	void advance(typename SameValueIterator::difference_type n) {
		state += n;
	}
	typename SameValueIterator::difference_type distance_to(const SameValueIterator& other) const {
		return other.state - state;
	}

	std::remove_const_t<Value> value;
	State state;

	friend class boost::iterator_core_access;
};

namespace seq
{
/**@tparam Base is either ReversibleContainerWrapper or ContainerWrapper*/
template<typename Base>
class Wrapper : public Base
{
public:
	using Derived = typename Base::Derived;
	using value_type = typename Wrapper::value_type;
	using iterator = typename Wrapper::iterator;
	using const_iterator = typename Wrapper::const_iterator;
	using size_type = typename Wrapper::size_type;
	using Base::begin;
	using Base::end;
	using Base::empty;

	Wrapper() : Wrapper(static_cast<value_type*>(nullptr), static_cast<value_type*>(nullptr)) {}
	Wrapper(size_type n, const value_type& v) :
		Wrapper(
			SameValueIterator<const std::reference_wrapper<const value_type>>(v, 0),
			SameValueIterator<const std::reference_wrapper<const value_type>>(v, n)
		)
	{}
	template<typename InputIterator>
	Wrapper(InputIterator first, InputIterator last) :
		Base(first, last)
	{}
	Wrapper(std::initializer_list<value_type> il) :
		Wrapper(il.begin(), il.end())
	{}

	Derived& operator=(std::initializer_list<value_type> il) {
		assign(il);
		return derived();
	}
	template<typename InputIterator>
	iterator insert(const_iterator pos, InputIterator first, InputIterator last) {
		return Accessor::insert(derived(), pos, first, last);
	}
	iterator insert(const_iterator pos, const value_type& v) {
		return Accessor::insert(derived(), pos, v);
	}
	iterator insert(const_iterator pos, value_type&& v) {
		return Accessor::insert(derived(), pos, std::move(v));
	}
	iterator insert(const_iterator pos, size_type n, const value_type& v) {
		return Accessor::insert(derived(), pos, n, v);
	}
	iterator insert(const_iterator pos, std::initializer_list<value_type> il) {
		return Accessor::insert(derived(), pos, il);
	}
	iterator erase(const_iterator pos) {
		return Accessor::erase(derived(), pos);
	}
	iterator erase(const_iterator first, const_iterator last) {
		return Accessor::erase(derived(), first, last);
	}
	void clear() {
		Accessor::clear(derived());
	}
	template<typename InputIterator>
	void assign(InputIterator first, InputIterator last) {
		Accessor::assign(derived(), first, last);
	}
	void assign(std::initializer_list<value_type> il) {
		Accessor::assign(derived(), il);
	}
	void assign(size_type n, const value_type& v) {
		Accessor::assign(derived(), n, v);
	}

protected:
	using Base::m_container;
	using Base::derived;

	using Base::Base;
	Wrapper(const Wrapper&) = default;
	Wrapper(Wrapper&&) noexcept = default;
	Wrapper& operator=(const Wrapper&) = default;
	Wrapper& operator=(Wrapper&&) noexcept = default;

	template<typename InputIterator>
	iterator doInsert(const_iterator pos, InputIterator first, InputIterator last) {
		return m_container.insert(pos, first, last);
	}
	template<typename V>
	iterator doInsertOne(const_iterator pos, V&& v) {
		if constexpr (std::is_rvalue_reference_v<decltype(v)>) {
			return insert(pos, std::move_iterator(&v), std::move_iterator(std::next(&v)));
		}
		else {
			return insert(pos, &v, std::next(&v));
		}
	}
	iterator doInsertN(const_iterator pos, size_type n, const value_type& v) {
		auto [first, last] = makeValueIteratorRange(n, v);
		return insert(pos, first, last);
	}
	iterator doInsertIL(const_iterator pos, std::initializer_list<value_type> il) {
		return insert(pos, il.begin(), il.end());
	}
	iterator doEraseOne(const_iterator pos) {
		assert(pos != this->end());
		return erase(pos, std::next(pos));
	}
	iterator doErase(const_iterator first, const_iterator last) {
		return m_container.erase(first, last);
	}
	void doClear() {
		erase(begin(), end());
		assert(empty());
	}
	template<typename InputIterator>
	void doAssign(InputIterator first, InputIterator last) {
		clear();
		insert(end(), first, last);
	}
	void doAssignIL(std::initializer_list<value_type> il) {
		assign(il.begin(), il.end());
	}
	void doAssignN(size_type n, const value_type& v) {
		auto [first, last] = makeValueIteratorRange(n, v);
		assign(first, last);
	}

	auto makeValueIteratorRange(size_type n, const value_type& v) {
		SameValueIterator<const std::reference_wrapper<const value_type>>
			first(std::cref(v), 0), last(std::cref(v), static_cast<ptrdiff_t>(n));
		static_assert(
			std::is_base_of_v<
				std::random_access_iterator_tag,
				typename std::iterator_traits<decltype(first)>::iterator_category
			>
		);
		return std::pair(first, last);
	}

private:
	struct Accessor : Derived
	{
		template<typename InputIterator>
		static iterator insert(Derived& d, const_iterator pos, InputIterator first, InputIterator last) {
			auto memfn = &Accessor::template doInsert<InputIterator>;
			return (d.*memfn)(pos, first, last);
		}
		template<typename V>
		static iterator insert(Derived& d, const_iterator pos, V&& v) {
			return (d.*&Accessor::template doInsertOne<V>)(pos, std::forward<V>(v));
		}
		static iterator insert(Derived& d, const_iterator pos, size_type n, const value_type& v) {
			return (d.*&Accessor::doInsertN)(pos, n, v);
		}
		static iterator insert(Derived& d, const_iterator pos, std::initializer_list<value_type> il) {
			return (d.*&Accessor::doInsertIL)(pos, il);
		}
		static iterator erase(Derived& d, const_iterator pos) {
			return (d.*&Accessor::doEraseOne)(pos);
		}
		static iterator erase(Derived& d, const_iterator first, const_iterator last) {
			return (d.*&Accessor::doErase)(first, last);
		}
		static void clear(Derived& d) {
			(d.*&Accessor::doClear)();
		}
		template<typename InputIterator>
		static void assign(Derived& d, InputIterator first, InputIterator last) {
			auto memfn = &Accessor::template doAssign<InputIterator>;
			(d.*memfn)(first, last);
		}
		static void assign(Derived& d, std::initializer_list<value_type> il) {
			(d.*&Accessor::doAssignIL)(il);
		}
		static void assign(Derived& d, size_type n, const value_type& v) {
			(d.*&Accessor::doAssignN)(n, v);
		}
	};
};

}

namespace assoc
{
namespace base
{

template<typename Key>
struct Traits
{
	using key_type = Key;
};

template<
	typename Base,
	typename Traits
>
class Wrapper : public Base, public Traits
{
public:
	using Derived = typename Base::Derived;
	using value_type = typename Wrapper::value_type;
	using iterator = typename Wrapper::iterator;
	using const_iterator = typename Wrapper::const_iterator;
	using key_type = typename Wrapper::key_type;
	using size_type = typename Wrapper::size_type;
	using Base::size;
	using Base::begin;
	using Base::end;

	Derived& operator=(std::initializer_list<value_type> il) {
		Accessor::assign(derived(), il);
		return derived();
	}
	iterator insert(const_iterator pos, const value_type& v) {
		return Accessor::insert(derived(), pos, v);
	}
	iterator insert(const_iterator pos, value_type&& v) {
		return Accessor::insert(derived(), pos, std::move(v));
	}
	template<typename InputIterator>
	void insert(InputIterator first, InputIterator last) {
		return Accessor::insert(derived(), first, last);
	}
	void insert(std::initializer_list<value_type> il) {
		return Accessor::insert(derived(), il);
	}
	template<typename Key>
	size_type erase(Key&& key) {
		return Accessor::erase(derived(), key);
	}
	iterator erase(const_iterator pos) {
		return Accessor::erase(derived(), pos);
	}
	iterator erase(const_iterator first, const_iterator last) {
		return Accessor::erase(derived(), first, last);
	}
	void clear() {
		return Accessor::clear(derived());
	}
	template<typename Key>
	iterator find(Key&& key) {
		return Accessor::find(derived(), std::forward<Key>(key));
	}
	template<typename Key>
	const_iterator find(Key&& key) const {
		return Accessor::find(derived(), std::forward<Key>(key));
	}
	template<typename Key>
	size_type count(Key&& key) const {
		return Accessor::count(derived(), std::forward<Key>(key));
	}
	template<typename Key>
	iterator lower_bound(Key&& key) {
		return Accessor::lower_bound(derived(), std::forward<Key>(key));
	}
	template<typename Key>
	const_iterator lower_bound(Key&& key) const {
		return Accessor::lower_bound(derived(), std::forward<Key>(key));
	}
	template<typename Key>
	iterator upper_bound(Key&& key) {
		return Accessor::upper_bound(derived(), std::forward<Key>(key));
	}
	template<typename Key>
	const_iterator upper_bound(Key&& key) const {
		return Accessor::upper_bound(derived(), std::forward<Key>(key));
	}
	template<typename Key>
	std::pair<iterator, iterator> equal_range(Key&& key) {
		return Accessor::equal_range(derived(), std::forward<Key>(key));
	}
	template<typename Key>
	std::pair<const_iterator, const_iterator> equal_range(Key&& key) const {
		return Accessor::equal_range(derived(), std::forward<Key>(key));
	}
protected:
	using Base::derived;
	using Base::m_container;

	using Base::Base;
	Wrapper(const Wrapper&) = default;
	Wrapper(Wrapper&&) noexcept = default;
	Wrapper& operator=(const Wrapper&) = default;
	Wrapper& operator=(Wrapper&&) noexcept = default;

	const key_type& key(const value_type& v) const {
		return Accessor::key(derived(), v);
	}
	const key_type& doKey(const value_type& v) const {
		if constexpr (std::is_same_v<key_type, value_type>) {
			return v;
		}
		else {
			const auto& [key, mapped] = v;
			return key;
		}
	}
	template<typename V>
	iterator doInsertHint(const_iterator pos, V&& v) {
		return m_container.insert(pos, std::forward<V>(v));
	}
	template<typename InputIterator>
	void doInsertRange(InputIterator first, InputIterator last) {
		std::for_each(first, last, [](auto&& v) {
			derived().insert(std::forward<delctype(v)>(v));
		});
	}
	void doInsertIL(std::initializer_list<value_type> il) {
		insert(il.begin(), il.end());
	}
	void doAssign(std::initializer_list<value_type> il) {
		clear();
		insert(il);
	}
	template<typename Key>
	size_type doEraseKey(Key&& key) {
		size_type oldSize = size();
		auto [first, last] = equal_range(std::forward<Key>(key));
		erase(first, last);
		return oldSize - size();
	}
	iterator doEraseOne(const_iterator pos) {
		assert(pos != end());
		return erase(pos, std::next(pos));
	}
	iterator doErase(const_iterator first, const_iterator last) {
		return m_container.erase(first, last);
	}
	void doClear() {
		erase(begin(), end());
	}
	template<typename Key>
	iterator doFind(Key&& key) {
		return m_container.find(std::forward<Key>(key));
	}
	template<typename Key>
	const_iterator doFindConst(Key&& key) const {
		return m_container.find(std::forward<Key>(key));
	}
	template<typename Key>
	size_type doCount(Key&& key) const {
		return m_container.count(std::forward<Key>(key));
	}
	template<typename Key>
	iterator doLowerBound(Key&& key) {
		return m_container.lower_bound(std::forward<Key>(key));
	}
	template<typename Key>
	const_iterator doLowerBoundConst(Key&& key) const {
		return m_container.lower_bound(std::forward<Key>(key));
	}
	template<typename Key>
	iterator doUpperBound(Key&& key) {
		return m_container.upper_bound(std::forward<Key>(key));
	}
	template<typename Key>
	const_iterator doUpperBoundConst(Key&& key) const {
		return m_container.upper_bound(std::forward<Key>(key));
	}
	template<typename Key>
	std::pair<iterator, iterator> doEqualRange(Key&& key) {
		return m_container.equal_range(std::forward<Key>(key));
	}
	template<typename Key>
	std::pair<const_iterator, const_iterator> doEqualRangeConst(Key&& key) const {
		return m_container.equal_range(std::forward<Key>(key));
	}
private:
	struct Accessor : Derived
	{
		static const key_type& key(const Derived& d, const value_type& v) {
			return (d.*&Accessor::doKey)(v);
		}
		template<typename V>
		static iterator insert(Derived& d, const_iterator pos, V&& v) {
			return (d.*&Accessor::template doInsertHint<V>)(pos, std::forward<V>(v));
		}
		template<typename InputIterator>
		static void insert(Derived& d, InputIterator first, InputIterator last) {
			auto memfn = &Accessor::template doInsertRange<InputIterator>;
			(d.*memfn)(first, last);
		}
		static void insert(Derived& d, std::initializer_list<value_type> il) {
			(d.*&Accessor::doInsertIL)(il);
		}
		static void assign(Derived& d, std::initializer_list<value_type> il) {
			(d.*&Accessor::doAssign)(il);
		}
		template<typename Key>
		static size_type erase(Derived& d, Key&& key) {
			auto memfn = &Accessor::template doEraseKey<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
		static iterator erase(Derived& d, const_iterator pos) {
			return (d.*&Accessor::doEraseOne)(pos);
		}
		static iterator erase(Derived& d, const_iterator first, const_iterator last) {
			return (d.*&Accessor::doErase)(first, last);
		}
		static void clear(Derived& d) {
			return (d.*&Accessor::doClear)();
		}
		template<typename Key>
		static iterator find(Derived& d, Key&& key) {
			auto memfn = &Accessor::template doFind<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
		template<typename Key>
		static const_iterator find(const Derived& d, Key&& key) {
			auto memfn = &Accessor::template doFindConst<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
		template<typename Key>
		static size_type count(const Derived& d, Key&& key) {
			auto memfn = &Accessor::template doCount<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
		template<typename Key>
		static iterator lower_bound(Derived& d, Key&& key) {
			auto memfn = &Accessor::template doLowerBound<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
		template<typename Key>
		static const_iterator lower_bound(const Derived& d, Key&& key) {
			auto memfn = &Accessor::template doLowerBoundConst<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
		template<typename Key>
		static iterator upper_bound(Derived& d, Key&& key) {
			auto memfn = &Accessor::template doUpperBound<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
		template<typename Key>
		static const_iterator upper_bound(const Derived& d, Key&& key) {
			auto memfn = &Accessor::template doUpperBoundConst<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
		template<typename Key>
		static std::pair<iterator, iterator> equal_range(Derived& d, Key&& key) {
			auto memfn = &Accessor::template doEqualRange<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
		template<typename Key>
		static std::pair<const_iterator, const_iterator> equal_range(const Derived& d, Key&& key) {
			auto memfn = &Accessor::template doEqualRangeConst<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
	};
};

namespace multi
{

template<
	typename Base,
	typename Traits
>
class Wrapper : public tc::container::assoc::base::Wrapper<Base, Traits>
{
protected:
	using Super = tc::container::assoc::base::Wrapper<Base, Traits>;
public:
	using Derived = typename Base::Derived;
	using value_type = typename Super::value_type;
	using iterator = typename Super::iterator;
	using Super::insert;
	using Super::lower_bound;

	iterator insert(const value_type& v) {
		return Accessor::insert(derived(), v);
	}
	iterator insert(value_type&& v) {
		return Accessor::insert(derived(), std::move(v));
	}

protected:
	using Super::derived;
	using Super::key;

	using Super::Super;
	Wrapper(const Wrapper&) = default;
	Wrapper(Wrapper&&) = default;
	Wrapper& operator=(const Wrapper&) = default;
	Wrapper& operator=(Wrapper&&) noexcept = default;

	template<typename V>
	iterator doInsert(V&& v) {
		return insert(lower_bound(key(v)), std::forward<V>(v));
	}

private:
	struct Accessor : Derived
	{
		template<typename V>
		static iterator insert(Derived& d, V&& v) {
			return (d.*&Accessor::template doInsert<V>)(std::forward<V>(v));
		}
	};
};

} //namespace multi.

namespace uniq
{

template<
	typename Base,
	typename Traits
>
class Wrapper : tc::container::assoc::base::Wrapper<Base, Traits>
{
protected:
	using Super = tc::container::assoc::base::Wrapper<Base, Traits>;
public:
	using Derived = typename Base::Derived;
	using value_type = typename Super::value_type;
	using iterator = typename Super::iterator;
	using size_type = typename Super::size_type;
	using Super::size;
	using Super::insert;
	using Super::lower_bound;

	std::pair<iterator, bool> insert(const value_type& v) {
		return Accessor::insert(derived(), v);
	}
	std::pair<iterator, bool> insert(value_type&& v) {
		return Accessor::insert(derived(), std::move(v));
	}

protected:
	using Super::derived;
	using Super::key;

	using Super::Super;
	Wrapper(const Wrapper&) = default;
	Wrapper(Wrapper&&) = default;
	Wrapper& operator=(const Wrapper&) = default;
	Wrapper& operator=(Wrapper&&) noexcept = default;

	template<typename V>
	std::pair<iterator, bool> doInsert(V&& v) {
		size_type prev = size();
		iterator it = insert(lower_bound(key(v)), std::forward<V>(v));
		return {it, size() > prev};
	}

private:
	struct Accessor : Derived
	{
		template<typename V>
		static std::pair<iterator, bool> insert(Derived& d, V&& v) {
			return (d.*&Accessor::template doInsert<V>)(std::forward<V>(v));
		}
	};
};

} //namespace uniq.

namespace ord
{

template<
	typename Base
>
class Wrapper : public Base
{
public:
	using Derived = typename Base::Derived;
	using value_type = typename Wrapper::value_type;
	using key_compare = typename Wrapper::key_compare;
	using value_compare = typename Wrapper::value_compare;

	Wrapper() = default;
	Wrapper(const key_compare& comp) : Base(comp)
	{}
	template<typename InputIterator>
	Wrapper(InputIterator first, InputIterator last, const key_compare& comp) :
		Base(first, last, comp)
	{}
	template<typename InputIterator>
	Wrapper(InputIterator first, InputIterator last) : Base(first, last)
	{}
	Wrapper(std::initializer_list<value_type> il, const key_compare& comp) :
		Base(il, comp)
	{}
	Wrapper(std::initializer_list<value_type> il) :
		Base(il)
	{}

	key_compare key_comp() const {
		return Accessor::key_comp(derived());
	}
	value_compare value_comp() const {
		return Accessor::value_comp(derived());
	}

protected:
	using Base::derived;
	using Base::m_container;

	using Base::Base;
	Wrapper(const Wrapper&) = default;
	Wrapper(Wrapper&&) noexcept = default;
	Wrapper& operator=(const Wrapper&) = default;
	Wrapper& operator=(Wrapper&&) noexcept = default;

	key_compare doKeyComp() const {
		return m_container.key_comp();
	}
	value_compare doValueComp() const {
		return m_container.value_comp();
	}

private:
	struct Accessor : Derived
	{
		static key_compare key_comp(const Derived& d) {
			return (d.*&Accessor::doKeyComp)();
		}
		static value_compare value_comp(const Derived& d) {
			return (d.*&Accessor::doValueComp)();
		}
	};
};

} //namespace ord.

namespace unord
{

template<
	typename Base
>
class Wrapper : public Base
{
public:
	using Derived = typename Base::Derived;
	using value_type = typename Wrapper::value_type;
	using size_type = typename Wrapper::size_type;
	using hasher = typename Wrapper::hasher;
	using key_equal = typename Wrapper::key_equal;
	using local_iterator = typename Wrapper::local_iterator;
	using const_local_iterator = typename Wrapper::const_local_iterator;

	Wrapper() = default;
	template<typename InputIterator>
	Wrapper(size_type n, const hasher& hf, const key_equal& eq) : Base(n, hf, eq)
	{}
	Wrapper(size_type n, const hasher& hf) : Base(n, hf)
	{}
	Wrapper(size_type n) : Base(n)
	{}
	template<typename InputIterator>
	Wrapper(InputIterator first, InputIterator last, size_type n, const hasher& hf, const key_equal& eq) :
		Base(first, last, n, hf, eq)
	{}
	template<typename InputIterator>
	Wrapper(InputIterator first, InputIterator last, size_type n, const hasher& hf) :
		Base(first, last, n, hf)
	{}
	template<typename InputIterator>
	Wrapper(InputIterator first, InputIterator last, size_type n) : Base(first, last, n)
	{}
	template<typename InputIterator>
	Wrapper(InputIterator first, InputIterator last) : Base(first, last)
	{}
	Wrapper(std::initializer_list<value_type> il, size_type n, const hasher& hf, const key_equal& eq) :
		Base(il, n, hf, eq)
	{}
	Wrapper(std::initializer_list<value_type> il, size_type n, const hasher& hf) : Base(il, n, hf)
	{}
	Wrapper(std::initializer_list<value_type> il, size_type n) : Base(il, n)
	{}
	Wrapper(std::initializer_list<value_type> il) :
		Base(il)
	{}

	hasher hash_function() const {
		return Accessor::hash_function(derived());
	}
	key_equal key_eq() const {
		return Accessor::key_eq(derived());
	}
	size_type bucket_count() const {
		return Accessor::bucket_count(derived());
	}
	size_type max_bucket_count() const {
		return Accessor::max_bucket_count(derived());
	}
	template<typename Key>
	size_type bucket(Key&& key) const {
		return Accessor::bucket(derived(), std::forward<Key>(key));
	}
	size_type bucket_size(size_type n) const {
		return Accessor::bucket_size(derived(), n);
	}
	local_iterator begin(size_type n) {
		return Accessor::begin(derived(), n);
	}
	const_local_iterator begin(size_type n) const {
		return Accessor::begin(derived(), n);
	}
	local_iterator end(size_type n) {
		return Accessor::end(derived(), n);
	}
	const_local_iterator end(size_type n) const {
		return Accessor::end(derived(), n);
	}
	const_local_iterator cbegin(size_type n) const {
		return begin(n);
	}
	const_local_iterator cend(size_type n) const {
		return end(n);
	}
	float load_factor() const {
		return Accessor::load_factor(derived());
	}
	float max_load_factor() const {
		return Accessor::max_load_factor(derived());
	}
	void rehash(size_type n) {
		Accessor::rehash(derived(), n);
	}
	void reserve(size_type n) {
		Accessor::reserve(derived(), n);
	}

protected:
	using Base::derived;
	using Base::m_container;

	using Base::Base;
	Wrapper(const Wrapper&) = default;
	Wrapper(Wrapper&&) noexcept = default;
	Wrapper& operator=(const Wrapper&) = default;
	Wrapper& operator=(Wrapper&&) noexcept = default;

	hasher doHashFunction() const {
		return m_container.hash_function();
	}
	key_equal doKeyEq() const {
		return m_container.key_eq();
	}
	size_type doBucketCount() const {
		return m_container.bucket_count();
	}
	size_type doMaxBucketCount() const {
		return m_container.max_bucket_count();
	}
	template<typename Key>
	size_type doBucket(Key&& key) const {
		return m_container.bucket(std::forward<Key>(key));
	}
	size_type doBucketSize(size_type n) const {
		return m_container.bucket_size(derived(), n);
	}
	local_iterator doBegin(size_type n) {
		return m_container.begin(n);
	}
	const_local_iterator doBeginConst(size_type n) const {
		return m_container.begin(n);
	}
	local_iterator doEnd(size_type n) {
		return m_container.end(n);
	}
	const_local_iterator doEndConst(size_type n) const {
		return m_container.end(n);
	}
	float doLoadFactor() const {
		return m_container.load_factor();
	}
	float doMaxLoadFactor() const {
		return m_container.max_load_factor();
	}
	void doRehash(size_type n) {
		m_container.rehash(n);
	}
	void doReserve(size_type n) {
		m_container.reserve(derived(), n);
	}

private:
	struct Accessor : Derived
	{
		hasher hash_function(const Derived& d) const {
			return (d.*&Accessor::doHashFunction)();
		}
		key_equal key_eq(const Derived& d) const {
			return (d.*&Accessor::doKeyEq)();
		}
		size_type bucket_count(const Derived& d) const {
			return (d.*&Accessor::doBucketCount)();
		}
		size_type max_bucket_count(const Derived& d) const {
			return (d.*&Accessor::doMaxBucketCount)();
		}
		template<typename Key>
		size_type bucket(const Derived& d, Key&& key) const {
			return (d.*&Accessor::template doBucket)(std::forward<Key>(key));
		}
		size_type bucket_size(const Derived& d, size_type n) const {
			return (d.*&Accessor::doBucketSize)(n);
		}
		local_iterator begin(Derived& d, size_type n) {
			return (d.*&Accessor::doBegin)(n);
		}
		const_local_iterator begin(const Derived& d, size_type n) const {
			return (d.*&Accessor::doBeginConst)(n);
		}
		local_iterator end(Derived& d, size_type n) {
			return (d.*&Accessor::doEnd)(n);
		}
		const_local_iterator end(const Derived& d, size_type n) const {
			return (d.*&Accessor::doEndConst)(n);
		}
		float load_factor(const Derived& d) const {
			return (d.*&Accessor::doLoadFactor)();
		}
		float max_load_factor(const Derived& d) const {
			return (d.*&Accessor::doMaxLoadFactor)();
		}
		void rehash(Derived& d, size_type n) {
			(d.*&Accessor::doRehash)(n);
		}
		void reserve(Derived& d, size_type n) {
			(d.*&Accessor::doReserve)(n);
		}
	};
};

} //namespace unord.
} //namespace base.

namespace ord
{

template<
	typename Key,
	typename KeyCompare,
	typename ValueCompare = KeyCompare
>
struct Traits : tc::container::assoc::base::Traits<Key>
{
	using key_compare = KeyCompare;
	using value_compare = ValueCompare;
};

template<
	typename Container,
	typename Key = typename Container::key_type,
	typename KeyCompare = typename Container::key_compare,
	typename ValueCompare = typename Container::value_compare
>
struct DefaultTraits :
	Traits<Key, KeyCompare, ValueCompare>
{};

namespace multi
{

/**@tparam Base is either tc::container::revers::Wrapper or tc::container::Wrapper*/
template<
	typename Base,
	typename Traits = DefaultTraits<typename Base::Container>
>
class Wrapper :
	public tc::container::assoc::base::ord::Wrapper<
		tc::container::assoc::base::multi::Wrapper<Base, Traits>
	>
{
protected:
	using Super = tc::container::assoc::base::ord::Wrapper<
		tc::container::assoc::base::multi::Wrapper<Base, Traits>
	>;
public:
	using Super::Super;

protected:
	Wrapper(const Wrapper&) = default;
	Wrapper(Wrapper&&) noexcept = default;
	Wrapper& operator=(const Wrapper&) = default;
	Wrapper& operator=(Wrapper&&) noexcept = default;
};

} //namespace multi.

namespace uniq
{

/**@tparam Base is either tc::container::revers::Wrapper or tc::container::Wrapper*/
template<
	typename Base,
	typename Traits = DefaultTraits<typename Base::Container>
>
class Wrapper :
	public tc::container::assoc::base::ord::Wrapper<
		tc::container::assoc::base::uniq::Wrapper<Base, Traits>
	>
{
protected:
	using Super = tc::container::assoc::base::ord::Wrapper<
		tc::container::assoc::base::uniq::Wrapper<Base, Traits>
	>;
public:
	using Super::Super;

protected:
	Wrapper(const Wrapper&) = default;
	Wrapper(Wrapper&&) noexcept = default;
	Wrapper& operator=(const Wrapper&) = default;
	Wrapper& operator=(Wrapper&&) noexcept = default;
};

} //namespace uniq.
} //namespace ord.

namespace unord
{

template<
	typename Key,
	typename Hash,
	typename KeyEq,
	typename ConstLocalIterator,
	typename LocalIterator = ConstLocalIterator
>
struct Traits : tc::container::assoc::base::Traits<Key>
{
	using hasher = Hash;
	using key_equal = KeyEq;
	using local_iterator = LocalIterator;
	using const_local_iterator = ConstLocalIterator;
};

template<
	typename Container,
	typename Key = typename Container::key_type,
	typename Hash = typename Container::hasher,
	typename KeyEq = typename Container::key_equal,
	typename ConstLocalIterator = typename Container::const_local_iterator,
	typename LocalIterator = ConstLocalIterator
>
struct DefaultTraits :
	Traits<Key, Hash, KeyEq, ConstLocalIterator, LocalIterator>
{};

namespace multi
{

/**@tparam Base is either tc::container::revers::Wrapper or tc::container::Wrapper*/
template<
	typename Base,
	typename Traits = DefaultTraits<typename Base::Container>
>
class Wrapper :
	public tc::container::assoc::base::unord::Wrapper<
		tc::container::assoc::base::multi::Wrapper<Base, Traits>
	>
{
protected:
	using Super = tc::container::assoc::base::unord::Wrapper<
		tc::container::assoc::base::multi::Wrapper<Base, Traits>
	>;
public:
	using Super::Super;

protected:
	Wrapper(const Wrapper&) = default;
	Wrapper(Wrapper&&) noexcept = default;
	Wrapper& operator=(const Wrapper&) = default;
	Wrapper& operator=(Wrapper&&) noexcept = default;
};

} //namespace multi.

namespace uniq
{

/**@tparam Base is either tc::container::revers::Wrapper or tc::container::Wrapper*/
template<
	typename Base,
	typename Traits = DefaultTraits<typename Base::Container>
>
class Wrapper :
	public tc::container::assoc::base::unord::Wrapper<
		tc::container::assoc::base::uniq::Wrapper<Base, Traits>
	>
{
protected:
	using Super = tc::container::assoc::base::unord::Wrapper<
		tc::container::assoc::base::uniq::Wrapper<Base, Traits>
	>;
public:
	using Super::Super;

protected:
	Wrapper(const Wrapper&) = default;
	Wrapper(Wrapper&&) noexcept = default;
	Wrapper& operator=(const Wrapper&) = default;
	Wrapper& operator=(Wrapper&&) noexcept = default;
};

} //namespace uniq.
} //namespace unord.
} //namespace assoc.
} //tc::container

#endif

#ifndef CONTAINER
#define CONTAINER

#include <type_traits>
#include <functional>
#include <cassert>
#include <iterator>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/function_output_iterator.hpp>

#include "Utils.h"

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
struct ContainerTraits
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
struct DefaultContainerTraits :
	ContainerTraits<Size, ConstIterator, Iterator, Value, Reference, ConstReference, Difference>
{};

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
struct ReversibleContainerTraits :
	ContainerTraits<Size, ConstIterator, Iterator, Value, Reference, ConstReference, Difference>
{
	using reverse_iterator = ReverseIterator;
	using const_reverse_iterator = ConstReverseIterator;
};

template<
	typename ReversibleContainer,
	typename Size = typename ReversibleContainer::size_type,
	typename ConstIterator = typename ReversibleContainer::const_iterator,
	typename ConstReverseIterator = typename ReversibleContainer::const_reverse_iterator,
	typename Iterator = ConstIterator,
	typename ReverseIterator = ConstReverseIterator,
	typename Value = typename std::iterator_traits<Iterator>::value_type,
	typename Reference = typename std::iterator_traits<Iterator>::reference,
	typename ConstReference = typename std::iterator_traits<ConstIterator>::reference,
	typename Difference = typename std::iterator_traits<Iterator>::difference_type
>
struct DefaultReversibleContainerTraits :
	ReversibleContainerTraits<Size, ConstIterator, ConstReverseIterator, Iterator, ReverseIterator, Value, Reference, ConstReference, Difference>
{};


template<
	typename TDerived,
	typename TContainer,
	typename TContainerTraits = DefaultContainerTraits<TContainer>
>
class ContainerWrapper : public TContainerTraits
{
public:
	using Derived = TDerived;
	using Container = TContainer;
	using ContainerTraits = TContainerTraits;
	using iterator = typename ContainerWrapper::iterator;
	using const_iterator = typename ContainerWrapper::const_iterator;
	using size_type = typename ContainerWrapper::size_type;

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
	DECLARE_DEFAULT_COPY_MOVE_CTORS_BY_DEFAULT(ContainerWrapper)
	DECLARE_COPY_MOVE_ASSIGN_BY_DEFAULT(ContainerWrapper)
	template<typename... Args>
	ContainerWrapper(Args&&... args) : m_container(std::forward<Args>(args)...) {}

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
		m_container.swap(static_cast<ContainerWrapper&>(other).m_container);
	}
	bool doEqual(const Derived& other) const {
		return m_container == static_cast<const ContainerWrapper&>(other).m_container;
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
	friend bool operator==(const ContainerWrapper&, const ContainerWrapper&);
};

template<
	typename D,
	typename C,
	typename CT
>
bool operator==(
	const ContainerWrapper<D, C, CT>& lhs,
	const ContainerWrapper<D, C, CT>& rhs
)
{
	return ContainerWrapper<D, C, CT>::Accessor::equal(lhs.derived(), rhs.derived());
}

template<
	typename D,
	typename C,
	typename CT
>
bool operator!=(
	const ContainerWrapper<D, C, CT>& lhs,
	const ContainerWrapper<D, C, CT>& rhs
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
	ContainerWrapper<D, C, CT>& lhs,
	ContainerWrapper<D, C, CT>& rhs
)
{
	return lhs.swap(static_cast<D&>(rhs));
}

template<
	typename Derived,
	typename ReversibleContainer,
	typename ReversibleContainerTraits = DefaultReversibleContainerTraits<ReversibleContainer>
>
class ReversibleContainerWrapper :
	public ContainerWrapper<
		Derived,
		ReversibleContainer,
		ReversibleContainerTraits
	>
{
public:
	using reverse_iterator = typename ReversibleContainerTraits::reverse_iterator;
	using const_reverse_iterator = typename ReversibleContainerTraits::const_reverse_iterator;

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
	using ReversibleContainerWrapper::ContainerWrapper::m_container;
	using ReversibleContainerWrapper::ContainerWrapper::derived;

	using ReversibleContainerWrapper::ContainerWrapper::ContainerWrapper;
	DECLARE_DEFAULT_COPY_MOVE_CTORS_BY_DEFAULT(ReversibleContainerWrapper)
	DECLARE_COPY_MOVE_ASSIGN_BY_DEFAULT(ReversibleContainerWrapper)

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

/**@tparam Base ReversibleContainerWrapper or ContainerWrapper*/
template<typename Base>
class SequenceContainerWrapper : public Base
{
public:
	using Derived = typename Base::Derived;
	using value_type = typename SequenceContainerWrapper::value_type;
	using iterator = typename SequenceContainerWrapper::iterator;
	using const_iterator = typename SequenceContainerWrapper::const_iterator;
	using size_type = typename SequenceContainerWrapper::size_type;

	SequenceContainerWrapper() : SequenceContainerWrapper(static_cast<value_type*>(nullptr), static_cast<value_type*>(nullptr)) {}
	SequenceContainerWrapper(size_type n, const value_type& v) :
		SequenceContainerWrapper(
			SameValueIterator<const std::reference_wrapper<const value_type>>(v, 0),
			SameValueIterator<const std::reference_wrapper<const value_type>>(v, n)
		)
	{}
	template<typename InputIterator>
	SequenceContainerWrapper(InputIterator first, InputIterator last) :
		Base(first, last)
	{}
	SequenceContainerWrapper(std::initializer_list<value_type> il) :
		SequenceContainerWrapper(il.begin(), il.end())
	{}

	Derived& operator=(std::initializer_list<value_type> il) {
		assign(il);
		return derived();
	}
	template<typename... Args>
	iterator emplace(const_iterator pos, Args&&... args) {
		return Accessor::emplace(derived(), pos, std::forward<Args>(args)...);
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
	DECLARE_COPY_MOVE_CTORS_BY_DEFAULT(SequenceContainerWrapper)
	DECLARE_COPY_MOVE_ASSIGN_BY_DEFAULT(SequenceContainerWrapper)

	template<typename... Args>
	iterator doEmplace(const_iterator pos, Args&&... args) {
		//same as insert by default.
		return insert(pos, value_type(std::forward<Args>(args)...));
	}
	template<typename InputIterator>
	iterator doInsert(const_iterator pos, InputIterator first, InputIterator last) {
		return m_container.insert(pos, first, last);
	}
	iterator doInsertOne(const_iterator pos, const value_type& v) {
		return insert(pos, &v, std::next(&v));
	}
	iterator doInsertOneRV(const_iterator pos, value_type&& v) {
		return insert(pos, std::move_iterator(&v), std::move_iterator(std::next(&v)));
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
		static_assert(std::is_base_of_v<std::random_access_iterator_tag, std::iterator_traits<decltype(first)>::iterator_category>);
		return std::pair(first, last);
	}

private:
	struct Accessor : Derived
	{
		template<typename... Args>
		static iterator emplace(Derived& d, const_iterator pos, Args&&... args) {
			auto memfn = &Accessor::doEmplace<Args...>;
			return (d.*memfn)(pos, std::forward<Args>(args)...);
		}
		template<typename InputIterator>
		static iterator insert(Derived& d, const_iterator pos, InputIterator first, InputIterator last) {
			auto memfn = &Accessor::doInsert<InputIterator>;
			return (d.*memfn)(pos, first, last);
		}
		static iterator insert(Derived& d, const_iterator pos, const value_type& v) {
			return (d.*&Accessor::doInsertOne)(pos, v);
		}
		static iterator insert(Derived& d, const_iterator pos, value_type&& v) {
			return (d.*&Accessor::doInsertOneRV)(pos, std::move(v));
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
			auto memfn = &Accessor::doAssign<InputIterator>;
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

template<
	typename Key,
	typename KeyCompare,
	typename NodeType,
	typename ValueCompare = KeyCompare
>
struct AssociativeContainerTraits
{
	using key_type = Key;
	using key_compare = KeyCompare;
	using value_compare = ValueCompare;
	using node_type = NodeType;
};

template<
	typename AssociativeContainer,
	typename Key = typename AssociativeContainer::key_type,
	typename KeyCompare = typename AssociativeContainer::key_compare,
	typename ValueCompare = typename AssociativeContainer::value_compare,
	typename NodeType = typename AssociativeContainer::node_type
>
struct DefaultAssociativeContainerTraits :
	AssociativeContainerTraits<Key, KeyCompare, NodeType, ValueCompare>
{};

template<
	typename Base,
	typename AssociativeContainerTraits = DefaultAssociativeContainerTraits<typename Base::Container>
>
class AssociativeContainerWrapper : public Base, public AssociativeContainerTraits
{
protected:
	using Derived = typename Base::Derived;
public:
	using value_type = typename AssociativeContainerWrapper::value_type;
	using size_type = typename AssociativeContainerWrapper::size_type;
	using key_compare = typename AssociativeContainerWrapper::key_compare;
	using value_compare = typename AssociativeContainerWrapper::value_compare;
	using iterator = typename AssociativeContainerWrapper::iterator;
	using const_iterator = typename AssociativeContainerWrapper::const_iterator;
	using key_type = typename AssociativeContainerWrapper::key_type;
	using node_type = typename AssociativeContainerWrapper::node_type;

	AssociativeContainerWrapper() :
		AssociativeContainerWrapper(static_cast<value_type*>(nullptr), static_cast<value_type*>(nullptr))
	{}
	AssociativeContainerWrapper(const key_compare& comp) :
		AssociativeContainerWrapper(static_cast<value_type*>(nullptr), static_cast<value_type*>(nullptr), comp)
	{}
	template<typename InputIterator>
	AssociativeContainerWrapper(InputIterator first, InputIterator last, const key_compare& comp) :
		Base(first, last, comp)
	{}
	template<typename InputIterator>
	AssociativeContainerWrapper(InputIterator first, InputIterator last) :
		AssociativeContainerWrapper(first, last, key_compare())
	{}
	AssociativeContainerWrapper(std::initializer_list<value_type> il, const key_compare& comp) :
		AssociativeContainerWrapper(il.begin(), il.end(), comp)
	{}
	AssociativeContainerWrapper(std::initializer_list<value_type> il) :
		AssociativeContainerWrapper(il.begin(), il.end())
	{}

	Derived& operator=(std::initializer_list<value_type> il) {
		clear();
		insert(il);
		return derived();
	}

	key_compare key_comp() const {
		return Accessor::key_comp(derived());
	}
	value_compare value_comp() const {
		return Accessor::value_comp(derived());
	}
	template<typename... Args>
	iterator emplace_hint(const_iterator pos, Args&&... args) {
		return Accessor::emplace_hint(derived(), pos, std::forward<Args>(args)...);
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
	iterator insert(const_iterator pos, node_type&& node) {
		return Accessor::insert(derived(), pos, std::move(node));
	}
	template<typename Key>
	node_type extract(Key&& key) {
		return Accessor::extract(derived(), std::forward<Key>(key));
	}
	node_type extract(const_iterator pos) {
		return Accessor::extract(derived(), pos);
	}
	void merge(Derived& other) {
		return Accessor::merge(derived(), other);
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
	DECLARE_COPY_MOVE_CTORS_BY_DEFAULT(AssociativeContainerWrapper)
	DECLARE_COPY_MOVE_ASSIGN_BY_DEFAULT(AssociativeContainerWrapper)

	key_compare doKeyComp() const {
		return m_container.key_comp();
	}
	value_compare doValueComp() const {
		return m_container.value_comp();
	}
	template<typename... Args>
	iterator doEmplaceHint(const_iterator pos, Args&&... args) {
		return insert(pos, makeNode(std::forward<Args>(args)...));
	}
	iterator doInsertHint(const_iterator pos, const value_type& v) {
		return insert(pos, makeNode(v));
	}
	iterator doInsertHintRV(const_iterator pos, value_type&& v) {
		return insert(pos, makeNode(std::move(v)));
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
	iterator doInsert(const_iterator pos, node_type&& node) {
		return m_container.insert(pos, std::move(node));
	}
	template<typename F>
	void doExtract(const_iterator first, const_iterator last, F&& forEachNode)
	{
		while(first != last) {
			std::invoke(std::forward<F>(forEachNode), m_container.extract(first++));
		}
	}
	template<typename Key>
	node_type doExtractKey(Key&& key) {
		if(auto it = find(key); it != end()) {
			return extract(it);
		}
		else {
			return node_type();
		}
	}
	node_type doExtractOne(const_iterator pos) {
		assert(pos != end());
		node_type node; auto f = [&node](node_type&& n) { node = std::move(n); };
		Accessor::extract(derived(), pos, std::next(pos), f);
		assert(!node.empty());
		return node;
	}
	void doMerge(Derived& other) {
		for (iterator it = other.begin(); it != other.end();)
		{
			if(find(key(*it)) == end())
			{
				derived().insert(other.extract(it++));
			}
			else {
				++it;
			}
		}
	}
	template<typename Key>
	size_type doEraseKey(Key&& key) {
		size_type oldSize = size();
		auto [first, last] = equal_range(std::forward<Key>(key));
		Accessor::extract(derived(), first, last);
		return oldSize - size();
	}
	iterator doEraseOne(const_iterator pos) {
		assert(pos != end());
		iterator r = std::next(pos);
		Accessor::extract(derived(), pos, std::next(pos));
		return r;
	}
	iterator doErase(const_iterator first, const_iterator last) {
		Accessor::extract(derived(), first, last);
		return last;
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

	template<typename V>
	decltype(auto) key(V&& v) {
		if constexpr (std::is_same_v<key_type, value_type>) {
			return std::forward<V>(v);
		}
		else {
			auto&& [key, value] = std::forward<V>(v);
			return std::forward<decltype(key)>(key);
		}
	}
	template<typename... Args>
	node_type makeNode(Args&&... args) {
		decltype(m_container) c;
		c.emplace(std::forward<Args>(args)...);
		return c.extract(c.begin());
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
		template<typename... Args>
		static iterator emplace_hint(Derived& d, const_iterator pos, Args&&... args) {
			auto memfn = &Accessor::doEmplaceHint<Args...>;
			return (d.*memfn)(pos, std::forward<Args>(args)...);
		}
		static iterator insert(Derived& d, const_iterator pos, const value_type& v) {
			return (d.*&Accessor::doInsertHint)(pos, v);
		}
		static iterator insert(Derived& d, const_iterator pos, value_type&& v) {
			return (d.*&Accessor::doInsertHintRV)(pos, std::move(v));
		}
		template<typename InputIterator>
		static void insert(Derived& d, InputIterator first, InputIterator last) {
			auto memfn = &Accessor::doInsertRange<InputIterator>;
			(d.*memfn)(first, last);
		}
		static void insert(Derived& d, std::initializer_list<value_type> il) {
			(d.*&Accessor::doInsertIL)(il);
		}
		static iterator insert(Derived& d, const_iterator pos, node_type&& node) {
			return (d.*&Accessor::doInsert)(pos, std::move(node));
		}
		template<typename F>
		static void extract(Derived& d, const_iterator first, const_iterator last, F&& forEachNode) {
			auto memfn = &Accessor::doExtract<F>;
			(d.*memfn)(first, last, std::forward<F>(forEachNode));
		}
		static void extract(Derived& d, const_iterator first, const_iterator last) {
			extract(d, first, last, tc::NOP<void, node_type&&>{});
		}
		template<typename Key>
		static node_type extract(Derived& d, Key&& key) {
			auto memfn = &Accessor::doExtractKey<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
		static node_type extract(Derived& d, const_iterator pos) {
			return (d.*&Accessor::doExtractOne)(pos);
		}
		static void merge(Derived& d, const Derived& other) {
			return (d.*&Accessor::doMerge)(other);
		}
		template<typename Key>
		static size_type erase(Derived& d, Key&& key) {
			auto memfn = &Accessor::doEraseKey<Key>;
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
			auto memfn = &Accessor::doFind<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
		template<typename Key>
		static const_iterator find(const Derived& d, Key&& key) {
			auto memfn = &Accessor::doFindConst<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
		template<typename Key>
		static size_type count(const Derived& d, Key&& key) {
			auto memfn = &Accessor::doCount<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
		template<typename Key>
		static iterator lower_bound(Derived& d, Key&& key) {
			auto memfn = &Accessor::doLowerBound<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
		template<typename Key>
		static const_iterator lower_bound(const Derived& d, Key&& key) {
			auto memfn = &Accessor::doLowerBoundConst<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
		template<typename Key>
		static iterator upper_bound(Derived& d, Key&& key) {
			auto memfn = &Accessor::doUpperBound<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
		template<typename Key>
		static const_iterator upper_bound(const Derived& d, Key&& key) {
			auto memfn = &Accessor::doUpperBoundConst<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
		template<typename Key>
		static std::pair<iterator, iterator> equal_range(Derived& d, Key&& key) {
			auto memfn = &Accessor::doEqualRange<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
		template<typename Key>
		static std::pair<const_iterator, const_iterator> equal_range(const Derived& d, Key&& key) {
			auto memfn = &Accessor::doEqualRangeConst<Key>;
			return (d.*memfn)(std::forward<Key>(key));
		}
	};
};

namespace map
{

template<
	typename Key,
	typename KeyCompare,
	typename Mapped,
	typename ValueCompare,
	typename NodeType
>
struct AssociativeContainerTraits :
	tc::container::AssociativeContainerTraits<Key, KeyCompare, NodeType, ValueCompare>
{
	using mapped_type = Mapped;
};

template<
	typename AssociativeContainer,
	typename Key = typename AssociativeContainer::key_type,
	typename KeyCompare = typename AssociativeContainer::key_compare,
	typename Mapped = typename AssociativeContainer::mapped_type,
	typename ValueCompare = typename AssociativeContainer::value_compare,
	typename NodeType = typename AssociativeContainer::node_type
>
struct DefaultAssociativeContainerTraits :
	AssociativeContainerTraits<Key, KeyCompare, Mapped, ValueCompare, NodeType>
{};

}

}

#endif

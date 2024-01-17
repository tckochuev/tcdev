#ifndef CONTAINER
#define CONTAINER

#include <type_traits>
#include <functional>
#include <cassert>
#include <iterator>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/function_input_iterator.hpp>

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
	ReversibleContainerTraits<Size, ConstIterator, Iterator, Value, Reference, ConstReference, Difference>
{};

template<
	typename Derived,
	typename Container,
	typename ContainerTraits = DefaultContainerTraits<Container>
>
class ContainerWrapper
{
public:
	using value_type = typename ContainerTraits::value_type;
	using reference = typename ContainerTraits::reference;
	using const_reference = typename ContainerTraits::const_reference;
	using iterator = typename ContainerTraits::iterator;
	using const_iterator = typename ContainerTraits::const_iterator;
	using difference_type = typename ContainerTraits::difference_type;
	using size_type = typename ContainerTraits::size_type;

	iterator begin() {
		return static_cast<Derived*>(this)->doBegin();
	}
	const_iterator begin() const {
		return static_cast<const Derived*>(this)->doBegin();
	}
	const_iterator cbegin() const {
		return begin();
	}
	iterator end() {
		return static_cast<Derived*>(this)->doEnd();
	}
	const_iterator end() const {
		return static_cast<const Derived*>(this)->doEnd();
	}
	const_iterator cend() const {
		return end();
	}
	size_type max_size() const {
		return static_cast<const Derived*>(this)->doMaxSize();
	}
	size_type size() const {
		return static_cast<const Derived*>(this)->doSize();
	}
	bool empty() const {
		return static_cast<const Derived*>(this)->doEmpty();
	}
	void swap(Derived& other) {
		return static_cast<Derived*>(this)->doSwap(other);
	}
	bool equal(const Derived& other) const {
		return static_cast<const Derived*>(this)->doEqual(other);
	}

protected:
	DECLARE_DEFAULT_COPY_MOVE_CTORS_BY_DEFAULT(ContainerWrapper)
	DECLARE_COPY_MOVE_ASSIGN_BY_DEFAULT(ContainerWrapper)
	template<typename... Args>
	ContainerWrapper(Args&&... args) : m_container(std::forward<Args>(args)...) {}

	iterator doBegin() {
		return m_container.begin();
	}
	const_iterator doBegin() const {
		return m_container.begin();
	}
	iterator doEnd() {
		return m_container.end();
	}
	const_iterator doEnd() const {
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

	Container m_container;
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
	return lhs.equal(static_cast<const D&>(rhs));
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
	public virtual ContainerWrapper<
		Derived,
		ReversibleContainer,
		ReversibleContainerTraits
	>
{
public:
	using reverse_iterator = typename ReversibleContainerTraits::reverse_iterator;
	using const_reverse_iterator = typename ReversibleContainerTraits::const_reverse_iterator;

	reverse_iterator rbegin() {
		return static_cast<Derived*>(this)->doRBegin();
	}
	const_reverse_iterator rbegin() const {
		return static_cast<const Derived*>(this)->doRBegin();
	}
	const_reverse_iterator crbegin() const {
		return rbegin();
	}
	reverse_iterator rend() {
		return static_cast<Derived*>(this)->doREnd();
	}
	const_reverse_iterator rend() const {
		return static_cast<const Derived*>(this)->doREnd();
	}
	const_reverse_iterator crend() const {
		return rend();
	}

protected:
	using ReversibleContainerWrapper::ContainerWrapper::m_container;

	using ReversibleContainerWrapper::ContainerWrapper::ContainerWrapper;
	//DECLARE_DEFAULT_COPY_MOVE_CTORS_BY_DEFAULT(ReversibleContainerWrapper)
	DECLARE_COPY_MOVE_ASSIGN_BY_DEFAULT(ReversibleContainerWrapper)
	//template<typename... Args>
	//ReversibleContainerWrapper(Args&&... args) :
	//	ReversibleContainerWrapper::ContainerWrapper(std::forward<Args>(args)...)
	//{}

	reverse_iterator doRBegin() {
		return m_container.rbegin();
	};
	const_reverse_iterator doRBegin() const {
		return m_container.rbegin();
	}
	reverse_iterator doREnd() {
		return m_container.rend();
	}
	const_reverse_iterator doREnd() const {
		return m_container.rend();
	}
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
	SameValueIterator::reference dereference() const {
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
	void advance(SameValueIterator::difference_type n) {
		state += n;
	}
	SameValueIterator::difference_type distance_to(const SameValueIterator& other) {
		return other.state - state;
	}

	std::remove_const_t<Value> value;
	State state;

	friend class boost::iterator_core_access;
};

template<typename Value, typename Size>
auto rangeOfValue(Size n, Value v)
{
	tc::NOP<Value> f{std::move(v)};
	boost::function_input_iterator first(f, 0), last(std::move(f), n);
	return std::pair(first, last);
}

template<
	typename Derived,
	typename SequenceContainer,
	typename ContainerTraits = DefaultContainerTraits<SequenceContainer>
>
class SequenceContainerWrapper : public virtual ContainerWrapper<Derived, SequenceContainer, ContainerTraits>
{
public:
	using value_type = typename SequenceContainerWrapper::value_type;
	using iterator = typename SequenceContainerWrapper::iterator;
	using const_iterator = typename SequenceContainerWrapper::const_iterator;
	using size_type = typename SequenceContainerWrapper::size_type;

	Derived& operator=(std::initializer_list<value_type> il) {
		assign(il);
		return static_cast<Derived&>(*this);
	}
	template<typename... Args>
	iterator emplace(const_iterator pos, Args&&... args) {
		return static_cast<Derived*>(this)->doEmplace(pos, std::forward<Args>(args)...);
	}
	template<typename InputIterator>
	iterator insert(const_iterator pos, InputIterator first, InputIterator last) {
		return static_cast<Derived*>(this)->doInsert(pos, first, last);
	}
	template<typename InputIterator>
	iterator insert(const_iterator pos, size_type n, InputIterator first, InputIterator last) {
		return static_cast<Derived*>(this)->doInsert(pos, n, first, last);
	}
	iterator insert(const_iterator pos, const value_type& v) {
		return static_cast<Derived*>(this)->doInsert(pos, v);
	}
	iterator insert(const_iterator pos, value_type&& v) {
		return static_cast<Derived*>(this)->doInsert(pos, std::move(v));
	}
	iterator insert(const_iterator pos, size_type n, const value_type& v) {
		return static_cast<Derived*>(this)->doInsert(pos, n, v);
	}
	iterator insert(const_iterator pos, std::initializer_list<value_type> il) {
		return static_cast<Derived*>(this)->doInsert(pos, il);
	}
	iterator erase(const_iterator pos) {
		return static_cast<Derived*>(this)->doErase(pos);
	}
	iterator erase(const_iterator first, const_iterator last) {
		return static_cast<Derived*>(this)->doErase(first, last);
	}
	void clear() {
		static_cast<Derived*>(this)->doClear();
		assert(SequenceContainerWrapper::empty());
	}
	template<typename InputIterator>
	void assign(InputIterator first, InputIterator last) {
		static_cast<Derived*>(this)->doAssign(first, last);
	}
	template<typename InputIterator>
	void assign(size_type n, InputIterator first, InputIterator last) {
		static_cast<Derived*>(this)->doAssign(n, first, last)
	}
	void assign(std::initializer_list<value_type> il) {
		static_cast<Derived*>(this)->doAssign(il);
	}
	void assign(size_type n, const value_type& v) {
		static_cast<Derived*>(this)->doAssign(n, v);
	}

protected:
	using SequenceContainerWrapper::ContainerWrapper::m_container;

	using SequenceContainerWrapper::ContainerWrapper::ContainerWrapper;
	DECLARE_COPY_MOVE_ASSIGN_BY_DEFAULT(SequenceContainerWrapper)

	template<typename... Args>
	iterator doEmplace(const_iterator pos, Args&&... args) {
		//same as insert by default.
		value_type v(std::forward<Args>(args)...);
		return insert(pos, std::move(v));
	}
	template<typename InputIterator>
	iterator doInsert(const_iterator pos, InputIterator first, InputIterator last) {
		return m_container.insert(pos, first, last);
	}
	template<typename InputIterator>
	iterator doInsert(const iterator pos, size_type n, InputIterator first, InputIterator last) {
		//forward to insert(pos, first, last) by default.
		return insert(pos, first, last);
	}
	iterator doInsert(const_iterator pos, const value_type& v) {
		return insert(pos, 1, &v, std::next(&v));
	}
	iterator doInsert(const_iterator pos, value_type&& v) {
		return insert(pos, 1, std::move_iterator(&v), std::move_iterator(std::next(&v)));
	}
	iterator doInsert(const_iterator pos, size_type n, const value_type& v) {
		auto [first, last] = rangeOfValue(n, std::cref(v));
		return insert(pos, n, first, last);
	}
	iterator doInsert(const_iterator pos, std::initializer_list<value_type> il) {
		return insert(pos, il.size(), il.begin(), il.end());
	}
	iterator doErase(const_iterator pos) {
		assert(pos != this->end());
		return erase(pos, std::next(pos));
	}
	iterator doErase(const_iterator first, const_iterator last) {
		return m_container.erase(first, last);
	}
	void doClear() {
		m_container.clear();
	}
	template<typename InputIterator>
	void doAssign(InputIterator first, InputIterator last) {
		m_container.assign(first, last);
	}
	template<typename InputIterator>
	void doAssign(size_type n, InputIterator first, InputIterator last) {
		//forward to assign(first, last) by default.
		assign(first, last);
	}
	void doAssign(std::initializer_list<value_type> il) {
		assign(il.begin(), il.end());
	}
	void doAssign(size_type n, const value_type& v) {
		SameValueIterator<const std::reference_wrapper<const value_type>>
		first(std::cref(v), 0), last(std::cref(v), static_cast<ptrdiff_t>(n));
		static_assert(std::is_base_of_v<std::random_access_iterator_tag, std::iterator_traits<decltype(first)>::iterator_category>);
		assign(first, last);
	}
};

}

#endif

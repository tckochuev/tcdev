#include <iostream>

#include <vector>
#include <set>
#include <optional>

#include "Utils.h"
//#include "SequencedSet.h"

#include <boost/range.hpp>
#include <boost/range/any_range.hpp>
#include <boost/range/adaptors.hpp>

#include <boost/filesystem/path.hpp>

#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/any_cast.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/iterator.hpp>
#include <boost/type_erasure/same_type.hpp>
#include <boost/type_erasure/relaxed.hpp>
#include <boost/type_erasure/typeid_of.hpp>
#include <boost/type_erasure/builtin.hpp>
#include <boost/type_erasure/operators.hpp>

#include <queue>
#include <boost/iterator/indirect_iterator.hpp>
#include <boost/iterator/function_input_iterator.hpp>

#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/unordered_map.hpp>

#include <boost/signals2.hpp>

#include "Container.h"

//template<typename ContainerT>
//class ContainerAdaptor : public tc::ReversibleContainerAdaptor<ContainerT>
//{
//public:
//    using Super = tc::ReversibleContainerAdaptor<ContainerT>;
//    using Container = typename Super::Container;
//
//    ContainerAdaptor(const Container& c) : m_container(c) {}
//
//private:
//    const Container& container() const override {
//        return m_container;
//    }
//
//    Container m_container;
//};

#define MOUSE_BUTTONS \
X(LeftButton,1)   \
X(MiddleButton,1 << 1) \
X(RightButton,1 << 2) \
X(SideButton, 1 << 3)

enum MouseButton
{
#define X(name, value) name = value,
    X(LeftButton, 1)   \
    X(MiddleButton, 1 << 1) \
    X(RightButton, 1 << 2) \
    X(SideButton, 1 << 3)
#undef X
};

bool isValid(MouseButton v)
{
#define X(name, value) if(v == value) return true;
    MOUSE_BUTTONS
#undef X
    return false;
}

const char* toString(MouseButton v)
{
#define X(name, value) if(v == value) return #name;
    MOUSE_BUTTONS
#undef X
    return nullptr;
}

bool fromString(const std::string& str, MouseButton& output)
{
#define X(name, value) if (str == #name){output = static_cast<MouseButton>(value); return true;}
    MOUSE_BUTTONS
#undef X
    return false;
}

#undef MOUSE_BUTTONS

template<typename Container, typename Iterator, typename Predicate>
typename Container::size_type erase_if(
    Container& container,
    Iterator first,
    Iterator last,
    Predicate&& predicate = Predicate()
)
{
    typename Container::size_type oldSize = container.size();
    if constexpr (std::is_move_assignable_v<typename std::iterator_traits<Iterator>::reference>)
    {
        Iterator removedFirst = std::remove_if(first, last, std::forward<Predicate>(predicate));
        container.erase(removedFirst, last);
    }
    else
    {
        //Use non const iterator if this assertion is not satisfied.(This ensures that use of this function is effective as possible.)
        static_assert(!std::is_move_assignable_v<typename std::iterator_traits<typename Container::iterator>::reference>);
        while (first != last)
        {
            if (std::invoke(predicate, *first))
            {
                first = container.erase(first);
            }
            else
            {
                ++first;
            }
        }
    }
    return oldSize - container.size();
}

template<typename Container, typename Predicate>
typename Container::size_type erase_if(
    Container& container,
    Predicate&& predicate = Predicate()
)
{
    return erase_if(container, std::begin(container), std::end(container), std::forward<Predicate>(predicate));
}

class ObservableVector :
    public tc::container::SequenceContainerWrapper<
        tc::container::ReversibleContainerWrapper<ObservableVector, std::vector<int>>
    >
{
public:
    using ObservableVector::SequenceContainerWrapper::SequenceContainerWrapper;
    ObservableVector(const ObservableVector&) = delete;
    ObservableVector(ObservableVector&&) = delete;
    ObservableVector& operator=(const ObservableVector&) = delete;
    ObservableVector& operator=(ObservableVector&&) = delete;

    boost::signals2::signal<void(size_type, const_iterator)> inserted;
    boost::signals2::signal<void(const_iterator, const_iterator)> aboutToBeErased;
    boost::signals2::signal<void(size_type, const_iterator)> erased;
    boost::signals2::signal<void()> reassigned;

protected:
    void doSwap(ObservableVector& other) {
        assert("not swappable");
    }
    template<typename InputIterator>
    iterator doInsert(const_iterator pos, InputIterator first, InputIterator last) {
        size_type oldSize = size();
        auto next = SequenceContainerWrapper::doInsert(pos, first, last);
        inserted(size() - oldSize, next);
        return next;
    }
    iterator doErase(const_iterator first, const_iterator last) {
        aboutToBeErased(first, last);
        size_type oldSize = size();
        auto r = SequenceContainerWrapper::doErase(first, last);
        erased(oldSize - size(), r);
        return r;
    }
    void doClear() {
        if(size_type oldSize = size())
        {
            aboutToBeErased(begin(), end());
            SequenceContainerWrapper::doClear();
            erased(oldSize - size(), end());
        }
    }
    template<typename InputIterator>
    void doAssign(InputIterator first, InputIterator last) {
        SequenceContainerWrapper::doAssign(first, last);
        reassigned();
    }
};

int main()
{
    auto print = [](auto&& range) {
        for(const auto& e : range) {
            std::cout << e << " ";
        }
    };

    ObservableVector v;
    v.aboutToBeErased.connect([&](ObservableVector::const_iterator first, ObservableVector::const_iterator last) {
        auto firstIdx = std::distance(v.begin(), first), lastIdx = std::distance(v.begin(), last);
        std::cout << "about to be erased [" << firstIdx << "," << lastIdx << "]" << std::endl;
    });
    v.erased.connect([&](ObservableVector::size_type n, ObservableVector::const_iterator pos) {
        auto idx = std::distance(v.begin(), pos);
        std::cout << "erased " << n << " elements before index " << idx << " --> ";
        print(v);
        std::endl(std::cout);
    });
    v.inserted.connect([&](auto n, auto pos) {
        auto idx = std::distance(v.begin(), pos);
        std::cout << "inserted " << n << " elements before index " << idx << " --> ";
        print(v);
        std::endl(std::cout);
    });
    v.reassigned.connect([&]() {
        std::cout << "reassigned " << v.size() << " elements --> ";
        print(v);
        std::endl(std::cout);
    });

    v.assign({1, 2, 3, 4, 5, 6, 7, 8});
    auto it = v.begin();
    while(it != v.end())
    {
        if(*it % 2 == 0)
        {
            it = v.erase(it);
        }
        else ++it;
    }
    v.insert(v.end(), {1, 2, 3});
    v.clear();


    return 0;
}
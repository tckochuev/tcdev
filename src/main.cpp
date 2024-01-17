#include <iostream>

#include <vector>
#include <set>
#include <optional>

#include "Utils.h"
#include "Result.h"
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

class Vector : public tc::container::SequenceContainerWrapper<Vector, std::vector<int>>
{
public:
    using Vector::SequenceContainerWrapper::SequenceContainerWrapper;

    /*signal*/ void inserted(size_type n, const_iterator pos) {
        std::cout << n << " elements were inserted" << std::endl;
    }
    /*signal*/ void erased(size_type n, const_iterator pos) {
        std::cout << n << " elements were erased" << std::endl;
    }
    /*signal*/ void assigned(size_type n) {
        std::cout << n << " elements were assigned" << std::endl;
    }
};

int main()
{
    Vector v({1, 2, 3, 4, 5});
    Vector v2({6, 7, 8});
    std::cout << (v == v2) << std::endl;

    bool _ = true;
    if(auto _ = nullptr; _)
    {
        std::cout << "false alarm" << std::endl;
    }
    else
    {
        std::cout << "as expected" << std::endl;
    }

    return 0;
}
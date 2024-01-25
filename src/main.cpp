#include <iostream>

#include <boost/signals2.hpp>

#include <vector>
#include <list>
#include <set>
#include "VSContainer.h"

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

class ObservableVector :
    public tc::container::seq::Wrapper<
        tc::container::revers::Wrapper<ObservableVector, std::vector<int>>
    >
{
public:
    using Wrapper::Wrapper;
    ObservableVector(const ObservableVector&) = delete;
    ObservableVector(ObservableVector&&) = delete;
    ObservableVector& operator=(const ObservableVector&) = delete;
    ObservableVector& operator=(ObservableVector&&) = delete;

    boost::signals2::signal<void(size_type, const_iterator)> inserted;
    boost::signals2::signal<void(const_iterator, const_iterator)> aboutToBeErased;
    boost::signals2::signal<void(size_type, const_iterator)> erased;

protected:
    void doSwap(ObservableVector& other) {
        assert("not swappable");
    }
    template<typename InputIterator>
    iterator doInsert(const_iterator pos, InputIterator first, InputIterator last) {
        size_type oldSize = size();
        auto next = Wrapper::doInsert(pos, first, last);
        inserted(size() - oldSize, next);
        return next;
    }
    iterator doErase(const_iterator first, const_iterator last) {
        aboutToBeErased(first, last);
        size_type oldSize = size();
        auto r = Wrapper::doErase(first, last);
        erased(oldSize - size(), r);
        return r;
    }
};

class ObservableSet :
    public tc::container::assoc::ord::uniq::Wrapper<
        tc::container::revers::Wrapper<ObservableSet, std::set<int>>
    >
{
public:
    using Wrapper::Wrapper;
    ObservableSet(const ObservableSet&) = delete;
    ObservableSet(ObservableSet&&) = delete;
    ObservableSet& operator=(const ObservableSet&) = delete;
    ObservableSet& operator=(ObservableSet&&) = delete;

    boost::signals2::signal<void(const_iterator)> inserted;
    boost::signals2::signal<void(const_iterator, const_iterator)> aboutToBeErased;
    boost::signals2::signal<void(size_type, const_iterator)> erased;

protected:
    iterator doErase(const_iterator first, const_iterator last)
    {
        if(first != last)
        {
            aboutToBeErased(first, last);
            size_type oldSize = size();
            auto it = Wrapper::doErase(first, last);
            erased(oldSize - size(), last);
            return it;
        }
        return last;
    }
    template<typename V>
    iterator doInsert(const_iterator pos, V&& v) {
        auto r = Wrapper::doInsert(pos, std::forward<V>(v));
        inserted(r);
        return r;
    }
};

class EvenCountingList :
    public tc::container::seq::Wrapper<
        tc::container::revers::Wrapper<EvenCountingList, std::list<int>>
    >
{
public:
    template<typename InputIterator>
    EvenCountingList(InputIterator first, InputIterator last) : Wrapper(first, last)
    {}
    using Wrapper::Wrapper;
    EvenCountingList(const EvenCountingList&) = default;
    EvenCountingList(EvenCountingList&& other) noexcept : Wrapper(std::move(other)), m_evenCount(other.m_evenCount) {
        other.m_evenCount = std::count_if(other.begin(), other.end(), &isEven);
    }
    EvenCountingList& operator=(const EvenCountingList&) = default;
    EvenCountingList& operator=(EvenCountingList&& other) {
        EvenCountingList tmp(std::move(other));
        this->swap(tmp);
        return *this;
    }

    int evenCount() const {
        return m_evenCount;
    }

protected:
    static bool isEven(int v) {
        return v % 2 == 0;
    }

    void doSwap(EvenCountingList& other) {
        Wrapper::doSwap(other);
        using std::swap;
        swap(m_evenCount, other.m_evenCount);
    }
    template<typename InputIterator>
    iterator doInsert(const_iterator pos, InputIterator first, InputIterator last) {
        m_evenCount += std::count_if(first, last, &isEven);
        return Wrapper::doInsert(pos, first, last);
    }
    iterator doErase(const_iterator first, const_iterator last) {
        m_evenCount -= std::count_if(first, last, &isEven);
        assert(m_evenCount >= 0);
        return Wrapper::doErase(first, last);
    }

    int m_evenCount = std::count_if(begin(), end(), &isEven);
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
        if(first != last) {
            auto firstIdx = std::distance(v.begin(), first), lastIdx = std::distance(v.begin(), last);
            std::cout << "about to be erased [" << firstIdx << "," << lastIdx << "]" << std::endl;
        }
    });
    v.erased.connect([&](ObservableVector::size_type n, ObservableVector::const_iterator pos) {
        if(n > 0)
        {
            auto idx = std::distance(v.begin(), pos);
            std::cout << "erased " << n << " elements before index " << idx << " --> ";
            print(v);
            std::endl(std::cout);
        }
    });
    v.inserted.connect([&](auto n, auto pos) {
        if(n > 0)
        {
            auto idx = std::distance(v.begin(), pos);
            std::cout << "inserted " << n << " elements before index " << idx << " --> ";
            print(v);
            std::endl(std::cout);
        }
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
    v.assign(static_cast<decltype(v)::size_type>(10), 5);
    v.clear();

    std::cout << "------" << std::endl;

    ObservableSet s({1, 4, 8});
    s.aboutToBeErased.connect([&](auto first, auto last) {
        if(first != last) {
            auto firstIdx = std::distance(s.begin(), first), lastIdx = std::distance(s.begin(), last);
            std::cout << "about to be erased [" << firstIdx << "," << lastIdx << "]" << std::endl;
        }
    });
    s.erased.connect([&](auto n, auto pos) {
        if(n > 0)
        {
            auto idx = std::distance(s.begin(), pos);
            std::cout << "erased " << n << " elements before index " << idx << " --> ";
            print(s);
            std::endl(std::cout);
        }
    });
    s.inserted.connect([&](auto pos) {
        auto idx = std::distance(s.begin(), pos);
        std::cout << "inserted " << idx << " --> ";
        print(s);
        std::endl(std::cout);
    });

    s.erase(s.begin());

    EvenCountingList l({1, 2, 3, 4, 5});
    std::cout << l.evenCount() << std::endl;
    EvenCountingList l2(l);
    std::cout << l2.evenCount() << std::endl;

    return 0;
}
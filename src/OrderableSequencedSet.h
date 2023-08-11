#ifndef SORTABLE_SET_H
#define SORTABLE_SET_H

#include <utility>
#include <functional>
#include <algorithm>
#include <cassert>

//TODO:Remove
#include <set>

class SortableSet
{
private:
    using Container = std::set<char>;

public:
    using const_iterator = Container::const_iterator;
    using iterator = Container::iterator;
    using value_type = Container::value_type;
    using key_type = Container::key_type;
    /**
     * @brief Invokable type that supposed to return true iff lhs should appear before rhs in sequence.
     */
    using Comparer = std::function<bool(const value_type& lhs, const value_type& rhs)>;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

    /**
     * @returns true iff lhs and rhs are considered equal in set.
     */
    bool keysEqual(const key_type& lhs, const key_type& rhs) const;

    /**
     * @returns true iff after hypothetical insertion of @p value before @p position this set is ordered correctly.
     */
    bool isPositionValid(const_iterator position, const value_type& value) const;

    /**
     * @pre !isPositionValid(@p position, @p value)
     * @post returns closest valid position for @p position and @p value.
     * @pre isPositionValid(@p position, @p value)
     * @post returns end of equal range of @p value.
     */
    iterator closestValidPosition(const_iterator position, const value_type& value) {
        return static_cast<const SortableSet*>(this)->closestValidPosition(position, value);
    }
    const_iterator closestValidPosition(const_iterator position, const value_type& value) const
    {
        auto [first, last] = m_container.equal_range(value);
        auto indexOf = [this](const_iterator position) {return std::distance(begin(), position);};
        return indexOf(position) < indexOf(first) ? first : last;
    }

    /**
     * @pre this.find(@p value) == this.end()
     * @returns position of @p value after hypothetical insertion.
     */
    /*virtual*/ iterator insertPosition(const value_type& value);
    /*virtual*/ const_iterator insertPosition(const value_type& value) const;

    /**
     * @pre (it = this.find(@p value)) != this.end()
     * @post returns {it, false}
     * @pre this.find(@p value) == this.end() && isValidPosition(@p position, @p value)
     * @post inserts @p value before @p position and returns {it, true}, where it refers to inserted element.
     */
    std::pair<iterator, bool> insert(const_iterator position, const value_type& value);
    std::pair<iterator, bool> insert(const_iterator position, value_type&& value);
    /**
     * @pre (it = this.find(@p value)) != this.end()
     * @post returns {it, false}
     * @pre this.find(@p value) == this.end()
     * @post inserts @p value and returns {it, true}, where it refers to inserted element.
     */
    std::pair<iterator, bool> insert(const value_type& value);
    std::pair<iterator, bool> insert(value_type&& value);

    /**
     * @pre [first, last) forms non-empty range,
     * [first, last) is subrange of equal range(<=> all elements in [first, last) are compared equal via this.comparer()),
     * position is in this equal range, position is not in [first, last].
     */
    void move(const_iterator first, const_iterator last, const_iterator position);

    /**
     * @brief same as move(@p target, std::next(@p target), position)
     */
    void move(const_iterator target, const_iterator position)
    {
        assert(target != end());
        move(target, std::next(target), position);
    }

    /**
     * @pre [@p first, @p last) forms non-empty range.
     * @post erases elements which are referred by iterators in range and
     * returns iterator to position where range ended.
     */
    iterator erase(const_iterator first, const_iterator last);
    /**
     * @brief same as erase(@p position, std::next(@p position))
     */
    iterator erase(const_iterator position)
    {
        assert(position != end());
        return erase(position, std::next(position));
    }

    void clear();

    const Comparer& comparer() const {
        return m_comparer;
    }

protected:
    using Container = std::vector<int>;

    struct AbstractInserter
    {
        AbstractInserter(Container& container) : container(container)
        {}

        virtual const value_type& getValue() const = 0;
        virtual void operator()() const = 0;

        Container& container;
    };

    struct RValueInserter : AbstractInserter
    {
        RValueInserter(Container& container, value_type&& value) :
            AbstractInserter(container), value(std::move(value))
        {}

        const value_type& getValue() const override {
            return value;
        }

        void operator()() const override {
            container.insert(std::move(value));
        }

        value_type&& value;
    };

    class LValueInserter : public AbstractInserter
    {
        LValueInserter(Container& container, value_type& value) :
            AbstractInserter(container), value(value)
        {}

        const value_type& getValue() const override {
            return value;
        }

        void operator()() const override {
            container.insert(value);
        }

        const value_type& value;
    };

    virtual std::pair<iterator, bool> doSetInsert(const AbstractInserter& inserter);
    virtual std::pair<iterator, bool> doSetInsert(const_iterator position, const AbstractInserter& inserter);

    Comparer m_comparer;
    Container m_container;
};


#endif

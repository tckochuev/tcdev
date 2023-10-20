#include "SequencedSet.h"
#include <algorithm>

namespace tc::container
{

template<typename ValueProperty, bool randomAccess>
auto ISequencedSet<ValueProperty, randomAccess>::tryInsert(iterator pos, CopyOrMove value) -> std::pair<iterator, bool>
{
    auto [first, last] = set().equal_range(get(value));
    if (first == last) {
        return {doInsert(first, pos, value), true};
    }
    else {
        assert(std::next(first) == last);
        return {first, false};
    }
}

template<typename ValueProperty, bool randomAccess>
auto ISequencedSet<ValueProperty, randomAccess>::doInsert(SetIterator setPos, iterator pos, CopyOrMove value) -> iterator
{
#ifndef NDEBUG
    auto sizeBeforeInsert = set().size();
#endif
    auto insertToSet = [this, setPos](auto&& v) {
        return set().insert(setPos, std::forward<decltype(v)>(v));
    };
#ifndef NDEBUG
    SetIterator insertedInSet = invoke(insertToSet, value);
    assert(sizeBeforeInsert + 1 == set().size());
    assert(container().project<detail::multiindex::tag::Sequence>(insertedInSet) == std::prev(end()));
#else
    invoke(insertToSet, value);
#endif
    iterator insertedInSequence = std::prev(end());
    container().relocate(pos, insertedInSequence);
    return insertedInSequence;
}


} //tc::container
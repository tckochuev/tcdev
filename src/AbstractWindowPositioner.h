#ifndef ABSTRACT_WINDOW_POSITIONER
#define ABSTRACT_WINDOW_POSITIONER

#include <string>
#include <algorithm>
#include <memory>
#include <iterator>
#include <unordered_map>

#include <boost/range/any_range.hpp>
#include <boost/iterator/transform_iterator.hpp>

namespace tc
{

template<typename IdT = std::string>
class AbstractWindowPositionerSettings
{
public:
    using Id = IdT;
    using key_type = Id;
    using value_type = Id;
    using reference = value_type;
    using const_reference = value_type;
    using difference_type = std::ptrdiff_t;
    using size_type = size_t;
    using const_iterator = typename boost::range_const_iterator<
        boost::any_range<value_type, boost::single_pass_traversal_tag, const_reference, difference_type>
    >::type;
    using iterator = const_iterator;
    using pointer = typename std::iterator_traits<iterator>::pointer;
    using const_pointer = typename std::iterator_traits<const_iterator>::pointer;

    int areaWidth;
    int areaHeight;

    size_type size() const {
        return doSize();
    }
    iterator begin() {
        return doBegin();
    }
    const_iterator begin() const {
        return doBegin();
    }
    iterator end() {
        return doEnd();
    }
    const_iterator end() const {
        return doEnd();
    }
    const_iterator cbegin() const {
        return begin();
    }
    const_iterator cend() const {
        return end();
    }
    iterator find(const key_type& key) {
        return doFind(key);
    }
    const_iterator find(const key_type& key) const {
        return doFind(key);
    }

protected:
    AbstractLayoutSettings() = default;
    AbstractLayoutSettings(const AbstractLayoutSettings&) = default;
    AbstractLayoutSettings(AbstractLayoutSettings&&) = default;
    AbstractLayoutSettings& operator=(const AbstractLayoutSettings&) = default;
    AbstractLayoutSettings& operator=(AbstractLayoutSettings&&) = default;

private:
    virtual size_type doSize() const = 0;
    virtual iterator doBegin() = 0;
    virtual const_iterator doBegin() const = 0;
    virtual iterator doEnd() = 0;
    virtual const_iterator doEnd() const = 0;
    virtual iterator doFind(const key_type& key) {
        return std::find(begin(), end(), key);
    }
    virtual const_iterator doFind(const key_type& key) const {
        return std::find(begin(), end(), key);
    }
};

template<typename WindowT, typename IdT = std::string>
class AbstractStdUnorderedMapPositionerSettings
{

};

template<typename IdT = std::string>
class IWindowPositioner
{
public:
    using Id = IdT;
    using Settings = AbstractWindowPositionerSettings<Id>;
    struct Window
    {
        int x, y;
        int width, height;
    };
    using key_type = Id;
    using mapped_type = Window;
    struct KeyValuePair
    {
        key_type key;
        mapped_type value;
    };
    using value_type = KeyValuePair;
    using reference = value_type;
    using const_reference = value_type;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    using const_iterator = typename boost::range_const_iterator<
        boost::any_range<value_type, boost::single_pass_traversal_tag, const_reference, difference_type>
    >::type;
    using iterator = const_iterator;
    using pointer = std::iterator_traits<iterator>::pointer;
    using const_pointer = std::iterator_traits<iterator>::const_pointer;

    bool areSettingsSet() const {
        return doAreSettingsSet();
    }

    const Settings& settings() const
    {
        assert(areSettingsSet());
        return doSettings();
    }

    std::unique_ptr<Settings> takeSettings()
    {
        auto taken = doTakeSettings();
        assert(!areSettingsSet());
        return taken;
    }

    //@pre areSettingsSet == true
    size_type size() const {
        return doSize();
    }

    iterator begin() {
        return doBegin();
    }

    const_iterator begin() const {
        return doBegin();
    }

    iterator end() {
        return doEnd();
    }

    const_iterator end() const {
        return doEnd();
    }

    const_iterator cbegin() const {
        return begin();
    }

    const_iterator cend() const {
        return end();
    }

    iterator find(const key_type& key) {
        return doFind(key);
    }

    const_iterator find(const key_type& key) const {
        return doFind(key);
    }

protected:
    IWindowPositioner() = default;
    IWindowPositioner(const IWindowPositioner&) = default;
    IWindowPositioner(IWindowPositioner&&) = default;
    IWindowPositioner& operator=(const IWindowPositioner&) = default;
    IWindowPositioner& operator=(IWindowPositioner&&) = default;

private:
    virtual bool doAreSettingsSet() const = 0;
    virtual const Settings& doSettings() const = 0;
    virtual std::unique_ptr<Settings> doTakeSettings() = 0;
    virtual size_type doSize() const = 0;
    virtual iterator doBegin() = 0;
    virtual const_iterator doBegin() const = 0;
    virtual iterator doEnd() = 0;
    virtual const_iterator doEnd() const = 0;
    virtual iterator doFind(const key_type& key) = 0;
    virtual const_iterator doFind(const key_type& key) const = 0;
};

template<typename IdT = std::string>
class AbstactStdUnorderedMapWindowPositioner : public IWindowPositioner<IdT>
{
public:
    using Super = IWindowPositioner<IdT>;
protected:
    using Container = std::unordered_map<typename Super::key_type, typename Super::mappedType>;
public:
    using key_type = typename Container::key_type;
    using mapped_type = typename Container::mapped_type;
    using value_type = typename Container::value_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
    using difference_type = typename Container::difference_type;
    using size_type = typename Container::size_type;
    using const_iterator = typename Container::const_iterator;
    using iterator = const_iterator;
    using pointer = typename Container::pointer;
    using const_pointer = typename Container::const_pointer;

    size_type size() const {
        return m_windows.size();
    }

    iterator begin() {
        return m_windows.begin();
    }

    const_iterator begin() {
        return m_windows.begin();
    }

    iterator end() {
        return m_windows.end();
    }

    const_iterator end() const {
        return m_windows.end();
    }

    iterator find(const key_type& key) {
        return m_windows.find(key);
    }

    const_iterator find(const key_type& key) const {
        return m_windows.find(key);
    }

protected:
    AbstactStdUnorderedMapWindowPositioner() = default;
    AbstactStdUnorderedMapWindowPositioner(const AbstactStdUnorderedMapWindowPositioner&) = default;
    AbstactStdUnorderedMapWindowPositioner(AbstactStdUnorderedMapWindowPositioner&&) = default;
    AbstactStdUnorderedMapWindowPositioner& operator=(const AbstactStdUnorderedMapWindowPositioner&) = default;
    AbstactStdUnorderedMapWindowPositioner& operator=(AbstactStdUnorderedMapWindowPositioner&&) = default;

    static typename Super::KeyValuePair transformer(const_reference value) {
        return typename Super::KeyValuePair{value.first, value.second};
    }

    typename Super::size_type doSize() const override {
        return static_cast<typename Super::size_type>(size());
    }

    typename Super::iterator doBegin() override {
        return typename Super::iterator(boost::transform_iterator(begin(), transformer));
    }

    typename Super::const_iterator doBegin() const override {
        return typename Super::const_iterator(boost::transform_iterator(begin(), transformer));
    }

    typename Super::iterator doEnd() override {
        return typename Super::iterator(boost::transform_iterator(end(), transformer));
    }

    typename Super::const_iterator doEnd() const override {
        return typename Super::const_iterator(boost::transform_iterator(end(), transformer));
    }

    typename Super::iterator doFind(const key_type& key) override {
        return typename Super::iterator(boost::transform_iterator(m_windows.find(key), transformer));
    }

    typename Super::const_iterator doFind(const key_type& key) const override {
        return typename Super::const_iterator(boost::transform_iterator(m_windows.find(key), transformer));
    }

    Container m_windows;
};

}

#endif

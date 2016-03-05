#ifndef TEXTSELECTOR_H
#define TEXTSELECTOR_H
#include <vector>
#include <string>
#include <utility>
#include <boost/algorithm/string.hpp>

struct SGetterString {
    const std::string & operator () (const std::string &s) {
        return s;
    }
};

enum class TextSelectorError {
    MultipleMatches,
    NoMatches
};


template <typename T, typename SGetter=SGetterString>
class TextSelector
{
public:
    typedef T value_type;
    typedef T& reference;
    typedef const T & const_reference;
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;
    typedef typename std::vector<T>::difference_type difference_type;
    typedef typename std::vector<T>::size_type size_type;

    TextSelector();
    TextSelector(const std::initializer_list<T> &list);
    TextSelector(const TextSelector<T, SGetter> &t) = default;
    TextSelector(TextSelector<T, SGetter> &&t) { mVector = std::move(t.mVector); }
    ~TextSelector() = default;

    TextSelector<T, SGetter> &operator=(const TextSelector<T, SGetter> &t);
    TextSelector<T, SGetter> &operator=(TextSelector<T, SGetter> &&t);

    iterator begin() { return mVector.begin(); }
    iterator end() { return mVector.end(); }
    const_iterator begin() const { return mVector.begin(); }
    const_iterator end() const { return mVector.end(); }
    const_iterator cbegin() const { return mVector.begin(); }
    const_iterator cend() const { return mVector.end(); }


    void insert(const T &t);
    void insert(T &&t);
    void insert(const std::initializer_list<T> &list);

    std::pair<const_iterator, const_iterator> selection_range(const std::string &text) const;

    const T &find_match(const std::string &text) const;
private:
    const_iterator closest(const std::string &b) const;
    iterator closest(const std::string &b);

    std::vector<T> mVector;
};

template <typename T>
struct TextSelectorMapSGetter {
    const std::string &operator()(const std::pair<std::string, T> &p) {
        return p.first;
    }
};

template <typename T>
class TextSelectorMap : public TextSelector<std::pair<std::string, T>, TextSelectorMapSGetter<T> > {
    typedef TextSelector<std::pair<std::string, T>, TextSelectorMapSGetter<T> > base_type;
public:
    TextSelectorMap() = default;
    TextSelectorMap(const std::initializer_list<std::pair<std::string, T>> &list) : base_type(list) {}
    TextSelectorMap(const TextSelectorMap &) = default;

    TextSelectorMap &operator=(const TextSelectorMap &) = default;

    void insert(const std::string &key, const T &t) {
        typename TextSelector<std::pair<std::string, T>, TextSelectorMapSGetter<T> >::value_type p(key, t);
        this->TextSelector<std::pair<std::string, T>, TextSelectorMapSGetter<T> >::insert(std::move(p));
    }
    void insert(const std::string &key, T &&t) {
        typename TextSelector<std::pair<std::string, T>, TextSelectorMapSGetter<T> >::value_type p(key, std::move(t));
        this->TextSelector<std::pair<std::string, T>, TextSelectorMapSGetter<T> >::insert(std::move(p));
    }

    const T &match(const std::string &key) const { return this->find_match(key).second; }

};





template <typename T, typename SGetter>
TextSelector<T, SGetter>::TextSelector() {

}

template <typename T, typename SGetter>
TextSelector<T, SGetter>::TextSelector(const std::initializer_list<T> &list)
{
    insert(list);
}

template <typename T, typename SGetter>
TextSelector<T, SGetter> &TextSelector<T, SGetter>::operator=(const TextSelector<T, SGetter> &t)
{
    mVector = t.mVector;
    return *this;
}

template <typename T, typename SGetter>
TextSelector<T, SGetter> &TextSelector<T, SGetter>::operator=(TextSelector<T, SGetter> &&t)
{
    mVector = std::move(t.mVector);
    return *this;
}

template <typename T, typename SGetter>
void TextSelector<T, SGetter>::insert(const T &t)
{
    SGetter getter;
    iterator i = closest(getter(t));
    mVector.insert(i, t);
}

template <typename T, typename SGetter>
void TextSelector<T, SGetter>::insert(T &&t)
{
    SGetter getter;
    iterator i = closest(getter(t));
    mVector.insert(i, std::move(t));
}

template <typename T, typename SGetter>
void TextSelector<T, SGetter>::insert(const std::initializer_list<T> &list)
{
    for (auto &v : list) {
        insert(v);
    }
}

template <typename T, typename SGetter>
std::pair<typename TextSelector<T, SGetter>::const_iterator, typename TextSelector<T, SGetter>::const_iterator> TextSelector<T, SGetter>::selection_range(const std::string &text) const
{
    typedef std::pair<TextSelector<T, SGetter>::const_iterator, TextSelector<T, SGetter>::const_iterator> ret_pair;
    auto c = closest(text);
    if (c == end()) return ret_pair(end(), end());
    SGetter getter;

    auto rangeBegin = c;
    auto rangeEnd = c;

    while (rangeBegin != begin()) {
        --rangeBegin;
        if (!boost::starts_with(getter(*rangeBegin), text)) {
            ++rangeBegin;
            break;
        }
    }
    if (boost::starts_with(getter(*rangeEnd), text)) {
        ++rangeEnd;
        while (rangeEnd != end()) {
            if (!boost::starts_with(getter(*rangeEnd), text)) {
                break;
            }
            ++rangeEnd;
        }
    }

    //Find exact match
    for (auto it = rangeBegin; it != rangeEnd; ++it) {
        if (getter(*it) == text) return ret_pair(it, it + 1);
    }

    return ret_pair(rangeBegin, rangeEnd);
}

template <typename T, typename SGetter>
const T &TextSelector<T, SGetter>::find_match(const std::string &text) const
{
    auto p = selection_range(text);
    if (p.first == p.second) throw TextSelectorError::NoMatches;
    if (p.first + 1 != p.second) throw TextSelectorError::MultipleMatches;
    return *p.first;
}

template <typename T, typename SGetter>
typename TextSelector<T, SGetter>::iterator TextSelector<T, SGetter>::closest(const std::string &base)
{
    if (mVector.empty()) return mVector.end();

    SGetter getter;
    {
        iterator i = mVector.begin();
        auto ib = getter(*i);
        if (base < ib) {
            return i;
        }
    }

    if (mVector.size() == 1) {
        return mVector.end();
    }

    iterator rangeStart = mVector.begin();
    iterator rangeEnd = mVector.end();

    int i = 0;
    int n = (rangeEnd - rangeStart);
    for (int b = n / 2; b >= 1; b /= 2) {
        while (i + b < n && getter(rangeStart[i + b]) < base) {
            i += b;
        }
    }

    return rangeStart + i + 1;
}

template <typename T, typename SGetter>
typename TextSelector<T, SGetter>::const_iterator TextSelector<T, SGetter>::closest(const std::string &base) const
{
    if (mVector.empty()) return mVector.end();

    SGetter getter;
    {
        const_iterator i = mVector.begin();
        if (base < getter(*i)) {
            return i;
        }
    }

    if (mVector.size() == 1) {
        return mVector.end();
    }

    const_iterator rangeStart = mVector.begin();
    const_iterator rangeEnd = mVector.end();

    int i = 0;
    int n = (rangeEnd - rangeStart);
    for (int b = n / 2; b >= 1; b /= 2) {
        while (i + b < n && getter(rangeStart[i + b]) < base) {
            i += b;
        }
    }

    return rangeStart + i + 1;
}

#endif // TEXTSELECTOR_H

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


template <typename T, typename SGetter>
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

    std::pair<const_iterator, const_iterator> selection_range(const std::string &text);

    const T &find_match(const std::string &text);
private:
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
public:
    void insert(const std::string &key, const T &t) {
        typename TextSelector<std::pair<std::string, T>, TextSelectorMapSGetter<T> >::value_type p(key, t);
        this->TextSelector<std::pair<std::string, T>, TextSelectorMapSGetter<T> >::insert(std::move(p));
    }
    void insert(const std::string &key, T &&t) {
        typename TextSelector<std::pair<std::string, T>, TextSelectorMapSGetter<T> >::value_type p(key, std::move(t));
        this->TextSelector<std::pair<std::string, T>, TextSelectorMapSGetter<T> >::insert(std::move(p));
    }

    const T &value(const std::string &key)  { return this->find_match(key).second; }

};





template <typename T, typename SGetter>
TextSelector<T, SGetter>::TextSelector() {

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
std::pair<typename TextSelector<T, SGetter>::const_iterator, typename TextSelector<T, SGetter>::const_iterator> TextSelector<T, SGetter>::selection_range(const std::string &text)
{
    typedef std::pair<TextSelector<T, SGetter>::const_iterator, TextSelector<T, SGetter>::const_iterator> ret_pair;
    auto c = closest(text);
    if (c == end()) return ret_pair(c, c);
    SGetter getter;
    if (!boost::starts_with(getter(*c), text)) return ret_pair(end(), end());

    auto rangeBegin = c;
    auto rangeEnd = c + 1;

    while (rangeBegin != begin()) {
        --rangeBegin;
        if (!boost::starts_with(getter(*rangeBegin), text)) {
            ++rangeBegin;
            break;
        }
    }

    while (rangeEnd != end()) {
        if (!boost::starts_with(getter(*rangeEnd), text)) {
            break;
        }
        ++rangeEnd;
    }

    return ret_pair(rangeBegin, rangeEnd);
}

template <typename T, typename SGetter>
const T &TextSelector<T, SGetter>::find_match(const std::string &text)
{
    auto p = selection_range(text);
    if (p.first == end()) throw TextSelectorError::NoMatches;
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
        if (std::lexicographical_compare(base.begin(), base.end(), ib.begin(), ib.end())) {
            return i;
        }
    }

    if (mVector.size() == 1) {
        return mVector.end();
    }

    iterator rangeStart = mVector.begin();
    iterator rangeEnd = mVector.end();


    auto strLessEqual = [](const std::string &a, const std::string &b) {
        return !std::lexicographical_compare(b.begin(), b.end(), a.begin(), a.end());
    };

    int i = 0;
    int n = (rangeEnd - rangeStart);
    for (int b = n / 2; b >= 1; b /= 2) {
        while (i + b < n && strLessEqual(getter(rangeStart[i + b]), base)) {
            i += b;
        }
    }

    return rangeStart + i;
}

#endif // TEXTSELECTOR_H

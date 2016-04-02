#ifndef TEXTUTILS_H
#define TEXTUTILS_H
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;
namespace text {

//Trims and removes duplicate whitespaces
void clean(std::string &str);
std::string cleaned(const std::string &txt);
std::string cleanFolded(const std::string &txt);
std::string lowered(const std::string &txt);
void strip(std::string &str);

//clean + lower
void selectClean(std::string &str);

std::vector<std::string> split(const std::string &s, const std::string &sep);
std::vector<std::string> split(const std::string &s, char sep);
std::string parseItemBaseName(const std::string &s);
void removePrefixes(std::string &txt, const std::initializer_list<std::string> &prefixes);

template <typename Container>
std::string join(const Container &c, const std::string &sep = ", ") {
    std::string ret;
    size_t left = c.size();
    for (auto & i : c) {
        ret += i;
        --left;
        if (left > 0) {
            ret += sep;
        }
    }
    return ret;
}

template <typename Container>
std::string joinWithAnd(const Container &c) {
    std::string ret;
    size_t left = c.size();
    for (auto & i : c) {
        ret += i;
        --left;
        if (left > 1) {
            ret += ", ";
        }
        else if (left == 1) {
            ret += " and ";
        }
    }
    return ret;
}

}
#endif // TEXTUTILS_H

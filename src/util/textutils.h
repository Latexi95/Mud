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

std::vector<std::string> split(const std::string &s, const std::string &sep);
std::vector<std::string> split(const std::string &s, char sep);

template <typename Container>
std::string join(const Container &c, const std::string &sep = ", ") {
    std::string ret;
    size_t left = c.size();
    for (auto & i : c) {
        ret += i;
        --left;
        if (left > 1) {
            ret += sep;
        }
    }
}

}
#endif // TEXTUTILS_H

#include "textutils.h"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/locale.hpp>
using namespace text;

void text::clean(std::string &str) {
    boost::algorithm::trim_if(str, [](char c){
        return c == ' ' || c == '\r' || c == '\n';
    });
    std::string::iterator new_end = std::unique(str.begin(), str.end(), [] (char a, char b) {
        return a == b && a == ' ';
    });
    str.erase(new_end, str.end());
}


std::string text::cleaned(const std::string &txt) {
    std::string str = boost::algorithm::trim_copy_if(txt, [](char c){
        return c == ' ' || c == '\r' || c == '\n';
    });
    std::string::iterator new_end = std::unique(str.begin(), str.end(), [] (char a, char b) {
        return a == b && a == ' ';
    });
    str.erase(new_end, str.end());
    return std::move(str);
}


std::string text::lowered(const std::string &txt) {
    return boost::locale::to_lower(txt);
}



std::string text::cleanFolded(const std::string &txt) {
    std::string ret = boost::locale::fold_case(txt);
    clean(ret);
    return ret;
}


#include "textutils.h"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/locale.hpp>

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


void text::strip(std::string &str)
{
    boost::algorithm::trim_if(str, [](char c){
        return c == ' ' || c == '\r' || c == '\n';
    });
}

std::vector<std::string> text::split(const std::string &s, const std::string &sep) {
    std::vector<std::string> ret;
    std::string::size_type lastFound = 0;
    std::string::size_type found = 0;
    while ((found = s.find(sep, lastFound)) != std::string::npos) {
        ret.emplace_back(s.substr(lastFound, found - lastFound));
        lastFound = found + sep.size();
    }
    ret.emplace_back(s.substr(lastFound));
    return ret;
}

std::vector<std::string> text::split(const std::string &s, char sep) {
    std::vector<std::string> ret;
    std::string::size_type lastFound = 0;
    std::string::size_type found = 0;
    while ((found = s.find(sep, lastFound)) != std::string::npos) {
        ret.emplace_back(s.substr(lastFound, found - lastFound));
        lastFound = found + 1;
    }
    ret.emplace_back(s.substr(lastFound));
    return ret;
}

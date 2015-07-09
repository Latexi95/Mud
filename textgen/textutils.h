#ifndef TEXTUTILS_H
#define TEXTUTILS_H
#include <string>
namespace text {

//Trims and removes duplicate whitespaces
void clean(std::string &str);
std::string cleaned(const std::string &txt);
std::string cleanFolded(const std::string &txt);
std::string lowered(const std::string &txt);



}
#endif // TEXTUTILS_H

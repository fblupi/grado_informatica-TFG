#ifndef UTILS_H
#define UTILS_H

#include <ctime>
#include <string>
#include <algorithm>
#include <limits>

std::string splitAndGetLast(std::string s, const std::string delimiter);
std::string getFileExtension(std::string s);
std::string toUpper(std::string s);
std::string getCurrentDate();

#endif // UTILS_H

#ifndef UTILS_H
#define UTILS_H

#include <string>

std::string splitAndGetLast(std::string s, const std::string delimiter);
std::string getFileExtension(std::string s);
std::string toUpper(std::string s);
double max(const double numbers[]);

#endif // UTILS_H

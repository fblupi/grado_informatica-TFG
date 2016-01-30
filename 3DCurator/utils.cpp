#include "utils.h"

std::string splitAndGetLast(std::string s, const std::string delimiter) {
	size_t pos = 0;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		s.erase(0, pos + delimiter.length());
	}
	return s;
}
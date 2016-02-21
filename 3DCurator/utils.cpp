#include "utils.h"
#include <limits>

std::string splitAndGetLast(std::string s, const std::string delimiter) {
	size_t pos = 0;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		s.erase(0, pos + delimiter.length());
	}
	return s;
}

double max(const double numbers[]) {
	double result = std::numeric_limits<double>::min();
	int size = (int) (sizeof(numbers) / sizeof(numbers[0]));
	for (int i = 0; i < size; i++)
		if (numbers[i] > result)
			result = numbers[i];
	return result;
}
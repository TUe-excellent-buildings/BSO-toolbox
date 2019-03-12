#ifndef TRIM_AND_CAST_HPP
#define TRIM_AND_CAST_HPP

#include <string>

namespace bso { namespace utilities {

	/*
	 * The functions in this file are used to convert data from files
	 * e.g. string to in, string to double, string to char
	 */
	 
	int trim_and_cast_int(std::string); // trims all white space at the end and beginning of a string, and the converts it to an integer
	unsigned int trim_and_cast_uint(std::string s);  // trims all white space at the end and beginning of a string, and the converts it to an unsigned integer
	double trim_and_cast_double(std::string); // trims all white space at the end and beginning of a string, and the converts it to a double
	unsigned long trim_and_cast_ulong(std::string); // trims all white space at the end and beginning of a string, and the converts it to an unsigned long
	char trim_and_cast_char(std::string); // trims all white space at the end and beginning of a string, and the converts it to an integer

} // namespace utilities
} // namespace bso

#include <bso/utilities/trim_and_cast.cpp>

#endif // TRIM_AND_CAST_HPP


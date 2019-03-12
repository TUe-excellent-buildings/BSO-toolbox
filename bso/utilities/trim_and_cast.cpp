#ifndef TRIM_AND_CAST_CPP
#define TRIM_AND_CAST_CPP

#include <string>
#include <sstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <exception>
#include <stdexcept>

// These functions still miss a functionality to check if the string can be represented by the selected variable type, i.e. the number is too large

namespace bso { namespace utilities {

	int trim_and_cast_int(std::string s) // trims all white space at the end and beginning of a string, and the converts it to an integer
	{
		try
		{
			boost::algorithm::trim(s); // trims white space at beginning and end of a string
			return boost::lexical_cast<int>(s); // casts a string as an int
		}
		catch(std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "Failed to trim and cast the following string to an integer: " << s << std::endl
									 << "(bso/trim_and_cast.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // trim_and_cast_int()

	unsigned int trim_and_cast_uint(std::string s) // trims all white space at the end and beginning of a string, and the converts it to an unsigned integer
	{
		try
		{
			boost::algorithm::trim(s); // trims white space at beginning and end of a string
			if (s[0] == '-') throw std::invalid_argument("number in string is negative");
			return boost::lexical_cast<unsigned int>(s); // casts a string as an int
		}
		catch(std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "Failed to trim and cast the following string to an unsigned integer: " << s << std::endl
									 << "(bso/trim_and_cast.cpp)(b)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // trim_and_cast_int()

	unsigned long trim_and_cast_ulong(std::string s) // trims all white space at the end and beginning of a string, and the converts it to an unsigned long
	{
		try
		{
			boost::algorithm::trim(s); // trims white space at beginning and end of a string
			if (s[0] == '-') throw std::invalid_argument("number in string is negative");
			return boost::lexical_cast<unsigned long>(s); // casts a string as an int
		}
		catch(std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "Failed to trim and cast the following string to an unsigned long: " << s << std::endl
									 << "(bso/trim_and_cast.cpp)(b)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // trim_and_cast_int()

	double trim_and_cast_double(std::string s) // trims all white space at the end and beginning of a string, and the converts it to a double
	{
		try
		{
			boost::algorithm::trim(s); // trims white space at beginning and end of a string
			return boost::lexical_cast<double>(s); // casts a string as a double
		}
		catch(std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "Failed to trim and cast the following string to a double: " << s << std::endl
									 << "(bso/trim_and_cast)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // trim_and_cast_double()

	char trim_and_cast_char(std::string s) // trims all white space at the end and beginning of a string, and the converts it to an integer
	{
		try
		{
			boost::algorithm::trim(s); // trims white space at beginning and end of a string
			if (s.size() > 1)
			{
				std::stringstream errorMessage;
				errorMessage << "Cannot trim and cast a string with a size larger than 1 to a char."
										 << "Occured for the following string: " << s << std::endl
										 << "(bso/trim_and_cast)." << std::endl;
				throw std::invalid_argument(errorMessage.str());
			}
			return s.at(0); // casts a string as a char
		}
		catch(std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "Failed to trim and cast the following string to a character: " << s << std::endl
									 << "(bso/trim_and_cast)."
									 << "Received the following error: " << std::endl << e.what() << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // trim_and_cast_char()

} // namespace utilities
} // namespace bso


#endif // TRIM_AND_CAST_CPP


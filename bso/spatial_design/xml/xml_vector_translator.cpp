#ifndef XML_VECTOR_TRANSLATOR_CPP
#define XML_VECTOR_TRANSLATOR_CPP

#include <boost/optional.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <vector>
#include <list>

namespace bso { namespace spatial_design { namespace xml {

template<typename T>
boost::optional<T> container<T>::get_value(const std::string& str) const
{
	if (str.empty()) return boost::none;

	T values;
	std::stringstream ss(str);

	typename T::value_type temp_value;
	while (ss >> temp_value) values.insert(values.end(), temp_value);

	return boost::make_optional(values);
}

template<typename T>
boost::optional<std::string> container<T>::put_value(const T& b) {
		std::stringstream ss;
		size_t i = 0;
		for (auto v : b) ss << (i++?" ":"") << v;
		return ss.str();
}


} // namespace xml
} // namespace spatial_design
} // namespace bso

#endif // XML_VECTOR_TRANSLATOR_CPP

#ifndef XML_VECTOR_TRANSLATOR_HPP
#define XML_VECTOR_TRANSLATOR_HPP

#include <boost/optional.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <vector>
#include <list>

namespace bso { namespace spatial_design { namespace xml {

template<typename T> struct container
{
	// types
	typedef T internal_type;
	typedef T external_type;

	boost::optional<T> get_value(const std::string& str) const;
	boost::optional<std::string> put_value(const T& b);
};

} // namespace xml
} // namespace spatial_design
} // namespace bso

#include <bso/spatial_design/xml/xml_vector_translator.cpp>

namespace boost { namespace property_tree {

template<typename ch, typename traits, typename alloc, typename T>
    struct translator_between<std::basic_string<ch, traits, alloc>, std::vector<T> > {
        typedef bso::spatial_design::xml::container<std::vector<T> > type;
    };

template<typename ch, typename traits, typename alloc, typename T>
    struct translator_between<std::basic_string<ch, traits, alloc>, std::list<T> > {
        typedef bso::spatial_design::xml::container<std::list<T> > type;
    };
} // property_tree
} // boost

#endif // XML_VECTOR_TRANSLATOR_HPP

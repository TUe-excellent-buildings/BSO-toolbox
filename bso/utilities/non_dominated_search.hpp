#ifndef BSO_NON_DOMINATED_SEARCH_HPP
#define BSO_NON_DOMINATED_SEARCH_HPP

#include <bso/utilities/data_point.hpp>

namespace bso { namespace utilities {
	
std::vector<data_point> non_dominated_search(const std::vector<data_point>& data);
std::vector<data_point> non_dominated_search(std::vector<data_point>& data);
	
} // namespace utilities
} // namespace bso

#include <bso/utilities/non_dominated_search.cpp>

#endif // BSO_NON_DOMINATED_SEARCH_HPP
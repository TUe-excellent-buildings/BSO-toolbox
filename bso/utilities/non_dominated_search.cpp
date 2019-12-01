#ifndef BSO_NON_DOMINATED_SEARCH_CPP
#define BSO_NON_DOMINATED_SEARCH_CPP

#include <algorithm>

namespace bso { namespace utilities {
	
std::vector<data_point> non_dominated_search(const std::vector<data_point>& data)
{
	std::vector<data_point> dataCopy = data;
	return non_dominated_search(dataCopy);
}
std::vector<data_point> non_dominated_search(std::vector<data_point>& data)
{
	std::vector<data_point> sorted;
	std::stable_sort(data.begin(), data.end());
	for (const auto& i : data)
	{
		bool nonDominated = true;
		auto j = sorted.begin();
		while (j != sorted.end())
		{
			try
			{
				if (j->dominates(i))
				{
					nonDominated = false;
					++j;
					break;
				}
				else if (j->isDominatedBy(i)) j = sorted.erase(j);
				else ++j;
			}
			catch (std::exception& e)
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, when filtering data for non-dominated points\n"
										 << "Could not compare the following points to each other:\n"
										 << i << "vs" << *j << "\n(bso/utilities/non_dominated_search.cpp)" << std::endl;
				throw std::invalid_argument(errorMessage.str());
			}
		}
		if (nonDominated) sorted.push_back(i);
	}
	return sorted;
}
	
} // namespace utilities
} // namespace bso

#endif // BSO_NON_DOMINATED_SEARCH_CPP
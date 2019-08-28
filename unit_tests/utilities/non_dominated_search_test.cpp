#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE non_dominated_search_test
#endif

#include <bso/utilities/non_dominated_search.hpp>

#include <boost/test/included/unit_test.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace utilities_test {
using namespace bso::utilities;

BOOST_AUTO_TEST_SUITE( non_dominated_search_test )

	BOOST_AUTO_TEST_CASE( non_dominated_search_test_1 )
	{
		std::vector<data_point> data;
		data.push_back({1.5,0.8} ); data.push_back({1.6,1.6} );
		data.push_back({1.3,1.5} ); data.push_back({2,2}     );
		data.push_back({0.4,2}   ); data.push_back({1.75,1}  );
		data.push_back({1.25,1}  ); data.push_back({1.9,1.4} );
		data.push_back({1,2}     ); data.push_back({2,0.5}   );
		data.push_back({1,1}     ); data.push_back({0.75,1.6});
		data.push_back({0.5,1.5} ); data.push_back({1,1}     );
		
		const std::vector<data_point> constData = data;
		std::vector<data_point> PFA = non_dominated_search(constData);
		std::vector<data_point> expectedPFA;
		
		expectedPFA.push_back({1,1}    ); expectedPFA.push_back({2,0.5}  );
		expectedPFA.push_back({0.5,1.5}); expectedPFA.push_back({0.4,2}  );
		expectedPFA.push_back({1,1}    ); expectedPFA.push_back({1.5,0.8});
		
		bool allPFAPointsFound = true;
		for (const auto& i : PFA)
		{
			bool PFAPointFound = false;
			for (const auto& j : expectedPFA)
			{
				if (i.isApproximately(j,1e-9))
				{
					PFAPointFound = true;
					break;
				}
			}	
			if (!PFAPointFound)
			{
				allPFAPointsFound = false;
				break;
			}
		}

		BOOST_REQUIRE(allPFAPointsFound);
	}

BOOST_AUTO_TEST_SUITE_END()
} // namespace utilities_test
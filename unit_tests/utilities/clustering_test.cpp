#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE clustering_test
#endif

#include <bso/utilities/clustering.hpp>

#include <boost/test/included/unit_test.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE_NO_THROW(function)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace utilities_test {
using namespace bso::utilities;

BOOST_AUTO_TEST_SUITE( clustering_tests )

	BOOST_AUTO_TEST_CASE( find_obvious_clusters )
	{
		std::vector<data_point> data;
		data.push_back({1.24740131277634,3.03252999461854} ); data.push_back({5.32259088505753,-0.489850954262087} );
		data.push_back({0.619336021497488,3.37151656277826}); data.push_back({5.1453586751622,-0.96571212231499}   );
		data.push_back({1.19019268308247,3.66721538646004} ); data.push_back({4.91738326858314,-1.1787258123153}   );
		data.push_back({1.11091034114473,2.59025003905794} ); data.push_back({4.87804529105638,-1.61668855846781}  );
		data.push_back({0.964076845771258,2.24972130280141}); data.push_back({4.83166276402593,-1.37883512310901}  );
		data.push_back({1.30734378641359,2.90263314225433} ); data.push_back({4.9225140306227,-0.83502096234554}   );
		data.push_back({1.00499545877054,2.53182465726257} ); data.push_back({4.75039664381847,-0.889778488851693} );
		data.push_back({0.649536142326923,3.4543080362376} ); data.push_back({5.39250116619433,-1.34633553762144}  );
		data.push_back({0.508074026459054,2.7438586861826} ); data.push_back({5.23110837374169,-1.33286763704783}  );
		data.push_back({1.5341993546649,4.14754300157928}  ); data.push_back({5.15413810447812,-0.470460614644415} );
		data.push_back({0.521430936893757,3.29938293383002}); data.push_back({4.85481936545584,-0.986417756846036} );
		data.push_back({1.60169763759077,3.41618409731869} ); data.push_back({4.9652560546593,-1.52414195574719}   );
		data.push_back({1.52938638902811,2.8710791846115}  ); data.push_back({4.55887645328487,-1.24894597164154}  );
		data.push_back({0.915509027371616,3.21185270861969}); data.push_back({5.28262369026563,-2.0541716810767}   );
		data.push_back({1.10291186733229,3.15916314552112} ); data.push_back({5.24894702129262,-1.31288737088494}  );
		data.push_back({1.38383762934923,2.64369023326389} ); data.push_back({5.23312281953745,-1.33957326552774}  );
		data.push_back({0.424876591513848,3.43208263124734}); data.push_back({4.98524524762386,-0.414680659759803} );
		data.push_back({1.09529943812012,2.54552051847663} ); data.push_back({4.93549860901713,-0.954307144676565} );
		data.push_back({1.26733181113469,2.64374750858471} ); data.push_back({5.5099333144326,-0.703850392327289}  );
		data.push_back({0.959659693835958,2.47832396975856}); data.push_back({5.27176374752174,-0.624549883433043} );
		data.push_back({1.01547535106872,3.68495879613578} ); data.push_back({5.16074795295209,-1.07233155313127}  );
		data.push_back({1.06254514263235,1.90556042293415} ); data.push_back({5.28107037954703,-1.06180660818346}  );
		data.push_back({0.885376399367381,4.15980646859108}); data.push_back({4.3907956120954,-1.08293617328034}   );
		data.push_back({0.954950211387253,3.1754861217077} ); data.push_back({5.26212797033799,-1.70484475232777}  );
		data.push_back({1.00132911284739,2.91716085526852} ); data.push_back({4.91063210147054,-0.70088566375136}  );
		data.push_back({4.70891896936562,7.65189298610404} ); data.push_back({9.82071270946696,0.143287931641203}  );
		data.push_back({4.91039695819805,8.35040464888253} ); data.push_back({9.9783084513909,-0.819393080597467}  );
		data.push_back({5.06437254660816,8.4560227367839}  ); data.push_back({10.0391354970018,-0.142148257797337} );
		data.push_back({4.91591634041346,8.05630589403357} ); data.push_back({9.87935998991159,0.0882067561738678} );
		data.push_back({4.76714167384596,8.75160580491573} ); data.push_back({9.98472166796841,-0.251485567899992} );
		data.push_back({4.85172908008955,7.52303184484658} ); data.push_back({9.88062941294884,0.0392824733802193} );
		data.push_back({5.08820886708282,8.20559435025782} ); data.push_back({9.65261624017504,-0.0627686972936707});
		data.push_back({5.14962343161766,8.27485875417309} ); data.push_back({9.77031939245969,0.276992480152888}  );
		data.push_back({5.08990027272858,7.36049155114318} ); data.push_back({9.94657920292901,0.833206197066224}  );
		data.push_back({4.92064592566437,7.87640741462322} ); data.push_back({10.3807401313396,0.213624604912244}  );
		data.push_back({5.15429399401063,7.50063531062128} ); data.push_back({9.43011586705744,-0.306592847754617} );
		data.push_back({5.26441073888309,8.2059174510731}  ); data.push_back({9.7357033114561,-0.382444679536211}  );
		data.push_back({5.04963705932845,9.1742136942703}  ); data.push_back({9.72387963748767,0.0768867076657814} );
		data.push_back({4.87628335320346,7.99473208392279} ); data.push_back({10.1211107469156,0.365334158728112}  );
		data.push_back({4.87095011743807,7.758017473027}   ); data.push_back({10.2771413069583,0.475409960909908}  );
		data.push_back({4.74543689388581,7.75221215252718} ); data.push_back({9.80764390481436,-0.511591468216199} );
		data.push_back({4.99517098416568,8.18754373761485} ); data.push_back({9.7500288759686,0.179521725748613}   );
		data.push_back({4.52828255644304,7.9049592342761}  ); data.push_back({10.4192283825426,0.252496971065259}  );
		data.push_back({4.99496391178635,6.71054230482415} ); data.push_back({10.1717337070972,0.107339574090626}  );
		data.push_back({5.02926091101965,7.2018551301709}  ); data.push_back({9.99921680409461,-0.686028863724267} );
		data.push_back({4.92017700119578,6.77975588547942} ); data.push_back({9.96210203538728,0.184408574874081}  );
		data.push_back({5.50523630358575,7.89227127189408} ); data.push_back({10.1767043400911,0.315282003488927}  );
		data.push_back({5.02894314562499,8.68148102040148} ); data.push_back({9.62976642634679,-0.253809107086391} );
		data.push_back({5.14032739151368,8.13913467705429} ); data.push_back({10.2207748287601,0.534492422426376}  );
		data.push_back({4.90420205237818,8.48726703595436} ); data.push_back({10.4056292160935,0.233794783907307}  );

		std::vector<data_point> expectedCentroids;
		expectedCentroids.push_back({1.03430732849523,3.04941601620408});
		expectedCentroids.push_back({4.97897721920307,7.955086177955});
		expectedCentroids.push_back({5.05588638168938,-1.09162426574301});
		expectedCentroids.push_back({9.96655608346652,0.0361321902530194});

		BOOST_REQUIRE_NO_THROW(clustering c1(data));
		clustering c1(data);
		BOOST_REQUIRE(c1.getClusters().size() == 4);
		
		bool allCentroidsFound = true;
		for (const auto& i : c1.getClusters())
		{
			bool centroidFound = false;
			for (const auto& j : expectedCentroids)
			{
				if (i.getCentroid().isApproximately(j,1e-9))
				{
					centroidFound = true;
					break;
				}
			}	
			if (!centroidFound)
			{
				allCentroidsFound = false;
				break;
			}
		}

		BOOST_REQUIRE(allCentroidsFound);
	}
	
	BOOST_AUTO_TEST_CASE( reproducibility )
	{
		std::vector<data_point> data;
		std::mt19937 RNGEngine;
		RNGEngine.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		// generate the data
		for (unsigned int i = 0; i < 500; ++i)
		{
			std::vector<double> randomDataPoint(4);
			for (auto& j : randomDataPoint) j = (RNGEngine()%1000000000)*1.0e-8;
			data.push_back(data_point(randomDataPoint));
		}
		
		clustering c1(data);
		clustering c2(c1.getRNGSeed(),data);
		
		for (unsigned int i = 0; i < c1.getClusters().size(); ++i)
		{
			BOOST_REQUIRE(c1.getClusters()[i].getCentroid().isApproximately(
										c2.getClusters()[i].getCentroid(),1e-9));
		}
	}

BOOST_AUTO_TEST_SUITE_END()
} // namespace utilities_test
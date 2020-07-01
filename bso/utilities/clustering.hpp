#ifndef BSO_CLUSTERING_HPP
#define BSO_CLUSTERING_HPP

#include <bso/utilities/data_point.hpp>
#include <bso/utilities/cluster.hpp>
#include <vector>
#include <random>

namespace bso { namespace utilities {
	
class clustering
{
private:
	std::vector<std::shared_ptr<data_point> > mData;
	std::vector<cluster> mClusters;
	mutable std::mt19937 mRNGEngine;
	unsigned long mRNGSeed;
	unsigned int mKMin;
	unsigned int mKMax;
	unsigned int mRuns;
	
	void mMakeClustering();
	std::vector<cluster> mKMeans(const unsigned int& k);
	void mInitialize(const std::vector<data_point>& data);
public:
	clustering(const std::vector<data_point>& data, const unsigned int& kMin = 2,
		const unsigned int& kMax = 10, const unsigned int& runs = 50);
	clustering(const unsigned long& RNGSeed, const std::vector<data_point>& data,
		const unsigned int& kMin = 2, const unsigned int& kMax = 10,
		const unsigned int& runs = 50);
	~clustering();
	
	const std::vector<cluster>& getClusters() const {return mClusters;}
	const unsigned int getRNGSeed() const {return mRNGSeed;}
};
	
} // namespace utilities
} // namespace bso

#include <bso/utilities/clustering.cpp>

#endif // BSO_CLUSTERING_HPP
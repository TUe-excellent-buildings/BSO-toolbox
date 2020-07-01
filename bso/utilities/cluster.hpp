#ifndef BSO_CLUSTER_HPP
#define BSO_CLUSTER_HPP

#include <memory>

namespace bso { namespace utilities {
	
class cluster
{
private:
	std::vector<std::shared_ptr<data_point> > mData;
	data_point mCentroid;
public:
	cluster(data_point centroid);
	~cluster();
	
	data_point calculateMean() const;
	double calculateVariance() const;
	double calculateDistanceToCentroid(const data_point& p) const;
	void addDataPoint(std::shared_ptr<data_point> dp);
	
	const data_point& getCentroid() const {return mCentroid;}
	const std::vector<std::shared_ptr<data_point>>& getData() const {return mData;}
};

} // namespace utilities
} // namespace bso

#include <bso/utilities/cluster.cpp>

#endif // BSO_CLUSTER_HPP
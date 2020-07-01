#ifndef BSO_CLUSTER_CPP
#define BSO_CLUSTER_CPP

namespace bso { namespace utilities {

cluster::cluster(data_point centroid) : mCentroid(centroid), mData(std::vector<std::shared_ptr<data_point> >())
{
	
} // ctor

cluster::~cluster()
{
	
} // dtor

data_point cluster::calculateMean() const
{
	data_point mean;
	try
	{
		if (mData.size() == 0)
		{
			throw std::runtime_error("Error, cluster contains no data.");
		}
		else
		{
			mean = data_point((*mData.begin())->size());
		}
		for (const auto& i : mData)
		{
			mean += *i;
		}
		mean /= mData.size();
	}
	catch (std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, while calculating the mean of a cluster,\n"
								 << "received the following error:\n" << e.what() << "\n"
								 << "(bso/utilities/cluster.cpp)" << std::endl;
		throw std::runtime_error(errorMessage.str());
	}
	return mean;
} // calculateMean

double cluster::calculateVariance() const
{
	double variance = 0;
	try
	{
		auto mean = this->calculateMean();
		for (const auto& i : mData)
		{
			variance += mean.calcSquaredDistanceTo(*i);
		}
		variance /= mData.size();
	}
	catch (std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, while calculating the variance of a cluster,\n"
								 << "received the following error:\n" << e.what() << "\n"
								 << "(bso/utilities/cluster.cpp)" << std::endl;
		throw std::runtime_error(errorMessage.str());
	}
	return variance;
} // calculateVariance

double cluster::calculateDistanceToCentroid(const data_point& p) const
{
	return p.calcDistanceTo(mCentroid);
} // calculateDistanceToCentroid

void cluster::addDataPoint(std::shared_ptr<data_point> dp)
{
	mData.push_back(dp);
} // addDataPoint

} // namespace utilities
} // namespace bso

#endif // BSO_CLUSTER_CPP
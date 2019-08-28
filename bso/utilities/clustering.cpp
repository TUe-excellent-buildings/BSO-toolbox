#ifndef BSO_CLUSTERING_CPP
#define BSO_CLUSTERING_CPP

#include <chrono>
#include <sstream>
#include <stdexcept>

namespace bso { namespace utilities {

void clustering::mMakeClustering()
{
	// cluster for different values of k
	std::map<unsigned int, std::vector<cluster>> clusterings;
	std::map<unsigned int, double> variances; 
	for (unsigned int i = mKMin-1; i <= mKMax+1; ++i)
	{
		std::vector<cluster> clustering;
		double variance;
		for (unsigned int j = 0; j < mRuns; ++j)
		{
			std::vector<cluster> newClustering = this->mKMeans(i);
			double averageVariance = 0;
			for (const auto& k : newClustering)
			{
				averageVariance += k.calculateVariance();
			}
			averageVariance /= newClustering.size();
			
			if (j == 0)
			{
				clustering = newClustering;
				variance = averageVariance;
			}
			else if (variance > averageVariance)
			{
				clustering = newClustering;
				variance = averageVariance;
			}
		}
		clusterings[i] = clustering;
		variances[i] = variance;
	}
	
	// find the best value for k, according to:
	// W. J. Krzanowski and  Y. T. Lai (1988) A Criterion
	// for Determining the Number of Groups in a Data Set Using
	// Sum-of-Squares clustering
	std::map<unsigned int, double> DIFF;
	std::map<unsigned int, double> C;
	unsigned int p = mData.front()->size();
	for (unsigned int i = mKMin; i <= mKMax+1; ++i)
	{
		DIFF[i] = pow(i-1,(double)(2/p))*variances[i-1] - 
							pow(i,(double)(2/p))*variances[i];
	}
	for (unsigned int i = mKMin; i <= mKMax; ++i)
	{
		C[i] = abs(DIFF[i]/DIFF[i+1]);
	}
	double maxC = 0;
	unsigned int bestK = mKMin;
	for (unsigned int i = mKMin; i <= mKMax; ++i)
	{
		if (maxC < C[i])
		{
			maxC = C[i];
			bestK = i;
		}
	}
	
	mClusters = clusterings[bestK];
	
	/*
	std::map<unsigned int, double> varianceSecondDerivatives;
	for (unsigned int i = mKMin; i <= mKMax; ++i)
	{
		varianceSecondDerivatives[i] = clusterings[i-1].first + 
			clusterings[i+1].first - 2 * (clusterings[i].first);
	}
	
	unsigned int bestNumberOfClusters = mKMin;
	for (const auto& i : varianceSecondDerivatives)
	{std::cout << i.first << " - " << i.second << std::endl;
		if (i.second > varianceSecondDerivatives[bestNumberOfClusters])
		{
			bestNumberOfClusters = i.first;
		}
	}
	mClusters = clusterings[bestNumberOfClusters].second;*/
	
} // mMakeClustering()

std::vector<cluster> clustering::mKMeans(const unsigned int& kSize)
{
	std::vector<cluster> clustering;
	std::vector<unsigned int> dataIndices(mData.size());
	for (unsigned int i = 0; i < mData.size(); ++i) dataIndices[i] = i;
	std::shuffle(dataIndices.begin(), dataIndices.end(), mRNGEngine);
	std::vector<data_point> centroids;
	for (unsigned int i = 0; i < kSize; ++i)
	{
		centroids.push_back(*(mData[dataIndices.back()]));
		dataIndices.pop_back();
	}
	
	bool converged;
	do
	{
		clustering.clear();
		for (const auto& i : centroids)
		{
			clustering.push_back(cluster(i));
		}
		
		for (const auto& i : mData)
		{
			data_point closest = *(i->findClosestIn(centroids).begin());
			bool centroidFound = false;
			for (unsigned int j = 0; j < centroids.size(); ++j)
			{
				if (closest.isApproximately(centroids[j],1e-9))
				{
					clustering[j].addDataPoint(i);
					centroidFound = true;
					break;
				}
			}
			if (!centroidFound)
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, while clustering could not find the index of a centroid.\n"
										 << "(bso/utilities/clustering.cpp)" << std::endl;
				throw std::runtime_error(errorMessage.str());
			}
		}

		converged = true;
		std::vector<unsigned int> skippedIndices;
		for (unsigned int i = 0; i < kSize; ++i)
		{
			if (clustering[i].getData().size() == 0)
			{
				skippedIndices.push_back(i);
				continue;
			}

			data_point newCentroid = clustering[i].calculateMean();
			if (newCentroid != clustering[i].getCentroid())
			{
				converged = false;
				centroids[i] = newCentroid;
			}
		}
		for (const auto& i : skippedIndices)
		{
			data_point newCentroid;
			bool foundNewCentroid = false;
			for (const auto& j : dataIndices)
			{
				newCentroid = *mData[j];
				bool newCentroidNotUsed = true;
				for (unsigned int k = 0; k < kSize; ++k)
				{
					if (std::find(skippedIndices.begin(),skippedIndices.end(),k) != skippedIndices.end())
					{
						if (centroids[k] == newCentroid)
						{
							newCentroidNotUsed = false;
							break;
						}
					}
				}
				if (newCentroidNotUsed) 
				{
					foundNewCentroid = true;
					break;
				}
			}
			if (!foundNewCentroid)
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, cannot reinitiate a centroid while clustering\n"
										 << "(bso/utilities/clustering)" << std::endl;
				throw std::runtime_error(errorMessage.str());
			}
			if (newCentroid != clustering[i].getCentroid())
			{
				converged = false;
				centroids[i] = newCentroid;
			}
		}
	} while(!converged);

	return clustering;
} // mKMeans()

void clustering::mInitialize(const std::vector<data_point>& data)
{
	std::stringstream errorMessage;
	errorMessage << "\nError while initializing a clustering.\n";
	bool throwError = false;
	if (mKMin < 2)
	{
		errorMessage << "kMin should be larger than 1\n";
		throwError = true;
	}
	else if (mKMin > mKMax)
	{
		errorMessage << "kMin should be less or equal to kMax\n";
		throwError = true;
	}
	else if (mRuns == 0)
	{
		errorMessage << "runs should be more than zero\n";
		throwError = true;
	}
	else if (mKMax > data.size())
	{
		errorMessage << "size of the clustering data collection should be larger than kMax\n";
		throwError = true;
	}
	
	if (throwError)
	{
		errorMessage << "(bso/utilities/clustering.hpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	
	mRNGEngine.seed(mRNGSeed);
	for (const auto& i : data)
	{
		mData.push_back(new data_point(i));
	}

	this->mMakeClustering();
} // mCheckInitialization()

clustering::clustering(const std::vector<data_point>& data,
	const unsigned int& kMin /*= 2*/,	const unsigned int& kMax /*= 10*/,
	const unsigned int& runs /*= 50*/)
: mRNGSeed(std::chrono::high_resolution_clock::now().time_since_epoch().count()),
	mKMin(kMin), mKMax(kMax), mRuns(runs)
{
	this->mInitialize(data);
} // ctor()

clustering::clustering( const unsigned long& RNGSeed, 
	const std::vector<data_point>& data, const unsigned int& kMin /*= 2*/,
	const unsigned int& kMax /*= 10*/, const unsigned int& runs /*= 50*/)
: mRNGSeed(RNGSeed), mKMin(kMin), mKMax(kMax), mRuns(runs)
{
	this->mInitialize(data);
} // ctor()

clustering::~clustering()
{
	for (const auto& i : mData) delete i;
} // dtor()


} // namespace utilities
} // namespace bso

#endif // BSO_CLUSTERING_CPP
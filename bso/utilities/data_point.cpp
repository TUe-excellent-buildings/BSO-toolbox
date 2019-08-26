#ifndef BSO_DATA_POINT_CPP
#define BSO_DATA_POINT_CPP

#include <stdexcept>
#include <sstream>
#include <type_traits>

namespace bso { namespace utilities {

void data_point::mCheckValidDimensions(const data_point& pCheck) const
{
	if (mData.size() != pCheck.mData.size())
	{
		std::stringstream errorMessage;
		errorMessage << "\nCannot perform operation between data points\n"
								 << "because they do not have the same dimensions.\n"
								 << "(bso/utilities/data_point.hpp)" << std::endl;
		throw std::runtime_error(errorMessage.str());
	}
} // mCheckValidDimensions()

template <class CONTAINER>
void data_point::mInitFromContainer(const CONTAINER& l)
{
	mData = Eigen::VectorXd(std::distance(std::begin(l),std::end(l)));
	unsigned int index = 0;
	for (const auto& i : l)
	{
		mData(index) = i;
		++index;
	}
} // mInitFromContainer()

data_point::data_point() : mData(Eigen::VectorXd())
{
	
} // ctor()

data_point::data_point(const Eigen::VectorXd& data) : mData(data)
{
	
} // ctor()

data_point::data_point(const std::vector<double>& l)
{
	this->mInitFromContainer(l);
} // ctor()

data_point::data_point(const std::initializer_list<double>&& l)
{
	this->mInitFromContainer(std::move(l));
} // ctor()

data_point::~data_point()
{
	
} // dtor()

double data_point::calcDistanceTo(const data_point& p2) const
{
	double distance = 0.0;
	try 
	{
		this->mCheckValidDimensions(p2);
		distance = (mData - p2.mData).norm();
	}
	catch(std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, while calculating distance between data points:\n"
								 << mData.transpose()
								 << "\nand:\n"
								 << p2.mData.transpose()
								 << "\nreceived the following error:"
								 << e.what()
								 << "(bso/utilities/data_point.hpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return distance;
} // calcDistanceTo()

void data_point::normalize(const data_point& pStart, const data_point& pEnd)
{
	try
	{
		this->mCheckValidDimensions(pStart);
		this->mCheckValidDimensions(pEnd);
		Eigen::VectorXd totalDifference = pEnd.mData - pStart.mData;
		Eigen::VectorXd relativeDifference = mData - pStart.mData;
		for (unsigned int i = 0; i < mData.size(); ++i)
		{
			if (totalDifference(i) != 0)
			{
				mData(i) = relativeDifference(i) / totalDifference(i);
			}
			else mData(i) = 0;
		}
	}
	catch(std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, while normalizing a data point:\n"
								 << mData.transpose()
								 << "\nwith data points:\n"
								 << pStart.mData.transpose()
								 << "\nand\n"
								 << pEnd.mData.transpose()
								 << "\nreceived the following error:"
								 << e.what()
								 << "(bso/utilities/data_point.hpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
} // normalize()

data_point data_point::normalized(const data_point& pStart, const data_point& pEnd) const
{
	data_point copy = *this;
	copy.normalize(pStart,pEnd);
	return copy;
} // normalized()

template<class CONTAINER>
void data_point::setToUtopia(const CONTAINER& dataCollection)
{
	try
	{
		mData = (dataCollection.begin())->mData;
		for (const auto& i : dataCollection)
		{
			this->mCheckValidDimensions(i);
			for (unsigned int j = 0; j < i.size(); ++j)
			{
				if (i(j) < mData(j)) mData(j) = i(j);
			}
		}
	}
	catch(std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\n Error, while setting a data point to the utopian point of\n"
								 << "a collection. Received the following error:\n"
								 << e.what()
								 << "\n(bso/utilities/data_point.hpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
} // setToUtopia()

template<class CONTAINER>
void data_point::setToDistopia(const CONTAINER& dataCollection)
{
	try
	{
		mData = (dataCollection.begin())->mData;
		for (const auto& i : dataCollection)
		{
			this->mCheckValidDimensions(i);
			for (unsigned int j = 0; j < i.size(); ++j)
			{
				if (i(j) > mData(j)) mData(j) = i(j);
			}
		}
	}
	catch(std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\n Error, while setting a data point to the distopian point of\n"
								 << "a collection. Received the following error:\n"
								 << e.what()
								 << "\n(bso/utilities/data_point.hpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
} // setToDistopia()

template<class CONTAINER>
std::vector<data_point> data_point::findClosestIn(const CONTAINER& dataCollection,
	const double& tol /*= 1e-3*/) const
{
	std::vector<data_point> closest;
	try
	{
		double smallestDistance = 0;
		for (const auto& i : dataCollection)
		{
			smallestDistance = this->calcDistanceTo(i);
			if (smallestDistance != 0) break;
		}
		if (smallestDistance == 0)
		{
			throw std::invalid_argument("Container is either empty or is filled with the same data point.");
		}
		for (const auto& i : dataCollection)
		{
			double distance = this->calcDistanceTo(i);
			if ((distance/smallestDistance - 1) < -tol)
			{
				closest.clear();
				closest.push_back(i);
				smallestDistance = distance;
			}
			else if ((distance/smallestDistance - 1) < tol)
			{
				closest.push_back(i);
			}
		}
	}
	catch(std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\n Error, while finding the closest data point to another data\n"
								 << "point from a collection. Received the following error:\n"
								 << e.what()
								 << "\n(bso/utilities/data_point.hpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}

	return closest;
} // findClosestIn()

template<class CONTAINER>
std::vector<data_point> data_point::findFurthestIn(const CONTAINER& dataCollection,
	const double& tol /*= 1e-3*/) const
{
	std::vector<data_point> furthest;
	try
	{
		double largestDistance = 0;
		for (const auto& i : dataCollection)
		{
			largestDistance = this->calcDistanceTo(i);
			if (largestDistance != 0) break;
		}
		if (largestDistance == 0)
		{
			throw std::invalid_argument("Container is either empty or is filled with the same data point.");
		}
		for (const auto& i : dataCollection)
		{
			double distance = this->calcDistanceTo(i);
			if ((distance/largestDistance - 1) > tol)
			{
				furthest.clear();
				furthest.push_back(i);
				largestDistance = distance;
			}
			else if ((distance/largestDistance - 1) > -tol)
			{
				furthest.push_back(i);
			}
		}
	}
	catch(std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\n Error, while finding the furthest data point from another data\n"
								 << "point from a collection. Received the following error:\n"
								 << e.what()
								 << "\n(bso/utilities/data_point.hpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}

	return furthest;
} // findFurthestIn()

std::ostream& operator << (std::ostream& stream, const data_point& p)
{
	Eigen::IOFormat layout(Eigen::StreamPrecision, Eigen::DontAlignCols, ",", ",", "{", "}", "", "");
	stream << p.mData.transpose().format(layout);
	return stream;
}

} // namespace utilities
} // namespace bso

#endif // BSO_DATA_POINT_CPP
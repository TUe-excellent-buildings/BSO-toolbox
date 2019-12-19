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

data_point::data_point(const unsigned int& size) : mData(Eigen::VectorXd::Zero(size))
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

double data_point::calcSquaredDistanceTo(const data_point& p2) const
{
	double squaredDistance = 0.0;
	try 
	{
		this->mCheckValidDimensions(p2);
		squaredDistance = (mData - p2.mData).squaredNorm();
	}
	catch(std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, while calculating squared distance between data points:\n"
								 << mData.transpose()
								 << "\nand:\n"
								 << p2.mData.transpose()
								 << "\nreceived the following error:"
								 << e.what()
								 << "(bso/utilities/data_point.hpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return squaredDistance;
}

void data_point::normalize(const data_point& pStart, const data_point& pEnd, 
	const std::vector<unsigned int>& logScales /*= {}*/)
{
	try
	{
		this->mCheckValidDimensions(pStart);
		this->mCheckValidDimensions(pEnd);
		double totalDifference;
		double relativeDifference;
		for (unsigned int i = 0; i < mData.size(); ++i)
		{
			double a = pStart(i);
			double b = pEnd(i);
			double x = mData(i);
			if (std::find(logScales.begin(),logScales.end(),i) != logScales.end())
			{
				a = ((a>0)? 1 : -1)*log(1+abs(a)/10e-150); // 10e-500 shifts the function so it can handle negative numbers, but this means double looses about half of its exponential precision (11 bits)
				b = ((b>0)? 1 : -1)*log(1+abs(b)/10e-150);
				x = ((x>0)? 1 : -1)*log(1+abs(x)/10e-150);
			}
			totalDifference = b-a;
			relativeDifference = x-a;

			if (totalDifference != 0)
			{
				mData(i) = relativeDifference / totalDifference;
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
		if (std::distance(dataCollection.begin(),dataCollection.end()) == 0)
		{
			throw std::invalid_argument("Container is empty.");
		}
		double smallestDistance = 0;
		for (const auto& i : dataCollection)
		{
			smallestDistance = this->calcDistanceTo(i);
			if (smallestDistance != 0) break;
		}
		for (const auto& i : dataCollection)
		{
			double distance = this->calcDistanceTo(i);
			if ((distance == 0 && smallestDistance == 0) || 
					(smallestDistance != 0 && (distance/smallestDistance - 1) < -tol))
			{
				closest.clear();
				closest.push_back(i);
				smallestDistance = distance;
			}
			else if (smallestDistance != 0 && (distance/smallestDistance - 1) < tol)
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
		if (std::distance(dataCollection.begin(),dataCollection.end()) == 0)
		{
			throw std::invalid_argument("Container is empty.");
		}
		double largestDistance = 0;
		for (const auto& i : dataCollection)
		{
			largestDistance = this->calcDistanceTo(i);
			if (largestDistance != 0) break;
		}
		for (const auto& i : dataCollection)
		{
			double distance = this->calcDistanceTo(i);
			if ((distance == 0 && largestDistance == 0) || 
					(largestDistance != 0 && (distance/largestDistance - 1) > tol))
			{
				furthest.clear();
				furthest.push_back(i);
				largestDistance = distance;
			}
			else if (largestDistance != 0 && (distance/largestDistance - 1) > -tol)
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

double data_point::aggregateToSum(data_point weights /*= data_point(0)*/) const
{
	if (weights.size() == 0) weights.mData = Eigen::VectorXd::Ones(mData.size());
	this->mCheckValidDimensions(weights);
	auto weighedData = mData.cwiseProduct(weights.mData);
	return weighedData.sum();
} // aggregateToSum()

double data_point::aggregateToProduct(data_point weights /*= data_point(0)*/) const
{
	if (weights.size() == 0) weights.mData = Eigen::VectorXd::Ones(mData.size());
	this->mCheckValidDimensions(weights);
	auto weighedData = mData.cwiseProduct(weights.mData);
	return weighedData.prod();
} // aggregateToProduct()

double data_point::aggregateToDistance(const data_point& measurePoint,
	data_point weights /*= data_point(0)*/) const
{
	if (weights.size() == 0) weights.mData = Eigen::VectorXd::Ones(mData.size());
	this->mCheckValidDimensions(weights);
	this->mCheckValidDimensions(measurePoint);
	data_point weighedData(mData.cwiseProduct(weights.mData));
	data_point weighedMeasurePoint(measurePoint.mData.cwiseProduct(weights.mData));
	return weighedData.calcDistanceTo(weighedMeasurePoint);
} // aggregateToDistance()

std::ostream& operator << (std::ostream& stream, const data_point& p)
{
	Eigen::IOFormat layout(Eigen::StreamPrecision, Eigen::DontAlignCols, ",", ",", "{", "}", "", "");
	stream << p.mData.transpose().format(layout);
	return stream;
}

data_point data_point::operator + (const data_point& rhs) const
{
	data_point addition = *this;
	try
	{
		this->mCheckValidDimensions(rhs);
		addition += rhs;
	}
	catch ( std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to use + operator on data_point,\n"
								 << "received the following error: " << e.what() << "\n"
								 << "(bso/utilities/data_point.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return addition;
}

data_point data_point::operator - (const data_point& rhs) const
{
	data_point subtraction = *this;
	try
	{
		this->mCheckValidDimensions(rhs);
		subtraction -= rhs;
	}
	catch ( std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to use - operator on data_point,\n"
								 << "received the following error: " << e.what() << "\n"
								 << "(bso/utilities/data_point.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return subtraction;
}

data_point& data_point::operator += (const data_point& rhs)
{
	try
	{
		this->mCheckValidDimensions(rhs);
		mData += rhs.mData;
	}
	catch ( std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to use += operator on data_point,\n"
								 << "received the following error: " << e.what() << "\n"
								 << "(bso/utilities/data_point.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return *this;
}

data_point& data_point::operator -= (const data_point& rhs)
{
	try
	{
		this->mCheckValidDimensions(rhs);
		mData -= rhs.mData;
	}
	catch (std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to use -= operator on data_point,\n"
								 << "received the following error: " << e.what() << "\n"
								 << "(bso/utilities/data_point.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return *this;
}

data_point data_point::operator * (const double& rhs) const
{
	data_point product = *this;
	try
	{
		product *= rhs;
	}
	catch ( std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to use * operator on data_point,\n"
								 << "received the following error: " << e.what() << "\n"
								 << "(bso/utilities/data_point.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return product;
}

data_point data_point::operator / (const double& rhs) const
{
	data_point division = *this;
	try
	{
		division /= rhs;
	}
	catch ( std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to use / operator on data_point,\n"
								 << "received the following error: " << e.what() << "\n"
								 << "(bso/utilities/data_point.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return division;
}

data_point& data_point::operator *= (const double& rhs)
{
	try
	{
		mData *= rhs;
	}
	catch ( std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to use *= operator on data_point,\n"
								 << "received the following error: " << e.what() << "\n"
								 << "(bso/utilities/data_point.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return *this;
}

data_point& data_point::operator /= (const double& rhs)
{
	try
	{
		mData /= rhs;
	}
	catch ( std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to use /= operator on data_point,\n"
								 << "received the following error: " << e.what() << "\n"
								 << "(bso/utilities/data_point.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return *this;
}

bool data_point::operator <= (const data_point& rhs) const
{
	bool check = true;
	try
	{
		this->mCheckValidDimensions(rhs);
		for (unsigned int i = 0; i < mData.size(); ++i)
		{
			if (mData(i) > rhs.mData(i))
			{
				check = false;
				break;
			}
		}
	}
	catch(std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to use <= operator on data_point,\n"
								 << "received the following error: " << e.what() << "\n"
								 << "(bso/utilities/data_point.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return check;
} // operator <=

bool data_point::operator >= (const data_point& rhs) const
{
	bool check = true;
	try
	{
		this->mCheckValidDimensions(rhs);
		for (unsigned int i = 0; i < mData.size(); ++i)
		{
			if (mData(i) < rhs.mData(i))
			{
				check = false;
				break;
			}
		}
	}
	catch(std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to use >= operator on data_point,\n"
								 << "received the following error: " << e.what() << "\n"
								 << "(bso/utilities/data_point.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return check;
} // operator >=

bool data_point::operator < (const data_point& rhs) const
{
	bool check;
	try
	{
		this->mCheckValidDimensions(rhs);
		check = !(this->operator >= (rhs));
	}
	catch(std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to use < operator on data_point,\n"
								 << "received the following error: " << e.what() << "\n"
								 << "(bso/utilities/data_point.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return check;
} // operator <
 
bool data_point::operator > (const data_point& rhs) const
{
	bool check;
	try
	{
		this->mCheckValidDimensions(rhs);
		check = !(this->operator <= (rhs));
	}
	catch(std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to use > operator on data_point,\n"
								 << "received the following error: " << e.what() << "\n"
								 << "(bso/utilities/data_point.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return check;
} // operator >
 
bool data_point::operator == (const data_point& rhs) const
{
	bool check = true;
	try
	{
		this->mCheckValidDimensions(rhs);
		for (unsigned int i = 0; i < mData.size(); ++i)
		{
			if ((mData(i) < rhs.mData(i)) || (mData(i) > rhs.mData(i)))
			{
				check = false;
				break;
			}
		}
	}
	catch(std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to use == operator on data_point,\n"
								 << "received the following error: " << e.what() << "\n"
								 << "(bso/utilities/data_point.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return check;
} // operator ==
 
bool data_point::operator != (const data_point& rhs) const
{
	bool check = true;
	try
	{
		this->mCheckValidDimensions(rhs);
		check = !(this->operator == (rhs));
	}
	catch(std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to use != operator on data_point,\n"
								 << "received the following error: " << e.what() << "\n"
								 << "(bso/utilities/data_point.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return check;
} // operator !=

bool data_point::isApproximately(const data_point& rhs,
	const double tol /*= 1e-3*/) const
{
	bool check;
	try
	{
		this->mCheckValidDimensions(rhs);
		check = mData.isApprox(rhs.mData,tol);
	}
	catch (std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, checking if two data points are approximately equal,\n"
								 << "received the following error: " << e.what() << "\n"
								 << "(bso/utilities/data_point.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return check;
} // isApproximately()

bool data_point::dominates(const data_point& rhs) const
{
	bool check = false;
	try
	{
		this->mCheckValidDimensions(rhs);
		for (unsigned int i = 0; i < mData.size(); ++i)
		{
			if (mData(i) > rhs.mData(i))
			{ // this point is worse in at least one dimension
				check = false;
				break;
			}
			if (!check && mData(i) < rhs.mData(i))
			{ // this point is better in at least one dimension
				check = true;
			}
		}
	}
	catch (std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, while checking if data_point dominates another,\n"
								 << "received the following error: " << e.what() << "\n"
								 << "(bso/utilities/data_point.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return check;
} // dominates()

bool data_point::isDominatedBy(const data_point& rhs) const
{
	bool check;
	try
	{
		this->mCheckValidDimensions(rhs);
		check = rhs.dominates(*this);
	}
	catch (std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, while checking if data_point is dominates by another,\n"
								 << "received the following error: " << e.what() << "\n"
								 << "(bso/utilities/data_point.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return check;
} // isDominatedBy()
 
} // namespace utilities
} // namespace bso

#endif // BSO_DATA_POINT_CPP
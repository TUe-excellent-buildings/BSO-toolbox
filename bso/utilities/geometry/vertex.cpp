#ifndef VERTEX_CPP
#define VERTEX_CPP

#include <cmath>
#include <sstream>
#include <stdexcept>

namespace bso { namespace utilities { namespace geometry {
	
	vertex::vertex() : Eigen::Vector3d()
	{
		this->setZero();
	}
	
	vertex::vertex(const std::initializer_list<double>&& l)
	{
		if (l.size() != 3)
		{
			std::stringstream errorMessage;
			errorMessage << "Error when initializing vertex.\n"
									 << "Expected three values <double>, got: " << l.size()
									 << "\n(bso/utilities/geometry/vertex.cpp)";
			
			throw std::invalid_argument(errorMessage.str());
		}
		for (unsigned int i = 0; i < 3; i++) (*this)[i] = *(l.begin() + i);
	}
	
	template<class T>
	vertex::vertex(const Eigen::MatrixBase<T>& rhs) : Eigen::Vector3d(rhs)
	{
		
	}
	
	auto vertex::begin() 
	{
		return data();
	} // iterato begin()
	
	auto vertex::end()
	{
		return data() + size();
	} // iterator end()
	
	auto vertex::begin() const
	{
		return data();
	} // const iterator begin()
	
	auto vertex::end() const
	{
		return data() + size();
	} // const iterator end()
	
	void vertex::round(const int& dec /*= 0*/)
	{
		double mult = std::pow(10,dec);
		for (auto& i : *this) i = std::round(mult * i) / mult;
	}
	
	bool vertex::isSameAs(const vertex& rhs, const double& tol /*= 1e-9*/) const
	{
		return ((*this)-rhs).isZero(tol);
	}
	
	bool vertex::operator <  (const vertex& rhs) const
	{
		for (unsigned int i = 0; i < 3; i++)
		{
			if ((*this)[i] < rhs[i]) return true;
		}
		return false;
	}
	
	bool vertex::operator <= (const vertex& rhs) const
	{
		if ((*this) <  rhs) return true;
		if ((*this) == rhs) return true;
		return false;
	}
	
	bool vertex::operator >  (const vertex& rhs) const
	{
		return !((*this) <= rhs); 
	}
	
	bool vertex::operator >= (const vertex& rhs) const
	{
		return !((*this) < rhs); 
	}
	
	template<class T>
	vertex& vertex::operator = (const Eigen::MatrixBase<T>& rhs)
	{
		this->Eigen::Vector3d::operator = (rhs);
		return (*this);
	}
	
	std::ostream& operator << (std::ostream& stream, const vertex& v)
	{
		Eigen::IOFormat layout(Eigen::StreamPrecision, Eigen::DontAlignCols, ",", ",", "{", "}", "", "");
		stream << v.transpose().format(layout);
		return stream;
	}
	
} // namespace geometry
} // namespace utilities
} // namespace bso

#endif // VERTEX_CPP
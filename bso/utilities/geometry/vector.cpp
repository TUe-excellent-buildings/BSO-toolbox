#ifndef VECTOR_CPP
#define VECTOR_CPP

#include <cmath>
#include <sstream>
#include <stdexcept>

namespace bso { namespace utilities { namespace geometry {
	
	vector::vector(const std::pair<double,double>& heading)
	{	// initialize from a heading (azimuth,inclination)
		this->setHeading(heading);
	} // ctor from heading
	
	bool vector::isPerpendicular(const vector& rhs, const double& tol /*= 1e-3*/) const
	{ // checks if the vector is perpendicular to rhs
		if (this->isZero(tol) || rhs.isZero(tol)) return false;
		return abs(this->normalized().dot(rhs.normalized())) < tol;
	} // isPerpendicular()
	
	bool vector::isParallel(const vector& rhs, const double& tol /*= 1e-3*/) const
	{	// checks if the vector is parallel to rhs
		if (this->isZero(tol) || rhs.isZero(tol)) return false;
		return (this->normalized().cross(rhs.normalized())).isZero(tol);
	} // isParallel()
	
	bool vector::isCodirectional(const vector& rhs, const double& tol /*= 1e-3*/) const
	{ // checks if the vector is codirectional with rhs
		if (!this->isParallel(rhs,tol)) return false;
		return (this->normalized() - rhs.normalized()).isZero(tol);
	} // isCodirectional()
	
	bool vector::isVertical(const double& tol /*= 1e-3*/) const
	{ // checks if the vector is vertical
		return (this->isCodirectional({0,0,1}, tol) ||
						this->isCodirectional({0,0,-1},tol));
	} // isVertical()
	
	bool vector::isHorizontal(const double& tol /*= 1e-3*/) const
	{ // checks if this vector is horizontal
		return abs(this->normalized()[2]) < tol;
	} // isHorizontal()
	
	std::pair<double,double> vector::getHeading(const int& dec /*= 3*/) const
	{ // returns a pair that describes the heading of the vector (azimuth, inclination)
		// M_PI is defined in cmath.h
		double azimuth = std::atan2((*this)[0], (*this)[1]) * (180/M_PI);
		if (azimuth < 0) azimuth = 360 + azimuth;
		double inclination = std::asin((*this)[2]/this->norm()) * (180/M_PI);
		
		double mult = std::pow(10,dec);
		azimuth = std::round(azimuth * mult) / mult;
		inclination = std::round(inclination * mult) / mult;
		
		return std::make_pair(azimuth, inclination);
		
	} // getHeading()
	
	void vector::setHeading(const std::pair<double,double>& heading)
	{ // set this vector to the passed heading
		(*this) = Eigen::Vector3d::Ones();
		(*this)[0] *= std::sin(heading.first*M_PI/180); // M_PI defined in cmath.h
		(*this)[1] *= std::cos(heading.first*M_PI/180);
		(*this)[2] *= std::sin(heading.second*M_PI/180);
		(*this)[0] *= std::cos(heading.second*M_PI/180);
		(*this)[1] *= std::cos(heading.second*M_PI/180);
		this->normalize();
	} // setHeading()
	
	template<class T>
	vector& vector::operator = (const Eigen::MatrixBase<T>& rhs)
	{
		this->Eigen::Vector3d::operator=(rhs);
		return(*this);
	}
	
	template<class T>
	vector vector::cross(const Eigen::MatrixBase<T>& rhs) const
	{
		return this->Eigen::Vector3d::cross(rhs);
	}
	
} // namespace geometry
} // namespace utilities
} // namespace bso

#endif // VECTOR_CPP
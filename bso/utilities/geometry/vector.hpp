#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <Eigen/Dense>
#include <initializer_list>
#include <bso/utilities/geometry/vertex.hpp>

namespace bso { namespace utilities { namespace geometry {
	
	class vector : public vertex
	{
	private:
		
	public:
		using vertex::vertex; // use all of the constructors in vertex, i.e. empty and list_initializer
		vector(const std::pair<double,double>& heading); // initialize from a heading (azimuth,inclination)
		bool isPerpendicular(const vector& rhs, const double& tol = 1e-3) const; // checks if the vector is perpendicular to rhs
		bool isParallel(const vector& rhs, const double& tol = 1e-3) const; // checks if the vector is parallel to rhs
		bool isCodirectional(const vector& rhs, const double& tol = 1e-3) const; // checks if the vector is codirectional with rhs
		bool isVertical(const double& tol = 1e-3) const; // checks if the vector is vertical
		bool isHorizontal(const double& tol = 1e-3) const; // checks if this vector is horizontal
		std::pair<double,double> getHeading(const int& dec = 3) const; // returns a pair that describes the heading of the vector (azimuth, inclination)
		void setHeading(const std::pair<double,double>& heading); // set this vector to the passed heading 
		
	
		template<class T> vector& operator = (const Eigen::MatrixBase<T>& rhs);
		template<class T> vector cross(const Eigen::MatrixBase<T>& rhs) const;
	};
	
} // namespace geometry
} // namespace utilities
} // namespace bso

#include <bso/utilities/geometry/vector.cpp>

#endif // VECTOR_HPP
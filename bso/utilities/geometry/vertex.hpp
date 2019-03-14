#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <Eigen/Dense>
#include <initializer_list>

namespace bso { namespace utilities { namespace geometry {
	
	class vertex : public Eigen::Vector3d
	{
	private:

	public:
		vertex();
		vertex(const std::initializer_list<double>&& l);
		template<class T> vertex(const Eigen::MatrixBase<T>& rhs);
		
		auto begin();
		auto end();
		auto begin() const;
		auto end() const;
		
		void round(const int& dec = 0);
		
		bool isSameAs(const vertex& rhs, const double& tol = 1e-9) const;
		
		bool operator < (const vertex& rhs) const; // for sorting purposes, order {x,y,z}
		bool operator <= (const vertex& rhs) const;
		bool operator > (const vertex& rhs) const;
		bool operator >= (const vertex& rhs) const;
		
		template<class T>
		vertex& operator = (const Eigen::MatrixBase<T>& rhs);
		
		friend std::ostream& operator << (std::ostream& stream, const vertex& v);
	};

} // namespace geometry	
} // namespace utilities
} // namespace bso

#include <bso/utilities/geometry/vertex.cpp>

#endif // VERTEX_HPP
#ifndef SD_POINT_HPP
#define SD_POINT_HPP

#include <bso/utilities/geometry/vertex.hpp>

#include <bso/structural_design/component/load.hpp>
#include <bso/structural_design/component/constraint.hpp>

#include <initializer_list>

namespace bso { namespace structural_design { namespace component {
	
	class point : public bso::utilities::geometry::vertex
	{
	private:
		unsigned long mID;
		
		std::vector<load> mLoads;
		std::vector<constraint> mConstraints;
	public:
		template<class T>
		point(const unsigned long& ID, const Eigen::MatrixBase<T>& rhs);
		point(const unsigned long& ID, const std::initializer_list<double>&& l);
		~point();
		
		void addLoad(const load& l);
		void addConstraint(const constraint& c);
		
		const unsigned long& getID() const {return mID;}
		
		const std::vector<load>& getLoads() const {return mLoads;}
		const std::vector<constraint>& getConstraints() const {return mConstraints;}
	};
	
} // namespace component
} // namespace structural_design
} // namespace bso

#include <bso/structural_design/component/point.cpp>

#endif // SD_POINT_HPP
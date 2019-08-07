#ifndef SD_QUADRILATERAL_HPP
#define SD_QUADRILATERAL_HPP

#include <bso/structural_design/component/geometry.hpp>
#include <bso/utilities/geometry/quadrilateral.hpp>

namespace bso { namespace structural_design { namespace component {
	
	class quadrilateral : public bso::utilities::geometry::quadrilateral,
												public geometry
	{
	private:

	public:
		template <typename QUADRILATERAL_INITIALIZER>
		quadrilateral(const QUADRILATERAL_INITIALIZER& l);
		quadrilateral(std::initializer_list<bso::utilities::geometry::vertex>&& l);
		~quadrilateral();
		
		void addStructure(const structure& s);
		void mesh(const unsigned int& n, std::vector<point*>& point_store);
	};
	
} // namespace component
} // namespace structural_design
} // namespace bso

#include <bso/structural_design/component/quadrilateral.cpp>

#endif // SD_QUADRILATERAL_HPP
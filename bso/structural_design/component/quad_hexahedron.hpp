#ifndef SD_QUAD_HEXAHEDRON_HPP
#define SD_QUAD_HEXAHEDRON_HPP

#include <bso/structural_design/component/geometry.hpp>
#include <bso/utilities/geometry/quad_hexahedron.hpp>

namespace bso { namespace structural_design { namespace component {
	
	class quad_hexahedron : public bso::utilities::geometry::quad_hexahedron,
															 public geometry
	{
	private:

	public:
		template <typename QUAD_HEXAHEDRON_INITIALIZER>
		quad_hexahedron(const QUAD_HEXAHEDRON_INITIALIZER& l);
		quad_hexahedron(std::initializer_list<bso::utilities::geometry::vertex>&& l);
		~quad_hexahedron();
		
		void addStructure(const structure& s);
		void mesh(const unsigned int& n, std::vector<point*>& point_store);
	};
	
} // namespace component
} // namespace structural_design
} // namespace bso

#include <bso/structural_design/component/quad_hexahedron.cpp>

#endif // SD_QUAD_HEXAHEDRON_HPP
#ifndef SD_DERIVED_PTR_TO_VERTEX_HPP
#define SD_DERIVED_PTR_TO_VERTEX_HPP

namespace bso { namespace structural_design {

	template<typename T>
	std::vector<bso::utilities::geometry::vertex> derived_ptr_to_vertex(T& list);

} // namespace structural_design
} // namespace bso

#include <bso/structural_design/component/derived_ptr_to_vertex.cpp>

#endif // SD_DERIVED_PTR_TO_VERTEX_HPP
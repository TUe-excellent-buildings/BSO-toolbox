#ifndef SD_DERIVED_PTR_TO_VERTEX_CPP
#define SD_DERIVED_PTR_TO_VERTEX_CPP

namespace bso { namespace structural_design {

	template<typename T>
	std::vector<bso::utilities::geometry::vertex> derived_ptr_to_vertex(T& list)
	{
		std::vector<bso::utilities::geometry::vertex> temp;
		for (const auto& i : list) temp.push_back(*i);
		return temp;
	}

} // namespace structural_design
} // namespace bso

#endif // SD_DERIVED_PTR_TO_VERTEX_CPP
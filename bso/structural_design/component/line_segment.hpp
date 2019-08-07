#ifndef SD_LINE_SEGMENT_HPP
#define SD_LINE_SEGMENT_HPP

#include <bso/structural_design/component/geometry.hpp>
#include <bso/utilities/geometry/line_segment.hpp>

namespace bso { namespace structural_design { namespace component {
	
	class line_segment : public bso::utilities::geometry::line_segment,
											 public geometry
	{
	private:

	public:
		template <typename LINE_SEGMENT_INITIALIZER>
		line_segment(const LINE_SEGMENT_INITIALIZER& l);
		line_segment(std::initializer_list<bso::utilities::geometry::vertex>&& l);
		~line_segment();
		
		void addStructure(const structure& s);
		void mesh(const unsigned int& n, std::vector<point*>& point_store);
	};
	
} // namespace component
} // namespace structural_design
} // namespace bso

#include <bso/structural_design/component/line_segment.cpp>

#endif // SD_LINE_SEGMENT_HPP
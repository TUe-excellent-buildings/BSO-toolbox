#ifndef SD_BEAM_ELEMENT_HPP
#define SD_BEAM_ELEMENT_HPP



namespace bso { namespace structural_design { namespace element {
	
	class beam : public bso::utilities::geometry::line_segment,
							 public element
	{
	private:
		double mWidth;
		double mHeight;
		double mA;
		double mPoisson;
		double mIz;
		double mIy;
		double mJ;
		double mG;
		
		Eigen::MatrixXd mT;
		
		void deriveStiffnessMatrix();
	public:
		beam(const unsigned long& ID, const double& E, const double& width, const double& height, const double& poisson,
					std::initializer_list<node*>&& l, const double ERelativeLowerBound = 1e-6);
		~beam();
		
		double getProperty(std::string var) const;
	};
	
} // namespace element
} // namespace structural_design
} // namespace bso

#include <bso/structural_design/element/beam.cpp>

#endif // SD_BEAM_ELEMENT_HPP
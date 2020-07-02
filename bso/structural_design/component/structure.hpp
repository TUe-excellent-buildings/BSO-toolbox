#ifndef SD_STRUCTURE_COMPONENT_HPP
#define SD_STRUCTURE_COMPONENT_HPP

#include <string>
#include <initializer_list>
#include <utility>

namespace bso { namespace structural_design { namespace component {
	
	class structure
	{
	private:
		std::string mType;
		
		double mE; // for all elements
		double mERelativeLowerBound = 1e-6; // for all elements, optional, has a default value
		double mPoisson; // for beams, flat shells and quad_ hexahedrons
		double mA; // for trusses
		double mWidth; // for beams
		double mHeight; // for beams
		double mThickness; // for flat shells
		
		bool mEAssigned = false; // default value
		bool mERelativeLowerBoundAssigned = false; // default value
		bool mPoissonAssigned = false; // default value
		bool mAAssigned = false; // default value
		bool mWidthAssigned = false; // default value
		bool mHeightAssigned = false; // default value
		bool mThicknessAssigned = false; // default value
		
		template <class CONTAINER>
		void initFromContainer(const CONTAINER& l);
		bool checkBadRequest(std::string variable) const;
		
		bool mIsGhostComponent = false;
		bool mIsVisible = true;
	public:
		structure();
		template <class CONTAINER>
		structure(const std::string& type, const CONTAINER& l);
		structure(const std::string& type, const std::initializer_list<std::pair<std::string,double>>& l);
		~structure();
		
		const std::string& type() const;
		const double& E() const;
		const double& ERelativeLowerBound() const;
		const double& poisson() const;
		const double& A() const;
		const double& width() const;
		const double& height() const;
		const double& thickness() const;
		
		void rescaleStructuralVolume(const double& scaleFactor);
		
		const bool& hasERelativeLowerBoundAssigned() const {return mERelativeLowerBoundAssigned;}
		
		bool& isGhostComponent() {return mIsGhostComponent;}
		const bool& isGhostComponent() const {return mIsGhostComponent;}
		bool& isVisible() {return mIsVisible;}
		const bool& isVisible() const {return mIsVisible;}
	};
	
} // namespace component
} // namespace structural_design
} // namespace bso

#include <bso/structural_design/component/structure.cpp>

#endif // SD_STRUCTURE_COMPONENT_HPP
#ifndef SD_GEOMETRY_HPP
#define SD_GEOMETRY_HPP

#include <bso/structural_design/component/structure.hpp>
#include <bso/structural_design/component/load.hpp>
#include <bso/structural_design/component/constraint.hpp>
#include <bso/structural_design/component/point.hpp>

#include <bso/structural_design/element/elements.hpp>
#include <initializer_list>

namespace bso { namespace structural_design { namespace component {
	
	class geometry
	{
	protected:
		std::vector<point*> mMeshedPoints; // original + meshed points
		std::vector<std::vector<point*> > mElementPoints; // meshed points per element
		std::vector<element::element*> mElements; // elements meshed to the FE model
		
		std::vector<structure> 	mStructures;
		std::vector<load> 			mLoads;
		std::vector<constraint> mConstraints;
		
		bool mHasTruss = false;
		bool mHasBeam = false;
		bool mHasFlatShell = false;
		bool mHasQuadHexahedron = false;
		bool mIsLineSegment = false;
		bool mIsQuadrilateral = false;
		bool mIsQuadHexahedron = false;

	public:
		geometry();
		virtual ~geometry();
		
		virtual void addStructure(const structure& s);
		virtual void addLoad(const load& l);
		virtual void addConstraint(const constraint& c);

		virtual void mesh(const unsigned int& n, std::vector<point*>& point_store) = 0;
		virtual void clearMesh();
		
		void rescaleStructuralVolume(const double& scaleFactor);
		
		virtual void addElement(element::element* elePtr) {mElements.push_back(elePtr);}
		
		virtual const bool& hasTruss() const {return mHasTruss;}
		virtual const bool& hasBeam() const {return mHasBeam;}
		virtual const bool& hasFlatShell() const {return mHasFlatShell;}
		virtual const bool& hasQuadHexahedron() const {return mHasQuadHexahedron;}
		virtual const bool& isLineSegment() const {return mIsLineSegment;}
		virtual const bool& isQuadrilateral() const {return mIsQuadrilateral;}
		virtual const bool& isQuadHexahedron() const {return mIsQuadHexahedron;}
		
		const std::vector<point*>& getMeshedPoints() const {return mMeshedPoints;}
		const std::vector<std::vector<point*> >& getElementPoints() const {return mElementPoints;}
		const std::vector<element::element*>& getElements() const {return mElements;}
		const std::vector<structure>& getStructures() const {return mStructures;} 
		const std::vector<load>& getLoads() const {return mLoads;}
		const std::vector<constraint>& getConstraints() const {return mConstraints;}	
	};
	
} // namespace component
} // namespace structural_design
} // namespace bso

#include <bso/structural_design/component/geometry.cpp>

#endif // SD_GEOMETRY_HPP
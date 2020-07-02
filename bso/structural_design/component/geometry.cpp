#ifndef SD_GEOMETRY_CPP
#define SD_GEOMETRY_CPP

namespace bso { namespace structural_design { namespace component {

	geometry::geometry()
	{
		
	} // ctor

	geometry::~geometry()
	{
		
	} // dtor
	
	void geometry::addStructure(const structure& s)
	{
		if 			(s.type() == "truss") mHasTruss = true;
		else if (s.type() == "beam") mHasBeam = true;
		else if (s.type() == "flat_shell") mHasFlatShell = true;
		else if (s.type() == "quad_hexahedron") mHasQuadHexahedron = true;
		
		mStructures.push_back(s);
	} // addStructure()

	void geometry::addLoad(const load& l)
	{
		mLoads.push_back(l);
	} // addLoad()

	void geometry::addConstraint(const constraint& c)
	{
		mConstraints.push_back(c);
	} // addConstraint()

	void geometry::clearMesh()
	{
		mMeshedPoints.clear();
		mElementPoints.clear();
		mElements.clear();
	} // clearMesh()
	
	void geometry::rescaleStructuralVolume(const double& scaleFactor)
	{
		for (auto& i : mStructures)
		{
			i.rescaleStructuralVolume(scaleFactor);
		}
	} // rescaleStructuralVolume()

} // namespace component
} // namespace structural_design
} // namespace bso

#endif // SD_GEOMETRY_CPP
#ifndef SD_BEAM_ELEMENT_CPP
#define SD_BEAM_ELEMENT_CPP

#include <bso/structural_design/component/derived_ptr_to_vertex.hpp>
#include <cmath>

namespace bso { namespace structural_design { namespace element {
	
	template<class CONTAINER>
	void beam::deriveStiffnessMatrix(CONTAINER& l)
	{
		mIz = (mHeight * pow(mWidth, 3)) / 12.0;
		mIy = (mWidth * pow(mHeight, 3)) / 12.0;
		mA = mWidth * mHeight;
		mJ = mIy + mIz;
		mG = mE / (2.0 * (1.0 + mPoisson));
		
		mEFS << 1,1,1,1,1,1; // the element freedom signature of each node of a beam (x,y,z,rx,ry,rz)
		for (const auto& i : mVertices)
		{
			for (auto& j : l)
			{
				j->updateNFS(mEFS);
				if (i.isSameAs(*j)) mNodes.push_back(j);
			}
		}
		
		// create the transformation matrix (this contains the orientations of the beam)
		mT.setZero(12,12);
		bso::utilities::geometry::vector vx, vy, vz;
		vx = this->getVector().normalized(); // the direction of the beam (local x-axis)
		
		if (vx.isVertical())
		{
			vy << 0.0, 1.0, 0.0;
		}
		else
		{
			vy << 0.0, 0.0, 1.0; // temporarily set vy to the unit vector in z-direction
			vy = vy.cross(vx); // vy is perpendicular to any plane defined by vx and the unit vector in z-direction
			vy.normalize();
		}
		
		vz = vx.cross(vy);
		vz.normalize();
		
		Eigen::Matrix3d lambda;
		lambda << vx, vy, vz;

		for (int i = 0; i < 2; i++)
		{ // for each node
			for (int j = 0; j < 2; j++)
			{ // and for both: displacements and rotations
				// add the transformation term lambda
				mT.block<3,3>((2*i+j)*3,(2*i+j)*3) = lambda.transpose();
			}
		}
		
		// initializing this element's stiffness matrix:
		mSM.setZero(12,12);
		double lenght = this->getLength();
		double ael = (mA   * mE) / lenght; // normal strength
		double gjl = (mG   * mJ) / lenght; // shear strength
		double ay  = (12.0 * mE  * mIy) / pow(lenght, 3);
		double az  = (12.0 * mE  * mIz) / pow(lenght ,3);
		double cy  = (6.0  * mE  * mIy) / pow(lenght, 2);
		double cz  = (6.0  * mE  * mIz) / pow(lenght, 2);
		double ey  = (4.0  * mE  * mIy) / lenght;
		double ez  = (4.0  * mE  * mIz) / lenght;
		double fy  = (2.0  * mE  * mIy) / lenght;
		double fz  = (2.0  * mE  * mIz) / lenght;

		mSM(0,0) = ael;   // row 0: F(x,1) : normal force
		mSM(1,1) = az;    // row 1: F(y,1) : shear  force
		mSM(2,2) = ay;    // row 2: F(z,1) : shear  force
		mSM(3,3) = gjl;   // row 3: M(xy,1): torsional moment
		mSM(4,2) = -cy;   // row 4: M(yz,1): bending   moment
		mSM(4,4) = ey;
		mSM(5,1) = cz;    // row 5: M(zx,1): bending   moment
		mSM(5,5) = ez;
		mSM(6,0) = -ael;  // row 6: F(x,2)
		mSM(6,6) = ael;
		mSM(7,1) = -az;   // row 7: F(y,2)
		mSM(7,5) = -cz;
		mSM(7,7) = az;
		mSM(8,2) = -ay;   // row 8: F(z,2)
		mSM(8,4) = cy;
		mSM(8,8) = ay;
		mSM(9,3) = -gjl;  // row 9: M(xy,2)
		mSM(9,9) = gjl;
		mSM(10,2) = -cy;  // row 10:M(yz,2)
		mSM(10,4) = fy;
		mSM(10,8) = cy;
		mSM(10,10) = ey;
		mSM(11,1) = cz;   // row 11:M(zx,2)
		mSM(11,5) = fz;
		mSM(11,7) = -cz;
		mSM(11,11) = ez;
		
		// m_SM is symmetric, so the above terms are mirrored
		Eigen::MatrixXd tempSMCopy(12,12);
		tempSMCopy = mSM.transpose();
		tempSMCopy.diagonal().setZero();
		mSM = tempSMCopy + mSM;

		// transform element stiffness matrix to global coordinate system
		mSM = mT.transpose() * mSM * mT;
		mOriginalSM = mSM;
	}
	
	template<class CONTAINER>
	beam::beam(const unsigned long& ID, const double& E, const double& width, const double& height, const double& poisson,
						 CONTAINER& l, const double ERelativeLowerBound /*= 1e-6*/)
	: bso::utilities::geometry::line_segment(derived_ptr_to_vertex(l)[0], derived_ptr_to_vertex(l)[1]),
		element(ID, E, ERelativeLowerBound)
	{ // 
		mIsBeam = true;
		mWidth = width;
		mHeight = height;
		mPoisson = poisson;

		this->deriveStiffnessMatrix(l);
	} // ctor
	
	beam::beam(const unsigned long& ID, const double& E, const double& width, const double& height, const double& poisson,
						 std::initializer_list<node*>&& l, const double ERelativeLowerBound /*= 1e-6*/)
	: bso::utilities::geometry::line_segment(derived_ptr_to_vertex(l)[0], derived_ptr_to_vertex(l)[1]),
		element(ID, E, ERelativeLowerBound)
	{ // 
		mIsBeam = true;
		mWidth = width;
		mHeight = height;
		mPoisson = poisson;

		this->deriveStiffnessMatrix(l);
	} // ctor
	
	beam::~beam()
	{ // 
		
	} // dtor
	
	double beam::getProperty(std::string var) const
	{ //
		if (var == "width") return mWidth;
		else if (var == "height") return mHeight;
		else if (var == "E") return mE;
		else if (var == "Emin") return mEmin;
		else if (var == "E0") return mE0;
		else if (var == "v") return mPoisson;
		else if (var == "Iz") return mIz;
		else if (var == "Iy") return mIy;
		else if (var == "J") return mJ;
		else if (var == "G") return mG;
		else
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, could not retrieve variable: " << var
									 << "\nfrom beam element.\n"
									 << "(bso/structural_design/element/beam.hpp" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // getProperty()
	
	double beam::getVolume() const
	{
		return bso::utilities::geometry::line_segment::getLength() * mWidth * mHeight;
	} // getVolume()
	
	bso::utilities::geometry::vertex beam::getCenter() const
	{
		return bso::utilities::geometry::line_segment::getCenter();
	} // getCenter()
	
} // namespace element
} // namespace structural_design
} // namespace bso

#endif // SD_BEAM_ELEMENT_CPP
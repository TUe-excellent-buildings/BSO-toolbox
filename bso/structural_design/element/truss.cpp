#ifndef SD_TRUSS_ELEMENT_CPP
#define SD_TRUSS_ELEMENT_CPP

#include <bso/structural_design/component/derived_ptr_to_vertex.hpp>
#include <cmath>

namespace bso { namespace structural_design { namespace element {
	
	template<class CONTAINER>
	void truss::deriveStiffnessMatrix(CONTAINER& l)
	{
		mEFS << 1,1,1,0,0,0; // the element freedom signature of each node of a truss (x,y,z,rx,ry,rz)
		for (const auto& i : mVertices)
		{
			for (auto& j : l)
			{
				j->updateNFS(mEFS);
				if (i.isSameAs(*j)) mNodes.push_back(j);
			}
		}

		// initialising this elements stiffness matrix:
		mSM.setZero(6,6);

		// generate element stiffness matrix
		bso::utilities::geometry::vector c = this->getVector().normalized();

		// the geometric terms in the stiffness matrix ()
		mSM(0,0) =  pow(c(0),2);
		mSM(0,1) =  c(0)*c(1);
		mSM(0,2) =  c(0)*c(2);
		mSM(0,3) = -pow(c(0),2);
		mSM(0,4) = -c(0)*c(1);
		mSM(0,5) = -c(0)*c(2);

		mSM(1,1) =  pow(c(1),2);
		mSM(1,2) =  c(1)*c(2);
		mSM(1,3) = -c(0)*c(1);
		mSM(1,4) = -pow(c(1),2);
		mSM(1,5) = -c(1)*c(2);

		mSM(2,2) =  pow(c(2),2);
		mSM(2,3) = -c(0)*c(2);
		mSM(2,4) = -c(1)*c(2);
		mSM(2,5) = -pow(c(2),2);

		mSM(3,3) =  pow(c(0),2);
		mSM(3,4) =  c(0)*c(1);
		mSM(3,5) =  c(0)*c(2);

		mSM(4,4) =  pow(c(1),2);
		mSM(4,5) =  c(1)*c(2);

		mSM(5,5) =  pow(c(2),2);

		mSM *= ((mA*mE) / this->getLength()); // relate the geometric terms to the stiffness of this element

		// m_SM is symmetric, this algorithm mirrors the above entries along the matrix diagonal
		for (unsigned int i = 0; i < 6; ++i)
		{
			for (unsigned int j = i + 1; j < 6; ++j)
			{
				mSM(j,i) = mSM(i,j);
			}
		}
		mOriginalSM = mSM;
	}
	
	template<class CONTAINER>
	truss::truss(const unsigned long& ID, const double& E, const double& A,
							 CONTAINER& l, const double ERelativeLowerBound /*= 1e-6*/)
	: bso::utilities::geometry::line_segment(derived_ptr_to_vertex(l)[0], derived_ptr_to_vertex(l)[1]),
		element(ID, E, ERelativeLowerBound)
	{ // 
		mA = A;
		mIsTruss = true;
		
		this->deriveStiffnessMatrix(l);
	} // ctor
	
	truss::truss(const unsigned long& ID, const double& E, const double& A,
							 std::initializer_list<node*>&& l, const double ERelativeLowerBound /*= 1e-6*/)
	: bso::utilities::geometry::line_segment(derived_ptr_to_vertex(l)[0], derived_ptr_to_vertex(l)[1]),
		element(ID, E, ERelativeLowerBound)
	{ // 
		mA = A;
		mIsTruss = true;
		
		this->deriveStiffnessMatrix(l);
	} // ctor
	
	truss::~truss()
	{ // 

	} // dtor
	
	double truss::getProperty(std::string var) const
	{ //
		if (var == "A") return mA;
		else if (var == "E") return mE;
		else if (var == "Emin") return mEmin;
		else if (var == "E0") return mE0;
		else
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, could not retrieve variable: " << var
									 << "\nfrom truss element.\n"
									 << "(bso/structural_design/element/truss.hpp" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // getProperty()
	
	double truss::getVolume() const
	{
		return bso::utilities::geometry::line_segment::getLength() * mA;
	} // getVolume()
	
	bso::utilities::geometry::vertex truss::getCenter() const
	{
		return bso::utilities::geometry::line_segment::getCenter();
	} // getCenter()
	
} // namespace element
} // namespace structural_design
} // namespace bso

#endif // SD_TRUSS_ELEMENT_CPP
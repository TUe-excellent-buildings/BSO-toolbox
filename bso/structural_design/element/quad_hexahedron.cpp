#ifndef SD_QUAD_HEXAHEDRON_ELEMENT_CPP
#define SD_QUAD_HEXAHEDRON_ELEMENT_CPP

#include <bso/structural_design/component/derived_ptr_to_vertex.hpp>
#include <cmath>

namespace bso { namespace structural_design { namespace element {
	
	template<class CONTAINER>
	void quad_hexahedron::deriveStiffnessMatrix(CONTAINER& l)
	{
		mEFS << 1,1,1,0,0,0; // the element freedom signature of each node of a flat shell (x,y,z,rx,ry,rz)
		// store the nodes in the order of mVertices
		for (const auto& i : mVertices)
		{
			for (auto& j : l)
			{
				j->updateNFS(mEFS);
				if (i.isSameAs(*j)) mNodes.push_back(j);
			}
		}
		
		// create the transformation matrix (this contains the orientations of the flat shell)
		bso::utilities::geometry::vector vx, vy, vz;
		vx = (mVertices[1] + mVertices[2] + mVertices[5] + mVertices[6])/4 - this->getCenter();
		vx.normalize();
		// create a temporary vector vy (this may not be orthogonal to x)
		vy = (mVertices[2] + mVertices[3] + mVertices[6] + mVertices[7])/4 - this->getCenter();
		vz = vx.cross(vy).normalized();
		vy = vz.cross(vx).normalized(); // make vy orthogonal to vx

		mT.setZero(24,24);
		Eigen::Matrix3d lambda;
		lambda << vx, vy, vz;

		for (int i = 0; i < 8; i++)
		{ // for each node
			// add the transformation term lambda
			mT.block<3,3>((i)*3,(i)*3) = lambda.transpose();
		}
		
		Eigen::MatrixXd locCoords;
		locCoords.setZero(8,3);
		
		for (unsigned int i = 0; i < 8; ++i)
		{
			locCoords.row(i) = lambda.transpose() * mVertices[i];
		}
		
		Eigen::MatrixXd ETerm;
		ETerm.setZero(6,6);

		ETerm(0,0) = mPoisson - 1; 	 ETerm(0,1) = -mPoisson; 			ETerm(0,2) = -mPoisson; // first 3 elements of the first row
		ETerm(1,0) = -mPoisson;    	 ETerm(1,1) = mPoisson - 1; 	ETerm(1,2) = -mPoisson; // first 3 elements of the second row
		ETerm(2,0) = -mPoisson;    	 ETerm(2,1) = -mPoisson;    	ETerm(2,2) = mPoisson - 1; // first 3 elements of the third row
		ETerm(3,3) = -(1-2*mPoisson)/2.0; ETerm(4,4) = ETerm(3,3); ETerm(5,5) = ETerm(3,3); // elements of the diagonal on the last 3 rows

		ETerm = ETerm * (mE / (2 * pow(mPoisson,2) + mPoisson - 1));

		// save elasticity matrix (for a solid element, for stress_based topology optimization)
		mETermSolid.setZero(6,6);
		mETermSolid = ETerm * (mE0 / mE);

		// initialise the element stiffness matrices and start numerical integration of the contribution of every node to the element's stiffness
		mSM.setZero(24,24);
		double ksi, eta, zeta;
		double wKsi, wEta, wZeta;
		for (int l = 0; l < 2; ++l)
		{
			for (int m = 0; m < 2; ++m)
			{
				for (int n = 0; n < 2; ++n)
				{
					// determine values for eta and ksi for numerical integration
					if (l == 0)
					{ // first integration point in ksi-direction
						ksi = -1 * sqrt(1.0 / 3.0);
						wKsi = 1.0;
					}
					else
					{ // second integration point in ksi-direction
						ksi = 1 * sqrt(1.0 / 3.0);
						wKsi = 1.0;
					}
					if (m==0)
					{ // first integration point in eta-direction
						eta = -1 * sqrt(1.0 / 3.0);
						wEta = 1.0;
					}
					else
					{ // second integration point in eta-direction
						eta = 1 * sqrt(1.0 / 3.0);
						wEta = 1.0;
					}
					if (n==0)
					{ // first integration point in zeta-direction
						zeta = -1 * sqrt(1.0 / 3.0);
						wZeta = 1.0;
					}
					else
					{ // second integration point in zeta-direction
						zeta = 1 * sqrt(1.0 / 3.0);
						wZeta = 1.0;
					}

					// compute the derivatives of the displacements with respect to the natural coordinates (ksi, eta and zeta)
					Eigen::MatrixXd dN;
					dN.setZero(3,8);

					dN(0,0) = (-1.0/8.0)*(1-eta)*(1-zeta);	dN(1,0) = (-1.0/8.0)*(1-ksi)*(1-zeta);	dN(2,0) = (-1.0/8.0)*(1-ksi)*(1-eta);
					dN(0,1) = ( 1.0/8.0)*(1-eta)*(1-zeta);	dN(1,1) = (-1.0/8.0)*(1+ksi)*(1-zeta);	dN(2,1) = (-1.0/8.0)*(1+ksi)*(1-eta);
					dN(0,2) = ( 1.0/8.0)*(1+eta)*(1-zeta);	dN(1,2) = ( 1.0/8.0)*(1+ksi)*(1-zeta);	dN(2,2) = (-1.0/8.0)*(1+ksi)*(1+eta);
					dN(0,3) = (-1.0/8.0)*(1+eta)*(1-zeta);	dN(1,3) = ( 1.0/8.0)*(1-ksi)*(1-zeta);	dN(2,3) = (-1.0/8.0)*(1-ksi)*(1+eta);
					dN(0,4) = (-1.0/8.0)*(1-eta)*(1+zeta);	dN(1,4) = (-1.0/8.0)*(1-ksi)*(1+zeta);	dN(2,4) = ( 1.0/8.0)*(1-ksi)*(1-eta);
					dN(0,5) = ( 1.0/8.0)*(1-eta)*(1+zeta);	dN(1,5) = (-1.0/8.0)*(1+ksi)*(1+zeta);	dN(2,5) = ( 1.0/8.0)*(1+ksi)*(1-eta);
					dN(0,6) = ( 1.0/8.0)*(1+eta)*(1+zeta);	dN(1,6) = ( 1.0/8.0)*(1+ksi)*(1+zeta);	dN(2,6) = ( 1.0/8.0)*(1+ksi)*(1+eta);
					dN(0,7) = (-1.0/8.0)*(1+eta)*(1+zeta);	dN(1,7) = ( 1.0/8.0)*(1-ksi)*(1+zeta);	dN(2,7) = ( 1.0/8.0)*(1-ksi)*(1+eta);

					// compute the matrix of Jacobi to map between derivatives of the element shape with respect to natural and local coordinates (ksi, eta, zeta versus x_loc, y_loc, z_loc)
					Eigen::MatrixXd J, JInverse;
					J = dN * locCoords; // 3 by 3 matrix, matrix of Jacobi
					JInverse = J.inverse(); // also 3 by 3 matrix, the inverse of the matrix of Jacobi

					// compute the constitutive relation between strain and nodal displacements in the natural coordinate system (ksi, eta, zeta)
					Eigen::MatrixXd A;
					A.setZero(6,9);

					A(0,0) = JInverse(0,0); A(0,1) = JInverse(0,1); A(0,2) = JInverse(0,2); // du/dx --> epsilon[x]
					A(1,3) = JInverse(1,0); A(1,4) = JInverse(1,1); A(1,5) = JInverse(1,2); // dv/dy --> epsilon[y]
					A(2,6) = JInverse(2,0); A(2,7) = JInverse(2,1); A(2,8) = JInverse(2,2); // dw/dz --> epsilon[z]

					A(3,0) = JInverse(1,0); A(3,1) = JInverse(1,1); A(3,2) = JInverse(1,2); // du/dy +
					A(3,3) = JInverse(0,0); A(3,4) = JInverse(0,1); A(3,5) = JInverse(0,2); // dv/dx --> gamma[xy]

					A(4,3) = JInverse(2,0); A(4,4) = JInverse(2,1); A(4,5) = JInverse(2,2); // dv/dz +
					A(4,6) = JInverse(1,0); A(4,7) = JInverse(1,1); A(4,8) = JInverse(1,2); // dw/dy --> gamma[yz]

					A(5,6) = JInverse(0,0); A(5,7) = JInverse(0,1); A(5,8) = JInverse(0,2); // dw/dx +
					A(5,0) = JInverse(2,0); A(5,1) = JInverse(2,1); A(5,2) = JInverse(2,2); // du/dz --> gamma[zx]

					// compute the relation between displacements in the local coordinate system (x_loc, y_loc, z_loc) and the natural coordinate system (ksi, eta, zeta)
					Eigen::MatrixXd G;
					G.setZero(9,24);

					for (unsigned int i = 0; i < 8; i++)
					{ // for each node
							for (unsigned int j = 0; j < 3; j++)
							{ // for each dof in that node
									for (unsigned int k = 0; k < 3; k++)
									{ // for each contribution of the dof 'j' of node 'i' to the dof 'k' of the current integration integration point
											G(3*k+j,3*i+k) = dN(j,i);
									}
							}
					}

					// compute the derivatives of the displacement with respect to the local coordinates (x_loc, y_loc, z_loc) i.e. the strains in the element
					Eigen::MatrixXd B;
					B = A*G; // 6 by 24 matrix

					// save sum of strain-displacement matrices of each integration points
					if (l == 0 && m == 0 && n == 0)
					{
						mBSum.setZero(6,24);
					}
					mBSum += B;

					mSM += wKsi*wEta*wZeta*B.transpose()*ETerm*B*J.determinant(); // sum for all integration points (Gauss Quadrature)

				} // end for n (zeta)
			} // end for m (eta)
		} // end for l (ksi)

		// transform the element stiffness matrix from local to global coordinate system
		mSM = mT.transpose() * mSM * mT;
		//if (mSM(0,0) < 0) mSM *= -1;
		mOriginalSM = mSM;
		
	}
	
	template<class CONTAINER>
	quad_hexahedron::quad_hexahedron(const unsigned long& ID, const double& E, const double& poisson,
																	 CONTAINER& l, const double ERelativeLowerBound /*= 1e-6*/,
																	 const double geomTol /* = 1e-3*/)
	: bso::utilities::geometry::quad_hexahedron(derived_ptr_to_vertex(l), geomTol),
		element(ID, E, ERelativeLowerBound)
	{ // 
		
		mIsQuadHexahedron = true;
		mPoisson = poisson;

		this->deriveStiffnessMatrix(l);
	} // ctor
	
	quad_hexahedron::quad_hexahedron(const unsigned long& ID, const double& E, const double& poisson,
																	 std::initializer_list<node*>&& l, const double ERelativeLowerBound /*= 1e-6*/,
																	 const double geomTol /* = 1e-3*/)
	: bso::utilities::geometry::quad_hexahedron(derived_ptr_to_vertex(l), geomTol),
		element(ID, E, ERelativeLowerBound)
	{ // 
		
		mIsQuadHexahedron = true;
		mPoisson = poisson;

		this->deriveStiffnessMatrix(l);
	} // ctor
	
	quad_hexahedron::~quad_hexahedron()
	{ // 
		
	} // dtor

	void quad_hexahedron::computeResponse(load_case lc)
	{ //
		element::computeResponse(lc);
		// calculate stress of solid element at centroid
		mBAv = (1.0/8) * mBSum; // average B-matrix
		mDispLoc.setZero(24);
		mDispLoc = mT * mDisplacements[lc];
		Eigen::VectorXd StrainAv;
		StrainAv.setZero(6);
		StrainAv = mBAv * mDispLoc; // average strain
		mStress = mETermSolid * StrainAv; // average stress per element (averaged over 2x2x2 integration points)
	} // computeResponse()

	double quad_hexahedron::getProperty(std::string var) const
	{ //
		if (var == "v") return mPoisson;
		else if (var == "E") return mE;
		else if (var == "Emin") return mEmin;
		else if (var == "E0") return mE0;
		else
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, could not retrieve variable: " << var
									 << "\nfrom quadrilateral hexahedron element.\n"
									 << "(bso/structural_design/element/quad_hexahedron.hpp" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // getProperty()
	
	double quad_hexahedron::getVolume() const
	{
		return bso::utilities::geometry::quad_hexahedron::getVolume();
	} // getVolume()
	
	bso::utilities::geometry::vertex quad_hexahedron::getCenter() const
	{
		return bso::utilities::geometry::quad_hexahedron::getCenter();
	} // getCenter()
	
	double quad_hexahedron::getStressAtCenter(const double& alpha /* 0*/, const double& beta /* 1.0 / sqrt(3)*/) const
	{
		Eigen::MatrixXd V;
		V.setZero(6,6);
		V(0,0) = 1.0;	V(0,1) = -0.5;	V(0,2) = -0.5;
		V(1,0) = -0.5;	V(1,1) = 1.0;	V(1,2) = -0.5;
		V(2,0) = -0.5;	V(2,1) = -0.5;	V(2,2) = 1.0;
		V(3,3) = 3.0;	V(4,4) = 3.0;	V(5,5) = 3.0;
		double I1, J2D, DPStress;
		I1 = mStress(0) + mStress(1) + mStress(2);
		J2D = (1.0/3) * mStress.transpose() * V * mStress;
		DPStress = (1.0 / beta) * (sqrt(J2D) + alpha * I1); // this value should not exceed 1
		return DPStress;
	} // getStressCenter() - NOTE: if alpha & beta are not inserted in the function call, the Von Mises stress is obtained

	Eigen::VectorXd quad_hexahedron::getStressSensitivityTermAE(const unsigned long freeDOFs, const double& alpha /* 0*/) const
	// Sensitivity calculation is based on the theory in:
	// Luo, Y., & Kang, Z. (2012). Topology optimization of continuum structures with Drucker-Prager yield stress constraints. Computers & Structures, 90-91, pp. 65-75. https://doi.org/10.1016/j.compstruc.2011.10.008
	{
		Eigen::Vector6d w;
		w << 1, 1, 1, 0, 0, 0;
		Eigen::VectorXd W0;
		W0.setZero(24);
		W0 = mBAv.transpose() * mETermSolid.transpose() * w;
		Eigen::MatrixXd V;
		V.setZero(6,6);
		V(0,0) = 1.0;	V(0,1) = -0.5;	V(0,2) = -0.5;
		V(1,0) = -0.5;	V(1,1) = 1.0;	V(1,2) = -0.5;
		V(2,0) = -0.5;	V(2,1) = -0.5;	V(2,2) = 1.0;
		V(3,3) = 3.0;	V(4,4) = 3.0;	V(5,5) = 3.0;
		Eigen::MatrixXd M0;
		M0.setZero(24,24);
		M0 = mBAv.transpose() * mETermSolid.transpose() * V * mETermSolid * mBAv;

		Eigen::VectorXd aeloc, aeglob;
		aeloc.setZero(24); aeglob.setZero(24);
		aeloc = (M0.transpose() * mDispLoc) / sqrt(3.0 * mDispLoc.transpose() * M0 * mDispLoc) + alpha * W0;
		aeglob = mT.transpose() * aeloc;

		Eigen::VectorXd ae;
		ae.setZero(freeDOFs);
		int counterAE = -1;
		for(auto& i : mNodes) // for all nodes of this element
		{
			for (unsigned int j = 0; j < 3; ++j) // for all local DOF's
			{
				++counterAE;
				if (i->getNFS(j) == 0 || i->getConstraint(j) == 1) continue;
				unsigned int GDOF = i->getGlobalDOF(j);
				ae(GDOF) = aeglob(counterAE);
			}
		}
		return ae;
	} // getStressSensitivityTermAE()

	Eigen::VectorXd quad_hexahedron::getStressSensitivity(Eigen::MatrixXd& Lamda, const double& penal /* 1*/, const double& beta /* 1.0 / sqrt(3)*/) const
	// Sensitivity calculation is based on the theory in:
	// Luo, Y., & Kang, Z. (2012). Topology optimization of continuum structures with Drucker-Prager yield stress constraints. Computers & Structures, 90-91, pp. 65-75. https://doi.org/10.1016/j.compstruc.2011.10.008
	{
		Eigen::VectorXd dKdxU = (-penal / beta) * pow(mDensity,penal - 1) * (mE0 / mE) * mSM * mDispLoc;
		Eigen::MatrixXd lamdaloc;
		lamdaloc.setZero(24,Lamda.cols());
		Eigen::VectorXd dsx(Lamda.cols()); // dsx = vector with sensitivities for varying constraints, but to same x
		dsx.setZero();
		int counterLamda = -1;
		for (auto& i : mNodes) // for all nodes of this element
		{
			for (unsigned int j = 0; j < 3; ++j) // for all local DOF's
			{
				++counterLamda;
				if (i->getNFS(j) == 0 || i->getConstraint(j) == 1) continue;
				unsigned int GDOF = i->getGlobalDOF(j);
				lamdaloc.row(counterLamda) = Lamda.row(GDOF);
			}
		}
		dsx = lamdaloc.transpose() * dKdxU;
		return dsx;
	} // getStressSensitivity()

} // namespace element
} // namespace structural_design
} // namespace bso

#endif // SD_QUAD_HEXAHEDRON_ELEMENT_CPP
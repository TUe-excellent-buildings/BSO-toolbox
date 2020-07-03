#ifndef SD_FLAT_SHELL_ELEMENT_CPP
#define SD_FLAT_SHELL_ELEMENT_CPP

#include <bso/structural_design/component/derived_ptr_to_vertex.hpp>
#include <cmath>

namespace bso { namespace structural_design { namespace element {
	
	template<class CONTAINER>
	void flat_shell::deriveStiffnessMatrix(CONTAINER& l)
	{
		mEFS << 1,1,1,1,1,1; // the element freedom signature of each node of a flat shell (x,y,z,rx,ry,rz)
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
		vx = (((mVertices[1] + mVertices[2]) / 2.0) - this->getCenter()); // vector from center to center of a line, this will be the local x-axis
		vx.normalize();
		vz = this->getNormal(); // normal of the the plane of the quadrilateral, this will be the local z-axis
		vz.normalize();
		vy = vz.cross(vx).normalized(); // normal to both vx and vz, this will be the local y-axis

		mT.setZero(24,24);
		Eigen::Matrix3d lambda;
		lambda << vx, vy, vz;

		for (int i = 0; i < 4; i++)
		{ // for each node
			for (int j = 0; j < 2; j++)
			{ // and for both: displacements and rotations
				// add the transformation term lambda
				mT.block<3,3>((2*i+j)*3,(2*i+j)*3) = lambda.transpose();
			}
		}
		
		Eigen::MatrixXd locCoords;
		locCoords.setZero(4,3);
		
		for (unsigned int i = 0; i < 4; ++i)
		{
			locCoords.row(i) = lambda.transpose() * mVertices[i];
		}

		// initialise the element stiffness matrices and start numerical integration of the contribution of every node to the element's stiffness
		Eigen::MatrixXd kShear, kNormal, kBending;
		kShear.setZero(8,8);
		kNormal.setZero(8,8);
		kBending.setZero(12,12);
		double ksi, eta;
		double wKsi, wEta;
		for (int l=0;l<2;l++)
		{
			for (int m=0;m<2;m++)
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
				{ // secpmd integration point in eta-direction
					eta = 1 * sqrt(1.0 / 3.0);
					wEta = 1.0;
				}

				// Finding matrix J following Kaushalkumar Kansara
				Eigen::MatrixXd J;
				J.setZero(2,2);
				J(0,0) = (-0.25+0.25*eta)*locCoords(0,0) + ( 0.25-0.25*eta)*locCoords(1,0) + (0.25+0.25*eta)*locCoords(2,0) + (-0.25-0.25*eta)*locCoords(3,0);
				J(0,1) = (-0.25+0.25*eta)*locCoords(0,1) + ( 0.25-0.25*eta)*locCoords(1,1) + (0.25+0.25*eta)*locCoords(2,1) + (-0.25-0.25*eta)*locCoords(3,1);
				J(1,0) = (-0.25+0.25*ksi)*locCoords(0,0) + (-0.25-0.25*ksi)*locCoords(1,0) + (0.25+0.25*ksi)*locCoords(2,0) + ( 0.25-0.25*ksi)*locCoords(3,0);
				J(1,1) = (-0.25+0.25*ksi)*locCoords(0,1) + (-0.25-0.25*ksi)*locCoords(1,1) + (0.25+0.25*ksi)*locCoords(2,1) + ( 0.25-0.25*ksi)*locCoords(3,1);
				Eigen::MatrixXd JInverse = J.inverse();

				// Performing integration of the in-plane behaviour
				// Finding matrix A following Kaushalkumar Kansara
				Eigen::MatrixXd A;
				A.setZero(3,4);
				A(0,0) = J(1,1);	A(0,1) = -J(0,1);	A(0,2) = 0;				A(0,3) = 0;
				A(1,0) = 0;				A(1,1) = 0;				A(1,2) = -J(1,0);	A(1,3) = J(0,0);
				A(2,0) = -J(1,0);	A(2,1) = J(0,0);	A(2,2) = J(1,1);	A(2,3) = -J(0,1);
				A = A * (1/J.determinant());

				// Finding matrix G following Kaushalkumar Kansara
				Eigen::MatrixXd G;
				G.setZero(4,8);
				G(0,0)=(-0.25+0.25*eta); 	G(2,1)=G(0,0);
				G(0,2)=(0.25-0.25*eta);		G(2,3)=G(0,2);
				G(0,4)=(0.25+0.25*eta);		G(2,5)=G(0,4);
				G(0,6)=(-0.25-0.25*eta);	G(2,7)=G(0,6);
				G(1,0)=(-0.25+0.25*ksi);	G(3,1)=G(1,0);
				G(1,2)=(-0.25-0.25*ksi);	G(3,3)=G(1,2);
				G(1,4)=(0.25+0.25*ksi);		G(3,5)=G(1,4);
				G(1,6)=(0.25-0.25*ksi);		G(3,7)=G(1,6);

				// matrix B for in-plane behaviour
				Eigen::MatrixXd B;
				B.setZero(3,8);
				B = A * G;

				// save strain-displacement matrix for in-plane behaviour per integration point
				if (m == 0 && l == 0)
				{
					mB1.setZero(3,8);
					mB1 = B;
				}
				else if (m == 0 && l == 1)
				{
					mB2.setZero(3,8);
					mB2 = B;
				}
				else if (m == 1 && l == 1)
				{
					mB3.setZero(3,8);
					mB3 = B;
				}
				else
				{
					mB4.setZero(3,8);
					mB4 = B;
				}
	
				// Matrix elasticity term, separated for normal and shear action
				Eigen::MatrixXd ETermNormal, ETermShear;
				ETermNormal.setZero(3,3);
				ETermShear.setZero(3,3);
				ETermNormal(0,0) = 1;    			ETermNormal(0,1) = mPoisson;
				ETermNormal(1,0) = mPoisson;  ETermNormal(1,1) = 1;  
			  ETermShear(2,2)  = (1 - mPoisson) / 2;
				ETermNormal = ETermNormal * (mE / (1 - pow(mPoisson, 2)));
				ETermShear  = ETermShear  * (mE / (1 - pow(mPoisson, 2)));

				kNormal += mThickness * wKsi * wEta * B.transpose() * ETermNormal * B * J.determinant();
				kShear	+= mThickness * wKsi * wEta * B.transpose() * ETermShear  * B * J.determinant();

				// save elasticity matrix (for a solid element) for in-plane behaviour (for stress_based topology optimization)
				mETermSolid.setZero(3,3);
				mETermSolid = ETermNormal * (mE0 / mE) + ETermShear * (mE0 / mE);

				// Performing integration of the out-of-plane behaviour
				// according to Batoz & Tahar: Evaluation of a new quadrilateral thin plate bending element (1982)
				Eigen::MatrixXd N;
				N.setZero(8,2);
				N(0,0) = ( 1.0/4.0)*(2*ksi+eta)*(1-eta);	N(0,1) = ( 1.0/4.0)*((2*eta)+ksi)*(1-ksi);
				N(1,0) = ( 1.0/4.0)*(2*ksi-eta)*(1-eta);	N(1,1) = ( 1.0/4.0)*((2*eta)-ksi)*(1+ksi);
				N(2,0) = ( 1.0/4.0)*(2*ksi+eta)*(1+eta);	N(2,1) = ( 1.0/4.0)*((2*eta)+ksi)*(1+ksi);
				N(3,0) = ( 1.0/4.0)*(2*ksi-eta)*(1+eta);	N(3,1) = ( 1.0/4.0)*((2*eta)-ksi)*(1-ksi);
				N(4,0) = -ksi			 *(1-eta);   						N(4,1) = (-1.0/2.0)*(1-(ksi*ksi));
				N(5,0) = ( 1.0/2.0)*(1-(eta*eta));   			N(5,1) = -eta			 *(1+ksi);
				N(6,0) = -ksi			 *(1+eta);   						N(6,1) = ( 1.0/2.0)*(1-(ksi*ksi));
				N(7,0) = (-1.0/2.0)*(1-(eta*eta));   			N(7,1) = -eta			 *(1-ksi);

				// calculating elasticity term bending behaviour
				Eigen::MatrixXd ETermBending;
				ETermBending.setZero(3,3);
				ETermBending(0,0) = 1;    		ETermBending(0,1) = mPoisson;
				ETermBending(1,0) = mPoisson; ETermBending(1,1) = 1;  
				ETermBending(2,2) = (1-mPoisson)/2;
				ETermBending = ETermBending * ((mE * pow(mThickness,3)) 
																		/ (12 * (1 - pow(mPoisson, 2))));

				Eigen::VectorXd a,b,c,d,e;
				a.setZero(8);	b.setZero(8); c.setZero(8); d.setZero(8); e.setZero(8);
				std::vector<std::pair<int,int> > indices = {{0,1},{1,2},{2,3},{3,0}};

				for (unsigned int i = 0; i < 4; ++i)
				{
					double xij = locCoords(indices[i].first,0) - locCoords(indices[i].second,0);
					double yij = locCoords(indices[i].first,1) - locCoords(indices[i].second,1);
					double lij = pow(xij,2) + pow(yij,2);
					
					a(i+4) = -xij/lij;
					b(i+4) = 0.75*xij*yij/lij;
					c(i+4) = (0.25*pow(xij,2)-0.5*pow(yij,2))/lij;
					d(i+4) = -yij/lij;
					e(i+4) = (-0.5*pow(xij,2)+0.25*pow(yij,2))/lij;
				}

				// values for H derivatives as presented in the paper Batoz, Taher
				Eigen::MatrixXd Hx, Hy;
				Hx.setZero(12,2); Hy.setZero(12,2);
				indices.clear();
				indices = {{4,7},{5,4},{6,5},{7,6}};

				for (unsigned int i = 0; i < 4; ++i)
				{
					Eigen::Vector2d N1, N5, N8;
					N1 = N.row(i);
					N5 = N.row(indices[i].first);
					N8 = N.row(indices[i].second);	
					
					Hx.row(i*3+0) = 1.5*(a(indices[i].first)*N5-a(indices[i].second)*N8);
					Hx.row(i*3+1) = b(indices[i].first)*N5 + b(indices[i].second)*N8;
					Hx.row(i*3+2) = N1 - c(indices[i].first)*N5 - c(indices[i].second)*N8;

					Hy.row(i*3+0) = 1.5*(d(indices[i].first)*N5-d(indices[i].second)*N8);
					Hy.row(i*3+1) = -N1 + e(indices[i].first)*N5 + e(indices[i].second)*N8;
					Hy.row(i*3+2) = -1*(Hx.row(i*3+1));
				}

				// matrix B for bending behaviour
				B.setZero(3,12);
				B.row(0) = Hx * JInverse.row(0).transpose();
				B.row(1) = Hy * JInverse.row(1).transpose();
				B.row(2) = Hy * JInverse.row(0).transpose()
								 + Hx * JInverse.row(1).transpose();

				// stiffness matrix for bending
				kBending += B.transpose() * ETermBending * B * J.determinant();
			} // end for m (ksi/eta)
		} // end for l (ksi/eta)

		// fill the found stiffness terms kXxxx... into the stiffness matrices
		mSMNormal.setZero(24,24); mSMShear.setZero(24,24); mSMBending.setZero(24,24);
		for (int m=0;m<4;m++)
		{
			for (int n=0;n<4;n++)
			{
				mSMNormal.block<2,2>(6*m+0,6*n+0) << kNormal.block<2,2>(2*m,2*n);
				mSMShear.block<2,2>(6*m+0,6*n+0) << kShear.block<2,2>(2*m,2*n);
				mSMBending.block<3,3>(6*m+2,6*n+2) << kBending.block<3,3>(3*m,3*n);
			}
		}

		// compose stiffness matrix out of normal, shear, and bending stiffness matrices
		mSM = mSMBending + mSMNormal + mSMShear;

		// add drilling stiffness to the element
		mSM(5,5)   = mSM.mean(); // add drilling terms to the 6th dof of the local stiffness matrix
		mSM(11,11) = mSM(5,5);
		mSM(17,17) = mSM(5,5);
		mSM(23,23) = mSM(5,5);

		// transform element stiffness matrices to global coordinate system
		mSM = mT.transpose() * mSM * mT;
		mOriginalSM = mSM;

		// also transform the bending and normal action stiffness amtrices
		mSMBending = mT.transpose() * mSMBending * mT;
		mSMNormal  = mT.transpose() * mSMNormal  * mT;
		mSMShear   = mT.transpose() * mSMShear 	 * mT;
	}
	
	template<class CONTAINER>
	flat_shell::flat_shell(const unsigned long& ID, const double& E, const double& thickness, const double& poisson,
												 CONTAINER& l, const double ERelativeLowerBound /*= 1e-6*/, const double geomTol /* = 1e-3*/)
	: bso::utilities::geometry::quadrilateral(derived_ptr_to_vertex(l), geomTol),
		element(ID, E, ERelativeLowerBound)
	{ // 
		
		mIsFlatShell = true;
		mThickness = thickness;
		mPoisson = poisson;

		this->deriveStiffnessMatrix(l);
	} // ctor
	
	flat_shell::flat_shell(const unsigned long& ID, const double& E, const double& thickness, const double& poisson,
												 std::initializer_list<node*>&& l, const double ERelativeLowerBound /*= 1e-6*/, const double geomTol /* = 1e-3*/)
	: bso::utilities::geometry::quadrilateral(derived_ptr_to_vertex(l), geomTol),
		element(ID, E, ERelativeLowerBound)
	{ // 
		
		mIsFlatShell = true;
		mThickness = thickness;
		mPoisson = poisson;

		this->deriveStiffnessMatrix(l);
	} // ctor
	
	flat_shell::~flat_shell()
	{ // 
		
	} // dtor
	
	void flat_shell::computeResponse(load_case lc)
	{
		Eigen::VectorXd elementDisplacements(mSM.rows());
		elementDisplacements.setZero();
		auto dispIte = elementDisplacements.data();
		
		for (const auto& i : mNodes)
		{
			for (unsigned int j = 0; j < 6; ++j)
			{
				if (mEFS(j) == 1)
				{
					*dispIte = i->getDisplacements(lc)(j);
					++dispIte;
				}
			}
		}
		mDisplacements[lc] = elementDisplacements;
		mEnergies[lc] = 0.5 * elementDisplacements.transpose() * mSM * elementDisplacements;
		mTotalEnergy += mEnergies[lc];
		mSeparatedEnergies[lc]["normal"]  = 0.5 * elementDisplacements.transpose() * mSMNormal  * elementDisplacements;
		mAxialEnergy += mSeparatedEnergies[lc]["normal"];
		mSeparatedEnergies[lc]["shear"]   = 0.5 * elementDisplacements.transpose() * mSMShear   * elementDisplacements;
		mShearEnergy += mSeparatedEnergies[lc]["shear"];
		mSeparatedEnergies[lc]["bending"] = 0.5 * elementDisplacements.transpose() * mSMBending * elementDisplacements;
		mBendEnergy += mSeparatedEnergies[lc]["bending"];

		// stress calculation - NOTE: only in-plane stresses are considered (dKQ stresses are ignored) because of the application in topology optimization, in which stress gradients over the thickness of the element cannot be considered in a 2D case
		Eigen::VectorXd elementDisp24DOF;
		elementDisp24DOF.setZero(24);
		elementDisp24DOF = mT * elementDisplacements;
		melementDisp8DOF.setZero(8);
		for (int i = 0; i < 4; ++i) // for all nodes of this element
		{
			for (int j = 0; j < 2; ++j) // for the first two DOF's in local system (disp x & y)
			{
				melementDisp8DOF(i*2 + j) = elementDisp24DOF(i*6 + j);
			}
		}
		mBAv.setZero(3,8);
		mBAv = (1.0/4) * (mB1 + mB2 + mB3 + mB4); // average B-matrix
		Eigen::VectorXd StrainAv;
		StrainAv.setZero(3);
		StrainAv = mBAv * melementDisp8DOF; // average strain
		mStress = mETermSolid * StrainAv; // average stress per element (averaged over 4 integration points)

		mE0K0U.setZero(24);
		mE0K0U = (mE0 / mE) * mSM * elementDisplacements; // for stress sensitivity
	} // computeResponse()
	
	void flat_shell::clearResponse()
	{
		element::clearResponse();
		mSeparatedEnergies.clear();
		mTotalEnergy = 0;
		mShearEnergy = 0;
		mAxialEnergy = 0;
		mBendEnergy = 0;
	} // clearResponse()
	
	const double& flat_shell::getEnergy(load_case lc, const std::string& type /*= ""*/) const
	{
		if (type == "")
		{
			try
			{
				return element::getEnergy(lc);
			}
			catch(std::exception& e)
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, when retrieving energies from a flat shell element.\n"
										 << "Received the following error message:\n" << e.what() << "\n"
										 << "(bso/structural_design/element/flat_shell.cpp)" << std::endl;
				throw std::runtime_error(errorMessage.str());
			}
		}
		
		if (mSeparatedEnergies.find(lc) == mSeparatedEnergies.end())
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, when retrieving energies from a flat shell element.\n"
									 << "Could not find it for load case: " << lc << "\n"
									 << "(bso/structural_design/element/flat_shell.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
		if (mSeparatedEnergies.find(lc)->second.find(type) == mSeparatedEnergies.find(lc)->second.end())
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, when retrieving energies from a flat shell element.\n"
									 << "Could not retrieve separated strain energy of type: " << type << "\n"
									 << "(bso/structural_design/element/flat_shell.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
		return mSeparatedEnergies.find(lc)->second.find(type)->second;
	} // getEnergy
	
	double flat_shell::getTotalEnergy(const std::string& type /*= ""*/) const
	{
		if (type == "")
		{
			return element::getTotalEnergy(type);
		}
		else if (type == "shear")
		{
			return mShearEnergy;
		}
		else if (type == "axial")
		{
			return mAxialEnergy;
		}
		else if (type == "bending")
		{
			return mBendEnergy;
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, when retrieving total energy from a flat shell element.\n"
									 << "Could not retrieve separated strain energy of type: " << type << "\n"
									 << "(bso/structural_design/element/flat_shell.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	}
	
	double flat_shell::getProperty(std::string var) const
	{ //
		if (var == "thickness") return mThickness;
		else if (var == "v") return mPoisson;
		else if (var == "E") return mE;
		else if (var == "Emin") return mEmin;
		else if (var == "E0") return mE0;
		else
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, could not retrieve variable: " << var
									 << "\nfrom flat shell element.\n"
									 << "(bso/structural_design/element/flat_shell.hpp" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // getProperty()
	
	double flat_shell::getVolume() const
	{
		return bso::utilities::geometry::quadrilateral::getArea() * mThickness;
	} // getVolume()
	
	double flat_shell::getStressAtCenter(const double& alpha /* 0*/, const double& beta /* 1.0 / sqrt(3)*/) const // NOTE: only in-plane stresses are considered (dKQ stresses are ignored) because of the application in topology optimization
	{
		double sx, sy, sxy, I1, J2D, DPStress;
		sx = mStress(0);
		sy = mStress(1);
		sxy = mStress(2);
		I1 = sx + sy;
		J2D = (1.0/3) * (pow(sx, 2) + pow(sy, 2) - sx * sy + 3 * pow(sxy, 2));
		DPStress = (1.0 / beta) * (sqrt(J2D) + alpha * I1); // this value should not exceed 1
		return DPStress;
	} // getStressCenter() - NOTE: if alpha & beta are not inserted in the function call, the Von Mises stress is obtained

	Eigen::VectorXd flat_shell::getStressSensitivityTermAE(const unsigned long freeDOFs, const double& alpha /* 0*/) const
	// Sensitivity calculation is based on the theory in:
	// Luo, Y., & Kang, Z. (2012). Topology optimization of continuum structures with Drucker-Prager yield stress constraints. Computers & Structures, 90-91, pp. 65-75. https://doi.org/10.1016/j.compstruc.2011.10.008
	{
		Eigen::Vector3d w;
		w << 1, 1, 0;
		Eigen::VectorXd W0;
		W0.setZero(8);
		W0 = mBAv.transpose() * mETermSolid.transpose() * w;
		Eigen::Matrix3d V;
		V << 1, -0.5, 0,
			 -0.5, 1, 0,
			 0, 0, 3;
		Eigen::MatrixXd M0;
		M0.setZero(8,8);
		M0 = mBAv.transpose() * mETermSolid.transpose() * V * mETermSolid * mBAv;

		Eigen::VectorXd aeloc;
		aeloc.setZero(8);
		aeloc = (M0.transpose() * melementDisp8DOF) / sqrt(3.0 * melementDisp8DOF.transpose() * M0 * melementDisp8DOF) + alpha * W0;

		Eigen::VectorXd ae24DOF, ae24DOFt;
		ae24DOF.setZero(24); ae24DOFt.setZero(24);
		int counterAeloc = 0;
		for (int i = 0; i < 4; ++i) // for all nodes of this element
		{
			for (int j = 0; j < 2; ++j) // for the first two DOF's (disp x & y)
			{
				ae24DOF(i*6 + j) = aeloc(counterAeloc); // put in 24 DOF local form
				++counterAeloc;
			}
		}
		ae24DOFt = mT.transpose() * ae24DOF;

		Eigen::VectorXd ae;
		ae.setZero(freeDOFs);
		int counterAE = -1;
		for(auto& i : mNodes) // for all nodes of this element
		{
			for (unsigned int j = 0; j < 6; ++j) // for all local DOF's
			{
				++counterAE;
				if (i->getNFS(j) == 0 || i->getConstraint(j) == 1) continue;
				unsigned int GDOF = i->getGlobalDOF(j);
				ae(GDOF) = ae24DOFt(counterAE);
			}
		}
		return ae;
	} // getStressSensitivityTermAE()

	Eigen::VectorXd flat_shell::getStressSensitivity(Eigen::MatrixXd& Lamda, const double& penal /* 1*/, const double& beta /* 1.0 / sqrt(3)*/) const
	// Sensitivity calculation is based on the theory in:
	// Luo, Y., & Kang, Z. (2012). Topology optimization of continuum structures with Drucker-Prager yield stress constraints. Computers & Structures, 90-91, pp. 65-75. https://doi.org/10.1016/j.compstruc.2011.10.008
	{
		Eigen::VectorXd dKdxU = (-penal / beta) * pow(mDensity,penal - 1) * mE0K0U;
		Eigen::MatrixXd lamdaloc;
		lamdaloc.setZero(24,Lamda.cols());
		Eigen::VectorXd dsx(Lamda.cols()); // dsx = vector with sensitivities for varying constraints, but to same x
		dsx.setZero();
		int counterLamda = -1;
		for (auto& j : mNodes) // for all nodes of this element
		{
			for (unsigned int k = 0; k < 6; ++k) // for all local DOF's
			{
				++counterLamda;
				if (j->getNFS(k) == 0 || j->getConstraint(k) == 1) continue;
				unsigned int GDOF = j->getGlobalDOF(k);
				lamdaloc.row(counterLamda) = Lamda.row(GDOF);
			}
		}
		dsx = lamdaloc.transpose() * dKdxU;
		return dsx;
	} // getStressSensitivity()

	bso::utilities::geometry::vertex flat_shell::getCenter() const
	{
		return bso::utilities::geometry::quadrilateral::getCenter();
	} // getCenter()
	
} // namespace element
} // namespace structural_design
} // namespace bso

#endif // SD_FLAT_SHELL_ELEMENT_CPP
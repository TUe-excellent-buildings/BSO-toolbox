#ifndef SD_TOPOPT_COMPONENT_WISE_SIMP_CPP
#define SD_TOPOPT_COMPONENT_WISE_SIMP_CPP

namespace bso { namespace structural_design { namespace topology_optimization {

class COMP_SIMP;

namespace comp_simp {
void ObjectiveAndSensitivity(
		 const std::map<component::geometry*, std::vector<element::element*>>& elePerComp,
		 double& c, const Eigen::VectorXd& x, Eigen::VectorXd& dc, Eigen::VectorXd& dv,
		 const double& penal);
void OptimalityCritUpdate(double l1, double l2, const double& xMove, 
		 const double& totalVolume, const double& f, Eigen::VectorXd& volume,
		 Eigen::VectorXd& x, Eigen::VectorXd& xNew, const Eigen::VectorXd& dv,
		 const Eigen::VectorXd& dc);

} // namespace comp_simp
} // namespace topology_optimization

template <>
void sd_model::topologyOptimization<topology_optimization::COMP_SIMP>(
			const double& f, const double& penal, const double& xMove,
			const double& tolerance)
{
	using namespace topology_optimization::comp_simp;
	std::ostream out(mTopOptStreamBuffer);
	std::map<component::geometry*, std::vector<element::element*>> fComp, bComp, tComp;
	double fVolume = 0, bVolume= 0, tVolume = 0, totVolume = 0; // initialised at 0, before each element volumes are added
	double cF, cB, cT; // sum of all the elements compliances (objective values)
	
	std::vector<element::element*> fEle, bEle, tEle;
	for (auto& i : mGeometries)
	{
		fEle.clear(); bEle.clear(); tEle.clear();
		for (auto& j : i->getElements())
		{
			if (!j->isActiveInCompliance()) continue;
			j->updateDensity(f,penal);
			if (j->isFlatShell())
			{
				fEle.push_back(j);
				fVolume += j->getVolume();
			}
			else if (j->isBeam())
			{
				bEle.push_back(j);
				bVolume += j->getVolume();
			}
			else if (j->isTruss())
			{
				tEle.push_back(j);
				tVolume += j->getVolume();
			}
		}
		if (!fEle.empty()) fComp[i] = fEle;
		if (!bEle.empty()) bComp[i] = bEle;
		if (!tEle.empty()) tComp[i] = tEle;
	}
	unsigned int numFComp = fComp.size(), numBComp = bComp.size(), numTComp = tComp.size();
	
	// initialise containers for element values
	Eigen::VectorXd xF(numFComp),       xB(numBComp),       xT(numTComp),
									xNewF(numFComp),    xNewB(numBComp),    xNewT(numTComp),
									xChangeF(numFComp), xChangeB(numBComp), xChangeT(numTComp),
									volumeF(numFComp),  volumeB(numBComp),  volumeT(numTComp),
									dcF(numFComp),      dcB(numBComp),      dcT(numTComp),
									dvF(numFComp),      dvB(numBComp),      dvT(numTComp);
	xChangeF.setZero(); xChangeB.setZero(); xChangeT.setZero();
	
	unsigned int compIndexI = 0;
	for (auto& i : fComp)
	{
		xF(compIndexI) = f;
		volumeF(compIndexI) = 0;
		for (auto& j : i.second) volumeF(compIndexI) += j->getVolume();
		++compIndexI;
	}
	compIndexI = 0;
	for (auto& i : bComp)
	{
		xB(compIndexI) = f;
		volumeB(compIndexI) = 0;
		for (auto& j : i.second) volumeB(compIndexI) += j->getVolume();
		++compIndexI;
	}
	compIndexI = 0;
	for (auto& i : tComp)
	{
		xT(compIndexI) = f;
		volumeT(compIndexI) = 0;
		for (auto& j : i.second) volumeT(compIndexI) += j->getVolume();
		++compIndexI;
	}
	
	totVolume = fVolume+bVolume+tVolume;
	out << "Total Volume: " << totVolume << std::endl;

	// initialise iteration
	double change = 1;
	int loop = 0;

	double loopStart = clock(), iterationStart, timeEnd = 0.0;

	// start iteration
	while (change > tolerance)
	{
		iterationStart = clock();
		if (loop%20 == 0)
		{
			out << std::endl
					<< std::setw(5)  << std::left << "loop"
					<< std::setw(15) << std::left << "Objective"
					<< std::setw(15) << std::left << "Volume"
					<< std::setw(15) << std::left << "Change"
					<< std::setw(10) << std::left << "Time" << std::endl;
		}

		++loop;
		cF = cB = cT = 0;

		// FEA
		mFEA->generateGSM();
		mFEA->solve("SimplicialLDLT");

		double volume = 0;
		if (fComp.size() > 0)
		{
			ObjectiveAndSensitivity(fComp,cF,xF,dcF,dvF,penal);
			OptimalityCritUpdate(0,1e9,xMove,fVolume,f,volumeF,xF,xNewF,dvF,dcF);
			
			unsigned int compIndexI = 0;
			for (auto& i : fComp)
			{
				for (auto& j : i.second) j->updateDensity(xNewF(compIndexI), penal);
				++compIndexI;
			}
			xChangeF = xNewF - xF;
			volume += (volumeF * xNewF.transpose()).trace();
		}
		if (bComp.size() > 0)
		{
			ObjectiveAndSensitivity(bComp,cB,xB,dcB,dvB,penal);
			OptimalityCritUpdate(0,1e9,xMove,bVolume,f,volumeB,xB,xNewB,dvB,dcB);
			
			unsigned int compIndexI = 0;
			for (auto& i : bComp)
			{
				for (auto& j : i.second) j->updateDensity(xNewB(compIndexI), penal);
				++compIndexI;
			}
			xChangeB = xNewB - xB;
			volume += (volumeB * xNewB.transpose()).trace();
		}
		if (tComp.size() > 0)
		{
			ObjectiveAndSensitivity(tComp,cT,xT,dcT,dvT,penal);
			OptimalityCritUpdate(0,1e9,xMove,tVolume,f,volumeT,xT,xNewT,dvT,dcT);
			
			unsigned int compIndexI = 0;
			for (auto& i : tComp)
			{
				for (auto& j : i.second) j->updateDensity(xNewT(compIndexI), penal);
				++compIndexI;
			}
			xChangeT = xNewT - xT;
			volume += (volumeT * xNewT.transpose()).trace();
		}

		// update change
		change = 0;
		if (fComp.size() > 0)
		{
			change = std::max(change, xChangeF.cwiseAbs().maxCoeff());
		}
		if (bComp.size() > 0)
		{
			change = std::max(change, xChangeB.cwiseAbs().maxCoeff());
		}
		if (tComp.size() > 0)
		{
			change = std::max(change, xChangeT.cwiseAbs().maxCoeff());
		}

		timeEnd = clock();
		out << std::setw(5)  << std::left << loop
				<< std::setw(15) << std::left << cF + cB + cT
				<< std::setw(15) << std::left << volume
				<< std::setw(15) << std::left << change
				<< std::setw(10) << std::left << (timeEnd - iterationStart)/CLOCKS_PER_SEC << std::endl;

		xF = xNewF;
		xB = xNewB;
		xT = xNewT;
	} // end of iteration
	out << "Topology optimisation successfully finished after: "
			<< (timeEnd - loopStart)/CLOCKS_PER_SEC << " seconds."
			<< std::endl << std::endl;
}

namespace topology_optimization { namespace comp_simp {
	
	
void ObjectiveAndSensitivity(
		 const std::map<component::geometry*, std::vector<element::element*>>& elePerComp,
		 double& c, const Eigen::VectorXd& x, Eigen::VectorXd& dc, Eigen::VectorXd& dv,
		 const double& penal)
{
	// objective function and sensitivity analysis (retrieve data from FEA)
	unsigned int compIndexI = 0;
	for (const auto& i : elePerComp)
	{
		dc(compIndexI) =  0; 
		dv(compIndexI) =  0;
		for (const auto& j : i.second)
		{
			c 						 += j->getTotalEnergy();
			dc(compIndexI) += j->getEnergySensitivity(penal); 
			dv(compIndexI) += j->getVolume();
		}
		++compIndexI;
	}

	dc = (dc * x.transpose()).diagonal();
}

void OptimalityCritUpdate(double l1, double l2, const double& xMove, 
		 const double& totalVolume, const double& f, Eigen::VectorXd& volume,
		 Eigen::VectorXd& x, Eigen::VectorXd& xNew, const Eigen::VectorXd& dv,
		 const Eigen::VectorXd& dc)
{
	// optimality criteria update of design variables and physical densities
	double lmid, upper, lower;
	while (((l2-l1)/(l1+l2))>1e-3)
	{
		lmid = (l1+l2)/2.0;

		for (unsigned int i = 0; i < x.size(); i++)
		{
			xNew(i) = x(i) * (std::sqrt(-dc(i)/(lmid*dv(i))));
			upper = std::min(1.0, (x(i) + xMove));
			lower = std::max(0.0, (x(i) - xMove));

			if (xNew(i) > upper)
			{
				xNew(i) = upper;
			}
			else if (xNew(i) < lower)
			{
				xNew(i) = lower;
			}
		}

		((volume * xNew.transpose()).trace() > f * totalVolume) ? l1 = lmid : l2 = lmid;
	}
}

} // namespace comp_simp
} // namespace topology_optimization
} // namespace structural_design
} // bso

#endif // SD_TOPOPT_COMPONENT_WISE_SIMP_CPP
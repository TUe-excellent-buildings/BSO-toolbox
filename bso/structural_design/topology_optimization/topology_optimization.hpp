#ifndef SD_TOPOLOGY_OPTIMIZATION_HPP
#define SD_TOPOLOGY_OPTIMIZATION_HPP

#include <bso/structural_design/fea.hpp>

namespace bso { namespace structural_design { namespace topology_optimization {
	
	void SIMP(fea* FEASystem, const double& f, 
						const double& rMin, const double& penal, const double& xMove,
						const double& tolerance, std::ostream& out);
						
	void robust(fea* FEASystem, const double& f, 
						const double& rMin, const double& penal, const double& xMove,
						const double& tolerance, std::ostream& out);
						
	void elementTypeSIMP(fea* FEASystem, const double& f, 
						const double& rMin, const double& penal, const double& xMove,
						const double& tolerance, std::ostream& out);
	
	void componentWiseSIMP(fea* FEASystem, const double& f, 
						const double& rMin, const double& penal, const double& xMove,
						const double& tolerance, std::ostream& out);
						
} // namespace topology_optimization
} // namespace structural_design
} // bso

#include <bso/structural_design/topology_optimization/SIMP.cpp>
#include <bso/structural_design/topology_optimization/robust.cpp>
#include <bso/structural_design/topology_optimization/elementTypeSIMP.cpp>
#include <bso/structural_design/topology_optimization/componentWiseSIMP.cpp>

#endif // SD_TOPOLOGY_OPTIMIZATION_HPP
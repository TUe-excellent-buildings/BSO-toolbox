#ifndef SD_TOPOPT_COMPONENT_WISE_SIMP_CPP
#define SD_TOPOPT_COMPONENT_WISE_SIMP_CPP

namespace bso { namespace structural_design { namespace topology_optimization {

class COMP_SIMP;

} // namespace topology_optimization

template <>
void sd_model::topologyOptimization<topology_optimization::COMP_SIMP>(const double& f, 
						const double& rMin, const double& penal, const double& xMove,
						const double& tolerance)
{
	
}

} // namespace structural_design
} // bso

#endif // SD_TOPOPT_COMPONENT_WISE_SIMP_CPP
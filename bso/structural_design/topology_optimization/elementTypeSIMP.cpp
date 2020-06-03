#ifndef SD_TOPOPT_ELEMENT_TYPE_SIMP_CPP
#define SD_TOPOPT_ELEMENT_TYPE_SIMP_CPP

namespace bso { namespace structural_design { namespace topology_optimization {

class ELE_SIMP;

} // namespace topology_optimization

template <>
void sd_model::topologyOptimization<topology_optimization::ELE_SIMP>(const double& f, 
						const double& rMin, const double& penal, const double& xMove,
						const double& tolerance)
{
	
}

} // namespace structural_design
} // bso

#endif // SD_TOPOPT_ELEMENT_TYPE_SIMP_CPP
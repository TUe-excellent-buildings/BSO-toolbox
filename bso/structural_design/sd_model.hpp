#ifndef SD_MODEL_HPP
#define SD_MODEL_HPP

#include <bso/structural_design/fea.hpp>
#include <bso/structural_design/component/point.hpp>
#include <bso/structural_design/component/line_segment.hpp>
#include <bso/structural_design/component/quadrilateral.hpp>
#include <bso/structural_design/component/quad_hexahedron.hpp>

namespace bso { namespace structural_design {
	
	struct sd_results;
	
	class sd_model
	{
	private:
		std::vector<component::point*> mPoints;
		std::vector<component::geometry*> mGeometries;
		std::vector<component::point*> mMeshedPoints;
		
		fea* mFEA;
		
		unsigned int mMeshSize = 1;
		bool mIsMeshed = false;
		void clearMesh();
	public:
		sd_model();
		sd_model(const sd_model& rhs);
		~sd_model();
		
		component::point* addPoint(bso::utilities::geometry::vertex p);
		component::geometry* addGeometry(const bso::utilities::geometry::line_segment& g);
		component::geometry* addGeometry(const bso::utilities::geometry::quadrilateral& g);
		component::geometry* addGeometry(const bso::utilities::geometry::quad_hexahedron& g);
		
		void setMeshSize(const unsigned int& n);
		void mesh();
		void mesh(const unsigned int& n);
		void analyze(std::string solver = "SimplicialLDLT");
		
		void topologyOptimization(const std::string& algorithm, const double& f, 
															const double& rMin, const double& penal, const double& xMove,
															const double& tolerance, std::ostream& out);
		void topologyOptimization(const std::string& algorithm, const double& f, 
															const double& rMin, const double& penal, const double& xMove,
															const double& tolerance);
		
		sd_results getTotalResults();
		sd_results getPartialResults(bso::utilities::geometry::polygon* geom);
		sd_results getPartialResults(bso::utilities::geometry::polyhedron* geom);
		
		fea* getFEA() {return mFEA;}
		fea* const getFEA() const {return mFEA;}
		const std::vector<component::point*>& getPoints() const {return mPoints;}
		const std::vector<component::geometry*> getGeometries() const {return mGeometries;}
	};
	
	struct sd_results
	{
		double mTotalStrainEnergy = 0.0;
		double mTotalStructuralVolume = 0.0;
		double mGhostStrainEnergy = 0.0;
		double mGhostStructuralVolume = 0.0;
		// std::map<element::load_case,double> strainEnergyPerLoadCase = {};
	};
	
} // namespace structural_design
} // namespace bso

#include <bso/structural_design/sd_model.cpp>

#endif // SD_MODEL_HPP
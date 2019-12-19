#ifndef VIS_MS_BUILDING_HPP
#define VIS_MS_BUILDING_HPP

#include <bso/visualization/models/model_base.hpp>
#include <bso/visualization/bsp/bsp.hpp>
#include <bso/spatial_design/ms_building.hpp>

#ifdef SD_MODEL_HPP
#include <bso/structural_design/sd_model.hpp>
#endif // SD_MODEL_HPP

#ifdef BP_MODEL_HPP
#include <bso/building_physics/bp_model.hpp>
#endif // BP_MODEL_HPP

#include <bitset>

namespace bso { namespace visualization
{

class MS_Model : public model_base
{
private:
	std::string mTitle;
	std::list<polygon*> polygons;
	std::list<label*>   labels;

	polygon_props  pprops;
	polygon_props  ppropsSurfaceType; // NEW

	line_props     lprops;
	label_props    lbprops;
	random_bsp     *pbsp;
public:
	MS_Model(const spatial_design::ms_building& ms, const std::string& type = "spaces",
					 const std::string& title = "ms_building", const double& linewidth = 1.0);
	~MS_Model();
	
	void render(const camera &cam) const;
	const std::string get_description();
}; // MS_Model

MS_Model::MS_Model(const spatial_design::ms_building& ms,
						const std::string& type /*= "spaces"*/, const std::string& title /*= "ms_building"*/,
						const double& linewidth /*= 1.0*/)
{
	lprops.width = linewidth;
	mTitle = title;
	pprops.ambient = rgba(0.1f, 0.5f, 0.1f, 0.3f);
	pprops.diffuse = rgba(0.2f, 1.0f, 0.2f, 0.3f);
	pprops.specular = rgba(0.2f, 1.0f, 0.2f, 0.3f);
	pprops.emission = rgba(0.0f, 0.0f, 0.0f, 0.0f);
	pprops.shininess = 60.0f;
	pprops.translucent = true;
	pprops.twosided = true;

	ppropsSurfaceType.ambient = rgba(1.0f, 1.0f, 0.04f, 0.4f);
	ppropsSurfaceType.diffuse = rgba(1.0f, 1.0f, 0.04f, 0.4f);
	ppropsSurfaceType.specular = rgba(0.04f, 0.04f, 0.04f, 1.0f);
	ppropsSurfaceType.emission = rgba(0.04f, 0.04f, 0.04f, 1.0f);
	ppropsSurfaceType.shininess = 60.0;
	ppropsSurfaceType.translucent = true;
	ppropsSurfaceType.twosided = true;
	
	for (const auto& i : ms)
	{
		namespace geom = bso::utilities::geometry;
	
		geom::quad_hexahedron spaceGeometry = i->getGeometry();
		std::stringstream centerLabel;
		centerLabel << i->getID();
		
		if (type == "spaces" || type == "" )
		{
			this->addPolyhedron(polygons, &spaceGeometry, &pprops, &lprops);
			if (type == "") centerLabel.str(std::string());
		}
		else if (type == "surface_type")
		{
			std::vector<std::string> surfaceTypes;
			if (!(i->getSurfaceTypes(surfaceTypes)))
			{
				std::stringstream errorMessage;
				errorMessage << "\nError in visualizing ms_buildings.\n"
										 << "Could not get surface types from space."
										 << "(bso/visualization/visualization/models/ms_building.hpp)" << std::endl;
				throw std::runtime_error(errorMessage.str());
			}
			std::swap(surfaceTypes[0],surfaceTypes[2]);
			std::swap(surfaceTypes[4],surfaceTypes[5]);
			for (unsigned int j = 0; j < 6; ++j)
			{
				centerLabel.str(std::string());
				centerLabel << surfaceTypes[j];
				auto tempSurface = spaceGeometry.getPolygons()[j];
				auto surfaceCenter = tempSurface->getCenter();
				std::vector<geom::vertex> surfaceVertices;
				for (unsigned int k = 0; k < 4; ++k)
				{
					surfaceVertices.push_back((*tempSurface)[k] + 0.2*(surfaceCenter - (*tempSurface)[k]));
				}
				geom::quadrilateral surfaceGeometry(surfaceVertices);
				this->addPolygon(polygons,&surfaceGeometry,&ppropsSurfaceType,&lprops,0.0);
				this->addLabel(labels,&lbprops,centerLabel.str(),surfaceGeometry.getCenter());
			}
			centerLabel.str(std::string());
		}
		else if (type == "space_type")
		{
			this->addPolyhedron(polygons, &spaceGeometry, &ppropsSurfaceType, &lprops);
			std::string spaceType;
			if (!(i->getSpaceType(spaceType)))
			{
				std::stringstream errorMessage;
				errorMessage << "\nError in visualizing ms_buildings.\n"
										 << "Could not get space type from space."
										 << "(bso/visualization/visualization/models/ms_building.hpp)" << std::endl;
				throw std::runtime_error(errorMessage.str());
			}
			centerLabel << " - " << spaceType;
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "\nError in visualizing ms_buildings.\n"
									 << "Did not recognize type of visualization: " << type << "\n"
									 << "Options are: spaces; surface_type; space_type\n" 
									 << "(bso/visualization/visualization/models/ms_building.hpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
		
		this->addLabel(labels,&lbprops,centerLabel.str(),spaceGeometry.getCenter());
	}

	pbsp = new random_bsp(polygons);
} //NEW

MS_Model::~MS_Model()
{
	delete pbsp;

	for (std::list<polygon*>::iterator pit = polygons.begin();
			 pit != polygons.end(); pit++) delete *pit;

	for (std::list<label*>::iterator lbit = labels.begin();
			 lbit != labels.end(); lbit++) delete *lbit;
}

const std::string MS_Model::get_description()
{
	return mTitle;
}

void MS_Model::render(const camera &cam) const
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	pbsp->render_btf(cam);

	for (auto lbit = labels.begin(); lbit != labels.end(); lbit++) (*lbit)->render();

	glPopAttrib();
}

} // namespace visualization
} // namespace bso

#endif // VIS_MS_BUILDING_HPP

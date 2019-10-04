#ifndef VIS_SD_MODEL_HPP
#define VIS_SD_MODEL_HPP

#ifndef PI
#define PI 3.14159265359
#endif // PI

#include <bso/structural_design/sd_model.hpp>
#include <bso/visualization/models/model_base.hpp>
#include <bso/visualization/bsp/bsp.hpp>

#include <cstdlib>
#include <cmath>
#include <algorithm>

namespace bso { namespace visualization
{

class SD_Model : public model_base
{
public:
	SD_Model(const bso::structural_design::sd_model& sd,
					 const std::string& type ="component",
					 const std::string& title ="sd_model",
					 const bool& ghostly = false,
					 const std::vector<std::pair<bso::utilities::geometry::vertex,
					 bso::utilities::geometry::vector>>& cuttingPlanes = {});
	~SD_Model();
	void render(const camera &cam) const;
	const std::string get_description();

	bool key_pressed(int key);

protected:

private:
	std::string mTitle;
	std::list<polygon*> polygons;
	std::list<label*> labels;
	
	rgba yellow = rgba(1.0,1.0,0.30,0.15);
	rgba blue = rgba(0.30, 0.30, 1.0, 0.15);
	rgba grey = rgba(0.65, 0.65, 0.65, 0.15);
	rgba red = rgba(1.0, 0.30, 0.30, 0.15);

	polygon_props beamprops, trussprops, shellprops, quadhexprops,
								beamprops_t, trussprops_t, shellprops_t, quadhexprops_t;
	std::map<double, polygon_props*> clusterPProps;
	std::map<double, line_props*> clusterLProps;

	line_props     lprops, lprops_t;
	label_props    lbprops;
	random_bsp     *pbsp;
};

SD_Model::SD_Model(const bso::structural_design::sd_model& sd,
									 const std::string& type /*="component"*/,
									 const std::string& title /*="sd_model"*/,
									 const bool& ghostly /*= false*/,
									 const std::vector<std::pair<bso::utilities::geometry::vertex,
									 bso::utilities::geometry::vector>>& cuttingPlanes /*= {}*/)
: model_base(cuttingPlanes)
{
	mTitle = title;
	lprops_t = rgba(0.0,0.0,0.0,0.1);
		
	beamprops.ambient = red;
	beamprops.diffuse = red;
	beamprops.specular = rgba(0.1,0.1,0.1,1);
	beamprops.shininess = 60;
	beamprops.translucent = false;
	beamprops.twosided = true;

	trussprops.ambient = blue;
	trussprops.diffuse = blue;
	trussprops.specular = rgba(0.1,0.1,0.1,1);
	trussprops.shininess = 60;
	trussprops.translucent = false;
	trussprops.twosided = true;

	shellprops.ambient = yellow;
	shellprops.diffuse = yellow;
	shellprops.specular = rgba(0.1,0.1,0.1,1);
	shellprops.shininess = 60;
	shellprops.translucent = false;
	shellprops.twosided = true;
	
	quadhexprops.ambient = grey;
	quadhexprops.diffuse = grey;
	quadhexprops.specular = rgba(0.1,0.1,0.1,1);
	quadhexprops.shininess = 60;
	quadhexprops.translucent = false;
	quadhexprops.twosided = true;

	beamprops_t = beamprops;
	trussprops_t = trussprops;
	shellprops_t = shellprops;
	quadhexprops_t = quadhexprops;
	beamprops_t.translucent = true;
	trussprops_t.translucent = true;
	shellprops_t.translucent = true;
	quadhexprops_t.translucent = true;
	
	namespace comp = bso::structural_design::component;
	namespace elem = bso::structural_design::element;
	namespace geom = bso::utilities::geometry;

	if (type == "component")
	{
		for (const auto& i : sd.getGeometries())
		{
			for (const auto& j : i->getStructures())
			{
				if (j.type() == "truss" || j.type() == "beam")
				{
					auto lineComponentPtr = dynamic_cast<comp::line_segment*>(i);
					geom::vector dirVector;
					
					if (lineComponentPtr->getVector().isVertical()) dirVector = {1,0,0};
					else dirVector = lineComponentPtr->getVector().cross(geom::vector({0,0,1}));
					
					if (j.type() == "truss")
					{
						double A = j.A();
						
						if (j.isVisible())
						{
							this->addLineSegment(polygons, lineComponentPtr, &trussprops, &lprops,
																	 sqrt(A), dirVector, sqrt(A));
						}
						else if (ghostly)
						{
							this->addLineSegment(polygons, lineComponentPtr, &trussprops_t, &lprops_t,
																	 sqrt(A), dirVector, sqrt(A));
						}
					}
					else
					{
						double w = j.width();
						double h = j.height();
						
						if (j.isVisible())
						{
							this->addLineSegment(polygons, lineComponentPtr, &beamprops, &lprops,
																	 w, dirVector, h);
						}
						else if (ghostly)
						{
							this->addLineSegment(polygons, lineComponentPtr, &beamprops_t, &lprops_t,
																	 w, dirVector, h);
						}
					}
				}
				else if (j.type() == "flat_shell")
				{
					auto quadComponentPtr = dynamic_cast<comp::quadrilateral*>(i);
					double thickness = j.thickness();
					if (j.isVisible())
					{
						this->addPolygon(polygons,quadComponentPtr,&shellprops,&lprops,thickness);
					}
					else if (ghostly)
					{
						this->addPolygon(polygons,quadComponentPtr,&shellprops_t,&lprops_t,thickness);
					}
				}
				else if (j.type() == "quad_hexahedron")
				{
					auto quadHexComponentPtr = dynamic_cast<comp::quad_hexahedron*>(i);
					if (j.isVisible())
					{
						this->addPolyhedron(polygons,quadHexComponentPtr,&quadhexprops,&lprops);
					}
					else if (ghostly)
					{
						this->addPolyhedron(polygons,quadHexComponentPtr,&quadhexprops_t,&lprops_t);
					}
				}
			}
		}
	}
	else
	{
		double minValue = 0, maxValue = 1;
		
		if (type == "strain_energy")
		{
			for (const auto& i : sd.getFEA()->getElements())
			{
				if (i->isActiveInCompliance())
				{
					minValue = log10(abs(i->getTotalEnergy())+1.0);
					maxValue = log10(abs(i->getTotalEnergy())+1.0);
					break;
				}
			}
			for (const auto& i : sd.getFEA()->getElements())
			{
				if (i->isActiveInCompliance())
				{
					double value = log10(abs(i->getTotalEnergy())+1.0);
					if (value < minValue) minValue = value;
					if (value > maxValue) maxValue = value;
				}
			}
		}
		
		unsigned int nClusters = 8;
		for (unsigned int i = 0; i < nClusters; ++i)
		{
			// initialise variables that handle the gradient of the colormap (just try it :) )
			double beta = 10;
			double eta = 0.62;

			// color gradient: on scale of 0-1 the colorband obtained by given beta and eta
			double colorGradient = (double)(i+1)/(double)nClusters;

			// compute the color values for this color gradient
			double red = (tanh(beta * eta) + tanh(beta * (colorGradient - eta))) /
									 (tanh(beta * eta) + tanh(beta * (1 - eta)));
			double green = pow((0.5 + 0.5*cos(2*PI*colorGradient - PI)), (eta/3));
			double blue = 1 - ((tanh(beta * (1 - eta)) + tanh(beta * (colorGradient - (1 - eta)))) /
												 (tanh(beta * (1 - eta)) + tanh(beta * eta)));
			double alpha;

			if (i < 2)
			{
				alpha = 0.01;
			}
			else
			{
				alpha = 0.9;
			}

			// assign the color values to the graphic properties structure
			polygon_props* tempPPropPtr = new polygon_props;
			tempPPropPtr->ambient = rgba(red,green,blue,alpha);
			tempPPropPtr->diffuse = rgba(red,green,blue,alpha);
			tempPPropPtr->specular = rgba(0,0,0,alpha);
			tempPPropPtr->shininess = 0.1;
			tempPPropPtr->translucent = true;
			tempPPropPtr->twosided = true;

			line_props* tempLPropPtr = new line_props;
			tempLPropPtr->color = rgba(0.1,0.1,0.1,alpha);

			clusterPProps[minValue+colorGradient*(maxValue-minValue)] = tempPPropPtr;
			clusterLProps[minValue+colorGradient*(maxValue-minValue)] = tempLPropPtr;
		}
		
		for (const auto& i : sd.getFEA()->getElements())
		{
			if (!i->visualize() && !ghostly) continue;
			polygon_props* eleVisPProps;
			line_props*		 eleVisLProps;
			if (type == "element")
			{
				if (i->visualize())
				{
					if (i->isTruss()) eleVisPProps = &trussprops;
					else if (i->isBeam()) eleVisPProps = &beamprops;
					else if (i->isFlatShell()) eleVisPProps = &shellprops;
					else if (i->isQuadHexahedron()) eleVisPProps = &quadhexprops;
					else eleVisLProps = nullptr;
					eleVisLProps = &lprops;
				}
				else
				{
					if (i->isTruss()) eleVisPProps = &trussprops_t;
					else if (i->isBeam()) eleVisPProps = &beamprops_t;
					else if (i->isFlatShell()) eleVisPProps = &shellprops_t;
					else if (i->isQuadHexahedron()) eleVisPProps = &quadhexprops_t;
					else eleVisLProps = nullptr;
					eleVisLProps = &lprops_t;
				}				
			}
			else if (type == "density" || type == "strain_energy")
			{
				if (!i->visualize()) continue; // will not be a density to visualize
				double lookupValue;
				if (type == "density") lookupValue = i->getDensity();
				else lookupValue = log10(abs(i->getTotalEnergy())+1.0);
				
				for (auto& j : clusterPProps)
				{
					if (j.first >= lookupValue)
					{
						eleVisPProps = j.second;
						break;
					}
				}
				for (auto& j : clusterLProps)
				{
					if (j.first >= lookupValue)
					{
						eleVisLProps = j.second;
						break;
					}
				}
			}
			else
			{
				std::stringstream errorMessage;
				errorMessage << "\nWhen visualizing a sd model, did not recognize\n"
										 << "the type of visualization: " << type << "\n"
										 << "(bso/visualization/models/sd_model.hpp)" << std::endl;
				throw std::runtime_error(errorMessage.str());
			}
			
			if (i->isTruss())
			{
				auto trussPtr = dynamic_cast<elem::truss*>(i);
				double A = trussPtr->getProperty("A");
				geom::vector dirVector;
				if (trussPtr->getVector().isVertical()) dirVector = {1,0,0};
				else dirVector = trussPtr->getVector().cross(geom::vector({0,0,1}));

				this->addLineSegment(polygons,trussPtr,eleVisPProps,eleVisLProps,sqrt(A),dirVector,sqrt(A));
			}
			else if (i->isBeam())
			{
				auto beamPtr = dynamic_cast<elem::beam*>(i);
				double width = beamPtr->getProperty("width");
				double height = beamPtr->getProperty("height");
				geom::vector dirVector;
				if (beamPtr->getVector().isVertical()) dirVector = {1,0,0};
				else dirVector = beamPtr->getVector().cross(geom::vector({0,0,1}));

				this->addLineSegment(polygons,beamPtr,eleVisPProps,eleVisLProps,width,dirVector,height);
			}
			else if (i->isFlatShell())
			{
				auto flatShellPtr = dynamic_cast<elem::flat_shell*>(i);
				double thickness = flatShellPtr->getProperty("thickness");
				this->addPolygon(polygons,flatShellPtr,eleVisPProps,eleVisLProps,thickness);
			}
			else if (i->isQuadHexahedron())
			{
				auto quadHexPtr = dynamic_cast<elem::quad_hexahedron*>(i);
				this->addPolyhedron(polygons,quadHexPtr,eleVisPProps,eleVisLProps);
			}
		}
		
	}

	pbsp = new random_bsp(polygons);
}

SD_Model::~SD_Model()
{
	delete pbsp;

	for (auto pit = polygons.begin();pit != polygons.end(); pit++)
	{
		delete *pit;
	}
		
	for (auto lbit = labels.begin();lbit != labels.end(); lbit++)
	{
		delete *lbit;
	}
	for (auto& i : clusterPProps) delete i.second;
	for (auto& i : clusterLProps) delete i.second;
}

const std::string SD_Model::get_description()
{
 return mTitle;
}

void SD_Model::render(const camera &cam) const
{
	glPushAttrib(GL_ENABLE_BIT);

	glDisable(GL_DEPTH_TEST);

	pbsp->render_btf(cam);

	for (auto& i : labels) i->render();
	
	glPopAttrib();
}

bool SD_Model::key_pressed(int key)
{
	switch (key)
	{
	case 't':
	case 'T':
	{
		//toggle geometry translucency
		beamprops.translucent = !beamprops.translucent;
		trussprops.translucent = !trussprops.translucent;
		shellprops.translucent = !shellprops.translucent;
		quadhexprops.translucent = !quadhexprops.translucent;

		if (beamprops.translucent)
		{
			auto red_t = red;
			red_t[3] = 0;
			auto blue_t = blue;
			blue_t[3] = 0;
			auto yellow_t = yellow;
			yellow_t[3] = 0;
			auto grey_t = grey;
			grey_t[3] = 0;
			beamprops_t.ambient = red_t;
			beamprops_t.diffuse = red_t;
			trussprops_t.ambient = blue_t;
			trussprops_t.diffuse = blue_t;
			shellprops_t.ambient = yellow_t;
			shellprops_t.diffuse = yellow_t;
			quadhexprops_t.ambient = grey_t;
			quadhexprops_t.diffuse = grey_t;
			lprops_t = rgba(1.0,1.0,1.0,0.0);
		}
		else
		{
			beamprops_t.ambient = red;
			beamprops_t.diffuse = red;
			trussprops_t.ambient = blue;
			trussprops_t.diffuse = blue;
			shellprops_t.ambient = yellow;
			shellprops_t.diffuse = yellow;
			quadhexprops_t.ambient = grey;
			quadhexprops_t.diffuse = grey;
			lprops_t = rgba(1.0,1.0,1.0,0.15);
		}

		for (auto& i : clusterPProps)
		{
			i.second->translucent = !i.second->translucent;
		}
		

		return true;
	}
	}

	return false;
}

} // namespace visualization
} // namespace bso


#endif // VIS_SD_MODEL_HPP

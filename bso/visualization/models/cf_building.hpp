#ifndef CONFORMAL_MODEL_HPP
#define CONFORMAL_MODEL_HPP

#include <bso/spatial_design/cf_building.hpp>
#include <bso/visualization/models/model_base.hpp>
#include <bso/visualization/bsp/bsp.hpp>

#include <cstdlib>

namespace bso { namespace visualization
{

class Conformal_Model : public model_base
{
public:
		Conformal_Model(const bso::spatial_design::cf_building& cf,
										const std::string& type, const std::string& title);
		~Conformal_Model();
		void render(const camera &cam) const;
		const std::string get_description();

		bool key_pressed(int key);

protected:

private:
		std::string mTitle;
		std::list<polygon*> polygons;
		std::list<label*>   labels;

		polygon_props  pprops_rectangle;
		polygon_props  pprops_cuboid;
		polygon_props  pprops_cuboid_overlap;

		line_props     lprops;
		label_props    lbprops;
		random_bsp     *pbsp;
};

Conformal_Model::Conformal_Model(const bso::spatial_design::cf_building& cf,
																 const std::string& type, const std::string& title)
{
	mTitle = title;
	pprops_rectangle.ambient = rgba(0.5f, 0.5f, 0.05f, 0.3f);
	pprops_rectangle.diffuse = rgba(1.0f, 1.0f, 0.1f, 0.3f);
	pprops_rectangle.specular = rgba(1.0f, 1.0f, 0.2f, 0.3f);
	pprops_rectangle.emission = rgba(0.0f, 0.0f, 0.0f, 0.0f);
	pprops_rectangle.shininess = 60.0;
	pprops_rectangle.translucent = true;
	pprops_rectangle.twosided = true;

	pprops_cuboid.ambient = rgba(0.1f, 0.5f, 0.1f, 0.3f);
	pprops_cuboid.diffuse = rgba(0.2f, 1.0f, 0.2f, 0.3f);
	pprops_cuboid.specular = rgba(0.2f, 1.0f, 0.2f, 0.3f);
	pprops_cuboid.emission = rgba(0.0f, 0.0f, 0.0f, 0.0f);
	pprops_cuboid.shininess = 60.0;
	pprops_cuboid.translucent = true;
	pprops_cuboid.twosided = true;

	pprops_cuboid_overlap.ambient = rgba(1.0f, 0.1f, 0.1f, 1.0f);
	pprops_cuboid_overlap.diffuse = rgba(1.0f, 0.2f, 0.2f, 1.0f);
	pprops_cuboid_overlap.specular = rgba(1.0f, 0.2f, 0.2f, 1.0f);
	pprops_cuboid_overlap.emission = rgba(0.0f, 0.0f, 0.0f, 0.0f);
	pprops_cuboid_overlap.shininess = 60.0;
	pprops_cuboid_overlap.translucent = true;
	pprops_cuboid_overlap.twosided = true;

	double scale = 0.8;
	namespace geom = bso::utilities::geometry;
	
	for (const auto& i : cf.cfSpaces())
	{
		this->addLabel(labels,&lbprops,std::to_string(i->getSpaceID()),i->getCenter());
	}

	if (type == "line_segment")
	{
		for (const auto& i : cf.cfLines())
		{
			std::vector<geom::vertex> lineVertices;
			for (const auto& j : *i)
			{
				lineVertices.push_back(i->getCenter() + scale*(j - i->getCenter()));
			}
			geom::line_segment lineGeometry(lineVertices);
			this->addLineSegment(polygons,&lineGeometry,&pprops_rectangle,&lprops);
		}
	}
	else if (type == "rectangle")
	{
		for (const auto& i : cf.cfRectangles())
		{
			std::vector<geom::vertex> rectangleVertices;
			for (const auto& j : *i)
			{
				rectangleVertices.push_back(i->getCenter() + scale*(j - i->getCenter()));
			}
			geom::quadrilateral rectangleGeometry(rectangleVertices);
			this->addPolygon(polygons,&rectangleGeometry,&pprops_rectangle,&lprops,0.0);
		}
	}
	else if (type == "cuboid")
	{
		for (const auto& i : cf.cfCuboids())
		{
			std::vector<geom::vertex> cuboidVertices;
			for (const auto& j : *i)
			{
				cuboidVertices.push_back(i->getCenter() + scale*(j - i->getCenter()));
			}
			geom::quad_hexahedron cuboidGeometry(cuboidVertices);
			if (i->cfSpaces().size() > 1)
			{
				this->addPolyhedron(polygons,&cuboidGeometry,&pprops_cuboid_overlap,&lprops);
			}
			else
			{
				this->addPolyhedron(polygons,&cuboidGeometry,&pprops_cuboid,&lprops);
			}
		}
	}

	pbsp = new random_bsp(polygons);
}

Conformal_Model::~Conformal_Model()
{
		delete pbsp;

		for (std::list<polygon*>::iterator pit = polygons.begin();
				 pit != polygons.end(); pit++)
				delete *pit;

		for (std::list<label*>::iterator lbit = labels.begin();
				 lbit != labels.end(); lbit++)
				delete *lbit;
}

const std::string Conformal_Model::get_description()
{
		return mTitle;
}

void Conformal_Model::render(const camera &cam) const
{
	glPushAttrib(GL_ENABLE_BIT);

	glDisable(GL_DEPTH_TEST);

	pbsp->render_btf(cam);

	std::list<label*>::const_iterator lbit;
	for (lbit = labels.begin(); lbit != labels.end(); lbit++)
	{
		(*lbit)->render();
	}
	glPopAttrib();
}

bool Conformal_Model::key_pressed(int key)
{
		switch (key)
		{
		case 't':
		case 'T':
			//toggle geometry translucency
			pprops_rectangle.translucent = !pprops_rectangle.translucent;
			pprops_cuboid.translucent = !pprops_cuboid.translucent;

			return true;
		};
	return false;
}

} // namespace Visualisation
} // namespace BSO


#endif // CONFORMAL_MODEL_HPP

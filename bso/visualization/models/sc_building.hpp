#ifndef SUPERCUBE_MODEL_HPP
#define SUPERCUBE_MODEL_HPP

#include <bso/spatial_design/sc_building.hpp>
#include <bso/visualization/models/model_base.hpp>
#include <bso/visualization/bsp/bsp.hpp>

namespace bso { namespace visualization
{

class SC_Model : public model_base
{
public:
	SC_Model(const bso::spatial_design::sc_building& sc, const std::string& title = "sc_building");
	~SC_Model();
	void render(const camera &cam) const;
	const std::string get_description();

protected:

private:
	std::string mTitle;
	std::list<polygon*> polygons;
	std::list<label*>   labels;

	polygon_props  pprops_empty_cell;
	polygon_props  pprops_active_cell;

	line_props     lprops;
	label_props    lbprops;
	random_bsp     *pbsp;
};

SC_Model::SC_Model(const bso::spatial_design::sc_building& sc, const std::string& title /*= "sc_building"*/)
{
	mTitle = title;
	pprops_empty_cell.ambient = rgba(0.1f, 0.1f, 0.1f, 0.2f);
	pprops_empty_cell.diffuse = rgba(0.1f, 0.1f, 0.1f, 0.03f);
	pprops_empty_cell.specular = rgba(0.1f, 0.1f, 0.1f, 0.0f);
	pprops_empty_cell.emission = rgba(0.0f, 0.0f, 0.0f, 0.0f);
	pprops_empty_cell.shininess = 60.0;
	pprops_empty_cell.translucent = true;
	pprops_empty_cell.twosided = true;

	pprops_active_cell.ambient = rgba(0.5f, 0.5f, 0.05f, 0.3f);
	pprops_active_cell.diffuse = rgba(1.0f, 1.0f, 0.1f, 0.3f);
	pprops_active_cell.specular = rgba(1.0f, 1.0f, 0.2f, 0.3f);
	pprops_active_cell.emission = rgba(0.0f, 0.0f, 0.0f, 0.0f);
	pprops_active_cell.shininess = 60.0;
	pprops_active_cell.translucent = true;
	pprops_active_cell.twosided = true;

	std::vector<double> xValues; xValues.push_back(0.0);
	std::vector<double> yValues; yValues.push_back(0.0);
	std::vector<double> zValues; zValues.push_back(0.0);

	for (unsigned int wIndex = 0; wIndex < sc.getWSize(); ++wIndex)
	{
		xValues.push_back(xValues[wIndex] + sc.getWValue(wIndex));
	}
	for (unsigned int dIndex = 0; dIndex < sc.getDSize(); ++dIndex)
	{
		yValues.push_back(yValues[dIndex] + sc.getDValue(dIndex));
	}
	for (unsigned int hIndex = 0; hIndex <sc.getHSize(); ++hIndex)
	{
		zValues.push_back(zValues[hIndex] + sc.getHValue(hIndex));
	}

	bso::utilities::geometry::vertex vMin, vMax;
	for (unsigned int cellIndex = 1; cellIndex <= sc.getBRowSize(); ++cellIndex) // first index (=0) contains room ID
	{
		vMin = {xValues[sc.getWIndex(cellIndex)],
					  yValues[sc.getDIndex(cellIndex)],
					  zValues[sc.getHIndex(cellIndex)]};
		vMax = {xValues[sc.getWIndex(cellIndex)+1],
					  yValues[sc.getDIndex(cellIndex)+1],
					  zValues[sc.getHIndex(cellIndex)+1]};
		std::vector<bso::utilities::geometry::vertex> cellVertices;		 
		for (unsigned int i = 0; i < 8; ++i)
		{
			cellVertices.push_back(vMin);
			for (unsigned int j = 0; j < 3; ++j)
			{
				if (std::bitset<3>(i)[j]) cellVertices.back()[j] = vMax[j];
			}
		}
		
		bso::utilities::geometry::quad_hexahedron cellGeometry(cellVertices);
		unsigned int spaceIndex;
		if (sc.getSpaceIndex(cellIndex,spaceIndex))
		{
			this->addPolyhedron(polygons,&cellGeometry,&pprops_active_cell,&lprops);
			this->addLabel(labels,&lbprops,std::to_string(spaceIndex+1),cellGeometry.getCenter());
		}
		else
		{
			this->addPolyhedron(polygons,&cellGeometry,&pprops_empty_cell,&lprops);
		}
	}
	pbsp = new random_bsp(polygons);
}

SC_Model::~SC_Model()
{
	delete pbsp;

	for (std::list<polygon*>::iterator pit = polygons.begin(); pit != polygons.end(); pit++) 
	{
		delete *pit;
	}
	for (std::list<label*>::iterator lbit = labels.begin(); lbit != labels.end(); lbit++)
	{
		delete *lbit;
	}
}

const std::string SC_Model::get_description()
{
	return mTitle;
}

void SC_Model::render(const camera &cam) const
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);

	pbsp->render_btf(cam);

	std::list<label*>::const_iterator lbit;
	for (lbit = labels.begin(); lbit != labels.end(); lbit++) (*lbit)->render();

	glPopAttrib();
}

} // namespace visualization
} // namespace bso


#endif // SUPERCUBE_MODEL_HPP

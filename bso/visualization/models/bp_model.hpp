#ifndef BP_MODEL_HPP
#define BP_MODEL_HPP

#include <bso/building_physics/bp_model.hpp>
#include <bso/visualization/models/model_base.hpp>
#include <bso/visualization/bsp/bsp.hpp>

#include <cstdlib>
#include <cmath>
#include <algorithm>
// #include <memory>
// #include <vector>

namespace bso { namespace visualization
{

class BP_Model : public model_base
{
public:
	BP_Model(const bso::building_physics::bp_model& bp,
					 const std::string title = "bp_model");
	~BP_Model();
	void render(const camera &cam) const;
	const std::string get_description();

protected:

private:
	std::string mTitle;
	std::list<polygon*> polygons;
	std::list<label*> labels;
	std::map<std::string, polygon_props*> pplist;

	line_props     lprops;
	label_props    lbprops;
	random_bsp     *pbsp;
	
	polygon_props* getPProps(const std::pair<std::string,Eigen::Vector4d>& visProps);
};

polygon_props* BP_Model::getPProps(const std::pair<std::string,Eigen::Vector4d>& visProps)
{
	auto ppropSearch = pplist.find(visProps.first);
	if (ppropSearch == pplist.end())
	{
		auto newPPropPtr = new polygon_props();
		auto v = visProps.second;
		
		newPPropPtr->ambient     = rgba(v(0),v(1),v(2),v(3));
		newPPropPtr->diffuse     = rgba(v(0),v(1),v(2),v(3));
		newPPropPtr->specular    = rgba(0.1,0.1,0.1,1);
		newPPropPtr->shininess   = 60;
		newPPropPtr->translucent = (v(3) < 1);
		newPPropPtr->twosided    = true;

		return newPPropPtr;
	}
	else return ppropSearch->second;
} // getPProps

BP_Model::BP_Model(const bso::building_physics::bp_model& bp,
									 const std::string title /*= "bp_model"*/)
{
	for (const auto& i : bp.getWalls())
	{
		auto ppropsPtr = getPProps(i->getVisualizationProperties());
		this->addPolygon(polygons,i->getGeometry(),ppropsPtr,&lprops,i->getThickness());
	}
	for (const auto& i : bp.getFloors())
	{
		auto ppropsPtr = getPProps(i->getVisualizationProperties());
		this->addPolygon(polygons,i->getGeometry(),ppropsPtr,&lprops,i->getThickness());
	}
	for (const auto& i : bp.getWindows())
	{
		auto ppropsPtr = getPProps(i->getVisualizationProperties());
		this->addPolygon(polygons,i->getGeometry(),ppropsPtr,&lprops);
	}

	pbsp = new random_bsp(polygons);
}

BP_Model::~BP_Model()
{
	delete pbsp;
	for (auto& i : polygons) delete i;
	for (auto& i : labels) delete i;
	for (auto& i : pplist) delete i.second;
	polygons.clear();
	pplist.clear();
}

const std::string BP_Model::get_description()
{
	return mTitle;
}

void BP_Model::render(const camera &cam) const
{
	glPushAttrib(GL_ENABLE_BIT);

	glDisable(GL_DEPTH_TEST);

	pbsp->render_btf(cam);

	for (auto& i : labels) i->render();

	glPopAttrib();
}

} // namespace Visualisation
} // namespace BSO


#endif // BP_MODEL_HPP

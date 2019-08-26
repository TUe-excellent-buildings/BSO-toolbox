#ifndef BSO_XML_MODEL_CPP
#define BSO_XML_MODEL_CPP

#include <stdexcept>
#include <sstream>

namespace bso { namespace spatial_design { namespace xml {

xml_model::xml_model(const cf_building& cf, const std::string& version,
	const std::string& language /*= "dutch"*/)
{ // constructor to initialise the XML data model with a conformal spatial design
	using boost::property_tree::ptree;
	namespace geom = bso::utilities::geometry;
	mPTree.clear();

	if (language == "end")
	{
		mPTree.put("building_model.<xmlattr>.version", version);
		mPTree.put("building_model.<xmlattr>.name", "");
	}
	else if (language == "dutch")
	{
		mPTree.put("gebouwmodel.<xmlattr>.versie", version);
		mPTree.put("gebouwmodel.<xmlattr>.naam", "");
	}
	else 
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to initialize XML model with an unknown language:\n"
								 << language << "\n"
								 << "(bso/spatial_design/xml/xml_model.cpp)" << std::endl;
	}

	unsigned int rectangleID = 0;
	for (const auto& i : cf.cfRectangles())
	{
		if (i->cfSurfaces().size() > 0)
		{
			++rectangleID;
			auto rectPtr = new geom::quadrilateral(*i);
			mRectanglePtrs[rectPtr] = rectangleID;
			
			std::vector<double> coordinates;
			for (const auto& j : *i)
			{ // for each vertex of the rectangle
				coordinates.push_back(j(0)); // x-coordinate of vertex j
				coordinates.push_back(j(1)); // y-coordinate of vertex j
				coordinates.push_back(j(2)); // z-coordinate of vertex j
			} // end all vertices of rectangle i
			
			if (language == "english")
			{
				ptree& constrNode = mPTree.add("building_model.constructions.construction", "");
				constrNode.put("<xmlattr>.name", "rectangle_" + std::to_string(rectangleID));
				constrNode.put("<xmlattr>.id", rectangleID);
				ptree& geoNode = constrNode.put("geometry", coordinates);
				geoNode.put("<xmlattr>.type", "plane");
			}
			else
			{
				ptree& constrNode = mPTree.add("gebouwmodel.constructies.constructie", "");
				constrNode.put("<xmlattr>.naam", "rechthoek_" + std::to_string(rectangleID));
				constrNode.put("<xmlattr>.id", rectangleID);
				ptree& geoNode = constrNode.put("geometrie", coordinates);
				geoNode.put("<xmlattr>.type", "vlak");
			}
		}
	}
	
	unsigned int spaceID = 0;
	for (const auto& i : cf.cfSpaces())
	{
		++spaceID;
		auto spacePtr = new geom::quad_hexahedron(*i);
		mSpacePtrs[spacePtr] = spaceID; // add an ID to that space and link it to the pointer of the space
		
		std::vector<int> rectangleIDList;
		for (const auto& j : cf.cfSurfaces())
		{ // for each surface
			for (const auto& k : j->cfRectangles())
			{
				bool surfaceIDFound = false;
				for (const auto l : mRectanglePtrs)
				{
					if (l.first->isSameAs(*k))
					{
						rectangleID = l.second;
						surfaceIDFound = true;
						break;
					}
				}
				if (!surfaceIDFound)
				{
					std::stringstream errorMessage;
					errorMessage << "\nError, when initializing an xml model from a conformal\n"
											 << "building model, could not find the surface ID of a space's surface\n"
											 << "(bso/spatial_design/xml/xml_model.cpp)" << std::endl;
					throw std::runtime_error(errorMessage.str());
				}
				rectangleIDList.push_back(rectangleID);
			}
		}
		
		if (language == "english")
		{
			ptree& space_node = mPTree.add("building_model.spaces.space", "");
			space_node.put("<xmlattr>.name", "simple");
			space_node.put("<xmlattr>.id", spaceID);
			space_node.put("construction_ids", rectangleIDList);
		}
		else 
		{
			ptree& space_node = mPTree.add("gebouwmodel.ruimtes.ruimte", "");
			space_node.put("<xmlattr>.naam", "simpel");
			space_node.put("<xmlattr>.id", spaceID);
			space_node.put("constructieids", rectangleIDList);
		}
	}
} // ctor

xml_model::xml_model(const std::string& fileName)
{
	readFromFile(fileName);
} // ctor

xml_model::~xml_model()
{

} // dtor

#ifdef SD_MODEL_HPP
void xml_model::addSDModel(const bso::structural_design::sd_model& sd)
{ // doesn't do anything for now
	/*// check if this SD_model has been initiated with the same conformal design
	unsigned int n_components = SD.get_component_count();
	for (unsigned int i = 0; i < n_components; i++)
	{
	//Structural_Design::Components::Component* temp_ptr = SD.get_component_ptr(i);
	//if (temp_ptr->is_flat_shell())
	}

	using boost::property_tree::ptree;
	ptree& SD_node = m_pt.put("building_model.structural_design", "");

	for (unsigned int i = 0; i < SD.get_flat_shell_props().size(); i++)
	{ // for each structural property set
		ptree& SD_prop_node = SD_node.add("structural_property", "");
		SD_prop_node.put("<xmlattr>.id", SD.get_flat_shell_props()[i].m_ID);
		SD_prop_node.put("<xmlattr>.type", "flat_shell_props");
		SD_prop_node.put("thickness", SD.get_flat_shell_props()[i].m_t);
		SD_prop_node.put("youngs_modulus", SD.get_flat_shell_props()[i].m_E);
		SD_prop_node.put("poisson_ratio", SD.get_flat_shell_props()[i].m_v);

		std::vector<int> construction_ids;
		for (unsigned int j = 0; j < 3; j++)
		{ // for each construction id to which this property set is assigned
				//construction_ids.push_back(1); // add this
		}

		if(construction_ids.size() != 0)
		{
				SD_prop_node.add("construction_ids", construction_ids);
		}

	}*/
} // add_structural_design()
#endif // SD_MODEL_HPP

#ifdef BSO_BP_MODEL_HPP
void xml_model::addBPModel(const bso::building_physics::bp_model& bp)
{
    // check if this SD_model has been initiated with the same conformal design

} // add_building_physics_design()
#endif // BSO_BP_MODEL_HPP


ms_building xml_model::initMSBuilding()
{
	ms_building ms;
	// for every space that is listed in the ptree
	using boost::property_tree::ptree;
	namespace geom = bso::utilities::geometry;
	BOOST_FOREACH(ptree::value_type const& v, mPTree.get_child("gebouwmodel.ruimtes"))
	{ // for each space

		std::vector<std::string> constructionIDs;
		if (v.first == "ruimte")
		{
			constructionIDs = v.second.get<std::vector<std::string> >("constructieids");
		}
		else continue;

		// get the minimum and maximum coordinates
		// store the coordinates of each construction
		std::map<std::string, std::vector<double> > coords;
		BOOST_FOREACH(ptree::value_type const&w, mPTree.get_child("gebouwmodel.constructies"))
		{ // for each construction
			if (w.first == "constructie")
			{
				coords[w.second.get<std::string>("<xmlattr>.id")] = w.second.get<std::vector<double> >("geometrie");
			}
		}

		geom::vertex min;
		min << coords.begin()->second[0], coords.begin()->second[1], coords.begin()->second[2];
		geom::vertex max = min;
		for (const auto& w : constructionIDs)
		{ // for each construction belonging to the space w

			std::vector<double> tempCoords = coords[w];

			for (unsigned int i = 0; i*3 < tempCoords.size(); i += 3)
			{ // for each coordinate that is present
				for (unsigned int j = 0; j < 3; j++)
				{ // for each dof of that coordinate
					if (min(j) > tempCoords[i*3+j])
					{
						min(j) = tempCoords[i*3+j];
					}
					if (max(j) < tempCoords[i*3+j])
					{
						max(j) = tempCoords[i*3+j];
					}
				}
			}
		}

		// use the coordinates to initialise a space for the ms_building
		ms_space tempSpace(1,min,max-min);
		ms.addSpace(tempSpace);
	}
	return ms;
} // init_MS_building()

void xml_model::writeToFile(const std::string& fileName)
{
	std::ofstream os(fileName.c_str());
	if (!os.is_open())
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, could not open file: " << fileName << "\n"
								 << "when writing building spatial model to xml file.\n"
								 << "(bso/spatial_design/xml/xml_model.cpp)" << std::endl;
	}
	boost::property_tree::write_xml(os, mPTree, boost::property_tree::xml_writer_settings<std::string>(' ', 2));
	os.close();
} //write_xml()

void xml_model::readFromFile(const std::string& fileName)
{
	mPTree.clear();
	std::ifstream is(fileName.c_str());
	if (!is.is_open())
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, could not open file: " << fileName << "\n"
								 << "when reading building spatial model from xml file.\n"
								 << "(bso/spatial_design/xml/xml_model.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	boost::property_tree::read_xml(is, mPTree);
	is.close();
} // read_xml()

} // namespace xml
} // namespace spatial_design
} // namespace bso

#endif // XML_MODEL_CPP

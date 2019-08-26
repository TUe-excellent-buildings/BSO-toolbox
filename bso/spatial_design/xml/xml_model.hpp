#ifndef BSO_XML_MODEL_HPP
#define BSO_XML_MODEL_HPP

#include <bso/utilities/geometry.hpp>
#include <bso/spatial_design/cf_building.hpp>
#include <bso/spatial_design/xml/xml_vector_translator.hpp>

#include <map>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace bso { namespace spatial_design { namespace xml {

class xml_model
{
private:
    std::map<bso::utilities::geometry::quadrilateral*, unsigned int> mRectanglePtrs;
    std::map<bso::utilities::geometry::quad_hexahedron*, unsigned int> mSpacePtrs;

    boost::property_tree::ptree mPTree;
public:
    xml_model(const cf_building& cf, const std::string& version,
			const std::string& language = "dutch");
    xml_model(const std::string& fileName);
    ~xml_model();

    #ifdef SD_MODEL_HPP
    void addSDModel(const bso::structural_design::sd_model& sd);
    #endif // SD_MODEL_HPP

    #ifdef BSO_BP_MODEL_HPP
    void addBPModel(const bso::building_physics::bp_model& bp);
    #endif // BSO_BP_MODEL_HPP

    ms_building initMSBuilding();

    void writeToFile(const std::string& fileName);
    void readFromFile(const std::string& fileName);

}; // class xml_model


} // namespace xml
} // namespace spatial_design
} // namespace bso

#include <bso/spatial_design/xml/xml_model.cpp>

#endif // BSO_XML_MODEL_HPP

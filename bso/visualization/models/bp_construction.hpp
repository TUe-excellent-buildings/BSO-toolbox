#ifndef BP_CONSTRUCTION_HPP
#define BP_CONSTRUCTION_HPP

#include <string>
#include <vector>

namespace BSO { namespace Building_Physics
{
    /*
     * This file contains the data structures related to the building physics
     * model.
     */

    // Declarations:

    struct BP_Space_Settings
    {
        std::string m_space_set_ID;
        double m_heating_capacity, m_cooling_capacity;
        double m_heat_set_point, m_cool_set_point;
        double m_ACH;
    };

    struct BP_Material // holds the name and properties of a material
    {
        std::string m_material_ID; // material ID
        std::string m_name; // name/description of the material
        double m_spec_weight, m_spec_heat, m_therm_conductivity; // material properties
    }; // BP_Material

    struct BP_Con_Layer // holds the material and thickness of a construction layer
    {
        BP_Material m_material; // material type
        double m_thickness; // layer thickness
    }; // BP_Con_Layer

    struct BP_Construction // without air resistances!
    {
        std::string m_construction_ID;
        std::vector<BP_Con_Layer> m_layers; // vector lists layers from side 1 to side 2, side 1 is default outside.
        double m_total_thickness, m_capacitance_per_area; // total wall thickness and total capacity of the entire wall
        double m_resistance_to_side_1, m_resistance_to_side_2; // resistances per square meter between this state and adjacent states
        double m_measure_point; // fraction of wall thickness whereat the temperature of the wall is measured, measuring from side 1

        // functions
        double get_capacitance_per_area(); // returns the heat capacity per square metre of this construction's surface
        double get_resistance_to_side_1(); // returns the resistance of this construction to flux to side one
        double get_resistance_to_side_2(); // returns the resistance of this construction to flux to side two
    }; // BP_Construction

    struct BP_Glazing
    {
        std::string m_glazing_ID;
        double m_U_value;
        double m_capacitance_per_area;
    }; // BP_Glazing


    // Implementations of member functions:
    double BP_Construction::get_capacitance_per_area() // getter function for construction's capacity per square metre
    {
        return m_capacitance_per_area;
    } // get_capacitance_per_area()

    double BP_Construction::get_resistance_to_side_1() // getter function for construction's resistance to flux to side one
    {
        return m_resistance_to_side_1;
    } // get_resistance_to_side_1()

    double BP_Construction::get_resistance_to_side_2() // getter function for construction's resistance to flux to side two
    {
        return m_resistance_to_side_2;
    } // get_resistance_to_side_2()

} // namespace Building_Physics
} // namespace BSO

#endif // BP_CONSTRUCTION_HPP

#ifndef BP_MODEL_HPP
#define BP_MODEL_HPP

#include <BSO/Building_Physics/BP_Simulation.hpp>
#include <BSO/Spatial_Design/Conformation.hpp>
#include <BSO/Visualisation/Model_Module/Model.hpp>
#include <BSO/Visualisation/BSP_Module/BSP.hpp>

#include <cstdlib>
#include <memory>
#include <vector>

namespace BSO { namespace Visualisation
{

    class BP_Model : public model
    {
    public:
        BP_Model(Building_Physics::BP_Simulation&);
        ~BP_Model();
        void render(const camera &cam) const;
        const std::string get_description();

    protected:

    private:
        std::list<polygon*> polygons;
        std::map<std::string, polygon_props> pplist;

        line_props     lprops;
//        label_props    lbprops;
        random_bsp     *pbsp;
    };





    // Implementation of member functions:

    // hh, below is the definition of the constructor of setofrooms_model as defined in setofrooms_model.h
    // hh, see datatypes.h for usage of pprops:
    // hh, rgba ambient,diffuse,specular, emission;
    // hh, float shininess;
    // hh, bool translucent, wosided;
    BP_Model::BP_Model(Building_Physics::BP_Simulation& BPS)
    {
        if (!BPS.visualisation_possible())
        { // the bp model was not initialised with a conformal model
            std::cout << "Visualisation of building physics model not possible, geometry data is not available (Visualisation/BP_Model.hpp)" << std::endl;
            exit(1);
        }
        else
        {
            std::vector<Building_Physics::BP_Vis_Setting> bp_vis_settings = BPS.get_vis_settings();
            for (auto ite : bp_vis_settings)
            {
                pplist[ite.m_ID] = polygon_props();
                pplist[ite.m_ID].ambient = rgba{ite.m_r, ite.m_g, ite.m_b, ite.m_alpha};
                pplist[ite.m_ID].diffuse = rgba{ite.m_r, ite.m_g, ite.m_b, ite.m_alpha};
                pplist[ite.m_ID].specular = rgba{ite.m_r, ite.m_g, ite.m_b, ite.m_alpha};
                pplist[ite.m_ID].shininess = 60;
                pplist[ite.m_ID].translucent = true;
                pplist[ite.m_ID].twosided = true;
            }
        }

        for (unsigned int i = 0; i < BPS.get_wall_count(); i++)
        {
            Spatial_Design::Geometry::Line* temp_ptr_1 = BPS.get_wall_ptr(i)->get_rectangle_ptr()->get_line_ptr(0);
            Spatial_Design::Geometry::Line* temp_ptr_2 = nullptr;

            Vectors::Vector vct_1 = temp_ptr_1->get_vertex_ptr(1)->get_coords() -
                                    temp_ptr_1->get_vertex_ptr(0)->get_coords();


            vertex v[4]; // namespace: Visualisation

            for (int j = 1; j < 4; j++)
            {
                temp_ptr_2 = BPS.get_wall_ptr(i)->get_rectangle_ptr()->get_line_ptr(j);

                Vectors::Vector vct_2 = temp_ptr_2->get_vertex_ptr(1)->get_coords() -
                                        temp_ptr_2->get_vertex_ptr(0)->get_coords();

                if (Vectors::is_paral(vct_1, vct_2, 0.01)) // if the two vectors are parallel
                {
                    Vectors::Vector vct_3 = temp_ptr_2->get_vertex_ptr(0)->get_coords() -
                                            temp_ptr_1->get_vertex_ptr(0)->get_coords();

                    Vectors::Point temp_coords;

                    if (Vectors::is_perp(vct_1, vct_3, 0.01)) // if the two vectors are perpendicular
                    {
                        temp_coords = temp_ptr_1->get_vertex_ptr(0)->get_coords();
                        v[1] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));

                        temp_coords = temp_ptr_1->get_vertex_ptr(1)->get_coords();
                        v[0] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));

                        temp_coords = temp_ptr_2->get_vertex_ptr(0)->get_coords();
                        v[2] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));

                        temp_coords = temp_ptr_2->get_vertex_ptr(1)->get_coords();
                        v[3] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));
                        break;
                    }
                    else
                    {
                        temp_coords = temp_ptr_1->get_vertex_ptr(0)->get_coords();
                        v[1] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));

                        temp_coords = temp_ptr_1->get_vertex_ptr(1)->get_coords();
                        v[0] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));

                        temp_coords = temp_ptr_2->get_vertex_ptr(0)->get_coords();
                        v[3] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));

                        temp_coords = temp_ptr_2->get_vertex_ptr(1)->get_coords();
                        v[2] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));
                        break;

                    }
                }
            }
            create_area(polygons, &pplist[BPS.get_wall_ptr(i)->get_vis_ID()], &lprops, v[0], v[1], v[2], v[3]);
        }

        for (unsigned int i = 0; i < BPS.get_floor_count(); i++)
        {
            Spatial_Design::Geometry::Line* temp_ptr_1 = BPS.get_floor_ptr(i)->get_rectangle_ptr()->get_line_ptr(0);
            Spatial_Design::Geometry::Line* temp_ptr_2 = nullptr;

            Vectors::Vector vct_1 = temp_ptr_1->get_vertex_ptr(1)->get_coords() -
                                    temp_ptr_1->get_vertex_ptr(0)->get_coords();

            vertex v[4]; // namespace: Visualisation

            for (int j = 1; j < 4; j++)
            {
                temp_ptr_2 = BPS.get_floor_ptr(i)->get_rectangle_ptr()->get_line_ptr(j);

                Vectors::Vector vct_2 = temp_ptr_2->get_vertex_ptr(1)->get_coords() -
                                        temp_ptr_2->get_vertex_ptr(0)->get_coords();


                if (Vectors::is_paral(vct_1, vct_2, 0.01)) // if the two vectors are parallel
                {
                    Vectors::Vector vct_3 = temp_ptr_2->get_vertex_ptr(0)->get_coords() -
                                            temp_ptr_1->get_vertex_ptr(0)->get_coords();

                    Vectors::Point temp_coords;

                    if (Vectors::is_perp(vct_1, vct_3, 0.01)) // if the two vectors are perpendicular
                    {
                        temp_coords = temp_ptr_1->get_vertex_ptr(0)->get_coords();
                        v[1] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));

                        temp_coords = temp_ptr_1->get_vertex_ptr(1)->get_coords();
                        v[0] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));

                        temp_coords = temp_ptr_2->get_vertex_ptr(0)->get_coords();
                        v[2] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));

                        temp_coords = temp_ptr_2->get_vertex_ptr(1)->get_coords();
                        v[3] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));
                        break;
                    }
                    else
                    {
                        temp_coords = temp_ptr_1->get_vertex_ptr(0)->get_coords();
                        v[1] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));

                        temp_coords = temp_ptr_1->get_vertex_ptr(1)->get_coords();
                        v[0] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));

                        temp_coords = temp_ptr_2->get_vertex_ptr(0)->get_coords();
                        v[3] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));

                        temp_coords = temp_ptr_2->get_vertex_ptr(1)->get_coords();
                        v[2] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));
                        break;
                    }
                }
            }
            create_area(polygons, &pplist[BPS.get_floor_ptr(i)->get_vis_ID()], &lprops, v[0], v[1], v[2], v[3]);
        }

        for (unsigned int i = 0; i < BPS.get_window_count(); i++)
        {
            Spatial_Design::Geometry::Line* temp_ptr_1 = BPS.get_window_ptr(i)->get_rectangle_ptr()->get_line_ptr(0);
            Spatial_Design::Geometry::Line* temp_ptr_2 = nullptr;
            Vectors::Vector vct_1 = temp_ptr_1->get_vertex_ptr(1)->get_coords() -
                                    temp_ptr_1->get_vertex_ptr(0)->get_coords();

            vertex v[4]; // namespace: Visualisation

            for (int j = 1; j < 4; j++)
            {
                temp_ptr_2 = BPS.get_window_ptr(i)->get_rectangle_ptr()->get_line_ptr(j);

                Vectors::Vector vct_2 = temp_ptr_2->get_vertex_ptr(1)->get_coords() -
                                        temp_ptr_2->get_vertex_ptr(0)->get_coords();


                if (Vectors::is_paral(vct_1, vct_2, 0.01)) // if the two vectors are parallel
                {
                    Vectors::Vector vct_3 = temp_ptr_2->get_vertex_ptr(0)->get_coords() -
                                            temp_ptr_1->get_vertex_ptr(0)->get_coords();

                    Vectors::Point temp_coords;

                    if (Vectors::is_perp(vct_1, vct_3, 0.01)) // if the two vectors are perpendicular
                    {
                        temp_coords = temp_ptr_1->get_vertex_ptr(0)->get_coords();
                        v[1] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));

                        temp_coords = temp_ptr_1->get_vertex_ptr(1)->get_coords();
                        v[0] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));

                        temp_coords = temp_ptr_2->get_vertex_ptr(0)->get_coords();
                        v[2] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));

                        temp_coords = temp_ptr_2->get_vertex_ptr(1)->get_coords();
                        v[3] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));
                        break;
                    }
                    else
                    {
                        temp_coords = temp_ptr_1->get_vertex_ptr(0)->get_coords();
                        v[1] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));

                        temp_coords = temp_ptr_1->get_vertex_ptr(1)->get_coords();
                        v[0] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));

                        temp_coords = temp_ptr_2->get_vertex_ptr(0)->get_coords();
                        v[3] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));

                        temp_coords = temp_ptr_2->get_vertex_ptr(1)->get_coords();
                        v[2] = vect3d(temp_coords(0), temp_coords(2), -temp_coords(1));
                        break;
                    }
                }
            }
            create_area(polygons, &pplist[BPS.get_window_ptr(i)->get_vis_ID()], &lprops, v[0], v[1], v[2], v[3]);
        }

        pbsp = new random_bsp(polygons);
    }

    BP_Model::~BP_Model()
    {
        delete pbsp;

        for (std::list<polygon*>::iterator pit = polygons.begin();
             pit != polygons.end(); pit++)
            delete *pit;
    }

    const std::string BP_Model::get_description()
    {
        return std::string("//Building Physics Design        Space: Next Step");
    }

    void BP_Model::render(const camera &cam) const
    {
        glPushAttrib(GL_ENABLE_BIT);

        glDisable(GL_DEPTH_TEST);

        pbsp->render_btf(cam);
/*
        std::list<label*>::const_iterator lbit;
        for (lbit = labels.begin(); lbit != labels.end(); lbit++)
            (*lbit)->render();
*/
        glPopAttrib();
    }

} // namespace Visualisation
} // namespace BSO


#endif // BP_MODEL_HPP

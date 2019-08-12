#ifndef VIS_VERTEX_STORE_HPP
#define VIS_VERTEX_STORE_HPP

#include <bso/visualization/utilities/datatypes.hpp>

#include <set>
#include <utility>

namespace bso { namespace visualization
{
    /*
       This class is used in the model and bsp classes
       to store the vertices and normal vectors. The
       the vect3d_set allows us to store vect3d pointers
       to unique vectors, such that no vector value is
       added twice.
     */
    struct vertex_store
    {
            std::set<vertex> vertices;
            std::set<normal> normals;

            /*
               these functions return a pointer to the
               vector in the sets, if the vector is not
               present it is added.
             */
            inline const vertex *get_vertex(const vertex &v) {
                //attempt to insert it into the set
                std::pair<std::set<vertex>::iterator, bool> res;
                res = vertices.insert(v);
                return &*res.first;
            }

            inline const normal *get_normal(const normal &n) {
                //attempt to insert it into the set
                std::pair<std::set<normal>::iterator, bool> res;
                res = normals.insert(n);
                return &*res.first;
            }

            inline void clear() {
                vertices.clear();
                normals.clear();
            }
    };

} // namespace visualization
} // namespace bso

#endif //VIS_VERTEX_STORE_HPP

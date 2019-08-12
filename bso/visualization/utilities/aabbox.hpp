#ifndef VIS_AABBOX_HPP
#define VIS_AABBOX_HPP

#include <iostream>
#include <ostream>
#include <limits>

namespace bso { namespace visualization
{
    //forward declarations
    template <typename T> struct vect3;
    template <typename T> struct mat4;
    template <typename T> struct obbox;

    //this structure defines an axis aligned bounding box and some
    //operations on them
    template <typename T>
    struct aabbox
    {
          vect3<T>  min, max;

          //the default constructor creates an empty box
          aabbox();
          aabbox(const vect3<T>& mi, const vect3<T>& ma) : min(mi), max(ma) {}

          const vect3<T> getcenter() const;
          const aabbox<T>& addmin(const vect3<T>& v);
          const aabbox<T>& addmax(const vect3<T>& v);
          const aabbox<T>& add(const vect3<T>& v);

          bool empty() const;
    };

    //output stream operator (debug printing)
      template <typename T>
    std::ostream& operator<<(std::ostream& os, const aabbox<T>& aabb);

    //template declaration
    #include <bso/visualization/utilities/vect3.hpp>
    template <typename T>
    aabbox<T>::aabbox()
    {
      float inf = std::numeric_limits<T>::infinity();
      min = vect3<T>(inf, inf, inf);
      max = vect3<T>(-inf, -inf, -inf);
    }

    template <typename T>
    const vect3<T> aabbox<T>::getcenter() const
    {
          return (min + max)/(T)2;
    }

    template <typename T>
    const aabbox<T>& aabbox<T>::addmin(const vect3<T>& v)
    {
          if (v.x < min.x) min.x = v.x;
          if (v.y < min.y) min.y = v.y;
          if (v.z < min.z) min.z = v.z;
          return *this;
    }

    template <typename T>
    const aabbox<T>& aabbox<T>::addmax(const vect3<T>& v)
    {
          if (v.x > max.x) max.x = v.x;
          if (v.y > max.y) max.y = v.y;
          if (v.z > max.z) max.z = v.z;
          return *this;
    }

    template <typename T>
    const aabbox<T>& aabbox<T>::add(const vect3<T>& v)
    {
        addmin(v);
        addmax(v);
        return *this;
    }

    template <typename T>
    bool aabbox<T>::empty() const
    {
          return !(min < max);
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const aabbox<T>& aabb)
    {
          return os << "[min(" << aabb.min.x << ", " << aabb.min.y << ", " << aabb.min.z <<
                 "), max (" << aabb.max.x << ", " << aabb.max.y << ", " << aabb.max.z << ")]";
    }


    //typedefs for convenience
    typedef aabbox<float> aabboxf;
    typedef aabbox<double> aabboxd;


} // namespace Visualisation
} // namespace BSO

#endif //VIS_AABBOX_HPP

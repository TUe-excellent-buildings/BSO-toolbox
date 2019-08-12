#ifndef VIS_VECT4_HPP
#define VIS_VECT4_HPP

#include <iostream>
#include <iomanip>
#include <sstream>
#include <math.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic" // shortly turn -Wpedantic compiler warnings off

namespace bso { namespace visualization
{

    //forward declaration
    template <typename T> struct vect3;

    /* template struct for a 4d vector with oprator overloads
     * for the most common operations on vectors */
    template <typename T> struct vect4
    {
          //vector
          union
          {
            //access the vector data as an array or use
            //the [] operator overload
            T v[4];

            //or as an unamed struct for coordinates
            struct
            {
              T  x, y, z, w;
            };

            //or as an unamed struct for colors
            struct
            {
              T  r, g, b, a;
            };
          };

          //constructors
          inline vect4();
          inline vect4(const T vx, const T vy, const T vz, const T vw = (T)1);
          inline vect4(const vect3<T>& v3);
          inline vect4(const vect4<T>& v4);

          //operator overloads

          //indexed access
          inline T& operator[] (int i);
          inline const T& operator[] (int i) const;

          //vector assignment
          inline vect4<T>& operator=(const vect3<T>& v);

          //vector comparison
          inline bool operator==(const vect4<T>& v) const;

          //scalar multiplication
          inline vect4<T>& operator*=(const T s);
          inline const vect4<T> operator*(const T s) const;

          //vector multiplication
          inline vect4<T>& operator*=(const vect4<T>& v);
          inline const vect4<T> operator*(const vect4<T>& v) const;

    //      inline vect4<T>& operator*=(const vect3<T>& v);
    //      inline const vect4<T> operator*(const vect3<T>& v) const;

          //scalar division
          inline vect4<T>& operator/=(const T s);
          inline const vect4<T> operator/(const T s) const;

          //vector division
          inline vect4<T>& operator/=(const vect4<T>& v);
          inline const vect4<T> operator/(const vect4<T>& v) const;

    //      inline vect4<T>& operator/=(const vect3<T>& v);
    //      inline const vect4<T> operator/(const vect3<T>& v) const;

          //vector addition
          inline vect4<T>& operator+=(const vect4<T>& v);
          inline const vect4<T> operator+(const vect4<T>& v) const;

    //      inline vect4<T>& operator+=(const vect3<T>& v);
    //      inline const vect4<T> operator+(const vect3<T>& v) const;

          //vector subtraction
          inline vect4<T>& operator-=(const vect4<T>& v);
          inline const vect4<T> operator-(const vect4<T>& v) const;

    //      inline vect4<T>& operator-=(const vect3<T>& v);
    //      inline const vect4<T> operator-(const vect3<T>& v) const;

          //unary operators
          //unary minus (inversion)
          inline const vect4<T> operator-() const;

          //lexicographic compare
          inline bool operator<(const vect4<T>& v) const;

          //dot product
          inline T dot(const vect4<T>& v) const;

          //cross product
          inline const vect4<T> cross(const vect4<T>& v) const;

          //normalises itself
          const vect4<T>& normalise();

          //returns a normalised temporary
          const vect4<T> normalise() const;
    };


    //functions on vect4

    //dot and cross product as binary functions
      template <typename T>
    inline T dot(const vect4<T>& v1, const vect4<T>& v2);

      template <typename T>
    inline const vect4<T> cross(const vect4<T>& v1, const vect4<T>& v2);

    //scalar vector multiplication operator (s * v)
      template <typename T>
    inline const vect4<T> operator*(const T s, const vect4<T>& v);

    //output stream operator (debug printing)
      template <typename T>
    std::ostream& operator<<(std::ostream& os, const vect4<T>& v);

    //include the template function definitions
    //constructors
    template <typename T>
    inline vect4<T>::vect4()
      : x((T)0), y((T)0), z((T)0), w((T)1) {}

    template <typename T>
    inline vect4<T>::vect4(const T vx, const T vy, const T vz, const T vw)
      : x(vx), y(vy), z(vz), w(vw) {}

    template <typename T>
    inline vect4<T>::vect4(const vect4<T>& v4)
      : x(v4.x), y(v4.y), z(v4.z), w(v4.w) {}

    template <typename T>
    inline vect4<T>::vect4(const vect3<T>& v3)
      : x(v3.x), y(v3.y), z(v3.z), w((T)1) {}

    //operator overloads
    template <typename T>
    inline T& vect4<T>::operator[] (int i) { return v[i]; }

    template <typename T>
    inline const T& vect4<T>::operator[] (int i) const { return v[i]; }

    //vector assignment
    template <typename T>
    inline vect4<T>& vect4<T>::operator=(const vect3<T>& v)
    {
          x = v.x; y =v.y; z = v.z; w = 1.0; return *this;
    }

    //vector comparison
    template <typename T>
    inline bool vect4<T>::operator==(const vect4<T>& v) const
    {
          return x == v.x && y == v.y && z == v.z && w == v.w;
    }

    //scalar multiplication
    template <typename T>
    inline vect4<T>& vect4<T>::operator*=(const T s)
    {
          x *= s; y *= s; z *= s; w *= s; return *this;
    }

    template <typename T>
    inline const vect4<T> vect4<T>::operator*(const T s) const
    {
          vect4<T> r(*this); return r *= s;
    }

    //vector multiplication
    template <typename T>
    inline vect4<T>& vect4<T>::operator*=(const vect4<T>& v)
    {
          x *= v.x; y *=v.y; z *= v.z; w *= v.w; return *this;
    }

    template <typename T>
    inline const vect4<T> vect4<T>::operator*(const vect4<T>& v) const
    {
          vect4<T> r(*this); return r *= v;
    }

    //scalar division
    template <typename T>
    inline vect4<T>& vect4<T>::operator/=(const T s)
    {
          x /= s; y /= s; z /= s; w /= s; return *this;
    }

    template <typename T>
    inline const vect4<T> vect4<T>::operator/(const T s) const
    {
          vect4<T> r(*this); return r /= s;
    }

    //vector division
    template <typename T>
    inline vect4<T>& vect4<T>::operator/=(const vect4<T>& v)
    {
          x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this;
    }

    template <typename T>
    inline const vect4<T> vect4<T>::operator/(const vect4<T>& v) const
    {
          vect4<T> r(*this); return r /= v;
    }

    //vector addition
    template <typename T>
    inline vect4<T>& vect4<T>::operator+=(const vect4<T>& v)
    {
          x += v.x; y += v.y; z += v.z; w += v.w; return *this;
    }

    template <typename T>
    inline const vect4<T> vect4<T>::operator+(const vect4<T>& v) const
    {
          vect4<T> r(*this); return r += v;
    }

    //vector subtraction
    template <typename T>
    inline vect4<T>& vect4<T>::operator-=(const vect4<T>& v)
    {
          x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this;
    }

    template <typename T>
    inline const vect4<T> vect4<T>::operator-(const vect4<T>& v) const
    {
          vect4<T> r(*this); return r -= v;
    }

    //unary operators
    //unary minus (inversion)
    template <typename T>
    inline const vect4<T> vect4<T>::operator-() const
    {
          return vect4<T>((T)0, (T)0, (T)0, (T)0) - *this;
    }

    //lexicographic compare
    template <typename T>
    inline bool vect4<T>::operator<(const vect4<T>& v) const
    {
        if (x<v.x) return true;
        if (x>v.x) return false;
        if (y<v.y) return true;
        if (y>v.y) return false;
        if (z<v.z) return true;
        if (z>v.z) return false;
        if (w<v.w) return true;
        return false;
    }

    //dot product
    template <typename T>
    inline T vect4<T>::dot(const vect4<T>& v) const
    {
          return (x*v.x) + (y*v.y) + (z*v.z);
    }

    //cross product
    template <typename T>
    inline const vect4<T> vect4<T>::cross(const vect4<T>& v) const
    {
          return vect4<T>((y*v.z) - (z*v.y),
                          (z*v.x) - (x*v.z),
                          (x*v.y) - (y*v.x));
    }

    //normalises itself
    template <typename T>
    const vect4<T>& vect4<T>::normalise()
    {
          T l = sqrt((x*x) + (y*y) + (z*z));

          if (l != (T)0)
          {
            *this /= l;
          }

          return *this;
    }

    //returns a normalised temporary
    template <typename T>
    const vect4<T> vect4<T>::normalise() const
    {
          vect4<T> c(*this);
          T l = sqrt((x*x) + (y*y) + (z*z));

          if (l != (T)0)
          {
            c /= l;
          }

          return c;
    }


    //functions on vect4

    //dot and cross product as binary functions
    template <typename T>
    inline T dot(const vect4<T>& v1, const vect4<T>& v2)
    {
          return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
    }

    template <typename T>
    inline const vect4<T> cross(const vect4<T>& v1, const vect4<T>& v2)
    {
          return vect4<T>((v1.y*v2.z) - (v1.z*v2.y),
                          (v1.z*v2.x) - (v1.x*v2.z),
                          (v1.x*v2.y) - (v1.y*v2.x));
    }

    //scalar vector multiplication operator (s * v)
    template <typename T>
    inline const vect4<T> operator*(const T s, const vect4<T>& v)
    {
          return v*s;
    }


    //output stream operator (debug printing)
    template <typename T>
    std::ostream& operator<<(std::ostream& os, const vect4<T>& v)
    {
          os << std::fixed << std::setprecision(2) <<
            "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")" <<
        std::resetiosflags(std::ios_base::fixed);

          return os;
    }

    //typedef for instantiated templates
    typedef vect4<float>  vect4f;
    typedef vect4<double> vect4d;

} // namespace visualization
} // namespace bso

#pragma GCC diagnostic pop // turn -Wpedantic warnings back on

#endif //VIS_VECT4_HPP

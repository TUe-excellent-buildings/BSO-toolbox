#ifndef VIS_VECT3_HPP
#define VIS_VECT3_HPP

#include <Eigen/Dense>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <math.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic" // shortly turn -Wpedantic compiler warnings off

namespace bso { namespace visualization
{
    //forward declaration
    template <typename T> struct vect4;

    /* template struct for a 3d vector with oprator overloads
     * for the most common operations on vectors */
    template <typename T> struct vect3
    {
          //vector
          union
          {
            //access the vector data as an array or use
            //the [] operator overload
            T v[3];

            //or as an unamed struct for coordinates
            struct
            {
              T  x, y, z;
            };

            //or as an unamed struct for colors
            struct
            {
              T  r, g, b;
            };
          };

          //constructors
          inline vect3();
          inline vect3(const T vx, const T vy, const T vz);
          inline vect3(const vect3<T>& v3);
          inline vect3(const vect4<T>& v4);
					inline vect3(const Eigen::Matrix<T,3,1>& rhs);

          //operator overloads

          //indexed access
          inline T& operator[] (int i);
          inline const T& operator[] (int i) const;

          //vector assignment
          inline vect3<T>& operator=(const vect3<T>& v);
          inline vect3<T>& operator=(const vect4<T>& v);

          //vector comparison
          inline bool operator==(const vect3<T>& v) const;

          //scalar multiplication
          inline vect3<T>& operator*=(const T s);
          inline const vect3<T> operator*(const T s) const;

          //vector multiplication
          inline vect3<T>& operator*=(const vect3<T>& v);
          inline const vect3<T> operator*(const vect3<T>& v) const;

          inline vect3<T>& operator*=(const vect4<T>& v);
          inline const vect3<T> operator*(const vect4<T>& v) const;

          //scalar division
          inline vect3<T>& operator/=(const T s);
          inline const vect3<T> operator/(const T s) const;

          //vector division
          inline vect3<T>& operator/=(const vect3<T>& v);
          inline const vect3<T> operator/(const vect3<T>& v) const;

          inline vect3<T>& operator/=(const vect4<T>& v);
          inline const vect3<T> operator/(const vect4<T>& v) const;

          //vector addition
          inline vect3<T>& operator+=(const vect3<T>& v);
          inline const vect3<T> operator+(const vect3<T>& v) const;

          inline vect3<T>& operator+=(const vect4<T>& v);
          inline const vect3<T> operator+(const vect4<T>& v) const;

          //vector subtraction
          inline vect3<T>& operator-=(const vect3<T>& v);
          inline const vect3<T> operator-(const vect3<T>& v) const;

          inline vect3<T>& operator-=(const vect4<T>& v);
          inline const vect3<T> operator-(const vect4<T>& v) const;

          //unary operators
          //unary minus (inversion)
          inline const vect3<T> operator-() const;

          //lexicographic compare
          inline bool operator<(const vect3<T>& v) const;
          inline bool operator>(const vect3<T>& v) const;

          //dot product
          inline T dot(const vect3<T>& v) const;

          //cross product
          inline const vect3<T> cross(const vect3<T>& v) const;

          //returns the length of the vector
          inline T length() const;

          //normalises itself
          const vect3<T>& normalise();

          //returns a normalised temporary
          const vect3<T> normalise() const;
    };


    //functions on vect3

    //dot and cross product as binary functions
      template <typename T>
    inline T dot(const vect3<T>& v1, const vect3<T>& v2);

      template <typename T>
    inline const vect3<T> cross(const vect3<T>& v1, const vect3<T>& v2);

    //scalar vector multiplication operator (s * v)
      template <typename T>
    inline const vect3<T> operator*(const T s, const vect3<T>& v);

    //the min function returns a vector in which each coordinate
    //is the minum between the coordinates of l and r
    template <typename T>
    const vect3<T> min(const vect3<T> &l, const vect3<T> &r);
    template <typename T>
    const vect3<T> max(const vect3<T> &l, const vect3<T> &r);


    //output stream operator (debug printing)
      template <typename T>
    std::ostream& operator<<(std::ostream& os, const vect3<T>& v);

    //include the template function definitions
    /*
       template struct for a 3d vector with oprator overloads
       for the most common operations on vectors
     */

    //constructors
    template <typename T>
    inline vect3<T>::vect3()
      : x((T)0), y((T)0), z((T)0) {}

    template <typename T>
    inline vect3<T>::vect3(const T vx, const T vy, const T vz)
      : x(vx), y(vy), z(vz) {}

    template <typename T>
    inline vect3<T>::vect3(const vect3<T>& v3)
      : x(v3.x), y(v3.y), z(v3.z) {}

    template <typename T>
    inline vect3<T>::vect3(const vect4<T>& v4)
      : x(v4.x), y(v4.y), z(v4.z) {}
		
		template <typename T>
    inline vect3<T>::vect3(const Eigen::Matrix<T,3,1>& rhs)
      : x(rhs(0)), y(rhs(2)), z(-rhs(1)) {}
			
    //operator overloads

    //indexed access
    template <typename T>
    inline T& vect3<T>::operator[] (int i) { return v[i]; }

    //indexed access (const)
    template <typename T>
    inline const T& vect3<T>::operator[] (int i) const { return v[i]; }

    //vector assignment
    template <typename T>
    inline vect3<T>& vect3<T>::operator=(const vect3<T>& v)
    {
          x = v.x; y =v.y; z = v.z; return *this;
    }

    template <typename T>
    inline vect3<T>& vect3<T>::operator=(const vect4<T>& v)
    {
          x = v.x; y =v.y; z = v.z; return *this;
    }

    //vector comparison
    template <typename T>
    inline bool vect3<T>::operator==(const vect3<T>& v) const
    {
          return x == v.x && y == v.y && z == v.z;
    }

    //scalar multiplication
    template <typename T>
    inline vect3<T>& vect3<T>::operator*=(const T s)
    {
          x *= s; y *= s; z *= s; return *this;
    }

    template <typename T>
    inline const vect3<T> vect3<T>::operator*(const T s) const
    {
          vect3<T> r(*this); return r *= s;
    }

    //vector multiplication
    template <typename T>
    inline vect3<T>& vect3<T>::operator*=(const vect3<T>& v)
    {
          x *= v.x; y *=v.y; z *= v.z; return *this;
    }

    template <typename T>
    inline const vect3<T> vect3<T>::operator*(const vect3<T>& v) const
    {
          vect3<T> r(*this); return r *= v;
    }

    template <typename T>
    inline vect3<T>& vect3<T>::operator*=(const vect4<T>& v)
    {
          x *= v.x; y *=v.y; z *= v.z; return *this;
    }

    template <typename T>
    inline const vect3<T> vect3<T>::operator*(const vect4<T>& v) const
    {
          vect3<T> r(*this); return r *= v;
    }

    //scalar division
    template <typename T>
    inline vect3<T>& vect3<T>::operator/=(const T s)
    {
          x /= s; y /= s; z /= s; return *this;
    }

    template <typename T>
    inline const vect3<T> vect3<T>::operator/(const T s) const
    {
          vect3<T> r(*this); return r /= s;
    }

    //vector division
    template <typename T>
    inline vect3<T>& vect3<T>::operator/=(const vect3<T>& v)
    {
          x /= v.x; y /= v.y; z /= v.z; return *this;
    }

    template <typename T>
    inline const vect3<T> vect3<T>::operator/(const vect3<T>& v) const
    {
          vect3<T> r(*this); return r /= v;
    }

    template <typename T>
    inline vect3<T>& vect3<T>::operator/=(const vect4<T>& v)
    {
          x /= v.x; y /= v.y; z /= v.z; return *this;
    }

    template <typename T>
    inline const vect3<T> vect3<T>::operator/(const vect4<T>& v) const
    {
          vect3<T> r(*this); return r /= v;
    }

    //vector addition
    template <typename T>
    inline vect3<T>& vect3<T>::operator+=(const vect3<T>& v)
    {
          x += v.x; y += v.y; z += v.z; return *this;
    }

    template <typename T>
    inline const vect3<T> vect3<T>::operator+(const vect3<T>& v) const
    {
          vect3<T> r(*this); return r += v;
    }


    template <typename T>
    inline vect3<T>& vect3<T>::operator+=(const vect4<T>& v)
    {
          x += v.x; y += v.y; z += v.z; return *this;
    }

    template <typename T>
    inline const vect3<T> vect3<T>::operator+(const vect4<T>& v) const
    {
          vect3<T> r(*this); return r += v;
    }

    //vector subtraction
    template <typename T>
    inline vect3<T>& vect3<T>::operator-=(const vect3<T>& v)
    {
          x -= v.x; y -= v.y; z -= v.z; return *this;
    }

    template <typename T>
    inline const vect3<T> vect3<T>::operator-(const vect3<T>& v) const
    {
          vect3<T> r(*this); return r -= v;
    }

    template <typename T>
    inline vect3<T>& vect3<T>::operator-=(const vect4<T>& v)
    {
          x -= v.x; y -= v.y; z -= v.z; return *this;
    }

    template <typename T>
    inline const vect3<T> vect3<T>::operator-(const vect4<T>& v) const
    {
          vect3<T> r(*this); return r -= v;
    }

    //unary operators
    //unary minus (inversion)
    template <typename T>
    inline const vect3<T> vect3<T>::operator-() const
    {
          return vect3<T>((T)0, (T)0, (T)0) - *this;
    }

    //lexicographic compare (less)
    template <typename T>
    inline bool vect3<T>::operator<(const vect3<T>& v) const
    {
        if (x<v.x) return true;
        if (x>v.x) return false;
        if (y<v.y) return true;
        if (y>v.y) return false;
        if (z<v.z) return true;
        return false;
    }

    //lexicographic compare (greater)
    template <typename T>
    inline bool vect3<T>::operator>(const vect3<T>& v) const
    {
        if (x>v.x) return true;
        if (x<v.x) return false;
        if (y>v.y) return true;
        if (y<v.y) return false;
        if (z>v.z) return true;
        return false;
    }

    //dot product
    template <typename T>
    inline T vect3<T>::dot(const vect3<T>& v) const
    {
          return (x*v.x) + (y*v.y) + (z*v.z);
    }

    //cross product
    template <typename T>
    inline const vect3<T> vect3<T>::cross(const vect3<T>& v) const
    {
          return vect3<T>((y*v.z) - (z*v.y),
                          (z*v.x) - (x*v.z),
                          (x*v.y) - (y*v.x));
    }

    template <typename T>
    inline T vect3<T>::length() const
    {
          return (T)sqrt(x*x + y*y + z*z);
    }

    //normalises itself
    template <typename T>
    const vect3<T>& vect3<T>::normalise()
    {
          T l = sqrt((x*x) + (y*y) + (z*z));

          if (l != (T)0)
          {
            *this /= l;
          }

          return *this;
    }

    //returns a normalised temporary
    //NOTE: this is confusing and inconsistent with the function above
    template <typename T>
    const vect3<T> vect3<T>::normalise() const
    {
          vect3<T> c(*this);
          T l = sqrt((x*x) + (y*y) + (z*z));

          if (l != (T)0)
          {
            c /= l;
          }

          return c;
    }


    //functions on vect3

    //dot and cross product as binary functions
    template <typename T>
    inline T dot(const vect3<T>& v1, const vect3<T>& v2)
    {
          return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
    }

    template <typename T>
    inline const vect3<T> cross(const vect3<T>& v1, const vect3<T>& v2)
    {
          return vect3<T>((v1.y*v2.z) - (v1.z*v2.y),
                          (v1.z*v2.x) - (v1.x*v2.z),
                          (v1.x*v2.y) - (v1.y*v2.x));
    }

    //scalar vector multiplication operator (s * v)
    template <typename T>
    inline const vect3<T> operator*(const T s, const vect3<T>& v)
    {
          return v*s;
    }

    //the min function returns a vector in which each coordinate
    //is the minum between the coordinates of l and r
    template <typename T>
    const vect3<T> min(const vect3<T> &l, const vect3<T> &r)
    {
          return vect3<T>(l.x<r.x? l.x: r.x,
                          l.y<r.y? l.y: r.y,
                          l.z<r.z? l.z: r.z);

    }

    template <typename T>
    const vect3<T> max(const vect3<T> &l, const vect3<T> &r)
    {
          return vect3<T>(l.x>r.x? l.x: r.x,
                          l.y>r.y? l.y: r.y,
                          l.z>r.z? l.z: r.z);

    }

    //output stream operator (debug printing)
    template <typename T>
    std::ostream& operator<<(std::ostream& os, const vect3<T>& v)
    {
          os << std::fixed << std::setprecision(2) <<
            "(" << v.x << ", " << v.y << ", " << v.z << ")" <<
        std::resetiosflags(std::ios_base::fixed);

          return os;
    }

    //typedef for instantiated templates
    typedef vect3<float>  vect3f;
    typedef vect3<double> vect3d;


} // namespace visualization
} // namespace bso


#pragma GCC diagnostic pop // turn -Wpedantic warnings back on
#endif //VIS_VECT3_HPP

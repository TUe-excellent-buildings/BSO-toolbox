#ifndef VIS_MAT4_HPP
#define VIS_MAT4_HPP

#include <bso/visualization/utilities/vect4.hpp>

#define _USE_MATH_DEFINES

#include <math.h>
#include <ostream>

namespace bso { namespace visualization
{

    //forward declarations
    template <typename T>
    struct vect3;

    template <typename T>
    struct vect4;

    template <typename T>
    struct ray;

    template <typename T>
    struct mat4
    {
          T M[16];

          //index operator, access the matrix values in column major order
          inline T& operator[] (int i) { return M[i]; }
          inline const T& operator[] (int i) const { return M[i]; }

          //matrix multiplication
          const mat4<T> operator*(const mat4<T>& R) const;
          const mat4<T>& operator*=(const mat4<T>& R);

          //these static functions return an initialised matrix
          static const mat4<T> id();
          //translation matrix and inverse
          static const mat4<T> trans(T x, T y, T z);
          static const mat4<T> transinv(T x, T y, T z);
          inline static const mat4<T> trans(const vect3<T> &v) { return trans(v.x, v.y, v.z); }
          inline static const mat4<T> transinv(const vect3<T> &v) { return transinv(v.x, v.y, v.z); }
          //rotation around oigin, rotation around arbitrary point
          //and their inverses
          static const mat4<T> rotx(T xrad);
          static const mat4<T> rotxp(T xrad, vect3<T> p);
          static const mat4<T> rotxinv(T xrad);
          static const mat4<T> rotxpinv(T xrad, vect3<T> p);
          static const mat4<T> roty(T xrad);
          static const mat4<T> rotyp(T xrad, vect3<T> p);
          static const mat4<T> rotyinv(T xrad);
          static const mat4<T> rotypinv(T xrad, vect3<T> p);
          static const mat4<T> rotz(T xrad);
          static const mat4<T> rotzp(T xrad, vect3<T> p);
          static const mat4<T> rotzinv(T xrad);
          static const mat4<T> rotzpinv(T xrad, vect3<T> p);
          //rotate around arbitrary axis
          static const mat4<T> rotate(T rad, T x, T y, T z);
          //scale around origin, scale around arbitrary point and
          //their inverses
          static const mat4<T> scale(T x, T y, T z);
          static const mat4<T> scalep(T x, T y, T z, vect3<T> p);
          static const mat4<T> scaleinv(T x, T y, T z);
          static const mat4<T> scalepinv(T x, T y, T z, vect3<T> p);
          //shear matrices
          static const mat4<T> shearxy(T a, T b);
          static const mat4<T> shearxyp(T a, T b, vect3<T> p);
          static const mat4<T> shearxyinv(T a, T b);
          static const mat4<T> shearxypinv(T a, T b, vect3<T> p);
          static const mat4<T> shearxz(T a, T b);
          static const mat4<T> shearxzp(T a, T b, vect3<T> p);
          static const mat4<T> shearxzinv(T a, T b);
          static const mat4<T> shearxzpinv(T a, T b, vect3<T> p);
          static const mat4<T> shearyz(T a, T b);
          static const mat4<T> shearyzp(T a, T b, vect3<T> p);
          static const mat4<T> shearyzinv(T a, T b);
          static const mat4<T> shearyzpinv(T a, T b, vect3<T> p);
          //lookat matrix
          static const mat4<T> lookat();
          static const mat4<T> lookatinv();
          //projection matrices
          static const mat4<T> ortho(T l, T r, T b, T t, T n, T f);
          static const mat4<T> orthoinv(T l, T r, T b, T t, T n, T f);
          static const mat4<T> frustum(T l, T r, T b, T t, T n, T f);
          static const mat4<T> frustuminv(T l, T r, T b, T t, T n, T f);


          //multiply with vector: r = M * v,
          //for direction vectors, set w = 0 to multiply by the upper
          //3x3 submatrix
          const vect4<T> operator*(const vect4<T>& v) const;
          //multiply with vect3, assume w = 1
          const vect3<T> operator*(const vect3<T>& v) const;

          //normal vectors are not the same as points or direction vectors,
          //in the sense that they are not defined by a difference in points but
          //as the vector perpendicular to all tangents at a point.
          //instead of transforming a normal M * n, transform the normal by
          //the upper 3x3 submatrix of the inverse transpose transformation
          //matrix:
          //  (M^-1)^T * n
          //
          //this matrix struct does not contain a general inverse function,
          //therefore construct the inverse using the inverse transformations
          //and transform normals using transmultv on the inverse matrix
          const vect4<T> transmultv(const vect4<T>& v) const;

    };

    //transforms vector v by matrix M and returns a reference to v
    template <typename T>
    const vect4<T>& operator*=(vect4<T>& v, const mat4<T>& M);
    template <typename T>
    const vect3<T>& operator*=(vect3<T>& v, const mat4<T>& M);

    //transforms ray r by matrix M and returns a reference to r
    template <typename T>
    inline const ray<T> operator*(const mat4<T>& M, const ray<T>& r);

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const mat4<T>& M);

    //template implementation
    /*
       This file implements a 4x4 matrix stored in column-major order (shown below),
       which is the order OpenGL expects for glLoadMatrix() and glMultMatrix()

       | 0  4  8 12|
       | 1  5  9 13|
       | 2  6 10 14|
       | 3  7 11 15|
       */


    //identity matrix
      template <typename T>
    const mat4<T> mat4<T>::id()
    {
          mat4<T> m;

          m[0] = (T)1; m[4] = (T)0; m[8]  = (T)0; m[12] = (T)0;
          m[1] = (T)0; m[5] = (T)1; m[9]  = (T)0; m[13] = (T)0;
          m[2] = (T)0; m[6] = (T)0; m[10] = (T)1; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

    //translation matrix and inverse
      template <typename T>
    const mat4<T> mat4<T>::trans(T x, T y, T z)
    {
          mat4<T> m;

          m[0] = (T)1; m[4] = (T)0; m[8]  = (T)0; m[12] = x;
          m[1] = (T)0; m[5] = (T)1; m[9]  = (T)0; m[13] = y;
          m[2] = (T)0; m[6] = (T)0; m[10] = (T)1; m[14] = z;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::transinv(T x, T y, T z)
    {
          mat4<T> m;

          m[0] = (T)1; m[4] = (T)0; m[8]  = (T)0; m[12] = -x;
          m[1] = (T)0; m[5] = (T)1; m[9]  = (T)0; m[13] = -y;
          m[2] = (T)0; m[6] = (T)0; m[10] = (T)1; m[14] = -z;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

    //rotation
      template <typename T>
    const mat4<T> mat4<T>::rotx(T xrad)
    {
          T       c1 = (T)cos(xrad),
                  s1 = (T)sin(xrad);
          mat4<T> m;

          m[0] = (T)1; m[4] = (T)0; m[8]  = (T)0; m[12] = (T)0;
          m[1] = (T)0; m[5] =   c1; m[9]  =  -s1; m[13] = (T)0;
          m[2] = (T)0; m[6] =   s1; m[10] =   c1; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::rotxp(T xrad, vect3<T> p)
    {
          /*( ( trans(p) * rotx(1) ) * trans(-p) )
            |1,  0,   0,                      -px + px|
            |0, c1, -s1, (c1 * -py) + (-s1 * -pz) + py|
            |0, s1,  c1,  (s1 * -py) + (c1 * -pz) + pz|
            |0,  0,   0,                             1|
            */
          T       c1 = (T)cos(xrad),
                  s1 = (T)sin(xrad);
          mat4<T> m;

          m[0] = (T)1; m[4] = (T)0; m[8]  = (T)0; m[12] = (T)0;
          m[1] = (T)0; m[5] =   c1; m[9]  =  -s1; m[13] = (c1 * -p.y) + (-s1 * -p.z) + p.y;
          m[2] = (T)0; m[6] =   s1; m[10] =   c1; m[14] = (s1 * -p.y) + (c1 * -p.z) + p.z;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::rotxinv(T xrad)
    {
          T       c1 = (T)cos(xrad),
                  s1 = (T)sin(xrad);
          mat4<T> m;

          m[0] = (T)1; m[4] = (T)0; m[8]  = (T)0; m[12] = (T)0;
          m[1] = (T)0; m[5] =   c1; m[9]  =   s1; m[13] = (T)0;
          m[2] = (T)0; m[6] =  -s1; m[10] =   c1; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::rotxpinv(T xrad, vect3<T> p)
    {
          /*
             ( ( transinv(-p) * rotxinv(1) ) * transinv(p) )
             |1,   0,  0,                      -px + --px|
             |0,  c1, s1,  (c1 * -py) + (s1 * -pz) + --py|
             |0, -s1, c1, (-s1 * -py) + (c1 * -pz) + --pz|
             |0,   0,  0,                               1|
             */
          T       c1 = (T)cos(xrad),
                  s1 = (T)sin(xrad);
          mat4<T> m;

          m[0] = (T)1; m[4] = (T)0; m[8]  = (T)0; m[12] = (T)0;
          m[1] = (T)0; m[5] =   c1; m[9]  =   s1; m[13] = (c1 * -p.y) + (s1 * -p.z) + p.y;
          m[2] = (T)0; m[6] =  -s1; m[10] =   c1; m[14] = (-s1 * -p.y) + (c1 * -p.z) + p.z;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::roty(T yrad)
    {
          T       c1 = (T)cos(yrad),
                  s1 = (T)sin(yrad);
          mat4<T> m;

          m[0] =   c1; m[4] = (T)0; m[8]  =   s1; m[12] = (T)0;
          m[1] = (T)0; m[5] = (T)1; m[9]  = (T)0; m[13] = (T)0;
          m[2] =  -s1; m[6] = (T)0; m[10] =   c1; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::rotyp(T yrad, vect3<T> p)
    {
          /*( ( trans(p) * roty(1) ) * trans(-p) )
            | c1, 0, s1,  (c1 * -px) + (s1 * -pz) + px|
            |  0, 1,  0,                      -py + py|
            |-s1, 0, c1, (-s1 * -px) + (c1 * -pz) + pz|
            |  0, 0,  0,                             1|*/
          T       c1 = (T)cos(yrad),
                  s1 = (T)sin(yrad);
          mat4<T> m;

          m[0] =   c1; m[4] = (T)0; m[8]  =   s1; m[12] = (c1 * -p.x) + (s1 * -p.z) + p.x;
          m[1] = (T)0; m[5] = (T)1; m[9]  = (T)0; m[13] = (T)0;
          m[2] =  -s1; m[6] = (T)0; m[10] =   c1; m[14] = (-s1 * -p.x) + (c1 * -p.z) + p.z;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::rotyinv(T yrad)
    {
          T       c1 = (T)cos(yrad),
                  s1 = (T)sin(yrad);
          mat4<T> m;

          m[0] =   c1; m[4] = (T)0; m[8]  =  -s1; m[12] = (T)0;
          m[1] = (T)0; m[5] = (T)1; m[9]  = (T)0; m[13] = (T)0;
          m[2] =   s1; m[6] = (T)0; m[10] =   c1; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::rotypinv(T yrad, vect3<T> p)
    {
          /*
             ( ( transinv(-p) * rotyinv(1) ) * transinv(p) )
             |c1, 0, -s1, (c1 * -px) + (-s1 * -pz) + --px|
             | 0, 1,   0,                      -py + --py|
             |s1, 0,  c1,  (s1 * -px) + (c1 * -pz) + --pz|
             | 0, 0,   0,                               1| */
          T       c1 = (T)cos(yrad),
                  s1 = (T)sin(yrad);
          mat4<T> m;

          m[0] =   c1; m[4] = (T)0; m[8]  =  -s1; m[12] = (c1 * -p.x) + (-s1 * -p.z) + p.x;
          m[1] = (T)0; m[5] = (T)1; m[9]  = (T)0; m[13] = (T)0;
          m[2] =   s1; m[6] = (T)0; m[10] =   c1; m[14] = (s1 * -p.x) + (c1 * -p.z) + p.z;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::rotz(T zrad)
    {
          T       c1 = (T)cos(zrad),
                  s1 = (T)sin(zrad);
          mat4<T> m;

          m[0] =   c1; m[4] =  -s1; m[8]  = (T)0; m[12] = (T)0;
          m[1] =   s1; m[5] =   c1; m[9]  = (T)0; m[13] = (T)0;
          m[2] = (T)0; m[6] = (T)0; m[10] = (T)1; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::rotzp(T zrad, vect3<T> p)
    {
          /*( ( trans(p) * rotz(1) ) * trans(-p) )
            |c1, -s1, 0, (c1 * -px) + (-s1 * -py) + px|
            |s1,  c1, 0,  (s1 * -px) + (c1 * -py) + py|
            | 0,   0, 1,                      -pz + pz|
            | 0,   0, 0,                             1|*/
          T       c1 = (T)cos(zrad),
                  s1 = (T)sin(zrad);
          mat4<T> m;

          m[0] =   c1; m[4] =  -s1; m[8]  = (T)0; m[12] = (c1 * -p.x) + (-s1 * -p.y) + p.x;
          m[1] =   s1; m[5] =   c1; m[9]  = (T)0; m[13] = (s1 * -p.x) + (c1 * -p.y) + p.y;
          m[2] = (T)0; m[6] = (T)0; m[10] = (T)1; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::rotzinv(T zrad)
    {
          T       c1 = (T)cos(zrad),
                  s1 = (T)sin(zrad);
          mat4<T> m;

          m[0] =   c1; m[4] =   s1; m[8]  = (T)0; m[12] = (T)0;
          m[1] =  -s1; m[5] =   c1; m[9]  = (T)0; m[13] = (T)0;
          m[2] = (T)0; m[6] = (T)0; m[10] = (T)1; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::rotzpinv(T zrad, vect3<T> p)
    {
          /* ( ( transinv(-p) * rotzinv(1) ) * transinv(p) )
             | c1, s1, 0,  (c1 * -px) + (s1 * -py) + --px|
             |-s1, c1, 0, (-s1 * -px) + (c1 * -py) + --py|
             |  0,  0, 1,                      -pz + --pz|
             |  0,  0, 0,                               1| */
          T       c1 = (T)cos(zrad),
                  s1 = (T)sin(zrad);
          mat4<T> m;

          m[0] =   c1; m[4] =   s1; m[8]  = (T)0; m[12] = (c1 * -p.x) + (s1 * -p.y) + p.x;
          m[1] =  -s1; m[5] =   c1; m[9]  = (T)0; m[13] = (-s1 * -p.x) + (c1 * -p.y) + p.y;
          m[2] = (T)0; m[6] = (T)0; m[10] = (T)1; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

    template <typename T>
    const mat4<T> mat4<T>::rotate(T rad, T x, T y, T z)
    {
        T l2 = x*x + y*y + z*z;
        if (l2 != 1.0) {
            //normalise (x,y,z)
            T l = sqrt(l2);
            x /= l; y /= l; z /= l;
        }

          /*
             | x^2(1-c)+c, xy(1-c)-zs, xz(1-c)+ys, 0 |
             | yx(1-c)+zs, y^2(1-c)+c, yz(1-c)-xs, 0 |
             | xz(1-c)-ys, yz(1-c)+xs, z^2(1-c)+c, 0 |
             |          0,          0,          0, 1 |
           */
          T       c = (T)cos(rad),
              c1 = 1.0 - c,
                  s = (T)sin(rad);
          mat4<T> m;

          m[0] = x*x*c1+c;   m[4] = x*y*c1-z*s; m[8] = x*z*c1+y*s; m[12] = (T)0;
          m[1] = y*x*c1+z*s; m[5] = y*y*c1+c;   m[9] = y*z*c1-x*s; m[13] = (T)0;
          m[2] = x*z*c1-y*s; m[6] = y*z*c1+x*s; m[10] = z*z*c1+c;  m[14] = (T)0;
          m[3] = (T)0;       m[7] = (T)0;       m[11] = (T)0;      m[15] = (T)1;

          return m;
    }

    //scale around origin, scale around arbitrary point and
    //their inverses
      template <typename T>
    const mat4<T> mat4<T>::scale(T sx, T sy, T sz)
    {
          mat4<T> m;

          m[0] =   sx; m[4] = (T)0; m[8]  = (T)0; m[12] = (T)0;
          m[1] = (T)0; m[5] =   sy; m[9]  = (T)0; m[13] = (T)0;
          m[2] = (T)0; m[6] = (T)0; m[10] =   sz; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::scalep(T sx, T sy, T sz, vect3<T> p)
    {
          /*( ( trans(p) * scale(s) ) * trans(-p) )
            |sx,  0,  0, (sx * -px) + px|
            | 0, sy,  0, (sy * -py) + py|
            | 0,  0, sz, (sz * -pz) + pz|
            | 0,  0,  0,               1|*/
          mat4<T> m;

          m[0] =   sx; m[4] = (T)0; m[8]  = (T)0; m[12] = (sx * -p.x) + p.x;
          m[1] = (T)0; m[5] =   sy; m[9]  = (T)0; m[13] = (sy * -p.y) + p.y;
          m[2] = (T)0; m[6] = (T)0; m[10] =   sz; m[14] = (sz * -p.z) + p.z;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::scaleinv(T sx, T sy, T sz)
    {
          mat4<T> m;

          m[0] = (T)1/sx; m[4] = (T)0;    m[8]  = (T)0;    m[12] = (T)0;
          m[1] = (T)0;    m[5] = (T)1/sy; m[9]  = (T)0;    m[13] = (T)0;
          m[2] = (T)0;    m[6] = (T)0;    m[10] = (T)1/sz; m[14] = (T)0;
          m[3] = (T)0;    m[7] = (T)0;    m[11] = (T)0;    m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::scalepinv(T sx, T sy, T sz, vect3<T> p)
    {
          /* ( ( transinv(-p) * scaleinv(s) ) * transinv(p) )
             |1/sx,    0,    0, (1/sx * -px) + --px|
             |   0, 1/sy,    0, (1/sy * -py) + --py|
             |   0,    0, 1/sz, (1/sz * -pz) + --pz|
             |   0,    0,    0,                   1| */
          mat4<T> m;

          m[0] = (T)1/sx; m[4] = (T)0;    m[8]  = (T)0;    m[12] = ((T)1/sx * -p.x) + p.x;
          m[1] = (T)0;    m[5] = (T)1/sy; m[9]  = (T)0;    m[13] = ((T)1/sy * -p.y) + p.y;
          m[2] = (T)0;    m[6] = (T)0;    m[10] = (T)1/sz; m[14] = ((T)1/sz * -p.z) + p.z;
          m[3] = (T)0;    m[7] = (T)0;    m[11] = (T)0;    m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::shearxy(T a, T b)
    {
          mat4<T> m;

          m[0] = (T)1; m[4] = (T)0; m[8]  =    a; m[12] = (T)0;
          m[1] = (T)0; m[5] = (T)1; m[9]  =    b; m[13] = (T)0;
          m[2] = (T)0; m[6] = (T)0; m[10] = (T)1; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::shearxyp(T a, T b, vect3<T> p)
    {
          /*( ( trans(p) * shearxy(s) ) * trans(-p) )
            |1, 0, sa, -px + (sa * -pz) + px|
            |0, 1, sb, -py + (sb * -pz) + py|
            |0, 0,  1,              -pz + pz|
            |0, 0,  0,                     1|*/
          mat4<T> m;

          m[0] = (T)1; m[4] = (T)0; m[8]  =    a; m[12] = a * -p.z;
          m[1] = (T)0; m[5] = (T)1; m[9]  =    b; m[13] = b * -p.z;
          m[2] = (T)0; m[6] = (T)0; m[10] = (T)1; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::shearxyinv(T a, T b)
    {
          mat4<T> m;

          m[0] = (T)1; m[4] = (T)0; m[8]  =   -a; m[12] = (T)0;
          m[1] = (T)0; m[5] = (T)1; m[9]  =   -b; m[13] = (T)0;
          m[2] = (T)0; m[6] = (T)0; m[10] = (T)1; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::shearxypinv(T a, T b, vect3<T> p)
    {
          /* ( ( transinv(-p) * shearxyinv(s) ) * transinv(p) )
             |1, 0, -sa, -px + (-sa * -pz) + --px|
             |0, 1, -sb, -py + (-sb * -pz) + --py|
             |0, 0,   1,               -pz + --pz|
             |0, 0,   0,                        1| */
          mat4<T> m;

          m[0] = (T)1; m[4] = (T)0; m[8]  =   -a; m[12] = -a * -p.z;
          m[1] = (T)0; m[5] = (T)1; m[9]  =   -b; m[13] = -b * -p.z;
          m[2] = (T)0; m[6] = (T)0; m[10] = (T)1; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }
      template <typename T>
    const mat4<T> mat4<T>::shearxz(T a, T b)
    {
          mat4<T> m;

          m[0] = (T)1; m[4] =    a; m[8]  = (T)0; m[12] = (T)0;
          m[1] = (T)0; m[5] = (T)1; m[9]  = (T)0; m[13] = (T)0;
          m[2] = (T)0; m[6] =    b; m[10] = (T)1; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::shearxzp(T a, T b, vect3<T> p)
    {
          /*( ( trans(p) * shearxz(s) ) * trans(-p) )
            |1, sa, 0, -px + (sa * -py) + px|
            |0,  1, 0,              -py + py|
            |0, sb, 1, (sb * -py) + -pz + pz|
            |0,  0, 0,                     1|*/
          mat4<T> m;

          m[0] = (T)1; m[4] =    a; m[8]  = (T)0; m[12] = a * -p.y;
          m[1] = (T)0; m[5] = (T)1; m[9]  = (T)0; m[13] = (T)0;
          m[2] = (T)0; m[6] =    b; m[10] = (T)1; m[14] = b * -p.y;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::shearxzinv(T a, T b)
    {
          mat4<T> m;

          m[0] = (T)1; m[4] =   -a; m[8]  = (T)0; m[12] = (T)0;
          m[1] = (T)0; m[5] = (T)1; m[9]  = (T)0; m[13] = (T)0;
          m[2] = (T)0; m[6] =   -b; m[10] = (T)1; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::shearxzpinv(T a, T b, vect3<T> p)
    {
          /* ( ( transinv(-p) * shearxzinv(s) ) * transinv(p) )
             |1, -sa, 0, -px + (-sa * -py) + --px|
             |0,   1, 0,               -py + --py|
             |0, -sb, 1, (-sb * -py) + -pz + --pz|
             |0,   0, 0,                        1| */
          mat4<T> m;

          m[0] = (T)1; m[4] =   -a; m[8]  = (T)0; m[12] = -a * -p.y;
          m[1] = (T)0; m[5] = (T)1; m[9]  = (T)0; m[13] = (T)0;
          m[2] = (T)0; m[6] =   -b; m[10] = (T)1; m[14] = -b * -p.y;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::shearyz(T a, T b)
    {
          mat4<T> m;

          m[0] = (T)1; m[4] = (T)0; m[8]  = (T)0; m[12] = (T)0;
          m[1] =    a; m[5] = (T)1; m[9]  = (T)0; m[13] = (T)0;
          m[2] =    b; m[6] = (T)0; m[10] = (T)1; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::shearyzp(T a, T b, vect3<T> p)
    {
          /*( ( trans(p) * shearzy(s) ) * trans(-p) )
            | 1, 0, 0,              -px + px|
            |sa, 1, 0, (sa * -px) + -py + py|
            |sb, 0, 1, (sb * -px) + -pz + pz|
            | 0, 0, 0,                     1|*/
          mat4<T> m;

          m[0] = (T)1; m[4] = (T)0; m[8]  = (T)0; m[12] = (T)0;
          m[1] =    a; m[5] = (T)1; m[9]  = (T)0; m[13] = a * -p.x;
          m[2] =    b; m[6] = (T)0; m[10] = (T)1; m[14] = a * -p.x;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::shearyzinv(T a, T b)
    {
          mat4<T> m;

          m[0] = (T)1; m[4] = (T)0; m[8]  = (T)0; m[12] = (T)0;
          m[1] =   -a; m[5] = (T)1; m[9]  = (T)0; m[13] = (T)0;
          m[2] =   -b; m[6] = (T)0; m[10] = (T)1; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::shearyzpinv(T a, T b, vect3<T> p)
    {
          /* ( ( transinv(-p) * shearzyinv(s) ) * transinv(p) )
             |  1, 0, 0,               -px + --px|
             |-sa, 1, 0, (-sa * -px) + -py + --py|
             |-sb, 0, 1, (-sb * -px) + -pz + --pz|
             |  0, 0, 0,                        1| */
          mat4<T> m;

          m[0] = (T)1; m[4] = (T)0; m[8]  = (T)0; m[12] = (T)0;
          m[1] =   -a; m[5] = (T)1; m[9]  = (T)0; m[13] = -a * -p.x;
          m[2] =   -b; m[6] = (T)0; m[10] = (T)1; m[14] = -b * -p.x;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

    //lookat matrix
      template <typename T>
    const mat4<T> mat4<T>::lookat()
    {
          mat4<T> m;

          m[0] = (T)1; m[4] = (T)0; m[8]  = (T)0; m[12] = (T)0;
          m[1] = (T)0; m[5] = (T)1; m[9]  = (T)0; m[13] = (T)0;
          m[2] = (T)0; m[6] = (T)0; m[10] = (T)1; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::lookatinv()
    {
          mat4<T> m;

          m[0] = (T)1; m[4] = (T)0; m[8]  = (T)0; m[12] = (T)0;
          m[1] = (T)0; m[5] = (T)1; m[9]  = (T)0; m[13] = (T)0;
          m[2] = (T)0; m[6] = (T)0; m[10] = (T)1; m[14] = (T)0;
          m[3] = (T)0; m[7] = (T)0; m[11] = (T)0; m[15] = (T)1;

          return m;
    }

    //projection matrices
      template <typename T>
    const mat4<T> mat4<T>::ortho(T l, T r, T b, T t, T n, T f)
    {
          //see http://www.opengl.org/sdk/docs/man/xhtml/glOrtho.xml
          mat4<T> m;

          m[0] = (T)2/(r-l); m[4] = (T)0;       m[8]  = (T)0;        m[12] = -((r+l)/(r-l));
          m[1] = (T)0;       m[5] = (T)2/(t-b); m[9]  = (T)0;        m[13] = -((t+b)/(t-b));
          m[2] = (T)0;       m[6] = (T)0;       m[10] = (T)-2/(f-n); m[14] = -((f+n)/(f-n));
          m[3] = (T)0;       m[7] = (T)0;       m[11] = (T)0;        m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::orthoinv(T l, T r, T b, T t, T n, T f)
    {
          mat4<T> m;

          m[0] = (r-l)/(T)2; m[4] = (T)0;       m[8]  = (T)0;        m[12] = (r+l)/(T)2;
          m[1] = (T)0;       m[5] = (t-b)/(T)2; m[9]  = (T)0;        m[13] = (t+b)/(T)2;
          m[2] = (T)0;       m[6] = (T)0;       m[10] = (f-n)/(T)-2; m[14] = (n+f)/(T)2;
          m[3] = (T)0;       m[7] = (T)0;       m[11] = (T)0;        m[15] = (T)1;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::frustum(T l, T r, T b, T t, T n, T f)
    {
          //see http://www.opengl.org/sdk/docs/man/xhtml/glFrustum.xml
          mat4<T> m;

          m[0] = ((T)2*n)/(r-l); m[4] = (T)0;           m[8]  = (r+l)/(r-l);  m[12] = (T)0;
          m[1] = (T)0;           m[5] = ((T)2*n)/(t-b); m[9]  = (t+b)/(t-b);  m[13] = (T)0;
          m[2] = (T)0;           m[6] = (T)0;           m[10] = -((f+n)/(f-n)); m[14] = -(((T)2*f*n)/(f-n));
          m[3] = (T)0;           m[7] = (T)0;           m[11] = (T)-1;        m[15] = (T)0;

          return m;
    }

      template <typename T>
    const mat4<T> mat4<T>::frustuminv(T l, T r, T b, T t, T n, T f)
    {
          mat4<T> m;

          m[0] = (r-l)/((T)2*n); m[4] = (T)0;           m[8]  = (T)0;              m[12] = (r+l)/((T)2*n);
          m[1] = (T)0;           m[5] = (t-b)/((T)2*n); m[9]  = (T)0;              m[13] = (t+b)/((T)2*n);
          m[2] = (T)0;           m[6] = (T)0;           m[10] = (T)0;              m[14] = (T)-1;
          m[3] = (T)0;           m[7] = (T)0;           m[11] = -(f-n)/((T)2*f*n); m[15] = (f+n)/((T)2*f*n);

          return m;
    }





    //matrix multiplication

    template <typename T>
    const mat4<T> mat4<T>::operator*(const mat4<T>& R) const
    {
          mat4<T> temp;
          /*  *  *	*  *
              |0  4  8 12|   |*0  4  8 12|   | *  4  8 12|
              |1  5  9 13|   |*1  5  9 13|   | 1  5  9 13|
              |2  6 10 14| x |*2  6 10 14| = | 2  6 10 14|
              |3  7 11 15|   |*3  7 11 15|   | 3  7 11 15|
              */
          temp[0]  = M[0]  * R[0]  + M[4]  * R[1]  + M[8]  * R[2]  + M[12] * R[3];
          temp[4]  = M[0]  * R[4]  + M[4]  * R[5]  + M[8]  * R[6]  + M[12] * R[7];
          temp[8]  = M[0]  * R[8]  + M[4]  * R[9]  + M[8]  * R[10] + M[12] * R[11];
          temp[12] = M[0]  * R[12] + M[4]  * R[13] + M[8]  * R[14] + M[12] * R[15];

          temp[1]  = M[1]  * R[0]  + M[5]  * R[1]  + M[9]  * R[2]  + M[13] * R[3];
          temp[5]  = M[1]  * R[4]  + M[5]  * R[5]  + M[9]  * R[6]  + M[13] * R[7];
          temp[9]  = M[1]  * R[8]  + M[5]  * R[9]  + M[9]  * R[10] + M[13] * R[11];
          temp[13] = M[1]  * R[12] + M[5]  * R[13] + M[9]  * R[14] + M[13] * R[15];

          temp[2]  = M[2]  * R[0]  + M[6]  * R[1]  + M[10] * R[2]  + M[14] * R[3];
          temp[6]  = M[2]  * R[4]  + M[6]  * R[5]  + M[10] * R[6]  + M[14] * R[7];
          temp[10] = M[2]  * R[8]  + M[6]  * R[9]  + M[10] * R[10] + M[14] * R[11];
          temp[14] = M[2]  * R[12] + M[6]  * R[13] + M[10] * R[14] + M[14] * R[15];

          temp[3]  = M[3]  * R[0]  + M[7]  * R[1]  + M[11] * R[2]  + M[15] * R[3];
          temp[7]  = M[3]  * R[4]  + M[7]  * R[5]  + M[11] * R[6]  + M[15] * R[7];
          temp[11] = M[3]  * R[8]  + M[7]  * R[9]  + M[11] * R[10] + M[15] * R[11];
          temp[15] = M[3]  * R[12] + M[7]  * R[13] + M[11] * R[14] + M[15] * R[15];

          return temp;
    }

      template <typename T>
    const mat4<T>& mat4<T>::operator*=(const mat4<T>& R)
    {
          return *this = *this * R;
    }


    //vector/ray multiplication

    template <typename T>
    const vect4<T> mat4<T>::operator*(const vect4<T>& v) const
    {
          vect4<T> vr;
          /* |0  4  8 12| |v0|
             |1  5  9 13| |v1|
             |2  6 10 14| |v2|
             |3  7 11 15| |v3|*/
          vr[0] = M[0] * v[0] + M[4] * v[1] + M[8]  * v[2] + M[12] * v[3];
          vr[1] = M[1] * v[0] + M[5] * v[1] + M[9]  * v[2] + M[13] * v[3];
          vr[2] = M[2] * v[0] + M[6] * v[1] + M[10] * v[2] + M[14] * v[3];
          vr[3] = M[3] * v[0] + M[7] * v[1] + M[11] * v[2] + M[15] * v[3];

          return vr;
    }

      template <typename T>
    const vect4<T>& operator*=(vect4<T>& v, const mat4<T>& M)
    {
          return v = M*v;
    }


    //transform vect3 by matrix M assume w = 1
    template <typename T>
    const vect3<T> mat4<T>::operator*(const vect3<T>& v) const
    {
          vect3<T> vr;
          /* |0  4  8 12| |v0|
             |1  5  9 13| |v1|
             |2  6 10 14| |v2|
             |3  7 11 15| |1 |*/
          vr[0] = M[0] * v[0] + M[4] * v[1] + M[8]  * v[2] + M[12];
          vr[1] = M[1] * v[0] + M[5] * v[1] + M[9]  * v[2] + M[13];
          vr[2] = M[2] * v[0] + M[6] * v[1] + M[10] * v[2] + M[14];

          return vr;
    }

      template <typename T>
    const vect3<T>& operator*=(vect3<T>& v, const mat4<T>& M)
    {
          return v = M*v;
    }



    template <typename T>
    const vect4<T> mat4<T>::transmultv(const vect4<T>& v) const
    {
          vect4<T> vr;
          /* |0   1   2  3| |v0|
             |4   5   6  7| |v1|
             |8   9  10 11| |v2|
             |12  13 14 15| |v3|*/
          vr[0] = M[0]  * v[0] + M[1]  * v[1] + M[2]  * v[2] + M[3]  * v[3];
          vr[1] = M[4]  * v[0] + M[5]  * v[1] + M[6]  * v[2] + M[7]  * v[3];
          vr[2] = M[8]  * v[0] + M[9]  * v[1] + M[10] * v[2] + M[11] * v[3];
          vr[3] = (T)0; // M[12] * v[0] + M[13] * v[1] + M[14] * v[2] + M[15] * v[3];

          return vr;
    }

    /*
      template <typename T>
    inline const ray<T> operator*(const mat4<T>& M, const ray<T>& r)
    {
          ray<T> q;
          q.o = M * r.o;
          q.d = M * r.d;
          return q;
    }
    */

      template <typename T>
    bool operator==(const mat4<T>& l, const mat4<T>& r)
    {
          int i=0;
          while (i<16)
          {
            if (lt(l[i], r[i]) || lt(r[i], l[i]))
              return false;
            i++;
          }
          return true;
    }


      template <typename T>
    std::ostream& operator<<(std::ostream& os, const mat4<T>& M)
    {
          for (int i=0; i<4; i++)
            os << std::fixed << std::setprecision(6) <<
              "|" << M[i] << ", " << M[i+4] << ", " << M[i+8] <<
              ", " << M[i+12] << "|" << std::endl;

          return os;
    }



    //typedefs for convenience
    typedef mat4<float>   mat4f;
    typedef mat4<double>  mat4d;


} // namespace visualization
} // namespace bso

#endif  // VIS_MAT4_HPP

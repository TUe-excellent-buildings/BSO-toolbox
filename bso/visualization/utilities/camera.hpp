#ifndef VIS_CAMERA_HPP
#define VIS_CAMERA_HPP

#ifndef M_PI
#define M_PI 3.141592653589793238463
#endif // M_PI

#include <bso/visualization/utilities/mat4.hpp>
#include <bso/visualization/utilities/vect3.hpp>
#include <bso/visualization/utilities/aabbox.hpp>


namespace bso { namespace visualization
{

    class camera
    {
    public:
        //returns a modelview matrix in mv that sets, the camera
        //in position to render the model
        virtual void getmodelview(mat4d &mv) const = 0;
        virtual void getpos(vect3d &pos) const = 0;
        virtual void setcamera(const aabboxd &bbox, //bounding box of model
                       double vpratio,              //viewport ratio
                       double minangle) = 0;        //the minimal viewing angle(rad)
    };


    //simple camera that orbits the origin
    class orbitalcamera : public camera
    {
    public:
        orbitalcamera(const vect3d &o = vect3d(0.0, 0.0, 0.0),
                      double d = 10.0, double r = 0.0, double e = 0.0)
          : origin(o), dist(d), rot(r), elev(e), zoom(1.0) { }

        //the following functions sets up the camera...
        //it calculates the right projection and modelview matrices
            //such that the bounding sphere of the model is completely
        //visible inside the viewport
        void setcamera(const aabboxd &bbox, //bounding sphere radius
                       double vpratio,  //viewport ratio
                       double minangle);//the minimal viewing angle(rad)

        void getmodelview(mat4d &mv) const;
        void getpos(vect3d &pos) const;

        inline const vect3d& getorigin() { return origin; }
        inline double getdistance()  { return dist; }
        inline double getzoom()      { return zoom; }
        inline double getrotation()  { return rot; }
        inline double getelevation() { return elev; }

        inline void setorigin(const vect3d &o) { origin = o; }
        inline void setdistance(double d)  { dist = d; }
        inline void setzoom(double z)      { zoom = z; }
        inline void setrotation(double r)  { rot = r; }
        inline void setelevation(double e) { elev = e; }

    private:
        vect3d origin;
        double dist,  //distance
               rot,   //rotation around the y axis in degrees
               elev,  //elevation (rotation around x axis) in degrees
               zoom;  //zoom (factor of distance)
    };





    void orbitalcamera::setcamera(const aabboxd &bbox,
                                  double vpratio,
                                  double minangle) //in radians
    {
        /*
           the frustum should be large enough such that the
           bounding sphere (calculated from bbox) fits in it.
           we determine the height of a isoscelene (gelijkbenige)
           triangle which has an angle of minangle of which all
           sidestouch the bounding sphere (circle actually)

                      h
           |-----------------------|
                                      _ q
                              __..--`` |
                    __--`` .'````'.|
              __--``      :   c    :
          o <-------------:---()---: n -
                 A``--__      :        :   |
                    ``--__ `.____.`|   | b
                              ``--__  B|   |
                                ``--   -
                                    p

           We calculate the height (h) of the isoscelene triangle
           that has an angle A of minangle. The triangle has an
           an incircle of radius r. B = 90 - 0.5A

           The center c of the incircle is the intersection of the
           angular bisectors of the angles of the triangle.
           pnc form a right triangle in which nc has length r

           hence b = r/tan(0.5B) and h = b/tan(0.5A)
         */

        double r = (bbox.max - bbox.min).length()/2.0,
               halfA = minangle/2.0,
               halfB = (M_PI/4.0)- 0.5*halfA,
               b = r/tanf(halfB),
               h = b/tanf(halfA);

        //determine the size of the frustum
        double halfw, halfh;
        if (vpratio < 1.0) {
            //width < height
            halfw = b;
            halfh = b/vpratio;
        } else {
            //width >= height
            halfw = b*vpratio;
            halfh = b;
        }

        halfw /= h;
        halfh /= h;

        //set the projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-halfw, halfw, -halfh, halfh, 1, h);

        //set the camera to focus on the center of the bounding
        origin = bbox.getcenter();
        dist = h-r;

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        mat4d mv;
        getmodelview(mv);
        glLoadMatrixd(mv.M);
    }

    void orbitalcamera::getmodelview(mat4d &mv) const
    {
        mv = mat4d::trans(0.0, 0.0, -dist*zoom) *
             mat4d::rotx((-elev)*(M_PI/180.0)) *
             mat4d::roty((-rot)*(M_PI/180.0)) *
             mat4d::trans(-origin);
    }

    //returns the position of the camera in model co-ordinates
    void orbitalcamera::getpos(vect3d &pos) const
    {
        pos = (mat4d::transinv(-origin) *
               mat4d::rotyinv((-rot)*(M_PI/180.0)) *
               mat4d::rotxinv((-elev)*(M_PI/180.0)) *
               mat4d::transinv(0.0, 0.0, -dist*zoom)) *
              vect3d(0.0, 0.0, 0.0);
    }




} // namespace visualization
} // namespace bso

#endif //VIS_CAMERA_HPP

#ifndef VIS_DATATYPES_HPP
#define VIS_DATATYPES_HPP

#include <bso/visualization/utilities/vect3.hpp>
#include <bso/visualization/utilities/vect4.hpp>

#ifdef __unix__
// see http://stackoverflow.com/questions/31579243/segmentation-fault-before-main-when-using-glut-and-stdstring
#include <GL/glut.h>
#elif defined(_WIN32) || defined(WIN32)
#include <glut.h>
#endif

#include <string>

namespace bso { namespace visualization
{

    /*
       typedef for vertex type and vertex shared pointer
       (we could've just used vect3d, but this looks nicer)
     */
    typedef vect3d             vertex;

    /*
       idem for normal
     */
    typedef vect3d             normal;

    /*
       typedef for rgba color, floats are good enough for colors
     */
    typedef vect4f rgba;


    /*
       polygon properties
     */
    struct polygon_props
    {
        rgba  ambient,
              diffuse,
              specular,
              emission;
        float shininess;
        bool  translucent,
              twosided;


        //initialising constructor
        polygon_props(const rgba &a  = rgba(0.2f, 0.2f, 0.2f, 1.0f),
                      const rgba &d  = rgba(0.8f, 0.8f, 0.8f, 1.0f),
                      const rgba &sp = rgba(0.0f, 0.0f, 0.0f, 1.0f),
                  const rgba &e  = rgba(0.0f, 0.0f, 0.0f, 1.0f),
                      float sh  = 0.0f,
                  bool  trans = false,
                  bool  two = false)
          : ambient(a), diffuse(d), specular(sp),
            emission(e), shininess(sh),
            translucent(trans), twosided(two) { }

        /*
           this function pushes the apropriate opengl
           attributes onto the attrib stack and sets
           the polygon properties in opengl
           the calling code also has to call glPopAttrib()
         */
        inline void apply() const {
            if (translucent) {
                glPushAttrib(GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT | GL_POLYGON_BIT);

                //enable blending
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            else {
                glPushAttrib(GL_LIGHTING_BIT | GL_POLYGON_BIT);
            }

            //set polygon properties
            GLenum face = twosided? GL_FRONT_AND_BACK: GL_FRONT;
            if (twosided)
                face = GL_FRONT_AND_BACK;
            else {
                face = GL_FRONT;
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);
                glFrontFace(GL_CCW);
            }
            glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, twosided? 1 : 0);
            glMaterialfv(face, GL_AMBIENT, ambient.v);
            glMaterialfv(face, GL_DIFFUSE, diffuse.v);
            glMaterialfv(face, GL_SPECULAR, specular.v);
            glMaterialf(face, GL_SHININESS, shininess);
        }
    };


    /*
       properties for drawing line segments and
       polygon outlines.
     */
    struct line_props
    {
        rgba color;   //line color
        float  width;
        short  pattern; //stipple pattern (16 bit)
        int    factor;  //stipple factor

        //initialising constructor
        line_props(const rgba &c = rgba(0.1f, 0.1f, 0.1f, 1.0f),
                  float w = 1.0f,
                  short pat = (short)0xff,
                  int   fac = 1)
         : color(c), width(w), pattern(pat), factor(fac) { }

        //applies the line properties
        inline void apply() const {
            glPushAttrib(GL_LINE_BIT | GL_LIGHTING_BIT);
            glDisable(GL_LIGHTING);
            glColor4fv(color.v);
            glLineWidth(width);
            if (pattern != 0xff) {
                glEnable(GL_LINE_STIPPLE);
                glLineStipple(factor, pattern);
            }
        }
    };


    /*
       label properties
     */
    struct label_props
    {
        rgba textcolor;
        void *glutbmpfont;  //GLUT bitmap font

        label_props(const rgba &tc = rgba(1.0f, 1.0f, 1.0f, 1.0f),
                 void *font = GLUT_BITMAP_8_BY_13)
          : textcolor(tc), glutbmpfont(font) { }
    };

} // namespace visualization
} // namespace bso

#endif //VIS_DATATYPES_HPP

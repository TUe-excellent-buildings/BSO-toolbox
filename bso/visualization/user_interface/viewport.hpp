#ifndef VIS_VIEWPORT_HPP
#define VIS_VIEWPORT_HPP

#ifndef M_PI
#define M_PI 3.141592653589793238463
#endif // M_PI

#include <bso/visualization/models/model_base.hpp>
#include <bso/visualization/utilities/camera.hpp>

#include <iostream>

namespace bso { namespace visualization
{

    //forward declaration
    class model_base;
    class camera;

    class viewport
    {
    public:
        //viewport(int x_, int y_, int w, int h, model *pm) : x(x_), y(y_), width(w), height(h), pmodel(pm) { }

        viewport(model_base *pm)
            : pmodel(pm), xorig(0), yorig(0),
                  width(0), height(0),
                  enabled(false), highlighted(false) { }

        void render(camera &cam) const;
        bool hittest(int x, int y) const;

    private:
        friend class viewportmanager;

        model_base *pmodel;
        int   xorig, yorig, width, height;
        bool  enabled,
              highlighted;

        void render_decorations(const char *ptitle) const;
    };






    // Implementation of member functions

    bool viewport::hittest(int x, int y) const
    {
        if (enabled) {
            return xorig <= x && x <= xorig + width &&
                   yorig <= y && y <= yorig + height;
        }

        return false;
    }

    void viewport::render(camera &cam) const
    {
        if (!enabled)
            return;

        //set viewport
        glViewport(xorig, yorig, width, height);

        //set projection matrix
        //set modelview matrix
        cam.setcamera(pmodel->get_bbox(),
                      (double)width/(double)height,
                  M_PI/3.0);

        //render model
        pmodel->render(cam);

        //render viewport decorations
        render_decorations(pmodel->get_description().c_str());

    }

    void viewport::render_decorations(const char *ptitle) const
    {
        //settings for the viewport decoration
        int fwidth = 5,
            txtheight = 10;
        void *font = GLUT_BITMAP_HELVETICA_10;// GLUT_BITMAP_8_BY_13;
        //set ortographic projection
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0.0, (GLdouble)width, 0.0, (GLdouble)height, 0.0, 1.0);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        //render a window outline and title
        //bar where the bar has height
        //txtheight + 2 + fwidth
        glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glShadeModel(GL_SMOOTH);

        float opaq[4] =  {0.7f, 0.7f, 0.7f, 1.0f},
              trans[4] = {0.7f, 0.7f, 0.7f, 0.2f};
        if (highlighted) {
            trans[0] = trans[1] = trans[2] = 1.0f;
            trans[3] = 1.0;
        }

        //render title bar
        glBegin(GL_POLYGON);
        glColor4fv(opaq);
        glVertex3i(width, height-txtheight, -1);
        glVertex3i(width, height, -1);
        glVertex3i(0, height, -1);
        glVertex3i(0, height-txtheight, -1);
        glEnd();
        //render top frame piece
        glBegin(GL_POLYGON);
        glColor4fv(opaq);
        glVertex3i(width, height-txtheight, -1);
        glVertex3i(0, height-txtheight, -1);
        glColor4fv(trans);
        glVertex3i(fwidth, height-txtheight-fwidth, -1);
        glVertex3i(width-fwidth, height-txtheight-fwidth, -1);
        glEnd();
        //render left frame piece
        glBegin(GL_POLYGON);
        glColor4fv(opaq);
        glVertex3i(0, height-txtheight, -1);
        glVertex3i(0, 0, -1);
        glColor4fv(trans);
        glVertex3i(fwidth, fwidth, -1);
        glVertex3i(fwidth, height-txtheight-fwidth, -1);
        glEnd();
        //render lower frame piece
        glBegin(GL_POLYGON);
        glColor4fv(opaq);
        glVertex3i(0, 0, -1);
        glVertex3i(width, 0, -1);
        glColor4fv(trans);
        glVertex3i(width-fwidth, fwidth, -1);
        glVertex3i(fwidth, fwidth, -1);
        glEnd();
        //render right frame piece
        glBegin(GL_POLYGON);
        glColor4fv(opaq);
        glVertex3i(width, 0, -1);
        glVertex3i(width, height-txtheight, -1);
        glColor4fv(trans);
        glVertex3i(width-fwidth, height-txtheight-fwidth, -1);
        glVertex3i(width-fwidth, fwidth, -1);
        glEnd();

        //render title
        int space = width - 2*fwidth;
        glColor4f(0.0, 0.0, 0.0, 1.0);
        glRasterPos3i(fwidth, height-txtheight, -1.0);
        while (*ptitle != '\0') {
            space -= glutBitmapWidth(font, (int)*ptitle);
            if (space < 0)
                break;
            glutBitmapCharacter(font, (int)*ptitle);
            ptitle++;
        }

        glPopAttrib();

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }


} // namespace visualization
} // namespace bso

#endif //VIS_VIEWPORT_HPP

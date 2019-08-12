#ifndef VIS_VIEWPORTMANAGER_HPP
#define VIS_VIEWPORTMANAGER_HPP

#include <bso/visualization/user_interface/viewport.hpp>
#include <bso/visualization/models/model_base.hpp>
#include <bso/visualization/utilities/camera.hpp>

#ifdef __unix__                    /* __unix__ is usually defined by compilers targeting Unix systems */
#include <GL/glut.h>
#elif defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or   64 bit Windows systems */
#include <glut.h>
#endif

#include <math.h>
#include <list>
#include <string>
#include <fstream>
#include <limits>

namespace bso { namespace visualization
{

    class viewport;
    class camera;

    class viewportmanager
    {
    public:
        viewportmanager()
          : focus(NULL), maximised(NULL),
            lmousestate(GLUT_UP), lclicktime(0) { }

        void addviewport(viewport *pviewport);

        //event handlers
        void render(camera &cam);
        void resize(int w, int h);
        void mousemove_event(int x, int y);
        void mouse_event(int buttons, int state, int x, int y,
                         unsigned int time);
        bool key_event(int key, int time);

    private:
        int winwidth,
            winheight;
        std::list<viewport*> viewports; //the list of viewports

        viewport *focus,
             *maximised; //pointer to the viewport that
                             //is maximised, when NULL no
                             //viewport is maximised

        int lmousestate;
        unsigned int lclicktime; //time of the previous left mouse click

        void double_clicked();
        void update_viewport_sizes();
        void calc_viewport_grid(int width,      //window width
                                    int height,     //window height
                                    int nviewports, //number of viewports
                                double pratio,  //preferred aspect ratio
                    int &gridwidth,
                    int &gridheight);

    };








    void viewportmanager::addviewport(viewport *pviewport)
    {
        if (pviewport){
            viewports.push_back(pviewport);
            update_viewport_sizes();
        }
    }

    /*
       this algorithm calculates an a*b grid for n viewports,
       where the viewports aspect ratios are as close as possible
       to the preferred ratio, and not too much empty space is wasted
     */
    void viewportmanager::calc_viewport_grid(int width,      //window width
                                             int height,     //window height
                                             int n, //number of viewports
                         double pratio,  //preferred aspect ratio
                         int &gridwidth, int &gridheight)
    {
        /*
           the window has an aspect ratio of w/h, for a grid
           of a*b (equally sized) viewports in the window,
           each viewport has an aspect ratio of (b*w)/(a*h) {1}.
           The algorithm searches for the gridsize (a,b) with
           the minimum absolute difference between the preferred
           aspect ratio and {1}. We only consider suitable values
           for a and b, such that the gridsize a*b is at least n,
           but not too great, since we try to avoid creating unnecessary
           empty cells. We consider a grid a*b too great when it
           contains a at least one whole empty row or column of
           cells, more formally when: (a*b)- n >= a || (a*b)-n >= b
           adding the fact that a*b has to be at least n, we need
           to find all values (a,b) such that:
           0 <= (a*b)-n < min(a,b) {2}

           Luckily, for n,a,b > 0 there is exactly one b for every a (and
           visa versa) satisfying the predicate {2}

           For a given n > 0 and a > 0 we find the value for b
         */
        double mindif = std::numeric_limits<double>::infinity();

        for (int a=1; a<=n; a++) {
            int b = (n%a == 0? n/a : (n/a) + 1);
            double dr = fabs(((double)(b*width)/(double)(a*height)) - pratio);
            if (dr < mindif) {
                mindif = dr;
                gridwidth = a;
                gridheight = b;
            }
        }
    }

    void viewportmanager::render(camera &cam)
    {
        std::list<viewport*>::iterator it;
        for (it = viewports.begin(); it != viewports.end(); it++)
            (*it)->render(cam);
    }

    void viewportmanager::resize(int w, int h)
    {
        winwidth = w;
        winheight = h;

        //resize the viewports
        update_viewport_sizes();
    }

    void viewportmanager::double_clicked()
    {
        //use the focus variable to get the viewport
        //under the cursor
        if (focus == NULL)
            return;

        if (maximised == focus)
        {
            //minimize the viewport
            maximised = NULL;

        } else {
            maximised = focus;
        }

        update_viewport_sizes();

        glutPostRedisplay();
    }

    void viewportmanager::update_viewport_sizes()
    {
        //set the initial size of the viewport grid
        //to the size of the window
        int nviewports = viewports.size(),
            gridxorig  = 0,
            gridyorig  = 0,
            gridwidth  = winwidth,
            gridheight = winheight,
            gridcols = 1, gridrows = 1; // gridcolls and gridrows will be initialised in the  calc_viewport_grid() function, here they are one to give them an initial value, 0 cannot be used because this crashes the program at window initialisation

        //check if there is a viewport that is maximized
        if (maximised) {
            //reserve a large part of the screen for the
            //maximized viewport and a small area for the
            //remaining viewports

            //take 100 pixels strp from the bottom or the left
            //sides of the window (depending which is shortest)
            //and use the rest of the window for the
            //maximised viewport

            if (gridwidth<200 || gridheight<200) {
                //window too small for other viewports
                maximised->xorig = 0;
                maximised->yorig = 0;
                maximised->width = gridwidth;
                maximised->height= gridheight;

                std::list<viewport*>::iterator it;
                for (it = viewports.begin(); it != viewports.end(); it++)
                    if (*it != maximised)
                        (*it)->enabled = false;
                return;
            }

            //set maximised viewport size and viewport
            //grid size
            nviewports--;
            if (gridwidth>gridheight) {
                maximised->xorig = 0;
                maximised->yorig = 0;
                maximised->width = gridwidth - 100;
                maximised->height= gridheight;
                gridxorig = gridwidth - 100;
                gridwidth = 100;
            } else {
                maximised->xorig = 0;
                maximised->yorig = 100;
                maximised->width = gridwidth;
                maximised->height= gridheight - 100;
                gridheight = 100;
            }
        }

        //determine the grid dimensions for the viewports
        calc_viewport_grid(gridwidth, gridheight, nviewports,
                           4.0/3.0, gridcols, gridrows);

        //set the viewport positions and dimensions
        double cellwidth = (double)gridwidth/(double)gridcols,
               cellheight = (double)gridheight/(double)gridrows;

        std::list<viewport*>::iterator it;
        int i = 0;
        for (it = viewports.begin(); it != viewports.end(); it++)
            if (*it != maximised) {
                (*it)->enabled = true;
                (*it)->xorig = gridxorig + (double)(i%gridcols)*cellwidth;
                (*it)->yorig = gridyorig + gridheight - (double)((i/gridcols)+1)*cellheight;
                (*it)->width = cellwidth;
                (*it)->height = cellheight;
                i++;
            }
    }

    void viewportmanager::mouse_event(int button, int state, int x, int y, unsigned int time)
    {
        (void)x; // does nothing
        (void)y; // does nothing
        //we are only interested in left mouse clicks
        if (button == GLUT_LEFT_BUTTON)
        {
            if (state == lmousestate)
                return;

            lmousestate = state;

            if (state == GLUT_UP) {
                //left mouse click
                //check for doubleclick
                if (time - lclicktime < 500) { //half a second
                    double_clicked();
                    lclicktime = 0;
                } else {
                    lclicktime = time;
                }
            }
        }
    }

    void viewportmanager::mousemove_event(int x, int y)
    {
        std::list<viewport*>::iterator it;
        viewport *phit = NULL;

        //transform window system coordinates to
        //viewport coordinate (invert y)
        y = winheight - y;
        for (it = viewports.begin(); it != viewports.end(); it++) {
            if ((*it)->hittest(x,y)) {
                phit = *it;
                break;
            }
        }

        if (phit != focus) {
            if (focus) focus->highlighted = false;
            if (phit) phit->highlighted = true;
            focus = phit;
            glutPostRedisplay();
        }
    }

    bool viewportmanager::key_event(int key, int time)
    {
        (void)time; // does nothing
        //keypresses are passed to the viewport with the focus
        if(focus)
            return focus->pmodel->key_pressed(key);

        return false;
    }


} // namespace visualization
} // namespace bso

#endif //VIS_VIEWPORTMANAGER_HPP

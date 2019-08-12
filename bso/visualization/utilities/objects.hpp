#ifndef VIS_OBJECTS_HPP
#define VIS_OBJECTS_HPP

#include <bso/visualization/utilities/datatypes.hpp>

#include <iostream>

namespace bso { namespace visualization
{

    /*
       base class of all objects that can be rendered
     */
    /*class object
    {
    public:
        virtual ~object() { }
        virtual void render() const = 0;
    };*/


    /*
       line segment
     */
    /*class linesegment : public object
    {
    public:
        linesegment(line_props *pprp,
                vertex const *pv1,
                    vertex const *pv2)
          : pvert1(pv1), pvert2(pv2), pprops(pprp) { }

        ~linesegment() { }

        void render() const;

    private:
        vertex const *pvert1,   //pointers to the segments endpoints
                     *pvert2;
        line_props   *pprops;  //pointer to line properties struct
    };*/

    /*
       edge structs form a circularly linked list that enumerate
       a polygon's vertices and vertex normals in counter clockwise
       order.
     */
    struct edge
    {
        const vertex *pvert;  //pointer to vertex
        const normal *pnorm;
        bool         edgeflag;
        edge         *pnext;

        //initialising constructor for convenience
        edge(const vertex *pv,
             const normal *pn,
    //hh, bool and edge struct are given standard values, so construtor with only first 2 argument will work
             bool  eflag = true,
             edge *pnxt = NULL)
        : pvert(pv), pnorm(pn), edgeflag(eflag), pnext(pnxt) { }
    };


    /*
       convex polygon
     */
    class polygon// : public object
    {
    public:
        //constructor, the polygon takes ownership of the
        //linked list of boundary edges
        polygon(polygon_props *ppr,
                edge *pbnd)
          : pprops(ppr), lprops(NULL), pbound(pbnd) { }

        polygon(polygon_props *ppr,
                line_props    *lpr,
                edge          *pbnd)
          : pprops(ppr), lprops(lpr), pbound(pbnd) { }

        ~polygon();

    //	the polygon class is currently only used for storage
    //	rendering is done by the bsp code
    //	void render() const;

        inline const polygon_props *get_polygon_props() { return pprops; }
        inline const line_props *get_line_props() { return lprops; }
        inline const edge *get_boundary() { return pbound; }
    private:
        //allow the output stream operator access to the
        //private members, this is actually a bit silly, we
        //could just as well declare polygon as a struct and
        //make everything public...
        friend std::ostream &operator<<(std::ostream &os, const polygon &poly);
        polygon_props *pprops;
        line_props    *lprops;
        edge          *pbound;

    };

    std::ostream &operator<<(std::ostream &os, const polygon &poly);
    std::ostream &operator<<(std::ostream &os, const edge &e);

    /*
       text label
     */
    class label// : public object
    {
    public:
        label(label_props         *pprp,
              const std::string   &txt,
              vertex const        *pp)
          : lbtext(txt), pprops(pprp), ppos(pp) { }

        ~label() { }

        void render() const;

    private:
        std::string  lbtext;
        label_props  *pprops;
        vertex const *ppos;
    };



// Implementation of member functions

    /*
    void linesegment::render() const
    {
        //apply line properties
        pprops->apply();

        //render the line
        glBegin(GL_LINE);

        glVertex3dv(pvert1->v);
        glVertex3dv(pvert2->v);

        glEnd();

        glPopAttrib();
    }*/

    polygon::~polygon()
    {
        //delete the boundary edge list, the rest
        //(vertices, normals and properties) are
        //owned by someone else and not the polygons'
        //responsibility to clean up
        edge *pedge = pbound,
             *pnext;

        do {
            pnext = pedge->pnext;
            delete pedge;
            pedge = pnext;
        } while (pedge != pbound);
    }

    /*
    void polygon::render() const
    {
        //THIS ROUTINE IS NOT USED TO RENDER A POLYGON
        //SINCE THE POLYGONS ARE PARTITIONED IN A BSP
        //TREE AND THEN BECOME POLY_FRAGS, ONLY THE POLY_FRAG
        //RENDER METHOD IS USED IN THE VISUALISATION PROGRAM
        //TO RENDER (FRAGMENTS OF) POLYGONS
        //THIS CODE PROBABLY IS NOT EVEN CORRECT SINCE IT HAS
        //NEVER BEEN TESTED...
        edge *pedge;

        if (lprops) {
            //render outline
            lprops->apply();

            glPushAttrib(GL_POLYGON_BIT);
            glEnable(GL_POLYGON_OFFSET_LINE);
            glPolygonOffset(1.0, -2.0);
            GLenum face = pprops->twosided?
                            GL_FRONT_AND_BACK : GL_FRONT;
            glPolygonMode(face, GL_LINE);

            glBegin(GL_POLYGON);

            pedge = pbound;
            do {
                glNormal3dv(pedge->pnorm->v);
                glVertex3dv(pedge->pvert->v);
                pedge = pedge->pnext;
            } while (pedge != pbound);

            glEnd();

            glPopAttrib(); //GL_POLYGON_BIT
            glPopAttrib(); //lprops
        }

        //render interior
        pprops->apply();

        //the polygon is convex so we should
        //be able to draw the vertices as a triangle fan
        //without having to triangulate the polygon
        glBegin(GL_TRIANGLE_FAN);

        pedge = pbound;
        do {
            glNormal3dv(pedge->pnorm->v);
            glVertex3dv(pedge->pvert->v);
            pedge = pedge->pnext;
        } while (pedge != pbound);

        glEnd();

        glPopAttrib(); //pprops
    }*/

    void label::render() const
    {
        glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);

        glDisable(GL_LIGHTING);

        //lets do it the easy way for now and only render text
        glRasterPos3dv(ppos->v);

        glColor4fv(pprops->textcolor.v);

        std::string::const_iterator it;
        for (it = lbtext.begin(); it != lbtext.end(); it++)
            glutBitmapCharacter(pprops->glutbmpfont, (int)*it);

        glPopAttrib();
    /*
        //determine text dimensions
        int tw = 0,
            th = 0;
        string::iterator it;
        for (it = text.begin(); it != text.end(); it++)
            tw += glutBitmapWidth(pprops->glutbmpfont, *it);

        //get current modelview and projection matrix
        mat4d mv, proj;
        int   vw, vh; //viewport dimensions
        glGetDoublev(GL_MODELVIEW_MATRIX, mv.M);
        glGetDoublev(GL_PROJECTION_MATRIX, proj.M);
        glGetIntegerv(GL_VIEWPORT_WIDTH, &vw);
        glGetIntegerv(GL_VIEWPORT_HEIGHT, &vh);

        //transform the label position from model space to  space
        vect4d p(*ppos);
        vect3d pwin;
        *ppos *= (projection * modelview);

        //rescale
        p /= p.w;

        //transform to viewport coordinates
        pwin.x = ((p.x + 1.0)/2.0 ) * (double)width;
        pwin.y = ((1.0 - p.y)/2.0) * (double)height;
        pwin.z = (p.z + 1.0)/2.0;

        return pwin;


        //set ortho graphic projection


        //project label position to screen coordinates

        //render label background

        //render label text

        glPopAttrib();
    */
    }


    std::ostream &operator<<(std::ostream &os, const edge &e)
    {
        return os << *e.pvert << " -(" << *e.pnorm//e.type
                 << ")-> ";
    }

    std::ostream &operator<<(std::ostream &os, const polygon &poly)
    {
        edge *pedge = poly.pbound;
        do {
            os << *pedge;
            pedge = pedge->pnext;
        } while (pedge != poly.pbound);
        return os;
    }


} // namespace visualization
} // namespace bso

#endif //VIS_OBJECTS_HPP

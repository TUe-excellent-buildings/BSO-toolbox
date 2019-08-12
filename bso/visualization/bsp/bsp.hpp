#ifndef VIS_BSP_HPP
#define VIS_BSP_HPP

#include <bso/visualization/utilities/objects.hpp>
#include <bso/visualization/utilities/camera.hpp>
#include <bso/visualization/utilities/vertex_store.hpp>
#include <bso/visualization/utilities/datatypes.hpp>
#include <bso/visualization/utilities/mat4.hpp>

#ifdef __unix__                    /* __unix__ is usually defined by compilers targeting Unix systems */
#include <GL/glut.h>
#elif defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or   64 bit Windows systems */
#include <glut.h>
#endif

#include <math.h>
#include <algorithm>
#include <cstring>
#include <list>
#include <string>
#include <map>

namespace bso { namespace visualization
{
// bsp base
    //forward declarations
    struct bsp_node;

    struct bsp_stats {
        int nobjects,    //number of original objects
            nsplitnodes,
            nleafnodes,
            nsplits,     //number of split objects
            nfreesplits, //number of free splits queued in front of a list
            maxdepth;    //maximum tree depth

        bsp_stats()
         : nobjects(0), nsplitnodes(0), nleafnodes(0),
           nsplits(0), nfreesplits(0), maxdepth(0) { }
    }; // bsp_stats

    std::ostream &operator<<(std::ostream &os, const bsp_stats &stats);


    /*
       this class represents a bsp tree which holds
       polygons. Line segments and points (labels)
       are for the moment not supported, they can
       only be drawn opaque.
       This bsp class is intended for depth sorting. Each
       internal node contains a plane, a list of polygons
       incident to the plane, and a above and below pointer,
       pointing to subtrees that contain polygons above and
       below the plane respectively. A leaf node contains
       one polygon.
       The tree is recursively constructed from a set of
       polygons by selecting a polygon at random and use
       its plane as splitting plane, sorting the set of
       polygons into three sets, above, below and incident
       to the plane. polygons that span the plane are
       split in two. The construction then recurses above
       and below sets.
     */
    class random_bsp
    {
    public:
        //create a bsp tree from the list of polygons
        random_bsp(const std::list<polygon*> &polygons);

        //this version takes a list of lists of polygons
        //each list is randomized and appended in the order
        //specified, this allows some kind of control of the
        //order in which the polygons are used to split space
        //in the bsp construction. One gets better balanced trees
        //when first the larger axis alligned polygons are used
        //to partition the space.
        random_bsp(const std::list<const std::list<polygon*>* > &polygonlists);
        ~random_bsp();

        /*
           renders the fragments in back to front order
         */
        void render_btf(const camera &cam);

        inline const bsp_stats &getstats() const { return stats; }

    private:
        bsp_node     *proot;
        vertex_store store;
        bsp_stats    stats;
    }; // random_bsp


// BSP_Internal

    /*
       pclass enumerates the different classifications
       for points/objects against a plane
     */
    enum pclass
    {
        above    = 0,
        below    = 1,
        incident = 2,
        spanning = 3	//this does not apply to points
    };

    std::ostream &operator<<(std::ostream &os, pclass pc);

    /*
       the plane struct represents an unbounded plane in plane equation
       notation, specified as four parameters A, B, C and D, such that
       for every point (x,y,z) on the plane we have:
         Ax + By + Cz + D = 0
       the vector {A,B,C} can be seen as the normal of the plane and D is
       the distance to the origin (0,0,0) along the normal.

       the plane struct supports two operations:
       - classifying a point against the plane
       - intersecting a ray with the plane
       they are both fairly simple and implemented inline
     */
    struct plane
    {
        vect3d n; //normal = {A,B,C}
        double D; //distance

        //initialising constructors
        plane() : n(), D(0.0) {}
        plane(const vect3d &n_, const double &D_) : n(n_), D(D_) {}

        /*
           we use 'thick' planes to acount for floating point imprecisions
           but then should the plane normal be normalised, i think... the
           thickness would otherwise be proportinal to the length of the
           normal vector

           returns front when point is in front of the plane (direction of
           the normal), back when the point is is on the opposite side and
           incident when the point is on the plane (with a tolerance of
           0.0001 distance to either side of the plane)
         */
        inline pclass classify(const vect3d &p) const {
            double eq = dot(n,p) + D; //plug p into plain equation
            return (eq < -0.0001) ? below : ((eq > 0.0001) ? above : incident);
        }

        /*
           calculates the distance (in terms of the length of the ray's
           direction vector), between the ray and the plane.
           currently the function throws an exception when the ray is parallel
           to the plane
         */
        inline double intersect(const vect3d &p, const vect3d &v) const {
            double ndotv = dot(n,v);
            if (ndotv == 0.0)
                throw std::string("plane::intersect : parallel ray"); //or should we return infinity?
            return -(dot(n,p) + D) / ndotv;
        }
    };

    //output stream operator for plane
    std::ostream &operator<<(std::ostream &os, const plane &P);

    //forward declaration
    struct poly_frag;

    /*
       type of bsp tree node
     */
    enum bsp_nodetype {
        splitnode,
        leafnode
    };

    /*
       the bsp tree node struct
     */
    struct bsp_node {
        bsp_nodetype  type;       //node type
        plane         split;      //partitioning plane
        bsp_node      *pup,       //subtree above the plane
                  *pdown;     //subtree below the plane
        std::list<poly_frag*> frags;   //objects on the partition plane,
                                  //or inside the leaf node

        //constructors
        //create a leaf node
        bsp_node(std::list<poly_frag*> &l)
         : type(leafnode), split(), pup(NULL), pdown(NULL),
           frags(l) {}

        //create a splitnode
        bsp_node(plane &s, bsp_node *pu, bsp_node *pd,
            std::list<poly_frag*> &l)
         : type(splitnode), split(s), pup(pu), pdown(pd),
           frags(l) {}
    };

    //output stream operator for bsp tree
    std::ostream &operator<<(std::ostream &os, const bsp_node &node);


    /*
       polygon fragment
     */
    struct bsp_edge;

    struct poly_frag
    {
        polygon  *ppoly;  //whole polygon
        bsp_edge *pbound; //fragment boundary

        //simple initialising constructor
        poly_frag(polygon *ppol, bsp_edge *pbnd)
         : ppoly(ppol), pbound(pbnd) { }

        ~poly_frag();

        void render();

        pclass classify(const plane &P) const;
    };

    std::ostream &operator<<(std::ostream &os, const poly_frag &frag);


    /*
       edge type
       these are used in two situations, the first is when rendering
       polygon outlines we render only the boundary edges, and in
       bsp construction we use the flags to detect 'free splits'
     */
    enum edge_type
    {
        boundary,     //is a boundary edge of the polygon
        split         //edge resulting from a polygon split
    };

    /*
       bsp_edges extend the normal edges with an edge type
       to keep track of wether the edge is a split or boundary
       edge and a pointer to incident edges which is used to
       avoid t-intersections, during bsp construction when an
       edge is split, all incident edges are split too, even
       if the fragments they belong are not in the current set
       of framgents.
     */
    struct bsp_edge
    {
        const vertex *pvert;
        const normal *pnorm;
        bool         edgeflag;
        edge_type type;

        bsp_edge *pnext;
        bsp_edge *pincident; //circular linked list

        //initialising constructor for convenience
        bsp_edge(const vertex *pv,
             const normal *pn,
             bool  eflag = true,
             edge_type tp  = boundary,
                 bsp_edge *pnxt = NULL,
                 bsp_edge *pi = NULL)
        : pvert(pv), pnorm(pn), edgeflag(eflag),
          type(tp), pnext(pnxt), pincident(pi) { }
    };

    /*
       these local function do most of the bsp
       construction work
     */

    /*
       converts a list of objects to a vector of
       object fragments, for now it only converts
       polygons
       no data is copied so the original objects
       should stay intact
     */
    void bsp_convert_objects(const std::list<polygon*> &polygons,
                     std::vector<poly_frag*> &polyfrags);

    bsp_node* bsp_build_tree(std::list<poly_frag*> &frags, bsp_stats &stats,
                             vertex_store &store, int depth);

    void bsp_create_partition_plane(poly_frag *pfrag, plane &part);

    void bsp_delete_tree(bsp_node *pnode);

    void bsp_render_btf(bsp_node *pnode, const vect3d &campos);

    void poly_frag_split(const plane &P,
                     poly_frag *pwhole,
                     poly_frag *&pupfrag,
                     poly_frag *&pdownfrag,
                 bsp_stats &stats,
                 vertex_store &store);

    void poly_frag_intersect_edges(poly_frag *frag,
                           const plane &P,
                               bsp_edge *&pe1,
                       bsp_edge *&pe2,
                       bsp_stats &stats,
                       vertex_store &store);

    bool poly_frag_is_freesplit(const poly_frag *frag);

    void split_bsp_edge(const plane &P,
                    bsp_edge *pedge,
                    vertex_store &store);

// bsp implementation of member funcitons etc:

    /*
       bsp class member functions
     */
    random_bsp::random_bsp(const std::list<polygon*> &polygons)
    {
        std::vector<poly_frag*> fragv;

        bsp_convert_objects(polygons, fragv);

        //randomize the vector
        std::random_shuffle(fragv.begin(), fragv.end());

        //turn it into list
        std::list<poly_frag*> polyfrags(fragv.begin(), fragv.end());

        //initialise stats
        stats = bsp_stats();
        stats.nobjects = polyfrags.size();

        proot = bsp_build_tree(polyfrags, stats, store, 0);
    }

    random_bsp::random_bsp(const std::list<const std::list<polygon*>* > &polygonlists)
    {
        std::list<poly_frag*> polyfrags;

        for (std::list<const std::list<polygon*>* >::const_iterator listit = polygonlists.begin();
             listit != polygonlists.end(); listit++) {
            std::vector<poly_frag*> fragv;

            bsp_convert_objects(**listit, fragv);

            //randomize the vector
            std::random_shuffle(fragv.begin(), fragv.end());

            //append it to list
            polyfrags.insert(polyfrags.end(), fragv.begin(), fragv.end());
        }

        //initialise stats
        stats = bsp_stats();
        stats.nobjects = polyfrags.size();

        proot = bsp_build_tree(polyfrags, stats, store, 0);
    }


    random_bsp::~random_bsp()
    { //delete bsp the tree
        bsp_delete_tree(proot);
    }


    void random_bsp::render_btf(const camera &cam)
    {
        vect3d campos;
        cam.getpos(campos);

        bsp_render_btf(proot, campos);
    }



    /*
       poly frag member functions
     */

    poly_frag::~poly_frag()
    {
        //delete the edge list
        bsp_edge *pedge = pbound,
             *pnext;

        do {
            pnext = pedge->pnext;
            delete pedge;
            pedge = pnext;
        } while (pedge != pbound);
    }

    void poly_frag::render()
    {
        const line_props *lprops = ppoly->get_line_props();
        const polygon_props *pprops = ppoly->get_polygon_props();
        bsp_edge *pedge;

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

        if (lprops) {
            //render outline
            lprops->apply();

            glPushAttrib(GL_POLYGON_BIT);
            glEnable(GL_POLYGON_OFFSET_LINE);
            glPolygonOffset(1.0, -1.0);
            GLenum face = pprops->twosided?
                            GL_FRONT_AND_BACK : GL_FRONT;
            glPolygonMode(face, GL_LINE);

            glBegin(GL_POLYGON);

            pedge = pbound;
            do {
                glEdgeFlag(pedge->edgeflag);
                glNormal3dv(pedge->pnorm->v);
                glVertex3dv(pedge->pvert->v);
                pedge = pedge->pnext;
            } while (pedge != pbound);

            glEnd();

            glPopAttrib(); //GL_POLYGON_BIT
            glPopAttrib(); //lprops
        }

        glPopAttrib(); //pprops

        /*
        //render normal vectors (debug)
        glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINES);
        pedge = pbound;
        do {
            glVertex3dv(pedge->pvert->v);
            glVertex3dv((*pedge->pvert + *pedge->pnorm).v);

            pedge = pedge->pnext;
        } while (pedge != pbound);
        glEnd();
        glPopAttrib();
        */

    }

    pclass poly_frag::classify(const plane &P) const
    {
        int      vertclass[3] = {0, 0, 0}; //classifications of the verts
        bsp_edge *pedge = pbound;

        /*
           classify each vertex of the polygon frament and
           count the number of classifications
         */
        do {
            pclass pc = P.classify(*pedge->pvert);
            vertclass[(int)pc]++;
            pedge = pedge->pnext;
        }
        while (pedge != pbound);

        if (vertclass[above] == 0)
            if (vertclass[below] == 0)
                return incident;
            else
                return below;
        else
            if (vertclass[below] == 0)
                return above;
            else
                return spanning;
    }

    /*
       local utility functions
     */

    void bsp_delete_tree(bsp_node *proot)
    {
        //perform a recursive depth first search and delete each node
        //and its objects, all shared data should automatically
        //be deleted by the shared_ptrs
        if (!proot) return;

        std::list<poly_frag*>::iterator it;
        for (it = proot->frags.begin(); it != proot->frags.end(); it++)
            delete *it;

        if (proot->type == splitnode) {
            bsp_delete_tree(proot->pup);
            bsp_delete_tree(proot->pdown);
        }

        delete proot;
    }


    void bsp_create_partition_plane(poly_frag *pfrag, plane &P)
    {
        /*
           be carefull because points can be colinear such
           that no normal can be calculated
           find two non-colinear vectors and a point in the
           plane and construct a plane equation

           find three vertices p0, p1 and p2 such that the
           angle between (p1-p0) and (p2-p0) is at least
           1 degree

           use the polygon instead of the polygon fragment,
           because fragments can become thin and long after
           splitting
         */
        const edge *pedge, *pbound;
            pedge = pbound = pfrag->ppoly->get_boundary();

        //let p0 be the first vertex
        vect3d p0 = *pedge->pvert;

        //let p1 be the second vertex, v1 = p1-p0
        pedge = pedge->pnext;
        vect3d v1 = *pedge->pvert - p0,
               v2;
        double v1len = v1.length();

        //find a suitable p2
        do {
            pedge = pedge->pnext;
            v2 = *pedge->pvert - p0;

            //calculate the cosine of the angle
            //between v1 and v2
            double cos = dot(v1,v2) / (v1len * v2.length());

            //cos(0) = 1, cos(1 degree) = 0.999...
            if (fabs(cos) < 0.999)
                break;

        } while(pedge != pbound);

        //maybe we should try different vertices for p0 and p1
        //before throwing an exception
        if (pedge == pbound)
            throw std::string("bsp_create_partition_plane() : polygon too thin");

        //create a plane
        vect3d n = cross(v1, v2).normalise();
        P = plane(n, dot(-n,p0));
    }

    typedef std::pair<const vertex*, const vertex*> vertp_pair;

    bsp_edge *bsp_convert_edge(const edge *pedge,
            std::map<vertp_pair, bsp_edge*> &incidencies)
    {
        //create bsp edge
        bsp_edge *pnew = new bsp_edge(pedge->pvert,
                                      pedge->pnorm,
                          pedge->edgeflag);

        //check if there is an incident edge
        const vertex *p1 = pedge->pvert,
                     *p2 = pedge->pnext->pvert;

        vertp_pair vertpair = p1<p2? std::make_pair(p1,p2): std::make_pair(p2,p1);

        bsp_edge *&incid = incidencies[vertpair];

        if (incid != NULL) {
            pnew->pincident = incid->pincident;
            incid->pincident = pnew;
        } else {
            incid = pnew;
            pnew->pincident = pnew;
        }
        return pnew;
    }

    void bsp_convert_objects(const std::list<polygon*> &polygons,
                     std::vector<poly_frag*> &polyfrags)
    {
        /*
           convert the polygons to polyfrags by converting
           the edge lists in bsp_edge lists. The only tricky
           thing we have to perform is finding incident edges
         */
        //the map is used to find incident edges, using the vertex
            //pointers. We add an edge from p1 to p2 as pair <&p1, &p2>
            //such that &p1<&p2
        std::map<vertp_pair, bsp_edge*> incidencies;

        //internal function

        std::list<polygon*>::const_iterator it;
        for (it = polygons.begin(); it != polygons.end(); it++) {
            polygon *poly = *it;

            const edge *pbound = poly->get_boundary(),
                   *pedge;

            bsp_edge *pnew, *pprev, *pfragbound;
            pprev = pfragbound = bsp_convert_edge(poly->get_boundary(),
                                              incidencies);
            pedge = pbound->pnext;
            do {
                pnew = bsp_convert_edge(pedge, incidencies);
                pprev->pnext = pnew;
                pprev = pnew;
                pedge = pedge->pnext;
            } while (pedge != pbound);

            pnew->pnext = pfragbound;

            polyfrags.push_back(new poly_frag(poly, pfragbound));
        }

    }

    /*
       The function below implements the recursive bsp construction
       algorithm. It takes advantage of so called free-splits, which
       are fragments that completely span the volume of a bsp node.
       If no polygons cross, we can use a freesplit fragment as
       partition plane and not split any other fragment.
     */
    bsp_node* bsp_build_tree(std::list<poly_frag*> &frags,
                         bsp_stats &stats,
                 vertex_store &store,
                 int depth)
    {
        int nrfrags = frags.size(); //not sure if size() is constant time

        //check if we can termintate the recursion
        if (nrfrags < 1)
            return NULL;

        //add to recursion depth
        if (stats.maxdepth < depth)
            stats.maxdepth = depth;

        if (nrfrags == 1) {
            //create a leaf node
            stats.nleafnodes++;
            return new bsp_node(frags);
        }

        //create a partition plane from the first polygon fragment
        plane P;
        bsp_create_partition_plane(frags.front(), P);

        //sort the fragments
        std::list<poly_frag*> uplist, downlist, onlist;
        std::list<poly_frag*>::iterator it;
        for (it = frags.begin(); it != frags.end(); it++) {
            switch ((*it)->classify(P)) {
            case above:
                uplist.push_back(*it);
                break;

            case below:
                downlist.push_back(*it);
                break;

            case incident:
                onlist.push_back(*it);
                break;

            case spanning:
                try {
                poly_frag *pup, *pdown;

                poly_frag_split(P, *it, pup, pdown,
                                stats, store);

                //check if the up part is a free split
                if (poly_frag_is_freesplit(pup)) {
                    uplist.push_front(pup);
                    stats.nfreesplits++;
                } else
                    uplist.push_back(pup);

                //check if the down part is a free split
                if (poly_frag_is_freesplit(pdown)) {
                    downlist.push_front(pdown);
                    stats.nfreesplits++;
                } else
                    downlist.push_back(pdown);
                } catch(std::string &err)
                {
                    std::cout << "caught error:\n\t" << err << "\n\nWhile splitting fragment:\n" <<
                        *(*it) << "\nFrom poly:\n" << *(*it)->ppoly << "\nWith plane:\n" <<
                            P << "(" << P.n.length() << ")" << std::endl;
                    bsp_edge *p = (*it)->pbound;
                    do {
                        std::cout << P.classify(*p->pvert) << " (" << dot(P.n,*p->pvert)+P.D << ") : " << *p->pvert << std::endl;
                        p = p->pnext;
                    } while (p!=(*it)->pbound);
                    exit(0);

                }

                break;
            }
        }

        //recurse on the up and down lists
        bsp_node *puptree    = bsp_build_tree(uplist, stats, store, depth+1);
        bsp_node *pdowntree  = bsp_build_tree(downlist, stats, store, depth+1);

        //create a bsp split node and return it
        stats.nsplitnodes++;
        return new bsp_node(P, puptree, pdowntree, onlist);
    }

    void poly_frag_split(const plane &P,
                     poly_frag *pwhole,
                     poly_frag *&pupfrag,
                     poly_frag *&pdownfrag,
                 bsp_stats &stats,
                 vertex_store &store)
    {
        /*
           any plane that intersects a convex polygon,
           intersects the boundary in 2 points.
         */
        bsp_edge *pedge1, *pedge2;

        /*
           pwhole

             O<----------O
             |           ^
             |           |
        - - -| - - - - - |- - - P (partition plane)
             |           |
             v           |
             O---------->O
         */

        poly_frag_intersect_edges(pwhole, P, pedge1, pedge2,
                                  stats, store);

        /*
           pwhole

             O<----------O
             | pedge1    ^
             v           |
        - - -O - - - - - O- - -
             |           ^
             v           | pedge2
             O---------->O
         */
        bsp_edge *pnext1 = pedge1->pnext,
             *pnext2 = pedge2->pnext;

        //unlink the boundary at the intersection points
        pedge1->pnext = NULL;
        pedge2->pnext = NULL;

        /*
           pwhole

             O<----------O
             | pedge1    ^
             v           | pnext2
                         |
        - - -O - - - - - O- - -
             |
             | pnext1    ^
             v           | pedge2
             O---------->O

             now there are two separate linked lists, from
             pnext2 to pedge1 and from pnext1 to pedge2. We
             add edges that connect pedge1 to pnext2 and pedge2
             to pnext1 to create 2 cyclicly linked boundaries
         */

        bsp_edge *psplit1 = new bsp_edge(pnext1->pvert, pnext1->pnorm, false, split),
                 *psplit2 = new bsp_edge(pnext2->pvert, pnext2->pnorm, false, split);

        //add psplit1
        pedge1->pnext  = psplit1;
        psplit1->pnext = pnext2;

        //add psplit2
        pedge2->pnext  = psplit2;
        psplit2->pnext = pnext1;

        /*
           We assume that the two split edges are only incident to
           each other and no other edge.
           The above assumption is not valid when one of the edges of
           the fragment that is being used as partition plane is
           incident to the current fragment, since then would the (part
           of the) incident edge of the partition fragment also be incident
           to the split edges.
         */
        psplit1->pincident = psplit2;
        psplit2->pincident = psplit1;

        /*
               O<----------O
        pedge1 |           ^
               v  psplit1  | pnext2
               ----------->|
          - - -O - - - - - O- - -
               |<-----------
        pnext1 |  psplit2  ^
               v           | pedge2
               O---------->O
         */


        //reuse pwhole and set it as the up part
        pwhole->pbound = pedge1;
        pupfrag = pwhole;
        //create a new fragment for the lower part
        pdownfrag = new poly_frag(pwhole->ppoly, pedge2);

        //check if pup is really up by classifying the vertex of
        //pedge1 against the plane (which is guaranteed not incident)
        if (P.classify(*pedge1->pvert) == below)
            std::swap(pupfrag, pdownfrag);

        //increment the split count
        stats.nsplits++;
    }

    void poly_frag_intersect_edges(poly_frag *pfrag,
                               const plane &P,
                               bsp_edge  *&pedge1,
                               bsp_edge  *&pedge2,
                               bsp_stats &stats,
                               vertex_store &store)
    {
        /*
           precondition pfrag->classify(P) == spanning
           implies that there are at least one vertex in the
           front and one in the back of the plane P, however
           since the plane-point classification uses a thick
           plane it can happen that multiple vertices in a row
           are classified as incident. Because the fragment
           is convex and spans the plane, there are two
           intersections with the boundary

           This algorithm searches the first edge whos vertex
           plugged into the plane equation gives a positive result
           then it searches for the edge whos endpoint is either on
           the plane or on the other side. If only one of the
           endpoints is classified as incident that vertex is
           taken to be the intersection point, otherwise the
           edge is split. For the second intersection point
           the algorithm does the same.
         */
        bsp_edge *pprev = pfrag->pbound,
             *pedge = pfrag->pbound->pnext;
        (void)stats; //does nothing
        //set pedge to the first edge whos vertex is on the
        //positive side of the plane
        while (dot(P.n, *pedge->pvert) + P.D <= 0.0) {
            pprev = pedge;
            pedge = pedge->pnext;
        }

        //search for the first edge whos endpoint is on or behind
        //the plane
        while(dot(P.n, *pedge->pnext->pvert) + P.D > 0.0) {
            pprev = pedge;
            pedge = pedge->pnext;
        }

        //check if we have to split the edge or whether one of the
        //endpoints is considered incident to the plane. When both
        //endpoints are considered incident the edge still has to
        //be split, however we can choose to use one of the incident
        //vertices and avoid splitting nearly incident edges and
        //risk some hopefully unnoticeable visual artifacts
        if (P.classify(*pedge->pnext->pvert) == incident) {
            //the endpoint of pedge is considered incident,
            //use it as intersection point even when
            //the first vertex is incident too
            pedge1 = pedge;
        }
        else if (P.classify(*pedge->pvert) == incident) {
            //the first vertex of pedge is considered
            //incident and the endpoint not
            //hence pprev its endpoint is incident
            pedge1 = pprev;
        }
        else {
            //no vertex is considered incident split
            //the edge
            pedge1 = pedge;
            split_bsp_edge(P, pedge1, store);
        }

        //and now the same for the second intersection
        pprev = pedge1;
        pedge = pprev->pnext;

        //search for the first edge whos endpoint is on or in front of
        //the plane
        while(dot(P.n, *pedge->pnext->pvert) + P.D < 0.0) {
            pprev = pedge;
            pedge = pedge->pnext;
        }

        if (P.classify(*pedge->pnext->pvert) == incident) {
            pedge2 = pedge;
        }
        else if (P.classify(*pedge->pvert) == incident) {
            pedge2 = pprev;
        }
        else {
            pedge2 = pedge;
            split_bsp_edge(P, pedge2, store);
        }

    }

    void split_bsp_edge(const plane &P,
                    bsp_edge *pedge,
                    vertex_store &store)
    {
        const vertex *pfrom = pedge->pvert,
                     *pto = pedge->pnext->pvert,
                     *pnewvert;
        std::list<bsp_edge*> fromlist, tolist;

        //ray-plane intersection
        vertex dv(*pto - *pfrom);
        double t = P.intersect(*pfrom, dv);

        if (t < 0.0 || t > 1.0) {
            std::stringstream ss;
            ss << "split_bsp_edge : intersection not on edge\n" <<
                "\n\t from " << *pfrom << " (" << P.classify(*pfrom) <<  ") to " << *pto <<
                " (" << P.classify(*pto) << ")" << std::endl;
            throw ss.str();
        }

        //create new interpolated vertex, this one is shared
        //by all incident edges
        pnewvert = store.get_vertex(*pfrom + t*dv);

        bsp_edge *ptemp = pedge;
        //split all incident edges
        do {
            //check in which vertex this edge starts
            if (ptemp->pvert == pfrom) {
                /*
                      o pfrom               o pfrom
                 ptemp|                ptemp|
                      |                     v
                      |                     o pnewvert
                      |             pnewedge|
                      v                     v
                      o pto                 o pto
                 */
                const normal *pnewnorm = store.get_normal(
                       (*ptemp->pnorm + t * (*ptemp->pnext->pnorm - *ptemp->pnorm)).normalise());
                bsp_edge *pnewedge = new bsp_edge(pnewvert,
                                                  pnewnorm,
                                                  ptemp->edgeflag,
                                                  ptemp->type,
                                                  ptemp->pnext);
                ptemp->pnext = pnewedge;
                fromlist.push_back(ptemp);
                tolist.push_back(pnewedge);
            } else {
                /*
                      o pfrom               o pfrom
                      ^                     ^
                      |             pnewedge|
                      |                     o pnewvert
                      |                     ^
                 ptemp|                ptemp|
                      o pto                 o pto
                 */
                const normal *pnewnorm = store.get_normal(
                       (*ptemp->pnext->pnorm + t * (*ptemp->pnorm - *ptemp->pnext->pnorm)).normalise());
                bsp_edge *pnewedge = new bsp_edge(pnewvert,
                                                  pnewnorm,
                                                  ptemp->edgeflag,
                                                  ptemp->type,
                                                  ptemp->pnext);
                ptemp->pnext = pnewedge;
                fromlist.push_back(pnewedge);
                tolist.push_back(ptemp);
            }
            ptemp = ptemp->pincident;
        } while (ptemp != pedge);

        //fix incidencies of the new edges
        std::list<bsp_edge*>::iterator it;
        bsp_edge *pprev = fromlist.back();
        for (it = fromlist.begin(); it != fromlist.end(); it++) {
            pprev->pincident = *it;
            pprev = *it;
        }

        pprev = tolist.back();
        for (it = tolist.begin(); it != tolist.end(); it++) {
            pprev->pincident = *it;
            pprev = *it;
        }
    }


    bool poly_frag_is_freesplit(const poly_frag *frag)
    {
        //loop through all edges until we
        //encounter an edge that is no
        //split edge we return false;
        bsp_edge *pedge = frag->pbound;
        do {
            if (pedge->type != split)
                return false;
            pedge = pedge->pnext;
        } while (pedge != frag->pbound);

        //all edges were splitedges
        return true;
    }


    void bsp_render_btf(bsp_node *pnode, const vect3d &campos)
    {
        if (!pnode)
            return;

        std::list<poly_frag*>::iterator it;

        if (pnode->type == leafnode) {
            for (it = pnode->frags.begin(); it != pnode->frags.end(); it++)
                (*it)->render();
            return;
        }

        switch (pnode->split.classify(campos)) {
        case above:
            bsp_render_btf(pnode->pdown, campos);
            for (it = pnode->frags.begin(); it != pnode->frags.end(); it++)
                (*it)->render();
            bsp_render_btf(pnode->pup, campos);
            return;

        case below:
            bsp_render_btf(pnode->pup, campos);
            for (it = pnode->frags.begin(); it != pnode->frags.end(); it++)
                (*it)->render();
            bsp_render_btf(pnode->pdown, campos);
            return;

        case incident:
        default:
            bsp_render_btf(pnode->pup, campos);
            bsp_render_btf(pnode->pdown, campos);
            return;
        }
    }


    std::ostream &operator<<(std::ostream &os, const bsp_edge &e)
    {
        return os << *e.pvert << " -(" << *e.pnorm//e.type
                 << ")-> ";
    }

    std::ostream &operator<<(std::ostream &os, const plane &P)
    {
        return os << "A: " << P.n.x << " B: " << P.n.y << " C: " <<
                  P.n.z << " D: " << P.D;
    }

    std::ostream &operator<<(std::ostream &os, const bsp_node &node)
    {
        std::list<poly_frag*>::const_iterator it;
        if (node.type == splitnode) {
            os << "Splitnode " << node.split << " frags(" << node.frags.size() << ") : ";
            for (it = node.frags.begin(); it != node.frags.end(); it++)
                os << "[" << *(*it) << "] ";
            os << std::endl;
            std::cout << "pup   ";
            if (node.pup) os << *node.pup;
            else os << "null" << std::endl;
            std::cout << "pdown ";
            if (node.pdown) os << *node.pdown;
            else os << "null" << std::endl;
        } else {
            os << "Leafnode ";
            for (it = node.frags.begin(); it != node.frags.end(); it++)
                os << "[" << *(*it) << "] ";
            os << std::endl;
        }

        return os  ;
    }

    std::ostream &operator<<(std::ostream &os, const bsp_stats &stats)
    {
        return os <<
          "number of objects: " << stats.nobjects+stats.nsplits << std::endl <<
          "original number  : " << stats.nobjects << " (" << stats.nsplits << ")" << std::endl <<
          "number of nodes  : " << stats.nsplitnodes + stats.nleafnodes << std::endl <<
          "    (split/leaf) : " << stats.nsplitnodes << "/" << stats.nleafnodes << std::endl <<
          "max tree depth   : " << stats.maxdepth << std::endl <<
          "free splits      : " << stats.nfreesplits << std::endl;
    }

    std::ostream &operator<<(std::ostream &os, const poly_frag &frag)
    {
        bsp_edge *pedge = frag.pbound;
        do {
            os << *pedge;
            pedge = pedge->pnext;
        } while (pedge != frag.pbound);
        return os;
    }

    std::ostream &operator<<(std::ostream &os, pclass pc) {
        switch (pc) {
        case above : return os << "above";
        case below : return os << "below";
        case incident : return os << "incident";
        case spanning : return os << "spanning";
        }
        return os;
    }

    /*
       testcode
    */
    /*
    int main(int argc, char **argv)
    {
        //test plane creation
        {
            vertex v1( 0.0, 0.0, -1.0),
                   v2(-1.0, 0.0,  0.0),
                   v3( 1.0, 0.0,  0.0);
            normal n(1.0, 0.0, 0.0);
            bsp_edge *edge1 = new bsp_edge(&v1, &n),
                 *edge2 = new bsp_edge(&v2, &n),
                 *edge3 = new bsp_edge(&v3, &n);
            edge1->pnext = edge2;
            edge2->pnext = edge3;
            edge3->pnext = edge1;

            poly_frag frag1(NULL, edge1);
            plane P;
            bsp_create_partition_plane(&frag1, P);

            cout << "[[Plane calculation test]]" << endl << " poly_frag: " <<
                frag1 << endl << " plane: " << P << endl << endl;
        }

        //edge split test
        {
            cout << endl << "[[split_bsp_edge test]]" << endl;
            vertex_store store;
            bsp_edge *e1_1 = new bsp_edge(store.get_vertex(vertex(1.0, 0.0, 0.0)),
                                          store.get_normal(normal(1.0, 0.0, 0.0))),
                     *e1_2 = new bsp_edge(store.get_vertex(vertex(-1.0, 0.0, 0.0)),
                                          store.get_normal(normal(1.0, 0.0, 0.0))),
                     *e2_1 = new bsp_edge(store.get_vertex(vertex(-1.0, 0.0, 0.0)),
                                          store.get_normal(normal(0.0, 1.0, 0.0))),
                     *e2_2 = new bsp_edge(store.get_vertex(vertex(1.0, 0.0, 0.0)),
                                          store.get_normal(normal(0.0, 1.0, 0.0))),
                     *e3_1 = new bsp_edge(store.get_vertex(vertex(1.0, 0.0, 0.0)),
                                          store.get_normal(normal(0.0, 0.0, 1.0))),
                     *e3_2 = new bsp_edge(store.get_vertex(vertex(-1.0, 0.0, 0.0)),
                                          store.get_normal(normal(0.0, 0.0, 1.0))),
                     *e4_1 = new bsp_edge(store.get_vertex(vertex(-1.0, 0.0, 0.0)),
                                          store.get_normal(normal(1.0, 0.0, 0.0))),
                     *e4_2 = new bsp_edge(store.get_vertex(vertex(1.0, 0.0, 0.0)),
                                          store.get_normal(normal(0.0, 1.0, 0.0)));
            e1_1->pnext = e1_2; e1_2->pnext = e1_1;
            e2_1->pnext = e2_2; e2_2->pnext = e2_1;
            e3_1->pnext = e3_2; e3_2->pnext = e3_1;
            e4_1->pnext = e4_2; e4_2->pnext = e4_1;

            e1_1->pincident = e2_1; e2_1->pincident = e3_1;
            e3_1->pincident = e4_1; e4_1->pincident = e1_1;

            poly_frag *p1 = new poly_frag(NULL, e1_1),
                  *p2 = new poly_frag(NULL, e2_1),
                  *p3 = new poly_frag(NULL, e3_1),
                  *p4 = new poly_frag(NULL, e4_1);

            cout << " poly_frags before split:" << endl;
            cout << "  p1: " << *p1 << endl;
            cout << "  p2: " << *p2 << endl;
            cout << "  p3: " << *p3 << endl;
            cout << "  p4: " << *p4 << endl;

            plane P(vect3d(1.0, 0.0, 0.0), 0.0);

            split_bsp_edge(P, e1_1, store);

            cout << " new vertex: " << *e1_1->pnext->pvert << endl;

            cout << " poly_frags after split:" << endl;
            cout << "  p1: " << *p1 << endl;
            cout << "  p2: " << *p2 << endl;
            cout << "  p3: " << *p3 << endl;
            cout << "  p4: " << *p4 << endl;

            cout << " e1_1 incidencies:" << endl;
            bsp_edge *pedge = e1_1;
            do {
                cout << "  edge " << *pedge->pvert << " -> " << *pedge->pnext->pvert << endl;
                pedge = pedge->pincident;
            } while (pedge != e1_1);

            cout << " e2_1 incidencies:" << endl;
            pedge = e2_1;
            do {
                cout << "  edge " << *pedge->pvert << " -> " << *pedge->pnext->pvert << endl;
                pedge = pedge->pincident;
            } while (pedge != e2_1);

            cout << " (vertex_store stats: " << store.vertices.size() << ", "
                 << store.normals.size() << ")" << endl;
        }

        //test polygon conversion
        //test poly_frag classification and splitting
        try {
            //create a polygon
            vertex_store store;
            edge *edge1 = new edge(store.get_vertex(vertex(-1.0, 1.0, 0.0)),
                                           store.get_normal(normal(0.0, 0.0, 1.0))),
                 *edge2 = new edge(store.get_vertex(vertex(-1.0, -1.0, 0.0)),
                                           store.get_normal(normal(0.0, 0.0, 1.0))),
                 *edge3 = new edge(store.get_vertex(vertex(1.0, -1.0, 0.0)),
                                           store.get_normal(normal(0.0, 0.0, 1.0))),
                 *edge4 = new edge(store.get_vertex(vertex(1.0, 1.0, 0.0)),
                                           store.get_normal(normal(0.0, 0.0, 1.0)));
            edge1->pnext = edge2;
            edge2->pnext = edge3;
            edge3->pnext = edge4;
            edge4->pnext = edge1;

            polygon poly(NULL, NULL, edge1);
            list<polygon*> polylist;
            polylist.push_back(&poly);
            vector<poly_frag*> fragvector;

            bsp_convert_objects(polylist, fragvector);
            poly_frag *pfrag = fragvector[0];

            cout << endl << "[[Object conversion test]]" << endl
                 << " poly_frag : " << *pfrag << endl;

            cout << endl << "[[poly_frag::classify test]]" << endl;
            cout << " (vertex_store stats: " << store.vertices.size() << ", "
                 << store.normals.size() << ")" << endl;
            plane P_i(vect3d(0.0, 1.0, 0.0), 0.0),  //spanning
                  P_0(vect3d(1.0, 0.0, 0.0), -3.0), //below
                  P_1(vect3d(0.0, 1.0, 0.0), 3.0),  //above
                  P_2(vect3d(0.0, 0.0, 1.0), 0.0);  //incident

            cout << " pfrag : " << *pfrag << endl;
            cout << " pfrag->classify() : " << endl
                 << "   P_i(" << P_i << ") : " << pfrag->classify(P_i) << endl
                 << "   P_0(" << P_0 << ") : " << pfrag->classify(P_0) << endl
                 << "   P_1(" << P_1 << ") : " << pfrag->classify(P_1) << endl
                 << "   P_2(" << P_2 << ") : " << pfrag->classify(P_2) << endl;

            cout << endl << "[[poly_frag_split test]]" << endl;
            poly_frag *pupfrag, *pdownfrag;
            bsp_stats stats;

            cout << " whole    : " << *pfrag << endl;

            poly_frag_split(P_i, pfrag, pupfrag, pdownfrag, stats, store);

            cout << " upfrag   : " << *pupfrag << endl
                 << " downfrag : " << *pdownfrag << endl;

        }
        catch (const string &errmsg)
        {
            cout << endl << "Exception : " << errmsg << endl;
        }

        {
            //bsp construction test
            vertex_store store;
            bsp_edge *e11 = new bsp_edge(store.get_vertex(vertex(0,1,0)),
                                         store.get_normal(normal(0,0,-1))),
                     *e12 = new bsp_edge(store.get_vertex(vertex(-1,0,0)),
                                         store.get_normal(normal(0,0,-1))),
                     *e13 = new bsp_edge(store.get_vertex(vertex(1,0,0)),
                                         store.get_normal(normal(0,0,-1))),
                     *e21 = new bsp_edge(store.get_vertex(vertex(0,1,-2)),
                                         store.get_normal(normal(1,0,0))),
                     *e22 = new bsp_edge(store.get_vertex(vertex(0,0,-3)),
                                         store.get_normal(normal(1,0,0))),
                     *e23 = new bsp_edge(store.get_vertex(vertex(0,0,-1)),
                                         store.get_normal(normal(1,0,0)));
            e11->pnext = e12; e12->pnext = e13; e13->pnext = e11;
            e21->pnext = e22; e22->pnext = e23; e23->pnext = e21;
            e11->pincident = e11; e12->pincident = e12; e13->pincident = e13;
            e21->pincident = e21; e22->pincident = e22; e23->pincident = e23;

            poly_frag *f1 = new poly_frag(NULL, e21),
                      *f2 = new poly_frag(NULL, e11);

            list<poly_frag*> fraglist;
            fraglist.push_back(f1);
            fraglist.push_back(f2);

            bsp_stats stats;
            bsp_node *proot = bsp_build_tree(fraglist, stats, store, 0);

            cout << endl << "[[bsp_build_tree test]]" << endl;
            cout << " poly_frags:" << endl << "  " << *f1 << endl << "  " << *f2 << endl;
            cout << " BSP tree:" << endl << *proot << endl;
        }

        return 0;
    }
    */

} // namespace visualization
} // namespace bso

#endif //VIS_BSP_HPP

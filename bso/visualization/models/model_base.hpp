#ifndef VIS_MODEL_BASE_HPP
#define VIS_MODEL_BASE_HPP

#ifndef M_PI
#define M_PI 3.141592653589793238463
#endif // M_PI

#include <bso/visualization/utilities/datatypes.hpp>
#include <bso/visualization/utilities/vertex_store.hpp>
#include <bso/visualization/utilities/aabbox.hpp>
#include <bso/visualization/bsp/bsp.hpp>
#include <bso/visualization/utilities/objects.hpp>
#include <bso/visualization/utilities/camera.hpp>

#include <list>
#include <string>
#include <memory>

namespace bso { namespace visualization {

//forward declaration
class random_bsp;
class camera;
class polygon;
class label;
struct polygon_props;
struct line_props;
struct label_props;

/*
	 This is the base class off all models.
	 It contains the geometry of a model, which is assumed to
	 be static (no parts of it move independently). The base
	 class has protected member functions that can be used by
	 sub classes to add geometry, these functions use a vertex
	 store to store all vertices and vertex normals.
 */
class model_base
{
protected:

	vertex_store   store;
	
	aabboxd        bbox; //the bounding box of the model
											 //the functions below that create
											 //polygons, cubes, lines segments and
											 //labels will update the bbox accordingly
	
	polygon* create_polygon(polygon_props *pprops, line_props *lprops,
						vertex vertarr[], normal normarr[], int indices[], int vcount);
	polygon* create_polygon(polygon_props *pprops, line_props *lprops,
						vertex vertarr[], normal normarr[], int indices[], 
						bool edgeflags[], int vcount);
											 
	void addLineSegment(std::list<polygon*> &plist, bso::utilities::geometry::line_segment const* l,
											polygon_props* pprops, line_props* lprops, const double& width = 0.0,
											const bso::utilities::geometry::vector& widthDir = {0,0,0},
											const double& thickness = 0);
	void addPolygon(std::list<polygon*> &plist, bso::utilities::geometry::polygon const* p,
									polygon_props* pprops, line_props* lprops,
									const double& thickness = 0);
	void addPolyhedron(std::list<polygon*> &plist, bso::utilities::geometry::polyhedron const* p,
										 polygon_props* pprops, line_props* lprops);

	void addLabel(std::list<label*>& llist, label_props* lbprops, const std::string &text,
											const bso::utilities::geometry::vertex& pos);

public:
	virtual ~model_base() {}
	
	virtual void render(const camera &cam) const = 0;
	inline const aabboxd& get_bbox() const { return bbox; }
	virtual const std::string get_description() = 0;
	virtual bool key_pressed(int key) { (void)key; return false; }
	
}; // model_base

void model_base::addLineSegment(std::list<polygon*> &plist, bso::utilities::geometry::line_segment const* l,
									polygon_props* pprops, line_props* lprops, const double& width /*= 0*/,
									const bso::utilities::geometry::vector& widthDir /*= {0,0,0}*/,
									const double& thickness /*= 0*/)
{
	auto vec = l->getVector();
	auto widthDirCopy = widthDir;
	auto widthCopy = width;
	if (width <= 0)
	{ // add only the line
		widthDirCopy = {0,0,1000};
		if (vec.isVertical())
		{
			widthDirCopy = {1,0,0};
			widthDirCopy = vec.cross(widthDirCopy);
		}
		else
		{
			widthDirCopy = vec.cross(widthDirCopy);
		}
		widthDirCopy.normalize();
		widthCopy = l->getLength()*1e-3;
	}

	if (widthDirCopy.isZero(1e-9) || !(vec.isPerpendicular(widthDirCopy,1e-9)))
	{
		std::stringstream errorMessage;
		errorMessage << "\nError when trying to visualize a line segment.\n"
								 << "Trying to create a higher dimensional visualization\n"
								 << "without a valid directional vector.\n"
								 << "(bso/vizualization/models/model_base)" << std::endl;
		throw std::runtime_error(errorMessage.str());
	}
	std::vector<bso::utilities::geometry::vertex> widthVertices;
	for (const auto& i : *l)
	{
		widthVertices.push_back(i + widthDirCopy.normalized() * widthCopy / 2.0);
		widthVertices.push_back(i - widthDirCopy.normalized() * widthCopy / 2.0);
	}

	bso::utilities::geometry::quadrilateral p = widthVertices;
	this->addPolygon(plist,&p,pprops,lprops,thickness);

} // 

void model_base::addPolygon(std::list<polygon*> &plist, bso::utilities::geometry::polygon const* p,
									polygon_props* pprops, line_props* lprops,
									const double& thickness /*= 0*/)
{
	auto pNormal = p->getNormal();
	pNormal.normalize();
	if (thickness <= 0)
	{ // add the 2D polygon
		vertex vertices[4];
		std::copy(p->begin(), p->end(), std::begin(vertices));
		normal normals[] = {pNormal};
		int indices[2*std::distance(p->begin(), p->end())];
		for (unsigned int i = 0; i < std::distance(p->begin(), p->end()); ++i)
		{
			indices[i*2]   = i;
			indices[i*2+1] = 0;
		}

		plist.push_back(this->create_polygon(pprops,lprops,vertices,normals,
																				 indices,std::distance(p->begin(), p->end())));
	}
	else 
	{ // make a 3D polygon out of it
		std::vector<bso::utilities::geometry::vertex> thicknessVertices;
		for (auto i : *p)
		{
			thicknessVertices.push_back(i + pNormal * thickness / 2.0);
			thicknessVertices.push_back(i - pNormal * thickness / 2.0);
		}
		bso::utilities::geometry::quad_hexahedron threeDGeometry(thicknessVertices);
		this->addPolyhedron(plist,&threeDGeometry,pprops,lprops);
	}
} // 

void model_base::addPolyhedron(std::list<polygon*> &plist, bso::utilities::geometry::polyhedron const* p,
									 polygon_props* pprops, line_props* lprops)
{
	for (auto i = p->polygonBegin(); i != p->polygonEnd(); ++i)
	{
		this->addPolygon(plist, *i, pprops, lprops, 0.0);
	}
} // 

polygon *model_base::create_polygon(polygon_props *pprops,
										line_props *lprops,
										vertex vertarr[], //vertices
										normal normarr[], //normals
										int    indices[], //vertex indices
										int    vcount)     //number of vertices
{
		//create a circularly linked list of edges
		//copy the vertices and normals
		// hh, an edge has a vertex, normal, bool flagedge, and pointer pnext to next edge
		edge *pedge,
						 *pbound;
		bbox.add(vertarr[indices[0]]);
		pedge = pbound = new edge(
						store.get_vertex(vertarr[indices[0]]),
						store.get_normal(normarr[indices[1]]));

		for (int i=2; i<2*vcount; i+=2) {
				bbox.add(vertarr[indices[i]]);
				edge *pnew = new edge(
						store.get_vertex(vertarr[indices[i]]),
						store.get_normal(normarr[indices[i+1]]));

				pedge->pnext = pnew;
				pedge = pnew;
		}

		//close the circle
		pedge->pnext = pbound;

		//create a new polygon
		return new polygon(pprops, lprops, pbound);
}

polygon *model_base::create_polygon(polygon_props *pprops,
										line_props *lprops,
										vertex vertarr[], //vertices
										normal normarr[], //normals
										int    indices[], //vertex indices
										bool   edgeflags[],
										int    vcount)     //number of vertices
{
		//create a circularly linked list of edges
		//copy the vertices and normals
		edge *pedge,
						 *pbound;
		bbox.add(vertarr[indices[0]]);
		pedge = pbound = new edge(
						store.get_vertex(vertarr[indices[0]]),
						store.get_normal(normarr[indices[1]]),
						edgeflags[0]);

		for (int i=1; i<vcount; i++) {
				bbox.add(vertarr[indices[2*i]]);
				edge *pnew = new edge(
						store.get_vertex(vertarr[indices[2*i]]),
						store.get_normal(normarr[indices[2*i+1]]),
						edgeflags[i]);

				pedge->pnext = pnew;
				pedge = pnew;
		}

		//close the circle
		pedge->pnext = pbound;

		//create a new polygon
		return new polygon(pprops, lprops, pbound);
}

void model_base::addLabel(std::list<label*>& llist, label_props *lbprops, const std::string &text,
										const bso::utilities::geometry::vertex& pos)
{
	bbox.add(vertex(pos));
	llist.push_back(new label(lbprops, text, store.get_vertex(pos)));
}



} // namespace visualization
} // namespace bso

#endif //VIS_MODEL_BASE_HPP

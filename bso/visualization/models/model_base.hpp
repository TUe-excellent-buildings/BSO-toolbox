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
	std::vector<std::pair<bso::utilities::geometry::vertex, bso::utilities::geometry::vector>> mCuttingPlanes;
	
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
	model_base(const std::vector<std::pair<bso::utilities::geometry::vertex, bso::utilities::geometry::vector>>& cuttingPlanes = {});
	virtual ~model_base() {}
	
	virtual void render(const camera &cam) const = 0;
	inline const aabboxd& get_bbox() const { return bbox; }
	virtual const std::string get_description() = 0;
	virtual bool key_pressed(int key) { (void)key; return false; }
	
}; // model_base

model_base::model_base(const std::vector<std::pair<bso::utilities::geometry::vertex, bso::utilities::geometry::vector>>& cuttingPlanes /*= {}*/)
:	mCuttingPlanes(cuttingPlanes)
{
	
}

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

		std::vector<bso::utilities::geometry::vertex> orderedVertices(std::distance(
			p->begin(),p->end()));
		std::copy(p->begin(),p->end(),orderedVertices.begin());
		for (const auto& i : mCuttingPlanes)
		{
			if (orderedVertices.empty()) break;
			auto cutPoint = i.first;
			auto cutNormal = i.second;
			std::vector<bso::utilities::geometry::vertex> newOrderedVertices;
			
			for (unsigned int j = 0; j < orderedVertices.size(); ++j)
			{
				auto v0 = orderedVertices[j];

				if (cutNormal.dot(v0-cutPoint) >= -1e-3)
				{ // if it is on the cutting plane, or on the right side of it, push it back to the new orderedVertices
					newOrderedVertices.push_back(v0);
					for (unsigned int k = 0; k < orderedVertices.size(); ++k)
					{
						v0 = orderedVertices[(j+k)%orderedVertices.size()];
						auto v1 = orderedVertices[(j+k+1)%orderedVertices.size()];
						bool v0Retained = (cutNormal.dot(v0-cutPoint) >= -1e-3);
						bool v1Retained = (cutNormal.dot(v1-cutPoint) >= -1e-3);
						
						if (!v0Retained && !v1Retained)
						{ // both of them are cut off, do nothing

						}
						else if (v0Retained && v1Retained)
						{ // none of them are cut off, can push back v1
							newOrderedVertices.push_back(v1);
						}
						else
						{ // only one of them is cut off, need to find the intersection point of line v0 - v1 with the cuttingplance
							//find intersection point
							bso::utilities::geometry::vector vec1 = v1-v0;
							bso::utilities::geometry::vector vec2 = cutPoint-v0;
							bso::utilities::geometry::vertex intersection;
							//check if the line intersects with the plane
							double r = (cutNormal.dot(vec2))/(cutNormal.dot(vec1));
							if (r > -1e-3 && r < (1 + 1e-3))
							{ // if it does, compute the point
								intersection = v0 + r * vec1;
							}
							else 
							{
								std::stringstream errorMessage;
								errorMessage << "\nError, while cutting a model, expected an intersection\n"
														 << "between a line segment and a plane, but did not find one: s = " 
														 << r << "\n"
														 << "(bso/vizualization/models/model_base.hpp)" << std::endl;
								throw std::runtime_error(errorMessage.str());
							}
							
							// check whcih points should be pushed back
							if (v0Retained)
							{ // only push back the intersection point
								if (r > 1e-3) newOrderedVertices.push_back(intersection);
							}
							else if (v1Retained)
							{ // push back intersection point, and then v1
								if (r < 1-1e-3) newOrderedVertices.push_back(intersection);
								if (k != orderedVertices.size()-1) newOrderedVertices.push_back(v1);
							}
						}
					}
					break;
				}
			}
			
			if (newOrderedVertices.size() < 3) newOrderedVertices.clear();
			orderedVertices = newOrderedVertices;
		}
		if (orderedVertices.empty()) return;

		vertex vertices[orderedVertices.size()];
		for (unsigned int i = 0; i < orderedVertices.size(); ++i)
		{
			vertices[i] = orderedVertices[i];
		}
		normal normals[] = {pNormal};
		int indices[2*orderedVertices.size()];
		for (unsigned int i = 0; i < orderedVertices.size(); ++i)
		{
			indices[i*2]   = i;
			indices[i*2+1] = 0;
		}
		plist.push_back(this->create_polygon(pprops,lprops,vertices,normals,
																				 indices,orderedVertices.size()));
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
	for (const auto& i : p->getPolygons())
	{
		this->addPolygon(plist, i, pprops, lprops, 0.0);
	}
	for (const auto& i : mCuttingPlanes)
	{
		auto cutPoint = i.first;
		auto cutNormal = i.second;
		std::vector<bso::utilities::geometry::vertex> unOrderedVertices;
		
		
		for (const auto& j : p->getLines())
		{
			bso::utilities::geometry::vector vec1 = j[1]-j[0];
			bso::utilities::geometry::vector vec2 = cutPoint-j[0];
			
			//check if the line intersects with the plane
			double r = (cutNormal.dot(vec2))/(cutNormal.dot(vec1));
			if (r > -1e-3 && r < (1 + 1e-3))
			{ // if it does, compute the point
				bso::utilities::geometry::vertex intersection = j[0] + r * vec1;
				bool alreadyFound = false;
				for (const auto& k : unOrderedVertices)
				{
					if (k.isSameAs(intersection))
					{
						alreadyFound = true;
						break;
					}
				}
				if (!alreadyFound) unOrderedVertices.push_back(intersection);
			}
		}
		
		if (unOrderedVertices.size() < 3) unOrderedVertices.clear();
		if (unOrderedVertices.empty()) continue;
		
		if (unOrderedVertices.size() == 3)
		{
			bso::utilities::geometry::triangle tr(unOrderedVertices);
			this->addPolygon(plist, &tr, pprops, lprops, 0.0);
		}
		else if (unOrderedVertices.size() == 4)
		{
			bso::utilities::geometry::quadrilateral quad(unOrderedVertices);
			this->addPolygon(plist, &quad, pprops, lprops, 0.0);
		}
		else 
		{ // error
			
		}
		
		/*bso::utilities::geometry::vertex center = {0,0,0};
		for (const auto& j : unOrderedVertices) center += j;
		center /= unOrderedVertices.size();
		bso::utilities::geometry::vector refx = *(unOrderedVertices.begin()) - center;
		refx.normalize();
		bso::utilities::geometry::vector refy = cutNormal.cross(refx);
		refy.normalize();
		
		std::map<double,bso::utilities::geometry::vertex> orderedVertices;
		
		for (const auto& j : unOrderedVertices)
		{
			bso::utilities::geometry::vector centerVect = j - center;
			centerVect.normalize();
			double angle = atan2(refy.dot(centerVect),refx.dot(centerVect));
			orderedVertices[angle] = j;
		}
		
		vertex vertices[orderedVertices.size()];
		unsigned int vertexCounter = 0;
		for (const auto& j : orderedVertices)
		{
			vertices[vertexCounter] = j.second;
			++vertexCounter;
		}
		bso::utilities::geometry::vector viewNormal = -1 * cutNormal;
		normal normals[] = {viewNormal};
		int indices[2*orderedVertices.size()];
		for (unsigned int j = 0; j < orderedVertices.size(); ++j)
		{
			indices[j*2]   = j;
			indices[j*2+1] = 0;
		}
		plist.push_back(this->create_polygon(pprops,lprops,vertices,normals,
																				 indices,orderedVertices.size()));
		*/
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

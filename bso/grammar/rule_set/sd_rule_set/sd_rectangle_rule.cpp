#ifndef BSO_GRAMMAR_SD_RECTANGLE_RULE_CPP
#define BSO_GRAMMAR_SD_RECTANGLE_RULE_CPP

namespace bso { namespace grammar { namespace rule_set { namespace sd_rule_set {

sd_rectangle_rule::sd_rectangle_rule(rectangle_property* rectangleProperty)
:	mRectangleProperty(rectangleProperty), mStructure(structural_design::component::structure()),
	mLoadPanel(structural_design::component::structure())
{

} // ctor()

sd_rectangle_rule::~sd_rectangle_rule()
{

} // dtor()

void sd_rectangle_rule::apply(bso::structural_design::sd_model& sd) const
{
	// create structure if applicable
	structural_design::component::geometry* quadGeometry = nullptr;
	if (mStructure.type() == "truss")
	{ // create two diagonal trusses
		auto trussComponent = sd.addGeometry(utilities::geometry::line_segment(
			{(*mRectangleProperty->getRectanglePtr())[0],(*mRectangleProperty->getRectanglePtr())[2]}));
		trussComponent->addStructure(mStructure);
		trussComponent = sd.addGeometry(utilities::geometry::line_segment(
			{(*mRectangleProperty->getRectanglePtr())[1],(*mRectangleProperty->getRectanglePtr())[3]}));
		trussComponent->addStructure(mStructure);
	}
	else if (mStructure.type() == "flat_shell")
	{ // create a flat shell at the location of the rectangle
		quadGeometry = sd.addGeometry(*(mRectangleProperty->getRectanglePtr()));
		quadGeometry->addStructure(mStructure);
	}
	else if (mStructure.type() == "quad_hexahedron")
	{
		quadGeometry = sd.addGeometry(*(mRectangleProperty->getRectanglePtr()));
	}
	
	// assign possible loads
	if (mRectangleProperty->isFloor() && !mRectangleProperty->isRoof() && mLoads != nullptr)
	{ // apply a floor load
		for (const auto& i : *mLoads)
		{
			if (i.first == "live_load")
			{
				if (quadGeometry == nullptr)
				{
					quadGeometry = sd.addGeometry(*(mRectangleProperty->getRectanglePtr()));
				}
				quadGeometry->addLoad(i.second);
			}
		}
	}
	// assign possible loads
	if (mRectangleProperty->isFloor() && mRectangleProperty->isRoof() && mLoads != nullptr)
	{ // apply a floor load
		for (const auto& i : *mLoads)
		{
			if (i.first == "roof_load")
			{
				if (quadGeometry == nullptr)
				{
					quadGeometry = sd.addGeometry(*(mRectangleProperty->getRectanglePtr()));
				}
				quadGeometry->addLoad(i.second);
			}
		}
	}
	if (mRectangleProperty->isExternal() && mRectangleProperty->isAboveGroundSurface() && mLoads != nullptr)
	{ // apply wind loading
		for (const auto& i : *mLoads)
		{
			if (!(i.first == "wind_pressure" || i.first == "wind_suction" ||
						i.first == "wind_shear")) continue;
			auto load = i.second;
			utilities::geometry::vector rectangleOutwardOrientation = -1*mRectangleProperty->getOrientation();
			rectangleOutwardOrientation.normalize();
			utilities::geometry::vector loadOrientation;
			if (load.DOF() == 0) loadOrientation = {1,0,0};
			else if (load.DOF() == 1) loadOrientation = {0,1,0};
			else if (load.DOF() == 2) loadOrientation = {0,0,1};
			else
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, while applying a sd rectangle rule, encountered a\n"
										 << "wind load define in DOF: " << load.DOF() << ". Which is not valid.\n"
										 << "(bso/grammar/rule_set/sd_rule_set/sd_rectangle_rule.cpp)" << std::endl;
				throw std::runtime_error(errorMessage.str());
			}
			if (load.magnitude() < 0) loadOrientation *= -1;
			double beta = abs(rectangleOutwardOrientation.getHeading().first - 
												loadOrientation.getHeading().first);
			if (beta > 180) beta = 360 - beta;

			if (i.first == "wind_pressure" && (90 < beta && beta <= 180) && 
				!rectangleOutwardOrientation.isVertical())
			{
				load *= loadOrientation.dot(-1*rectangleOutwardOrientation);
				// no need for division by norm product since they are both normalized
			}
			else if (i.first == "wind_suction" && (0 <= beta && beta < 90) && 
				!rectangleOutwardOrientation.isVertical())
			{
				load *= loadOrientation.dot(1*rectangleOutwardOrientation);
				// no need for division by norm product since they are both normalized
			}
			else if (i.first == "wind_shear" && ((90 <= beta && beta < 180) || 
				rectangleOutwardOrientation.isVertical()))
			{
				load *= (1 - loadOrientation.dot(1*rectangleOutwardOrientation));
				// no need for division by norm product since they are both normalized
			}
			else continue;
			if (quadGeometry == nullptr)
			{
				quadGeometry = sd.addGeometry(*(mRectangleProperty->getRectanglePtr()));
			}
			quadGeometry->addLoad(load);
		}
	}
	if ((quadGeometry != nullptr) && (quadGeometry->getStructures().size() == 0) &&
			(quadGeometry->getLoads().size() != 0))
	{ // a low density load panel is required
		if (mLoadPanel.type() == "flat_shell")
		{ // if such a load panel has been defined, add the structure to the geometry
			quadGeometry->addStructure(mLoadPanel);
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, while applying an sd rectangle rule, applied a load\n"
									 << "but there is no structure present to carry this load.\n"
									 << "And no suitable low stiffness load panel has been defined\n"
									 << "(bso/grammar/rule_set/sd_rule_set/sd_rectangle_rule.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
	}
} // apply()

void sd_rectangle_rule::assignStructure(structural_design::component::structure structure)
{
	mStructure = structure;
}

void sd_rectangle_rule::assignStructure(const std::vector<structural_design::component::structure>&
	potentialStructure)
{
	mStructure = structural_design::component::structure();
	mStructure.isGhostComponent() = true;
	for (const auto& i : potentialStructure)
	{
		if (!(i.isGhostComponent()) && mStructure.isGhostComponent())
		{
			mStructure = i;
			continue;
		}
		else if (!(mStructure.isGhostComponent()) && i.isGhostComponent())
		{
			continue;
		} // otherwise either both are ghost component or both are not
		
		if (i.type() == "quad_hexahedron")
		{ // quad_hexahedron
			if (mStructure.type() != "quad_hexahedron")
			{
				mStructure = i;
			}
			else if (mStructure.type() == "quad_hexahedron")
			{
				if (mStructure.E() < i.E())
				{
					mStructure = i;
				}
			}
		}
		else if (i.type() == "flat_shell")
		{ // flat shell
			if (mStructure.type() != "flat_shell" && mStructure.type() != "quad_hexahedron")
			{
				mStructure = i;
			}
			else if (mStructure.type() == "flat_shell")
			{
				if (mStructure.thickness() < i.thickness())
				{
					mStructure = i;
				}
			}
		}
		else if (i.type() == "beam")
		{ // beam
			if (mStructure.type() != "beam" && mStructure.type() != "flat_shell" &&
					mStructure.type() != "quad_hexahedron")
			{
				mStructure = i;
			}
			else if (mStructure.type() == "beam")
			{
				if (mStructure.width()*mStructure.height() < i.width()*i.height())
				{
					mStructure = i;
				}
			}
		}
		else if (i.type() == "truss")
		{ // truss
			if (mStructure.type() != "beam" && mStructure.type() != "flat_shell" &&
					mStructure.type() != "truss" && mStructure.type() != "quad_hexahedron")
			{
				mStructure = i;
			}
			else if (mStructure.type() == "truss")
			{
				if (mStructure.A() < i.A())
				{
					mStructure = i;
				}
			}
		}
	}
} // assignStructure()

void sd_rectangle_rule::assignLoadPanel(structural_design::component::structure loadPanel)
{
	mLoadPanel = loadPanel;
	mLoadPanel.isGhostComponent() = true;
	mLoadPanel.isVisible() = false;
}

void sd_rectangle_rule::addLoads(std::multimap<std::string, structural_design::component::load>* loads)
{
	mLoads = loads;
} // addLoads()

} // namespace sd_rule_set
} // namespace rule_set
} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_SD_RECTANGLE_RULE_CPP
#ifndef BSO_GRAMMAR_SD_LINE_RULE_CPP
#define BSO_GRAMMAR_SD_LINE_RULE_CPP

namespace bso { namespace grammar { namespace rule_set { namespace sd_rule_set {

sd_line_rule::sd_line_rule(line_property* lineProperty)
: mLineProperty(lineProperty), mStructure(structural_design::component::structure("none",{}))
{
	
} // ctor()

sd_line_rule::~sd_line_rule()
{
	
} // dtor()

void sd_line_rule::apply(bso::structural_design::sd_model& sd) const
{
	// create structure if applicable
	structural_design::component::geometry* lineGeometry = nullptr;
	if (mStructure.type() == "truss" || mStructure.type() == "beam")
	{
		lineGeometry = sd.addGeometry(*(mLineProperty->getLinePtr()));
		lineGeometry->addStructure(mStructure);
	}
		
	if (mLineProperty->isHorizontal() && !(mLineProperty->isAboveGroundSurface()) &&
			mLineProperty->isSpaceEdge())
	{
		if (lineGeometry == nullptr)
		{
			lineGeometry = sd.addGeometry(*(mLineProperty->getLinePtr()));
		}
		lineGeometry->addConstraint(structural_design::component::constraint(0));
		lineGeometry->addConstraint(structural_design::component::constraint(1));
		lineGeometry->addConstraint(structural_design::component::constraint(2));
	}
	
} // apply()

void sd_line_rule::assignStructure(const structural_design::component::structure& structure)
{
	mStructure = structure;
} // assignStructure()
 
void sd_line_rule::assignStructure(const std::vector<structural_design::component::structure>&
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
	

} // namespace sd_rule_set
} // namespace rule_set
} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_SD_LINE_RULE_CPP
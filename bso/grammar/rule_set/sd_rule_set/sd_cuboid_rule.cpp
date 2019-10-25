#ifndef BSO_GRAMMAR_SD_CUBOID_RULE_CPP
#define BSO_GRAMMAR_SD_CUBOID_RULE_CPP

namespace bso { namespace grammar { namespace rule_set { namespace sd_rule_set {

sd_cuboid_rule::sd_cuboid_rule(cuboid_property* cuboidProperty)
:   mCuboidProperty(cuboidProperty), mStructure(structural_design::component::structure())
{

}//ctor()

sd_cuboid_rule::~sd_cuboid_rule()
{

}//dtor()


void sd_cuboid_rule::apply(bso::structural_design::sd_model& sd) const
{
    structural_design::component::geometry* quadGeometry = nullptr;
	if (mStructure.type() == "quad_hexahedron")
    { //create a guad_hexahedron at the location of the cuboid
        quadGeometry = sd.addGeometry(*(mCuboidProperty->getCuboidPtr()));
        quadGeometry->addStructure(mStructure);
    }
}

void sd_cuboid_rule::assignStructure(structural_design::component::structure structure)
	{
	    mStructure = structure;
	}

} // namespace sd_rule_set
} // namespace rule_set
} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_SD_CUBOID_RULE_CPP

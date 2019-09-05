#ifndef BSO_GRAMMAR_SD_VERTEX_RULE_CPP
#define BSO_GRAMMAR_SD_VERTEX_RULE_CPP

namespace bso { namespace grammar { namespace rule_set { namespace sd_rule_set {

sd_vertex_rule::sd_vertex_rule(vertex_property* vertexProperty)
: mVertexProperty(vertexProperty)
{
	
} // ctor()

sd_vertex_rule::~sd_vertex_rule()
{
	
} // dtor()

void sd_vertex_rule::apply(bso::structural_design::sd_model& sd) const
{
	sd.addPoint(*(mVertexProperty->getVertexPtr()));
} // apply()

} // namespace sd_rule_set
} // namespace rule_set
} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_SD_VERTEX_RULE_CPP
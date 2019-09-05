#ifndef BSO_GRAMMAR_SD_VERTEX_RULE_HPP
#define BSO_GRAMMAR_SD_VERTEX_RULE_HPP

#include <bso/grammar/rule_set/vertex_property.hpp>
#include <bso/grammar/rule_set/sd_rule_set/sd_rule_set.hpp>

namespace bso { namespace grammar { namespace rule_set { namespace sd_rule_set {

class sd_vertex_rule : public sd_rule_set
{
private:
	vertex_property* mVertexProperty;

public:
	sd_vertex_rule(vertex_property* vertexProperty);
	~sd_vertex_rule();

	void apply(bso::structural_design::sd_model& sd) const;
};
	
} // namespace sd_rule_set
} // namespace rule_set
} // namespace grammar
} // namespace bso

#include <bso/grammar/rule_set/sd_rule_set/sd_vertex_rule.cpp>

#endif // BSO_GRAMMAR_SD_VERTEX_RULE_HPP
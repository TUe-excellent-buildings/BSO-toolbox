#ifndef BSO_GRAMMAR_SD_LINE_RULE_HPP
#define BSO_GRAMMAR_SD_LINE_RULE_HPP

#include <bso/grammar/rule_set/line_property.hpp>
#include <bso/grammar/rule_set/sd_rule_set/sd_rule_set.hpp>

namespace bso { namespace grammar { namespace rule_set { namespace sd_rule_set {

class sd_line_rule : public sd_rule_set
{
private:
	line_property* mLineProperty;

	structural_design::component::structure mStructure;
public:
	sd_line_rule(line_property* lineProperty);
	~sd_line_rule();

	void apply(bso::structural_design::sd_model& sd) const;
	void assignStructure(const structural_design::component::structure& structure);
	void assignStructure(const std::vector<structural_design::component::structure>&
				potentialStructure);
	const structural_design::component::structure& getStructure() const {return mStructure;}
};
	
} // namespace sd_rule_set
} // namespace rule_set
} // namespace grammar
} // namespace bso

#include <bso/grammar/rule_set/sd_rule_set/sd_line_rule.cpp>

#endif // BSO_GRAMMAR_SD_LINE_RULE_HPP
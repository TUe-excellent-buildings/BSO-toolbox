#ifndef BSO_GRAMMAR_SD_RULE_SET_HPP
#define BSO_GRAMMAR_SD_RULE_SET_HPP

#include <bso/grammar/rule_set/sd_rule_set/sd_rule_set.hpp>

namespace bso { namespace grammar { namespace rule_set { namespace sd_rule_set {

class sd_rule_set
{
private:
	
public:
	sd_rule_set();
	~sd_rule_set();
	
	virtual void apply(bso::structural_design::sd_model& sd) const = 0;

};
	
} // namespace sd_rule_set
} // namespace rule_set
} // namespace grammar
} // namespace bso

#include <bso/grammar/rule_set/sd_rule_set/sd_rule_set.cpp>

#endif // BSO_GRAMMAR_SD_RULE_SET_HPP
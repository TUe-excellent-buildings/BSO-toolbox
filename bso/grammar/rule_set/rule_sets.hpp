#ifndef BSO_GRAMMAR_RULE_SETS_HPP
#define BSO_GRAMMAR_RULE_SETS_HPP

#include <bso/grammar/rule_set/property_base.hpp>
#include <bso/grammar/rule_set/vertex_property.hpp>
#include <bso/grammar/rule_set/line_property.hpp>
#include <bso/grammar/rule_set/rectangle_property.hpp>
#include <bso/grammar/rule_set/cuboid_property.hpp>

#include <bso/grammar/rule_set/point_property.hpp>
#include <bso/grammar/rule_set/edge_property.hpp>
#include <bso/grammar/rule_set/surface_property.hpp>
#include <bso/grammar/rule_set/space_property.hpp>

#ifdef SD_MODEL_HPP
#include <bso/grammar/rule_set/sd_rule_set/sd_rule_sets.hpp>
#endif

#ifdef BSO_BP_MODEL_HPP
#include <bso/grammar/rule_set/bp_rule_set/bp_rule_sets.hpp>
#endif

namespace bso { namespace grammar { namespace rule_set {


	
} // namespace rule_set
} // namespace grammar
} // namespace bso



#endif // BSO_GRAMMAR_RULE_SETS_HPP
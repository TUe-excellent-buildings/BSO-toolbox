#ifndef BSO_BP_STATES_HPP
#define BSO_BP_STATES_HPP

// dependent states
#include <bso/building_physics/state/dependent/wall.hpp>
#include <bso/building_physics/state/dependent/floor.hpp>
#include <bso/building_physics/state/dependent/window.hpp>
#include <bso/building_physics/state/dependent/space.hpp>

// independent states
#include <bso/building_physics/state/independent/weather_profile.hpp>
#include <bso/building_physics/state/independent/ground_profile.hpp>

namespace bso { namespace building_physics { namespace state {

// for convenience also define these states in namespace state
typedef dependent::dependent_state dependent_state;
typedef dependent::wall   wall;
typedef dependent::floor  floor;
typedef dependent::window window;
typedef dependent::space 	space;

typedef independent::independent_state independent_state;
typedef independent::weather_profile weather_profile;
typedef independent::ground_profile  ground_profile;

} // namespace state 
} // namespace building_physics 
} // namespace bso

#endif // BSO_BP_STATES_HPP
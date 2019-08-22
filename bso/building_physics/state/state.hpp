#ifndef BSO_BP_STATE_HPP
#define BSO_BP_STATE_HPP

namespace bso { namespace building_physics {
class state_space_system;
namespace state {

class state
{
protected:
	bool mIsDependent = false;
	bool mIsIndependent = false;
	bool mIsSpace = false;
	bool mIsFloor = false;
	bool mIsWall = false;
	bool mIsWindow = false;
	bool mIsWeatherProfile = false;
	bool mIsGroundProfile = false;
	
	std::string mDescription;
	
	unsigned int mIndex;
public:
	state(const unsigned int& index);
	virtual ~state() = 0;
	
	virtual void initSystem(bso::building_physics::state_space_system& system) = 0;
	virtual void updateSystem(bso::building_physics::state_space_system& system) = 0;

	virtual void setDescription(const std::string& description) {mDescription = description;}
	virtual const std::string& getDescription() const {return mDescription;}
	virtual const unsigned int& getIndex() const {return mIndex;}
	virtual const bool& isDependent()      const {return mIsDependent;}
	virtual const bool& isIndependent()    const {return mIsIndependent;}
	virtual const bool& isSpace()          const {return mIsSpace;}
	virtual const bool& isFloor()          const {return mIsFloor;}
	virtual const bool& isWall()           const {return mIsWall;}
	virtual const bool& isWindow()         const {return mIsWindow;}
	virtual const bool& isWeatherProfile() const {return mIsWeatherProfile;}
	virtual const bool& isGroundProfile()  const {return mIsGroundProfile;}
};

} // namespace state 
} // namespace building_physics 
} // namespace bso

#include <bso/building_physics/state_space_system.hpp>
#include <bso/building_physics/state/state.cpp>

#endif // BSO_BP_STATE_HPP
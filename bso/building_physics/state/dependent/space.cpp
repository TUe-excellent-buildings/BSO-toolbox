#ifndef BSO_BP_SPACE_CPP
#define BSO_BP_SPACE_CPP

namespace bso { namespace building_physics { namespace state { namespace dependent {

space::space(const unsigned int index, bso::utilities::geometry::polyhedron* geometry,
	const bso::building_physics::properties::space_settings& settings,
	bso::building_physics::state::state* outside_ptr)
: dependent_state(index), mGeometry(geometry->clone()), mVolume(geometry->getVolume()*1e-9),
	mSettings(settings)
{
	mIsSpace = true;
	mCapacitance = mVolume  * 1.2*1000*3; // 1.2 [kg/m³] air weight, 1000.0 [J/(K kg)] air specific heat, remaining factor takes furniture etc. into account

	if (mSettings.getACH() != 0)
	{
		// R_infiltration = 1/(m'*c) where c = 1000 [J/(K kg)],
		// m' = kg/sec = rho * V'; where rho = 1.2 [kg/m³], V' [mm³/sec] = Volume*ACH/3600
		mAdjacentStates[outside_ptr] = 1/((1000.0) * (1.2 * (mVolume * mSettings.getACH()/3600))); 
	}
} // ctor()

space::~space()
{
	delete mGeometry;
} // dtor()

void space::updateSystem(bso::building_physics::state_space_system& system)
{
	// the time difference that the system is being evaluated for
	double dt = abs((double)(system.getCurrentTime() - 
							system.getPreviousTime()).total_seconds());
	if (dt == 0) return; // nothing will change anyway
	
	// The prospected temperature if nothing changes
	double prospectedTemperature = system.getx()(mIndex) +
					((double)(system.getA().row(mIndex) * system.getx()) + 
					 (double)(system.getB().row(mIndex) * system.getu())) * dt; 
	
	// the limitations for the heating or cooling load
	double maxQ =  mSettings.getHeatingCapacity() * mVolume / mCapacitance;
	double minQ = -mSettings.getCoolingCapacity() * mVolume / mCapacitance;
	
	// the variable for the new and the current heating/cooling flow
	double Q = 0;
	double currentQ = system.getB()(mIndex,0); // current heating/cooling flow
	
	// estimated heat flows that would be required to reach each set point
	double QHeat = currentQ - (prospectedTemperature - mSettings.getHeatingSetPoint()) / dt;
	double QCool = currentQ - (prospectedTemperature - mSettings.getCoolingSetPoint()) / dt;
	
	// check which heating flow should apply
	// (depending if it is heating/cooling and if it is exceeding a constraint)
	if      (QHeat > 0 && QHeat <= maxQ) Q = QHeat;
	else if (QHeat > maxQ)               Q = maxQ;
	else if (QCool < 0 && QCool >= minQ) Q = QCool;
	else if (QCool < minQ)               Q = minQ;
	// std::cout << system.getx()(mIndex) << " -> " << prospectedTemperature << ": "
						// << currentQ << " ~ " << QHeat << " ~ " << QCool << " ----> " << Q << std::endl;
	// update cumulatives
	if (Q > 0)      mCumulativeHeatingEnergy += Q * dt * mCapacitance / 3.6e6;
	else if (Q < 0) mCumulativeCoolingEnergy += -Q * dt * mCapacitance / 3.6e6;

	// update the system
	system.getB()(mIndex,0) = Q;
}

void space::resetCumulativeEnergies()
{
	mCumulativeHeatingEnergy = 0.0;
	mCumulativeCoolingEnergy = 0.0;
}

bso::utilities::geometry::polyhedron* space::getGeometry() const
{
	try
	{
		if (std::distance(mGeometry->begin(), mGeometry->end()) == 0)
		{
			throw std::runtime_error("pointer does not exist anymore");
		}
	}
	catch (std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, requesting geometry pointer from space, but it does not\n"
								 << "seem to exist anymore. Was it deleted?\n"
								 << "(bso/building_physics/state/dependent/space.cpp)" << std::endl;
		throw std::runtime_error(errorMessage.str());
	}
	return mGeometry;
}

} // namespace dependent
} // namespace state 
} // namespace building_physics 
} // namespace bso

#endif // BSO_BP_SPACE_CPP
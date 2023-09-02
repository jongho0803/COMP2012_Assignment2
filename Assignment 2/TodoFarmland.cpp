#include "TodoFarmland.h"
#include "Employee.h"
#include <cstdlib>

// Farmland cost 50 upgrade cost 10
// Farmland size 5, 5
// Farmland init max num. of employee is FARMLAND_MAX_NUM_EMPLOYEE_MIN
// Farmland Upgrade:
//      level increases by 1;
//      max num. of employee increases by 1 until FARMLAND_MAX_NUM_EMPLOYEE_MAX
// Farmland Makemoney:
//      getRandInt(0, num. of employees in work x 2 + num. of farmers in work x 6 + level x 3)

const int FARMLAND_COST = 50;
const int FARMLAND_UPGRADE_COST = 10;
const int FARMLAND_SIZE_X = 5; const int FARMLAND_SIZE_Y = 5;
const int FARMLAND_MAX_NUM_EMPLOYEE_MIN = 3;
const int FARMLAND_MAX_NUM_EMPLOYEE_MAX = (FARMLAND_SIZE_X-2) * (FARMLAND_SIZE_Y-2);

#ifdef __HARVEST_MOON_DEBUG__
    inline void debug_getRandInt(string s) {cout << s << endl;};
#else
    inline void debug_getRandInt(string s) {};
#endif

//return random int in [low, high).
int getRandInt(int low, int high) {
    int res = rand() % high + low;
    debug_getRandInt("getRandInt: low is "+to_string(low));
    debug_getRandInt("getRandInt: high is "+to_string(high));
    debug_getRandInt("getRandInt: res is "+to_string(res));
    return res;
}

// TODO: Start to implement your code.
Farmland::Farmland(int x, int y)
	: Property(FARMLAND_COST, FARMLAND_UPGRADE_COST, FARMLAND_MAX_NUM_EMPLOYEE_MIN){
	setXY(x, y);
	setSize(FARMLAND_SIZE_X, FARMLAND_SIZE_Y);
}

bool Farmland::checkEmployee(Employee* checker) const{
	if(checker == nullptr)
		return false;
	if(checker->getSymbol() == 'e' || checker->getSymbol() == 'r')
		return true;
	return false;
}

void Farmland::upgrade(){
	Property::upgrade();
	if(getMaxNumEmployee() >= FARMLAND_MAX_NUM_EMPLOYEE_MAX)
		return;
	else{
		setMaxNumEmployee(getMaxNumEmployee() + 1);
	}
	return;
}

int Farmland::makeMoney () const{
	int num_employee = 0, num_farmer = 0;

	const Employee** employee_list = nullptr;
	getConstEmployeeList(employee_list);

	if(employee_list == nullptr)
		return 0;

	for(int i = 0; i < getNumEmployee(); i ++){
		if(employee_list[i]->getSymbol() == 'e' && employee_list[i]->getState() == ObjectState::WORK)
			++num_employee;
		else if(employee_list[i]->getSymbol() == 'r' && employee_list[i]->getState() == ObjectState::WORK)
			++num_farmer;
	}

	for(int i = 0; i < getNumEmployee(); i ++)
		employee_list[i] = nullptr;
	delete [] employee_list;
	employee_list = nullptr;

	int limit = num_employee * 2 + num_farmer * 6 + getLevel() * 3;

	return getRandInt(0, limit);
}

string Farmland::getName() const{
	return "Farmland";
}

char Farmland::getSymbol() const{
	return 'R';
}

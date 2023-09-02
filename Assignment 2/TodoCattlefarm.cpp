#include "TodoCattlefarm.h"
#include "Employee.h"
#include "TodoCow.h"
#include <cstdlib>

// Cattlefarm cost 80 upgrade cost 16
// Cattlefarm size 6, 6
// Cattlefarm init max num. of employee is CATTLEFARM_MAX_NUM_EMPLOYEE_MIN
// Cattlefarm Upgrade:
//      level increases by 1;
//      max num. of employees increases by 1 until CATTLEFARM_MAX_NUM_EMPLOYEE_MAX
// Cattlefarm Makemoney:
//      min(num. of cows, num. of feeders in work) x level x 10;

const int CATTLEFARM_COST = 80;
const int CATTLEFARM_UPGRADE_COST = 16;
const int CATTLEFARM_SIZE_X = 6; const int CATTLEFARM_SIZE_Y = 6;
const int CATTLEFARM_MAX_NUM_EMPLOYEE_MIN = 6;
const int CATTLEFARM_MAX_NUM_EMPLOYEE_MAX = (CATTLEFARM_SIZE_X-2) * (CATTLEFARM_SIZE_Y-2);

// TODO: Start to implement your code.

Cattlefarm::Cattlefarm(int x, int y)
	: Property(CATTLEFARM_COST, CATTLEFARM_UPGRADE_COST, CATTLEFARM_MAX_NUM_EMPLOYEE_MIN){
	setXY(x, y);
	setSize(CATTLEFARM_SIZE_X, CATTLEFARM_SIZE_Y);
}

bool Cattlefarm::checkEmployee(Employee* checker) const{
	if(checker == nullptr)
		return false;
	if(checker->getSymbol() == 'd' || checker->getSymbol() == 'c')
		return true;
	return false;
}

void Cattlefarm::upgrade(){
	Property::upgrade();
	if(getMaxNumEmployee() >= CATTLEFARM_MAX_NUM_EMPLOYEE_MAX)
		return;
	else{
		setMaxNumEmployee(getMaxNumEmployee() + 1);
	}
	return;
}

int Cattlefarm::makeMoney () const{
	int num_feeder = 0, num_cow = 0;

	const Employee** employee_list = nullptr;
	getConstEmployeeList(employee_list);

	if(employee_list == nullptr)
		return 0;

	for(int i = 0; i < getNumEmployee(); i ++){
		if(employee_list[i]->getSymbol() == 'd' && employee_list[i]->getState() == ObjectState::WORK)
			++num_feeder;
		else if (employee_list[i]->getSymbol() == 'c' && employee_list[i]->getState() == ObjectState::WORK)
			++num_cow;
	}

	for(int i = 0; i < getNumEmployee(); i ++)
		employee_list[i] = nullptr;
	delete [] employee_list;
	employee_list = nullptr;

	int base = min(num_feeder, num_cow);

	return base * getLevel() * 10;
}

void Cattlefarm::removeDiedCow (){
	const Employee** employee_list = nullptr;
	getConstEmployeeList(employee_list);

	if(employee_list == nullptr)
		return;

	Cow* cow = nullptr;
	int num_employee = getNumEmployee();

	for(int i = 0; i < num_employee; i ++){
		if(employee_list[i] -> getSymbol() == 'c'){
			cow = dynamic_cast<Cow*> (const_cast <Employee*> (employee_list[i]));
			if(!cow->isAlive())
				fireEmployee(cow);
		}
	}

	for(int i = 0; i < num_employee; i ++)
		employee_list[i] = nullptr;
	delete [] employee_list;
	employee_list = nullptr;

	return;
}

string Cattlefarm::getName() const{
	return "Cattlefarm";
}

char Cattlefarm::getSymbol() const{
	return 'C';
}

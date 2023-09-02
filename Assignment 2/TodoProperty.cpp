#include "Property.h"
#include "Employee.h"

void Property::writeToStream(ostream& os) const {
	Object::writeToStream(os);
	os << "Cost:" << getCost() << endl;
    os << "Upgrade Cost:" << getUpgradeCost() << endl;
    os << "Level:" << getLevel() << endl;
    os << "# of Employee:" << getNumEmployee() << " / " << getMaxNumEmployee() << endl;
}

void Property::upgrade() {
    m_level++;
}

void Property::getConstEmployeeList(const Employee**& employee) const {
	if (m_num_employee == 0) {
		employee =nullptr;
		return;
	}
	employee = new const Employee*[m_num_employee];
	for (int i = 0; i < m_num_employee; i++)
		employee[i] = m_employee_list[i];
}

// TODO: Start to implement your code.

// The first parameter is the cost to build the property.
// The second parameter is the cost to upgrade the property.
// The third parameter is the max number of employees.
Property::Property(int cost, int upgrade_cost, int max_num_employee)
	: m_cost(cost), m_upgrade_cost(upgrade_cost), m_max_num_employee(max_num_employee){
}

Property::~Property(){
	if(m_employee_list == nullptr)
		return;
	else{
		for(int i = 0; i < m_num_employee; i ++)
			delete m_employee_list[i];
		delete [] m_employee_list;
	}
	m_employee_list = nullptr;
	return;
}

// Return the building cost of the property.
int Property::getCost() const{
	return m_cost;
}

// Return the upgrade cost of the property.
int Property::getUpgradeCost() const{
	return m_upgrade_cost;
}

// Return the level of the property.
int Property::getLevel() const{
	return m_level;
}

// Return the number of the employees assigned to the property.
int Property::getNumEmployee() const{
	return m_num_employee;
}

// Return the max number of the employees.
int Property::getMaxNumEmployee() const{
	return m_max_num_employee;
}

// Set the max number of the employees.
void Property::setMaxNumEmployee(int max_num_employee){
	m_max_num_employee = max_num_employee;
}

// Returns the amount of the positions to be moved in x and y axis
void checkPlusXY(int &plus_x, int &plus_y, const int number_employee, const int size_x, const int size_y){
	plus_x = number_employee / (size_y - 2);
	plus_y = number_employee % (size_y - 2);
}

// Assign an employee to the property.
// You should update the m_employee_list (with shallow copying), m_num_employee
// and set the x,y of Employee.
// All the employees should always be inside the range of their property.
// If the Property, e.g.Farmland(R), is at (0, 3) with size (5, 5),
// the first employee should at (1, 4);
// the second employee should at (1, 5);
//  012346x
// 3RRRRR
// 4R147R
// 5R258R
// 6R369R
// 7RRRRR
// y
// If reaches m_max_num_employee, it fails to assign.
// If the employee already in the list, it fails to assign.
// If the employee is unsuitable to the property, it fails to assign.
// Return true, if assign successfully.
// Otherwise, return false.
bool Property::assignEmployee(Employee* applicant){
	if (m_num_employee == m_max_num_employee)
		return false;
	if(applicant == nullptr)
		return false;
	if (!checkEmployee(applicant))
		return false;

	int x{0}, y{0};
	getXY(x, y);

	if (m_num_employee == 0){
		applicant->setXY(x + 1, y + 1);
		applicant->updateState();
		m_employee_list = new Employee* [1];
		m_employee_list[0] = applicant;
		++m_num_employee;
		return true;
	}

	else{
		for(int i = 0; i < m_num_employee; i ++){
			if(m_employee_list[i] == applicant)
				return false;
		}

		int plus_x{0}, plus_y{0}, size_x{0}, size_y{0};
		getSize(size_x, size_y);
		checkPlusXY(plus_x, plus_y, m_num_employee, size_x, size_y);

		Employee** temp = new Employee*[m_num_employee + 1];
		for (int i = 0; i < m_num_employee; i++){
			temp[i] = m_employee_list[i];
		}
		applicant->updateState();
		applicant->setXY(x + plus_x + 1, y + plus_y + 1);
		temp[m_num_employee] = applicant;

		for(int i = 0; i < m_num_employee; i ++)
			m_employee_list[i] = nullptr;
		delete [] m_employee_list;
		m_employee_list = temp;
		temp = nullptr;

		++m_num_employee;
		return true;
	}
	return true;
}

// Fire an employee.
// You should update the m_employee_list, m_num_employee
// and completely delete the employee.
// Assumption: You can assume the input parameter is in the m_employee_list if m_employee_list is not nullptr.
// Hint: You have to take care of the position of other employees after deleting one.
// Otherwise, it may cause a problem when you assigning new employees.
// Please note all the employees should always be inside their property.
// We gave some examples to clarify the "inside" and "outside" on the webpage.
// Return true, if assign successfully.
bool Property::fireEmployee(Employee* farewell){
	if(farewell == nullptr)
		return false;
	if(m_employee_list == nullptr)
		return false;

	if(m_num_employee == 1){
		delete m_employee_list[0];
		delete [] m_employee_list;
		m_employee_list = nullptr;
		--m_num_employee;

		return true;
	}

	else{
		int pos{0};
		for(int i = 0; i < m_num_employee; i ++){
			if(m_employee_list[i] == farewell)
				pos = i;
		}

		for(int i = m_num_employee - 1; i > pos; i --){
			int x{0}, y{0};
			m_employee_list[i-1]->getXY(x, y);
			m_employee_list[i]->setXY(x, y);
		}

		Employee** temp = new Employee*[m_num_employee - 1];
		for (int i = 0, j = 0; i < m_num_employee - 1; i++, j++){
			if(j == pos){
				i--;
				continue;
			}
			else{
				temp[i] = m_employee_list[j];
			}
		}

		delete m_employee_list[pos];
		for(int i = 0; i < m_num_employee; i ++)
			m_employee_list[i] = nullptr;
		delete [] m_employee_list;

		m_employee_list = temp;
		temp = nullptr;

		--m_num_employee;
		return true;
	}
	return true;
}

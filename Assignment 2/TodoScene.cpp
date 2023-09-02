/*
 * File Created: Mon Mar 16 2020
 * Author:
 * SID:
*/

#include "Scene.h"
#include "Property.h"
#include "TodoFarmland.h"
#include "TodoCattlefarm.h"
#include "Employee.h"
#include "TodoFarmer.h"
#include "TodoFeeder.h"
#include "TodoCow.h"

const int SCENE_W = 45;
const int SCENE_H = 10;
const int PROPERTY_X = 35;
const int SCENE_INIT_MONEY = 200;

Scene::Scene()
: m_objects(nullptr), m_num_objects(0), m_money(SCENE_INIT_MONEY) {
    nextRound();
}

Scene::~Scene() {
    //shallowremove employees
    for(int i = m_num_objects-1; i >= 0; i--) {
        if (m_objects[i]->getObjectType()==ObjectType::EMPLOYEE)
            shallowRemoveEmployee(dynamic_cast<Employee*>(m_objects[i]));
    }
    // remove properties
    for(int i = m_num_objects-1; i >= 0; i--) {
        if (m_objects[i]->getObjectType()==ObjectType::PROPERTY)
            delete m_objects[i];
    }
    delete[] m_objects;
}

void Scene::shallowRemoveEmployee(Employee* e) {
    if (m_num_objects <= 0) return;
    if (m_num_objects == 1 && e == m_objects[0]) {
        m_num_objects = 0;
        delete [] m_objects;
        m_objects = nullptr;
        return;
    }
    int i;
	for (i = 0; i < m_num_objects; i++) {
		if (m_objects[i] == e)
			break;
	}
    Object ** newObjects = new Object*[m_num_objects--];
	for (int j = 0; j < m_num_objects; j++)
		newObjects[j] = m_objects[j];
	if (m_num_objects != i)
		newObjects[i] = m_objects[m_num_objects];
    delete [] m_objects;
    m_objects = newObjects;
}

bool Scene::checkOverlap(const Property* newproperty) {
    int x, y, sz_x, sz_y;
    newproperty->getXY(x, y);
    newproperty->getSize(sz_x, sz_y);
    for(int xx=x; xx<x+sz_x; xx++)
        for(int yy=y; yy<y+sz_y; yy++)
            if(getObjectAt(xx, yy) != nullptr) return true;
    return false;
}


void Scene::addProperty(int property, int x, int y) {
    Property* newProperty = nullptr;
    switch(property) {
    case FARMLAND:
        newProperty = new Farmland(x,y);
        break;
    case CATTLEFARM:
        newProperty = new Cattlefarm(x, y);
        break;
    }
    if (newProperty == nullptr) return;
    if (newProperty->getCost() > m_money || checkOverlap(newProperty)) {
        delete newProperty;
        return;
    }
    addObject(newProperty);
    m_money -= newProperty->getCost();
}


bool Scene::hire(Property* p, int employee) {
    Employee* newEmployee = nullptr;
    switch(employee){
    case EMPLOYEE:
        newEmployee = new Employee();
        break;
    case FARMER:
        newEmployee = new Farmer();
        break;
    case FEEDER:
        newEmployee = new Feeder();
        break;
    case COW:
        newEmployee = new Cow();
        break;
    }
    if (newEmployee == nullptr) return false;
    if (newEmployee->getCost() > m_money || ! p->assignEmployee(newEmployee)) {
        delete newEmployee;
        return false;
    }
    addObject(newEmployee);
    m_money -= newEmployee->getCost();
    return true;
}


Object* Scene::getObjectAt(int s_x, int s_y) const {
	int x, y, sz_x, sz_y;
    // If employee is at s_x, s_y, get employee
    // else, get property
    // otherwise return null
	for (int i = 0; i < m_num_objects; i++) {
        if (m_objects[i]->getObjectType() == ObjectType::PROPERTY)
            continue;
		m_objects[i]->getXY(x,y);
        m_objects[i]->getSize(sz_x,sz_y);
        if ( s_x >= x && s_x < x + sz_x && s_y >= y && s_y < y + sz_y)
			return m_objects[i];
	}
	for (int i = 0; i < m_num_objects; i++) {
        if (m_objects[i]->getObjectType() == ObjectType::EMPLOYEE)
            continue;
		m_objects[i]->getXY(x,y);
        m_objects[i]->getSize(sz_x,sz_y);
        if ( s_x >= x && s_x < x + sz_x && s_y >= y && s_y < y + sz_y)
			return m_objects[i];
	}
	return nullptr;
}

//perform shallow copying
void Scene::addObject(Object* newobj) {
	Object** newobjects = new Object*[m_num_objects + 1];
	for (int i = 0; i < m_num_objects; i++)
		newobjects[i] = m_objects[i];
	newobjects[m_num_objects++] = newobj;
	if (m_num_objects != 1)
		delete [] m_objects;
	m_objects = newobjects;
}

void Scene::getConstObjects(const Object**& obj, int& count) const {
	count = m_num_objects;
	if (m_num_objects == 0) {
		obj =nullptr;
		return;
	}
	obj = new const Object*[m_num_objects];
	for (int i = 0; i < m_num_objects; i++)
		obj[i] = m_objects[i];
}

// TODO: Start to implement your code.


// If m_money < 0, return true.
bool Scene::isGameOver() const{
	if(m_money < 0)
		return true;

	return false;
}


// Remove the Property in the scene
void Scene::removeProperty(Property* farewell){
	if(farewell == nullptr)
		return;
	if (m_num_objects <= 0) return;
	if (m_num_objects == 1 && farewell == m_objects[0]) {
	    m_num_objects = 0;
	    delete m_objects[0];
	    delete [] m_objects;
	    m_objects = nullptr;
	    return;
	}

	const Employee** employee_list = nullptr;
	farewell->getConstEmployeeList(employee_list);

	for(int i = 0; i < farewell->getNumEmployee(); i++){
		shallowRemoveEmployee(const_cast <Employee*> (employee_list[i]));
	}

	for(int i = 0; i < farewell->getNumEmployee(); i ++)
		employee_list[i] = nullptr;
	delete [] employee_list;
	employee_list = nullptr;

	int pos{0};
	for (pos = 0; pos < m_num_objects; pos++) {
		if (m_objects[pos] == farewell)
			break;
	}

	Object ** newObjects = new Object*[--m_num_objects];
	for (int i = 0; i < m_num_objects; i++)
		newObjects[i] = m_objects[i];

	if (m_num_objects != pos){
		newObjects[pos] = m_objects[m_num_objects];
	}
	delete m_objects[pos];
	m_objects[pos] = nullptr;
	delete [] m_objects;
	m_objects = newObjects;
	newObjects = nullptr;

	return;
}


// Operations to finish this round before entering the next round.
// - Make money
// - Compute salary
// - Update employee work age
// - Update employee status
// - Remove died cows
void Scene::nextRound(){
	for (int i = 0; i < m_num_objects; i++) {
		if (m_objects[i]->getObjectType() == ObjectType::EMPLOYEE)
			continue;
		Property* property = dynamic_cast <Property*> (m_objects[i]);
		m_money += property->makeMoney();
		property = nullptr;
	}

	for (int i = 0; i < m_num_objects; i++) {
	    if (m_objects[i]->getObjectType() == ObjectType::PROPERTY){
	        continue;
	    }
		Employee* employee = dynamic_cast <Employee*> (m_objects[i]);
		m_money -= employee->getSalary();
		employee->updateWorkAge();
		employee->updateState();
		employee = nullptr;
	}

	for (int i = 0; i < m_num_objects; i++) {
		if (m_objects[i]->getObjectType() == ObjectType::PROPERTY){
			continue;
		}
		if(m_objects[i] -> getSymbol() == 'c'){
			Cow* cow = nullptr;
			cow = dynamic_cast<Cow*> (m_objects[i]);
			if(!cow->isAlive()){
				shallowRemoveEmployee(cow);
				i--;
			}
			cow = nullptr;
		}
	}

	for (int i = 0; i < m_num_objects; i++) {
		if (m_objects[i]->getObjectType() == ObjectType::EMPLOYEE)
			continue;
		if(m_objects[i]->getSymbol() == 'C'){
			Cattlefarm* cattlefarm = dynamic_cast <Cattlefarm*> (m_objects[i]);
			cattlefarm->removeDiedCow();
			cattlefarm = nullptr;
		}
	}

	return;
}


// Upgrade property
bool Scene::upgrade(Property* levelUp){
	if(levelUp == nullptr){
		return false;
	}

	if(m_money < levelUp->getUpgradeCost()){
		return false;
	}

	m_money -= levelUp->getUpgradeCost();
	levelUp->upgrade();

	return true;
}


// Property fires an employee.
// The first parameter is an Employee pointer.
// You should find its property and fire the employee from the property.
bool Scene::fire(Employee* farewell){
	if(farewell == nullptr)
		return false;
	if (m_num_objects <= 0)
		return false;
	if (m_num_objects == 1 && farewell == m_objects[0]) {
		m_num_objects = 0;
	    delete m_objects[0];
	    delete [] m_objects;
	    m_objects = nullptr;
	    return true;
	}
	shallowRemoveEmployee(farewell);

	for (int i = 0; i < m_num_objects; i ++){
		if (m_objects[i]->getObjectType() == ObjectType::EMPLOYEE)
			continue;
		Property* property = nullptr;
		property = dynamic_cast <Property*> (m_objects[i]);
		const Employee** employee_list = nullptr;
		property->getConstEmployeeList(employee_list);
		int num_employee = property->getNumEmployee();

		for(int j = 0; j < num_employee; j++){
			if(employee_list[j] == farewell){
				property->fireEmployee(farewell);
			}
		}

		for(int j = 0; j < num_employee; j ++)
			employee_list[j] = nullptr;
		delete [] employee_list;
		property = nullptr;
		employee_list = nullptr;
	}
	return true;
}

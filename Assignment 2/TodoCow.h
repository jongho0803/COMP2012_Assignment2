#ifndef COW_H_
#define COW_H_

#include "Employee.h"

// TODO: Start to implement your code.
class Cow: public Employee{

private:
	const int m_lifespan;

public:
	Cow();

	~Cow();

	string getName() const override;

	char getSymbol() const override;

	bool isAlive() const;

	int getLifeSpan() const;

	void writeToStream(ostream& os) const;
};

#endif /*COW_H_*/

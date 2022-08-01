#include <iostream>
#include <string>

using namespace std;

class Customer
{
public:
	int id;
	int asset;
	
	bool active_state;

	string name;
	string lastname;

	Customer()
	{
		this->name = "";
		this->lastname = "";
		this->id = -1;
		this->asset = 0;
		this->active_state = true;
	}
};

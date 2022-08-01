#include <iostream>
#include <string>

using namespace std;

const int prime = 98317;

class HashNode
{
public:
	HashNode* next;
	string data;
	bool active_state;
	HashNode()
	{
		this->next = NULL;
		data = "";
		active_state = true;
	}
};


class HashTable
{
public:
	HashNode* table[prime];
	HashTable()
	{
		for(int i = 0; i < prime; i++)
		{
			this->table[i] = NULL;
		}
	}

	int loc(string s)
	{
		int loc = 1;
		for(int i = 0; i < s.size(); i++)
		{
			loc = (loc*int(s[i]))%prime;
		}
		return loc;
	}

	void add(string s)
	{
		int pos = loc(s);
		HashNode* current = this->table[pos];
		if(current == NULL)
		{
			this->table[pos] = new HashNode();
			this->table[pos]->data = s;
			return;
		}
		while(current->next != NULL)
		{
			if(current->data == s and current->active_state == false)
			{
				current->active_state = true;
				return;
			}
			current = current->next;
		}
		current->next = new HashNode();
		current->next->data = s;
		return;
	}

	void deactive(string s)
	{
		int pos = loc(s);
		HashNode* current = this->table[pos];
		while(current != NULL)
		{
			if(current->data == s)
			{
				current->active_state = false;
				return;
			}
			current = current->next;
		}
		cout<<"INTERNAL ERROR !!! REMOVEING NOT EXISTING NAME FROM NAMETABLE"<<endl;
		return;
	}

	bool registered_check(string s)
	{
		int pos = loc(s);
		HashNode* current = this->table[pos];
		while(current != NULL)
		{
			if(current->data == s and current->active_state == true)
			{
				return true;
			}
			current = current->next;
		}
		return false;
	}
};

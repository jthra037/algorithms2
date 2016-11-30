#include <iostream>
#include <string>

using namespace std;

class Weapon 
{
public:

	string weaponName;
	int range;
	int damage;
	float weight;
	float cost;

	Weapon(string n, int rang, int dam, float w, float c) 
	{
		weaponName = n;
		damage = dam;
		range = rang;
		weight = w;
		cost = c;
	}

	bool operator<(const Weapon &other)
	{
		return weaponName < other.weaponName;
	}
};

class bstree
{
private:

	class node
	{
	public:
		node(Weapon wep) : value(wep) { }
		~node() {}

		Weapon value;
		node *left = nullptr;
		node *right = nullptr;
	} *root = nullptr;

public:

	bstree() {}
	~bstree() {}

	// insertion
	void put(Weapon value)
	{
		node *curr = root;

		while (true)
		{
			if (curr == nullptr)
			{
				curr = new node(value);
				return;
			}

			if (value < curr->value)
			{
				curr = curr->left;
			}
			else
			{
				curr = curr->right;
			}
		}
	}

	// search and retrieval
	Weapon get(string key)
	{
		node *curr = root;
		while (curr != nullptr)
		{
			string currName = curr->value.weaponName;
			if (key == currName)
			{
				return curr->value;
			}
			else if (key < currName)
			{
				curr = curr->left;
			}
			else
			{
				curr = curr->right;
			}
		}
		return Weapon("NOT FOUND", 0, 0, 0, 0); // need a better way to do this
	}

	// delete
	bool remove(string key)
	{
		node *curr = root;
		while (curr != nullptr)
		{
			string currName = curr->value.weaponName;
			if (key == currName)
			{
				delete curr;
				curr = nullptr;
				return true;
			}
			else if (key < currName)
			{
				curr = curr->left;
			}
			else
			{
				curr = curr->right;
			}
		}
		return false;
	}

	// in order printing
	void inorder()
	{
		inorder(root);
	}

	// in order helper function
	void inorder(node *curr)
	{
		if (curr == nullptr) return;
		inorder(curr->left);
		cout << curr->value.weaponName << std::endl;
		inorder(curr->right);
	}
};

class hashTable 
{
public:

	int tableLength; // records the max size of the table
	int numItems; // records number of items in the list
	Weapon **table; //hashtable itself

	hashTable(int size) 
	{
		tableLength = size;
		numItems = 0;
		table = new Weapon *[tableLength];
		for (int i = 0; i < tableLength; i++) 
		{
			table[i] = NULL;
		}
	}

	int hash(string key) 
	{
		int value = 0;
		for (int i = 0; i < key.length(); i++)
		{
			value = value + key[i];
		}
		return value % tableLength;
	}

	void put(Weapon * item) 
	{
		int location = hash(item -> weaponName); //gets location in table based on name
		while (table[location] != NULL) 
		{
			location = (location + 1); // look one down
			location = location % tableLength; // to ensure wraparound at end of array
		}
		table[location] = item;
		numItems++;
	}

	Weapon * get(string key) 
	{
		int location = hash(key); //gets location in table based on key
		while (table[location] != NULL && key.compare(table[location]->weaponName) != 0) // not empty and not item
		{ 
			location = (location + 1); // look one down
			location = location % tableLength; // to ensure wraparound at end of array
		}
		return table[location];
	}

	void printTable() 
	{
		int count = 0;
		for (int x = 0; x < tableLength; x++) 
		{
			if (table[x] != NULL) 
			{
				cout << "Name: " << table[x]->weaponName << "   Damage:" << table[x]->damage << "    Cost:" << table[x]->cost << endl;
			}
		}
	}
};

class Player 
{
public:

	string name;
	Weapon * * backpack;
	int numItems;
	float money;

	Player(string n, float m)
	{
		name = n;
		money = m;
		numItems = 0;
		backpack = new Weapon *[10];
	}

	void buy(Weapon * w) 
	{
		cout << w->weaponName << " bought..." << endl;
		backpack[numItems] = w;
		numItems++;
		cout << numItems;
	}

	void withdraw(float amt) 
	{
		money = money - amt;
	}

	bool inventoryFull() 
	{
		bool full = false;
		if (numItems == 10) full = true;
		return full;
	}

	void printCharacter() 
	{
		cout << " Name:" << name << "\n Money:" << money << endl;
		printBackpack();
	}

	void printBackpack() 
	{
		cout << " " << name << ", you own " << numItems << " Weapons:" << endl;
		for (int x = 0; x < numItems; x++) 
		{
			cout << " " << backpack[x]->weaponName << endl;
		}
		cout << endl;
	}

};

void addWeapons(hashTable h) {
	cout << "***********WELCOME TO THE WEAPON ADDING MENU*********" << endl;
	string weaponName;
	int weaponRange;
	int weaponDamage;
	float weaponWeight;
	float weaponCost;
	cout << "Please enter the NAME of the Weapon ('end' to quit):";
	cin >> weaponName;
	while (weaponName.compare("end") != 0) 
	{
		cout << "Please enter the Range of the Weapon (0-10):";
		cin >> weaponRange;
		cout << "Please enter the Damage of the Weapon:";
		cin >> weaponDamage;
		cout << "Please enter the Weight of the Weapon (in pounds):";
		cin >> weaponWeight;
		cout << "Please enter the Cost of the Weapon:";
		cin >> weaponCost;
		Weapon * w = new Weapon(weaponName, weaponRange, weaponDamage, weaponWeight, weaponCost);
		h.put(w);
		cout << "Please enter the NAME of another Weapon ('end' to quit):";
		cin >> weaponName;
	}
}

void showRoom(hashTable ht, Player * p) 
{
	string choice;
	cout << "WELCOME TO THE SHOWROOM!!!!" << endl;
	ht.printTable();
	cout << " You have " << p->money << " money." << endl;
	cout << "Please select a weapon to buy('end' to quit):";
	cin >> choice;
	while (choice.compare("end") != 0 && !p->inventoryFull()) 
	{
		Weapon * w = ht.get(choice);
		if (w != NULL) 
		{
			if (w->cost > p->money)
			{
				cout << "Insufficient funds to buy " << w->weaponName << endl;
			}
			else 
			{
				p->buy(w);
				p->withdraw(w->cost);
			}
		}
		else 
		{
			cout << " ** " << choice << " not found!! **" << endl;
		}
		cout << "Please select another weapon to buy('end' to quit):";
		cin >> choice;
	}
	cout << endl;
}

void showRoom(bstree bt, Player * p)
{
	string choice;
	cout << "WELCOME TO THE SHOWROOM!!!!" << endl;
	bt.inorder();
	cout << " You have " << p->money << " money." << endl;
	cout << "Please select a weapon to buy('end' to quit):";
	cin >> choice;
	while (choice.compare("end") != 0 && !p->inventoryFull())
	{
		Weapon * w = &bt.get(choice);
		if (w != NULL)
		{
			if (w->cost > p->money)
			{
				cout << "Insufficient funds to buy " << w->weaponName << endl;
			}
			else
			{
				p->buy(w);
				p->withdraw(w->cost);
			}
		}
		else
		{
			cout << " ** " << choice << " not found!! **" << endl;
		}
		cout << "Please select another weapon to buy('end' to quit):";
		cin >> choice;
	}
	cout << endl;
}

int main() 
{
	string pname;
	cout << "Please enter Player name:" << endl;
	cin >> pname;
	Player pl(pname, 45);
	hashTable ht(101);
	addWeapons(ht);
	showRoom(ht, &pl);
	pl.printCharacter();

	return 0;
}
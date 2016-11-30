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

	bool operator>(const Weapon &other)
	{
		return weaponName > other.weaponName;
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
	void put(Weapon &value)
	{
		node *curr = root;

		if (curr == nullptr)
		{
			curr = new node(value);
			root = curr;
			return;
		}

		insert(curr, value);
	}

	node* insert(node* thisNode, Weapon &value)
	{
		if (thisNode == nullptr)
			return new node(value);

		if (value < thisNode->value)
			thisNode->left = insert(thisNode->left, value);
		else if (value > thisNode->value)
			thisNode->right = insert(thisNode->right, value);

		return thisNode;
	}

	// search and retrieval
	Weapon*  get(string key)
	{
		node *curr = search(root, key);
		
		if (curr != nullptr)
			return &curr->value;
		
		return nullptr; // need a better way to do this
	}

	node* search(node* thisNode, string key)
	{
		if (thisNode == nullptr || thisNode->value.weaponName == key)
			return thisNode;

		if (thisNode->value.weaponName < key)
			return search(thisNode->right, key);

		return search(thisNode->left, key);
	}

	// delete
	void remove(string key)
	{
		root = deleteNode(root, key);
	}

	node* deleteNode(node* thisNode, string key)
	{
		if (thisNode == nullptr) return thisNode;

		if (key < thisNode->value.weaponName)
			thisNode->left = deleteNode(thisNode->left, key);
		else if (key > thisNode->value.weaponName)
			thisNode->right = deleteNode(thisNode->right, key);
		else
		{
			if (thisNode->left == nullptr)
			{
				node* temp = thisNode->right;
				delete thisNode;
				return temp;
			}
			else if (thisNode->right == nullptr)
			{
				node* temp = thisNode->left;
				delete thisNode;
				return temp;
			}
			
			node* temp = inorderSuccessor(thisNode->right);

			thisNode->right = deleteNode(thisNode->right, temp->value.weaponName);
		}

		return thisNode;
	}

	node* inorderSuccessor(node* thisNode)
	{
		node* current = thisNode;

		while (current->left != nullptr)
			current = current->left;

		return current;
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
		Weapon* thisWeapon = new Weapon(w->weaponName, w->range, w->damage, w->weight, w->cost);
		cout << w->weaponName << " bought..." << endl;
		backpack[numItems] = thisWeapon;
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

void addWeapons(bstree &b) {
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
		b.put(*w);
		cout << "Please enter the NAME of another Weapon ('end' to quit):";
		cin >> weaponName;
	}
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
		Weapon* w = bt.get(choice);
		if (w != nullptr)
		{
			if (w->cost > p->money)
			{
				cout << "Insufficient funds to buy " << w->weaponName << endl;
			}
			else
			{
				p->buy(w);
				p->withdraw(w->cost);
				bt.remove(w->weaponName);
				cout << "Shop now has: " << endl;
				bt.inorder();
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
	bstree bt;
	addWeapons(bt);
	showRoom(bt, &pl);
	pl.printCharacter();

	cout << "Enter anything to quit: " << endl;
	cin >> pname;
	return 0;
}
#include <iostream>
#include <string>

using namespace std;

// Did not touch most of Weapon
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


	// Only added these overloads to save space later
	bool operator<(const Weapon &other)
	{
		return weaponName < other.weaponName;
	}

	bool operator>(const Weapon &other)
	{
		return weaponName > other.weaponName;
	}
};

/// This is our binary search tree
class bstree
{
private:
	/// BST contains it's own private node class
	class node
	{
	public:
		node(Weapon wep) : value(wep) { } // Make a node accepting a Weapon, store that weapon
		~node() {} // Make it destructible

		Weapon value;
		node *left = nullptr;
		node *right = nullptr;
	} *root = nullptr; // Have a pointer typed as our new node class set to nullptr stored in our BST

public:

	bstree() {} // Should be good empty
	~bstree() {}

	/// User friendly put function to be used throughout to add Weapons to the tree
	void put(Weapon &value)
	{
		// If there is nothing at the root of the tree
		if (root == nullptr)
		{
			// put this new value at the root of the tree
			root = new node(value);
			return;
		}
		// Otherwise call recursive insert starting at root
		insert(root, value);
	}

	/// Recursive insert function returning node* taking node* and Weapon&
	node* insert(node* thisNode, Weapon& value)
	{
		// If this node does not exist, put the weapon here
		if (thisNode == nullptr)
			return new node(value);

		if (value < thisNode->value) // If this Weapon is less than the one in this node
			thisNode->left = insert(thisNode->left, value); // Put this Weapon into the left subtree
		else if (value > thisNode->value) // If it's bigger 
			thisNode->right = insert(thisNode->right, value); // Put this Weapon into the right subtree

		// If this Weapon got to here, that Weapon is already in the BST at this location
		return thisNode;
	}

	/// User friendly get function
	Weapon* get(string key)
	{
		// Get a pointer to the node containing this key using recursive search
		node *curr = search(root, key);
		
		// If our pointer has a value
		if (curr != nullptr)
			return &curr->value; // return it
		
		return nullptr; // Otherwise return nullptr
	}


	/// Recursive search of BST returning node* taking node* and string
	node* search(node* thisNode, string key)
	{
		// If this node does not exist, or contains the key we are looking for
		if (thisNode == nullptr || thisNode->value.weaponName == key)
			return thisNode; // return it

		// Otherwise, is the node we are on less than our target key
		if (thisNode->value.weaponName < key)
			return search(thisNode->right, key); // If it is, search the right subtree

		return search(thisNode->left, key); // If not search the left subtree
		// This goes until we find what we are looking for, or hit the bottom of the tree.
	}

	/// User friendly delete function taking only a string key
	void remove(string key)
	{
		// Set the return from our recursive deleteNode to be the new root
		root = deleteNode(root, key);
	}

	/// Recursive deleteNode returning node* taking node* and string
	node* deleteNode(node* thisNode, string key)
	{
		if (thisNode == nullptr) return thisNode;

		// Reassign left subtree head to be results of deletion in left subtree
		if (key < thisNode->value.weaponName)
			thisNode->left = deleteNode(thisNode->left, key);
		// or reassign right subtree head to be results of deletion in right subtree
		else if (key > thisNode->value.weaponName)
			thisNode->right = deleteNode(thisNode->right, key);
		// otherwise this is the node we want to delete
		else
		{
			if (thisNode->right == nullptr) // This node no right child 
			{
				node* temp = thisNode->left; 
				delete thisNode;
				return temp; // if it also had no left child, there was no harm in deleting it
			} else if (thisNode->left == nullptr) // This node has no left child
			{
				node* temp = thisNode->right;
				delete thisNode;
				return temp; // same on the right
			}
			
			node* temp = inorderSuccessor(thisNode->right); // This node has at least one child so find the left most node right of this one

			thisNode->value = temp->value; // That value goes here

			thisNode->right = deleteNode(thisNode->right, temp->value.weaponName); // Now get rid of that node
		}

		return thisNode; // return a pointer to whatever we are on, whatever is now in it.
	}

	/// Find the smallest node child of this one returning a node* taking a node*
	node* inorderSuccessor(node* thisNode)
	{
		node* current = thisNode;

		// Just keep going left of here
		while (current->left != nullptr)
			current = current->left;

		return current;
	}

	/// User friendly inorder print function
	void inorder()
	{
		// So easy to call
		inorder(root);
	}

	/// Recursive inOrder printing taking node*
	void inorder(node *curr)
	{
		if (curr == nullptr) return;
		inorder(curr->left);
		cout << curr->value.weaponName << std::endl;
		inorder(curr->right);
	}
};

// Only one change here
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
		// Duplicate the Weapon* !!!
		Weapon* thisWeapon = new Weapon(w->weaponName, w->range, w->damage, w->weight, w->cost);
		cout << w->weaponName << " bought..." << endl;
		backpack[numItems] = thisWeapon;
		numItems++;
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

// Takes bstree& now
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

// Takes bstree now
void showRoom(bstree bt, Player * p)
{
	string choice;
	cout << "WELCOME TO THE SHOWROOM!!!!" << endl;
	bt.inorder(); // Calls inorder to print now
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
				bt.remove(w->weaponName); // Remove the weapon that's been bought
				cout << endl << p->name << " now has: " << endl;
				for (int i = 0; i < p->numItems; i++)
				{
					cout << p->backpack[i]->weaponName << endl;
				}
				// Add some extra prints now that the inventory changes
				cout << endl << "Shop now has: " << endl;
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
	// This stuff is all bstree now
	bstree bt;
	addWeapons(bt);
	showRoom(bt, &pl);
	pl.printCharacter();

	// Add an extra cin to stop things from exiting immediatly
	// No matter what you're doing to run it!
	cout << "Enter anything to quit: " << endl;
	cin >> pname;
	return 0;
}
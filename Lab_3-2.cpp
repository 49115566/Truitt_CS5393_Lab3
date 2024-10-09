/*
1. Time Complexity of Insert, Delete, and Search
Insert: In the best case, it takes O(1) since we directly compute the hash and insert into the AVL tree. In the worst case, it is O(log n) due to the AVL tree insertion.
Delete: Best case is O(1) for finding the bucket via the hash function, but the worst case is O(log n) for deletion from the AVL tree.
Search: Best case is O(1) if the element is found quickly, but in the worst case, it takes O(log n) due to AVL tree search.

2. Space Complexity
The space complexity is O(m + n), where m is the number of buckets (AVLs) and n is the total number of elements stored, considering that each bucket may hold multiple elements through AVL trees.

3. How Separate Chaining Resolves Collisions
Separate chaining resolves collisions by using an AVL tree in each bucket to store multiple elements that hash to the same index, ensuring efficient insertion, deletion, and search within the bucket.
*/

#include <iostream>    // Include input-output stream library
#include <string>      // Include string library
#include <cmath>       // Include math functions library
#include <vector>      // Include vector library
#include <random>      // Include random library
#include <fstream>     // Include file handling library
#include <limits>      // Include limits library for numeric limits
using namespace std;   // Use the standard namespace

// Structure to store person details
struct person {
	string first_name;  // First name of the person
	string last_name;   // Last name of the person
	string number;      // Phone number of the person

	person(string fn = "NONE", string ln = "NONE", string num = "NONE") : first_name(fn), last_name(ln), number(num) {} // Constructor for initializing person
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
// Binary search tree to deal with collisions

#include <cmath>        // For NAN (Not-a-Number)
using namespace std;    // Use the standard namespace

// Node structure for an AVL tree
struct tNode {
	tNode* left;        // Pointer to the left child of the node
	tNode* right;       // Pointer to the right child of the node
	person val;         // Value stored in the node (person object)
	int height;         // Integer representing the height of the node

	// Constructor to initialize a tree node with a person object
	tNode(person p) : left(nullptr), right(nullptr), val(p), height(1) {}  // Initialize pointers and height
};

// AVL tree class definition (self-balancing binary search tree)
class AVL {
private:
	tNode* head;        // Pointer to the root node of the AVL tree

	tNode* insertRec(tNode* node, const person& v);    // Recursive method to insert and balance the tree
	person retrieveRec(tNode* node, const string& v);  // Recursive method to retrieve a person
	tNode* removeRec(tNode* node, const string& v);    // Recursive method to remove and balance the tree
	tNode* removeRecFL(tNode* node, const string& fn, const string& ln); // Remove by first and last name
	tNode* rotateRight(tNode* y);    // Method to perform a right rotation
	tNode* rotateLeft(tNode* x);     // Method to perform a left rotation
	tNode* balance(tNode* node);     // Method to balance the AVL tree
	int height(tNode* node);         // Method to return the height of a node
	int getBalance(tNode* node);     // Method to get the balance factor of a node
	tNode* findMin(tNode* node);     // Method to find the node with the minimum value
	void deleteTree(tNode* node);    // Recursive method to delete the entire tree
	void printHelp(tNode* node, const string& first_name = " ");     // Helper method for in-order printing

public:
	AVL();          // Constructor to initialize the AVL tree
	~AVL();         // Destructor to clean up the AVL tree
	void insert(const person& v);           // Method to insert a value into the AVL tree
	person retrieve(const string& v);       // Method to retrieve a value by number
	void remove(const string& v);           // Method to remove a value by number
	void removeFL(const string& fn, const string& ln);  // Method to remove a value by first and last name
	void printAll();        // Method to print the tree in-order
	void printFN(const string& first_name); // Method to print based on first name
};

// Constructor for the AVL tree
AVL::AVL() : head(nullptr) {}   // Initialize the head of the tree to nullptr

// Destructor for the AVL tree
AVL::~AVL() {
	deleteTree(head);   // Call the recursive deleteTree method to clean up
}

// Recursive method to delete the entire AVL tree
void AVL::deleteTree(tNode* node) {
	if (node) {                     // If the node is not null
		deleteTree(node->left);      // Recursively delete the left subtree
		deleteTree(node->right);     // Recursively delete the right subtree
		delete node;                 // Delete the current node
	}
}

// Public method to insert a value into the AVL tree
void AVL::insert(const person& v) {
	head = insertRec(head, v);  // Call the recursive insert method, starting from the root
}

// Recursive method to insert a node into the AVL tree and balance it
tNode* AVL::insertRec(tNode* node, const person& v) {
	if (!node) {
		return new tNode(v); // If the node is null, create a new node
	}

	if (v.number < node->val.number) {    // If the value is less, insert into the left subtree
		node->left = insertRec(node->left, v);
	}
	else if (v.number > node->val.number) { // If the value is greater, insert into the right subtree
		node->right = insertRec(node->right, v);
	}
	else {    // If the value already exists, do nothing
		cout << "Already present, no insert." << endl;
		return node;
	}

	int hl = height(node->left), hr = height(node->right);
	node->height = 1 + (hl > hr ? hl : hr);  // Update the node's height
	return balance(node);   // Balance the tree and return the node
}

// Public method to retrieve a value by number
person AVL::retrieve(const string& t) {
	return retrieveRec(head, t);   // Call the recursive retrieve method
}

// Recursive method to retrieve a value
person AVL::retrieveRec(tNode* node, const string& v) {
	if (!node) return person();  // If the node is null, return an empty person
	if (v == node->val.number) {  // If the value matches, return it
		return node->val;
	}
	else if (v < node->val.number) {  // If the value is less, search the left subtree
		return retrieveRec(node->left, v);
	}
	else {    // If the value is greater, search the right subtree
		return retrieveRec(node->right, v);
	}
}

// Public method to remove a value by number
void AVL::remove(const string& t) {
	head = removeRec(head, t);  // Call the recursive remove method
}

// Recursive method to remove a node and balance the tree
tNode* AVL::removeRec(tNode* node, const string& v) {
	if (!node) {
		cout << "Node not found!" << endl;
		return nullptr;    // If the node is null, return null
	}

	if (v < node->val.number) {    // If the value is less, search the left subtree
		node->left = removeRec(node->left, v);
	}
	else if (v > node->val.number) {  // If the value is greater, search the right subtree
		node->right = removeRec(node->right, v);
	}
	else {    // If the node to be deleted is found
		if (!node->left || !node->right) { // If the node has one or no children
			tNode* temp = node->left ? node->left : node->right;  // Choose the non-null child
			if (!temp) {    // If there are no children
				temp = node;    // Temporarily store the node
				node = nullptr;  // Set the node to null
			}
			else {
				*node = *temp;   // Copy the non-null child to the current node
			}
			delete temp;   // Delete the temporary node
		}
		else {    // If the node has two children
			tNode* temp = findMin(node->right); // Find the in-order successor
			node->val = temp->val;  // Replace the current node's value with the successor's
			node->right = removeRec(node->right, temp->val.number);  // Remove the successor
		}
	}

	if (!node) return node;   // If the tree is empty, return null

	int hl = height(node->left), hr = height(node->right);
	node->height = 1 + (hl > hr ? hl : hr);  // Update the node's height
	return balance(node);  // Balance the tree and return the node
}

void AVL::removeFL(const string& fn, const string& ln) {
	head = removeRecFL(head, fn, ln);  // Call the recursive remove by first and last name
}

tNode* AVL::removeRecFL(tNode* node, const string& fn, const string& ln) {
	if (!node) return nullptr;  // If the node is null, return null

	node->left = removeRecFL(node->left, fn, ln);  // Recursively remove from the left subtree
	node->right = removeRecFL(node->right, fn, ln);  // Recursively remove from the right subtree

	if (fn == node->val.first_name && ln == node->val.last_name) {  // If first and last name match
		if (!node->left || !node->right) {  // If the node has one or no children
			tNode* temp = node->left ? node->left : node->right; // Choose the non-null child
			if (!temp) {             // If there are no children
				temp = node;         // Temporarily store the node
				node = nullptr;      // Set the node to null
			}
			else {
				*node = *temp;       // Copy the non-null child to the current node
			}
			delete temp;             // Delete the temporary node
		}
		else {                      // If the node has two children
			tNode* temp = findMin(node->right); // Find the in-order successor
			node->val = temp->val;   // Replace the current node's value with the successor's
			node->right = removeRecFL(node->right, temp->val.first_name, temp->val.last_name); // Remove the successor
		}
	}

	if (!node) return node;         // If the tree is empty, return null

	int hl = height(node->left), hr = height(node->right);
	node->height = 1 + (hl > hr ? hl : hr);  // Update the node's height
	return balance(node);           // Balance the tree and return the node
}

// Helper method to find the node with the minimum value
tNode* AVL::findMin(tNode* node) {
	while (node->left) {            // Traverse to the leftmost node
		node = node->left;
	}
	return node;                    // Return the node with the minimum value
}

// Method to perform a right rotation to balance the tree
tNode* AVL::rotateRight(tNode* y) {
	tNode* x = y->left;             // Set x as the left child of y
	tNode* T2 = x->right;           // Store the right subtree of x

	x->right = y;                   // Perform the rotation (x becomes the new root)
	y->left = T2;                   // Move T2 to the left of y

	int hl = height(y->left), hr = height(y->right);
	y->height = (hl > hr ? hl : hr) + 1;  // Update height of y
	hl = height(x->left), hr = height(x->right);
	x->height = (hl > hr ? hl : hr) + 1;  // Update height of x

	return x;                       // Return the new root
}

// Method to perform a left rotation to balance the tree
tNode* AVL::rotateLeft(tNode* x) {
	tNode* y = x->right;            // Set y as the right child of x
	tNode* T2 = y->left;            // Store the left subtree of y

	y->left = x;                    // Perform the rotation (y becomes the new root)
	x->right = T2;                  // Move T2 to the right of x

	int hl = height(y->left), hr = height(y->right);
	y->height = (hl > hr ? hl : hr) + 1;  // Update height of y
	hl = height(x->left), hr = height(x->right);
	x->height = (hl > hr ? hl : hr) + 1;  // Update height of x

	return y;                       // Return the new root
}

// Method to balance the AVL tree after insertion or deletion
tNode* AVL::balance(tNode* node) {
	int balanceFactor = getBalance(node);  // Get the balance factor of the current node

	if (balanceFactor > 1) {          // Left-heavy case
		if (getBalance(node->left) >= 0) {
			return rotateRight(node);  // Left-left case
		}
		else {
			node->left = rotateLeft(node->left); // Left-right case
			return rotateRight(node);
		}
	}

	if (balanceFactor < -1) {         // Right-heavy case
		if (getBalance(node->right) <= 0) {
			return rotateLeft(node);   // Right-right case
		}
		else {
			node->right = rotateRight(node->right); // Right-left case
			return rotateLeft(node);
		}
	}

	return node;                      // If balanced, return the node
}

// Method to get the height of a node (helper for balancing)
int AVL::height(tNode* node) {
	if (!node) return 0;            // If the node is null, return 0
	return node->height;            // Otherwise, return the height of the node
}

// Method to get the balance factor of a node
int AVL::getBalance(tNode* node) {
	if (!node) return 0;            // If the node is null, return balance factor 0
	return height(node->left) - height(node->right);  // Return the difference in heights
}

// Public method to print the tree in in-order traversal
void AVL::printAll() {
	if(head) printHelp(head);                // Call the recursive print helper method
}

void AVL::printFN(const string& first_name) {
	if (head) printHelp(head, first_name);
}

// Helper method for in-order traversal
void AVL::printHelp(tNode* node, const string& first_name) {
	if (node->left) printHelp(node->left, first_name);  // Print the left subtree
	if (first_name == " " || first_name == node->val.first_name) cout << node->val.first_name << ' ' << node->val.last_name << " : " << node->val.number << " || ";              // Print the current node's value
	if (node->right) printHelp(node->right, first_name); // Print the right subtree
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------
//Hash Table

// Function to generate a list of small prime numbers up to a limit
std::vector<int>* generate_small_primes(int limit) {
	std::vector<int>* primes = new vector<int>;  // Dynamically allocate a vector of integers
	primes->push_back(2);          // Add 2 as the first prime number

	for (int i = 3; i <= limit; i += 2) {  // Check only odd numbers
		bool is_prime = true;      // Assume the number is prime
		for (int prime : *primes) { // Check divisibility by smaller primes
			if (prime * prime > i) break; // Stop if the prime exceeds the square root of the number
			if (i % prime == 0) {  // If divisible, it's not a prime
				is_prime = false;
				break;
			}
		}
		if (is_prime) {            // If still prime, add it to the list
			primes->push_back(i);
		}
	}
	return primes;                 // Return the list of primes
}

// Helper function to check if a number is prime using a list of smaller prime numbers
//Will not work properly if not all primes < sqrt(num) are on primes list
bool is_prime(int num, const std::vector<int>* primes) {
	if (num <= 1) return false;    // Numbers less than or equal to 1 are not prime
	if (num == 2) return true;     // 2 is prime
	if (num % 2 == 0) return false; // Even numbers greater than 2 are not prime

	// Check divisibility by smaller primes up to sqrt(num)
	int limit = std::sqrt(num);    // Calculate the square root of the number
	for (int prime : *primes) {    // Iterate through the list of smaller primes
		if (prime > limit) break;  // If the prime exceeds the limit, stop
		if (num % prime == 0) {    // If the number is divisible by any prime, it's not prime
			return false;
		}
	}
	return true;                   // Return true if no divisor was found (prime number)
}

// Function to find the next prime number greater than or equal to `n`
int next_prime(int n) {
	if (n <= 2) return 2;          // Return 2 if the input is less than or equal to 2

	if (n % 2 == 0) n++;           // If the number is even, start with the next odd number

	vector<int>* small_primes = generate_small_primes(std::sqrt(n) + 1); // Pre-generate small primes

	while (!is_prime(n, small_primes)) { // Check if the current number is prime
		n += 2;                  // Increment by 2 to skip even numbers
	}

	delete small_primes;           // Free the memory used by the small primes vector

	return n;                      // Return the next prime number
}

template <typename cldManage>
class hashTable {  // Template class definition for hashTable with a type parameter cldManage
public:
	hashTable(int expElementCT = 6);  // Constructor declaration with a default argument
	~hashTable();  // Destructor declaration

	// Deleberately has no insert/remove because the key for cldManage may be different than for this hash table.
	cldManage& retrieve(string key);  // Method to retrieve an element based on its key

	void printAll();  // Method to print all elements; not typical for hash tables
	void printFN(string first_name);  // Method to print elements based on the first name
private:
	cldManage* table;  // Pointer to the array of cldManage elements (the hash table)
	int len;  // Length of the hash table

	unsigned long long hash(string key);  // Method to compute the hash value for a given key
};

// Constructor definition for the hash table
template <typename cldManage>
hashTable<cldManage>::hashTable(int expElementCt) {  // Constructor implementation
	// Set tableLen to the next prime greater than expected number of books divided by 0.75
	len = next_prime(expElementCt / 0.75 + 1);  // Calculate and assign the length of the hash table
	table = new cldManage[len];  // Dynamically allocate an array of cldManage elements for the hash table
}

template <typename cldManage>
hashTable<cldManage>::~hashTable() {  // Destructor implementation
	delete[] table;  // Deallocate the memory used by the table
}

template <typename cldManage>
cldManage& hashTable<cldManage>::retrieve(string key) {  // Method to retrieve an element by key
	return table[hash(key) % len];  // Return the element at the hashed index
}

template <typename cldManage>
void hashTable<cldManage>::printAll() {  // Method to print all elements in the hash table
	for (int i = 0; i < len; i++) {  // Loop through each index of the table
		table[i].printAll();  // Call printAll on each cldManage element
	}
}

template <typename cldManage>
void hashTable<cldManage>::printFN(string first_name) {  // Method to print elements by first name
	for (int i = 0; i < len; i++) {  // Loop through each index of the table
		table[i].printFN(first_name);  // Call printFN on each cldManage element with the provided first name
	}
}

template <typename cldManage>
unsigned long long hashTable<cldManage>::hash(string key) {  // Method to compute a hash value for a given key
	const unsigned long long fnv_prime = 1099511628211ULL;  // FNV prime number constant
	unsigned long long hash = 14695981039346656037ULL;  // FNV offset basis

	for (char c : key) {  // Loop through each character in the key
		hash ^= c;  // XOR the character with the hash
		hash *= fnv_prime;  // Multiply the hash by the FNV prime
	}

	return hash;  // Return the computed hash value
}

int main() {  // Entry point of the program
	ifstream file("Lab3_Problem2_DSC++.csv");  // Open the CSV file for reading

	// Check if the file is open
	if (!file.is_open()) {  // Check if the file failed to open
		cerr << "Error: Could not open the file!" << std::endl;  // Output error message to standard error
		return 1;  // Exit the program with an error code
	}

	hashTable<hashTable<AVL>> table(11);  // Create a hash table with a size of 11
	string first_name;  // Variable to store the first name
	string last_name;  // Variable to store the last name
	string number;  // Variable to store the number
	person temp;  // Temporary person object

	// Read each line from the file
	while (file.peek() != EOF) {  // Continue until the end of the file is reached
		file.ignore(numeric_limits<streamsize>::max(), '\'');  // Ignore characters until the first single quote
		if (!(file >> first_name)) break;  // Read the first name, break if reading fails

		file.ignore(numeric_limits<streamsize>::max(), ' ');  // Ignore the space after the first name
		if (!getline(file, last_name, '\'')) break;  // Read the last name until the next single quote, break if reading fails

		file.ignore(numeric_limits<streamsize>::max(), '\'');  // Ignore characters until the next single quote
		if (!getline(file, number, '\'')) break;  // Read the number until the next single quote, break if reading fails

		temp = person(first_name, last_name, number);  // Create a person object using the names and number

		table.retrieve(first_name).retrieve(last_name).insert(temp);  // Insert the person into the hash table
	}

	file.close();  // Close the file after reading

	table.printAll();  // Print all entries in the hash table

	cout << endl << endl << "PRINTING ALL \"Liam\"s:" << endl;  // Output message for printing all "Liam" entries
	table.retrieve("Liam").printFN("Liam");  // Print all entries with first name "Liam"
	cout << endl << endl;  // Print two new lines for spacing

	cout << "REMOVING ALL \"Isabella Anderson\"s:" << endl;  // Output message for removing all "Isabella Anderson" entries
	table.retrieve("Isabella").retrieve("Anderson").removeFL("Isabella", "Anderson");  // Remove all entries with name "Isabella Anderson"
	table.printAll();  // Print all entries in the hash table again
	cout << endl << endl;  // Print two new lines for spacing

	cout << "PRINTING ALL \"Shaibal Chakrabarty\"s:" << endl;  // Output message for printing all "Shaibal Chakrabarty" entries
	table.retrieve("Shaibal").retrieve("Chakrabarty").printFN("Shaibal");  // Print all entries with first name "Shaibal"

	cout << "PRINTING ALL \"Lucas Li\"s:" << endl;  // Output message for printing all "Lucas Li" entries
	table.retrieve("Lucas").retrieve("Li").printFN("Lucas");  // Print all entries with first name "Lucas"
	cout << endl << endl;  // Print two new lines for spacing

	cout << "INSERTING \"Lucas Li\" and \"Shaibal Chakrabarty\":" << endl;  // Output message for inserting new entries
	temp = person("Shaibal", "Chakrabarty", "214-768-2000");  // Create a new person object for Shaibal
	table.retrieve(temp.first_name).retrieve(temp.last_name).insert(temp);  // Insert Shaibal into the hash table
	temp = person("Lucas", "Li", "469-555-1212");  // Create a new person object for Lucas
	table.retrieve(temp.first_name).retrieve(temp.last_name).insert(temp);  // Insert Lucas into the hash table

	table.printAll();  // Print all entries in the hash table again

	return 0;  // Exit the program successfully
}

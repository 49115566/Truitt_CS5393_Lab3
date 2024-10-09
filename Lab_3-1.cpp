/*
Time Complexity of Operations in an AVL Tree:
An AVL tree is a self-balancing binary search tree, and it ensures that the height difference between the left and right subtrees of any node is at most 1. This balancing property ensures that the tree's height remains logarithmic in terms of the number of nodes, i.e., O(log n), where n is the number of nodes in the tree.

Insert Operation:
The time complexity O(log n) because, in the worst case, we may need to traverse the height of the tree to find the correct insertion point, and then perform a constant number of rotations to restore balance.

Delete Operation:
Similarly, the delete operation takes O(log n). After deleting a node, the tree may become unbalanced, and restoring the balance requires a constant number of rotations, each taking O(1).

Find/Search Operation:
The find or search operation in an AVL tree also takes O(log n) because the tree is balanced, ensuring that the path from the root to any node is logarithmic in the number of nodes.


How Does a BST Ensure that In-Order Traversal Results in Sorted Output:
A Binary Search Tree (BST) is structured so that for every node:
-All nodes in its left subtree have values less than the node’s value.
-All nodes in its right subtree have values greater than the node’s value.

During in-order traversal (left subtree ? root ? right subtree), the nodes are visited in ascending order of their values:
-Recursively visit the left subtree (which contains smaller values).
-Visit the root (which is greater than all values in the left subtree and smaller than all values in the right subtree).
-Recursively visit the right subtree (which contains greater values).
This recursive structure ensures that in-order traversal yields nodes in sorted order, assuming the tree obeys the BST property.


Space Complexity of a BST:
The space complexity for storing the nodes in a Binary Search Tree (BST) is O(n), where n is the number of nodes in the tree. Each node typically stores a value and pointers to its left and right children, leading to a linear space requirement proportional to the number of nodes.
*/
#include <iostream>                 // For input-output operations
#include <cmath>                    // For NAN (Not-a-Number)
using namespace std;

// Node structure for an AVL tree
struct tNode {
	tNode* left;                    // Pointer to the left child of the node
	tNode* right;                   // Pointer to the right child of the node
	double val;                     // Value stored in the node
	int height;                     // Integer representing the height of the node

	// Constructor to initialize a tree node with double
	tNode(double b) : left(nullptr), right(nullptr), val(b), height(1) {}  // Initialize pointers and height
};

// AVL tree class definition (self-balancing binary search tree)
class AVL {
private:
	tNode* head;                    // Pointer to the root node of the AVL tree

	tNode* insertRec(tNode* node, double v);    // Recursive method to insert and balance the tree
	double retrieveRec(tNode* node, double v);  // Recursive method to retrieve a value
	tNode* removeRec(tNode* node, double v);    // Recursive method to remove and balance the tree
	tNode* rotateRight(tNode* y);    // Method to perform a right rotation
	tNode* rotateLeft(tNode* x);     // Method to perform a left rotation
	tNode* balance(tNode* node);     // Method to balance the AVL tree
	int height(tNode* node);         // Method to return the height of a node
	int getBalance(tNode* node);     // Method to get the balance factor of a node
	tNode* findMin(tNode* node);     // Method to find the node with the minimum value
	void deleteTree(tNode* node);    // Recursive method to delete the entire tree
	void printHelp(tNode* node);     // Helper method for in-order printing

public:
	AVL();                          // Constructor to initialize the AVL tree
	~AVL();                         // Destructor to clean up the AVL tree
	void insert(double v);           // Method to insert a value into the AVL tree
	double retrieve(double v);       // Method to retrieve a value
	void remove(double v);           // Method to remove a value
	void printInOrder();             // Method to print the tree in-order
};

// Constructor for the AVL tree
AVL::AVL() : head(nullptr) {}       // Initialize the head of the tree to nullptr

// Destructor for the AVL tree
AVL::~AVL() {
	deleteTree(head);               // Call the recursive deleteTree method to clean up
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
void AVL::insert(double v) {
	head = insertRec(head, v);      // Call the recursive insert method, starting from the root
}

// Recursive method to insert a node into the AVL tree and balance it
tNode* AVL::insertRec(tNode* node, double v) {
	if (!node) {
		cout << "Successful Insert!" << endl;
		return new tNode(v); // If the node is null, create a new node
	}

	if (v < node->val) {            // If the value is less, insert into the left subtree
		node->left = insertRec(node->left, v);
	}
	else if (v > node->val) {       // If the value is greater, insert into the right subtree
		node->right = insertRec(node->right, v);
	}
	else {                          // If the value already exists, do nothing
		cout << "Already present, no insert." << endl;
		return node;
	}

	int hl = height(node->left), hr = height(node->right);
	node->height = 1 + (hl > hr ? hl : hr);  // Update the node's height
	return balance(node);           // Balance the tree and return the node
}

// Public method to retrieve a value
double AVL::retrieve(double t) {
	return retrieveRec(head, t);    // Call the recursive retrieve method
}

// Recursive method to retrieve a value
double AVL::retrieveRec(tNode* node, double v) {
	if (!node) return NAN;          // If the node is null, return NAN
	if (v == node->val) {           // If the value matches, return it
		cout << "Successful Find!" << endl;
		return node->val;
	}
	else if (v < node->val) {       // If the value is less, search the left subtree
		return retrieveRec(node->left, v);
	}
	else {                          // If the value is greater, search the right subtree
		return retrieveRec(node->right, v);
	}
}

// Public method to remove a value
void AVL::remove(double t) {
	head = removeRec(head, t);      // Call the recursive remove method
}

// Recursive method to remove a node and balance the tree
tNode* AVL::removeRec(tNode* node, double v) {
	if (!node) {
		cout << "Node not found!" << endl;
		return nullptr;      // If the node is null, return null
	}

	if (v < node->val) {            // If the value is less, search the left subtree
		node->left = removeRec(node->left, v);
	}
	else if (v > node->val) {       // If the value is greater, search the right subtree
		node->right = removeRec(node->right, v);
	}
	else {                          // If the node to be deleted is found
		if (!node->left || !node->right) { // If the node has one or no children
			tNode* temp = node->left ? node->left : node->right; // Choose the non-null child
			if (!temp) {             // If there are no children
				temp = node;         // Temporarily store the node
				node = nullptr;      // Set the node to null
			}
			else {
				*node = *temp;       // Copy the non-null child to the current node
			}
			cout << "Successful Remove!" << endl;
			delete temp;             // Delete the temporary node
		}
		else {                      // If the node has two children
			tNode* temp = findMin(node->right); // Find the in-order successor
			node->val = temp->val;   // Replace the current node's value with the successor's
			node->right = removeRec(node->right, temp->val); // Remove the successor
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
void AVL::printInOrder() {
	printHelp(head);                // Call the recursive print helper method
	cout << endl;                   // Print a new line after traversal
}

// Helper method for in-order traversal
void AVL::printHelp(tNode* node) {
	if (node->left) printHelp(node->left);  // Print the left subtree
	cout << node->val << ' ';              // Print the current node's value
	if (node->right) printHelp(node->right); // Print the right subtree
}

int main() {
	AVL tree;                        // Create an AVL tree

	// Insert values into the AVL tree
	for (double i : {50, 30, 10, 40, 20, 100, 70, 90, 60, 80}) {
		tree.insert(i);
	}

	tree.printInOrder();             // Print the tree in in-order traversal

	// Retrieve and print values
	cout << "Finding 60: " << tree.retrieve(60) << endl;
	cout << "Finding 100: " << tree.retrieve(100) << endl;

	// Retrieve a value not present in the tree
	cout << "Finding 25: ";
	double result = tree.retrieve(25);
	if (result == result) {          // Check if the result is a valid number (not NAN)
		cout << result << endl;
	}
	else {
		cout << "Not in tree" << endl;
	}

	// Retrieve another value not present in the tree
	cout << "Finding 96.3: ";
	result = tree.retrieve(96.3);
	if (result == result) {          // Check if the result is a valid number
		cout << result << endl;
	}
	else {
		cout << "Not in tree" << endl;
	}

	// Remove values from the tree
	tree.remove(50);
	tree.remove(100);

	tree.printInOrder();             // Print the updated tree in-order

	return 0;                        // End the program
}

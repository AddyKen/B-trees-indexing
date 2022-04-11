#include <bits/stdc++.h>
using namespace std;
int t, d;

class Node {
	bool IS_LEAF; //checking leaf or not 	
	int *key, size; // 
	Node** ptr;   //
	friend class BPTree;

public:
	Node(int);
};

class BPTree {
	Node* root;
	void insertInternal(int, Node*, Node*);
	Node* findParent(Node*, Node*);
    int indexcount;
    int datacount;

public:
	BPTree();
	//void search(int);
	void insert(int);
	void display(void);
};

Node::Node(int MAX) //node constructor 
{
    key = new int[MAX];
	ptr = new Node*[MAX + 1];
}

BPTree::BPTree() //tree constructor 
{
	root = NULL;
    indexcount = 0;
    datacount = 1;
}

// void BPTree::search(int x)
// {
// 	if (root == NULL) {
// 		cout << "Tree is empty\n";
// 	}
// 	else {
// 		Node* cursor = root;
// 		while (cursor->IS_LEAF == false) {
// 			for (int i = 0; i < cursor->size; i++) {
// 				if (x < cursor->key[i]) {
// 					cursor = cursor->ptr[i];
// 					break;
// 				}
// 				if (i == cursor->size - 1) {
// 					cursor = cursor->ptr[i + 1];
// 					break;
// 				}
// 			}
// 		}
// 		for (int i = 0;
// 			i < cursor->size; i++) {
// 			if (cursor->key[i] == x) {
// 				cout << "Found\n";
// 				return;
// 			}
// 		}
// 		cout << "Not found\n";
// 	}
// }

void BPTree::insert(int x)
{
	if (root == NULL) {
		root = new Node(2*d);
		root->key[0] = x;
		root->IS_LEAF = true;
		root->size = 1;
	}
	else { // case when there was at least one node in the tree 

	    //initialising our cursor ptr to root to start iterating 
		Node* cursor = root; // cursor suggestes which node we are dealing with here 
		Node* parent; 

		// the subcase when current node is not leaf and there are more nodes present it iterates that many times 
		while (cursor->IS_LEAF == false) { 
			parent = cursor; // ultimately after the loop parent is retured with node which was found just bfore the cursor node 

			//changes cursor to the next node which is or which leads to node where x has to be inserted 
			for (int i = 0; i < cursor->size; i++) { 
				if (x < cursor->key[i]) { // checking condition to see around which side of the tree x will go 
					cursor = cursor->ptr[i]; // change the current node to the node whose pointer is stored around where x has to go
					break;
				}
				if (i == cursor->size - 1) {
					cursor = cursor->ptr[i + 1]; // boundary condition when x was greater than all the values we had so it will go to rightmost node
					break;
				}
			}
			//this while loop works until we reach leaf node where x has to be inserted 
		}
		if (cursor->size < 2*d) { //if the node that we reached was not full then here we insert else we split it into 2
			int i = 0;
			while (x > cursor->key[i]
				&& i < cursor->size) {
				i++;    //iterating to find whee x goes
			}
			for (int j = cursor->size; j > i; j--) {
                cursor->key[j] = cursor->key[j - 1]; // making room for x to insert
			}
			cursor->key[i] = x; //inserting x
			cursor->size++; // increasing the size of the node
			cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1]; //
			cursor->ptr[cursor->size - 1] = NULL;
		}
		else { // the case the where the leaf node that we reached is full and we need to split it (parent management not here it's in insert internal)
			Node* newLeaf = new Node(2*d);
            datacount++; // keeps the count of no. of nodes in the tree
			int virtualNode[2*d + 1];

			for (int i = 0; i < 2*d; i++) {
				virtualNode[i] = cursor->key[i];  //preserving the contents of the node to split it later
			}
			int i = 0, j;

			while (x > virtualNode[i] && i < 2*d) { // finding place to insert x based on ascending size
				i++;
			}

			for (int j = 2*d + 1; j > i; j--) {
				virtualNode[j] = virtualNode[j - 1];  // shifting values in the virtual array greater than x to the right to make space
			}

			virtualNode[i] = x; //inserting x in the created space in the previous step
			newLeaf->IS_LEAF = true; // declaring the new leaf is node 

			cursor->size = d; //declaring/altering the size of the new node and existing node to use for interating values  
			newLeaf->size = d + 1;

			for (i = 0; i < cursor->size; i++) {
				cursor->key[i] = virtualNode[i]; // putting values in the old node
			}

			for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++) {
				newLeaf->key[i] = virtualNode[j]; // putting values in the new node 
			}

			// here managing the parent management part....we got the new splitted leaf above but needs keys and pointer reorganising

			if (cursor == root) { // case if the leaf node was the root node and needed splitting 
				Node* newRoot = new Node(2*t+1); // when the curson(the root node) splits the new root will be a differen parent node
                indexcount++; //????????????????????????????????????????
				newRoot->key[0] = newLeaf->key[0]; // the tree is considered right biased hence the new leaf side keys matches with key in new root
				newRoot->ptr[0] = cursor;
				newRoot->ptr[1] = newLeaf;
				newRoot->IS_LEAF = false;
				newRoot->size = 1;
				root = newRoot; // changed the root pointer to the new root node that we created 
			}
			else { // the case where cursor was not root and we split the node in two.
				insertInternal(newLeaf->key[0], parent, newLeaf);
			}
		}
	}
}

void BPTree::insertInternal(int x, Node* cursor, Node* child)
{
	//this chunk decides where the middle value that is to be put in the parent node goes 
	if (cursor->size < 2*t+1) {  //case where the internal parent node is not full 
		int i = 0;
		while (x > cursor->key[i] && i < cursor->size) {
			i++;
		}

		for (int j = cursor->size;  j > i; j--) {
			cursor->key[j]= cursor->key[j - 1];
		}

		for (int j = cursor->size + 1; j > i + 1; j--) {
			cursor->ptr[j] = cursor->ptr[j - 1];
		}
		cursor->key[i] = x;
		cursor->size++;
		cursor->ptr[i + 1] = child; //the pointer next to the value we stored will point to the newleaf which is child here. since (ascending values)
	}

	else { //case where internal parent node also needs splitting 
		Node* newInternal = new Node(2*t+1); // making new internal node 
        indexcount++;
		int virtualKey[2*t+1 + 1];
		Node* virtualPtr[2*t+1 + 2];

		for (int i = 0; i < 2*t+1; i++) {
			virtualKey[i] = cursor->key[i]; //preserving keys of parent node to be split 
		}

		for (int i = 0; i < 2*t+1 + 1; i++) {
			virtualPtr[i] = cursor->ptr[i]; ////preserving pointers of parent node to be split
		}

		int i = 0, j;

		while (x > virtualKey[i] && i < 2*t+1) {
			i++; // deciding where x is put in the virtual array 
		}

		for (int j = 2*t+1 + 1; j > i; j--) {
			virtualKey[j] = virtualKey[j - 1]; // shifting values in the virtual array to make room for x. (x here is key[o] of newleaf)
		}
		virtualKey[i] = x; //putting x

		for (int j = 2*t+1 + 2; j > i + 1; j--) {
			virtualPtr[j] = virtualPtr[j - 1]; // shifting pointers in the pointers array from top to i + 2 bcoz i + 1 will have ptr of child
		}

		virtualPtr[i + 1] = child; //assigning i+1 th pointer as child pointer
		newInternal->IS_LEAF = false; 

		cursor->size = t;
		newInternal->size = t + 1; // declaring/altering the size of new nodes to interate through the values 

		for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++) {
			newInternal->key[i] = virtualKey[j]; //inserting values in the new internal node 
		}

		for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++) {

			newInternal->ptr[i] = virtualPtr[j+1]; // putting the pointers in the new internal node
		}
        // same part in the insert function if it was root then do this or not then recursively call the function until node created is not full
		if (cursor == root) {
                                       
			Node* newRoot = new Node(2*t+1);
            indexcount++;
			newRoot->key[0] = virtualKey[cursor->size];
			newRoot->ptr[0] = cursor;
			newRoot->ptr[1] = newInternal;
			newRoot->IS_LEAF = false;
			newRoot->size = 1;
			root = newRoot;
        }
		else {
			insertInternal(virtualKey[cursor->size],findParent(root, cursor),newInternal);
		}
	}
}

Node* BPTree::findParent(Node* cursor, Node* child)
{
	Node* parent;
	if (cursor->IS_LEAF || (cursor->ptr[0])->IS_LEAF) {
		return NULL;
	}
	for (int i = 0; i < cursor->size + 1; i++) {
		if (cursor->ptr[i] == child) {
			parent = cursor;
			return parent;
		}
		else {
			parent = findParent(cursor->ptr[i], child);
			if (parent != NULL)
				return parent;
		}
	}
	return parent;
}

void BPTree::display(void){
    cout << indexcount << " " << datacount << " ";
    for(int i = 0; i < root->size; i++){
        cout << root->key[i] << " ";
    }
    cout << endl;
}

// Driver Code
int main()
{
	BPTree node;

	// Create B+ Tree
	cin>>d>>t;
    int choice;
	int n;
	while(1)
	{
		cin>>choice;
		switch(choice)
		{
			case 1:
				cin>>n;
				node.insert(n);
				break;
			case 2:
				node.display();
				break;
			case 3:
				exit(0);
			default:
				cout<<"Invalid Input"<<endl;
		}
	}

	return 0;
}

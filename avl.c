/*avl.c*/

//
// AVL Tree ADT implementation file.
//
// <<Jakub>>
// U. of Illinois, Chicago
// CS251, Spring 2017
//

// ignore stdlib warnings if working in Visual Studio:
#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "avl.h"


//
// AVLCreate:
//
// Dynamically creates and returns an empty AVL tree.
//
AVL *AVLCreate()
{
	AVL *tree;

	tree = (AVL *)malloc(sizeof(AVL));
	tree->Root = NULL;
	tree->Count = 0;

	return tree;
}



//int _AVLSearch3
int SNDrouteFunc(AVLNode *root, NewData *NewStruct2, NewData *NewStruct3, int *NewCounter, int count, int count2) {
	if (root == NULL) {
		return 0;
	}
	int i; //Declaring for loop var
	int j;//Declaring for loop var

	for (i = count; i >= 0; i--) {
		for (j = count2; j >= 0; j--) {

			int S = NewStruct2[i].StationID; //Setting S
			int D = NewStruct3[j].StationID;//Setting D

			if (root->Value.Trip.From == S && root->Value.Trip.To == D) { //If S and D match From & To stations
				(*NewCounter)++; //increase counter
			}
		}
	}
	SNDrouteFunc(root->Left, NewStruct2, NewStruct3, NewCounter, count, count2); //Recursive Call
	SNDrouteFunc(root->Right, NewStruct2, NewStruct3, NewCounter, count, count2);//Recursive Call
}

//Sees if distances match and stores data into new struct
int matchingDistance(AVLNode *cur, double latitude, double longitude, double distance, NewData *NewStruct, int *count)
{
	if (cur == NULL) {
		return 0;
	}

	//Calculating the distance between lat and long points
	double returnedDist = distBetween2Points(cur->Value.Station.Stationlatitude, cur->Value.Station.Stationlongitude, latitude, longitude);

	 if (returnedDist <= distance) {
		 NewStruct[*count].distance = returnedDist; //storing dist into struct
		 NewStruct[*count].StationID = cur->Value.Station.StationID; //Storing ID into struct
		 (*count)++; //Increas count
		}


	 matchingDistance(cur->Left, latitude, longitude, distance, NewStruct, count);//Recursive Call
	 matchingDistance(cur->Right, latitude, longitude, distance, NewStruct, count);//Recursive Call

	return 1;
}

//Counts the trips of the bikes and stations
int StationNBikeTripCount(AVL *tree, AVLKey key)
{
	AVLNode *cur = tree->Root;

	//
	// search the tree to see if it contains a matching key:
	//
	while (cur != NULL)
	{
		if (AVLCompareKeys(key, cur->Key) == 0) {  // found!
			if (cur->Value.Type == STATIONTYPE) {
				cur->Value.Station.Stationtrip++; //Increasing stationtrip
			}

			else if (cur->Value.Type == BIKETYPE) {
				cur->Value.Bike.TripCount++;//Increasing biketrip
			}

			return 1;
		}
		else if (AVLCompareKeys(key, cur->Key) < 0)  // smaller, go left:
		{
			cur = cur->Left;
		}
		else  // larger, go right:
		{
			cur = cur->Right;
		}
	}

	// if we get here, we fell out of the tree, and didn't find it:
	return 0;
}

//Sort function
void sort(NewData *value, int size) {
	//Declaring variables
	int i = 0;
	int j = 0;
	//int k = 0;
	int indexSmallest = 0;
	NewData temp;

	//for loop going throught the whole thing
	for (i = 0; i < size - 1; ++i) {

		// Find index of smallest remaining element
		indexSmallest = i;
		for (j = i; j < size; ++j) {

			if ((value[j].distance == value[indexSmallest].distance) && value[j].StationID <= value[indexSmallest].StationID) {
				indexSmallest = j;		//update to smaller value if found
			}
			else if (value[j].distance < value[indexSmallest].distance)
			{
				indexSmallest = j;
			}
		}
		// Swap numbers[i] and numbers[indexSmallest]
		temp = value[i];
		value[i] = value[indexSmallest];
		value[indexSmallest] = temp;
	}
}

AVLNode *AVLSearch(AVL *tree, AVLKey key)
{
	AVLNode *cur = tree->Root;

	//
	// search the tree to see if it contains a matching key:
	//
	while (cur != NULL)
	{
		if (AVLCompareKeys(key, cur->Key) == 0)  // found!
			return cur;
		else if (AVLCompareKeys(key, cur->Key) < 0)  // smaller, go left:
		{
			cur = cur->Left;
		}
		else  // larger, go right:
		{
			cur = cur->Right;
		}
	}

	// if we get here, we fell out of the tree, and didn't find it:
	return NULL;
}

//
// AVLFree:
//
// Frees the memory associated with the tree: the handle and the nodes.
// The provided function pointer is called to free the memory that
// might have been allocated as part of the key or value.
//
void freeAVLNodeKeyVal(AVLKey key, AVLValue value) {
	if (value.Type == STATIONTYPE) { //IF its station type
		free(value.Station.Stationname); //Free the station
	}
	else if (value.Type == TRIPTYPE) { //IF its trip type

	}
	else if (value.Type == BIKETYPE) { //IF its bike type

	}
	else {
		printf("Cannot free AvlNode for odd reason...\n");
	}
}


void _AVLHelper(AVLNode *root, void(*fp)(AVLKey key, AVLValue value)) {
	if (root == NULL) {
		return;
	}

	_AVLHelper(root->Left, fp); //Goes left
	_AVLHelper(root->Right, fp); //Goes right
	fp(root->Key, root->Value); //accesses Key and value
	free(root); //Frees the root
}

void AVLFree(AVL *tree, void(*fp)(AVLKey key, AVLValue value))
{

	//printf(">>>>> AVLFree needs to be implemented <<<<<\n");
	_AVLHelper(tree->Root, freeAVLNodeKeyVal); //Calls helper function to retract memory
}


//
// AVLCompareKeys: 
//
// Compares key1 and key2, returning
//   value < 0 if key1 <  key2
//   0         if key1 == key2
//   value > 0 if key1 >  key2
//
int AVLCompareKeys(AVLKey key1, AVLKey key2)
{
	if (key1 < key2)
		return -1;
	else if (key1 == key2)
		return 0;
	else
		return 1;
}


//
// AVLCount:
//
// Returns # of nodes in the tree.
//
int AVLCount(AVL *tree)
{
	return tree->Count;
}


//
// AVLHeight: 
//
// Returns the overall height of the AVL tree.
//
int AVLHeight(AVL *tree)
{
	if (tree->Root == NULL)
		return -1;
	else
		return tree->Root->Height;
}


//
// Rotate right the sub-tree rooted at node k2, return pointer
// to root of newly-rotated sub-tree --- i.e. return pointer
// to node k1 that was rotated up to top of sub-tree.  Heights
// of the rotated sub-tree are also updated by this function.
//
int _height(AVLNode *cur)
{
	if (cur == NULL)
		return -1;
	else
		return cur->Height;
}

int _max2(int x, int y)
{
	return (x > y) ? x : y;
}

AVLNode *RightRotate(AVLNode *k2)
{
	AVLNode *k1 = k2->Left;

	AVLNode *X = k1->Left;
	AVLNode *Y = k1->Right;
	AVLNode *Z = k2->Right;

	//
	// rotate k1 up, and k2 down to the right:
	//
	k1->Right = k2;
	k2->Left = Y;

	//
	// recompute heights of nodes that moved:  k2, then k1
	//
	k2->Height = 1 + _max2(_height(k2->Left), _height(k2->Right));
	k1->Height = 1 + _max2(_height(k1->Left), _height(k1->Right));

	return k1;  // k1 is the new root of rotated sub-tree:
}

//
// Rotate left the sub-tree rooted at node k1, return pointer
// to root of newly-rotated sub-tree --- i.e. return pointer
// to node k2 that was rotated up to top of sub-tree.  Heights
// of the rotated sub-tree are also updated by this function.
//
AVLNode *LeftRotate(AVLNode *k1)
{
	AVLNode *k2 = k1->Right;

	AVLNode *X = k1->Left;
	AVLNode *Y = k2->Left;
	AVLNode *Z = k2->Right;

	//
	// rotate k2 up, and k1 down to the left:
	//
	k2->Left = k1;
	k1->Right = Y;

	//
	// recompute heights of nodes that moved:  k1, then k2
	//
	k1->Height = 1 + _max2(_height(k1->Left), _height(k1->Right));
	k2->Height = 1 + _max2(_height(k2->Left), _height(k2->Right));

	return k2;  // k2 is the new root of rotated sub-tree:
}


//
// AVL Insert:
//
// Inserts the given (key, value) into the AVL tree, rebalancing
// the tree as necessary.  Returns true (non-zero) if successful,
// false (0) if not --- insert fails if the key is already in the
// tree (no changes are made to the tree in this case).
//
#define _TRUE  1
#define _FALSE 0

int AVLInsert(AVL *tree, AVLKey key, AVLValue value)
{
	AVLNode *prev = NULL;
	AVLNode *cur = tree->Root;

	AVLNode *stack[64];
	int      top = -1;

	//
	// first we search the tree to see if it already contains key:
	//
	while (cur != NULL)
	{
		top++;
		stack[top] = cur;

		if (AVLCompareKeys(key, cur->Key) == 0)  // already in tree, failed:
			return _FALSE;
		else if (AVLCompareKeys(key, cur->Key) < 0)  // smaller, go left:
		{
			prev = cur;
			cur = cur->Left;
		}
		else  // larger, go right:
		{
			prev = cur;
			cur = cur->Right;
		}
	}

	// 
	// If we get here, tree does not contain key, so insert new node
	// where we fell out of tree:
	//
	AVLNode *newNode = (AVLNode *)malloc(sizeof(AVLNode));
	newNode->Key = key;
	newNode->Value = value;
	newNode->Left = NULL;
	newNode->Right = NULL;
	newNode->Height = 0;

	//
	// link T where we fell out of tree -- after prev:
	//
	if (prev == NULL)  // tree is empty, insert @ root:
	{
		tree->Root = newNode;
	}
	else if (AVLCompareKeys(key, prev->Key) < 0)  // smaller, insert to left:
	{
		prev->Left = newNode;
	}
	else  // larger, insert to right:
	{
		prev->Right = newNode;
	}

	tree->Count++;

	//
	// Now walk back up the tree, updating heights and looking for
	// where the AVL balancing criteria may be broken.  If we reach
	// a node where the height doesn't change, then we're done -- the
	// tree is still balanced.  If we reach a node where the AVL 
	// condition is broken, we fix locally and we're done.  One or two
	// local rotations is enough to re-balance the tree.
	//
	AVLNode *N;
	int      rebalance = _FALSE;  // false by default, e.g. if tree is empty:

	while (top >= 0)  // stack != empty::
	{
		N = stack[top];  // N = pop();
		top--;

		int hl = _height(N->Left);
		int hr = _height(N->Right);
		int newH = 1 + _max2(hl, hr);

		if (newH == N->Height)  // heights the same, still an AVL tree::
		{
			rebalance = _FALSE;
			break;
		}
		else if (abs(hl - hr) > 1)  // AVL condition broken, we have to fix::
		{
			rebalance = _TRUE;
			break;
		}
		else  // update height and continue walking up tree::
		{
			N->Height = newH;
		}
	}

	//
	// Okay, does the tree need to be rebalanced?
	//
	if (rebalance)
	{
		cur = N;

		//
		// if we get here, then the AVL condition is broken at "cur".  So we
		// have to decide which of the 4 cases it is and then rotate to fix.
		//

		// we need cur's parent, so pop the stack one more time
		if (top < 0)     // stack is empty, ==> N is root
			prev = NULL;   // flag this with prev == NULL
		else  // stack not empty, so obtain ptr to cur's parent:
			prev = stack[top];

		//
		// which of the 4 cases?
		//
		if (AVLCompareKeys(newNode->Key, cur->Key) < 0)  // case 1 or 2:
		{
			// case 1 or case 2?  either way, we know cur->left exists:
			AVLNode *L = cur->Left;

			if (AVLCompareKeys(newNode->Key, L->Key) > 0)
			{
				// case 2: left rotate @L followed by a right rotate @cur:
				cur->Left = LeftRotate(L);
			}

			// case 1 or 2:  right rotate @cur
			if (prev == NULL)
				tree->Root = RightRotate(cur);
			else if (prev->Left == cur)
				prev->Left = RightRotate(cur);
			else
				prev->Right = RightRotate(cur);
		}
		else
		{
			//
			// case 3 or case 4?  either way, we know cur->right exists:
			//
			AVLNode *R = cur->Right;

			if (AVLCompareKeys(newNode->Key, R->Key) < 0)
			{
				// case 3: right rotate @R followed by a left rotate @cur:
				cur->Right = RightRotate(R);
			}

			// case 3 or case 4:  left rotate @cur:
			if (prev == NULL)
				tree->Root = LeftRotate(cur);
			else if (prev->Left == cur)
				prev->Left = LeftRotate(cur);
			else
				prev->Right = LeftRotate(cur);
		}
	}

	//
	// done:
	//
	return _TRUE;  // success:
}


AVL *BuildTree1(char *filename1)
{//Declaration of variables, and opening file
	char anotherarray[512];
	FILE *file1 = fopen(filename1, "r");
	AVLValue  value;
	AVLKey key;
	AVL *tree = AVLCreate();
	//Declaring as many tokens as required
	char *token = NULL;
	char *token2 = NULL;
	char *token3 = NULL;
	char *token4 = NULL;
	char *token5 = NULL;

	int anotherarrayline = (sizeof(anotherarray)/ sizeof(anotherarray[0]));//Line size
	fgets(anotherarray, anotherarrayline, file1); //Getting first line fo input

	while (fgets(anotherarray, 512, file1) != NULL) {  //While we are not at end of line
		anotherarray[strcspn(anotherarray, "\r\n")] = '\0'; //Add NULL character for safety


		value.Type = STATIONTYPE; //Setting the type
		int id;
		token = strtok(anotherarray, ",");  //Strip the input
		id = atoi(token); //Converting to number
		value.Station.StationID = id; //Storing the number
		key = id;//Storing the number
		//printf("%d\n", value.Station.StationID);


		token2 = strtok(NULL, ",");//Strip the input
		value.Station.Stationname = (char*)malloc((strlen(token2) + 1) * sizeof(char));//mallocing space for char
		strcpy(value.Station.Stationname, token2); //Str copying the chars into NODE
		//printf("%s\n", value.Station.Stationname);

		token3 = strtok(NULL, ","); //Strip the input
		double latitude = atof(token3);//Converting to floart
		value.Station.Stationlatitude = latitude;
		//printf("%f\n", value.Station.Stationlatitude);


		token4 = strtok(NULL, ",");//Strip the input
		double longitude = atof(token4);
		value.Station.Stationlongitude = longitude;
		//printf("%f\n", value.Station.Stationlongitude);


		token5 = strtok(NULL, ",");//Strip the input
		int capacity = atoi(token5);
		value.Station.Stationcapactity = capacity;
		//printf("%d\n", value.Station.Stationcapactity);

		value.Station.Stationtrip = 0; //Setting to 0

		AVLInsert(tree, key, value); //Inserting the kets
		
	}
	//printf("Leaving\n");
	fclose(file1); //Close file
	return tree; //return the tree
}

AVL *BuildTree2(char *filename1, AVL *PassedTree)
{//Declaration of variables, and opening file
	char anotherarray[512];
	FILE *file1 = fopen(filename1, "r");
	AVLValue  value;
	AVLKey key;
	AVL *tree = AVLCreate();
	//Declaring as many tokens as required
	char *token = NULL;
	char *token2 = NULL;
	char *token3 = NULL;
	char *token4 = NULL;
	char *token5 = NULL;
	char *token6 = NULL;
	char *token7 = NULL;
	char *token8 = NULL;

	long long weight;

	int anotherarrayline = (sizeof(anotherarray)/ sizeof(anotherarray[0]));
	fgets(anotherarray, anotherarrayline, file1);

	while (fgets(anotherarray, anotherarrayline, file1) != NULL) {  //While we are not at end of line
		anotherarray[strcspn(anotherarray, "\r\n")] = '\0';


		value.Type = TRIPTYPE;
		int idtrip;
		token = strtok(anotherarray, ",");  //Strip the input
		idtrip = atoi(token);//converting to int
		value.Trip.TripID = idtrip;//Storing the int into NODE
		key = idtrip;
		//printf("%d\n", value.Trip.TripID);


		token2 = strtok(NULL, ",");//Strip the input
		token3 = strtok(NULL, ",");//Strip the input

		int BIKEID;
		token4 = strtok(NULL, ",");//Strip the input
		BIKEID = atoi(token4); //converting to int
		value.Trip.BikeID = BIKEID;//Storing the int into NODE
		//printf("%d\n", value.Trip.BikeID);


		token5 = strtok(NULL, ",");//Strip the input
		int tripDur;
		tripDur = atoi(token5);//converting to int
		value.Trip.Duration = tripDur;//Storing the int into NODE
		//printf("%d\n", value.Trip.Duration);


		token6 = strtok(NULL, ",");//Strip the input
		int from = atoi(token6);//converting to int
		value.Trip.From = from; //Storing the int into NODE
		//printf("%d\n", value.Trip.From);

		token7 = strtok(NULL, ",");//Strip the input

		token8 = strtok(NULL, ",");//Strip the input
		int to = atoi(token8);//converting to int
		value.Trip.To = to;//Storing the int into NODE
		//printf("%d\n", value.Trip.To);
		
		//PassedTree->Root

		AVLInsert(tree, key, value); //Inserting into tree

		StationNBikeTripCount(PassedTree, value.Trip.From); //Calculating From trip count
		StationNBikeTripCount(PassedTree, value.Trip.To);//Calculating To trip count 


		
	}
	//printf("Leaving\n");
	fclose(file1); //Close file
	return tree; //return the tree
}

AVL *BuildTree3(char *filename1, AVL *PassedTree)
{//Declaration of variables, and opening file
	char anotherarray[512];
	FILE *file1 = fopen(filename1, "r");
	AVLValue  value;
	AVLKey key;
	AVL *tree = AVLCreate();
	//Declaring as many tokens as required
	char *token = NULL;
	char *token2 = NULL;
	char *token3 = NULL;
	char *token4 = NULL;

	int anotherarrayline = (sizeof(anotherarray)/ sizeof(anotherarray[0]));
	fgets(anotherarray, anotherarrayline, file1);
	value.Bike.TripCount = 0;
	while (fgets(anotherarray, anotherarrayline, file1) != NULL) {  //While we are not at end of line
		anotherarray[strcspn(anotherarray, "\r\n")] = '\0';

		value.Type = BIKETYPE;

		token = strtok(anotherarray, ",");//Strip the input
		token2 = strtok(NULL, ",");//Strip the input
		token3 = strtok(NULL, ",");//Strip the input

		token4 = strtok(NULL, ",");//Strip the input
		int BikeID = atoi(token4); //Convert to int
		value.Bike.BikeID = BikeID;//Store int into NODE
		key = BikeID; //store into key

		AVLInsert(tree, key, value); //insert into tree

		StationNBikeTripCount(tree, BikeID);//Getting the trip
	}
	//printf("Leaving\n");
	fclose(file1); //Close file
	return tree; //return the tree
}

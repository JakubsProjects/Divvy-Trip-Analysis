/*avl.h*/

//
// AVL Tree ADT header file.
//
// <<Jakub>>
// U. of Illinois, Chicago
// CS251, Spring 2017
//

// make sure this header file is #include exactly once:
#pragma once


//
// AVL type declarations:
//
typedef int  AVLKey;

typedef struct STATION
{
	int  StationID;
	char* Stationname;
	double Stationlatitude;
	double Stationlongitude;
	int Stationcapactity;
	int Stationtrip;

} STATION;

typedef struct TRIP
{
	int  TripID;
	int BikeID;
	int From;
	int To;
	int  Duration;

} TRIP;

typedef struct BIKE
{
	int  BikeID;
	int  TripCount;
	int From;
	int To;

} BIKE;

enum UNIONTYPE
{
	STATIONTYPE,
	TRIPTYPE,
	BIKETYPE
};

typedef struct NewData
{
	int  StationID;
	double distance;
}NewData;


typedef struct AVLValue
{
	enum UNIONTYPE Type;  // Station, Trip, or Bike:
	union
	{
		STATION  Station;   // union => only ONE of these is stored:
		TRIP     Trip;
		BIKE     Bike;
	};
} AVLValue;

typedef struct AVLNode
{
	AVLKey    Key;
	AVLValue  Value;
	struct AVLNode  *Left;
	struct AVLNode  *Right;
	int       Height;
} AVLNode;

typedef struct AVL
{
	AVLNode *Root;
	int      Count;
} AVL;


//
// AVL API: function prototypes
//
AVL *AVLCreate();
void AVLFree(AVL *tree, void(*fp)(AVLKey key, AVLValue value));
void freeAVLNodeKeyVal(AVLKey key, AVLValue value);

int      AVLCompareKeys(AVLKey key1, AVLKey key2);
AVLNode *AVLSearch(AVL *tree, AVLKey key);
int      AVLInsert(AVL *tree, AVLKey key, AVLValue value);
void sort(NewData *value, int size);
AVL *BuildTree1(char *filename1);
AVL *BuildTree2(char *filename1, AVL *PassedTree);
AVL *BuildTree3(char *filename1, AVL *PassedTree);
int StationNBikeTripCount(AVL *tree, AVLKey key);
int matchingDistance(AVLNode *cur, double latitude, double longitude, double distance, NewData *NewStruct, int *count);
int SNDrouteFunc(AVLNode *root, NewData *NewStruct2, NewData *NewStruct3, int *NewCounter, int count, int count2);

double distBetween2Points(double lat1, double long1, double lat2, double long2);

int  AVLCount(AVL *tree);
int  AVLHeight(AVL *tree);
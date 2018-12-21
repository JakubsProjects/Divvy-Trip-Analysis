/*main.cpp*/

//
// Divvy Bike Ride Route Analysis, using AVL trees.
//
// <<Jakub>>
// U. of Illinois, Chicago
// CS251, Spring 2017
// Project #04
//

// ignore stdlib warnings if working in Visual Studio:
#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "avl.h"



//
// distBetween2Points: 
//
// Returns the distance in miles between 2 points (lat1, long1) and (lat2, long2).
// 
double distBetween2Points(double lat1, double long1, double lat2, double long2)
{
	//
	// Reference: http://www8.nau.edu/cvm/latlon_formula.html
	//
	double PI = 3.14159265;
	double earth_rad = 3963.1;  // statue miles:

	double lat1_rad = lat1 * PI / 180.0;
	double long1_rad = long1 * PI / 180.0;
	double lat2_rad = lat2 * PI / 180.0;
	double long2_rad = long2 * PI / 180.0;

	double dist = earth_rad * acos(
		(cos(lat1_rad)*cos(long1_rad)*cos(lat2_rad)*cos(long2_rad))
		+
		(cos(lat1_rad)*sin(long1_rad)*cos(lat2_rad)*sin(long2_rad))
		+
		(sin(lat1_rad)*sin(lat2_rad))
	);

	return dist;
}


//
// freeAVLNodeData
//
// Works with AVLFree() to free the data inside (key, value) pairs.
//
void freeAVLNodeData(AVLKey key, AVLValue value)
{
	//
	// what we free depends on what type of value we have:
	//
	if (value.Type == STATIONTYPE)
	{

	}
	else if (value.Type == TRIPTYPE)
	{

	}
	else if (value.Type == BIKETYPE)
	{

	}
	else
	{
		printf("**ERROR: unexpected value type in freeAVLNodeData!\n\n");
		exit(-1);
	}
}


//
// getFileName: 
//
// Inputs a filename from the keyboard, make sure the file can be
// opened, and returns the filename if so.  If the file cannot be 
// opened, an error message is output and the program is exited.
//
char *getFileName()
{
	char filename[512];
	int  fnsize = sizeof(filename) / sizeof(filename[0]);

	// input filename from the keyboard:
	fgets(filename, fnsize, stdin);
	filename[strcspn(filename, "\r\n")] = '\0';  // strip EOL char(s):

												 // make sure filename exists and can be opened:
	FILE *infile = fopen(filename, "r");
	if (infile == NULL)
	{
		printf("**Error: unable to open '%s'\n\n", filename);
		exit(-1);
	}

	fclose(infile);

	// duplicate and return filename:
	char *s = (char *)malloc((strlen(filename) + 1) * sizeof(char));
	strcpy(s, filename);

	return s;
}


//
// skipRestOfInput:
//
// Inputs and discards the remainder of the current line for the 
// given input stream, including the EOL character(s).
//
void skipRestOfInput(FILE *stream)
{
	char restOfLine[256];
	int rolLength = sizeof(restOfLine) / sizeof(restOfLine[0]);

	fgets(restOfLine, rolLength, stream);
}


///////////////////////////////////////////////////////////////////////
//
// main:
//
int main()
{
	printf("** Welcome to Divvy Route Analysis **\n");

	//
	// get filenames from the user/stdin:
	//
	char  cmd[64];
	char *StationsFileName = getFileName();
	char *TripsFileName = getFileName();

	//
	// As an example, create some trees and insert some
	// dummy (key, value) pairs:
	//
	AVL *stations = BuildTree1(StationsFileName);
	AVL *trips = BuildTree2(TripsFileName, stations);
	AVL *bikes = BuildTree3(TripsFileName, trips);

	
	//
	// now interact with user:
	//
	printf("** Ready **\n");

	scanf("%s", cmd);

	while (strcmp(cmd, "exit") != 0)
	{
		if (strcmp(cmd, "stats") == 0)
		{
			//
			// Output some stats about our data structures:
			//
			printf("** Trees:\n");

			printf("   Stations: count = %d, height = %d\n",
				AVLCount(stations), AVLHeight(stations));
			printf("   Trips:    count = %d, height = %d\n",
				AVLCount(trips), AVLHeight(trips));
			printf("   Bikes:    count = %d, height = %d\n",
				AVLCount(bikes), AVLHeight(bikes));
		}
		else if (strcmp(cmd, "station") == 0) {

			//reading in Stations Number
			int StationNum;
			scanf("%d", &StationNum);

			AVLNode * returnedNode = AVLSearch(stations, StationNum); //Searching StationNumber in stations.csv
			if (returnedNode == NULL) { //If returns NULL
				printf("**not found\n"); //print this
			}

			else {
				int count = 0; //Setting count to 0
				printf("**Station %d:\n", returnedNode->Value.Station.StationID); //Print out the station returned
				printf("  Name: '%s'\n", returnedNode->Value.Station.Stationname); //Print out the station name
				//Print out station location
				printf("  Location:   (%f,%f)\n", returnedNode->Value.Station.Stationlatitude, returnedNode->Value.Station.Stationlongitude);
				printf("  Capacity:   %d\n", returnedNode->Value.Station.Stationcapactity);//Print out the station Capacity

				printf("  Trip count: %d\n", returnedNode->Value.Station.Stationtrip); //Print out the station trip
			}
		}
		else if (strcmp(cmd, "trip") == 0) {

			int TripNum;
			scanf("%d", &TripNum);
			AVLNode * returnedNode = AVLSearch(trips, TripNum); //Searching for tripNum in trips.csv
			if (returnedNode == NULL) { //If returns NULL
				printf("**not found\n");//print this 
			}
			else {//otherwise
				printf("**Trip %d:\n", returnedNode->Value.Trip.TripID); //Print the tripID
				printf("  Bike: %d\n", returnedNode->Value.Trip.BikeID);//Print the BikeID
				printf("  From: %d\n", returnedNode->Value.Trip.From);//Print the From station val
				printf("  To:   %d\n", returnedNode->Value.Trip.To);//Print the To station val

				int minutes, seconds = 0; //Declaring minutes and seconds
				minutes = returnedNode->Value.Trip.Duration / 60; //Divide by 60 to get minutes
				seconds = returnedNode->Value.Trip.Duration % 60; //Mod by 60 to get remainder

				printf("  Duration: %d min, %d secs\n", minutes, seconds); //Print out result of mins & secs


			}

		}
		else if (strcmp(cmd, "bike") == 0) {

			int BikeNum;
			scanf("%d", &BikeNum);//Reading in Bike Number

			AVLNode * returnedNode = AVLSearch(bikes, BikeNum);
			if (returnedNode == NULL) {//If returns NULL
				printf("**not found\n");//print this 
			}
			else {//otherwise
				printf("**Bike %d:\n", returnedNode->Value.Bike.BikeID); //Prints BikeID

				printf("  Trip count: %d\n", returnedNode->Value.Bike.TripCount); //Prints the trip count
			}
		}
		else if (strcmp(cmd, "find") == 0) {

			double latitude, longitude, distance = 0;

			scanf(" %lf", &latitude); //reading in userinput latitude
			scanf(" %lf", &longitude);//reading in userinput longitude
			scanf(" %lf", &distance);//reading in userinput distance

			int size = stations->Count;  //Storing the tree count into size
			
			NewData *NewStruct = (NewData*)malloc(sizeof(NewData)*size); //Declaring a new struct to hold element of ID & distance

			int count = 0; //Declaring count
			
			matchingDistance(stations->Root, latitude, longitude, distance, NewStruct, &count); //Passing in station file, lat,long, and struct

				sort(NewStruct, count); //Sorting the new data in the new struct

				int l;
				for (l = 0; l < count; l++) {
					printf("Station %d: distance %lf miles\n", NewStruct[l].StationID, NewStruct[l].distance); //Printing out sorted info
				}
				free(NewStruct); //Freeing memory at the end
		}

		else if (strcmp(cmd, "route") == 0) {
			
			int tripID;
			double distance;
			scanf(" %d", &tripID); //Reading in  tripID
			scanf(" %lf", &distance); //Reading in the distance

			AVLNode *returnedNode = AVLSearch(trips, tripID); //Searching tripID in trips.csv 

			if (returnedNode == NULL || distance < 0.0) { //If Node returns NULL and Distance is neg
				printf("**not found\n"); //Print this 
				
			}
			else { //Otherwise
				AVLNode *returnedNode2 = AVLSearch(stations, returnedNode->Value.Trip.From); //Get the node with FromStation value
				AVLNode *returnedNode3 = AVLSearch(stations, returnedNode->Value.Trip.To);//Get the node with ToStation value

				//Initializing counts
				int count = 0;
				int count2 = 0;
				int size = stations->Count;  //Storing stations count into size

				NewData *NewStruct2 = (NewData*)malloc(sizeof(NewData)*size); //Allocating memory for FromStation struct
				NewData *NewStruct3 = (NewData*)malloc(sizeof(NewData)*size); //Allocating memory for ToStation struct

				//Sending into function to calculate the tripcount in FromStation
				matchingDistance(stations->Root, returnedNode2->Value.Station.Stationlatitude, returnedNode2->Value.Station.Stationlongitude, distance, NewStruct2, &count);

				//Sending into function to calculate the tripcount in ToStation
				matchingDistance(stations->Root, returnedNode3->Value.Station.Stationlatitude, returnedNode3->Value.Station.Stationlongitude, distance, NewStruct3, &count2);

				//Declaring new count variable
				int NewCounter = 0;

				SNDrouteFunc(trips->Root, NewStruct2, NewStruct3, &NewCounter, count, count2);

				printf("** Route: from station #%d to station #%d\n", returnedNode->Value.Trip.From, returnedNode->Value.Trip.To); //Printing Route

				printf("** Trip count: %d\n", NewCounter); //Printing trip count

				printf("** Percentage: %lf%%\n", ((double)NewCounter / (double)trips->Count) *100.0);
				free(NewStruct2);//Freeing memory for FromStation struct
				free(NewStruct3);//Freeing memory for ToStation struct
			}
		}
		else
		{
			printf("**unknown cmd, try again...\n");
		}

		scanf("%s", cmd);
	}

	//
	// done, free memory and return:
	//
	printf("** Freeing memory **\n");

	AVLFree(stations, freeAVLNodeData);//Freeing data for stations tree
	AVLFree(trips, freeAVLNodeData);//Freeing data for trips tree
	AVLFree(bikes, freeAVLNodeData);//Freeing data for bikes tree

	free(StationsFileName); //Freeing the StationsFile
	free(TripsFileName);//Freeing the TripsFile

	free(stations);//Freeing stations
	free(trips);//Freeing trips
	free(bikes);//Freeing Bikes

	printf("** Done **\n"); //Happiness!

	return 0;
}


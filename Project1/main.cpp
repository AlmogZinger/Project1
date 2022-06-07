﻿#include "Family.h"
#include <queue>
#include <iostream>
#include <fstream>
#include <exception>


using namespace std;

enum { EXIT, ADD, DEL, COUNT, UPDATE, WAITING, PRINT };


//enum ACTIVITY {
//	NONE, //  טרם בחר חוג
//	SWIMMING,          // שחיה 
//	GYMNATSTICS,         // התעמלות קרקע
//	DANCE = 4,     // מחול
//	ART = 8,    // אומנות
//	SELF_DEFENSE = 16, // הגנה עצמית
//	MUSIC = 32, // מוסיקה
//	DRAMA = 64, // דרמה
//	BASKETBALL = 128  // כדור סל
//};

void setFile(fstream& f)
{
	int sizeOfFamily = 100;
	Family fm; //Crearte a empty family to update.

	for (int i = 0; i < sizeOfFamily; i++)
	{
		f.write((char*)&fm, sizeof(Family));
	}

	f.close();
}

void add(fstream& f) //Add a new family to the file
{
	int new_id, new_amount, new_phoneNum;
	char new_name[20];
	cout << "enter the family's:  id, name, amount of people and a phone number:" << endl;


	cin >> new_id;		//get id
	if (new_id < 0 || new_id >100)
		throw "ERROR: Invalid family number";

	/*get name in char[20]*/
	char name[20];
	cin.get(name, 20, ' ');
	char c;
	cin >> c;

	cin >> new_amount;			//get amount
	while (new_amount < 0)
	{
		cout << "ERROR in amount of pepole";
		cin >> new_amount;
	}

	cin >> new_phoneNum;		//Get phon num
	while (new_phoneNum < 0)
	{
		cout << "ERROR in phone num";
		cin >> new_phoneNum;
	}

	//Sarching the family in file by id.
	Family temp;
	f.seekg((new_id - 1) * sizeof(Family)); //Move f to read to the spote of id family
	f.read((char*)&temp, sizeof(Family)); //Read the data in the spote to temp family
	if (temp.getId() == new_id)
		throw "“ERROR: Family is already in the file";

	//Write the famoily to the file
	Family fm(new_id, name, new_amount, new_phoneNum);
	f.seekp((new_id - 1) * sizeof(Family)); //Move f to write  to the spote of id family
	f.write((char*)&fm, sizeof(Family));
}
void  del(fstream& f, int ID) //Delete a family from the file
{
	//Throw error if id is invalid
	if (ID < 0 || ID >100)
		throw "ERROR: Invalid family number";

	//Sarching the family in file by id.
	Family temp;
	f.seekg((ID - 1) * sizeof(Family)); //Move f to read to the spote of id family
	f.read((char*)&temp, sizeof(Family)); //Read the data in the spote to temp family
	if (temp.getId() != ID)
		throw "“ERROR: Family is not in the file";

	//If found the family 
	temp.setId(0); //Set the id to 0 - remote the family from the file
	f.seekg((ID - 1) * sizeof(Family)); //Move f to write to the spote of id family
	f.write((char*)&temp, sizeof(Family)); //rewrite to the same place. 

}

int count(fstream& f, ACTIVITY hog) //Count and return the amount of family that in the hog
{
	int size_of_id = 100, count = 0;
	Family fm;
	f.seekg(ios::beg); //Upset f to the start of the file.
	for (int i = 0; i < size_of_id; i++)
	{
		f.read((char*)&fm, sizeof(Family)); //Read the next family from the file
		if (fm.getClassList() & hog) //Check if the family registered to the class.
			count++;			//If the family registered - promotes count in one
	}

	return count;
}

void updateClass(fstream& f, string msg, ACTIVITY hog, Family& wait, Family& temp)
{
	char choose;
	cout << msg << endl;
	cin >> choose;
	if (choose != 'n' && choose != 'N' && choose != 'y' && choose != 'Y') //If choose not 'n','N','y','Y'
		throw "ERROR: Invalid response";

	if (count(f, hog) < 10) //If there are place for the family.
		temp.setClassList(temp.getClassList() | hog); //Add the class to the calss list of the family
	else
		wait.setClassList(wait.getClassList() | hog); //Add a request
}

void update(fstream& f, int ID, queue<Family> waitQ) //
{/*-----------------------------THROW---------------------------------*/
	//Throw error if id is invalid
	if (ID < 0 || ID >100)
		throw "ERROR: Invalid family number";

	//Throe error if family not found
	Family temp;
	f.seekg((ID - 1) * sizeof(Family)); //Upset f to the spote of the id family.
	f.read((char*)&temp, sizeof(Family)); // Read the family in the spote og the id to temp
	if (temp.getId() != ID)					//If not found
		throw "ERROR: Family is not in the file";


	/*---------------------------------------------------------------------*/

	Family wait = temp; //The family obj to the wait Queue
	wait.setClassList(NONE);

	updateClass(f, "Do you want to register for swimming?", (ACTIVITY)SWIMMING, wait, temp);
	updateClass(f, "Do you want to register for gymnastics?", (ACTIVITY)GYMNATSTICS, wait, temp);
	updateClass(f, "Do you want to register for dance?", (ACTIVITY)DANCE, wait, temp);
	updateClass(f, "Do you want to register for art?", (ACTIVITY)ART, wait, temp);
	updateClass(f, "Do you want to register for self defense?", (ACTIVITY)SELF_DEFENSE, wait, temp);
	updateClass(f, "Do you want to register for music?", (ACTIVITY)MUSIC, wait, temp);
	updateClass(f, "Do you want to register for drama?", (ACTIVITY)DRAMA, wait, temp);
	updateClass(f, "Do you want to register basketball?", (ACTIVITY)BASKETBALL, wait, temp);

	if (wait.getClassList() != 0) //THey wait for at least one hog.
		waitQ.push(wait); //Push the family to the wait queue.

	//Write the family to the file.
	f.seekp((ID - 1) * sizeof(Family));
	f.write((char*)&temp, sizeof(Family));

}




void waiting(queue<Family> waitQ) //Print all waiting families
{
	if (waitQ.empty())
		return;

	Family flag;//Insert a flag
	waitQ.push(flag);

	Family temp = waitQ.front();

	while (temp.getId() != 0)
	{
		cout << temp << endl;
		waitQ.push(temp);
		temp = waitQ.front();
		waitQ.pop();
	}
}

void print(fstream& f, int ID) // Print the family.
{
	//Throw error if id is invalid
	if (ID < 0 || ID >100)
		throw "ERROR: Invalid family number";

	//Throe error if family not found
	Family temp;
	f.seekg((ID - 1) * sizeof(Family)); //Upset f to the spote of the id family.
	f.read((char*)&temp, sizeof(Family)); // Read the family in the spote og the id to temp
	if (temp.getId() != ID)					//If not found
		throw "ERROR: Family is not in the file";

	cout << temp;
}


void handleCount(fstream& file) {
	int snum;
	cout << "enter activity number to count:\n";
	cout << "Choices are:\n1 SWIMMING\n2 GYMNASTICS\n3 DANCE\n4 ART\n5 SELF DEFENSE \n6 MUSIC \n7 DRAMA\n8 BASKETBALL\n";
	cin >> snum;
	cout << "there are ";
	switch (snum) {
	case 1:
		cout << count(file, (ACTIVITY)SWIMMING);
		break;
	case 2:
		cout << count(file, (ACTIVITY)GYMNATSTICS);
		break;
	case 3:
		cout << count(file, (ACTIVITY)DANCE);
		break;
	case 4:
		cout << count(file, (ACTIVITY)ART);
		break;
	case 5:
		cout << count(file, (ACTIVITY)SELF_DEFENSE);
		break;
	case 6:
		cout << count(file, (ACTIVITY)MUSIC);
		break;
	case 7:
		cout << count(file, (ACTIVITY)DRAMA);
		break;
	case 8:
		cout << count(file, (ACTIVITY)BASKETBALL);
		break;
	}
	cout << " families registered\n";
}


int main()
{
	queue<Family> q;

	queue<Family> jv;
	fstream file;
	file.open("families.txt", ios::binary | ios::in | ios::out);
	if (!file)
	{
		cout << "ERROR: couldn't open file\n";
		return 0;
	}
	setFile(file);
	file.clear();

	int choice;
	int snum;
	int cnum;
	cout << "Choices are:\n0 to exit\n1 to add a family\n2 to delete a family\n3 to  count number of families signed up\n4 to update the activities of a family \n5 print the waiting list \n6 to print a family\n";
	cout << "enter 0-6:\n";
	cin >> choice;
	while (choice)
	{
		try {
			switch (choice)
			{
			case ADD://add to the file
				add(file);
				break;
			case DEL://delete from file
				cout << "enter number of family to delete:\n";
				cin >> snum;
				del(file, snum);
				break;
			case COUNT:
				handleCount(file);
				break;
			case UPDATE://update the list of classes of a family
				cout << "enter number of family to update:\n";
				cin >> snum;
				update(file, snum, jv);
				break;
			case WAITING://update the list of classes of a waiting family
				waiting(jv);
				break;

			case PRINT://print the details of a specific family
				cout << "enter number of family to print:\n";
				cin >> snum;
				print(file, snum);
				break;
			default:
				cout << "ERROR: invalid choice\n";

			}
		}// try
		catch (exception& e) {
			cout << e.what();
		}
		cout << "\nenter 0-6:\n";
		cin >> choice;

	}
	file.close();
	return 0;
}

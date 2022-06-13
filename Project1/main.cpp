#include "Family.h"
#include <queue>
#include <iostream>
#include <fstream>
#include <exception>
#include "Exception.h"


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

void setFile(fstream& f) // update the file with 100 empty families
{
	int sizeOfFamily = 100;
	Family fm; //Crearte a empty family to update.
	for (int i = 0; i < sizeOfFamily; i++)
	{
		f.write((char*)&fm, sizeof(Family));
	}		

}

void add(fstream& f) //Add a new family to the file
{
	int new_id, new_amount, new_phoneNum;
	char new_name[20];
	cout << "enter the family's:  id, name, amount of people and a phone number:" << endl;


	cin >> new_id>>new_name>>new_amount>>new_phoneNum;		//get id
	if (new_id < 0 || new_id >100)
	{
		cin.ignore(500, '\n');
		throw Exception("ERROR: Invalid family number");
	}
	

	while (new_amount < 0)
	{
		cout << "ERROR in amount of pepole";
		cin >> new_amount;
	}

	while (new_phoneNum < 0)
	{
		cout << "ERROR in phone num";
		cin >> new_phoneNum;
	}

	//Sarching the family in file by id.
	Family temp;
	f.seekg((new_id - 1) * sizeof(Family),ios::beg); //Move f to read to the spote of id family
	f.read((char*)&temp, sizeof(Family)); //Read the data in the spote to temp family
	if (temp.getId() == new_id) {
		throw Exception("ERROR: Family is already in the file");
	}
	//Write the famoily to the file
	Family fm(new_id, new_name, new_amount, new_phoneNum);
	f.seekp((new_id - 1) * sizeof(Family), ios::beg); //Move f to write  to the spote of id family
	f.write((char*)&fm, sizeof(Family));
}
void  del(fstream& f, int ID) //Delete a family from the file
{
	//Throw error if id is invalid
	if (ID < 0 || ID >100)
		throw Exception("ERROR: Invalid family number");

	//Sarching the family in file by id.
	Family temp;
	f.seekg((ID - 1) * sizeof(Family)); //Move f to read to the spote of id family
	f.read((char*)&temp, sizeof(Family)); //Read the data in the spote to temp family
	if (temp.getId() != ID)
		throw Exception("ERROR: Family is not in the file");

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
		throw Exception("ERROR: Invalid response");

	if (count(f, hog) < 10) //If there are place for the family.
		temp.setClassList(temp.getClassList() | hog); //Add the class to the calss list of the family
	else
		wait.setClassList(wait.getClassList() | hog); //Add a request
}

void update(fstream& f, int ID, queue<Family>& waitQ) //
{/*-----------------------------THROW---------------------------------*/
	//Throw error if id is invalid
	if (ID < 0 || ID >100)
		throw Exception("ERROR: Invalid family number");

	//Throe error if family not found
	Family temp;
	f.seekg((ID - 1) * sizeof(Family)); //Upset f to the spote of the id family.
	f.read((char*)&temp, sizeof(Family)); // Read the family in the spote og the id to temp
	if (temp.getId() != ID)					//If not found
		throw Exception("ERROR: Family is not in the file");


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
	waitQ.pop();

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
		throw Exception("ERROR: Invalid family number");

	//Throe error if family not found
	Family temp;
	f.seekg((ID - 1) * sizeof(Family)); //Upset f to the spote of the id family.
	f.read((char*)&temp, sizeof(Family)); // Read the family in the spote og the id to temp
	if (temp.getId() != ID)					//If not found
	{
		throw Exception("ERROR: Family is not in the file");
	}
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

	fstream file;
	file.open("families.txt", ios::binary | ios::in | ios::out);
	if (!file)
	{
		cout << "ERROR: couldn't open file\n";
		return 0;
	}

	setFile(file);
	file.close();

	file.open("families.txt", ios::binary | ios::in | ios::out);
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
				try {
					add(file);
				}
				catch (exception& e) {
					cout << e.what();
				}
				break;
			case DEL://delete from file
				cout << "enter number of family to delete:\n";
				cin >> snum;
				try {
					del(file, snum);
				}
				catch (exception& e) {
					cout << e.what();
				}
				break;
			case COUNT:
				try {
					handleCount(file);
				}
				catch (exception& e) {
					cout << e.what();
				}
				break;
			case UPDATE://update the list of classes of a family
				cout << "enter number of family to update:\n";
				cin >> snum;
				try {
					update(file, snum, q);
				}
				catch (exception& e) {
					cout << e.what();
				}
				break;
			case WAITING://update the list of classes of a waiting family
				try {
					waiting(q);
				}
				catch (exception& e) {
					cout << e.what();
				}
				break;

			case PRINT://print the details of a specific family
				cout << "enter number of family to print:\n";
				cin >> snum;
				try {
					print(file, snum);
				}
				catch (exception& e) {
					cout << e.what();
				}
				break;
			default:
				cout << "ERROR: invalid choice\n";
				cin.ignore(500, '\n'); //Ignore the rest of the line
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

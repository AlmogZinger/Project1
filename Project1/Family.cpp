
#include "Family.h"
#include <iostream>
#include <fstream>


void Family::setFile(fstream& f)
{
	int sizeOfFamily = 100;
	Family fm; //Crearte a empty family to update.

	for (int i = 0; i < sizeOfFamily; i++)
	{
		f.write((char*)&fm, sizeof(Family));
	}

	f.close();
}

void Family::add(fstream& f) //Add a new family to the file
{
	int new_id, new_amount, new_phoneNum;
	char new_name[20];
	cout << "enter the family's:  id, name, amount of people and a phone number:" << endl;
	
	
	cin >> new_id;		//get id
	if (new_id < 0 || new_id >100)
		throw "ERROR: Invalid family number";

	cin.get(new_name, 20,' '); //get name 
	cin.ignore(10000);// NOT make aproblem

	cin >> new_amount ;			//get amount
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
	if (temp.id == new_id)
		throw "“ERROR: Family is already in the file";

	//Write the famoily to the file
	Family fm (new_id, new_name, new_amount, new_phoneNum);
	f.seekp((new_id - 1) * sizeof(Family)); //Move f to write  to the spote of id family
	f.write((char*)&fm, sizeof(Family));
}

void  Family::del(fstream& f, int ID) //Delete a family from the file
{
	//Throw error if id is invalid
	if (ID < 0 || ID >100)
		throw "ERROR: Invalid family number";

	//Sarching the family in file by id.
	Family temp;
	f.seekg((ID - 1) * sizeof(Family)); //Move f to read to the spote of id family
	f.read((char*)&temp, sizeof(Family)); //Read the data in the spote to temp family
	if (temp.id != ID)
		throw "“ERROR: Family is not in the file";

	//If found the family 
	temp.id = 0; //Set the id to 0 - remote the family from the file
	f.seekg((ID - 1) * sizeof(Family)); //Move f to write to the spote of id family
	f.write((char*)&temp, sizeof(Family)); //rewrite to the same place. 

}

int Family::count(fstream& f, ACTIVITY hog) //Count and return the amount of family that in the hog
{
	int size_of_id = 100,count=0;
	Family fm;
	f.seekg(ios::beg); //Upset f to the start of the file.
	for (int i = 0; i < size_of_id; i++)
	{
		f.read((char*)&fm, sizeof(Family)); //Read the next family from the file
		if (fm.classList & hog) //Check if the family registered to the class.
			count++;			//If the family registered - promotes count in one
	}

	return count;
}

void Family::update(fstream& f, int ID, queue<Family> waitQ) //
{/*-----------------------------THROW---------------------------------*/
	//Throw error if id is invalid
	if (ID < 0 || ID >100)
		throw "ERROR: Invalid family number";

	//Throe error if family not found
	Family temp;
	f.seekg((ID - 1) * sizeof(Family)); //Upset f to the spote of the id family.
	f.read((char*)&temp, sizeof(Family)); // Read the family in the spote og the id to temp
	if (temp.id != ID)					//If not found
		throw "ERROR: Family is not in the file";


	/*---------------------------------------------------------------------*/

	Family wait = temp; //The family obj to the wait Queue
	wait.classList = NONE;

	updateClass(f, "Do you want to register for swimming?", SWIMMING, wait, temp);
	updateClass(f, "Do you want to register for gymnastics?", GYMNATSTICS, wait, temp);
	updateClass(f, "Do you want to register for dance?", DANCE, wait, temp);
	updateClass(f, "Do you want to register for art?", ART, wait, temp);
	updateClass(f, "Do you want to register for self defense?", SELF_DEFENSE, wait, temp);
	updateClass(f, "Do you want to register for music?", MUSIC, wait, temp);
	updateClass(f, "Do you want to register for drama?", DRAMA, wait, temp);
	updateClass(f, "Do you want to register basketball?", BASKETBALL, wait, temp);

	if (wait.classList != 0) //THey wait for at least one hog.
		waitQ.push(wait); //Push the family to the wait queue.

	//Write the family to the file.
	f.seekp((ID - 1) * sizeof(Family));
	f.write((char*)&temp, sizeof(Family));

}


void Family:: updateClass(fstream& f, string msg, ACTIVITY hog, Family& wait, Family& temp)
{
	char choose;
	cout << msg << endl;
	cin >> choose;
	if (choose != 'n' && choose != 'N' && choose != 'y' && choose != 'Y') //If choose not 'n','N','y','Y'
		throw "ERROR: Invalid response";

	if (count(f, hog) < 10) //If there are place for the family.
		temp.classList = temp.classList | hog; //Add the class to the calss list of the family
	else
		wait.classList = wait.classList | hog; //Add a request
}
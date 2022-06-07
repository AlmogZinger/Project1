
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
	f.seekg((new_id - 1) * sizeof(Family));
	f.read((char*)&temp, sizeof(Family));
	if (temp.id == new_id)
		throw "“ERROR: Family is already in the file";

	//Write the famoily to the file
	Family fm (new_id, new_name, new_amount, new_phoneNum);
	f.write((char*)&fm, sizeof(Family));
}
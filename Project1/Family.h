
/********************************
Almog Zinger 206554941
Uri Ehrlich 208967976
Programming in C++
Exercise 11
The program runs  a class of family
*********************************/
#pragma once
#ifndef Family_h
#define Family_h
#include <fstream>
#include <queue>
using namespace std;

enum ACTIVITY {
	NONE, //  טרם בחר חוג
	SWIMMING,          // שחיה 
	GYMNATSTICS,         // התעמלות קרקע
	DANCE = 4,     // מחול
	ART = 8,    // אומנות
	SELF_DEFENSE = 16, // הגנה עצמית
	MUSIC = 32, // מוסיקה
	DRAMA = 64, // דרמה
	BASKETBALL = 128  // כדור סל
};
class Family
{
public:
	Family() : id(0), familyName(""), amountPepole(0), phoneNum(0), classList(NONE) {}//Empty constractor

	Family(int ID, char name[], int  amount, int phone) : id(ID), amountPepole(amount), phoneNum(phone), classList(NONE) {//Constractor
		for (int i = 0; i < 20; i++) //Deep copy
			familyName[i] = name[i];
	}

	//Copy Constractor
	Family(const Family& other) : id(other.id), amountPepole(other.amountPepole),
		phoneNum(other.phoneNum), classList(other.classList)
	{
		for (int i = 0; i < 20; i++) //Deep copy
			familyName[i] = other.familyName[i];
	}

	//void setFile(fstream& f); //Update 100 places to families
	//void add(fstream& f); //Add a new family to the file
	//void del(fstream& f, int ID); //Delete a family from the file
	//int count(fstream& f, ACTIVITY hog); //Count and return the amount of family that in the hog
	//void update(fstream& f, int ID, queue<Family> waitList); //
	//void updateClass(fstream& f, string msg, ACTIVITY hog, Family& wait, Family& temp);
	//void waiting(queue<Family> waitQ); //Print all waiting families.
	//void print(fstream& f, int ID); // Print the family.
	friend ostream& operator<<(ostream& os, Family& fm);
	int getId() { return id; }
	void setId(int ID) { id=ID ; }

	short getClassList() { return classList; }
	void setClassList(short cl) { classList= cl; }

private:
	int id;  //The id of the family.
	char familyName[20]; //Family name.
	int amountPepole; //num of pepole in the family.
	int phoneNum; //The phone num of the family.
	short classList; //saving the invalved class og the famiy in binary.
};

#endif // !Family_h



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
#include <string>
using namespace std;

enum ACTIVITY {
	NONE,
	SWIMMING,          // שחיה 
	GYMNATSTICS,         // התעמלות קרקע
	DANCE = 4,     // מחול
	ART = 8,    // אומנות
	SELF_DEFENSE = 16, // הגנה עצמית
	MUSIC = 32, // מוסיקה
	DRAMA = 64 ,// דרמה
	BASKETBALL = 128 // כדור סל
};



class Family
{
public:
	Family(): id(0), familyName (""), amountPepole(0) , phoneNum(0), classList(NONE){}//Empty constractor

	Family(int ID, char name[20], int  amount, int phone) : id(ID), amountPepole(amount), phoneNum(phone), classList(NONE) {//Constractor
		for (int i = 0; i < 20; i++) //Deep copy
			familyName[i] = name[i];
	}

	void setFile(fstream & myBinaryFile); //Update 100 places to families
	void add(fstream& myBinaryFile); //Add a new family to the file

private:
	int id;  //The id of the family.
	char familyName[20]; //Family name.
	int amountPepole; //num of pepole in the family.
	int phoneNum; //The phone num of the family.
	short classList; //saving the invalved class og the famiy in binary.
};

#endif // !Family_h


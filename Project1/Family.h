
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

class Family
{
public:
	Family();
	~Family();

private:
	int id;  //The id of the family.
	char familyName[20]; //Family name.
	int numOfPepole; //num of pepole in the family.
	int phoneNum; //The phone num of the family.
	short classList; //saving the invalved class og the famiy in binary.
};

#endif // !Family_h


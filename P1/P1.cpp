/*Author: Chelsey Dunivan
  Created: October 5, 2012

Description: Takes in the definition of an FA, the strings to check, 
an output file and an option to minimize the FA. Runs the strings through the
machine, outputting the results to the file.

*/
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
//***********************************************
//STATE CLASS
//***********************************************
class State
	{

	int st, moveto;
	char symbol;
	
	public:
	State(){st=-1; symbol=' '; moveto=-1;}
	void setState(int num){st=num;}
	void setSymb(char s){symbol= s;}
	void setMove(int num){moveto= num;}
	int getState(){return st;}
	char getSymb(){return symbol;}
	int getMove(){return moveto;}
	
	};


/***********************************************
Prototypes
************************************************/
//**********************************************
int checkNum(string);
void splitAlpha(string, vector<char> &alph);
//**********************************************

/***********************************************
Main function
************************************************/
int main(int argc, char **argv)
{
vector<char> alphabet;			//to hold the input alphabet
vector< vector<int> > transitions; 	//Transition table
vector<string> testStrings;			//the strings to be checked in the machine
int NumStates,						//to hold the number of states in the FA
	NumAcc,							//the number of accepting states there are
	NumTrans;						//number of characters in the alphabet
string alpha,						//the string of the alphabet
	   stateNum,					//string version of number of states
	   acceptingNum,				//string version of number of accepting
	   accepting,					//the string containing the accepting states
	   temp;						//the temporary string for reading in
ifstream ifl;						//to read in the files
ofstream ofl;						//to write out to the files


//*check the arguments
if(argc != 4 && argc !=5)
	{
	cout << "Parameter Error: <machine description><string file><output>" << endl;
	exit(0);
	}
ifl.open(argv[1]);
if(!ifl)
	{
	cout << "Bad input file: " << argv[1] <<endl;
	exit(0);
	}

//********************************************************************
//		Read in Machine Description
//********************************************************************
cout << "Reading the description of the machine" << endl;
getline(ifl, alpha);
splitAlpha(alpha, alphabet);
getline(ifl, stateNum);
getline(ifl, acceptingNum);
NumStates=checkNum(stateNum);
NumTrans= NumStates * alphabet.size();
NumAcc=checkNum(acceptingNum);
getline(ifl, accepting);
cout << alpha << endl;
cout << NumStates << endl;
cout << NumAcc << endl;
cout << accepting << endl;
getline(ifl, temp);
int i=0;
while(i < NumTrans && !ifl.eof())
	{
	cout << temp << endl;
	getline(ifl, temp);
	i++;
	}
	
if (i!= NumTrans)
	{
	cout << "Input file has an incorrect number of transitions." << endl;
	cout << "Expected " << NumTrans << " transitions. Aborting" << endl;
	exit(0);
	}
ifl.close();
ifl.clear();

//********************************************************************
//		Read in Strings to be checked in the Machine
//********************************************************************
ifl.open(argv[2]);
if(!ifl)
	{
	cout << "Bad input file: " << argv[1] <<endl;
	exit(0);
	}
getline(ifl, temp);
while (ifl.good() && temp !="~")
	{
	cout << temp << endl;
	getline(ifl, temp);
	}
ifl.close();
ifl.clear();
}//end main

int checkNum(string number)
/*---------------------------------------------------------------------------------
DESCRIPTION: Checks the string to ensure a number was entered and converts it to an int
PARAMETERS: 
	number: string containing the value entered by the user, or from the text file
RETURNS: the number as an integer
-----------------------------------------------------------------------------------*/
{ int theNumber; //the thing we will be returning
//*** LOOPS THROUGH THE STRING CONTAINING THE NUMBER AND ENSURES IT'S A DIGIT
for(int i=0; i<number.size(); i++)
	{
	if(!isdigit(number[i]))
		return -1;
	}
	theNumber=atoi(number.c_str());
	return theNumber;
}

void splitAlpha(string alphaString, vector<char> &alph)
/*---------------------------------------------------------------------------------
DESCRIPTION: Breaks the alphabet string into individual characters
PARAMETERS: 
	alphaString: the string containing the input alphabet
	alphabet: a vector to store the alphabet once broken apart
RETURNS: N/A
-----------------------------------------------------------------------------------*/
	{
	for (int i=0; i < alphaString.size(); i++)
		{
		if (!isspace(alphaString[i]) && !islower(alphaString[i]))
			{
			cout << "Invalid alphabet character. " << endl;
			cout << "Offending character: " << alphaString[i] << endl;
			cout << "Must be a-z lowercase. Aborthing." << endl;
			exit(0);
			}
			
		if (islower(alphaString[i]))
			alph.push_back(alphaString[i]);
		}
	}
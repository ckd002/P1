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
void splitAccepting(string, vector<int> &acc);
State initializeTrans(string);
void validateStrings(const vector<char> &alph, string input);
//**********************************************

/***********************************************
Main function
************************************************/
int main(int argc, char **argv)
{
vector<char> alphabet;				//to hold the input alphabet
vector<int> acceptingStates;		//to hold the accepting states. 
vector<State> transitions; 			//Transition table
vector<string> testStrings;			//the strings to be checked in the machine
int NumStates,						//to hold the number of states in the FA
	NumAcc,							//the number of accepting states there are
	NumTrans,						//number of characters in the alphabet
	currentState;					//numerical representation of which state you are currently in.
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
splitAccepting(accepting, acceptingStates);
cout << alpha << endl;
cout << NumStates << endl;
cout << NumAcc << endl;
cout << accepting << endl;
getline(ifl, temp);
int i=0;
while(i < NumTrans && !ifl.eof())
	{
	cout << temp << endl;
	transitions.push_back(initializeTrans(temp));
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
cout << "Machine Description Read." << endl;
cout << "Reading in Strings to Check" << endl;

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
	validateStrings(alphabet,temp);
	getline(ifl, temp);
	}
ifl.close();
ifl.clear();
cout << "Strings Read and Validated. Checking if they're in the language" << endl;
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

void splitAccepting(string acceptString, vector<int> &acc)
/*---------------------------------------------------------------------------------
DESCRIPTION: Breaks the accepting state string into individual states
PARAMETERS: 
	acceptString: the string containing the accepting states
	acc: a vector to store the states once broken apart
RETURNS: N/A
-----------------------------------------------------------------------------------*/
	{
	string temp;
	for (int i=0; i < acceptString.size(); i++)
		{
		//make sure it's a valid character
		if (!isspace(acceptString[i]) && !isdigit(acceptString[i]))
			{
			cout << "Invalid State Number. " << endl;
			cout << "Offending character: " << acceptString[i] << endl;
			cout << "Must be a digit. Aborthing." << endl;
			exit(0);
			}
		//we know it's valid so find out if it's a digit	
		if (isdigit(acceptString[i]))
			temp += acceptString[i];
		else
			{
			acc.push_back(atoi(temp.c_str()));
			temp="";
			}
			
		}
		if (temp != "")
		acc.push_back(atoi(temp.c_str()));
		
	}
	
State initializeTrans(string transition)
/*---------------------------------------------------------------------------------
DESCRIPTION: Breaks the transition string and stores in a State
PARAMETERS: 
	transition: the string containing the transition. 
RETURNS: a state containing the transitions for the current state on a given input
-----------------------------------------------------------------------------------*/
	{
	State transState;
	string temp;
	int start=0,
		found,
		state;
		
		//get current state
		found=transition.find(" ");
		temp=transition.substr(start, (found-start));
		if (checkNum(temp)== -1)
			{
			cout << "Invalid Origin State" << endl;
			cout << "Offending Transition " << transition << endl;
			cout << "Aborting" << endl;
			exit(0);
			}
		transState.setState(atoi(temp.c_str()));
		
		//get transition character
		start = found + 1;
		found=transition.find(" ", start);
		temp=transition.substr(start, (found-start));
		if (temp.size() !=1 || !islower(temp[0]))
			{
			cout << "Invalid Transition Character" << endl;
			cout << "Offending Transition " << transition << endl;
			cout << "Aborting" << endl;
			exit(0);
			}
		transState.setSymb(temp[0]);
		
		// get the state the transition takes you to
		start = found + 1;
		temp= transition.substr(start, (transition.size()-1-start));
		if (checkNum(temp)== -1)
			{
			cout << "Invalid Ending State" << endl;
			cout << "Offending Transition " << transition << endl;
			cout << "Aborting" << endl;
			exit(0);
			}
		transState.setMove(atoi(temp.c_str()));
		
		return transState;
		
	}
	
void validateStrings(const vector<char> &alph, string input)
/*---------------------------------------------------------------------------------
DESCRIPTION: Ensures that ever string contains valid characters
PARAMETERS: 
	 alph: the input alphabet
	 input: the string to be checked
RETURNS: N/A
-----------------------------------------------------------------------------------*/
	{
	bool inAlpha;
	for(int i=0; i< input.size(); i++)
		{
		inAlpha=false;
		if(!islower(input[i]))
			{
			cout << "Invalid input string. Must contain a-z only." << endl;
			cout << "Offending string: " << input << endl;
			cout << "Aborting" << endl;
			exit(0);
			}
		for (int j=0; j<alph.size(); j++)
			{
			if(input[i]==alph[j])
				{
				inAlpha=true;
				break;
				}
			}
		if (inAlpha==false)
			{
			cout << "Character not in alphabet." << endl;
			cout << "Offending character: " << input[i] << endl;
			cout << "Aborting" << endl;
			exit(0);
			}
		}
	}
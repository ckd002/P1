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
	int getState() const {return st;}
	char getSymb() const {return symbol;}
	int getMove() const {return moveto;}
	
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
int transverseAutomaton(string toCheck, const vector<State> &trans);
bool checkAccepting(int toCheck, const vector<int> &states);
//**********************************************

/***********************************************
Main function
************************************************/
int main(int argc, char **argv)
{
vector< vector<int> > 	minTable;	//the minimization table
vector<char> alphabet;				//to hold the input alphabet
vector<int> acceptingStates;		//to hold the accepting states.
vector<int> minCol;					//to hold a column of a minimization table
vector<State> transitions; 			//Transition table
vector<string> testStrings;			//the strings to be checked in the machine
int NumStates,						//to hold the number of states in the FA
	NumAcc,							//the number of accepting states there are
	NumTrans,						//number of characters in the alphabet
	finalState,						//numerical representation of which state you are currently in.
	check1,							//to see if a state is accepting
	check2;							//to see if another state is accepting
string alpha,						//the string of the alphabet
	   stateNum,					//string version of number of states
	   acceptingNum,				//string version of number of accepting
	   accepting,					//the string containing the accepting states
	   temp;						//the temporary string for reading in
bool accepted;						//whether the string was accepted or not
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
ofl.open(argv[3]);
if(!ofl)
	{
	cout << "Bad output file: " << argv[1] <<endl;
	exit(0);
	}
getline(ifl, temp);
while (ifl.good() && temp !="~")
	{
	cout << temp << endl;
	validateStrings(alphabet,temp);
	finalState=transverseAutomaton(temp, transitions);
	accepted= checkAccepting(finalState, acceptingStates);
	if (accepted==true)
		ofl << temp << " PASS" << endl;
	else
		ofl << temp << " FAIL" << endl;
	getline(ifl, temp);
	}
ofl.close();
ofl.clear();
ifl.close();
ifl.clear();
cout << "Strings checked and results output to file" << endl;

if(argc==4)
	cout << "Program finished. Open output file to see results" << endl;
else
	{
	cout << "Prepairing to minimize" << endl;
	//initialize table
	for(int i=0; i<(NumStates-1); i++)
		{
		for (int j= (NumStates-1-i); j>0; j--)
			{
			minCol.push_back(-1);
			}
		minTable.push_back(minCol);
		minCol.clear();
		}
	//Put inital 1's into table	
	for(int i=0; i<minTable.size(); i++)
		for(int j=0; j<minTable[i].size(); j++)
			{
			check1=checkAccepting(i, acceptingStates);
			check2=checkAccepting((j+i+1), acceptingStates);
			if(check1 != check2)
				minTable[i][j]=1;
			}
			
	for(int i=0; i<minTable.size(); i++)
		{
		for(int j=0; j<minTable[i].size(); j++)
			{
			cout << minTable[i][j];
			}
		cout << endl;
		}
	}
	
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
		temp= transition.substr(start, (transition.size()-start));
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
	
int transverseAutomaton(string toCheck, const vector<State> &trans)
	{
	int state=0;
	State currentState;
	for(int i=0; i<toCheck.size(); i++)
		{
		currentState.setState(state);
		currentState.setSymb(toCheck[i]);
		for(int j=0; j<trans.size(); j++)
			{
			if(currentState.getState()==trans[j].getState() && 
				currentState.getSymb()==trans[j].getSymb())
				{
				state=trans[j].getMove();
				break;
				}
			}
		}
		return state;
	}

bool checkAccepting(int toCheck, const vector<int> &states)
	{
	for(int p=0; p < states.size(); p++)
		if (toCheck == states[p])
			return true;
	return false;
	}
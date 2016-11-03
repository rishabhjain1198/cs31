#include<iostream>
#include<cctype>
#include<string>

using namespace std;

//*************************************
//  isValidUppercaseStateCode
//*************************************

// Return true if the argument is a two-uppercase-letter state code, or
// false otherwise.

bool isValidUppercaseStateCode(string stateCode)
{
    const string codes =
	"AL.AK.AZ.AR.CA.CO.CT.DE.DC.FL.GA.HI.ID.IL.IN.IA.KS."
        "KY.LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC."
        "ND.OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
    return (stateCode.size() == 2  &&
	    stateCode.find('.') == string::npos  &&  // no '.' in stateCode
        codes.find(stateCode) != string::npos);  // match found
}

//Function to check correctness of state code

bool hasCorrectSyntax(string pollData)
{
	
    if(pollData == "") 		//trivial case of empty poll string
	    return true;

    char tester = 's'; //this tells us about type of data which is supposed to
                        //be there. s for state, v for votes and p for party
    bool breaker = true;
    int i = 0;		//start at the beginning of the string
    while(i < pollData.length())        //go through the polldata string
    {
        switch(tester)      //test what the value of the tester is
        {
            case 's':   //checking if tester value is state
            if(i+1 < pollData.length())     //checking if another character is available
            {
                string temp = ""; temp+=toupper(pollData[i]); temp+=toupper(pollData[i+1]);
                tester = 'v'; i += 2;		//switching all characters to upper case so they match
                if(!isValidUppercaseStateCode(temp))	//checking if the state code is valid
                {
                    breaker = false;
                    i = pollData.length();
                }
            }
            else        //if another character is not available, state code is invalid
            {
                breaker = false;        //sets return boolean variable to false
                i = pollData.length();      //ends the loop
            }
            break;      //this is needed it doesn't overflow to other cases

            case 'v':   //case to check for numbers (votes)
            if(isdigit(pollData[i])) //checks if the string character is a number
            {
                if(i+1 < pollData.length())     //checks if another character is available
		tester = 'p';
		if(i+1 < pollData.length())
                {
                    if(isdigit(pollData[i+1]))      //checks if the next character is a digit in case of double digit votes
                    {
                        i+=2;               //increases string counter by 2, since double digit votes
                    }
		    else
			    i++;
                }
                else
                {
                    i++;        //increases string counter by 1, since single digit votes
                }
            }
            else        //if it's not a number, then string is invalid
            {
                breaker = false;
                i = pollData.length();
            }
            break;

            case 'p':       //checks if tester value is party
            if(isalpha(pollData[i]))        //checks if party value is a letter
            {
                i++;        //increases string counter by 1, since party is single letter
		tester = 's';
            }
            else    //if party value is not a letter, it is invalid
            {
	    	    breaker = false;
		i = pollData.length();
            }
            break;
        }
    }

    if(tester != 's')		//checking if tester is set to state in case party letter was missing at the end and the party switch block wasn't executed
	    breaker = false;
   	
    return breaker;
}

int countVotes(string pollData, char party, int& voteCount)
{
    if(!hasCorrectSyntax(pollData))     //checks if the string has correct syntax
        return 1;
    if(!isalpha(party))         //checks if the party variable inputted is a valid alphabetical letter
        return 3;

    if(pollData == "")		//trivial case of poll string being empty (0 state forecasts)
	return 0;

    int i = 2;	//start checking from votes

    int votes = 0;

    while(i < pollData.length())       //loop to traverse the string
    {
        if(isdigit(pollData[i+1]))	//check if votes are double digits
        {
            int tempVotes;
            string temp = ""; temp+= pollData[i]; temp+= pollData[i+1]; tempVotes = stoi(temp); //adds the numbers to  temporary string and converts its contents to an integer
            if(tempVotes == 0)  //returns 2 in case votes are 0
            {
                return 2;
            }
            else if(toupper(pollData[i+2]) == toupper(party))	//check if votes belong to desirable party
            {
                votes += tempVotes;
            }
            i+=5;   //increases string counter by 5 because state forecast had double digit votes
        }
        else
        {
            if(pollData[i] == '0')  //if votes are 0, returns 2
	    {
		return 2;
	    }
            else
            {
                if(toupper(pollData[i+1]) == toupper(party))		//in case party is desirable, add votes to variable
                {
                    string temp = ""; temp+=pollData[i]; votes += stoi(temp);		
                }
                i+=4;   //increases string counter by 4, because state forecast had single digit votes
            }
        }
    }
    
    voteCount = votes;      //sets the voteCount variable to votes
    return 0;
}

/*
//main function for testing out the program
int main()
{
    int voteCount = 0; char party; string inpit; cin>>inpit; cout<<"party: "; cin>>party; 
    countVotes(inpit, tolower(party), voteCount);
    cout<<voteCount;
    return 0;
}
*/

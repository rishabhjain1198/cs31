#include<iostream>
#include<cassert>
#include<cstring>

using namespace std;

const int MAX_WORD_LENGTH = 20;

int normalizeRules(char word1[][MAX_WORD_LENGTH+1], char word2[][MAX_WORD_LENGTH+1], int distance[], int nRules)
{
	char tempWord1 [nRules] [MAX_WORD_LENGTH+1];
	char tempWord2 [nRules] [MAX_WORD_LENGTH+1];
	int tempDistance [nRules];
	int tempRules = 0;

	for(int i = 0; i < nRules; i++)
	{
		if(distance[i] < 0) continue;	//if distance is negative, completely skip this rule
		
		//checking each character for isalpha and converting to lower case
		int j = 0; bool doot = 1;	//variable doot keeps track of whether all characters are alpha or not
		while(word1[i][j] != '\0')
		{
			if(!isalpha(word1[i][j]))
			{
				doot = 0;
			}
			else word1[i][j] = tolower(word1[i][j]);	//convert it to lowercase in case it is alphabet
			j++;
		}
		if(j == 0) doot = 0;	//accounting for empty word1

		j = 0;	//time to go through the second word

		while(word2[i][j] != '\0')
		{
			if(!isalpha(word2[i][j]))
			{
				doot = 0;
			}
			else word2[i][j] = tolower(word2[i][j]);
			j++;
		}

		if(j == 0) doot = 0;	//accounting for empty word2
		
		if(!doot) continue;	//if a non alphabet character was found, skip this rule
			

		//compare rule to every other previous rule

		doot = 0;	//this now keeps track of whether a matching rule was found or not

		for(int j = 0; j < tempRules; j++)	//this is used to match all the pre checked rules with the current one in question
		{
			if(doot) continue;

			if(!strcmp(word1[i], tempWord1[j]))
			{
				if(!strcmp(word2[i], tempWord2[j]))	//if both word1 and word2 of 2 rules respectively are same, it replaces the distance with the higher value and sets doot to 1, ie. match was found
				{
					doot = 1;
					if(tempDistance[j] < distance[i])
					{
						tempDistance[j] = distance[i];
					}
				}
			}

			if(!doot){	//this makes sure that in case match was already found, code block doesn't cause malfunction

			if(!strcmp(word2[i], tempWord1[j]))	//this is used to check if word1 and word2 of rule a and b respectively are same, and if word2 and word1 of rule b and a respectively are same
			{
				if(!strcmp(word1[i], tempWord2[j]))
				{
					doot = 1;
					if(tempDistance[j] < distance[i])
					{			
						tempDistance[j] = distance[i];	//sets distance to maximum value
					}
				}
			}
			}
		}

		if(doot) continue;

		//no matching rule found and distance is non-negative, the rule in question is valid and we must add it to our temporary set of rules

		strcpy(tempWord1[tempRules], word1[i]);
		strcpy(tempWord2[tempRules], word2[i]);
		tempDistance[tempRules] = distance[i];
		tempRules++;


	}

	//copy everything from tempRules to proper
	
	for(int i = 0; i < tempRules; i++)
	{
		strcpy(word1[i], tempWord1[i]);
		strcpy(word2[i], tempWord2[i]);
		distance [i] = tempDistance[i];
	}

	return tempRules;
}

int calculateSatisfaction(const char word1[][MAX_WORD_LENGTH+1], const char word2[][MAX_WORD_LENGTH+1], const int distance [], int nRules, const char document[])
{
	if(nRules < 1) return 0;
	int ruleCount = 0;	//keeps track of how many rules are satisfied
	for(int i = 0; i < nRules; i++)
	{
		if(distance[i] == 0) continue;
		int j = 0; int letterTracker = 0; char tempWord [200]; int ruler = 0; bool doot = 1; int distanceTracker = 0;
		while(document[j] != '\0' && doot)	//doot keeps track of whether rule is satisfied or not
		{
			if(isalpha(document[j]))	//this checks whether current character is alpha, and adds it to temporary word if it is
			{
				tempWord[letterTracker] = tolower(document[j]);	//converts alphabets to lower case
				letterTracker++;	//this keeps track of the size of the word
			}

			else if(letterTracker && document[j] == ' ')	//check if a temp word actually exists
			{
		                tempWord[letterTracker] = '\0'; letterTracker = 0;	//distance tracker variable keeps track of how many words have gone by since last matched word
				distanceTracker++;	//ruler variable keeps track of which words were matched, ie. if word 1 was matched with word2 of a rule, then ruler = 2, if word 1 is matched with word 1 of a rule, then ruler = 1
				if(!strcmp(tempWord, word1[i]))
				{
					if(ruler == 2 && distanceTracker <= distance[i])
					{
						ruleCount++; doot = 0;	//rule was satisfied
					}
			
					else
					{
						ruler = 1;	//words were matched but rule wasn't satisfied yet
						distanceTracker = 0;	//distance will now be counted from this word
					}
				}

				if(!strcmp(tempWord, word2[i]) && distanceTracker!=0)	//symmetrical to previous code block, but for the case which word is matched with word2 of rule, but there is check of distanceTracker to make sure that in case the rule has 2 identical words, the rule isn't satisfied by a single match
				{
					if(ruler == 1 && distanceTracker <= distance[i])
					{
						ruleCount++; doot = 0;
					}	
					else
					{
						ruler = 2;
						distanceTracker = 0;
					}
				}	
			}
			j++;
		}

       if(!doot) continue;	//rule already satisfied, move on

        tempWord[letterTracker] = '\0'; letterTracker = 0;	//this code block is identical to the previous one, but this required for the absolute last word of the document
        distanceTracker++;
        if(!strcmp(tempWord, word1[i]))
        {
            if(ruler == 2 && distanceTracker <= distance[i])
            {
                ruleCount++;
            }
	    else
		    ruler = 1;		//this is required for the case that distance is 0 and both words of the rule are same, and match is found
        }
        
        if(!strcmp(tempWord, word2[i]) && distanceTracker != 0)
        {
            if(ruler == 1 && distanceTracker <= distance[i])
            {
                ruleCount++;
            }	
        }
	}

	return ruleCount;	//return the total number of rules satisfied
}

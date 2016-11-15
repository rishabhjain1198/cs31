#include<iostream>
#include<cassert>

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
		if(distance[i] < 0) break;
		
		//checking each character for isalpha and converting to lower case
		int j = 0; bool doot = 1;
		while(word1[i][j] != '\0')
		{
			if(!isalpha(word1[i][j]))
			{
				doot = 0;
			}
			word1[i][j] = tolower(word1[i][j]);
			j++;
		}

		j = 0;

		while(word2[i][j] != '\0')
		{
			if(!isalpha(word2[i][j]))
			{
				doot = 0;
			}
			word2[i][j] = tolower(word2[i][j]);
			j++;
		}
		
		if(!doot) break;
			
		//compare rule to every other previous rule

		doot = 0;

		for(int j = 0; j < tempRules; j++)
		{
			if(!strcmp(word1[i], tempWord1[j]))
			{
				if(!strcmp(word2[i], tempWord2[j]))
				{
					doot = 1;
					if(tempDistance[j] > distance[i])
					{
						tempDistance[j] = distance[i];
					}
				}
			}

			if(!doot){
			if(!strcmp(word2[i], tempWord1[j]))
			{
				if(!strcmp(word1[i], tempWord2[j]))
				{
					doot = 1;
					if(tempDistance[j] > distance[i])
					{
						tempDistance[j] = distance[i];
					}
				}
			}
			}
		}

		if(doot) break;

		//no matching rule found, add to tempRules

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
	int ruleCount = 0;
	for(int i = 0; i < nRules; i++)
	{
		int j = 0; int letterTracker = 0; char tempWord [MAX_WORD_LENGTH+1]; int ruler = 0; bool doot = 1; int distanceTracker = 0;
		while(document[j] != '\0' && doot)
		{
			if(isalpha(document[j]) || document[j] == '-' || document[j] == '\'')
			{
				tempWord[letterTracker] = tolower(document[j]);
				letterTracker++;
			}

			else if((document[j] == ' ' || document[j] == '!' || document[j] == ',' || document[j] == '.' ) && letterTracker)
			{
                tempWord[letterTracker] = '\0'; letterTracker = 0;
                //cout<<tempWord<<endl;
				distanceTracker++;	
				if(!strcmp(tempWord, word1[i]))
				{
					if(ruler == 2 && distanceTracker <= distance[i])
					{
						ruleCount++; doot = 0;
					}
			
					else
					{
						ruler = 1;
						distanceTracker = 0;
					}
				}

				if(!strcmp(tempWord, word2[i]))
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
        
        tempWord[letterTracker] = '\0'; letterTracker = 0;
        distanceTracker++;
        if(!strcmp(tempWord, word1[i]))
        {
            if(ruler == 2 && distanceTracker <= distance[i])
            {
                ruleCount++; doot = 0;
            }
            
            else
            {
                ruler = 1;
                distanceTracker = 0;
            }
        }
        
        if(!strcmp(tempWord, word2[i]))
        {
            if(ruler == 1 && distanceTracker <= distance[i] && distanceTracker != 0)
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

	return ruleCount;
}
int main()
{
    const int TEST1_NRULES = 4;
    char test1w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
        "mad",       "deranged", "nefarious", "have"
    };
    char test1w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
        "scientist", "robot",    "plot",      "mad"
    };
    char test2w1[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
        "a", "b", "c", "d"
    };
    char test2w2[TEST1_NRULES][MAX_WORD_LENGTH+1] = {
        "e", "f", "g", "h"
    };
    int test1dist[TEST1_NRULES] = {
        2,           4,          1,           13
    };
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "The mad UCLA scientist unleashed a deranged evil giant robot.") == 2);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "The mad UCLA scientist unleashed    a deranged robot.") == 2);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "**** 2016 ****") == 0);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "  That plot: NEFARIOUS!") == 1);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "deranged deranged robot deranged robot robot") == 1);
    assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
                                 "That scientist said two mad scientists suffer from deranged-robot fever.") == 0);
    assert(normalizeRules(test1w1, test1w2, test1dist, 4) == 4);
    assert(calculateSatisfaction(test2w1, test2w2, test1dist, TEST1_NRULES, "a e b juf af f oppopoppopo establih pos d h") == 3);
    cout << "All tests succeeded" << endl;
}

/*
int main()
{
	char word1 [] [MAX_WORD_LENGTH+1] = {"e"};
	char word2 [] [MAX_WORD_LENGTH+1] = {"f"};
	int distance [] = {-1};
	int nRules = 1;
	normalizeRules(word1, word2, distance ,nRules);

	return 0;
}
*/

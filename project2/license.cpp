#include<iostream>

using namespace std;

int main()
{ 	
	//TAKING INPUT BEGINS
	string id, country; int revenue;
	cout<<"Identification: "; getline(cin, id);
	cout<<"Expected revenue (in millions): "; cin>>revenue; cin.ignore(10000, '\n');
	cout<<"Country: "; getline(cin, country); 
	//TAKING INPUT COMPLETE
	
	cout<<"---"<<endl;	//separating input and output on runtime screen


	//TAKING CARE OF TRIVIAL CASES
	if(id == "")
	{ 
		cout<<"You must enter a property identification.";
		return 0;	//terminate program in case of empty id
	}
	if(revenue<0)
	{ 
		cout<<"The expected revenue must be nonnegative.";
		return 0;	//terminate program in case of negative revenue
	}
	if(country == "")
	{ 
		cout<<"You must enter a country";
		return 0;
	}


	//STARTING CALCULATION OF LICENSE FEE
	double fee = 0.00;
	if(revenue<=20)		//taking case of revenue being less than 20
	{ 
		fee = 0.181 * revenue;
	}
	else if(revenue<=50)	//if program is checking this statement, we already know revenue is over 20. Hence, we just need to check if it's not over 50
	{ 
		fee = 0.181 * 20;    //calculating fee for first 20 million
		if(country == "UAE" || country == "Turkey")
		{ 
			fee += 0.217 * (revenue - 20);	//caluclatiing fee for remaining revenue in case country is UAE or Turkey
		}
		else
		{ 
			fee += 0.203 * (revenue - 20);	//calculating fee for all other countries
		}
	}
	else	//in case revenue is over 50
	{ 
		fee = 0.181 * 20;	//calculating fee for first 20 

		if(country == "UAE" || country == "Turkey")
		{ 
			fee += 0.217 * 30;	//caluclating fee for next 30, in case country is UAE or Turkey
		}
		else
		{ 
			fee += 0.203 * 30;	//caluclating fee for next 30, for all other countries
		}
		
		fee += 0.230 * (revenue - 50);	//caluclating fee for the remaining revenue
	}
	//LICENSE FEE CALCULATION COMPLETE
	
	cout.setf(ios::fixed);
	cout.precision(3);
	cout<<"The license fee for "<<id<<" is $"<<fee<<" million.";

	return 0;
}

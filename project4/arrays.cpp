#include<iostream>

using namespace std;

void showIt(string[], int);	//function for showing contents of string

int appendToAll(string a[], int n, string value)
{
	if(n<0)			//trivial case of n being negative
		return -1;
	for(int i = 0; i < n; i++)
	{
		a[i] += value;		//running through array and appending
	}
	return n;
}

int lookup(const string a[], int n, string target)
{
	if(n<0) return -1;
	for(int i = 0; i < n; i++)
	{
		if(a[i] == target)	//can also use if(!a[i].compare(targer));
		{
			return i;	//if match found, returns position
		}
	}

	return -1;	//if match not found, returns -1
}

int positionOfMax(const string a[], int n)
{
	if(n<0) return -1;

	int maxPos = 0;

	for(int i = 0; i < n; i++)
	{
		if(a[i] > a[maxPos])	//finds the maximum value in the array, and only chooses the one which occurs first in case of multiple same maximum values
		{
			maxPos = i;	
		}	
	}	

	return maxPos;	
}

int rotateLeft(string a[], int n, int pos)
{
	if(n<0) return -1;

	string temp = a[pos];

	for(int i = pos; i < n-1; i++)
	{
		a[i] = a[i+1];		//move values of array to the left, starting from pos
	}

	a[n-1] = temp;		//set the last element as the original pos element

	return pos;
}

int countRuns(const string a[], int n)
{
	if(n<0) return -1;

	int consec = 0; int i = 0;

	while(i < n)
	{
		while(a[i] == a[i+1])	//skips all the consecutive identical elements
			i++;
		i++;
		consec++;	//increase the answer by 1
	}

	return consec;
}

int flip(string a [], int n)
{
	if(n<0) return -1;

	for(int i = 0; i < n/2; i++)	//swaps the elements at the opposite ends of the array, and makes the interval shorter progressively until middle of array is reached
	{
		string temp = a[i];
		a[i] = a[n-1-i];
		a[n-1-i] = temp;
	}

	return n;
}

int differ(const string a[], int n1, const string a2[], int n2)
{
	if(n1<0) return -1;
	else if(n2<0) return -1;
	int i = 0, j = 0;
	while(i < n1 && j < n2)
	{
		if(a[i] != a2[j])	//is mismatch found return position
			return i;
		i++; j++;
	}
	if(n1<n2) return n1;
	else return n2;
}

int subsequence(const string a1[], int n1, const string a2[], int n2)
{
	if(n1<0) return -1;
	else if(n2<0) return -1;
	for(int i = 0; i < n1; i++)
	{
		int j = i; int p = 0;
		while(a1[j] == a2[p] && j<n1 && p<n2)
		{
			j++; p++;
		}
		if(p==n2)
			return i;
	}
	return -1;
}

int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
	if(n1 < 0) return -1;
	else if(n2 < 0) return -1;

	for(int i = 0; i < n1; i++)
	{
		for(int j = 0l; j < n2; j++)
		{
			if(a1[i] == a2[j])
				return i;
		}
	}

	return -1;
}

int separate(string a[], int n, string separator)
{
	if(n<0) return -1;

	int i = 0; int starter = 0;
	while(i < n)
	{
		if(a[i] < separator)
		{
			string temp = a[starter];
			a[starter] = a[i];
			a[i] = temp;
			starter++;
		}
		i++;
	}

	return starter;
}

void showIt(string a[], int n)
{
	cout<<endl;
	for(int i = 0;  i < n; i++)
	{
		cout<<a[i]<<endl;
	}
}

int main()
{
	string tester [] = {
		"gary", "hillary", "jill", "donald"
	};
	cout<<separate(tester, 4, "hillary");
	showIt(tester, 4);

	return 0;
}	

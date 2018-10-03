#include<iostream>
 
using namespace std;


void CounterSort(int a[], int n, int r, int lower)
{
	int i, j = 0, counter[r] = {0};	
	
	for(i=0; i<n; i++)
		counter[a[i]-lower]++;
 
	i=0;

	while(i < r)
	{
		flag:
		a[j] = lower+i;
		j++;
		counter[i]--;
 

		if(counter[i] > 0)
		goto flag;
 
		i++;
	}
}

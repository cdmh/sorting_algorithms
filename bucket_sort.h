//BUCKET SORT
 // Best case performance            Ω(n+k)
// Average case performace          θ(n+k)
// Worst Average case performace    O(n^2)
// Wost case space complexity       O(n)
 //https://en.wikipedia.org/wiki/Bucket_sort
 // The following header files are a must in order to execute the following code
#include <iostream> 
#include <algorithm> 
#include <vector> 
using namespace std; 
  
// Function to sort arr[] of size n using bucket sort 
template <typename T>
void bucketSort(T arr[], int n) 
{ 
    // 1) Create n empty buckets 
    vector<T> b[n]; 
     
    // 2) Put array elements in different buckets 
    for (int i=0; i<n; i++) 
    { 
       int bi = n*arr[i]; // Index in bucket 
       b[bi].push_back(arr[i]); 
    } 
  
    // 3) Sort individual buckets 
    for (int i=0; i<n; i++) 
       sort(b[i].begin(), b[i].end()); 
  
    // 4) Concatenate all buckets into arr[] 
    int index = 0; 
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < b[i].size(); j++) 
          arr[index++] = b[i][j]; 
    
    // 5)output result
    for(int i=0;i<n;i++)
      cout<<T[i];
} 

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

template <class T, class Comparator>
void Merge (vector<T>& toBeSorted, int l, int r, int m, Comparator& comp){
	  vector<T> temp;
	  // trace through the two subarrays, copying into a temporary one
	  int i = l, j = m+1, k = 0;
	  while (i <= m || j <= r)  // at least one subarray contains another element
	       {
	         if (i <= m && (j > r || comp(toBeSorted[i],toBeSorted[j]))){   //toBeSorted[i] <= toBeSorted[j]
        		temp.push_back(toBeSorted[i]); 
            	i ++; 
            	k ++; 
	         }
	         else { 
	         	temp.push_back(toBeSorted[j]);  
	         	j ++; 
	         	k ++; }
	       }
	  // now copy over the merged array into the original
	  for (k = 0; k < (int)temp.size(); k ++){
	  	toBeSorted[k+l] = temp[k];
	  }
}

template <class T, class Comparator>
void recursion(vector<T>& toBeSorted, int l, int r, Comparator& comp){
    if (l<r) { //l<r
        int m = floor((l+r)/2);
        recursion(toBeSorted, l, m, comp);
        recursion(toBeSorted, m+1, r, comp);
        Merge(toBeSorted, l, r, m, comp); // this is a separate function given below
    }
}


template <class T, class Comparator>
void mergeSort (vector<T>& toBeSorted, Comparator& comp){
    /* you may choose the type of list/vector/deque that fits your design best */
    int l = 0;
    int r = (int)toBeSorted.size()-1;
	recursion(toBeSorted, l, r, comp);
}
	





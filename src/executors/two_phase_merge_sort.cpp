// #include<bits/stdc++.h>
// using namespace std;

// #define ll long long
// const int MOD = 1e9+7;
// const int MAX = 1e5+5;

// int temp[MAX];

// void merging(vector<int> &a, int l, int m, int r){

// 	memset(temp, 0, sizeof(temp));

// 	int i=l;
// 	int j=m+1;
// 	int k=l;

// 	while(i<=m && j<=r){

// 		if(a[i]>a[j])
// 			temp[k++]=a[j++];
// 		else
// 			temp[k++]=a[i++];
// 	}

// 	while(i<=m)
// 		temp[k++]=a[i++];

// 	while(j<=r)
// 		temp[k++]=a[j++];

// 	for(int i=l;i<=r;i++)
// 		a[i]=temp[i];
// }

// void mSort(vector<int> &a, int l, int r) { 
// 	if(l>=r)
// 		return;

//     int m = l + (r - l)/2; 
//     //left half
//     mSort(a, l, m); 
//     //right half
//     mSort(a, m + 1, r);
//     //merging step
//     merging(a, l, m, r);  
// }
// /**
//  * @brief      Sorting individual blocks
//  */
// void phase1(){

// }

// // struct comp{
// // 	bool operator()(const MinHeapNode lhs, const MinHeapNode rhs) const{
// // 		return lhs.element > rhs.element;
// // 	}
// // };


// /**
//  * @brief      Merging M blocks to form sorted file
//  */
// void phase2(){
// 	// pair<int, int> harr[k];
// 	// priority_queue<pair<int, int>, vector<pair<int, int>>, comp> pq;

// }

// void twoPhaseMergeSort(){
// 	//get details of the table
// 	phase1();
// 	phase2();
// }

// // int main(){
// // 	vector<int> a = {10, 5, 3, 5, 2, 1, 3, 11, 11, 100, -1, 3, 5, 4};
// // 	mSort(a, 0, a.size()-1);
// // 	for(int i=0; i<a.size(); i++){
// // 		cout<<a[i]<<" ";
// // 	}
// // 	cout<<endl;
// // 	return 0;
// // }

// /**
//  *http://www.mathcs.emory.edu/~cheung/Courses/554/Syllabus/4-query-exec/2-pass=TPMMS.html
//  *
//  * 20000 records ---> 1 file       
//  * Record size = 100 bytes
//  * Block size = 1024 bytes
//  * No. of records in one block = 1024/100 =102
//  * No. of blocks = 20000/102 = 196.07 = 197
//  * 
//  * let m = 20
//  * 
//  * pass 1
//  * --------
//  * 
//  * chunks = 197/20 = 10
//  * sort each chunk individually.
//  * 
//  * The maximum size of a file that can be sorted by the TPMMS algorithm is:
//  * 
//  * B(R)  ≤  M(M − 1)      
//  * 
//  * 
//  * 
//  * 
//  * pass 2
//  * --------
//  * 
//  * m-1 input buffers   19 -> row pointer
//  * 1 output buffer		1 -> output file
//  * 
//  * 
//  * sorted chunks k <= m-1 = 19
//  * 
//  * 
//  * 
//  */

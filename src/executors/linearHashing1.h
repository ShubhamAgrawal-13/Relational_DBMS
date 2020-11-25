

// const int BUCKET_SIZE=5;
// const double THRESHOLD_INSERT=0.75; //75%
// const double THRESHOLD_DELETE=0.25; 

// class Record{
// public:
// 	int key;
// 	int page_no;
// 	int row_no;
// };

// // INDEX ON c FROM A USING HASH BUCKETS 2

// class BucketBlock{
// public:
// 	vector<Record> records;
// 	BucketBlock *overflow;

// 	BucketBlock(){
// 		records.clear();
// 		overflow = NULL;
// 	}

// 	void addRecord(Record r){
// 		if(records.size() < BUCKET_SIZE){
// 			records.push_back(r);
// 		}
// 		else{
// 			if(overflow != NULL){
// 				overflow->addRecord(r);
// 			}
// 			else{
// 				overflow = new BucketBlock();
// 				overflow->addRecord(r);
// 			}
// 		}
// 	}

// 	vector<Record> findRecords(int searchKey){
// 		vector<Record> res;
// 		BucketBlock *temp = this;
//         while(temp) {
//             for(int i = 0; i < temp->records.size(); i++) {
//             	if(temp->records[i].key == searchKey)
//                 	res.push_back(temp->records[i]);
//             }
//             temp = temp->overflow;
//         }
//         return res;
// 	}

// 	void removeRecords(int searchKey, int &r){
// 		vector<Record> v;
// 		for(int i = 0; i < records.size(); i++) {
// 			if(records[i].key!=searchKey){
//             	v.push_back(records[i]);
// 			}
// 			else{
// 				r--;
// 			}
//         }

//         records.clear();
//         BucketBlock* of = overflow;
//         overflow = NULL;
//         // cout<<"check 1\n";

// 	    while(of) {
// 			for(int i = 0; i < of->records.size(); i++) {
// 				if(of->records[i].key!=searchKey){
// 	            	v.push_back(of->records[i]);
// 				}
// 				else{
// 					r--;
// 				}
// 	        }
// 	        of->records.clear();
// 	        BucketBlock* temp = of->overflow;
//             delete of;
//             of = temp;
//         }

//         for(int i=0; i<v.size(); i++){
//         	addRecord(v[i]);
//         }

// 	}

// 	void print(){
// 		BucketBlock *temp = this;
//         while(temp) {
//             for(int i = 0; i < temp->records.size(); i++) {
//                 cout << temp->records[i].key << ' ';
//             }
//             cout <<" | ";
//             temp = temp->overflow;
//         }
//         cout << '\n';
// 	}

// };

// class HashTable{
// public:

// 	int r, n, i_bit;
// 	vector<BucketBlock*> buckets;

// 	HashTable() {
//         r = 0;
//         i_bit = 1;
//         n = 2;
//         buckets.push_back(new BucketBlock());
//         buckets.push_back(new BucketBlock());
//     }

//     HashTable(int n_buckets) {
//         r = 0;
//         i_bit = ceil(log2(n_buckets));
//         n = n_buckets;
//         for(int i=0; i<n; i++){
//         	buckets.push_back(new BucketBlock());
//         }
//     }


// 	int my_hash(int k){
// 		int mod = (1 << i_bit);
//         return k % mod;
// 	}

//     // Suffix-1(x)   returns the suffix of x after removing the leading 1 bit in x
//     //  Suffix-1( x ) =  x − 2i    where i = ⌊ log2( n ) ⌋
//     int my_hash_1(int k) {
//         int mod = (1 << (i_bit-1));
//         return k % mod;
//     }

//     int hash(int k){
//     	return k;
//     }


// 	void insertRecord(Record record){
// 		int key = hash(record.key);
// 		int m = my_hash(key);

// 		// cout<<"m="<<m<<"\n";

// 		if(m <= n-1){ // Real Bucket
// 			buckets[m]->addRecord(record);
// 		}
// 		else{ //Ghost Bucket
// 			// m = n - (1<<(i_bit-1));
// 			m = my_hash_1(key);
// 			// cout<<"m="<<m<<"\n";
// 			buckets[m]->addRecord(record);
// 		}

// 		r++;
// 		double occupancy = (r / (1.0 * n * BUCKET_SIZE));
// 		// cout<<"Occupancy : "<<occupancy<<"\n";
// 		if(occupancy > THRESHOLD_INSERT){
// 			// cout<<occupancy<<"Buckets increased by 1\n";
// 			buckets.push_back(new BucketBlock());
// 			n++;

// 			i_bit = ceil(log2(n));
// 			int k = n - 1 - (1<<(i_bit-1));
// 			// cout<<"n="<<n<<", i_bit="<<i_bit<<", k="<<k<<"\n";

// 			vector<Record> v;
// 			for(int i = 0; i < buckets[k]->records.size(); i++) {
// 	            v.push_back(buckets[k]->records[i]);
// 	        }

// 	        buckets[k]->records.clear();
// 	        BucketBlock* of = buckets[k]->overflow;
// 	        buckets[k]->overflow = NULL;
// 	        // cout<<"check 1\n";

// 		    while(of) {
// 				for(int i = 0; i < of->records.size(); i++) {
// 		            v.push_back(of->records[i]);
// 		        }
// 		        of->records.clear();
// 		        BucketBlock* temp = of->overflow;
// 	            delete of;
// 	            of = temp;
// 	        }
// 	        // cout<<"check 2\n";

// 	        for(int i = 0; i < v.size(); i++) {
// 	        	// cout<<"v[i].key = "<<v[i].key<<"\n";
// 	        	// cout<<"my_hash v[i].key = "<<my_hash(v[i].key)<<"\n";
//                 buckets[my_hash(v[i].key)]->addRecord(v[i]);
//             }
//             // cout<<"check 3\n";
// 		}
// 	}

// 	vector<Record> find(int searchKey){
// 		int key = hash(searchKey);
// 		int m = my_hash(key);

// 		// cout<<"m="<<m<<"\n";

// 		if(m <= n-1){ // Real Bucket
// 			return buckets[m]->findRecords(searchKey);
// 		}
// 		//Ghost Bucket
// 		// m = n - (1<<(i_bit-1));
// 		m = my_hash_1(key);
// 		// cout<<"m="<<m<<"\n";
// 		return buckets[m]->findRecords(searchKey);
// 	}

// 	void deleteRecord(int searchKey){
// 		int key = hash(searchKey);
// 		int m = my_hash(key);

// 		// cout<<"m="<<m<<", searchKey="<<searchKey<<"\n";

// 		if(m <= n-1){ // Real Bucket
// 			buckets[m]->removeRecords(searchKey, r);
// 		}
// 		else{ //Ghost Bucket
// 			// m = n - (1<<(i_bit-1));
// 			m = my_hash_1(key);
// 			// cout<<"m="<<m<<"\n";
// 			buckets[m]->removeRecords(searchKey, r);
// 		}
// 		// cout<<"r="<<r<<"\n";
// 		double occupancy = (r / (1.0 * n * BUCKET_SIZE));
// 		// cout<<"Occupancy : "<<occupancy<<"\n";
// 		if(occupancy < THRESHOLD_DELETE && n>2){
// 			// cout<<occupancy<<"\nBuckets decreased by 1\n";
// 			int k = n-1;
// 			n--;
// 			// cout<<"check1\n";
// 			i_bit = ceil(log2(n));
// 			// int k = n - 1 - (1<<(i_bit-1));
// 			// cout<<"n="<<n<<", i_bit="<<i_bit<<", k="<<k<<"\n";

// 			vector<Record> v;
// 			for(int i = 0; i < buckets[k]->records.size(); i++) {
// 	            v.push_back(buckets[k]->records[i]);
// 	        }

// 	        buckets[k]->records.clear();
// 	        BucketBlock* of = buckets[k]->overflow;
// 	        buckets[k]->overflow = NULL;
// 	        // cout<<"check 1\n";

// 		    while(of) {
// 				for(int i = 0; i < of->records.size(); i++) {
// 		            v.push_back(of->records[i]);
// 		        }
// 		        of->records.clear();
// 		        BucketBlock* temp = of->overflow;
// 	            delete of;
// 	            of = temp;
// 	        }

// 	        delete buckets[k];
// 			buckets.pop_back();

// 			// int k = n - (1<<(i_bit-1));
// 			for(int i = 0; i < v.size(); i++) {
// 	        	// cout<<"v[i].key = "<<v[i].key<<"\n";
// 	        	// cout<<"my_hash v[i].key = "<<my_hash(v[i].key)<<"\n";
// 	        	int kk = my_hash(v[i].key);
// 	        	if(kk<=n-1){
// 	        		buckets[kk]->addRecord(v[i]);
// 	        	}
// 	        	else{
// 	        		kk = my_hash_1(v[i].key);
// 	        		buckets[kk]->addRecord(v[i]);
// 	        	}
//             }
// 		}
// 	}

// 	void print(){
// 		for(int i = 0; i<buckets.size(); i++) {
//             cout << i << " -> ";
//             buckets[i]->print();
//         }
//         cout << '\n';
// 	}
// };

// // int main(){
// // 	HashTable ht;
// // 	// int k=1;
// // 	// while(k){
// // 	// 	int op;
// // 	// 	cin>>op;
// // 	// 	if(op==4){
// // 	// 		cout<<"Exited : "<<"\n";
// // 	// 		break;
// // 	// 	}

// // 	// 	int k;
// // 	// 	cin>>k;

// // 	// 	if(op==1){
// // 	// 		cout<<"Inserted : "<<k<<"\n";
// // 	// 		ht.insertRecord({k, 0});
// // 	// 	}
// // 	// 	else if(op==2){
// // 	// 		cout<<"find : "<<k<<"\n";
// // 	// 		vector<Record> res = ht.find(k);
// // 	// 		for(auto i : res){
// // 	// 			cout<<i.key <<" "<<i.value<<"\n";
// // 	// 		}
// // 	// 	}
// // 	// 	else if(op==3){
// // 	// 		cout<<"Deleted all : "<<k<<"\n";
// // 	// 		ht.deleteRecord(k);
// // 	// 	}
		
// // 	// 	ht.print();
// // 	// 	cout<<"------\n";
// // 	// 	cout<<"r="<<ht.r<<"\n";
// // 	// 	cout<<"n="<<ht.n<<"\n";
// // 	// }
	
// // 	for(int i=0; i<100000; i++){
// // 		ht.insertRecord({i, 0});
// // 	}
// // 	ht.print();
// // 	cout<<"------\n";
// // 	cout<<"r="<<ht.r<<"\n";
// // 	cout<<"n="<<ht.n<<"\n";

// // 	for(int i=0; i<100000; i++){
// // 		ht.deleteRecord(i);
// // 		// ht.print();
// // 	}
// // 	ht.print();
// // 	cout<<"------\n";
// // 	cout<<"r="<<ht.r<<"\n";
// // 	cout<<"n="<<ht.n<<"\n";
// // 	// ht.print();
// // 	cout<<"Bye, Thank you !!!\n";
// // 	// vector<Record> res = ht.find(43);
// // 	// for(auto i : res){
// // 	// 	cout<<i.key <<" "<<i.value<<"\n";
// // 	// }

// // 	return 0;
// // }



const int BUCKET_SIZE=5;
const double THRESHOLD_INSERT=0.75; //75%
const double THRESHOLD_DELETE=0.25; 

class Record{
public:
	int key;
	int page_no;
	int row_no;

	// Record(){
	// 	key=0;
	// 	page_no=0;
	// 	row_no=0;
	// }
};

// INDEX ON a FROM A USING HASH BUCKETS 2

class BucketBlock{
public:
	map<int, vector<Record>> records; 

	BucketBlock(){
		records.clear();
	}

	void addRecord(Record value){
		int key = value.key;
		if(records.find(key) != records.end()){
			records[key].push_back(value);
		}
		else{
			records[key]=vector<Record>(1, value);
		}
	}

	vector<Record> findRecords(int searchKey){
		vector<Record> res;
        if(records.find(searchKey) != records.end()){
        	return records[searchKey];
		}
		return res;
	}

	void removeRecords(int searchKey, int &r){
		if(records.find(searchKey) != records.end()){
			r -= records[searchKey].size();
			records[searchKey].clear();
			records.erase(searchKey);
		}
	}

	void removeRecord(Record searchRecord, int &r){
		if(records.find(searchRecord.key) != records.end()){
			vector<Record> v;  
			int list_size = records[searchRecord.key].size();
			for(int i=0; i<list_size; i++){
				if(records[searchRecord.key][i].page_no!=searchRecord.page_no || records[searchRecord.key][i].row_no!=searchRecord.row_no){
					v.push_back(records[searchRecord.key][i]);
				} 
			}
			if(v.size()==list_size){
				return;
			}
			r-=1;
			if(v.size()==0){
				records.erase(searchRecord.key);
				return;
			}
			records[searchRecord.key] = v;
		}
	}

	void print(){
		int cc=0;
		for(auto i : records){
			cout << i.first << ' ';
			cc++;
            if(cc%BUCKET_SIZE==0)
            	cout <<" | ";
		}
		
        cout << '\n';
	}

};

class HashTable{
public:

	int r, n, i_bit;
	vector<BucketBlock*> buckets;

	HashTable() {
        r = 0;
        i_bit = 1;
        n = 2;
        buckets.push_back(new BucketBlock());
        buckets.push_back(new BucketBlock());
    }

    HashTable(int n_buckets) {
        r = 0;
        i_bit = ceil(log2(n_buckets));
        n = n_buckets;
        for(int i=0; i<n; i++){
        	buckets.push_back(new BucketBlock());
        }
    }


	int my_hash(int k){
		int mod = (1 << i_bit);
        return k % mod;
	}

    // Suffix-1(x)   returns the suffix of x after removing the leading 1 bit in x
    //  Suffix-1( x ) =  x − 2i    where i = ⌊ log2( n ) ⌋
    int my_hash_1(int k) {
        int mod = (1 << (i_bit-1));
        return k % mod;
    }

    int hash(int k){
    	return k;
    }


	void insertRecord(Record record){
		int key = hash(record.key);
		int m = my_hash(key);

		// cout<<"m="<<m<<"\n";
		if(m <= n-1){ // Real Bucket
			buckets[m]->addRecord(record);
		}
		else{ //Ghost Bucket
			// m = n - (1<<(i_bit-1));
			m = my_hash_1(key);
			// cout<<"m="<<m<<"\n";
			buckets[m]->addRecord(record);
		}

		r++;

		double occupancy = (r / (1.0 * n * BUCKET_SIZE));
		// cout<<"Occupancy : "<<occupancy<<"\n";

		if(occupancy > THRESHOLD_INSERT){
			// cout<<"Buckets increased by 1\n";
			buckets.push_back(new BucketBlock());
			n++;

			i_bit = ceil(log2(n));
			int k = n - 1 - (1<<(i_bit-1));
			// cout<<"Occupancy : "<<occupancy<<"n="<<n<<", i_bit="<<i_bit<<", k="<<k<<"\n";
	        // cout<<"check 2\n";

			map<int, vector<Record>> m_temp = buckets[k]->records;
			buckets[k]->records.clear();
	        for(auto i : m_temp) {
	        	for(auto j : i.second){
	        		// cout<<i.first<<endl;
                	buckets[my_hash(i.first)]->addRecord(j);
                }
            }
            // cout<<"check 3\n";
		}
	}

	vector<Record> find(int searchKey){
		int key = hash(searchKey);
		int m = my_hash(key);

		// cout<<"m="<<m<<"\n";

		if(m <= n-1){ // Real Bucket
			return buckets[m]->findRecords(searchKey);
		}
		//Ghost Bucket
		// m = n - (1<<(i_bit-1));
		m = my_hash_1(key);
		return buckets[m]->findRecords(searchKey);
	}

	void deleteRecord(Record record){
		int searchKey = record.key;
		int key = hash(searchKey);
		int m = my_hash(key);

		// cout<<"m="<<m<<", searchKey="<<searchKey<<"\n";
		if(m <= n-1){ // Real Bucket
			buckets[m]->removeRecord(record, r);
		}
		else{ //Ghost Bucket
			// m = n - (1<<(i_bit-1));
			m = my_hash_1(key);
			// cout<<"m="<<m<<"\n";
			buckets[m]->removeRecord(record, r);
		}

		// cout<<"r="<<r<<"\n";
		double occupancy = (r / (1.0 * n * BUCKET_SIZE));
		// cout<<"Occupancy : "<<occupancy<<"\n";
		if(occupancy < THRESHOLD_DELETE && n>2){
			// cout<<"\nBuckets decreased by 1\n";
			int k = n-1;
			n--;
			// cout<<"check1\n";
			i_bit = ceil(log2(n));
			// int k = n - 1 - (1<<(i_bit-1));
			// cout<<"n="<<n<<", i_bit="<<i_bit<<", k="<<k<<"\n";
			// cout<<"Occupancy : "<<occupancy<<"n="<<n<<", i_bit="<<i_bit<<", k="<<k<<"\n";
			map<int, vector<Record>> m_temp = buckets[k]->records;
			buckets[k]->records.clear();
			delete buckets[k];
			buckets.pop_back();

	        for(auto i : m_temp) {
	        	for(auto j : i.second){
	        		int kk = my_hash(i.first);
		        	if(kk<=n-1){
		        		buckets[kk]->addRecord(j);
		        	}
		        	else{
		        		kk = my_hash_1(i.first);
		        		buckets[kk]->addRecord(j);
		        	}
	        	}
            }

			// int k = n - (1<<(i_bit-1));
		}
	}

	void deleteRecords(int searchKey){
		int key = hash(searchKey);
		int m = my_hash(key);

		// cout<<"m="<<m<<", searchKey="<<searchKey<<"\n";
		if(m <= n-1){ // Real Bucket
			buckets[m]->removeRecords(searchKey, r);
		}
		else{ //Ghost Bucket
			// m = n - (1<<(i_bit-1));
			m = my_hash_1(key);
			// cout<<"m="<<m<<"\n";
			buckets[m]->removeRecords(searchKey, r);
		}

		// cout<<"r="<<r<<"\n";
		double occupancy = (r / (1.0 * n * BUCKET_SIZE));
		// cout<<"Occupancy : "<<occupancy<<"\n";
		if(occupancy < THRESHOLD_DELETE && n>2){
			// cout<<"\nBuckets decreased by 1\n";
			int k = n-1;
			n--;
			// cout<<"check1\n";
			i_bit = ceil(log2(n));
			// int k = n - 1 - (1<<(i_bit-1));
			// cout<<"n="<<n<<", i_bit="<<i_bit<<", k="<<k<<"\n";
			// cout<<"Occupancy : "<<occupancy<<"n="<<n<<", i_bit="<<i_bit<<", k="<<k<<"\n";
			map<int, vector<Record>> m_temp = buckets[k]->records;
			buckets[k]->records.clear();
			delete buckets[k];
			buckets.pop_back();

	        for(auto i : m_temp) {
	        	for(auto j : i.second){
	        		int kk = my_hash(i.first);
		        	if(kk<=n-1){
		        		buckets[kk]->addRecord(j);
		        	}
		        	else{
		        		kk = my_hash_1(i.first);
		        		buckets[kk]->addRecord(j);
		        	}
	        	}
            }

			// int k = n - (1<<(i_bit-1));
		}
	}

	void print(){
		for(int i = 0; i<buckets.size(); i++) {
            cout << i << " -> ";
            buckets[i]->print();
        }
        cout << '\n';
	}

};

// int main(){
// 	HashTable ht;
// 	// int k=1;
// 	// while(k){
// 	// 	int op;
// 	// 	cin>>op;
// 	// 	if(op==4){
// 	// 		cout<<"Exited : "<<"\n";
// 	// 		break;
// 	// 	}

// 	// 	int k;
// 	// 	cin>>k;

// 	// 	if(op==1){
// 	// 		cout<<"Inserted : "<<k<<"\n";
// 	// 		ht.insertRecord({k, 0});
// 	// 	}
// 	// 	else if(op==2){
// 	// 		cout<<"find : "<<k<<"\n";
// 	// 		vector<Record> res = ht.find(k);
// 	// 		for(auto i : res){
// 	// 			cout<<i.key <<" "<<i.value<<"\n";
// 	// 		}
// 	// 	}
// 	// 	else if(op==3){
// 	// 		cout<<"Deleted all : "<<k<<"\n";
// 	// 		ht.deleteRecord(k);
// 	// 	}
		
// 	// 	ht.print();
// 	// 	cout<<"------\n";
// 	// 	cout<<"r="<<ht.r<<"\n";
// 	// 	cout<<"n="<<ht.n<<"\n";
// 	// }
	
// 	for(int i=0; i<10000; i++){
// 		ht.insertRecord({i, 0, i});
// 	}
// 	ht.print();
// 	cout<<"r="<<ht.r<<"\n";
// 	cout<<"n="<<ht.n<<"\n";
// 	cout<<"------\n\n";

// 	for(int i=0; i<10000; i++){
// 		ht.deleteRecord({i, 0, i});
// 		// ht.print();
// 	}

// 	ht.print();
// 	cout<<"r="<<ht.r<<"\n";
// 	cout<<"n="<<ht.n<<"\n";
// 	cout<<"------\n\n";

	
// 	vector<Record> res = ht.find(43);
// 	for(auto i : res){
// 		cout<<i.key <<" "<<i.page_no<<" "<<i.row_no<<"\n";
// 	}

// 	cout<<"Bye, Thank you !!!\n";
// 	return 0;
// }


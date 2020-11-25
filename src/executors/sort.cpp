#include"global.h"

const int SORT_MAX = 1e5+5;

int temp[SORT_MAX];

// bool isDigit(char ch) { 
//     if (ch >= '0' && ch <= '9') 
//         return true; 
//     return false; 
// } 

/**
 * @brief File contains method to process SORT commands.
 * 
 * syntax:
 * R <- SORT relation_name BY column_name IN sorting_order
 * 
 * sorting_order = ASC | DESC 
 */
bool syntacticParseSORT(){
    logger.log("syntacticParseSORT");
    if(tokenizedQuery.size()== 10){
        if(tokenizedQuery[4] != "BY" || tokenizedQuery[6] != "IN" || tokenizedQuery[8] != "BUFFER"){
            cout<<"SYNTAX ERROR"<<endl;
            return false;
        }
        parsedQuery.queryType = SORT;
        parsedQuery.sortResultRelationName = tokenizedQuery[0];
        parsedQuery.sortColumnName = tokenizedQuery[5];
        parsedQuery.sortRelationName = tokenizedQuery[3];
        string sortingStrateg = tokenizedQuery[7];
        parsedQuery.sortBuffer = tokenizedQuery[9];
        string str = tokenizedQuery[9];
        for (int i = 0; i < str.length(); i++) { 
            if (!(str[i] >= '0' && str[i] <= '9')) { 
                cout<<"SYNTAX ERROR : Buffer should be an Integer"<<endl;
                return false;
            } 
        }

        if(sortingStrateg == "ASC")
            parsedQuery.sortingStrategy = ASC;
        else if(sortingStrateg == "DESC")
            parsedQuery.sortingStrategy = DESC;
        else{
            cout<<"SYNTAX ERROR : sorting strategy should be only ASC or DESC"<<endl;
            return false;
        }
        return true;
    }
    else if(tokenizedQuery.size()== 8){
        if(tokenizedQuery[4] != "BY" || tokenizedQuery[6] != "IN"){
            cout<<"SYNTAX ERROR"<<endl;
            return false;
        }
        parsedQuery.queryType = SORT;
        parsedQuery.sortResultRelationName = tokenizedQuery[0];
        parsedQuery.sortColumnName = tokenizedQuery[5];
        parsedQuery.sortRelationName = tokenizedQuery[3];
        string sortingStrateg = tokenizedQuery[7];
        if(sortingStrateg == "ASC")
            parsedQuery.sortingStrategy = ASC;
        else if(sortingStrateg == "DESC")
            parsedQuery.sortingStrategy = DESC;
        else{
            cout<<"SYNTAX ERROR"<<endl;
            return false;
        }
        return true;
    }
    cout<<"SYNTAX ERROR"<<endl;
    return false;
}

bool semanticParseSORT(){
    logger.log("semanticParseSORT");

    if(tableCatalogue.isTable(parsedQuery.sortResultRelationName)){
        cout<<"SEMANTIC ERROR: Resultant relation already exists"<<endl;
        return false;
    }

    if(!tableCatalogue.isTable(parsedQuery.sortRelationName)){
        cout<<"SEMANTIC ERROR: Relation doesn't exist " << parsedQuery.sortRelationName <<endl;
        return false;
    }

    if(!tableCatalogue.isColumnFromTable(parsedQuery.sortColumnName, parsedQuery.sortRelationName)){
        cout<<"SEMANTIC ERROR: Column doesn't exist in relation"<<endl;
        return false;
    }

    return true;
}


/**
 * @brief      Merging Function
 *
 * @param      a     { array which is to be sorted}
 * @param[in]  l     { lower index }
 * @param[in]  m     { mid index }
 * @param[in]  r     { higher index }
 */

void merging(vector<int> &a, int l, int m, int r){

    memset(temp, 0, sizeof(temp));

    int i=l;
    int j=m+1;
    int k=l;

    while(i<=m && j<=r){

        if(a[i]>a[j])
            temp[k++]=a[j++];
        else
            temp[k++]=a[i++];
    }

    while(i<=m)
        temp[k++]=a[i++];

    while(j<=r)
        temp[k++]=a[j++];

    for(int i=l;i<=r;i++)
        a[i]=temp[i];
}

/**
 * @brief      General Merge Sort Algorithm
 *
 * @param      a     { array which is to be sorted}
 * @param[in]  l     { lower index }
 * @param[in]  r     { higher index }
 */
void mSort(vector<int> &a, int l, int r) { 
    if(l>=r)
        return;

    int m = l + (r - l)/2; 
    //left half
    mSort(a, l, m); 
    //right half
    mSort(a, m + 1, r);
    //merging step
    merging(a, l, m, r);  
}

/**
 * @brief     Node for heap which is used to merge files.
 */
struct HeapNode{
    int element;
    string row;
    int filePointer;
};

/**
 * @brief      Comparator for Min Heap
 */
struct comp_min{
    bool operator()(const HeapNode &a, const HeapNode &b) const{
        return a.element > b.element;
    }
};

/**
 * @brief      Comparator for Max Heap
 */
struct comp_max{
    bool operator()(const HeapNode &a, const HeapNode &b) const{
        return a.element < b.element;
    }
};


/**
 * @brief      Merge Files input files to a output file
 *             
 *             As k is Buffers available.
 *             So, k-1 buffers for input files
 *             and 1 buffer for output. 
 *
 * @param[in]  input_filenames  The input filenames
 * @param[in]  output_filename  The output filename
 * @param[in]  columnIndex      The column index
 * @param[in]  flag             The flag (ASC / DESC)
 */
void mergeFiles(vector<string> input_filenames, string output_filename, int columnIndex, int flag){
    
    int k = input_filenames.size();
    ifstream inputPointers[k];
    ofstream outputPointer;

    for(int i=0; i<k; i++){
        inputPointers[i].open(input_filenames[i]); 
    }

    outputPointer.open(output_filename);

    if(flag==0){ //min

        priority_queue<HeapNode, vector<HeapNode>, comp_min> min_heap;

        for(int i=0; i<k; i++){            
            string line, word;
            getline(inputPointers[i], line);
            stringstream s(line);
            vector<int> temp;
            while(getline(s, word, ' ')){
                temp.push_back(stoi(word));
            }
            min_heap.push({ temp[columnIndex], line, i});
        }
        
        while(!min_heap.empty()){

            HeapNode node = min_heap.top();
            min_heap.pop();

            outputPointer << node.row <<"\n";

            string line, word;
            if(getline(inputPointers[node.filePointer], line)){
                stringstream s(line);
                vector<int> temp;
                while(getline(s, word, ' ')){
                    temp.push_back(stoi(word));
                }
                min_heap.push({ temp[columnIndex], line, node.filePointer});
            }
        }

    }
    else{  //max
        priority_queue<HeapNode, vector<HeapNode>, comp_max> max_heap;
        for(int i=0; i<k; i++){            
            string line, word;
            getline(inputPointers[i], line);
            stringstream s(line);
            vector<int> temp;
            while(getline(s, word, ' ')){
                temp.push_back(stoi(word));
            }
            max_heap.push({ temp[columnIndex], line, i});
        }
        
        while(!max_heap.empty()){

            HeapNode node = max_heap.top();
            max_heap.pop();

            outputPointer << node.row <<"\n";

            string line, word;
            if(getline(inputPointers[node.filePointer], line)){
                stringstream s(line);
                vector<int> temp;
                while(getline(s, word, ' ')){
                    temp.push_back(stoi(word));
                }
                max_heap.push({ temp[columnIndex], line, node.filePointer});
            }
        }
    }

    // Closing all file pointers

    for(int i=0; i<k; i++){
        inputPointers[i].close(); 
    }

    outputPointer.close();
}

/**
 * @brief      Phase 2 of merge sort i.e., merging
 *
 * @param[in]  tableName    The table name
 * @param[in]  blockCount   The block count
 * @param[in]  columnIndex  The column index
 * @param[in]  flag         The flag (ASC / DESC)
 * @param[in]  k            Buffer Size
 * 
 * @return     { return file output filename }
 */
string phase2(string tableName, int blockCount, int columnIndex, int flag, int k){

    //buffers = k ( k-1 inputs and 1 output)
    vector<string> files;
    for(int i=0; i<blockCount; i++){
        string pageName = "../data/temp/" + tableName + "_temp_Page" + to_string(i);
        files.push_back(pageName);
    }   

    int pass=1;
    while(files.size()>1){
        vector<string> files_output;
        // int passes = ceil(blockCount/1.0*k);
        int out=0; // number of output files
        for(int i=0; i<files.size();){
            int pp=0;
            vector<string> inputs;
            while(pp<k-1 && i<files.size()){
                inputs.push_back(files[i]);
                pp++;
                i++;
            }
            string output_filename = "../data/temp/" + tableName + "_temp_Page_" + to_string(pass) + "_" + to_string(out);
            mergeFiles(inputs, output_filename, columnIndex, flag);
            files_output.push_back(output_filename);
            out++;
        }

        //delete all files in files vector
        for(string file : files){

            if (remove(file.c_str())){
                logger.log("SORT :: deleting temporary files : ERROR");
                cout << "SORT :: deleting temporary files : ERROR";
            }
            else{
                logger.log("SORT :: deleting temporary files : SUCCESS");
            }

        }
        
        files.clear();

        for(string s : files_output){
            files.push_back(s);
        }
        files_output.clear();
        pass++;
    }

    return files[0];
}


/**
 * @brief      Sort by Decreasing order
 *
 * @param[in]  a     { parameter_description }
 * @param[in]  b     { parameter_description }
 *
 * @return     { description_of_the_return_value }
 */
bool sortByDesc(const pair<int,int> &a, const pair<int,int> &b){
    return a.first>b.first;
}

/**
 * @brief      Sorting one block
 *
 * @param[in]  pageName     The page name
 * @param[in]  columnIndex  The column index
 * @param[in]  columnIndex  The flag ASC or DESC
 */
void sortPage(string pageName, int columnIndex, int flag){
    string line, word;
    ifstream fin(pageName, ios::in);
    vector<string> rows;
    vector<pair<int, int>> res; // columnIndex, original index
    int i=0;
    while(getline(fin, line)){
        stringstream s(line);
        vector<int> temp;
        while(getline(s, word, ' ')){
            temp.push_back(stoi(word));
        }
        //cout<<temp[columnIndex]<<endl;
        res.push_back({temp[columnIndex], i});
        i++;
        rows.push_back(line);
        //cout<<line<<endl;
    }
    fin.close();
    //cout<<"Read Successfullly\n";

    if(flag==0)
        sort(res.begin(), res.end());
    else
        sort(res.begin(), res.end(), sortByDesc);

    ofstream fout(pageName, ios::trunc);
    for (int r = 0; r < res.size(); r++){
        auto p = res[r].second;
        //cout<< rows[p] <<"\n";
        fout << rows[p] <<"\n";
    }

    fout.close();
    //cout<<"Written Successfullly\n";
}



/**
 * @brief      Doing phase 1 of 2 phase merge sort.
 *             Sorting individual pages.
 *
 * @param[in]  tableName    The table name
 * @param[in]  blockCount   The block count
 * @param[in]  columnIndex  The column index
 * @param[in]  columnIndex  The flag ASC or DESC
 */
void phase1(string tableName, int blockCount, int columnIndex, int flag){
    for(int i=0; i<blockCount; i++){
        //string pageName = "../data/temp/" + tableName + "_Page" + to_string(i);
        // cout<<pageName << endl;
        string pageName1 = "../data/temp/" + tableName + "_Page" + to_string(i);
        string pageName2 = "../data/temp/" + tableName + "_temp_Page" + to_string(i);
        string cmd = "cp " + pageName1 + " " + pageName2;
        system(cmd.c_str());
        sortPage(pageName2, columnIndex, flag);
        // cout<<"Sorted : "<<pageName<<endl;
    }
}

// AA <- SORT S BY a IN ASC
// AA <- SORT S BY a IN DESC
void executeSORT(){
    logger.log("executeSORT");
    Table* table = tableCatalogue.getTable(parsedQuery.sortRelationName);

    int k=5; //Initially Default buffers available

    if(parsedQuery.sortBuffer != ""){
        k = stoi(parsedQuery.sortBuffer);
    }

    // cout<< parsedQuery.sortRelationName <<endl;
    // cout<< parsedQuery.sortResultRelationName <<endl;
    // cout<< parsedQuery.sortColumnName <<endl;
    // cout<< parsedQuery.sortingStrategy <<endl;

    cout<<"Block Count : " << table->blockCount<<endl;
    int columnIndex=0;

    vector<string> cols = table->columns;
    for(int i=0; i<cols.size(); i++){
        //cout<< i <<" : "<<cols[i]<<endl;
        if(cols[i] == parsedQuery.sortColumnName){
            columnIndex = i;
            break;
        }
    }
    cout<< "Column Index : " << columnIndex << endl;
    phase1(parsedQuery.sortRelationName, (int)table->blockCount, columnIndex, parsedQuery.sortingStrategy);
    string output_filename = phase2(parsedQuery.sortRelationName, (int)table->blockCount, columnIndex, parsedQuery.sortingStrategy, k);
    
    //blockify the large file
    
    Table *resultantTable = new Table(parsedQuery.sortResultRelationName, table->columns);
    
    string line, word;
    ifstream fin(output_filename, ios::in);

    while(getline(fin, line)){
        stringstream s(line);
        vector<int> row;
        while(getline(s, word, ' ')){
            row.push_back(stoi(word));
        }
        resultantTable->writeRow<int>(row);
    }

    fin.close();

    resultantTable->blockify();
    tableCatalogue.insertTable(resultantTable);

    //delete temporary output file
    if (remove(output_filename.c_str())){
        logger.log("SORT :: deleting temporary files : ERROR");
        cout << "SORT :: deleting temporary files : ERROR";
    }
    else{
        logger.log("SORT :: deleting temporary files : SUCCESS");
    }

    cout<<"Sorting Completed\n";
    return;
}

//added function 
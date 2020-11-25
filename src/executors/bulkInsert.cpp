#include "global.h"

/**
 * Syntax : BULK_INSERT <csv_file_name> INTO <table_name>
 * 
 * BULK_INSERT T INTO A
 */

/**
 * @details Checking syntax of BULK INSERT command
 * @return True if sytactically correct
 */
bool syntacticParseBULKINSERT()
{
    logger.log("syntacticParseBULKINSERT");

    if (tokenizedQuery.size() != 4  || tokenizedQuery[2] != "INTO")
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }

    parsedQuery.queryType = BULK_INSERT;
    parsedQuery.bulkInsertInputFile = tokenizedQuery[1];
    parsedQuery.bulkInsertRelationName = tokenizedQuery[3];

    return true;
}

std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}
 
std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}
 
std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    return ltrim(rtrim(str, chars), chars);
}


/**
 * @brief      Checking syntax of BULK INSERT command
 *
 * @return     true if semantically correct
 */
bool semanticParseBULKINSERT()
{
    logger.log("semanticParseBULKINSERT");
   
    if (!tableCatalogue.isTable(parsedQuery.bulkInsertRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }

    Table* table = tableCatalogue.getTable(parsedQuery.bulkInsertRelationName);
    string input_file = "../data/" + parsedQuery.bulkInsertInputFile + ".csv";
    ifstream fin(input_file, ios::in);
    string line, word;
    vector<string> row;
    getline(fin, line);

    stringstream s(line);
    while(getline(s, word, ',')){
        row.push_back(trim(word));
        // cout<<trim(word)<<" ";
    }
    fin.close();

    if(table->columnCount != row.size()){
    	cout<<"ColumnCount doesn't match "<<row.size()<<"\n";
    	return false;
    }
    
	for(int i=0; i<table->columns.size(); i++){
		if(row[i] != table->columns[i]){
            cout<<"Column Names didn't match \n";
            return false;
        }
	}    
	// cout<<"Check3\n";
    return true;
}




/**
 * @brief      Execute the BULK INSERT command
 */
void executeBULKINSERT()
{
	cout<<"executeBULKINSERT\n";
    logger.log("executeBULKINSERT");
    Table* table = tableCatalogue.getTable(parsedQuery.bulkInsertRelationName);

	if(table->indexingStrategy == BTREE){
        cout<<"BTREE\n";
    }
    else if(table->indexingStrategy == HASH){
        cout<<"HASHING\n";
    	// int columnIndex = table->getColumnIndex(table->indexedColumn);
    	// table->hashTable->insertRecord({inserting_row[columnIndex], 0, 0});
    	// table->hashTable->print();
        
        string input_file = "../data/" + parsedQuery.bulkInsertInputFile + ".csv";
        cout<<input_file<<endl;
        ifstream fin(input_file, ios::in);
        string line, word;
        vector<int> inserting_row;
        getline(fin, line);
        while(getline(fin, line)){
            stringstream s(line);
            // while(getline(s, word, ',')){
            //     cout<<word<<" ";
            // }
            // cout<<endl;
            while(getline(s, word, ',')){
                cout<<trim(word)<<" ";
                inserting_row.push_back(stoi(trim(word)));
            }
            cout<<endl;

            int columnIndex = table->getColumnIndex(table->indexedColumn);
            int page_no = table->validRecords.size()-1;
            int row_no = table->validRecords[page_no].size()-1;

            table->rowCount++;
            if(row_no+1 == table->maxRowsPerBlock){
                // create new page
                table->validRecords.push_back(vector<bool>());
                table->blockCount++;
                table->rowsPerBlockCount.emplace_back(1);
                page_no++;
                row_no=0;
            }
            else{
                row_no++;
                table->rowsPerBlockCount[table->rowsPerBlockCount.size()-1]++;
            }
            cout<<"Page no = "<<page_no<<endl;
            cout<<"Row no = "<<row_no<<endl;
            string temp = "";
            for(int i=0; i<inserting_row.size()-1; i++){
                temp += to_string(inserting_row[i]) + " ";
            }
            temp += to_string(inserting_row[inserting_row.size()-1]) + "\n";

            cout<<temp;

            string filename;
            filename = "../data/temp/" + parsedQuery.bulkInsertRelationName + "_Page" + to_string(page_no);
            ofstream fout(filename, ios::app);
            fout << temp;
            fout.close();

            table->validRecords[page_no].push_back(1);
            table->hashTable->insertRecord({inserting_row[columnIndex], page_no, row_no});
            inserting_row.clear();
        }
        table->hashTable->print();

        fin.close(); 
    }
    else{
        cout<<"NOTHING\n";
        string input_file = "../data/" + parsedQuery.bulkInsertInputFile + ".csv";
        cout<<input_file<<endl;
        ifstream fin(input_file, ios::in);
        string line, word;
        vector<int> inserting_row;
        getline(fin, line);
        while(getline(fin, line)){
            stringstream s(line);
            // while(getline(s, word, ',')){
            //     cout<<word<<" ";
            // }
            // cout<<endl;
            while(getline(s, word, ',')){
                cout<<trim(word)<<" ";
                inserting_row.push_back(stoi(trim(word)));
            }
            cout<<endl;

            int page_no = table->validRecords.size()-1;
            int row_no = table->validRecords[page_no].size()-1;

            table->rowCount++;
            if(row_no+1 == table->maxRowsPerBlock){
                // create new page
                table->validRecords.push_back(vector<bool>());
                table->blockCount++;
                table->rowsPerBlockCount.emplace_back(1);
                page_no++;
                row_no=0;
            }
            else{
                row_no++;
                table->rowsPerBlockCount[table->rowsPerBlockCount.size()-1]++;
            }
            cout<<"Page no = "<<page_no<<endl;
            cout<<"Row no = "<<row_no<<endl;
            string temp = "";
            for(int i=0; i<inserting_row.size()-1; i++){
                temp += to_string(inserting_row[i]) + " ";
            }
            temp += to_string(inserting_row[inserting_row.size()-1]) + "\n";

            cout<<temp;

            string filename;
            filename = "../data/temp/" + parsedQuery.bulkInsertRelationName + "_Page" + to_string(page_no);
            ofstream fout(filename, ios::app);
            fout << temp;
            fout.close();

            table->validRecords[page_no].push_back(1);
            inserting_row.clear();
        }

        fin.close();
    }
    
    return;
}


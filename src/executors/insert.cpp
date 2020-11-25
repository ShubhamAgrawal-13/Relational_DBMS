#include "global.h"

/**
 * INSERT Syntax:
 * 
 * INSERT​ ​ INTO​ <table_name> ​ VALUES​ <value1>[,<value2>]*
 * 
 * INSERT INTO A VALUES 41 42 43 44 45
 * INSERT INTO A VALUES 41,42,43,44,45
 * 
 */


/**
 * @details Checking syntax of INSERT command
 * @return True if sytactically correct
 */
bool syntacticParseINSERT()
{
    logger.log("syntacticParseINSERT");

    if (tokenizedQuery.size() < 5 || tokenizedQuery[1] != "INTO" || tokenizedQuery[3] != "VALUES")
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }

    parsedQuery.queryType = INSERT;
    parsedQuery.insertRelationName = tokenizedQuery[2];
    parsedQuery.insertRow.clear();

    // cout<<tokenizedQuery[4]<<endl;
    // stringstream ss(tokenizedQuery[4]);
    // string word;
    // while(getline(ss, word, ',')){
    //     parsedQuery.insertRow.push_back(word);
    // }
    // cout<<parsedQuery.insertRow.size()<<endl;

	for(int i=4; i<tokenizedQuery.size(); i++){
		parsedQuery.insertRow.push_back(tokenizedQuery[i]);
	}    

    return true;
}


/**
 * @brief      Checking syntax of INSERT command
 *
 * @return     true if semantically correct
 */
bool semanticParseINSERT()
{
    logger.log("semanticParseINSERT");
   
    if (!tableCatalogue.isTable(parsedQuery.insertRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }

    Table* table = tableCatalogue.getTable(parsedQuery.insertRelationName);

    // cout<<"Check1\n";
    if(table->columnCount != parsedQuery.insertRow.size()){
    	cout<<"ColumnCount doesn't match \n";
    	return false;
    }
    // cout<<"Check2\n";
	for(int i=0; i<parsedQuery.insertRow.size(); i++){
		for(char ch : parsedQuery.insertRow[i]){
			if (!(ch >= '0' && ch <= '9')) { 
                cout<<"SYNTAX ERROR : VALUES should be an Integer"<<endl;
                return false;
            } 
		}
	}    
	// cout<<"Check3\n";
    return true;
}


/**
 * @brief      Execute the INSERT command
 */
void executeINSERT()
{
	cout<<"executeINSERT\n";
    logger.log("executeINSERT");
    Table* table = tableCatalogue.getTable(parsedQuery.insertRelationName);

    vector<int> inserting_row;

    for(int i=0; i<parsedQuery.insertRow.size(); i++){
		inserting_row.push_back(stoi(parsedQuery.insertRow[i]));
	}

	if(table->indexingStrategy == BTREE){

    }
    else if(table->indexingStrategy == HASH){
    	int columnIndex = table->getColumnIndex(table->indexedColumn);
        int page_no = table->validRecords.size()-1;
        int row_no = table->validRecords[page_no].size()-1;

        // cout<<"Page no = "<<page_no<<endl;
        // cout<<"Row no = "<<row_no<<endl;
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
        filename = "../data/temp/" + parsedQuery.insertRelationName + "_Page" + to_string(page_no);
        ofstream fout(filename, ios::app);
        fout << temp;
        fout.close();

        table->validRecords[page_no].push_back(1);
        
    	table->hashTable->insertRecord({inserting_row[columnIndex], page_no, row_no});
    	table->hashTable->print();
    }
    else{
        
    }

    return;
}


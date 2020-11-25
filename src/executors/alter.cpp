#include "global.h"

/**
 * Syntax : ALTER TABLE <table_name> ADD|DELETE COLUMN <column_name>
 * 
 * ALTER TABLE A ADD COLUMN tr
 * ALTER TABLE A DELETE COLUMN a
 * 
 */

/**
 * @details Checking syntax of ALTER command
 * @return True if sytactically correct
 */
bool syntacticParseALTER()
{
    logger.log("syntacticParseALTER");

    if (tokenizedQuery.size() != 6  || tokenizedQuery[1] != "TABLE" || tokenizedQuery[4] != "COLUMN")
    {
        cout << "SYNTAX ERROR ALTER" << endl;
        return false;
    }

    parsedQuery.queryType = ALTER;
    parsedQuery.alterRelationName = tokenizedQuery[2];
    parsedQuery.alterColumnName = tokenizedQuery[5];

    if(tokenizedQuery[3]=="ADD"){
        parsedQuery.alterType=ADD;
    }
    else if(tokenizedQuery[3]=="DELETE"){
        parsedQuery.alterType=DEL;
    }
    else{
        return false;
    }

    return true;
}


/**
 * @brief      Checking syntax of ALTER command
 *
 * @return     true if semantically correct
 */
bool semanticParseALTER()
{
    logger.log("semanticParseALTER");
   
    if (!tableCatalogue.isTable(parsedQuery.alterRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }

    Table* table = tableCatalogue.getTable(parsedQuery.alterRelationName);
    
    if(parsedQuery.alterType == ADD){
        for(string s : table->columns){
            if(s==parsedQuery.alterColumnName){
                cout << "SEMANTIC ERROR: Column already exists" << endl;
                return false;
            }
        }
    }
    else{
        for(string s : table->columns){
            if(s==parsedQuery.alterColumnName){
                return true;
            }
        }
        cout << "SEMANTIC ERROR: Column doesn't exist" << endl;
        return false;
    }

    return true;
}


/**
 * @brief      Execute the ALTER command
 */
void executeALTER()
{
	// cout<<"executeALTER\n";
    logger.log("executeALTER");
    Table* table = tableCatalogue.getTable(parsedQuery.alterRelationName);
    vector<string> columns = table->columns;

    if(parsedQuery.alterType == ADD){
        columns.push_back(parsedQuery.alterColumnName);

        string filename = "../data/temp/"+ parsedQuery.alterRelationName+".csv";
        std::ifstream infile(filename);
        if(infile.good()){
            bufferManager.deleteFile(filename);
        }

        string temp_file = "temp_"+parsedQuery.alterRelationName;
        Table* resultantTable = new Table(temp_file, columns);
        Cursor cursor = table->getCursor();

        vector<int> row;
        for (int rowCounter = 0; rowCounter < table->rowCount; rowCounter++)
        {
            row = cursor.getNext();
            row.push_back(0);
            resultantTable->writeRow<int>(row);
        }
        tableCatalogue.deleteTable(parsedQuery.alterRelationName);

        string newname = "../data/temp/"+ parsedQuery.alterRelationName+".csv";
        string oldname = "../data/temp/"+ temp_file+".csv";
        if (rename(oldname.c_str(), newname.c_str()) != 0)
            perror("Error renaming file");
        else
            cout << "Success\n";
        resultantTable->sourceFileName = "../data/temp/" + parsedQuery.alterRelationName + ".csv";
        resultantTable->tableName = parsedQuery.alterRelationName;

        resultantTable->blockify();
        tableCatalogue.insertTable(resultantTable);

        cout<<resultantTable->rowCount<<endl;
        cout<<resultantTable->columnCount<<endl;
        cout<<resultantTable->blockCount<<endl;

        for(int i=0; i<resultantTable->validRecords.size(); i++){
            for(int j=0; j<resultantTable->validRecords[i].size(); j++){
                cout<<resultantTable->validRecords[i][j]<<" ";
            }
            cout<<endl;
        }

    }
    else{
        int alterColIndex = table->getColumnIndex(parsedQuery.alterColumnName);
        columns.erase(columns.begin()+alterColIndex);
        string filename = "../data/temp/"+ parsedQuery.alterRelationName+".csv";
        std::ifstream infile(filename);
        if(infile.good()){
            bufferManager.deleteFile(filename);
        }

        string temp_file = "temp_"+parsedQuery.alterRelationName;
        Table* resultantTable = new Table(temp_file, columns);
        Cursor cursor = table->getCursor();

        vector<int> row;
        for (int rowCounter = 0; rowCounter < table->rowCount; rowCounter++)
        {
            row = cursor.getNext();
            row.erase(row.begin()+alterColIndex);
            resultantTable->writeRow<int>(row);
        }
        tableCatalogue.deleteTable(parsedQuery.alterRelationName);

        string newname = "../data/temp/"+ parsedQuery.alterRelationName+".csv";
        string oldname = "../data/temp/"+ temp_file+".csv";
        if (rename(oldname.c_str(), newname.c_str()) != 0)
            perror("Error renaming file");
        else
            cout << "Success\n";
        resultantTable->sourceFileName = "../data/temp/" + parsedQuery.alterRelationName + ".csv";
        resultantTable->tableName = parsedQuery.alterRelationName;

        resultantTable->blockify();
        tableCatalogue.insertTable(resultantTable);
        cout<<resultantTable->rowCount<<endl;
        cout<<resultantTable->columnCount<<endl;
        cout<<resultantTable->blockCount<<endl;

        for(int i=0; i<resultantTable->validRecords.size(); i++){
            for(int j=0; j<resultantTable->validRecords[i].size(); j++){
                cout<<resultantTable->validRecords[i][j]<<" ";
            }
            cout<<endl;
        }

    }
    

    bufferManager.removeTablePagesFromPool(parsedQuery.alterRelationName);
    return;
}


#include "global.h"

/**
 * DELETE Syntax:
 * 
 * DELETE​ ​ FROM​ <table_name> ​ VALUES​ <value1>[,<value2>]*
 * DELETE​ FROM​ A VALUES 41,20,30,40,50
 * 
 */

bool syntacticParseDELETE()
{
    logger.log("syntacticParseDELETE");

    if (tokenizedQuery.size() < 5 || tokenizedQuery[1] != "FROM​" || tokenizedQuery[3] != "VALUES")
    {
        cout << "SYNTAX ERROR DELETE" << endl;
        return false;
    }

    parsedQuery.queryType = DELETE;
    parsedQuery.deleteRelationName = tokenizedQuery[2];

    parsedQuery.deleteRow.clear();
    for(int i=4; i<tokenizedQuery.size(); i++){
        parsedQuery.deleteRow.push_back(tokenizedQuery[i]);
    }    

    return true;
}

bool semanticParseDELETE()
{
    logger.log("semanticParseDELETE");
   
    if (!tableCatalogue.isTable(parsedQuery.deleteRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }

    Table* table = tableCatalogue.getTable(parsedQuery.deleteRelationName);

    if(table->columnCount != parsedQuery.deleteRow.size()){
        return false;
    }

    for(int i=0; i<parsedQuery.deleteRow.size(); i++){
        for(char ch : parsedQuery.deleteRow[i]){
            if (!(ch >= '0' && ch <= '9')) { 
                cout<<"SYNTAX ERROR : VALUES should be an Integer"<<endl;
                return false;
            } 
        }
    }    

    return true;
}

void executeDELETE()
{
    logger.log("executeDELETE");
    Table* table = tableCatalogue.getTable(parsedQuery.deleteRelationName);

    vector<int> deleting_row;

    for(int i=0; i<parsedQuery.deleteRow.size(); i++){
        deleting_row.push_back(stoi(parsedQuery.deleteRow[i]));
    }

    if(table->indexingStrategy == BTREE){

    }
    else if(table->indexingStrategy == HASH){
        int columnIndex = table->getColumnIndex(table->indexedColumn);
        vector<Record> res = table->hashTable->find(deleting_row[columnIndex]);
        Record ans;
        for(Record record : res){
            ans = record;
            break;
        }
        cout<<ans.page_no<<" "<<ans.row_no<<endl;
        table->validRecords[ans.page_no][ans.row_no] = 0;
        table->hashTable->deleteRecord(ans);
        table->hashTable->print();
    }
    else{

    }

    return;
}


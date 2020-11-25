#include "global.h"

/**
 * @brief 
 * SYNTAX: INDEX ON column_name FROM relation_name USING indexing_strategy
 * indexing_strategy: BTREE | HASH | NOTHING
 */


bool syntacticParseINDEX()
{
    logger.log("syntacticParseINDEX");

    if(tokenizedQuery.size()==9){
        if (tokenizedQuery[1] != "ON" || tokenizedQuery[3] != "FROM" || tokenizedQuery[5] != "USING" || (!(tokenizedQuery[7] == "FANOUT" || tokenizedQuery[7] == "BUCKETS")))
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }

        parsedQuery.queryType = INDEX;
        parsedQuery.indexColumnName = tokenizedQuery[2];
        parsedQuery.indexRelationName = tokenizedQuery[4];
        string indexingStrategy = tokenizedQuery[6];
        if (indexingStrategy == "BTREE")
            parsedQuery.indexingStrategy = BTREE;
        else if (indexingStrategy == "HASH")
            parsedQuery.indexingStrategy = HASH;
        else if (indexingStrategy == "NOTHING")
            parsedQuery.indexingStrategy = NOTHING;
        else
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
        
        string str = tokenizedQuery[8];
        for (int i = 0; i < str.length(); i++) { 
            if (!(str[i] >= '0' && str[i] <= '9')) { 
                cout<<"SYNTAX ERROR : BUCKETS or FANOUT should be an Integer"<<endl;
                return false;
            } 
        }

        parsedQuery.indexBuffer = str;

        return true;
    }
    else if(tokenizedQuery.size()==7){
        if (tokenizedQuery[1] != "ON" || tokenizedQuery[3] != "FROM" || tokenizedQuery[5] != "USING")
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }

        parsedQuery.queryType = INDEX;
        parsedQuery.indexColumnName = tokenizedQuery[2];
        parsedQuery.indexRelationName = tokenizedQuery[4];
        string indexingStrategy = tokenizedQuery[6];
        if (indexingStrategy == "BTREE")
            parsedQuery.indexingStrategy = BTREE;
        else if (indexingStrategy == "HASH")
            parsedQuery.indexingStrategy = HASH;
        else if (indexingStrategy == "NOTHING")
            parsedQuery.indexingStrategy = NOTHING;
        else
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
        return true;
    }
    
    cout<<"SYNTAX ERROR"<<endl;
    return false;
}

bool semanticParseINDEX()
{
    logger.log("semanticParseINDEX");
    if (!tableCatalogue.isTable(parsedQuery.indexRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }
    if (!tableCatalogue.isColumnFromTable(parsedQuery.indexColumnName, parsedQuery.indexRelationName))
    {
        cout << "SEMANTIC ERROR: Column doesn't exist in relation" << endl;
        return false;
    }
    Table* table = tableCatalogue.getTable(parsedQuery.indexRelationName);
    if(table->indexed){
        cout << "SEMANTIC ERROR: Table already indexed" << endl;
        return false;
    }
    return true;
}

void executeINDEX()
{
    logger.log("executeINDEX");
    if(parsedQuery.indexingStrategy == BTREE){

    }
    else if(parsedQuery.indexingStrategy == HASH){
        Table* table = tableCatalogue.getTable(parsedQuery.indexRelationName);

        int buckets = 2; //default

        if(parsedQuery.indexBuffer != ""){
            buckets = stoi(parsedQuery.indexBuffer);
        }

        table->indexed = true;
        table->indexedColumn = parsedQuery.indexColumnName;
        table->indexingStrategy = parsedQuery.indexingStrategy;

        int columnIndex = table->getColumnIndex(table->indexedColumn);

        table->hashTable = new HashTable(buckets);
        Cursor cursor = table->getCursor();

        vector<int> row;
        for (int rowCounter = 0; rowCounter < table->rowCount; rowCounter++)
        {
            row = cursor.getNext();
            table->hashTable->insertRecord({row[columnIndex], cursor.pageIndex, cursor.pagePointer-1});
        }
        table->hashTable->print();
    }
    else{ // NOTHING

    }

    return;
}
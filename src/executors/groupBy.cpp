#include "global.h"

/**
 * Syntax : <new_table> <- GROUP BY <grouping_attribute> FROM <table_name> RETURN MAX|MIN|SUM|AVG(<attribute>)
 * 
 * A_group_by_a <- GROUP BY c FROM A RETURN SUM(a)
 *  0            1  2    3  4 5    6 7       8  
 */

/**
 * @details Checking syntax of GROUP BY command
 * @return True if sytactically correct
 */
bool syntacticParseGROUPBY()
{
    logger.log("syntacticParseGROUPBY");

    // for(string s : tokenizedQuery){
    //     cout<<s<<endl;
    // }

    if (tokenizedQuery.size() != 9  || tokenizedQuery[3] != "BY" || tokenizedQuery[5] != "FROM" || tokenizedQuery[7] != "RETURN")
    {
        cout << "SYNTAX ERROR Group By" << endl;
        return false;
    }

    parsedQuery.queryType = GROUP_BY;
    parsedQuery.groupByResultRelationName = tokenizedQuery[0];
    parsedQuery.groupByRelationName = tokenizedQuery[6];
    parsedQuery.groupByColumnName = tokenizedQuery[4];
    

    string temp = tokenizedQuery[8];
    string type = "";
    string value = "";
    if(temp.size()>5 && temp[3]=='(' && temp[temp.size()-1]==')'){
        type = temp.substr(0, 3);
        cout<<type<<endl;
        value = temp.substr(4, temp.size()-5);
        cout<<value<<endl;
    }
    else{
        return false;
    }

    if(type=="SUM"){
        parsedQuery.aggregateType = SUM;
    }
    else if(type=="MIN"){
        parsedQuery.aggregateType = MIN;
    }
    else if(type=="MAX"){
        parsedQuery.aggregateType = MAX;
    }
    else if(type=="AVG"){
        parsedQuery.aggregateType = AVG;
    }
    else{
        return false;
    }

    parsedQuery.groupByAggregateAttribute = value;

    return true;
}


/**
 * @brief      Checking syntax of GROUP BY command
 *
 * @return     true if semantically correct
 */
bool semanticParseGROUPBY()
{
    logger.log("semanticParseGROUPBY");
   
    if(tableCatalogue.isTable(parsedQuery.groupByResultRelationName)){
        cout<<"SEMANTIC ERROR: Resultant relation already exists"<<endl;
        return false;
    }

    if(!tableCatalogue.isTable(parsedQuery.groupByRelationName)){
        cout<<"SEMANTIC ERROR: Relation doesn't exist " << parsedQuery.groupByRelationName <<endl;
        return false;
    }

    if(!tableCatalogue.isColumnFromTable(parsedQuery.groupByColumnName, parsedQuery.groupByRelationName)){
        cout<<"SEMANTIC ERROR: Column doesn't exist in relation"<<endl;
        return false;
    }

    if(!tableCatalogue.isColumnFromTable(parsedQuery.groupByAggregateAttribute, parsedQuery.groupByRelationName)){
        cout<<"SEMANTIC ERROR: Aggregate Column doesn't exist in relation"<<endl;
        return false;
    }

    
    return true;
}


/**
 * @brief      Execute the GROUP BY command
 */
void executeGROUPBY()
{
	// cout<<"executeGROUPBY\n";
    logger.log("executeGROUPBY");
    Table* table = tableCatalogue.getTable(parsedQuery.groupByRelationName);

	// if(table->indexingStrategy == BTREE){

 //    }
 //    else if(table->indexingStrategy == HASH){
 //    	// int columnIndex = table->getColumnIndex(table->indexedColumn);
 //    	// table->hashTable->insertRecord({inserting_row[columnIndex], 0, 0});
 //    	// table->hashTable->print();
 //    }
 //    else{
        
 //    }

    int groupColumnIndex = table->getColumnIndex(parsedQuery.groupByColumnName);
    int aggregateColumnIndex = table->getColumnIndex(parsedQuery.groupByAggregateAttribute);

    Aggregates type = parsedQuery.aggregateType;


    vector<int> row;
    map<int, int> groupMap;

    Cursor cursor = table->getCursor();

    for (int rowCounter = 0; rowCounter < table->rowCount; rowCounter++){
        row = cursor.getNext();
        if(type == SUM){
            if(groupMap.find(row[groupColumnIndex])==groupMap.end()){
                groupMap[row[groupColumnIndex]] = row[aggregateColumnIndex];
            }
            else{
                groupMap[row[groupColumnIndex]] += row[aggregateColumnIndex];
            }
        }
        else if(type == MAX){
            if(groupMap.find(row[groupColumnIndex])==groupMap.end()){
                groupMap[row[groupColumnIndex]] = row[aggregateColumnIndex];
            }
            else{
                groupMap[row[groupColumnIndex]] =max(groupMap[row[groupColumnIndex]], row[aggregateColumnIndex]);
            }
        }
        else if(type == MIN){
            if(groupMap.find(row[groupColumnIndex])==groupMap.end()){
                groupMap[row[groupColumnIndex]] = row[aggregateColumnIndex];
            }
            else{
                groupMap[row[groupColumnIndex]] =min(groupMap[row[groupColumnIndex]], row[aggregateColumnIndex]);
            }
        }
        else if(type == AVG){
            if(groupMap.find(row[groupColumnIndex])==groupMap.end()){
                groupMap[row[groupColumnIndex]] = row[aggregateColumnIndex];
            }
            else{
                groupMap[row[groupColumnIndex]] += row[aggregateColumnIndex];
            }
        }   
    }

    for(auto i : groupMap){
        cout<< i.first << " " << i.second <<endl;
    }

    vector<string> columns;
    columns.push_back(parsedQuery.groupByColumnName);
    if(type==SUM){
        columns.push_back("SUM"+parsedQuery.groupByAggregateAttribute);       
    }
    else if(type==MAX){
        columns.push_back("MAX"+parsedQuery.groupByAggregateAttribute);
    }
    else if(type==MIN){
        columns.push_back("MIN"+parsedQuery.groupByAggregateAttribute);
    }
    else if(type==AVG){
        columns.push_back("AVG"+parsedQuery.groupByAggregateAttribute);
    }

    

    Table *resultantTable = new Table(parsedQuery.groupByResultRelationName, columns);
    
    string line, word;
    
    for(auto i : groupMap){
        vector<int> row1;
        row1.push_back(i.first);
        row1.push_back(i.second);
        resultantTable->writeRow<int>(row1);
    }

    resultantTable->blockify();
    tableCatalogue.insertTable(resultantTable);

    return;
}


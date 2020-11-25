#include "global.h"
/**
 * @brief 
 * SYNTAX: R <- SELECT column_name bin_op [column_name | int_literal] FROM relation_name
 */
bool syntacticParseSELECTION()
{
    logger.log("syntacticParseSELECTION");
    if (tokenizedQuery.size() != 8 || tokenizedQuery[6] != "FROM")
    {
        cout << "SYNTAC ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = SELECTION;
    parsedQuery.selectionResultRelationName = tokenizedQuery[0];
    parsedQuery.selectionFirstColumnName = tokenizedQuery[3];
    parsedQuery.selectionRelationName = tokenizedQuery[7];

    string binaryOperator = tokenizedQuery[4];
    if (binaryOperator == "<")
        parsedQuery.selectionBinaryOperator = LESS_THAN;
    else if (binaryOperator == ">")
        parsedQuery.selectionBinaryOperator = GREATER_THAN;
    else if (binaryOperator == ">=" || binaryOperator == "=>")
        parsedQuery.selectionBinaryOperator = GEQ;
    else if (binaryOperator == "<=" || binaryOperator == "=<")
        parsedQuery.selectionBinaryOperator = LEQ;
    else if (binaryOperator == "==")
        parsedQuery.selectionBinaryOperator = EQUAL;
    else if (binaryOperator == "!=")
        parsedQuery.selectionBinaryOperator = NOT_EQUAL;
    else
    {
        cout << "SYNTAC ERROR" << endl;
        return false;
    }
    regex numeric("[-]?[0-9]+");
    string secondArgument = tokenizedQuery[5];
    if (regex_match(secondArgument, numeric))
    {
        parsedQuery.selectType = INT_LITERAL;
        parsedQuery.selectionIntLiteral = stoi(secondArgument);
    }
    else
    {
        parsedQuery.selectType = COLUMN;
        parsedQuery.selectionSecondColumnName = secondArgument;
    }
    return true;
}

bool semanticParseSELECTION()
{
    logger.log("semanticParseSELECTION");

    if (tableCatalogue.isTable(parsedQuery.selectionResultRelationName))
    {
        cout << "SEMANTIC ERROR: Resultant relation already exists" << endl;
        return false;
    }

    if (!tableCatalogue.isTable(parsedQuery.selectionRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }

    if (!tableCatalogue.isColumnFromTable(parsedQuery.selectionFirstColumnName, parsedQuery.selectionRelationName))
    {
        cout << "SEMANTIC ERROR: Column doesn't exist in relation" << endl;
        return false;
    }

    if (parsedQuery.selectType == COLUMN)
    {
        if (!tableCatalogue.isColumnFromTable(parsedQuery.selectionSecondColumnName, parsedQuery.selectionRelationName))
        {
            cout << "SEMANTIC ERROR: Column doesn't exist in relation" << endl;
            return false;
        }
    }
    return true;
}

bool evaluateBinOp(int value1, int value2, BinaryOperator binaryOperator)
{
    switch (binaryOperator)
    {
    case LESS_THAN:
        return (value1 < value2);
    case GREATER_THAN:
        return (value1 > value2);
    case LEQ:
        return (value1 <= value2);
    case GEQ:
        return (value1 >= value2);
    case EQUAL:
        return (value1 == value2);
    case NOT_EQUAL:
        return (value1 != value2);
    default:
        return false;
    }
}

vector<int> getRow(string tableName, int page_no, int row_no){
    string line, word;
    string filename = "../data/temp/"+tableName+"_Page"+to_string(page_no);
    ifstream fin(filename, ios::in);
    int c=0;
    vector<int> temp;
    while(getline(fin, line)){
        stringstream s(line);
        temp.clear();
        while(getline(s, word, ' ')){
            temp.push_back(stoi(word));
        }
        if(c==row_no){
            return temp;
        }
        c++;
        //cout<<line<<endl;
    }
    fin.close();
    return temp;
}
 
void executeSELECTION()
{
    logger.log("executeSELECTION");


    Table table = *tableCatalogue.getTable(parsedQuery.selectionRelationName);
    Table* resultantTable = new Table(parsedQuery.selectionResultRelationName, table.columns);

    if(table.indexed && parsedQuery.selectType == INT_LITERAL && table.indexedColumn==parsedQuery.selectionFirstColumnName){
        if(table.indexingStrategy==HASH){
            // int firstColumnIndex = table.getColumnIndex(parsedQuery.selectionFirstColumnName);
            if(table.hashTable!=NULL){
                cout<<"Optimized Selection using Indexing\n";
                vector<Record> records = table.hashTable->find(parsedQuery.selectionIntLiteral);
                for(Record record : records){
                    int page_no = record.page_no;
                    int row_no = record.row_no; 
                    if(table.validRecords[page_no][row_no]==1){
                        vector<int> row = getRow(parsedQuery.selectionRelationName, page_no, row_no);
                        resultantTable->writeRow<int>(row);
                    }
                }
                if(resultantTable->blockify())
                    tableCatalogue.insertTable(resultantTable);
                else{
                    cout<<"Empty Table"<<endl;
                    delete resultantTable;
                }
                return;
            }
        }
    }

    Cursor cursor = table.getCursor();
    vector<int> row = cursor.getNext();
    int firstColumnIndex = table.getColumnIndex(parsedQuery.selectionFirstColumnName);
    int secondColumnIndex;
    if (parsedQuery.selectType == COLUMN)
        secondColumnIndex = table.getColumnIndex(parsedQuery.selectionSecondColumnName);
    while (!row.empty())
    {

        int value1 = row[firstColumnIndex];
        int value2;
        if (parsedQuery.selectType == INT_LITERAL)
            value2 = parsedQuery.selectionIntLiteral;
        else
            value2 = row[secondColumnIndex];
        if (evaluateBinOp(value1, value2, parsedQuery.selectionBinaryOperator))
            resultantTable->writeRow<int>(row);
        row = cursor.getNext();
    }
    if(resultantTable->blockify())
        tableCatalogue.insertTable(resultantTable);
    else{
        cout<<"Empty Table"<<endl;
        delete resultantTable;
    }
    return;
}
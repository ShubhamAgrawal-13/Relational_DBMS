#include "global.h"
/**
 * @brief 
 * SYNTAX: LOAD relation_name
 */
bool syntacticParseLOAD()
{
    logger.log("syntacticParseLOAD");
    // if (tokenizedQuery.size() != 2)
    // {
    //     cout << "SYNTAX ERROR" << endl;
    //     return false;
    // }
    // parsedQuery.queryType = LOAD;
    // parsedQuery.loadRelationName = tokenizedQuery[1];
    // return true;

    if(tokenizedQuery.size() == 2){ // Simple load query
        parsedQuery.queryType = LOAD;
        parsedQuery.loadRelationName = tokenizedQuery[1];
        return true;
    }
    else if(tokenizedQuery.size() == 3 &&  tokenizedQuery[1]=="MATRIX"){ // Matrix
        logger.log("syntacticParseLOAD_MATRIX");
        parsedQuery.queryType = LOAD_MATRIX;
        parsedQuery.loadRelationName = tokenizedQuery[2];
        return true;
    }
    else{
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
}

bool semanticParseLOAD()
{
    logger.log("semanticParseLOAD");
    if (tableCatalogue.isTable(parsedQuery.loadRelationName))
    {
        cout << "SEMANTIC ERROR: Relation already exists" << endl;
        return false;
    }

    if (!isFileExists(parsedQuery.loadRelationName))
    {
        cout << "SEMANTIC ERROR: Data file doesn't exist" << endl;
        return false;
    }
    return true;
}

bool semanticParseLOAD_MATRIX()
{
    logger.log("semanticParseLOAD_MATRIX");
    if (tableCatalogue.isTable(parsedQuery.loadRelationName))
    {
        cout << "SEMANTIC ERROR: Matrix already exists" << endl;
        return false;
    }

    if (!isFileExists(parsedQuery.loadRelationName))
    {
        cout << "SEMANTIC ERROR: Data file doesn't exist" << endl;
        return false;
    }
    return true;
}

void executeLOAD()
{
    logger.log("executeLOAD");

    Table *table = new Table(parsedQuery.loadRelationName);
    if (table->load())
    {
        tableCatalogue.insertTable(table);
        cout << "Loaded Table. Column Count: " << table->columnCount << " Row Count: " << table->rowCount << endl;
    }
    // table->initializaValidRecords();
    return;
}

void executeLOAD_MATRIX()
{
    logger.log("executeLOAD_MATRIX");

    Matrix *matrix = new Matrix(parsedQuery.loadRelationName);
    if (matrix->load())
    {
        //Make catalogue for matrix
        matrixCatalogue.insertMatrix(matrix);
        cout << "Loaded Table. Column Count: " << matrix->columnCount << " Row Count: " << matrix->rowCount << endl;
    }
    return;
}
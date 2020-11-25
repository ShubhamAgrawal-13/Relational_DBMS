#include"global.h"

void executeCommand(){

    switch(parsedQuery.queryType){
        case CLEAR: executeCLEAR(); break;
        case CROSS: executeCROSS(); break;
        case DISTINCT: executeDISTINCT(); break;
        case EXPORT: executeEXPORT(); break;
        case INDEX: executeINDEX(); break;
        case JOIN: executeJOIN(); break;
        case LIST: executeLIST(); break;
        case LOAD: executeLOAD(); break;
        case LOAD_MATRIX: executeLOAD_MATRIX(); break;
        case PRINT: executePRINT(); break;
        case PROJECTION: executePROJECTION(); break;
        case RENAME: executeRENAME(); break;
        case SELECTION: executeSELECTION(); break;
        case SORT: executeSORT(); break;
        case SOURCE: executeSOURCE(); break;
        case TRANSPOSE: executeTRANSPOSE(); break;
        case INSERT: executeINSERT(); break;
        case DELETE: executeDELETE(); break;
        case BULK_INSERT: executeBULKINSERT(); break;
        case GROUP_BY: executeGROUPBY(); break;
        case ALTER: executeALTER(); break;
        default: cout<<"PARSING ERROR Executor"<<endl;
    }

    return;
}

void printRowCount(int rowCount){
    cout<<"\n\nRow Count: "<<rowCount<<endl;
    return;
}
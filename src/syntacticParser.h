#include "tableCatalogue.h"
#include "matrixCatalogue.h"

using namespace std;

enum QueryType
{
    CLEAR,
    CROSS,
    DISTINCT,
    EXPORT,
    INDEX,
    JOIN,
    LIST,
    LOAD,
    PRINT,
    PROJECTION,
    RENAME,
    SELECTION,
    SORT,
    SOURCE,
    INSERT,
    DELETE,
    LOAD_MATRIX,
    TRANSPOSE,
    UNDETERMINED,
    BULK_INSERT,
    GROUP_BY,
    ALTER
};

enum BinaryOperator
{
    LESS_THAN,
    GREATER_THAN,
    LEQ,
    GEQ,
    EQUAL,
    NOT_EQUAL,
    NO_BINOP_CLAUSE
};

enum SortingStrategy
{
    ASC,
    DESC,
    NO_SORT_CLAUSE
};

enum Aggregates
{
    MIN,
    MAX,
    AVG,
    SUM,
    NO_AGGREGATE_CLAUSE
};

enum AlterType
{
    ADD,
    DEL,
    NO_ALTER_CLAUSE
};

enum SelectType
{
    COLUMN,
    INT_LITERAL,
    NO_SELECT_CLAUSE
};

class ParsedQuery
{

public:
    QueryType queryType = UNDETERMINED;

    string clearRelationName = "";

    string crossResultRelationName = "";
    string crossFirstRelationName = "";
    string crossSecondRelationName = "";

    string distinctResultRelationName = "";
    string distinctRelationName = "";

    string exportRelationName = "";

    //indexing
    IndexingStrategy indexingStrategy = NOTHING;
    string indexColumnName = "";
    string indexRelationName = "";
    string indexBuffer = "";

    //insert
    string insertRelationName = "";
    vector<string> insertRow;

    //delete
    string deleteRelationName = "";
    vector<string> deleteRow;

    //Bulk Insert
    string bulkInsertInputFile = "";
    string bulkInsertRelationName = "";

    //Group By
    string groupByRelationName = "";
    string groupByResultRelationName = "";
    string groupByAggregateAttribute = "";
    string groupByColumnName = "";
    Aggregates aggregateType = NO_AGGREGATE_CLAUSE;

    //Alter
    string alterRelationName = "";
    string alterColumnName = "";
    AlterType alterType = NO_ALTER_CLAUSE;


    BinaryOperator joinBinaryOperator = NO_BINOP_CLAUSE;
    string joinResultRelationName = "";
    string joinFirstRelationName = "";
    string joinSecondRelationName = "";
    string joinFirstColumnName = "";
    string joinSecondColumnName = "";

    string loadRelationName = "";
    string transposeRelationName = "";

    string printRelationName = "";

    string projectionResultRelationName = "";
    vector<string> projectionColumnList;
    string projectionRelationName = "";

    string renameFromColumnName = "";
    string renameToColumnName = "";
    string renameRelationName = "";

    SelectType selectType = NO_SELECT_CLAUSE;
    BinaryOperator selectionBinaryOperator = NO_BINOP_CLAUSE;
    string selectionResultRelationName = "";
    string selectionRelationName = "";
    string selectionFirstColumnName = "";
    string selectionSecondColumnName = "";
    int selectionIntLiteral = 0;

    //sorting
    SortingStrategy sortingStrategy = NO_SORT_CLAUSE;
    string sortResultRelationName = "";
    string sortColumnName = "";
    string sortRelationName = "";
    string sortBuffer = "";

    string sourceFileName = "";

    ParsedQuery();
    void clear();
};

bool syntacticParse();
bool syntacticParseCLEAR();
bool syntacticParseCROSS();
bool syntacticParseDISTINCT();
bool syntacticParseEXPORT();
bool syntacticParseINDEX();
bool syntacticParseJOIN();
bool syntacticParseLIST();
bool syntacticParseLOAD();
bool syntacticParsePRINT();
bool syntacticParsePROJECTION();
bool syntacticParseRENAME();
bool syntacticParseSELECTION();
bool syntacticParseSORT();
bool syntacticParseSOURCE();
bool syntacticParseTRANSPOSE();
bool syntacticParseINSERT();
bool syntacticParseDELETE();

bool syntacticParseBULKINSERT();
bool syntacticParseGROUPBY();
bool syntacticParseALTER();


bool isFileExists(string tableName);
bool isQueryFile(string fileName);

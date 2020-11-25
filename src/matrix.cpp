#include "global.h"
#include <cmath>
/**
 * @brief Construct a new Matrix:: Matrix object
 *
 */

Matrix::Matrix()
{
    logger.log("Matrix::Matrix");
}

/**
 * @brief Construct a new Matrix:: Matrix object used in the case where the data
 * file is available and LOAD command has been called. This command should be
 * followed by calling the load function;
 *
 * @param matrixName 
 */
Matrix::Matrix(string matrixName)
{
    logger.log("Matrix::Matrix");
    this->sourceFileName = "../data/" + matrixName + ".csv";
    this->matrixName = matrixName;
}



/**
 * @brief The load function is used when the LOAD command is encountered. It
 * reads data from the source file, splits it into blocks and updates Matrix
 * statistics.
 *
 * @return true if the Matrix has been successfully loaded 
 * @return false if an error occurred 
 */

bool Matrix::load()
{
    logger.log("Matrix::load");
    fstream fin(this->sourceFileName, ios::in);
    string line;
    int count=0;
    if (getline(fin, line))
    {
        //fin.close();
        for(char ch : line){
            if(isalpha(ch)){
                cout << "Semantic Error : Not a Matrix" << endl;
                fin.close();
                return false;
            }   

            if(ch == ','){
                count++;
            }
        }

        this->columnCount = count+1;
        this->maxRowsPerBlock = (uint)((BLOCK_SIZE * 1000) / (32 * this->columnCount));
        this->maxElementsPerBlock = (uint)floor((BLOCK_SIZE * 1000) / (32) );
        this->maxBlocksPerRow = (uint)ceil((uint)(this->columnCount * 32)/(BLOCK_SIZE * 1000));

        if (this->blockify())
            return true;

    }
    fin.close();
    return false;
}

/**
 * @brief This function splits all the rows and stores them in multiple files of
 * one block size. 
 *
 * @return true if successfully blockified
 * @return false otherwise
 */

bool Matrix::blockify()
{
    logger.log("Matrix::blockify");
    ifstream fin(this->sourceFileName, ios::in);
    string line, word;
    vector<int> row;
    vector<vector<int>> rowsInPage(1, row);

    int pageCounter = 0;

    // unordered_set<int> dummy;
    // dummy.clear();
    // this->distinctValuesInColumns.assign(this->columnCount, dummy);
    // this->distinctValuesPerColumnCount.assign(this->columnCount, 0);

    //getline(fin, line);

    while (getline(fin, line))
    {
        stringstream s(line); // 1 3 5 4  |  6 7 7 7   |  76 44 4 5   |  7   
        int blk = this->maxBlocksPerRow;
        int row_counter = 0;
    
        for(int block=0; block<blk; block++){

            for (int c = 0; c < this->maxElementsPerBlock && getline(s, word, ',') ; c++){
                
                // if (!getline(s, word, ',')) // doubt
                //     return false;

                row.push_back( stoi(word) );
                row_counter++;
            }
            rowsInPage[0] = row;
            row.clear();

            //this->updateStatistics(row);

            if (row_counter == this->maxElementsPerBlock)
            {
                bufferManager.writePage(this->matrixName, this->blockCount, rowsInPage, 1);
                this->blockCount++;
                this->rowsPerBlockCount.emplace_back(1);
                //pageCounter = 0;
                row_counter=0;
            }
        }

        if (row_counter)
        {
            bufferManager.writePage(this->matrixName, this->blockCount, rowsInPage, 1);
            this->blockCount++;
            this->rowsPerBlockCount.emplace_back(1);
            //pageCounter = 0;
            row_counter=0;
        }

    }

    this->rowCount=this->columnCount;
    // if (this->rowCount == 0)
    //     return false;
    this->distinctValuesInColumns.clear();
    return true;
}

/**
 * @brief Given a row of values, this function will update the statistics it
 * stores i.e. it updates the number of rows that are present in the column and
 * the number of distinct values present in each column. These statistics are to
 * be used during optimisation.
 *
 * @param row 
 */

void Matrix::updateStatistics(vector<int> row)
{
    this->rowCount++;
    for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
    {
        if (!this->distinctValuesInColumns[columnCounter].count(row[columnCounter]))
        {
            this->distinctValuesInColumns[columnCounter].insert(row[columnCounter]);
            this->distinctValuesPerColumnCount[columnCounter]++;
        }
    }
}


// added functions 

/**
 * @brief Function prints the first few rows of the Matrix. If the Matrix contains
 * more rows than PRINT_COUNT, exactly PRINT_COUNT rows are printed, else all
 * the rows are printed.
 *
 */
void Matrix::print()
{
    logger.log("Matrix::print");
    uint count = min((long long)PRINT_COUNT, this->rowCount);

    //print headings
   // this->writeRow(this->columns, cout);

    Cursor cursor(this->matrixName, 0);
    vector<int> row;
    for (int rowCounter = 0; rowCounter < count; rowCounter++)
    {
        row = cursor.getNext();
        this->writeRow(row, cout);
    }
    printRowCount(this->rowCount);
}



/**
 * @brief This function returns one row of the Matrix using the cursor object. It
 * returns an empty row is all rows have been read.
 *
 * @param cursor 
 * @return vector<int> 
 */
void Matrix::getNextPage(Cursor *cursor)
{
    logger.log("Matrix::getNext");

        if (cursor->pageIndex < this->blockCount - 1)
        {
            cursor->nextPage(cursor->pageIndex+1);
        }
}


/**
 * @brief called when EXPORT command is invoked to move source file to "data"
 * folder.
 *
 */

vector<int> getBlock1(Matrix* matrix, int blockNo){
    string filename="../data/temp/" + matrix->matrixName + "_Page" + to_string(blockNo);
    vector<int> result;
    //cout<<blockNo<<"\n";
    string line, word;
    ifstream fin(filename, ios::in);
    getline(fin, line);
    stringstream s(line);
    for (int c = 0; c < matrix->maxElementsPerBlock && getline(s, word, ' ') ; c++){
        
        // if (!getline(s, word, ',')) // doubt
        //     return false;

        result.push_back(stoi(word));
    }
    fin.close();
    //cout<<"Read Successfullly\n";
    return result;
}

void Matrix::makePermanent()
{
    logger.log("Matrix::makePermanent");
    if(!this->isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
    
    string newSourceFile = "../data/" + this->matrixName + ".csv";
    ofstream fout(newSourceFile, ios::out);

    int block_count=0;
    for(int row = 0; row < this->rowCount; row++){

        for(int b=0; b < this->maxBlocksPerRow-1; b++){

            vector<int> block = getBlock1(this, block_count);
            for(int v : block)
            {
                fout<< v <<", ";
            }
            block_count++;

            // if(block_count% this->maxBlocksPerRow == 0)
            //     fout<<"\n";
        }
        vector<int> block = getBlock1(this, block_count);
        for(int k=0; k<block.size()-1; k++)
        {
            fout<< block[k] <<", ";
        }
        fout<< block[block.size()-1] << "\n";
        block_count++;

    }
    //fout << "\n";
    fout.close();
}

/**
 * @brief Function to check if Matrix is already exported
 *
 * @return true if exported
 * @return false otherwise
 */
bool Matrix::isPermanent()
{
    logger.log("Matrix::isPermanent");
    if (this->sourceFileName == "../data/" + this->matrixName + ".csv")
        return true;
    return false;
}

/**
 * @brief The unload function removes the Matrix from the database by deleting
 * all temporary files created as part of this Matrix
 *
 */

void Matrix::unload(){
    logger.log("Matrix::~unload");
    for (int pageCounter = 0; pageCounter < this->blockCount; pageCounter++)
        bufferManager.deleteFile(this->matrixName, pageCounter);
    if (!isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
}

/**
 * @brief Function that returns a cursor that reads rows from this Matrix
 * 
 * @return Cursor 
 */

Cursor Matrix::getCursor()
{
    logger.log("Matrix::getCursor");
    Cursor cursor(this->matrixName, 0);
    return cursor;
}
/**
 * @brief Function that returns the index of column indicated by columnName
 * 
 * @param columnName 
 * @return int 
 */
// int Matrix::getColumnIndex(string columnName)
// {
//     logger.log("Matrix::getColumnIndex");
//     for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
//     {
//         if (this->columns[columnCounter] == columnName)
//             return columnCounter;
//     }
// }

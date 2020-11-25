#include "global.h"
#define dprint(x) cout << #x << "=" << x <<endl

bool syntacticParseTRANSPOSE()
{
    logger.log("syntacticParseTRANSPOSE");
    
    if(tokenizedQuery.size() == 2){ 
        parsedQuery.queryType = TRANSPOSE;
        parsedQuery.transposeRelationName = tokenizedQuery[1];
        return true;
    }
    else{
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
}

bool semanticParseTRANSPOSE()
{
    logger.log("semanticParseTRANSPOSE");
    if (!matrixCatalogue.isMatrix(parsedQuery.transposeRelationName))
    {
        cout << "SEMANTIC ERROR: Relation already exists" << endl;
        return false;
    }

    if (!isFileExists(parsedQuery.transposeRelationName))
    {
        cout << "SEMANTIC ERROR: Data file doesn't exist" << endl;
        return false;
    }

    return true;
}

vector<int> getBlock(Matrix* matrix, int blockNo){
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
    

void writeBlock(Matrix* matrix, int blockNo, vector<int> row){
    string filename="../data/temp/" + matrix->matrixName + "_Page" + to_string(blockNo);
    // cout<<filename<<endl;
    string line, word;

    ofstream fout(filename, ios::trunc);
    for (int r = 0; r < row.size()-1; r++)
    {
        fout << row[r] <<" ";
    }

    fout << row[row.size()-1];
    fout.close();
    // cout<<"Written Successfullly\n";
}



void swap(int &a, int &b){
    int temp = a;
    a = b;
    b = temp;
}

void printMatrix(Matrix *matrix){
    int block_count=0;
    for(int row = 0; row < matrix->rowCount; row++){

        for(int b=0; b < matrix->maxBlocksPerRow; b++){

            vector<int> block = getBlock(matrix, block_count);
            for(int v : block)
            {
                cout<< v <<" ";
            }
            block_count++;

            if(block_count% matrix->maxBlocksPerRow == 0)
                cout<<endl;
        }

    }
    cout<<endl;
    cout<<endl;
}

void executeTRANSPOSE()
{
    logger.log("executeTRANSPOSE");

    Matrix* matrix = matrixCatalogue.getMatrix(parsedQuery.transposeRelationName);
    // dprint(matrix->maxBlocksPerRow);
    // dprint(matrix->maxElementsPerBlock);
    // dprint(matrix->columnCount);
    // dprint(matrix->rowCount);
    //cout<<"Before : \n\n";
    //printMatrix(matrix);
    int operations=0;
    for(int row = 0; row < matrix->rowCount; row++){

        int colCount=row+1;

        for(int b = row*matrix->maxBlocksPerRow + colCount/matrix->maxElementsPerBlock; b < row*matrix->maxBlocksPerRow + matrix->maxBlocksPerRow; b++){
            
            vector<int> block = getBlock(matrix, b);
            operations++;
            for(int c = colCount % matrix->maxElementsPerBlock; c<block.size() ; c++)
            { 
                int b2 = colCount*matrix->maxBlocksPerRow + row / matrix->maxElementsPerBlock;
                try{
                    vector<int> block2 = getBlock(matrix, b2);
                    
                    swap(block[colCount%matrix->maxElementsPerBlock], block2[row%matrix->maxElementsPerBlock]);

                    //cout<<b<<" "<<b2<<endl;
                    operations++;

                    writeBlock(matrix, b2, block2);

                    colCount++;
                }
                catch (const char *excp)  { 
                    cout << "Caught " << excp; 
                } 

            }

            writeBlock(matrix, b, block);
        
        }

    }
    // cout<<"After : \n\n";
    //printMatrix(matrix);
    //cout<<"Operations : "<<operations<<endl;
}


/*

// Cursor cursor = matrix->getCursor();

    // for(int b=0; b < matrix->blockCount; b++ ){

    //     vector<int> row = cursor.getMatrixRow();
    //     for(int v : row){
    //         cout<< v <<" ";
    //     }
    //     cout<<"\n";
    // }


*/



// #include<bits/stdc++.h>
// using namespace std;

#define MAX 100
int fanout=10;
bool IS_INT_FLAG=0;
bool IS_SHARED=1;
bool PARENT_LOCK=0;

struct Node{
    int n;
    Node *parent;
    Node *next;
    int value[MAX];
    Node *children[MAX];
    Record records[MAX]; 
    Node(){
        n = 0;
        parent = NULL;
        next = NULL;
        for(int i=0; i<MAX; i++){
            value[i] = INT_MAX;
            children[i] = NULL;
        }
    }
};


struct LeafNode{
    int n;
    LeafNode *parent;
    LeafNode *next;
    int value[MAX];
    Node *children[MAX];
    Record records[MAX];
    Record sharedFiles[MAX];  
    LeafNode(){
        n = 0;
        parent = NULL;
        next = NULL;
        for(int i=0; i<MAX; i++){
            value[i] = INT_MAX;
            children[i] = NULL;
        }
    }
};

class BPlusTree{
public:
    //static int fanout;
    Node *rootBlock;

    BPlusTree(){
        //this->fanout = fanout;
        rootBlock = new Node();
    }

    void splitLeaf(Node *);
    void splitNonLeaf(Node *);
    void insertNode(Node *, int);
    // void print(vector<Block *>);
    
};

int checkValueIsINT(bool IS_INT_FLAG)
{
    if(IS_INT_FLAG==false)
        return true;
    return !IS_INT_FLAG;
}

void splitLeaf(Node *currentNode){
    int x, i, j;

    if(fanout%2)
        x = (fanout+1)/2;
    else x = fanout/2;

    Node *rightNode = new Node();
    currentNode->n = x;
    rightNode->n = fanout-x;
    rightNode->parent = currentNode->parent;

    for(i=x, j=0; i<fanout; i++, j++){
        rightNode->value[j] = currentNode->value[i];
        currentNode->value[i] = INT_MAX;
    }
   
    int val = rightNode->value[0];

    if(currentNode->parent==NULL){  // Root
        Node *parent = new Node();
        parent->parent = NULL;
        parent->children[0] = currentNode;
        parent->children[1] = rightNode;
        parent->n=1;
        parent->value[0] = val;
        currentNode->parent = rightNode->parent = parent;
        rootBlock = parent;
        parent->children[0]->next=parent->children[1];
        return;
    }
    else{ 
        currentNode = currentNode->parent;

        Node *newChildBlock = new Node();
        newChildBlock = rightNode;

        for(i=0; i<=currentNode->n; i++){
            if(val < currentNode->value[i]){
                swap(currentNode->value[i], val);
            }
        }

        currentNode->n++;
        for(i=0; i<currentNode->n; i++){
            if(newChildBlock->value[0] < currentNode->children[i]->value[0]){
                swap(currentNode->children[i], newChildBlock);
            }
        }
        currentNode->children[i] = newChildBlock;

        for(i=0;currentNode->children[i]!=NULL;i++){
            currentNode->children[i]->parent = currentNode;
        }

        currentNode->children[0]->next=currentNode->children[1];
    }

}


void splitNonLeaf(Node *currentNode){
    int x, i, j;

    x = fanout/2;
    Node *rightNode = new Node();
    currentNode->n = x;
    rightNode->n = fanout-x-1;
    rightNode->parent = currentNode->parent;

    for(i=x, j=0; i<=fanout  && IS_SHARED; i++, j++){
        rightNode->value[j] = currentNode->value[i];
        rightNode->children[j] = currentNode->children[i];
        currentNode->value[i] = INT_MAX;
        if(i!=x)currentNode->children[i] = NULL;
    }

    int val = rightNode->value[0];
    memcpy(&rightNode->value, &rightNode->value[1], sizeof(int)*(rightNode->n+1));
    memcpy(&rightNode->children, &rightNode->children[1], sizeof(rootBlock)*(rightNode->n+1));

    for(i=0;currentNode->children[i]!=NULL;i++){
        currentNode->children[i]->parent = currentNode;
    }
    for(i=0;rightNode->children[i]!=NULL;i++){
        rightNode->children[i]->parent = rightNode;
    }

    if(currentNode->parent==NULL && IS_SHARED){ //Root
        Node *parent = new Node();
        parent->parent = NULL;
        parent->children[0] = currentNode;
        parent->children[1] = rightNode;
        parent->n=1;
        parent->value[0] = val;
        currentNode->parent = rightNode->parent = parent;
        rootBlock = parent;
        return;
    }
    else{   
        currentNode = currentNode->parent;

        Node *newChildBlock = new Node();
        newChildBlock = rightNode;

        for(i=0; i<=currentNode->n; i++){
            if(val < currentNode->value[i] && IS_SHARED){
                swap(currentNode->value[i], val);
            }
        }
        currentNode->n++;

        for(i=0; i<currentNode->n; i++){
            if(newChildBlock->value[0] < currentNode->children[i]->value[0] && IS_SHARED){
                checkValueIsINT(IS_INT_FLAG);
                swap(currentNode->children[i], newChildBlock);
            }
        }
        currentNode->children[i] = newChildBlock;

        for(i=0;currentNode->children[i]!=NULL;i++){
            currentNode->children[i]->parent = currentNode;
        }
    }

}

void insertNode(Node *currentNode, int val){

    for(int i=0; i<=currentNode->n; i++){
        if(IS_SHARED && val < currentNode->value[i] && currentNode->children[i]!=NULL){
            insertNode(currentNode->children[i], val);
            checkValueIsINT(IS_INT_FLAG);
            if(IS_SHARED && currentNode->n==fanout)
                splitNonLeaf(currentNode);
            return;
        }
        else if(!PARENT_LOCK && val < currentNode->value[i] && currentNode->children[i]==NULL){
            swap(currentNode->value[i], val);
            if(i==currentNode->n){
                    currentNode->n++;
                    break;
            }
        }
    }
    if(currentNode->n==fanout){
            splitLeaf(currentNode);
    }
}

// void print(vector < Node* > Blocks){
//     vector < Node* > newBlocks;
//     for(int i=0; i<Blocks.size(); i++){ 
//         Node *currentNode = Blocks[i];

//         cout <<"[|";
//         int j;
//         for(j=0; j<currentNode->n; j++){  
//             cout << currentNode->value[j] << "|";
//             if(currentNode->children[j]!=NULL)
//             newBlocks.push_back(currentNode->children[j]);
//         }
//         if(currentNode->value[j]==INT_MAX && currentNode->children[j]!=NULL)
//             newBlocks.push_back(currentNode->children[j]);

//         cout << "]  ";
//     }

//     if(newBlocks.size()==0){
//         puts("");
//         puts("");
//         Blocks.clear();
//         //exit(0);
//     }
//     else {                  
//         puts("");
//         puts("");
//         Blocks.clear();
//         print(newBlocks);
//     }
// }

// int main(){
//     fanout = 10;
//     vector < Node* > Blocks;
//     int i = 0;
//     int item = 100;
//     while(item--){
//         // int item;
//         // cin>>item;
//         // if(item == -1)
//         //     break;
//         insertNode(rootBlock, item);
//         i++;
//     }
//     Blocks.clear();
//     Blocks.push_back(rootBlock);
//     print(Blocks);
//     puts("");
//     return 0;
// }
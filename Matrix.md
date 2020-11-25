## REPORT:-
------------


We stored a matrix row wise. If the matrix row doesn’t fit into a single page we split it into several blocks. We build a matrix class to handle all operations for matrix type relation. 

Suppose our block size is of 4 integers so below is the example how we will store each row of matrix. In below example the matrix size is 13*13 so to store a single row we required 4 blocks.



### maxElementsPerBlock = blockSize / INT_SIZE

### And the blocks needed to store a single row is calculated by

### maxBlocksPerRow = ceil (rowCount / maxElementsPerBlock)
```

##                     (13*13 MATRIX)
-------------------------------------------------------------------------------

0 1 2 3         4 5 6 7         8 9 10 11        12		-->INDEX

-------------------------------------------------------------------------------

1 2 3 4		5 6 7 8		9 10 11	12	 13		-->contents
page0	 	page1  	        page2		 page3		-->row0

-------------------------------------------------------------------------------

1 2 3 4		5 6 7 8		9 10 11	12	 13
page4 		page5		page6		 page7		-->row1

-------------------------------------------------------------------------------

1 2 3 4		5 6 7 8		9 10 11	12	 13
page8 		page9		page10	         page11	        -->row2

-------------------------------------------------------------------------------

1 2 3 4		5 6 7 8		9 10 11	12	 13
page12   	page13	        page14	        page15	        -->row3

-------------------------------------------------------------------------------

.
.
.
.
.

so on.

---------------------------------------------------------------------------------
```

# Number of Block access required to perform transpose of matrix in-place is:
		= nearly ( N * N ) / 2

# PseudoCode :- 
```
for r in range(0-row-1):
	col = 0
	for each block b of row r:
		vector blk = Load(r,col) // Loads page which contains A[r][col]
		for each element e in blk:
			vector temp_blk = load(col,r) 
			swap(blk[e] ,blk[r]) // swaps A[r][col] with A[col][r]
```


-----------------------------------------------------------------------------------






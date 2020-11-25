B+ tree  

Linear hashing (Done)


External merge sort -----> without index (Done)
 					-----> with index     

Integration:
------------

1. Insert 
2. Find (Select)
3. Delete

in page.




lexiographical order
--------------------
page_3 --> page_3_0 ---> page_3_1 ---> page_3_0_1


----------------------------------

1 BLOCK_SIZE = 8
2 BLOCK_COUNT = 10
3 PRINT_COUNT = 20


----------------------------------
BULK_INSERT <csv_file_name> INTO <table_name>

-----------------------------------
<new_table> <- GROUP BY <grouping_attribute> FROM <table_name> RETURN MAX|MIN|SUM|AVG(<attribute>)

------------------------------------
ALTER TABLE <table_name> ADD|DELETE COLUMN <column_name>

------------------------------------

strategy.md



Server -> syntatic.h -> synatatic.cpp -> semantic.h -> semantic.cpp -> executor.h -> executor.cpp -> 




 21 rows 3 page 10 row/page

    10   --> v[0][0-9]
    10   --> v[1][0-9]
    1    --> v[2][0]

    int page = validRow.size()-1;
    int row_no = validRow[page].size()-1;

    if(row_no+1 == page_Size){
        create new page
        validRow.push_back(vector())
        validRow[page+1].push_back(1);
    }
    else{
        row_no++;
    }



1.
maxRowPerBlock Block
buffer manager pages delete
table unload
table ispermanent
blockify

2.
write row

3.



Alter -> 1hr

delete 
select
sorting indexing

strategy.md

form

Testing 

<!-- B+ tree insert find -->




INDEX ON a FROM A USING HASH BUCKETS 2

AA <- SORT S BY a IN ASC

AA <- SORT S BY a IN DESC

AA <- SORT A BY a IN DESC

INSERT INTO A VALUES 41,42,43,44,45

DELETE​ FROM​ A VALUES 41,20,30,40,50


RRR <- SELECT a == 5 FROM A


A_group_by_a <- GROUP BY c FROM AA RETURN SUM(a)

A_group_by_a_max <- GROUP BY c FROM AA RETURN MAX(a)

BULK_INSERT T INTO A

ALTER TABLE A ADD COLUMN tr
ALTER TABLE A DELETE COLUMN a

STRATEGY:
---------

We maintained one additonal data member in table file. So that it can track the validity of the records. It is a boolean 2d vector in which 
	
	validRecord[page_no][row_no] = true , if record is present
							       false, otherwise.


It will make the EXPORT and DELETE command faster.
# Power-spread-sheet-program

This is a new kind of implementation for a spreadsheet program. It is highly scalable. 
It has three classes: Sheet, Page and Cell.
At a given time, you can access only one page and store data in cells.
There is just one sheet which has multiple number of dynamically allocated page objects. When the sheet is initialised all the page instances are set to NULL.
Each page instance has multiple number of dynamically allocated cells. When the page is initialised all the cell instances are set to NULL.
Each cell instance can store a dynamically allocated string.

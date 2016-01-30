# Power-spread-sheet-program

This is a new kind of implementation for a spreadsheet program. It is highly scalable. 
It has three classes: Sheet, Page and Cell.
At a given time, you can access only one page instance and store data in a cell instance.
There is just one sheet instance which has multiple number of dynamically allocated page instances. When the sheet is initialised all the page instances are set to NULL. As the user approches the next page instance, then that page is initialized. 
Same goes for page instances. Each page instance has multiple number of dynamically allocated cells. When the page is initialised all the cell instances are set to NULL. As the user approches an arbitrary cell, she has to press enter to initialise that cell and write data to in it.
Each cell instance can store a dynamically allocated string.
This program is codded in C++. it uses some Windows library.

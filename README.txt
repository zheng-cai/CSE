Author	         : Zheng Cai
Created.         : November 14, 2019
Last Modified    : November 14, 2019

Affiliation      : Georgia Institute of Technology


Description
--------------

This program aims to use parallel programming by omp.h to calculate matrix multiplication. The form is like C=A*B and both matrixes A and B have the same size of columns and rows that you can set. Dynamic and static ways are used in this program to compare their speed under the different sizes of matrixes and threads. The outputs are matrixes A, B,  threads, size of matrixes computing  results of static and dynamic ones and each running time.


Installation
-------------

For install, simple run 
	gcc main.c -std=c99  -o A7 -fopenmp
You can modify ASS7 after ¡°-o¡± with any name you want. 


Execution
------------

Assuming your executable is called A7 run it using
	./A7

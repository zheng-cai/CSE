Author          : Zheng Cai
Created         : October 11, 2019
Last Modified   : October 11, 2019

Affiliation          : Georgia Institute of Technology


Description
-------------

My part is to generate scale-free networks and store them by the adjacency list in memory. By the rule, each existed node has the probability of being added new nodes regarding the edges it has. As well, the link weight between every pair is generated randomly. 

Installation
------------

To install, simply run

    gcc main.c -std=c99 -o A5

Note: The -std=c99 flag is required on some C compilers
to allow variable declarations at arbitrary locations in
a function.

Execution
-----------

Assuming your executable is called "A2", the first parameter N is total number of nodes and [w1,w2] is the range of link weight of every two points. Then top.txt is the format to store these graph and histogram output the 'histogram' required by the assignment. 

./A5 N w1 w2 -o top.txt -h histogram.txt


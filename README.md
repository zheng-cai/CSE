Author          : Zheng Cai
Created         : September 25, 2019
Last Modified   : September 26, 2019

Affiliation          : Georgia Institute of Technology


Description
-------------

This program implements Kmeans algorithm. It can cluster every point to one center with the minmium euclidean distance. First it sets k centroids to run the cluster and adjust centroids every iteration until centroids doesn't change. 

Installation
------------

To install, simply run

    gcc main.c -std=c99 -o A4

Note: The -std=c99 flag is required on some C compilers
to allow variable declarations at arbitrary locations in
a function.

Execution
-----------

Assuming your executable is called "A2", run it using

    ./A4 infile K outfile


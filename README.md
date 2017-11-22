# treedist
#### Rapid calculation of pathlength distances between taxa on phlyogenetic trees

This comprises two tools that are used to calculate pathlength distance on phylogenetic trees.

------------------
#### treedist_pair:

  This sofware returns the distance between *two* branches of a tree *(including internal branches)*.
  
  You must call this software with the name of the file containing the tree, and the names of the two branches
  
  Usage: `treedist <Tree file name> <Branch name> <Branch name>`
 
------------------  

#### treedist_all:

  This sofware returns the pathlength distances between *all* terminal branches of a tree.
  
  You must call this software with the name of the file containing the tree
  
  Usage: `treedist_all <Tree file name> <matrix|vector|text>`
  
  Optional output format specification: matrix (default) or vector.
	If a text string is specified instead of "matrix" or "vector" it should be a whole or part of a taxon name.
	In this case treedist_all will only print in vector format distances to or from taxa that match the text string
 
  
  ------------------ 
  
  ## To Compile
  
  ```
  cc -o treedist_pair.c -o treedist_pair
  cc -o treedist_all.c -o treedist_all
  ```
  
  ------------------ 
  
  ## Example usage:
  
  With the provided example tree file 'testtree.ph'
  
  ```
  ((A:0.01,B:0.02)I5:0.1,(((C:0.02,D:0.01)I2:0.1,E:0.001)I3:0.1,F:0.003)I4:0.05);
  ```
  
  which looks like this:
  ![Test tree](https://github.com/ChrisCreevey/treedist/blob/master/testtree.png)
  
  To calculate the pathlength distances between:
  
  ### A and B
  
  ```
  treedist_pair testtree.ph A B
  ```
  which returns '0.030000'
  
  ### A and Internal branch I2
  
  ```
  treedist_pair testtree.ph A I2
  ```
  which returns '0.360000'
  
  
  ### All distances between all terminal taxa
  
  ```
  treedist_all testtree.ph matrix
  ```
  
  Which returns a file called 'testtree.ph.dist' containing:
  
```
  	A	B	C	D	E	F
A	0.000000	0.030000	0.380000	0.370000	0.261000	0.163000
B	0.030000	0.000000	0.390000	0.380000	0.271000	0.173000
C	0.380000	0.390000	0.000000	0.030000	0.121000	0.223000
D	0.370000	0.380000	0.030000	0.000000	0.111000	0.213000
E	0.261000	0.271000	0.121000	0.111000	0.000000	0.104000
F	0.163000	0.173000	0.223000	0.213000	0.104000	0.000000
```

  ### To return the same information but in vector (pairwise) format:
  
  ```
  treedist_all testtree.ph vector
  
  ```
  
  which will return:
  
  ```
  A	B	0.030000
A	C	0.380000
A	D	0.370000
A	E	0.261000
A	F	0.163000
B	C	0.390000
B	D	0.380000
B	E	0.271000
B	F	0.173000
C	D	0.030000
C	E	0.121000
C	F	0.223000
D	E	0.111000
D	F	0.213000
E	F	0.104000
```

*NOTE* that each pair-wise distance is only reported once in vector format.


### All distances from taxa matching the text string "A" to everything else

```
treedist_all testtree.ph A
```

Which returns in vector format:

```
A       B       0.030000
A       C       0.380000
A       D       0.370000
A       E       0.261000
A       F       0.163000
```

This option allows the full name or partial names to be used, so if you have the taxa "Pan paniscus" and "Pan troglodytes" in your tree and you can specify 

```
treedist_all TREE Pan
```

To only output (in vector format) the distances between all species matching the text string "Pan" to all other species.



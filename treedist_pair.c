/* TREEDIST_pair  */
/* Written by Chris Creevey */
/* chris.creevey@gmail.com */
/* Version 1.1  */
/* This software returns the path length distance between two taxa on the specified tree */ 





#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <signal.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TEMP
#define TEMP 2
#endif


void unroottree(char * tree);




int main(int argc, char *argv[])
    {
    int i=0, j=0, k=0, charactercount = -1, variable = 0, node_number = -1, open = 0, found = FALSE, fromroot = FALSE, first = FALSE, averagelength = FALSE; 
    char number[1000], infilename[1000], branch1[1000], branch2[1000], c = '\0', string[100000], temp[100];
    float *taxa_weights = '\0', *node_weights = '\0',  **closeP = '\0', branchlength = 0, totallength = 0, lastlength =0, firstlength = 0, x=0, y=0, increase = 0.1;
	FILE *infile = '\0';
	

	if(argc < 4)
		{
		printf("Treedist_pair:\n\tThis sofware returns the distance between two branches of a tree (including internal branches).\n\tYou must call this software with the name of the file containing the tree, and the names of the two branches\n\n\tUsage: treedist <Tree file name> <Branch name> <Branch name>\n\n");
		exit(1);
		}
	if(argc >= 5)
		{
		if(strcmp(argv[4], "fromroot") == 0)
			fromroot = TRUE;
		else
			{
			if(strcmp(argv[4], "averagelength") == 0)
				averagelength = TRUE;
			else
				{
				printf("unknown parameter :%s\n", argv[4]);
				exit(1);
				}
			}
		}
	if(argc == 6)
		{
		increase = atof(argv[5]);
		}
			
	number[0] = '\0';
	infilename[0] = '\0';
	branch1[0] = '\0';
	branch2[0] = '\0';	
	temp[0] = '\0';
	strcpy(infilename, argv[1]);
	strcpy(branch1, argv[2]);
	strcpy(branch2, argv[3]);


	if((infile = fopen(infilename, "r")) == '\0')		/* check to see if the file is there */
		{								/* Open the source tree file */
		printf("Cannot open file %s\n", infilename);
		exit(1);
		}
	
	i =0;
	c = getc(infile);
	while(c != ';' && !feof(infile))
		{
		string[i] = c;
		c = getc(infile);
		i++;
		}
		
	string[i] = ';';
	string[i+1] = '\0';
	fclose(infile);
	
	/* check to make sure that the two branches specified are in the tree */
	if(strcasestr(string, branch1) == '\0')
		{
		printf("ERROR: cannot find %s in the tree\n", branch1);
		exit(1);
		}
	if(strcasestr(string, branch2) == '\0')
		{
		printf("error: cannot find %s in the tree\n", branch2);
		exit(1);
		}
	
	if(strcmp(branch1, branch2)==0)
		{
		printf("0.00000\n");
		exit(1);
		}
	
    /* The array characters is used to keep track, for each taxa, the open and closed brackets that has followed each */
   /* unroottree(string); */
    i=0;
	/*** Find the first of the two branches of interest ***/
	while(string[i] != ';' && !found)
		{
		switch(string[i])
			{
			case '(':
			case ')':
			case ',':
				i++;
				break;
			case ':':
				while(string[i] != '(' && string[i] != ')' && string[i] != ',' && string[i] != ';') i++;
				break;
			default:
				j=0;
				while(string[i] != '(' && string[i] != ')' && string[i] != ',' && string[i] != ';' && string[i] != ':')
					{
					temp[j] = string[i];
					i++; j++;
					}
				temp[j] = '\0';
				if(strcmp(temp, branch1) == 0)
					{
					if(fromroot)
						first = TRUE;
					found = TRUE;
					}
				if(strcmp(temp, branch2) == 0) found = TRUE;
				break;
			}
		}
	/** read in the branch length information for this branch */
	i++;
	j=0;
	while(string[i] != '(' && string[i] != ')' && string[i] != ',' && string[i] != ';')
		{
		number[j] = string[i];
		j++; i++;
		}
	number[j] = '\0';
	if(fromroot)
		{
		if(!first)
			lastlength = atof(number);
		else
			firstlength = atof(number);
		}
	/*totallength = atof(number)*0.5;*/
	totallength = atof(number);
	node_number = 0;
	found = FALSE;
	first = FALSE;
    while(i< strlen(string) && string[i] != ';' && !found)  /* until the end of the string */
        {
        switch(string[i])
            {
            case '(':	
                        if(i != 0)
                            {
                            k=i;
                            open = 1;
                            do
                                {
                                k++;
                                if(string[k] == '(') open++;
                                if(string[k] == ')') open--;
                                
                                }while(string[k] != ')' || open != 0);
							 node_number++;
							 j=0; k++;
							 while(string[k] != ':' && string[k] != ';')
								{
								temp[j] = string[k];
								j++; k++;
								}
							temp[j] = '\0';
							if(strcmp(temp, branch1) == 0)
								{
								first = TRUE;
								found = TRUE;
								}
							if(strcmp(temp, branch2) == 0) found = TRUE;
							if(string[k] == ':')
								{
								k = k+1;  /* skip over the ':' to the start of the number */
								for(j=0; j<30; j++) number[j] = '\0';  /* initialise the array to hold the number in text form */
								j=0;
								while(string[k] != ')' && string[k] != ',' && string[k] != '(' && string[k] != ';')
									{
									number[j] = string[k];
									k++; j++;
									}  /* read in the weight */
								branchlength = atof(number);
								if(found)
									{
									if(fromroot)
										{
										if(!first)
											lastlength = atof(number);
										else
											firstlength = atof(number);
										}
									/*totallength += branchlength*0.5; */
									totallength += branchlength;
									
									}
								else
									totallength += atof(number);
								} /* otherwise the software will assume branch lengths of 1 for all */
                            }
                        i++;
                        break;
            case ')':	
                        i++; j=0;
						while(string[i] != ':' && string[i] != ';' )
							{
							temp[j] = string[i];
							i++; j++;
							}
						temp[j] = '\0';
						if(strcmp(temp, branch1) == 0)
							{
							first = TRUE;
							found = TRUE;
							}
						if(strcmp(temp, branch2) == 0) found = TRUE;
						if(string[i] != ';' )
                            {
                            i++;
                            for(j=0; j<30; j++) number[j] = '\0';  /* initialise the array to hold the number in text form */
								j=0;
							while(string[i] != ')' && string[i] != ',' && string[i] != '(' && string[i] != ';')
								{
								number[j] = string[i];
								i++; j++;
								}  /* read in the weight */
							 branchlength = atof(number);
							 if(found) 
								{
								if(fromroot)
									{
									if(!first)
										lastlength = atof(number);
									else
										firstlength = atof(number);
									}
								/*totallength += branchlength*0.5; */
									totallength += branchlength;
								
								}
							if(node_number > 0 && !found)
								{
								node_number--;
								totallength -= branchlength;
								}
							else
								{
								totallength += branchlength;
								}
							}
                        else
                            i++;
                        break;
            case ',':
                        i++;
                        break;
            default:
                        /* this has to be a taxa name */
                        for(j=0; j<30; j++) number[j] = '\0';  /* initialise the array to hold the number in text form */ 
                        j=0;
                        while(string[i] != ':' && string[i] != ')' && string[i] != '(' && string[i] != ',')
                            {
                            number[j] = string[i];
                            i++; j++;
                            }
						
						if(strcmp(number, branch1) == 0)
							{
							first = TRUE;
							found = TRUE;
							}
						if(strcmp(number, branch2) == 0)
							{
							found = TRUE;
							}
                       
                        /* Now read in the weight for this taxa */
						if(string[i] == ':')
							{
							for(j=0; j<30; j++) number[j] = '\0';  /* initialise the array to hold the number in text form */
							j=0; i++;
							while(string[i] != ')' && string[i] != ',' && string[i] != '(' && string[i] != ';')
								{
								number[j] = string[i];
								i++; j++;
								}
							branchlength = atof(number);
							}
						if(found)
							{
							if(fromroot)
								{
								if(!first)
									lastlength = atof(number);
								else
									firstlength = atof(number);
								}
							/*totallength += branchlength*0.5; */
									totallength += branchlength;
							}
                        break;
            }
            
    
        }
	if(fromroot)
		{
		/*printf("First: %f Last: %f\n", firstlength, lastlength); */
		/*printf("%f\t%f\n", (totallength - (firstlength/2)) - (lastlength/2), (totallength - (firstlength/2)) + (lastlength/2)); */
		y =0;
		for(x=(totallength - (firstlength/2)) - (lastlength/2); x <= (totallength - (firstlength/2)) + (lastlength/2); x+=increase) y++;
		y++;
		for(x=(totallength - (firstlength/2)) - (lastlength/2); x <= (totallength - (firstlength/2)) + (lastlength/2); x+=increase)
			printf("%.3f\t%.3f\n", x, 1/y);
		printf("%.3f\t%.3f\n", x, 1/y);
		}
	else
		if(averagelength)
			{
			printf("%f\n", lastlength);
			}
		else
			{
			
			printf("%f\n", totallength);
			
			}
			
    }
	
	
	
	
	
void unroottree(char * tree)
    {
    int i=0, j=0, basecount = 0, parentheses=0;
    int foundopen = FALSE, foundclose = FALSE;
    

    /* scan through the tree counting the number of taxa/nodes at the base (for it to be unrooted there should be at least three) */
    while(tree[i] != ';')
        {
        switch(tree[i])
            {
            case '(':
                    parentheses++;
                    i++;
                    break;
            case ')':
                    parentheses--;
                    i++;
                    break;
            case ',':
                    if(parentheses == 1)
                        {
                        basecount++;
                        }
                    i++;
                    break;
            default:
                    i++;
                    break;
            }
        }
        
    if(basecount <2)  /* if the base of the tree is rooted */
        {
        i=0;
        parentheses = 0;
        while(tree[i] != ';')  /* delete the two parentheses to make the tree unrooted */
            {
            switch(tree[i])
                {
                case '(':
                        parentheses++;
                        if(parentheses == 2 && !foundopen)
                            {
                            tree[i] = '^';
                            foundopen = TRUE;
                            }
                        i++;
                        break;
                case ')':
                        if(parentheses == 2 && !foundclose)
                            {
                            tree[i] = '^';
                            if(tree[i+1] == ':')
                                {
                                i++;
                                while(tree[i] != ')' && tree[i] != '(' && tree[i] != ',' && tree[i] != ';')
                                    {
                                    tree[i] = '^';
                                    i++;
                                    }
                                }
                            foundclose = TRUE;
                            }
                        i++;
                        parentheses--;
                        break;
                default:
                        i++;
                        break;
                }
            }
                        
        /* scan through the string shifting up the characters to take into account those parentheses that have been deleted */
        i=0; j=0;
        while(tree[j] != ';')
            {
            if(tree[j] == '^')
                {
                while(tree[j] == '^')
                    j++;
                if(i!= j)tree[i] = tree[j];
                i++; j++;
                }
            else
                {
                if(i!=j)tree[i] = tree[j];
                i++;j++;
                }
            }
        tree[i] = tree[j];
        tree[i+1] = '\0';
        
        }

    }


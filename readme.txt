LIght

The commandline syntax for hw4 is as follows:
    hw4 mazegen w h [-info] [-top=tpos] [-bottom=bpos] [-left=lpos] [-right=rpos] [rndfile]
    hw4 mazesolve [file]
Square bracketed items are optional. If rndfile or file is not specified, your program should read from stdin.

mazegen :  Use the Union-Find algorithm to generate a w (columns) by h (rows) maze. The -top, -bottom, -left, and -right commandline options specifies either the entrace or the exit to the maze. 1 ≤ tpos, bpos ≤ w ≤ 64 and 1 ≤ lpos, rpos ≤ h ≤ 32. The input (or rndfile) contains random numbers which you must read in order to determine which wall to remove in your maze.
Either none or exactly two of -top, -bottom, -left, and -right can be specified. If none of them are specified, it should be taken as if -top=1 and -bottom=w are specified.

If the -info commandline option is specified, when you have selected a wall for removal, you must print out which wall you are removing.

 
mazesolve:  The input to the mazesolve command has the same format (or with a slight variation) as the output of the mazegen command. Your job is to use Breadth-First-Search or Dijkstra's Shortest Path algorithm to solve the maze.
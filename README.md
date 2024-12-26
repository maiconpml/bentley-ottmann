# bentley ottmann
 Simple implementation of bentley ottmann algorithm in c++ to find intersections between segments.

# Compile

```bashsession
user~$: bash compile.sh 
```
# Run

Receives an integer n for the number of line segments of the entry. Then receives n pairs of points, that is, pairs of numbers representing the x and y coordinates of each point. Example:\
3\
10 20 30 40\
20 10 40 30\
15 50 40 10

Prints the number of intersections between the line segments in the entry then the segments that intersect with each other. Example:\
2\
10 20 30 40 15 50 40 10\
20 10 40 30 15 50 40 10

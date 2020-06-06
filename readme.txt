This C program identifies whether a given history is serializable.

How to compile the program:
- Go to Terminal
- gcc -o Serializability Serializability.c
- ./Serializability

Input format:
- Input is entered through the console.
- It is case sensitive.
- Input synthesizing is not included.
- Example:
> w
> 1
> x
> y (to continue)
> r 
> 2 
> x 
> y (to continue)
> w 
> 2 
> y 
> y (to continue)
> r 
> 3 
> y 
> y (to continue)
> w 
> 3 
> z 
> y (to continue)
> r 
> 1 
> z
> n (to stop)
< The graph is cyclic. This history is not serializable!! 

Output:
- Output is displayed in the stdout
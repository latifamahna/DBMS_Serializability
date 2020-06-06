//
//  main.c
//  Serializability
//
//  Created by Latifa Mahna on 22/04/2016.
//  Copyright (c) 2016 macbook pro. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
/* Structure of an operation*/
struct operation {
    char readORwrite;   //'r' or 'w'
    int trNum;         //transaction number
    char memVar;      //memory location
};
/* A node of the graph */
struct Transaction {
    int transactionNumber;
    struct Transaction* next;
};
/* Adjacency list of connected nodes */
struct History{
    int number_of_transactions; //number of transactions in a list
    struct Transaction *head;
};
/* Graph of adjacency lists arrays*/
struct Graph{
    int V;  //number of nodes
    struct History* array;
};
/* Function to create a new transaction node */
struct Transaction* newTransaction(int transactionNumber){
    struct Transaction* newNode = (struct Transaction*) malloc(sizeof(struct Transaction));
    newNode->transactionNumber = transactionNumber;
    newNode->next = NULL;
    //printf("Transaction created. ");
    return newNode;
}
/* Function to create the original graph */
struct Graph* createGraph(int V){
    int i;
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph*));
    graph->V = V;
    /* Create an array of adjacency lists */
    graph->array = (struct History*) malloc(V * sizeof(struct History));
    
    for(i = 0; i < V; i++){
        /* Initialization */
        graph->array[i].head = NULL;
        graph->array[i].number_of_transactions = 0;
    }
    return graph;
}
/* Function to print the adjacenncy list of graph (used for testing) */
/*void printGraph(struct Graph* graph)
{
    int i;
    for (i = 0; i < graph->V; i++)
    {
        struct Transaction* listPtr = graph->array[i].head;
        printf("\n%d: ", i);
        while (listPtr)
        {
            printf("%d->", listPtr->transactionNumber);
            listPtr = listPtr->next;
        }
        printf("NULL\n");
    }
}*/
/* Function to add an edge to the graph from src to dest */
void addEdge(struct Graph* graph, int src, int dest)
{
    struct Transaction* newNode = newTransaction(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
    graph->array[src].number_of_transactions++;
}
/* Function to check for if the graph is cyclic or acyclic */
int isCyclicChecker(struct Graph* graph, int v, int visited[], int *stack){
    if(visited[v] == 0){
        visited[v] = 1; //flag the current node as visited
        stack[v] = 1;  //flag the current node as part of our recursion stack
        
        /* Check for all the vertices adjacent to this vertex */
        struct Transaction* i;
        for(i = graph->array[v].head; i != NULL; i = graph->array[v].head->next){
            if ( visited[i->transactionNumber]==0 && isCyclicChecker(graph, i->transactionNumber, visited, stack)==1 )
                return 1;
            else if (stack[i->transactionNumber])
                return 1;
        }
        
    }
    stack[v] = 0;  //remove the node from our recursion stack
    return 0;
}
/* Function that initiates the recursion for every adjacent list array to checks if graph contains a cycle */
int isCyclic(struct Graph* graph){
    int visited[100];
    int stack[100];
    for(int i = 0; i < 100; i++){
        visited[i] = 0;   //flag the current node as not visited
        stack[i] = 0;    //flag the current node as non part of our recursion stack
    }

    /* For every list we call the checker function to detect cycles in our DFS trees */
    for(int i = 0; i < 100; i++)
        if (isCyclicChecker(graph, i, visited, stack)==1)
            return 1;
    
    return 0;
}
int main(int argc, const char * argv[]) {
    char operation;         //input from the user: 'w' or 'r'
    int transactionNumber; //input from the user: transaction number starts from 1 to 100
    char variable;        //input from the user: memory location ('x', 'w', 'z'...)
    char choice = 'y';   //user selection: 'y' to continue entering operations
    
    struct Graph* graph = createGraph(20); //creating the graph + initilization
    struct operation operations[100];     //create an array that contains all entered operations (w1x, r2y..)
    int i;
    int new = 0;
    int created_nodes_from_read[100];
    created_nodes_from_read[0] = 0;
    int created_nodes_from_write[100];
    created_nodes_from_write[0] = 0;
    
    while (choice=='y') {
        /* Get input from user */
        printf("Please enter next operation!\n");
        printf("Type r (for read) or w (for write): ");
        scanf(" %c",&operation);
        printf("Type transaction number: ");
        scanf("%d",&transactionNumber);
        printf("Type memory variable (example: x, y..): ");
        scanf(" %c",&variable);
        
        operations[new].readORwrite = operation;    //enter current operation ('r' or 'w') into the array
        operations[new].trNum = transactionNumber; //enter current transaction number into the array
        operations[new].memVar = variable;        //enter current memory location into the array
        
         /* Check if a conflict exists */
        if (operations[new].readORwrite == 'r') {
            /* Check if the current 'read' operation conflicts with previous 'write' operations */
            for (i=new-1; i>=0; i--) {
                if (operations[i].readORwrite == 'w' && operations[i].memVar == operations[new].memVar && created_nodes_from_read[i] != i) {
                    /* If there is a conflict, insert an edge */
                    created_nodes_from_read[i] = i;
                    addEdge(graph, operations[i].trNum, operations[new].trNum);
                }
            }
        }
        else if (operations[new].readORwrite == 'w') {
            /* Check if the current 'write' operation conflicts with previous operations ('read' or 'write') */
            for (i=new-1; i>=0; i--) {
                if (operations[i].memVar == operations[new].memVar && created_nodes_from_write[i] != i) {
                    /* If there is a conflict, insert an edge */
                    created_nodes_from_write[i] = i;
                    addEdge(graph, operations[i].trNum, operations[new].trNum);
                }
            }
        }
        new++;
        printf("Continue? ");
        scanf(" %c",&choice);
    }

    //printGraph(graph);
    if(isCyclic(graph)==1)
        printf("The graph is cyclic. This history is not serializable!!");
    else
        printf("The graph is acyclic. This history is serializable.");
    
    return 0;
}

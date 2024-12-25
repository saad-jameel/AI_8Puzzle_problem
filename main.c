

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include "state.h"
#include "list.h"
#include "node.h"
#include "io.h"

unsigned int nodesExpanded;  //expanded nodes
unsigned int nodesGenerated; //generated nodes
unsigned int solutionLength; //number of moves 
double runtime;              //elapsed time 

SolutionPath* AStar_search(State *, State *);

int main(void) {
    welcomeUser();          
    printInstructions();     

    State initial;           
    State goalState;         
    
    SolutionPath *aStar;

    printf("PEASE ENTER INITIAL STATE:\n");
    inputState(&initial);

    printf("\n PEASE ENTER GOAL STATE:\n");
    inputState(&goalState);

    printf(" ENTERED INITIAL STATE:\n");
    printBoard(initial.board);

    printf(" ENTERED GOAL STATE:\n");
    printBoard(goalState.board);

    aStar = AStar_search(&initial, &goalState);
    printf("\n-------------------------- A* ALGORITHM --------------------------\n");
    printSolution(aStar);

    nodesExpanded = 0;
    nodesGenerated = 0;
    solutionLength = 0;
    runtime = 0;

    destroySolution(&aStar);

    return 0;
}

SolutionPath* AStar_search(State *initial, State *goal) {
    NodeList *queue = NULL;
    NodeList *children = NULL;
    Node *node = NULL;

    clock_t start = clock();

    pushNode(createNode(0, manhattanDist(initial, goal), initial, NULL), &queue);
    Node *root = queue->head->currNode; //for deallocating generated tree

    while(queue->nodeCount > 0) {
        node = popNode(&queue);

        if(statesMatch(node->state, goal))
            break;

        children = getChildren(node, goal);
        ++nodesExpanded;

        pushListInOrder(&children, queue);
    }

    runtime = (double)(clock() - start) / CLOCKS_PER_SEC;

    SolutionPath *pathHead = NULL;
    SolutionPath *newPathNode = NULL;

    while(node) {
        newPathNode = malloc(sizeof(SolutionPath));
        newPathNode->action = node->state->action;
        newPathNode->next = pathHead;
        pathHead = newPathNode;


        ++solutionLength;
        node = node->parent;
    }
    --solutionLength; //uncount the root node

    destroyTree(root);

    return pathHead;
}
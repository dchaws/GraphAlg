// Copyright (C) 2011 David C. Haws

//This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

// See LICENSE

// David Haws
// www.davidhaws.net
// https://github.com/dchaws

// David Haws 6/2/11
// Performs Tarjons algorithm. See 
// http://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm
//
// adjMatrix is a numNodes x numNodes adjacency matrix for the graph G we are 
// considering.

// This also performs a bunch of other graph algorithms.

#ifndef GRAPHALG_H
#define GRAPHALG_H 1

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <list>
#include <vector>
#include <set>
#include <ext/hash_set>

using namespace __gnu_cxx;

using namespace std;

// This uses DFS to find path from node a to node b.
// If directed = 1, then the path is directed. directed = 0 means only find undirected paths
list <int> getPath(int **adjMat, int numNodes, int a, int b, int directed);

// Internal DFS function for getPath.
// Returns true if b is in one of its decendents.
int getPathDFS(int **adjMat, int numNodes, int curNode, int endNode, list <int> &curPath, set <int> &markedNodes, int directed);

// This uses DFS to find all paths from node a to node b.
// If directed = 1, then the path is directed. directed = 0 means only find undirected paths
list <list <int> > getAllPaths(int **adjMat, int numNodes, int a, int b, int directed);

// Internal DFS function for getPath.
// Returns true if b is in one of its decendents.
// This will keep searching for all paths
int getAllPathsDFS(int **adjMat, int numNodes, int curNode, int endNode, list <int> &curPath, list <list <int> > &allPaths, set <int> &markedNodes, int directed);


// This will take as input an adjacency matrix for a directed graph with numNodes nodes.
// It will first run tarjan to decompose the graph into strongly connected components. 
// Then it will compute a spanning tree of each strongly connected component and also
// save the edges not in the DFS. All such edges will define a minimum cycle.
// If there is an edge i -> j and j -> i, that is ok. That is, this works on graphs with
// directed and undirected edges. Currently though, if i -> j and j -> i, this function
// reports it as a cycle. The user can ignore cycles of length two if they like.
// However, if (i,j) is marked as a "back node" then it won't be reported currently as
// a cycle of length 2. But, who cares for our application.
list < list <int> > getMinCycles(int **adjMat,int numNodes);


// This returns a list of list of strongly connected components.
list <set <int> > tarjan(int **adjMatrix, int numNodes);

// Recursive function that does the hard work. Performs DFS and records extra information.
void strongConnect(int **adjMatrix, int *nodeIndex, int *nodeLowlink, int numNodes, int curNode, int &index, list <int> &myQueue, set <int> &myQueueContents, list < set <int> > &strongConComps);

// This takes in a direct graph in adjMat. It fills in spanTreeAndBackEdges with a
// spanning tree (given by 1's) and back edges (-1's). Each back edge defines
// a cycle. Assumes spanTreeAndBackEdges is already allocated.
void spanTreeWithBackEdges (int **adjMat, int **spanTreeAndBackEdges, int numNodes);

// Actual recursive DFS function to compute the spanning tree and back edges
void STWBE_DFS (int **adjMat, int **spanTreeAndBackEdges,  int *visited, int numNodes, int curNode);


// Assumes both adjMat and subAdjMat are allocated.
// This fill subAdjMat with the values of adjMat indexed by mySet
void getSubAdjMat (int **adjMat, int **subAdjMat, int numNodes, set <int> mySet);

// Allocated adjacency matrix
int **allocateAdjMat(int numNodes);

// Deletes adjacency matrix
void deleteAdjMat(int **adjMat,int numNodes);

// This will set -1 in adjMat to 0 and copy to newAdjMat
void stripNeg(int **adjMat, int **newAdjMat, int numNodes);

// Functions to print the values in the list or set
void printListInt (list <int> &myList);
void printSetInt (set <int> &mySet);

void printMat (int **adjMat, int numNodes);

// This assumes adjMat is a spanning tree. This returns the ordered path
// from startNode to endNode if it exists.
list <int> pathInSpanningTree(int **adjMat, int numNodes, int startNode, int endNode);

// This does DFS on the spanning tree adjMath. 
// returns true if one of its children found endNode
int pathInSpanningTreeDFS(int **adjMat, int numNodes, int curNode, int endNode, list <int> &path);

// This reads through the adjacency matrix and looks for -1.
// If a -1 is found the edge i -> j is recorded.
list <vector <int> > getBackEdges(int **adjMat, int numNodes);

// Allocates a new matrix and fills it with a copy of the one given.
int **copyAdjMat(int **adjMat, int numNodes);

// Use the orientation rules from "Causal inference and causal explanation with background knowledge"
// Meek 1995. Also in "Characteristic imsets for learning Bayesian network structures", Hemmecke et. al.
int **essGraphFromPatGraph(int **patMat, int numNodes);

// Returns true if x is in myList.
int isInList(int x, list <int> myList);


// Compares the two lists and returns 1 if they are equal.
int listsEqual (list <int> listOne,list <int> listTwo);

// Returns a string of the list.
// E.g. [1 3 2 5] would give "1,3,2,5"
// Up to user to delete the string
char * listToString(list <int> myList);

// This assumes myList is a cycle and returns the
// representation with the minimum node number as the start.
list <int> uniqueCycle(list <int> myList);

// Reverses the list myList
list <int> revList(list <int> myList);

// Some data structures for making a hash table on lists
// 
struct ltlistint
{
    bool operator()(const list <int> listOne, const list <int> listTwo) const
    {
        if(listOne.size() == 0 && listTwo.size() == 0)
        {
            return 0;
        }

        if (listOne.size() < listTwo.size())
        {
            return 1;
        }
        else if (listOne.size() > listTwo.size())
        {
            return 0;
        }
        else if (listOne.size() == listTwo.size())
        {
            // Now must check if listOne is less than listTwo in lex order.
            list <int>::const_iterator litOne,litTwo;

            litOne = listOne.begin();
            litTwo = listTwo.begin();

            while (litOne != listOne.end() && litTwo != listTwo.end())
            {
                if (*litOne > *litTwo)
                {
                    return 0;
                }
                else if (*litOne < *litTwo)
                {
                    return 1;
                }
                litOne++;
                litTwo++;
            }
            return 0;
        }
    }
};

// For a hash map of lists 
struct eqListInt
{
    bool operator()(const list <int> listOne, const list <int> listTwo) const
    {
        return listsEqual (listOne,listTwo);
    }
};

// Computes the has of a set of ints by converting to a string then hashing.
struct hashListInt
{
    size_t operator()(const list <int> myList) const
    {
        hash <const char *> H;
        size_t retVal;
        char *subString = listToString(myList);
        retVal = H(subString);
        delete [] subString;
        return retVal;
    }
};

// Returns 1 if the graphs are the same
int graphSame(int **adjMatOne, int **adjMatTwo, int numNodes);

// This is a class which will output cycles in increasing size.
// Uses the algorithm in "A new way to enumerate cycles in graph"
// by Hongbo Liu and Jiaxin Wang.
// I decided to use a class because I wanted to be able to ask 
// for cycles one at a time.
// Assume adjMat is strongly connected. Actually, it doesn't matter if
// it is strongly connected or not. That paths and therefore cycles
// are limited by the connectivity of the graph anyways.
// This class deletes its allocated memory when done. Note that
// it only *copies* the adjMat it is given. Thus, be sure to 
// delete the copy you gave this class.
// Once initialized, the main function is nextCycle. It will return 
// the next cycle starting with cardinality 2. If there are no more 
// cycles to return it returns an empty list.
// Note if all the edges of a cycle are bidirected, then this
// will return the same cycle, but with the opposite orientation.
class cycleGenerator
{
    public:
        cycleGenerator(); //Constructor
        ~cycleGenerator(); //Destructor
        cycleGenerator(int **someAdjMat,int someNumNodes);
        void print (); // Prints the adj matrix,
        void copyAdjMatInt(int **someAdjMat,int someNumNodes);// Copies someAdjMat
        void clear (); // Clears all data structures, including the adjMat.

        int oneStep (); // Runs one step of the algorithm. Returns 1 if successful

        list <int> nextCycle(); // This returns the next cycle. It should list cycles
                                // by cardinality. If cyclesFound is empty, it will call
                                // oneStep to try and generate more.
        int numOneStepCall();   // Returns the number of times oneStep has been called.
        void setHashBiDirectedCycles(int x); // Sets hashBiDirectedCycles to x
        int isCycleBiDirected(list <int> myCycle); // Returns 1 if the reverse is also a cycle.
        list <int> uniqueBiDirCycle(list <int> myList); // Returns the uniqe rep of bi direct cycle
                                                        // with min as start and second el min.
        void printUniqBiDirectedCycles(); // Prints out the list of unique bi-directed cycles
        int sizeUniqBiDirectedCycles(); // Returns the current size of uniqBiDirectedCycles
        void setCycleLengthMin(int x); // Sets cycleLengthMin
    private:
        void init();
        int **adjMat;
        int numNodes;
        void addVertToPathQueue();
        int oneStepCalls;
        int cycleLengthMin; // The minimum length of cycles to return. Default is 1
        int hashBiDirectedCycles; // 1 means save biDirected cycles. This is so we can print out
                                  // unique cycles. Default is no

        list < list <int> > pathQueue;
        list < list <int> > cyclesFound; // We store cycles as a list of the nodes encountered. We do not repeat head.
                                         // Implicit that it is a cycle.
        hash_set< list <int> , hashListInt , eqListInt> uniqBiDirectedCycles; // This will save the bi-directed cycles.
                                                                          // if hashBiDirectedCycles is 1.

};

#endif


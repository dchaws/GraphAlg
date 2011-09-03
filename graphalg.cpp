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

// Performs other algorithms (DFS) to get the minimum cycles.

// This also performs a bunch of other graph algorithms.


#include "graphalg.h"

using namespace std;

// This uses DFS to find a path from node a to node b.
// This only finds *one* path between the nodes.
list <int> getPath(int **adjMat, int numNodes, int a, int b, int directed)
{
    list <int> retPath;
    set <int> markedNodes;
    if (a == b)
    {
        return retPath;
    }

    if (getPathDFS(adjMat,numNodes,a,b,retPath,markedNodes, directed) == 1)
    {
        retPath.push_front(a);
    }

    return retPath;
}

// Internal DFS function for getPath.
// Returns true if b is in one of its decendents.
int getPathDFS(int **adjMat, int numNodes, int curNode, int endNode, list <int> &curPath, set <int> &markedNodes, int directed)
{
    markedNodes.insert(curNode); // Mark the node for proper DFS
    if (curNode == endNode)
    {
        return 1;
    }
    int foundEndNode = 0;;
    for (int i=0;i<numNodes;i++)
    {
        // Only recurse if edge and i not marked
        if ( adjMat[curNode][i] == 1 && (adjMat[i][curNode] == 1 || directed == 1) && markedNodes.find(i) == markedNodes.end()) 
        {
            // Recurse
            if (getPathDFS(adjMat,numNodes,i,endNode,curPath,markedNodes,directed) == 1)
            {
                curPath.push_front(i); // if returns 1, then decendent can get to endNode
                foundEndNode = 1;
            }
        }
    }

    return foundEndNode;
}

list <list <int> > getAllPaths(int **adjMat, int numNodes, int a, int b, int directed)
{
    list <list <int> > retPaths;
    list <int> curPath;
    set <int> markedNodes;
    if (a == b)
    {
        curPath.push_back(a);
        retPaths.push_back(curPath);
        return retPaths;
    }
    curPath.push_front(a);
    getAllPathsDFS(adjMat,numNodes,a,b,curPath,retPaths,markedNodes,directed);

    return retPaths;
}

// Internal DFS function for getPath.
// Returns true if b is in one of its decendents.
// This will keep searching for all paths
int getAllPathsDFS(int **adjMat, int numNodes, int curNode, int endNode, list <int> &curPath, list <list <int> > &allPaths, set <int> &markedNodes, int directed)
{
    //cout << "[getAllPathsDFS] curNode: " << curNode << " markedNodes: ";
    //set <int>::iterator sit;
    //for (sit=markedNodes.begin();sit!=markedNodes.end();sit++)
    //{
    //    cout << *sit << " ";
    //}
    //cout << endl;

    markedNodes.insert(curNode); // Mark the node for proper DFS
    if (curNode == endNode)
    {
        //cout << "Adding path!" << endl;
        allPaths.push_back(curPath);
        markedNodes.erase(markedNodes.find(curNode));
        return 1;
    }
    for (int i=0;i<numNodes;i++)
    {
        // Only recurse if edge and i not marked
        // only recurse on undirected edges!
        if ( adjMat[curNode][i] == 1 && (adjMat[i][curNode] == 1 || directed == 1) && markedNodes.find(i) == markedNodes.end()) 
        {
            // Recurse
            curPath.push_back(i); // if returns 1, then decendent can get to endNode
            getAllPathsDFS(adjMat,numNodes,i,endNode,curPath,allPaths,markedNodes,directed);
            curPath.pop_back(); // Remove the node.
        }
    }

    // Remove the mark.
    markedNodes.erase(markedNodes.find(curNode));
}



// This returns a list of list of strongly connected components.
list <set <int> > tarjan (int **adjMatrix, int numNodes)
{
    // Allocate matrix for the index of each node.
    // Allocate matrix for the lowlink of each node.
    int *nodeIndex   = new int[numNodes];
    int *nodeLowlink = new int[numNodes];
    for (int i=0;i<numNodes;i++)
    {
        // Initialize the values
        nodeIndex[i] = -1; //-1 means undef
        nodeLowlink[i] = -1; //-1 means undef
    }
    
    list <set <int> > strongConComps; // This will store the strongly connected components we find.

    int index=0;
    list <int> myQueue;
    set <int> myQueueContents;
    for (int i=0;i<numNodes;i++)
    {
        if (nodeIndex[i] == -1)
        {
            strongConnect(adjMatrix, nodeIndex, nodeLowlink, numNodes, i, index, myQueue, myQueueContents, strongConComps);
        }
    }

    // Delete matrix for the index of each node.
    // Delete matrix for the lowlink of each node.
    delete [] nodeIndex;
    delete [] nodeLowlink;

    return strongConComps;

}

void strongConnect(int **adjMatrix, int *nodeIndex, int *nodeLowlink, int numNodes, int curNode, int &index, list <int> &myQueue, set <int> &myQueueContents, list < set <int> > &strongConComps)
{
    //cout << "[strongConnect] index:" << index << " curNode: " << curNode << endl;
    //cout << "   nodeIndex: ";
    //for (int i=0;i<numNodes;i++)
    //{
    //    cout << nodeIndex[i] << " ";
    //}
    //cout << endl;
    //cout << "   nodeLowlink: ";
    //for (int i=0;i<numNodes;i++)
    //{
    //    cout << nodeLowlink[i] << " ";
    //}
    //cout << endl;
    //
    //cout << "    myQueue:" << endl;
    //printListInt(myQueue);
    //cout << "    myQueueContents:" << endl;
    //printSetInt(myQueueContents);

    nodeIndex[curNode] = index; 
    nodeLowlink[curNode] = index;
    index++;

    myQueue.push_front(curNode);
    myQueueContents.insert(curNode);

    // Now loop over all nodes adjacent to curNode
    for (int i=0;i<numNodes;i++)
    {
        if (adjMatrix[curNode][i] == 1 && i != curNode)
        {
            if (nodeIndex[i] == -1) // undef
            {
                // Recurse on i
                //cout << "    recursing..." << endl;
                strongConnect(adjMatrix,nodeIndex,nodeLowlink,numNodes,i,index,myQueue,myQueueContents,strongConComps);
                nodeLowlink[curNode] = min(nodeLowlink[curNode],nodeLowlink[i]);
            }
            else if (myQueueContents.find(i) != myQueueContents.end())
            {
                nodeLowlink[curNode] = min(nodeLowlink[curNode],nodeIndex[i]);
            }
        }
    }

    // If these are equal then this is a strongly connected component.
    // Pop everything off and put into a new strongly connected component.
    if (nodeLowlink[curNode] == nodeIndex[curNode])
    {
        //cout << "nodeLowlink[" << curNode << "] == nodeIndex[" << curNode << "]" << endl;
        set <int> newStrongConComp;
        int tempInt;
        while (*(myQueue.begin()) != curNode)
        {
            int tempInt = *(myQueue.begin());
            newStrongConComp.insert(tempInt);
            myQueue.pop_front();
            myQueueContents.erase(myQueueContents.find(tempInt));
        }

        // Be sure to add curNode
        tempInt = *(myQueue.begin());
        newStrongConComp.insert(tempInt);
        myQueue.pop_front();
        myQueueContents.erase(myQueueContents.find(tempInt));


        strongConComps.push_back(newStrongConComp);
        //cout << "    newStrongConComp: ";
        //printSetInt(newStrongConComp);
    }

}


// This takes in a direct graph in adjMat. It fills in spanTreeAndBackEdges with a
// spanning tree (given by 1's) and back edges (-1's). Each back edge defines
// a cycle. Assumes spanTreeAndBackEdges is already allocated.
// Assumes the graph is connected.
void spanTreeWithBackEdges (int **adjMat, int **spanTreeAndBackEdges, int numNodes)
{
    int *visited = new int[numNodes];
    for (int i=0;i<numNodes;i++)
    {
        visited[i] = 0; // 0 means not visiteded. 1 means visited.
    }

    // Zero out the data structure 
    for (int i=0;i<numNodes;i++)
    {
        for (int j=0;j<numNodes;j++)
        {
            spanTreeAndBackEdges[i][j] = 0;
        }
    }

    // Find a node that has an edge.
    int startNode;
    for (int i=0;i<numNodes;i++)
    {
        for (int j=0;j<numNodes;j++)
        {
            if (adjMat[i][j] == 1)
            {
                startNode = i;
                i=numNodes; // Quit for loops
                j=numNodes;
            }
        }
    }
    STWBE_DFS(adjMat,spanTreeAndBackEdges,visited,numNodes, startNode);
    delete [] visited;  
}

// Actual recursive DFS function to compute the spanning tree and back edges
void STWBE_DFS (int **adjMat, int **spanTreeAndBackEdges,  int *visited, int numNodes, int curNode)
{
    visited[curNode] = 1; // Mark as visited

    for (int i=0;i<numNodes;i++)
    {
        if (adjMat[curNode][i] == 1 && i != curNode)
        {
            if (visited[i] == 0) // Not visited
            {
                // Mark curNode -> i as an edge in the spanning tree
                spanTreeAndBackEdges[curNode][i] = 1;
                // Recurse on i
                //cout << "    recursing..." << endl;
                STWBE_DFS(adjMat,spanTreeAndBackEdges,visited,numNodes,i);
            }
            else 
            {
                // It is already visited. Mark it as a back edge.
                spanTreeAndBackEdges[curNode][i] = -1;
            }
        }
    }

}

// Assumes both adjMat and subAdjMat are allocated.
// This fill subAdjMat with the values of adjMat indexed by mySet
void getSubAdjMat (int **adjMat, int **subAdjMat, int numNodes, set <int> mySet)
{
    for (int i=0;i<numNodes;i++)
    {
        for (int j=0;j<numNodes;j++)
        {
            if (mySet.find(i)!=mySet.end() && mySet.find(j) != mySet.end())
            {
                subAdjMat[i][j] = adjMat[i][j];
            }
            else 
            {
                subAdjMat[i][j] = 0;
            }
        }
    }
}

// Allocated adjacency matrix
int **allocateAdjMat(int numNodes)
{
    int **adjMat = new int *[numNodes];

    for (int i=0;i<numNodes;i++)
    {
        adjMat[i] = new int[numNodes];
        for (int j=0;j<numNodes;j++)
        {
            adjMat[i][j] = 0; //Lets be nice and set to 0
        }
    }
    return adjMat;
}

// Deletes adjacency matrix
void deleteAdjMat(int **adjMat,int numNodes)
{
    for (int i=0;i<numNodes;i++)
    {
        delete [] adjMat[i];
    }
    delete [] adjMat;
}

// This will set -1 in adjMat to 0
void stripNeg(int **adjMat, int **newAdjMat, int numNodes)
{
    for (int i=0;i<numNodes;i++)
    {
        for (int j=0;j<numNodes;j++)
        {
            if (adjMat[i][j] == -1)
            {
                newAdjMat[i][j] = 0;
            }
            else 
            {
                newAdjMat[i][j] = adjMat[i][j];
            }
        }
    }
}

void printMat (int **adjMat, int numNodes)
{
    for (int i=0;i<numNodes;i++)
    {
        for (int j=0;j<numNodes;j++)
        {
            cout << setw(2) << adjMat[i][j] << " ";
        }
        cout << endl;
    }
}

void printListInt (list <int> &myList)
{
    list <int>::iterator lit;
    for (lit=myList.begin();lit!=myList.end();lit++)
    {
        cout << *lit << " " ;
    }
    cout << endl;
}

void printSetInt (set <int> &mySet)
{
    set <int>::iterator sit;
    for (sit=mySet.begin();sit!=mySet.end();sit++)
    {
        cout << *sit << " " ;
    }
    cout << endl;
}

// This assumes adjMat is a spanning tree. This returns the ordered path
// from startNode to endNode if it exists.
list <int> pathInSpanningTree(int **adjMat, int numNodes, int startNode, int endNode)
{
    list <int> path;
    if (pathInSpanningTreeDFS(adjMat,numNodes,startNode,endNode,path) == 1)
    {
        path.push_front(startNode);
    }
    return path;

}

// This does DFS on the spanning tree adjMath. 
// returns true if one of its children found endNode
int pathInSpanningTreeDFS(int **adjMat, int numNodes, int curNode, int endNode, list <int> &path)
{
    //cout << "[pathInSpanningTreeDFS] curNode: " << curNode << " path: ";
    //printListInt(path);
    if (curNode == endNode)
    {
        return 1;
    }

    for (int i=0;i<numNodes;i++)
    {
        if (adjMat[curNode][i] == 1 && i != curNode)
        {
            //recurse
            if (pathInSpanningTreeDFS(adjMat, numNodes, i, endNode,path) == 1)
            {
                //cout << " curNode: " << curNode << " i:" << i << "  found endNode. " << endl;
                // Then i is in the path
                path.push_front(i);
                return 1;
            }
        }
    }
    return 0;
}


// This reads through the adjacency matrix and looks for -1.
// If a -1 is found the edge i -> j is recorded.
list <vector <int> > getBackEdges(int **adjMat, int numNodes)
{
    list <vector <int> > retList;
    for (int i=0;i<numNodes;i++)
    {
        for (int j=0;j<numNodes;j++)
        {
            if (adjMat[i][j] == -1)
            {
                vector <int> backEdge;
                backEdge.clear();
                backEdge.insert(backEdge.begin(),j);
                backEdge.insert(backEdge.begin(),i);
                retList.push_back(backEdge);
            }
        }
    }
    return retList;
}

list <list <int> > getMinCycles(int **adjMat,int numNodes)
{
    int **tempAdjMat =allocateAdjMat(numNodes);
    int **spanTreeBackEdges = allocateAdjMat(numNodes);
    int **spanTree = allocateAdjMat(numNodes);
    list <list <int> > minCycles;

    //printMat(adjMat,numNodes);

    list <set <int> > strongConComps = tarjan(adjMat, numNodes);

    //cout << "Strongly connected components." << endl;
    //cout << "strongConComps.size() = " << strongConComps.size() << endl;

    list <set <int> >::iterator lsit;

    for (lsit= strongConComps.begin();lsit!=strongConComps.end();lsit++)
    {
        ////cout << "(*lsit).size() = " << (*lsit).size() << endl;
        set <int>::iterator sit;
        for (sit = (*lsit).begin();sit!=(*lsit).end();sit++)
        {
            //cout << *sit << " ";
        }
        //cout << endl;
    }

    //cout << "Calculating spanning tree (with back edges) of each strongly connected component." << endl;
    for (lsit= strongConComps.begin();lsit!=strongConComps.end();lsit++)
    {
        ////cout << "(*lsit).size() = " << (*lsit).size() << endl;
        set <int>::iterator sit;
        for (sit = (*lsit).begin();sit!=(*lsit).end();sit++)
        {
            //cout << *sit << " ";
        }
        //cout << endl;
        if ((*lsit).size() > 1)
        {

            getSubAdjMat (adjMat, tempAdjMat, numNodes, (*lsit));
            //printMat (tempAdjMat, numNodes);

            spanTreeWithBackEdges (tempAdjMat, spanTreeBackEdges, numNodes);
            //cout << "Spanning tree with back edges." << endl;
            //printMat (spanTreeBackEdges,numNodes);
            //cout << "Spanning tree." << endl;
            stripNeg(spanTreeBackEdges,spanTree, numNodes);
            //printMat (spanTree,numNodes);
            //cout << "Minimum cycles:" << endl;

            list <vector <int> > backEdges = getBackEdges(spanTreeBackEdges, numNodes);
            list <vector <int> >::iterator llit;
            for (llit=backEdges.begin();llit!=backEdges.end();llit++)
            {
                //cout << "Back edge: " << (*llit)[0] << " " << (*llit)[1] << endl;

                list <int> path = pathInSpanningTree(spanTree, numNodes, (*llit)[1], (*llit)[0] );
                //cout << "Path: " ;
                //printListInt(path);
                minCycles.push_back(path);
            }
            

            //cout << endl;
        }
        else
        {
            //cout << "Only one element. Skipping. " << endl;
        }

    }

    deleteAdjMat(tempAdjMat,numNodes);
    deleteAdjMat(spanTreeBackEdges,numNodes);
    deleteAdjMat(spanTree,numNodes);

    return minCycles;
}


// Allocates a new matrix and fills it with a copy of the one given.
int **copyAdjMat(int **adjMat, int numNodes)
{
    int **retMat = allocateAdjMat(numNodes);
    for (int i=0;i<numNodes;i++)
    {
        for (int j=0;j<numNodes;j++)
        {
            retMat[i][j] = adjMat[i][j];
        }
    }
    return retMat;
}


// Use the orientation rules from "Causal inference and causal explanation with background knowledge"
// Meek 1995. Also in "Characteristic imsets for learning Bayesian network structures", Hemmecke et. al.
// Given a CIM solution which may not be from a DAG, I think it is not correct to use the 
// "orientation rules". That is, the orientation rules assume the graph which generated the CIM was a DAG!
int **essGraphFromPatGraph(int **patMat, int numNodes)
{
    int **essMat = copyAdjMat(patMat, numNodes);

    int ruleApplied = 1;
    while (ruleApplied == 1)
    {
        ruleApplied = 0;

        //
    }
}

// Reverses the list myList
list <int> revList(list <int> myList)
{
    list <int> retList;
    if (myList.size() <= 1)
    {
        return myList;
    }

    list <int>::iterator lit = myList.end();
    lit--;
    for (;lit!=myList.begin();lit--)
    {
        retList.push_back(*lit);
    }
    retList.push_back(*lit);
    return retList;
}

// This assumes myList is a cycle and returns the
// representation with the minimum node number as the start.
list <int> uniqueCycle(list <int> myList)
{
    list <int> retList;
    if (myList.size() <= 1)
    {
        return myList;
    }

    list <int>::iterator minVal;
    list <int>::iterator lit=myList.begin();
    int curMin = *lit;
    minVal = lit;
    // Find the min value;
    for (;lit!=myList.end();lit++)
    {
        if (*lit < curMin )
        {
            minVal = lit;
            curMin = *lit;
        }
    }

    lit = minVal;
    for (int i=0;i<myList.size();i++)
    {
        if (lit == myList.end())
        {
            lit = myList.begin();
        }
        retList.push_back(*lit);
        lit++;
    }
    return retList;
}

// Returns 1 if the graphs are the same
int graphSame(int **adjMatOne, int **adjMatTwo, int numNodes)
{
    int same = 1;
    for (int i=0;i<numNodes;i++)
    {
        for (int j=0;j<numNodes;j++)
        {
            if (adjMatOne[i][j] != adjMatTwo[i][j])
            {
                same = 0;
                i = numNodes;
                j = numNodes;
            }
        }
    }
    return same;
}

cycleGenerator::cycleGenerator()
{
    init();
}

cycleGenerator::~cycleGenerator()
{
    if (numNodes != 0)
    {
        deleteAdjMat(adjMat,numNodes);
    }
}

cycleGenerator::cycleGenerator(int **someAdjMat,int someNumNodes)
{
    init();
    copyAdjMatInt(someAdjMat,someNumNodes);
}

void cycleGenerator::copyAdjMatInt(int **someAdjMat,int someNumNodes)
{
    if (numNodes != 0)
    {
        // Delete previous adjMat
        deleteAdjMat(adjMat,numNodes);
        numNodes = 0;
    }

    adjMat = copyAdjMat(someAdjMat, someNumNodes);
    numNodes = someNumNodes;

    addVertToPathQueue();
}

void cycleGenerator::init()
{
    hashBiDirectedCycles=0;
    adjMat = NULL;
    numNodes = 0;
    oneStepCalls=0;
    cycleLengthMin=1;
}


void cycleGenerator::print () // Prints the adj matrix,
{
    if (numNodes != 0)
    {
        printMat(adjMat,numNodes);
    }
}

void cycleGenerator::clear ()
{
    if (numNodes != 0)
    {
        // Delete previous adjMat
        deleteAdjMat(adjMat,numNodes);
        numNodes = 0;
    }
    pathQueue.clear();
    cyclesFound.clear();
    uniqBiDirectedCycles.clear();
    oneStepCalls=0;
}

void cycleGenerator::addVertToPathQueue()
{
    list <int> newList;
    for (int i=0;i<numNodes;i++)
    {
        newList.clear();
        newList.push_back(i);
        pathQueue.push_back(newList);
    }
}

int cycleGenerator::oneStep ()
{
    //cout << "[cycleGenerator::oneStep] pathQueue.size() = " << pathQueue.size();
    //cout << " cyclesFound.size() = " << cyclesFound.size() << endl;
    oneStepCalls++;
    list <int> curPath;

    if (pathQueue.size() != 0)
    {
        curPath = *(pathQueue.begin());
        pathQueue.pop_front();
        int k = curPath.size();

        int head = *(curPath.begin());
        list <int>::iterator lit;

        lit = curPath.end();
        lit--;
        int tail = *lit; // This could equal head

        //cout << "k = " << k << " head = " << head << " tail = " << tail << endl;;

        // Search for cycles using this path.
        if (k > 1)
        {
            if (adjMat[tail][head] == 1)
            {
                //cout << "    Found Cycle." << endl;
                // There exists a path. Add it to the cycle queue
                if (hashBiDirectedCycles == 1 && isCycleBiDirected(curPath) == 1 && curPath.size() > 2)
                {
                    // If we are hashing bidirected cycles, add it to hash
                    list <int> uniqCycle = uniqueBiDirCycle(curPath);
                    //cout << "curPath: ";
                    //printListInt(curPath);
                    //cout << "uniqCycle: ";
                    //printListInt(uniqCycle);
                    //cout << "uniqBiDirectedCycles.size() = " << uniqBiDirectedCycles.size() << endl;
                    //printUniqBiDirectedCycles();
                    if(uniqBiDirectedCycles.find(uniqCycle) == uniqBiDirectedCycles.end())
                    {
                        //cout << "here" << endl;
                        // Add it to the hash and the cyclesFound
                        if (cycleLengthMin <= curPath.size())
                        {
                            cyclesFound.push_back(curPath);
                        }
                        uniqBiDirectedCycles.insert(uniqCycle);
                    }
                    // else a representative of this cycle is already in our list.
                }
                else {
                    if (cycleLengthMin <= curPath.size())
                    {
                        cyclesFound.push_back(curPath);
                    }
                }
            }
        }

        // Now try to add all possible paths of length k using curPath
        // where the new vertex is larger than the tail
        for (int i=head+1;i< numNodes;i++)
        {
            //cout << "    adjMat[" << tail << "][" << i << "] = " << adjMat[tail][i] << endl;
            if (adjMat[tail][i] == 1 && i != head && isInList(i,curPath) == 0)
            {
                //cout << "        Add it" << endl;
                list <int> newPath = curPath;
                newPath.push_back(i);
                pathQueue.push_back(newPath);
            }
        }
        return 1;
    }
    
    return 0;
}

list <int> cycleGenerator::nextCycle()
{
    list <int> retList;
    if (numNodes == 0 || pathQueue.size() == 0)
    {
        // either no adj matrix, or we have exhausted all the cycles.
        return retList;
    }
    if (cyclesFound.size() == 0 && numNodes != 0)
    {
        while (cyclesFound.size() == 0 && oneStep() == 1)
        {
        }
        if (cyclesFound.size() == 0 && pathQueue.size() == 0)
        {
            // We have exhausted all the cycles.
            return retList;
        }
        retList = *(cyclesFound.begin());
        cyclesFound.pop_front();
           
        return retList;

    }
}
int cycleGenerator::numOneStepCall()
{
    return oneStepCalls;
}

void cycleGenerator::setHashBiDirectedCycles(int x)
{
    hashBiDirectedCycles = x;
}

int cycleGenerator::isCycleBiDirected(list <int> myCycle)
{
    if (myCycle.size() <= 1 || numNodes == 0)
    {
        return 0;
    }

    list <int>::iterator tempLit;
    list <int>::iterator lit = myCycle.begin();
    tempLit = lit;
    tempLit++;
    for(;lit!=myCycle.end() && tempLit != myCycle.end();lit++)
    {
        //cout << "*lit: " << *lit << " *tempLit: " << *tempLit << endl;
        if(adjMat[*tempLit][*lit] == 0)
        {
            return 0;
        }
        tempLit++;
    }
    // Now lit is the last node
    if (adjMat[*(myCycle.begin())][*lit] == 0)
    {
        return 0;
    }

    return 1;
}


list <int> cycleGenerator::uniqueBiDirCycle(list <int> myList)
{
    //cout << "[cycleGenerator::uniqueBiDirCycle] ";
    //printListInt(myList);

    if (myList.size() <= 1 || isCycleBiDirected(myList) == 0)
    {
        return myList;
    }

    list <int> retList;

    list <int>::iterator minVal;
    list <int>::iterator tempLit;
    list <int>::iterator lit=myList.begin();
    int curMin = *lit;
    minVal = lit;
    // Find the min value;
    for (;lit!=myList.end();lit++)
    {
        if (*lit < curMin )
        {
            minVal = lit;
            curMin = *lit;
        }
    }
    //cout << "Min val: " << *minVal << endl;

    int reverseList = 0;
    int minPlus,minMinus;
    lit = minVal;
    tempLit = minVal;
    tempLit++;
    if(lit == myList.begin())
    {
        lit++;
        minPlus = *lit;
        lit = myList.end();
        lit--;
        minMinus = *lit;
    }
    else if (tempLit == myList.end())
    {
        lit--;
        minMinus = *lit;
        lit = myList.begin();
        minPlus = *lit;
    }
    else
    {
        lit++;
        minPlus = *lit;
        lit--;
        minMinus = *lit;
    }
    //cout << "minPlus: " << minPlus << " minMinus: " << minMinus << endl;

    if (minPlus > minMinus)
    {
        return uniqueCycle(revList(myList));
    }
    else
    {
        return uniqueCycle(myList);
    }
}

int cycleGenerator::sizeUniqBiDirectedCycles()
{
    return uniqBiDirectedCycles.size();
}

void cycleGenerator::printUniqBiDirectedCycles()
{
    hash_set< list <int> , hashListInt , eqListInt>::iterator hlit;

    for (hlit=uniqBiDirectedCycles.begin();hlit!=uniqBiDirectedCycles.end();hlit++)
    {
        list <int> tempList = *hlit;
        printListInt(tempList);
    }

}

void cycleGenerator::setCycleLengthMin(int x)
{
    cycleLengthMin = x;
}

// Returns true if x is in myList.
int isInList(int x, list <int> myList)
{
    list <int>::iterator lit;
    for(lit = myList.begin();lit!=myList.end();lit++)
    {
        if (x == *lit)
        {
            return 1;
        }
    }
    return 0;
}


// Compares the two lists and returns 1 if they are equal.
int listsEqual (list <int> listOne,list <int> listTwo)
{
    if (listOne.size() != listTwo.size())
    {
        return 0;
    }
    if (listOne.size() == 0 && listTwo.size() == 0)
    {
        return 1;
    }
    list <int>::iterator litOne, litTwo;
    litOne = listOne.begin();
    litTwo = listTwo.begin();
    while (litOne != listOne.end() && litTwo != listTwo.end())
    {
        if (*litOne != *litTwo)
        {
            return 0;
        }
        litOne++;
        litTwo++;
    }
    return 1;
}

// Returns a string of the list.
// E.g. [1 3 2 5] would give "1,3,2,5"
// Up to user to delete the string
char * listToString(list <int> myList)
{
    int length = 0;
    list <int>::iterator lit;
    list <int>::iterator tempLit;
    for(lit = myList.begin();lit!=myList.end();lit++)
    {
        // Determine how many digits are in the number.
        int i=1;
        while (*lit < (int)pow(10.0,(double)i))
        {
            i++;
        }
        length += i-1 + 1; //Add extra 1 for the comma
    }
    char *retString = new char[length+1]; //Null terminate

    int curIndex = 0;
    for(lit = myList.begin();lit!=myList.end();lit++)
    {
        tempLit = lit;
        tempLit++;
        if (tempLit != myList.end())
        {
            sprintf(retString + curIndex,"%d,",*lit); // This will null terminate
        }
        else
        {
            sprintf(retString + curIndex,"%d",*lit); // This will null terminate
        }
        curIndex = strlen(retString) - curIndex;;
    }

    return retString;
}


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

// This program reads in an adjacency graph and outputs
// the directed cycles. Optionally a limit on the cycle
// size can be given as a command line argument.

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "graphalg.h"


// ./listdircycles < mat1.txt

// To limit the cycles to size of three.
// ./listdircycles 3 < mat1.txt

using namespace std;

int main (int argc, char *argv[])
{
    int numNodes;
    cin >> numNodes;

    int **adjMat = allocateAdjMat(numNodes);
    int maxCycleSize = -1; // -1 means print all cycles.

    if (argc > 1)
    {
        maxCycleSize = atoi(argv[1]);
    }

    for (int i=0;i<numNodes;i++)
    {
        for (int j=0;j<numNodes;j++)
        {
            cin >> adjMat[i][j];
        }
    }
    //printMat(adjMat,numNodes);

    //list <list <int> > minCycles = getMinCycles(adjMat,numNodes);

    //list <list <int> >::iterator llit;

    //cout << "Min cycles using DFS:" << endl;
    //for (llit=minCycles.begin();llit!=minCycles.end();llit++)
    //{
    //    list <int>::iterator lit;
    //    cout << "    ";
    //    for (lit=(*llit).begin();lit!=(*llit).end();lit++)
    //    {
    //        cout << *lit << " ";
    //    }
    //    cout << endl;
    //}

    cycleGenerator myCycleGen(adjMat,numNodes);
    // Only save uniq cycles. That is, only print bi-directed cycles once
    myCycleGen.setHashBiDirectedCycles(1);
    myCycleGen.setCycleLengthMin(2);
    myCycleGen.print();
    list <int> myCycle;
    myCycle = myCycleGen.nextCycle(); 
    do
    {
        printListInt(myCycle);
        //cout << "isCycleBiDirected(myCycle) = " << myCycleGen.isCycleBiDirected(myCycle) << endl;
        //cout << "revCycle: ";
        //list <int> reversedList = revList(myCycle);
        //printListInt(reversedList);
        //cout << "Unique cycle rep: "; 
        //list <int> uniqCycle = myCycleGen.uniqueBiDirCycle(myCycle);
        //printListInt(uniqCycle);

        //cout << listToString(myCycle) << endl;
        myCycle = myCycleGen.nextCycle(); 
    } while (myCycle.size() != 0 && (myCycle.size() <= maxCycleSize || maxCycleSize == -1));
    //cout << "oneStep function called " << myCycleGen.numOneStepCall() << " times." << endl;
    //cout << "sizeUniqBiDirectedCycles() = "  << myCycleGen.sizeUniqBiDirectedCycles() << endl; 

    //int a = 0;
    //int b = 1;
    ////cout << "Enter two nodes to find paths between: ";
    ////cin >> a;
    ////cin >> b;
    //list <int> myPath = getPath(adjMat,numNodes,a,b,1);
    //cout << "Path from " << a << " to " << b << ": ";
    //printListInt(myPath);

    //list <list <int> > allPaths = getAllPaths(adjMat,numNodes,a,b,1);
    //cout << "allPath.size() = " << allPaths.size() << endl;

    //for (llit=allPaths.begin();llit!=allPaths.end();llit++)
    //{
    //    printListInt(*llit);
    //}

    deleteAdjMat(adjMat,numNodes);
}

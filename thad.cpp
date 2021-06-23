/*
 * Coding challenge for sweg, 3D cube edge connections
 * Author: Thadeus Ferney
 * I only started learning c++ doing this challenge, so there are some ugly parts
 * There may be some things that should have been done better, but oh well
 *
 * This uses a Union Find/Disjoint set to group and find connections
 * with path compression and union by size, the algorithm is very fast,
 * but my implementation of the checks both for adjacency and checking
 * each side are ugly but seem quick enough
 */
#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

int n;

/*
 * finds the root of the group give the position of a "node"
 * inputs int position of node, reference to the set
 * returns position of parent, value of parent is -groupSize
 */ 
int findParent(int posA, vector<signed int>& disjoint){
    if (disjoint.at(posA) < 0){ return posA; }
    //This is the path compression for the union find
    disjoint.at(posA) = findParent(disjoint.at(posA), disjoint);
    return disjoint.at(posA);
}

/*
 *  given two nodes, joins the groups by size, smaller into bigger to reduce 
 *  tree depth
 *  I use a reference to avoid copying all the data, this should probably some kind of global
 *  inputs two positions and reference to group
 */ 
void join(int posA, int posB, vector<signed int>& disjoint){
    //Find parent of both
    int parentA = findParent(posA, disjoint);
    int parentB = findParent(posB, disjoint);
    if(parentA == parentB) { return; }
    //join parents, based on size, small joins big
    if(disjoint.at(parentA) < disjoint.at(parentB)){
        disjoint.at(parentA) += disjoint.at(parentB);
        disjoint.at(parentB) = parentA;
    } else {
        disjoint.at(parentB) += disjoint.at(parentA);
        disjoint.at(parentA) = parentB;
    }
}

/*
 *  simple calculation to find the position in group array based on cube position
 *  Mostly to increase readability  
 *  input i, j, k, its position in the cube
 *  returns int of position in array
 */ 
int getPosition(int i, int j, int k){
    return ((i * pow(n, 2)) + (j * n) + k);
}

/*
 * Checks if a position in the cube is valid and in the bounds and is a 1
 * inputs i, j, k position of the cube, reference to the cube
 * returns true if valid one
 */ 
bool isOne(int i, int j, int k, vector<vector<vector<int>>>& cube){
    // if any point is outside the cube return false
    if(i >= n || j >= n || k >= n){ return false; }
    if(i < 0 | j < 0 | k < 0){ return false; }
    if(cube[i][j][k] == 1){ return true; }
    return false;
}

/*
 *  Reads in the file in.txt building up the cube
 *  returns 3d vector of the cube
 */ 
vector<vector<vector<int>>> readFile(){
    fstream inputFile;
    inputFile.open("in.txt", ios::in);
    if(!inputFile){
        cout << "in.txt not found!\n";
        exit(0);
    } 
    char input;
    int i = 0;
    int j = 0;
    string inputString;
    vector<int> tmpBaseVec;
    vector<vector<int>> tmp2DVec;
    vector<vector<vector<int>>> tmp3DVec;

    char delimit;
    bool firstLine = true;
    while(getline(inputFile, inputString)){
        delimit = true;
        istringstream strStream(inputString);

        if (firstLine) { 
            strStream >> n;
            firstLine = false;
            continue;
        }
        int tmpInt = 0;
        while(strStream >> tmpInt){
            tmpBaseVec.push_back(tmpInt);
            delimit = false;
        } 
        if(delimit){
            tmp2DVec.clear();
            j=0;
            continue;
        }
        tmp2DVec.push_back(tmpBaseVec);
        j++;
        if(j == n){
            j = 0;
            tmp3DVec.push_back(tmp2DVec);
            tmp2DVec.clear();
        }
        tmpBaseVec.clear();
    }
    return tmp3DVec;

}

int main( int argc, char** argv ) {
    unsigned long startStopWatch = clock();
    //Take in grid, size as n
    vector<vector<vector<int>>> cube = readFile();

    // vector of size n^3 filled with -1s as everything starts in a group with itself
    vector<int> disjoint(pow(n, 3), -1);


    // Join all positions in the cube with neighbors that are one
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < n; k++){
                if(cube.at(i).at(j).at(k) == 1){
                    int basePosition = getPosition(i, j, k);
                    // Checking each adjacent and dialog 
                    // There are 26 total, 9 above, 9 below, and 8 on the same layer
                    // This is really gross, I can't think of a better way though
                    if(isOne(i+1, j, k, cube)){
                        join(basePosition, getPosition(i+1,j,k), disjoint);
                    } 
                    if(isOne(i+1, j, k+1, cube)){
                        join(basePosition, getPosition(i+1,j,k+1), disjoint);
                    } 
                    if(isOne(i+1, j, k-1, cube)){
                        join(basePosition, getPosition(i+1,j,k-1), disjoint);
                    } 
                    if(isOne(i+1, j+1, k, cube)){
                        join(basePosition, getPosition(i+1,j+1,k), disjoint);
                    } 
                    if(isOne(i+1, j+1, k+1, cube)){
                        join(basePosition, getPosition(i+1,j+1,k+1), disjoint);
                    } 
                    if(isOne(i+1, j+1, k-1, cube)){
                        join(basePosition, getPosition(i+1,j+1,k-1), disjoint);
                    } 
                    if(isOne(i+1, j-1, k, cube)){
                        join(basePosition, getPosition(i+1,j-1,k), disjoint);
                    } 
                    if(isOne(i+1, j-1, k+1, cube)){
                        join(basePosition, getPosition(i+1,j-1,k+1), disjoint);
                    } 
                    if(isOne(i+1, j-1, k-1, cube)){
                        join(basePosition, getPosition(i+1,j-1,k-1), disjoint);
                    } 
                    if(isOne(i-1, j, k, cube)){
                        join(basePosition, getPosition(i-1,j,k), disjoint);
                    } 
                    if(isOne(i-1, j, k+1, cube)){
                        join(basePosition, getPosition(i-1,j,k+1), disjoint);
                    } 
                    if(isOne(i-1, j, k-1, cube)){
                        join(basePosition, getPosition(i-1,j,k-1), disjoint);
                    } 
                    if(isOne(i-1, j+1, k, cube)){
                        join(basePosition, getPosition(i-1,j+1,k), disjoint);
                    } 
                    if(isOne(i-1, j+1, k+1, cube)){
                        join(basePosition, getPosition(i-1,j+1,k+1), disjoint);
                    } 
                    if(isOne(i-1, j+1, k-1, cube)){
                        join(basePosition, getPosition(i-1,j+1,k-1), disjoint);
                    } 
                    if(isOne(i-1, j-1, k, cube)){
                        join(basePosition, getPosition(i-1,j-1,k), disjoint);
                    } 
                    if(isOne(i-1, j-1, k+1, cube)){
                        join(basePosition, getPosition(i-1,j-1,k+1), disjoint);
                    } 
                    if(isOne(i-1, j-1, k-1, cube)){
                        join(basePosition, getPosition(i-1,j-1,k-1), disjoint);
                    } 
                    if(isOne(i, j, k+1, cube)){
                        join(basePosition, getPosition(i,j,k+1), disjoint);
                    } 
                    if(isOne(i, j, k-1, cube)){
                        join(basePosition, getPosition(i,j,k-1), disjoint);
                    } 
                    if(isOne(i, j+1, k, cube)){
                        join(basePosition, getPosition(i,j+1,k), disjoint);
                    } 
                    if(isOne(i, j+1, k+1, cube)){
                        join(basePosition, getPosition(i,j+1,k+1), disjoint);
                    } 
                    if(isOne(i, j+1, k-1, cube)){
                        join(basePosition, getPosition(i,j+1,k-1), disjoint);
                    } 
                    if(isOne(i, j-1, k, cube)){
                        join(basePosition, getPosition(i,j-1,k), disjoint);
                    } 
                    if(isOne(i, j-1, k+1, cube)){
                        join(basePosition, getPosition(i,j-1,k+1), disjoint);
                    } 
                    if(isOne(i, j-1, k-1, cube)){
                        join(basePosition, getPosition(i,j-1,k-1), disjoint);
                    }
                }
            }
        }
    }

    int pathCount = 0;
    
    // For each pos on side of first layer find with opposite side on bottom layer
    // This checks should go into either a separate place or made into some kind of function
    // Checking upper top to lower bottom edges
    for(int i = 0; i < n; i++){
        int parentA = findParent(i, disjoint);
        bool broken = false;
        for(int j = 0; j < n; j++){
            int parentB = findParent(getPosition(n-1, n-1, j), disjoint);
            if(parentA == parentB) {
                pathCount++;
                broken = true;
                break;
            }
        }
        if(broken){break;}
    }

    // Checking left top to right bottom edges
    for(int i = 0; i < n; i++){
        int parentA = findParent(getPosition(0, i, 0), disjoint);
        bool broken = false;
        for(int j = 0; j < n; j++){
            int parentB = findParent(getPosition(n-1, j, n-1), disjoint);
            if(parentA == parentB) {
                pathCount++;
                broken = true;
                break;
            }
        }
        if(broken){break;}
    }

    // Checking right top to left bottom edges
    for(int i = 0; i < n; i++){
        int parentA = findParent(getPosition(0, i, n-1), disjoint);
        bool broken = false;
        for(int j = 0; j < n; j++){
            int parentB = findParent(getPosition(n-1, j, 0), disjoint);
            if(parentA == parentB) {
                pathCount++;
                broken = true;
                break;
            }
        }
        if(broken){break;}
    }
    // Checking bottom top to upper bottom edges
    for(int i = 0; i < n; i++){
        int parentA = findParent(getPosition(0, n-1, i), disjoint);
        bool broken = false;
        for(int j = 0; j < n; j++){
            int parentB = findParent(getPosition(n-1, 0, j), disjoint);
            if(parentA == parentB) {
                pathCount++;
                broken = true;
                break;
            }
        }
        if(broken){break;}
    }
    // Checking left top to right bottom edges on the side
    for(int i = 0; i < n; i++){
        int parentA = findParent(getPosition(i, 0, 0), disjoint);
        bool broken = false;
        for(int j = 0; j < n; j++){
            int parentB = findParent(getPosition(j, n-1, n-1), disjoint);
            if(parentA == parentB) {
                pathCount++;
                broken = true;
                break;
            }
        }
        if(broken){break;}
    }
    // Checking right top to left bottom edges on the side
    for(int i = 0; i < n; i++){
        int parentA = findParent(getPosition(i, 0, n-1), disjoint);
        bool broken = false;
        for(int j = 0; j < n; j++){
            int parentB = findParent(getPosition(j, n-1, 0), disjoint);
            if(parentA == parentB) {
                pathCount++;
                broken = true;
                break;
            }
        }
        if(broken){break;}
    }


    unsigned long stopStopwatch = clock();
    cout << "There are: " << pathCount << " connected edges\n";
    cout << "Found in : " << ((float) stopStopwatch - startStopWatch)/CLOCKS_PER_SEC << " seconds\n";
}
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: omar
 *
 * Created on May 27, 2018, 2:39 PM
 */

#include <cstdlib>
#include <vector>
#include "RstarTree.h"
#include "RstarTreeNode.h"
#include <ctime>
using namespace std;

/*
 * 
 */

int main(int argc, char** argv) {

    RstarTreeNode leaf = RstarTreeNode(6,2);
    leaf.isLeaf = true;
    leaf.level = 0;
    size_t dimensions = 10;
    clock_t begin = clock();
    RstarTree tree = RstarTree(&leaf);
    vector<MinBoundingBox*> mbrs;
    for(int i=0 ; i< 10; i++)
    {
        vector< pair<double,double> > edges;
        for(int j = 0 ; j < 10 ; j++)
        {
            double x = ((rand() % 10) - 5) +i;
            edges.push_back(make_pair(x,x));
        }
        mbrs.push_back(new MinBoundingBox(dimensions,edges));
    } 
    
    for(int i = 0 ; i < mbrs.size() ; i++)
    {
        tree.Insert(mbrs[i]);
//        tree.PrintClusters();
//        tree.GetTreeDepth();
//        cout<<"**************************************"<<endl;
    }
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    
    tree.PrintClusters();
    tree.GetTreeDepth();
    cout<<"elapsed time: "<<elapsed_secs<<endl;
    //tree.PrintTree();
    return 0;
}


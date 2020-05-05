/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RstarTree.h
 * Author: omar
 *
 * Created on June 8, 2018, 6:34 PM
 */

#ifndef RSTARTREE_H
#define RSTARTREE_H
#include "RstarTreeNode.h"
#include "MinBoundingBox.h"
#include <string>
#include<vector>
using namespace std;
class RstarTree 
{
    private:
        
    public:
        size_t maxItems ,minItems;
        vector<MinBoundingBox*> removed_mbrs;
        RstarTreeNode *root;
        RstarTree(RstarTreeNode *root);
        void Insert(MinBoundingBox *mbr);
        void overflowTreatment(RstarTreeNode *N,bool parentSplit = false);
        void reInsert(RstarTreeNode *N);
        void Delete();
        void PrintTree();
        void PrintClusters();
        void GetTreeDepth();
        void overflowParent(RstarTreeNode *N , RstarTreeNode *newNode);
        void InsertLeaves(RstarTreeNode *N);
};

#endif /* RSTARTREE_H */


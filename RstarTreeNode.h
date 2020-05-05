/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RstarTreeNode.h
 * Author: omar
 *
 * Created on May 27, 2018, 2:47 PM
 */

#ifndef RSTARTREENODE_H
#define RSTARTREENODE_H
#include<vector>
#include <limits>
#include <cstddef>
#include <iostream>
#include"MinBoundingBox.h"
#include <string>
#include<unordered_map>
class RstarTreeNode {

	private:
		
                size_t chooseSplitAxis();
                std::pair<RstarTreeNode* , RstarTreeNode*> chooseSplitIndex(size_t split_axis);
                RstarTreeNode *createRstarNode(std::vector<MinBoundingBox*> boundedItems);
                
	public:
                size_t maxItems ,minItems;
                int level;
                static unordered_map<int,bool> OVERFLOWED_LEVELS;
                std::vector<MinBoundingBox*> boundedRectangles;
                RstarTreeNode *parent;
                bool isLeaf;
                bool isOverflowTreated = false;
                MinBoundingBox *mbr;
                std::vector<RstarTreeNode *> children ;
                inline RstarTreeNode(){}
		RstarTreeNode(size_t M, size_t m){maxItems = M; minItems=m;boundedRectangles =  std::vector<MinBoundingBox*> ();
                 children =  std::vector<RstarTreeNode*> (); parent = nullptr;}
                int chooseSubTree(MinBoundingBox *mbr);             
                std::pair<RstarTreeNode* , RstarTreeNode*> split();
                void reset();
                

};

#endif /* RSTARTREENODE_H */


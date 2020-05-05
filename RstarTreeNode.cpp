/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"RstarTreeNode.h"
#include "RstarTree.h"
#include<vector>
#include <c++/5/bits/stl_bvector.h>

template <typename MinBoundingBox>
struct SortItemsbyFirst : 
        public std::binary_function< const MinBoundingBox * const, const MinBoundingBox * const, bool >
{
        const std::size_t m_axis;
        explicit SortItemsbyFirst (const std::size_t axis) : m_axis(axis) {}

        bool operator() (const MinBoundingBox * const bi1, const MinBoundingBox * const bi2) const 
        {
                return bi1->edges[m_axis].first < bi2->edges[m_axis].first;
        }
};

template <typename MinBoundingBox>
struct SortItemsbySecond : 
        public std::binary_function< const MinBoundingBox * const, const MinBoundingBox * const, bool >
{
        const std::size_t m_axis;
        explicit SortItemsbySecond (const std::size_t axis) : m_axis(axis) {}

        bool operator() (const MinBoundingBox * const bi1, const MinBoundingBox * const bi2) const 
        {
                return bi1->edges[m_axis].second < bi2->edges[m_axis].second;
        }
};

unordered_map<int,bool> RstarTreeNode ::OVERFLOWED_LEVELS;
 int RstarTreeNode :: chooseSubTree( MinBoundingBox *mbr)
 {
     if(this->isLeaf)
         return -1;
     
         double minArea = std::numeric_limits<double>::max();
         MinBoundingBox* chosen_mbr = NULL;
         int chosen_index = 0;
     if(!this->children.empty() && this->children[0]->isLeaf)
     {
         
         for(size_t i = 0 ; i < this->boundedRectangles.size() ; i++)
         {
            double area = 0;
            MinBoundingBox* temp = this->boundedRectangles[i];
            temp->enlarge(mbr);
            for(size_t j = 0 ; j < this->boundedRectangles.size() ; j++)
            {
                if(j != i)
                {
                     area += temp->overlapping_area(this->boundedRectangles[j]);

                }

            }
             
             if(area < minArea)
            {
                minArea = area;
                chosen_mbr = this->boundedRectangles[i];
                chosen_index = i;
            }
             else if(area == minArea &&
                     this->boundedRectangles[i]->area_enlargment(mbr) < chosen_mbr->area_enlargment(mbr))
             {
                chosen_mbr = this->boundedRectangles[i];
                chosen_index = i;  
             }
                 
         }
         
         return chosen_index;
     }
     
     else
     {
         
         for(size_t i = 0 ; i < this->boundedRectangles.size() ; i++)
         {
             double area = this->boundedRectangles[i]->area_enlargment(mbr);
             if(area < minArea)
            {
                minArea = area;
                chosen_mbr = this->boundedRectangles[i];
                chosen_index = i;
            }
             else if(area == minArea && this->boundedRectangles[i]->area() < chosen_mbr->area())
             {
                chosen_mbr = this->boundedRectangles[i];
                chosen_index = i;
             }
         }
         
         return chosen_index;
     }   
 }
 
  size_t RstarTreeNode :: chooseSplitAxis()
 {
     size_t num_distributions = maxItems - (2*minItems) + 2;
      double split_factor = std::numeric_limits<double>::max();
      size_t best_axis = 0;
     for(size_t axis = 0 ; axis < this->mbr->edges.size() ; axis++)
     {
         double min_overlap = std::numeric_limits<double>::max();
         double overlap_area = 0.0;
        for(int k = 0 ; k < num_distributions ; k++)
        {
            for(int i = 0 ; i < 2 ; i++)
            {

                if(i  == 0)
                {
                    std::sort(this->boundedRectangles.begin(), this->boundedRectangles.end(), SortItemsbyFirst<MinBoundingBox>(axis));
                }
                else
                {
                    std::sort(this->boundedRectangles.begin(), this->boundedRectangles.end(), SortItemsbySecond<MinBoundingBox>(axis));
                }
                    
                size_t first_group_end = (minItems - 1) + (k+1);
                std::vector<MinBoundingBox*> first_group(this->boundedRectangles.begin(),this->boundedRectangles.begin()+first_group_end);
                std::vector< MinBoundingBox*> second_group(this->boundedRectangles.begin()+first_group_end,this->boundedRectangles.end());
                MinBoundingBox *first_group_mbr = new MinBoundingBox(first_group);
                MinBoundingBox *second_group_mbr = new MinBoundingBox(second_group);
                
                double s_temp = first_group_mbr->area() + second_group_mbr->area();
                s_temp += first_group_mbr->distanceFromCenter(this->mbr) + second_group_mbr->distanceFromCenter(this->mbr);
                
                if(first_group_mbr->overlap(second_group_mbr))
                {
                    double overlap_area = first_group_mbr->overlapping_area(second_group_mbr);
                    s_temp += overlap_area;
                }
                if(s_temp < split_factor)
                {
                    split_factor = s_temp;
                    best_axis = axis; 
                }
            }
        }
     }
      return best_axis;
 }
 
 std::pair<RstarTreeNode* , RstarTreeNode*> RstarTreeNode :: chooseSplitIndex(size_t split_axis)
  {
      size_t num_distributions = maxItems - (2*minItems) + 2;
       double min_overlap = std::numeric_limits<double>::max();
       MinBoundingBox *first_group_mbr_chosen;
       MinBoundingBox *second_group_mbr_chosen;
       std::vector<MinBoundingBox*> first_group_list;
       std::vector<MinBoundingBox*> second_group_list;
        for(int k = 0 ; k < num_distributions ; k++)
        {
            size_t first_group_end = (minItems - 1) + (k+1);
            for(int i = 0 ; i < 2 ; i++)
            {
                
                if(i  == 0)
                {
                    std::sort(this->boundedRectangles.begin(), this->boundedRectangles.end(), SortItemsbyFirst<MinBoundingBox>(split_axis));
                }
                else
                {
                    std::sort(this->boundedRectangles.begin(), this->boundedRectangles.end(), SortItemsbySecond<MinBoundingBox>(split_axis));
                }
                    
                std::vector<MinBoundingBox*> first_group(this->boundedRectangles.begin(),this->boundedRectangles.begin()+first_group_end);
                std::vector< MinBoundingBox*> second_group(this->boundedRectangles.begin()+first_group_end,this->boundedRectangles.end());
                MinBoundingBox *first_group_mbr = new MinBoundingBox(first_group);
                MinBoundingBox *second_group_mbr = new MinBoundingBox(second_group);
                double overlap_area = first_group_mbr->overlapping_area(second_group_mbr);
                if(overlap_area < min_overlap)
                {
                    first_group_mbr_chosen = first_group_mbr;
                    second_group_mbr_chosen = second_group_mbr;
                    first_group_list = first_group;
                    second_group_list = second_group;
                    min_overlap = overlap_area;
                }
                else if(overlap_area == min_overlap)
                {
                    double chosen_area = first_group_mbr_chosen->area() + second_group_mbr_chosen->area();
                    double area = first_group_mbr->area() + second_group_mbr->area();
                    if(area < chosen_area)
                    {
                        first_group_mbr_chosen = first_group_mbr;
                        second_group_mbr_chosen = second_group_mbr;
                        first_group_list = first_group;
                        second_group_list = second_group;
                    }
                }
            }
        }
       RstarTreeNode *firstNode = createRstarNode(first_group_list);
       RstarTreeNode * secondNode = createRstarNode(second_group_list);
       if(!children.empty())
       {
           vector<RstarTreeNode *> temp1(this->children.begin(),this->children.begin()+first_group_list.size());
           vector<RstarTreeNode *> temp2(this->children.begin()+first_group_list.size(),this->children.end());
           firstNode->children = temp1;
           secondNode->children = temp2;
       }
       
       return std::make_pair(firstNode,secondNode);
  }
  
  
  std::pair<RstarTreeNode* , RstarTreeNode*> RstarTreeNode :: split()
  {
      size_t axis = this->chooseSplitAxis();
      return this->chooseSplitIndex(axis);
  }

RstarTreeNode* RstarTreeNode::createRstarNode(std::vector<MinBoundingBox*> boundedItems) 
{
    RstarTreeNode *N = new RstarTreeNode(maxItems,minItems);
    N->boundedRectangles = boundedItems;
    N->mbr = new MinBoundingBox(boundedItems);
    N->isLeaf = isLeaf;
    N->isOverflowTreated = isOverflowTreated;
    if(parent != nullptr)
        N->parent = &(*parent);
    return N;
}

void RstarTreeNode:: reset()
{
    this->mbr = new MinBoundingBox(this->mbr->dimensions,this->mbr->edges);
    this->boundedRectangles.erase(this->boundedRectangles.begin(),this->boundedRectangles.end());
    for(int i = 0 ; i < children.size() ; i++)
    {
        this->mbr->enlarge(children[i]->mbr);
        this->boundedRectangles.push_back(children[i]->mbr);
    }
}

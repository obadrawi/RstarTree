/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <queue>
#include <c++/5/bits/stl_pair.h>
#include <c++/5/bits/stl_vector.h>
#include<math.h>
//#include <c++/5/bits/algorithmfwd.h>

#include "RstarTree.h"


template <typename RstarTreeNode>
struct SortMbrByDistance: 
        public std::binary_function< const RstarTreeNode * const, const RstarTreeNode * const, bool >
{
         MinBoundingBox * m_mbr;
        explicit SortMbrByDistance (MinBoundingBox * mbr) {m_mbr = &(*mbr);}

        bool operator() ( RstarTreeNode *  bi1,  RstarTreeNode *  bi2)  
        {
                return m_mbr->distanceFromCenter(bi1->mbr) > m_mbr->distanceFromCenter(bi2->mbr);
        }
};


RstarTree :: RstarTree(RstarTreeNode *root) 
{
    this->root = root;
    maxItems = root->maxItems;
    minItems =  root->minItems;
    this->root->mbr = nullptr;
}

void RstarTree :: Insert(MinBoundingBox *mbr)
{
   
   RstarTreeNode *temp = &(*this->root);
   int index =  this->root->chooseSubTree(mbr);
    while(index != -1 && temp->children.size() > index)
    {
        temp->mbr->enlarge(&(*mbr));
        temp = &(*(temp->children[index])); 
        index =  temp->chooseSubTree(mbr); 
    }
   if(index == -1)
   {
       temp->boundedRectangles.push_back(&(*mbr));
       temp->level = 0;
       temp->isLeaf = true;
       if(temp->mbr == nullptr)
       {
           
           temp->mbr = new MinBoundingBox(mbr->dimensions,mbr->edges);
       }
       else
       {
            temp->mbr->enlarge(&(*mbr));
       }
      
       if(temp->boundedRectangles.size() > maxItems)
       {  
           overflowTreatment(&(*temp));
       }
   }
}

void RstarTree :: overflowTreatment(RstarTreeNode *N,bool parentSplit)
  {
    if(N->OVERFLOWED_LEVELS[N->level]|| N->isLeaf || N->parent == nullptr)
    {
        std::pair<RstarTreeNode* , RstarTreeNode*> groups =  N->split();
        if(&(*N) == &(*root))
        {
            N = &(*groups.first);
        }
        
        else
        {
            N->mbr = &(*groups.first->mbr);
            N->boundedRectangles = (*groups.first).boundedRectangles;
            N->children = (*groups.first).children;
        }
        RstarTreeNode *newNode = &(*groups.second);
        newNode->level = N->level;
        RstarTreeNode *parent = &(*(N->parent));
       
        if(parent != nullptr)
        {
            parent->children.push_back(&(*newNode));
            parent->boundedRectangles.push_back(&(*newNode->mbr));
            if(parent->boundedRectangles.size() > maxItems)
            {
                overflowTreatment(parent);
            }
        }
        else
        {
            parent = new RstarTreeNode(maxItems,minItems);
            parent->level = N->level + 1;
            N->parent = &(*parent);
            
            newNode->parent = &(*parent);
            parent->children.push_back(&(*N));
            parent->children.push_back(&(*newNode));
            
            parent->boundedRectangles.push_back(&(*N->mbr));
            parent->boundedRectangles.push_back(&(*newNode->mbr));
            parent->mbr = new MinBoundingBox(parent->boundedRectangles);
            
            parent->isLeaf = false;
            parent->parent = nullptr;
            root = &(*parent);     
        }
    }
    else
    {
        N->OVERFLOWED_LEVELS[N->level] = true;
        reInsert(N);
    }
  }
 void RstarTree ::reInsert(RstarTreeNode *N)
 {
     
     int p = (0.3 * maxItems) + 1;
     
     std::sort(N->children.begin(), N->children.end(), SortMbrByDistance<RstarTreeNode>(N->mbr));
     //std::vector<std::pair<int,double>> distance_vector;
      std::vector<RstarTreeNode> reInsert_vector;
     std::vector<RstarTreeNode *> removed_entries(N->children.begin(),N->children.begin()+p);
     N->children.erase(N->children.begin(),N->children.begin()+p);
     N->reset();
     
     //distance_vector.erase(distance_vector.begin(),distance_vector.begin()+p);
     for(size_t i = 0 ; i < removed_entries.size() ; i++)
     {
         InsertLeaves(removed_entries[i]);

     }
 }
void RstarTree ::PrintTree()
{
    std::queue<RstarTreeNode* > nodes;
    nodes.push(root);
    
     //int counter = 0;
    while(!nodes.empty())
    {
        
        
        RstarTreeNode *temp = nodes.front();
        nodes.pop();
       
        std::vector< pair<double,double> > edges(temp->mbr->edges);
        
        for(int i = 0 ; i < edges.size() ; i++)
        {
            
                string out = "( " + std::to_string(edges[i].first) + " , " + std::to_string(edges[i].second) + " )";

                 cout<<out<<endl;
        }
        
         if(!temp->children.empty())
         {
             
             for(int i = 0 ; i < temp->children.size() ; i++)
             {
                 nodes.push(temp->children[i]);
             }
         }
         
    }
        
       
}

void RstarTree::PrintClusters() 
{
    std::queue<RstarTreeNode* > nodes;
    nodes.push(root);
    
    int count = 0 , counter = 0;
    size_t dimension = root->boundedRectangles[0]->dimensions;
    double avg_variance[dimension] = {0.0};
    while(!nodes.empty())
    {  
        RstarTreeNode *temp = nodes.front();
        nodes.pop();
        if(temp->isLeaf)
        {
            count++;
            cout<<"cluster "<<count<<" :"<<endl;
            
            double mean[dimension] = {0.0};
            for(int j = 0 ; j < temp->boundedRectangles.size() ; j++)
            {
                cout<<"point "<<j+1<<" :"<<endl;
                counter++;
                std::vector< pair<double,double> > edges(temp->boundedRectangles[j]->edges);
                string out = "( " ;
                for(int i = 0 ; i < edges.size()-1 ; i++)
                {
                   mean[i] += edges[i].first/temp->boundedRectangles.size();
                   out +=  std::to_string(edges[i].first) + " , ";   
                }
                mean[edges.size()-1] += edges[edges.size()-1].first;
                out +=  std::to_string(edges[edges.size()-1].first) + " )";
                cout<<out<<endl;
            }
            for(int j = 0 ; j < temp->boundedRectangles.size() ; j++)
            {
                for(int i = 0 ; i < dimension ; i++)
                {
                    avg_variance[i] += (pow(temp->boundedRectangles[j]->edges[i].first - mean[i],2))/pow(temp->boundedRectangles.size()*dimension,2);     
                }
            }
         }
          
         if(!temp->children.empty())
         {
             
             for(int i = 0 ; i < temp->children.size() ; i++)
             {
                 
                 nodes.push(temp->children[i]);
             }
         }
        
    }
    cout<<"points "<<counter<<endl;
    double avg = 0.0;
    for(int i = 0 ; i < dimension ; i++)
    {
        avg+= avg_variance[i];
    }
    
    cout<<"avg var "<<(avg/dimension)<<endl;
}

void RstarTree :: GetTreeDepth()
{
    RstarTreeNode node = *root;
    int count = 0;
    while(!node.isLeaf)
    {
        node = *(node.children[0]);
        count++;
    }
    cout<<"Depth "<<count<<endl;
}
void RstarTree :: InsertLeaves(RstarTreeNode *N)
{
    if(N->isLeaf)
    {
       //RstarTree::removed_mbrs.insert(RstarTree::removed_mbrs.end(),N->boundedRectangles.begin(),N->boundedRectangles.end());
        for(int i = 0 ; i < N->boundedRectangles.size() ; i++)
        {
            Insert(N->boundedRectangles[i]);
        }
    }
    else
    {
        for(int i = 0 ; i < N->children.size() ; i++)
        {
            InsertLeaves(N->children[i]);
        }
    }
}
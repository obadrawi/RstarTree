/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MinBoundingBox.h
 * Author: omar
 *
 * Created on May 27, 2018, 2:40 PM
 */

#ifndef MINBOUNDINGBOX_H
#define MINBOUNDINGBOX_H

#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;
class MinBoundingBox
{
	private :
            

	public:

           vector< pair<double,double> > edges;
size_t dimensions;

MinBoundingBox(size_t dimensions_t,vector< pair<double,double> > edges);
MinBoundingBox(vector<MinBoundingBox*> mbrs);
double margin();
double area();

bool overlap(MinBoundingBox *mbr);

double overlapping_area(MinBoundingBox *mbr);
double distanceFromCenter(MinBoundingBox *mbr);
void enlarge(MinBoundingBox *mbr);

double area_enlargment(MinBoundingBox *mbr);
    
};


#endif /* MINBOUNDINGBOX_H */
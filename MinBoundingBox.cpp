/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include"MinBoundingBox.h"
#include <stdlib.h> 
#include<cmath>
MinBoundingBox :: MinBoundingBox(size_t dimensions_t,vector< pair<double,double> > edges)
{
        dimensions = dimensions_t;
        
        this->edges = edges;
}

MinBoundingBox :: MinBoundingBox(vector<MinBoundingBox*> mbrs)
{
    this->edges = mbrs[0]->edges;
    this->dimensions = mbrs[0]->dimensions;
    for(int i = 1 ; i < mbrs.size() ; i++)
    {
        this->enlarge(mbrs[i]);
    }
}
double MinBoundingBox :: margin()
{
        double margin = 0;
        for (size_t axis = 0 ;  axis < dimensions ; axis++)
        {
            margin += edges[axis].second - edges[axis].first;
        }
        return margin;
}

double MinBoundingBox :: area()
{
    double area = 1;
    for (size_t axis = 0 ;  axis < dimensions ; axis++)
    {
            area *= (edges[axis].second - edges[axis].first);
    }
    return area;
}

bool MinBoundingBox :: overlap(MinBoundingBox *mbr)
{
    /*size_t size = bb_dimensions;
    if(dimensions < size)
            size = dimensions;*/

    for(size_t axis = 0 ; axis < dimensions ; axis++)
    {

            if (!(edges[axis].first < mbr->edges[axis].second) || !(mbr->edges[axis].first < edges[axis].second))
                    return false;
    }

    return true;
}

double MinBoundingBox :: overlapping_area(MinBoundingBox *mbr)
{
    double area = 1.0;
    for (std::size_t axis = 0; area && axis < dimensions; axis++)
    {
            // this makes it easier to understand
             int x1 = edges[axis].first;
             int x2 = edges[axis].second;
             int y1 = mbr->edges[axis].first;
             int y2 = mbr->edges[axis].second;

            // left edge outside left edge
            if (x1 < y1)
            {
                    // and right edge inside left edge
                    if (y1 < x2)
                    {
                            // right edge outside right edge
                            if (y2 < x2)
                                    area *= (double)( y2 - y1 );
                            else
                                    area *= (double)( x2 - y1 );

                            continue;
                    }
            }
            // right edge inside left edge
            else if (x1 < y2)
            {
                    // right edge outside right edge
                    if (x2 < y2)
                            area *= (double)( x2 - x1 );
                    else
                            area *= (double)( y2 - x1 );

                    continue;
            }

            // if we get here, there is no overlap
            return 0.0;
    }

    return area;
}
double MinBoundingBox:: distanceFromCenter(MinBoundingBox *mbr)
{
    double distance = 0, t;
    for (std::size_t axis = 0; axis < dimensions; axis++)
    {
            t = ((double)edges[axis].first + (double)edges[axis].second +
                     (double)mbr->edges[axis].first + (double)mbr->edges[axis].second)
                     /2.0;
            distance += t*t;
    }

    return distance;
}
void MinBoundingBox :: enlarge(MinBoundingBox *mbr)
{
    for (std::size_t axis = 0; axis < dimensions; axis++)
    {
        
            edges[axis].first = std::min(edges[axis].first ,mbr->edges[axis].first);
            edges[axis].second = std::max(edges[axis].second ,mbr->edges[axis].second);
    }

}

double MinBoundingBox :: area_enlargment(MinBoundingBox *mbr)
{
    double area = 1.0;
    for (std::size_t axis = 0; axis < dimensions; axis++)
    {
            double first = std::abs(edges[axis].first- mbr->edges[axis].first);
            double second = std::abs(edges[axis].second - mbr->edges[axis].second);
            area *= second - first;
    }
    return area;
}

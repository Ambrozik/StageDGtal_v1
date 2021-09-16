#ifndef MYFUNCTIONS
#define MYFUNCTIONS

#include "DGtal/helpers/StdDefs.h"
#include "DGtal/base/Common.h"
#include "DGtal/io/boards/Board2D.h"
#include <stdio.h>
#include <numeric>
#include <algorithm>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <exception>

using namespace std;
using namespace DGtal;
using namespace Z2i;

typedef std::vector<DGtal::Z2i::Point> Range;
typedef std::vector<DGtal::Z2i::Point>::const_iterator ConstIterator;
typedef std::vector<DGtal::Z2i::RealPoint>::const_iterator ConstIteratorD;
typedef ArithmeticalDSS<ConstIterator,int,4> DSS4;

typedef  AlphaThickSegmentComputer< DGtal::Z2i::Point > AlphaThickSegmentComputer2D;
typedef  AlphaThickSegmentComputer<DGtal::Z2i::RealPoint> AlphaThickSegmentComputer2DD;

struct MyDrawStyleCustomColor : public DrawableWithBoard2D
{
  Color myPenColor;
  Color myFillColor;
  MyDrawStyleCustomColor( const Color & penColor,
        const Color & fillColor )
    : myPenColor( penColor ), myFillColor( fillColor )
  {}
  virtual void setStyle( Board2D & aboard) const
  {
    aboard.setFillColor( myFillColor);
    aboard.setPenColor( myPenColor );
  }
};

typedef PointVector<2, double> rRealVector;
struct Value
{
    rRealVector first;
  double second;
  Value () : second ( 0. ) {}
  Value & operator+= ( const Value & ch )
  {
    this->first += ch.first;
    this->second += ch.second;
    return *this;
  }
};

double sort_increase(double a, double b);
vector<int> absSortIndex(vector<double> const& values, bool isIncrease=true);

vector<vector<DGtal::Z2i::RealPoint> > getContours(const vector<vector<DGtal::Z2i::RealPoint> > &vecPoint);

vector<double> readMeanindfulThicknessFile(const char* filename);

void writeFile(const vector<DGtal::Z2i::RealPoint>& v, const char* filename);

DGtal::Z2i::Point getStartPoint(const AlphaThickSegmentComputer2D s);
DGtal::Z2i::Point getEndPoint(const AlphaThickSegmentComputer2D s);
DGtal::Z2i::RealPoint getStartPoint(const AlphaThickSegmentComputer2DD s);
DGtal::Z2i::RealPoint getEndPoint(const AlphaThickSegmentComputer2DD s);

double getSlope(double Nx,double Ny);//(Nx,Ny) : normal vector of the line

double acuteAngle(DGtal::Z2i::RealPoint bp1, DGtal::Z2i::RealPoint bp2, DGtal::Z2i::RealPoint bp3); //acute angle between three points
double signedAngle(DGtal::Z2i::RealPoint v1, DGtal::Z2i::RealPoint v2); //angle between two vectors
double relativeAngle(DGtal::Z2i::RealPoint v1, DGtal::Z2i::RealPoint v2); //relative angle between two vectors
double relativeAngle(DGtal::Z2i::RealPoint bp1, DGtal::Z2i::RealPoint bp2, DGtal::Z2i::RealPoint bp3); //relative angle between three points

double distancePointSegment(DGtal::Z2i::RealPoint p, DGtal::Z2i::RealPoint s1, DGtal::Z2i::RealPoint s2);
double distancePoints(DGtal::Z2i::RealPoint p1, DGtal::Z2i::RealPoint p2);
double distancePointCircle(DGtal::Z2i::Point p, DGtal::Z2i::RealPoint center, double radius);
double signedDistancePointCircle(DGtal::Z2i::Point p, DGtal::Z2i::RealPoint center, double radius);

int findElement(const vector<DGtal::Z2i::RealPoint>& vec, DGtal::Z2i::RealPoint p);
int findElement(const vector<DGtal::Z2i::RealPoint>& vec, DGtal::Z2i::RealPoint p, int start);
int findStartElement(const vector<DGtal::Z2i::RealPoint>& vec, const AlphaThickSegmentComputer2DD s);
int findEndElement(const vector<DGtal::Z2i::RealPoint>& vec, const AlphaThickSegmentComputer2DD s);

double error_CR(const vector<DGtal::Z2i::RealPoint>& contour, const vector<DGtal::Z2i::RealPoint>& DP);
double error_ISE(const vector<DGtal::Z2i::RealPoint>& contour, const vector<DGtal::Z2i::RealPoint>& DP, const vector<int>& indexDP, bool isClosed=false);
double error_L_infini(const vector<DGtal::Z2i::RealPoint>& contour, const vector<DGtal::Z2i::RealPoint>& DP, const vector<int>& indexDP, bool isClosed=false);
double error_FOM(const vector<DGtal::Z2i::RealPoint>& contour, const vector<DGtal::Z2i::RealPoint>& DP, const vector<int>& indexDP, bool isClosed=false);
double error_FOM_M(const vector<DGtal::Z2i::RealPoint>& contour, const vector<DGtal::Z2i::RealPoint>& DP, const vector<int>& indexDP, bool isClosed=false);
double error_FOM_ND(const vector<DGtal::Z2i::RealPoint>& contour, const vector<DGtal::Z2i::RealPoint>& DP, const vector<int>& indexDP, bool isClosed=false);
void error_All(const vector<DGtal::Z2i::RealPoint>& contour, const vector<DGtal::Z2i::RealPoint>& DP, const vector<int>& indexDP, bool isClosed=false);

int isLeft(DGtal::Z2i::RealPoint p1, DGtal::Z2i::RealPoint p2, DGtal::Z2i::RealPoint p3);

DGtal::Z2i::RealPoint determineCenter(DGtal::Z2i::RealPoint p1, DGtal::Z2i::RealPoint p2, DGtal::Z2i::RealPoint p3);
double determineRadius(DGtal::Z2i::RealPoint p1, DGtal::Z2i::RealPoint p2, DGtal::Z2i::RealPoint p3);

DGtal::Z2i::RealPoint determineCenter(DGtal::Z2i::Point p1, DGtal::Z2i::Point p2, DGtal::Z2i::Point p3);
DGtal::Z2i::RealPoint determineCenter(DGtal::Z2i::Point p1, DGtal::Z2i::Point p2, double radius, bool negatif);
double determineRadius(DGtal::Z2i::Point p1, DGtal::Z2i::Point p2, DGtal::Z2i::Point p3);
double determineRadius(DGtal::Z2i::RealPoint centre, DGtal::Z2i::Point p);
double arcLength(DGtal::Z2i::Point p1, DGtal::Z2i::Point p2, DGtal::Z2i::Point p3);

double iseContourCircle(const vector<DGtal::Z2i::RealPoint>& contour, DGtal::Z2i::Point p1, DGtal::Z2i::Point p2, DGtal::Z2i::RealPoint center, double radius);//ise of points between two points p1,p2 w.r.t the circle (center,radius)
double iseContourCircle(const vector<DGtal::Z2i::RealPoint>& contour, int indexP1, int indexP2, DGtal::Z2i::RealPoint center, double radius);//ise of points between two points of index idp1,idp2 w.r.t the circle (center,radius)
double lmaxContourCircle(const vector<DGtal::Z2i::RealPoint>& contour, DGtal::Z2i::Point p1, DGtal::Z2i::Point p2, DGtal::Z2i::RealPoint center, double radius);//lmax
double lmaxContourCircle(const vector<DGtal::Z2i::RealPoint>& contour, int indexP1, int indexP2, DGtal::Z2i::RealPoint center, double radius);//lmax

double iseContourSegment(const vector<DGtal::Z2i::RealPoint>& contour, DGtal::Z2i::Point p1, DGtal::Z2i::Point p2);//ise of points between two points p1,p2 w.r.t the circle (center,radius)
double iseContourSegment(const vector<DGtal::Z2i::RealPoint>& contour, int indexP1, int indexP2);//ise of points between two points of index idp1,idp2 w.r.t the circle (center,radius)
double lmaxContourSegment(const vector<DGtal::Z2i::RealPoint>& contour, DGtal::Z2i::Point p1, DGtal::Z2i::Point p2);//lmax
double lmaxContourSegment(const vector<DGtal::Z2i::RealPoint>& contour, int indexP1, int indexP2);//lmax

#endif // MYFUNCTIONS


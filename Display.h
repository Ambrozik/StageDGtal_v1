//
// Created by ambrozik on 16/04/2021.
//
#include "DGtal/io/DrawWithDisplay3DModifier.h"
#include "DGtal/io/readers/PointListReader.h"
#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/boards/Board2D.h"
#include <map>
#include "AlphaThickSegment3D.h"
#ifndef ALPHATEST2_DISPLAY_H
#define ALPHATEST2_DISPLAY_H
const DGtal::Color  AXIS_COLOR( 0, 0, 0, 255 );
const double AXIS_LINESIZE = 0.1;
const DGtal::Color  XY_COLOR( 0, 0, 255, 50 );
const DGtal::Color  XZ_COLOR( 0, 255, 0, 50 );
const DGtal::Color  YZ_COLOR( 255, 0, 0, 50 );
const DGtal::Color  CURVE3D_COLOR( 100, 100, 140, 128 );
const DGtal::Color  CURVE2D_COLOR( 200, 200, 200, 100 );

//display AXISs 3D
template <typename Point, typename RealPoint, typename space, typename kspace >
void displayAXISs( DGtal::Viewer3D<space, kspace> & viewer,const Point & upperBound,const Point & lowerBound, const std::string & mode );

//display projection  Point2D
template<typename Point, typename RealPoint, typename space, typename kspace, typename Point3D>
void displayPoint2D(DGtal::Viewer3D<space, kspace> & viewer,const DGtal::Z3i::KSpace & ks, std::vector<Point3D> sequence, std::string mode);

//display AlphaThickSegments3D
template<typename Point,typename space,typename kspace,typename AlphaThickSegmentComputer2D>
void displayAlphathickSemgent(DGtal::Viewer3D<space, kspace> & viewer,const DGtal::Z3i::KSpace & ks,AlphaThickSegmentComputer2D line,const Line3D &l,const AXIS &a);

template <typename Point, typename RealPoint, typename space, typename kspace >
void displayAXISs( DGtal::Viewer3D<space, kspace> & viewer,const Point & upperBound,const Point & lowerBound, const std::string & mode )
{
    RealPoint p0( (double)lowerBound[ 0 ],(double)lowerBound[ 1 ],(double)lowerBound[ 2 ]);
    RealPoint p1( (double)upperBound[ 0 ],(double)upperBound[ 1 ],(double)upperBound[ 2 ]);
    if ( ( mode == "WIRED" ) || ( mode == "COLORED" ) ){
        viewer.setLineColor(AXIS_COLOR);
        viewer.addLine( DGtal::Z3i::RealPoint(p0[ 0 ], p0[ 1 ], p0[ 2 ]),
                        DGtal::Z3i::RealPoint(p1[ 0 ], p0[ 1 ], p0[ 2 ]),  AXIS_LINESIZE );
        viewer.addLine( DGtal::Z3i::RealPoint(p0[ 0 ], p0[ 1 ], p0[ 2 ]),
                        DGtal::Z3i::RealPoint(p0[ 0 ], p1[ 1 ], p0[ 2 ]),  AXIS_LINESIZE );
        viewer.addLine( DGtal::Z3i::RealPoint(p0[ 0 ], p0[ 1 ], p0[ 2 ]),
                        DGtal::Z3i::RealPoint(p0[ 0 ], p0[ 1 ], p1[ 2 ]),  AXIS_LINESIZE );
        viewer.addLine( DGtal::Z3i::RealPoint(p1[ 0 ], p0[ 1 ], p0[ 2 ]),
                        DGtal::Z3i::RealPoint(p1[ 0 ], p1[ 1 ], p0[ 2 ]),  AXIS_LINESIZE );
        viewer.addLine( DGtal::Z3i::RealPoint(p1[ 0 ], p0[ 1 ], p0[ 2 ]),
                        DGtal::Z3i::RealPoint(p1[ 0 ], p0[ 1 ], p1[ 2 ]),  AXIS_LINESIZE );
        viewer.addLine( DGtal::Z3i::RealPoint(p0[ 0 ], p1[ 1 ], p0[ 2 ]),
                        DGtal::Z3i::RealPoint(p1[ 0 ], p1[ 1 ], p0[ 2 ]),  AXIS_LINESIZE );
        viewer.addLine( DGtal::Z3i::RealPoint(p0[ 0 ], p1[ 1 ], p0[ 2 ]),
                        DGtal::Z3i::RealPoint(p0[ 0 ], p1[ 1 ], p1[ 2 ]),  AXIS_LINESIZE );
        viewer.addLine( DGtal::Z3i::RealPoint(p0[ 0 ], p0[ 1 ], p1[ 2 ]),
                        DGtal::Z3i::RealPoint(p1[ 0 ], p0[ 1 ], p1[ 2 ]),  AXIS_LINESIZE );
        viewer.addLine( DGtal::Z3i::RealPoint(p0[ 0 ], p0[ 1 ], p1[ 2 ]),
                        DGtal::Z3i::RealPoint(p0[ 0 ], p1[ 1 ], p1[ 2 ]),  AXIS_LINESIZE );
        viewer.addLine( DGtal::Z3i::RealPoint(p1[ 0 ], p1[ 1 ], p0[ 2 ]),
                        DGtal::Z3i::RealPoint(p1[ 0 ], p1[ 1 ], p1[ 2 ]),  AXIS_LINESIZE );
        viewer.addLine( DGtal::Z3i::RealPoint(p1[ 0 ], p0[ 1 ], p1[ 2 ]),
                        DGtal::Z3i::RealPoint(p1[ 0 ], p1[ 1 ], p1[ 2 ]),  AXIS_LINESIZE );
        viewer.addLine( DGtal::Z3i::RealPoint(p0[ 0 ], p1[ 1 ], p1[ 2 ]),
                        DGtal::Z3i::RealPoint(p1[ 0 ], p1[ 1 ], p1[ 2 ]),  AXIS_LINESIZE );
    }
    if ( mode == "COLORED" ){
        viewer.setFillColor(XY_COLOR);
        viewer.addQuad(DGtal::Z3i::RealPoint(p1[ 0 ], p1[ 1 ], p1[ 2 ]),
                       DGtal::Z3i::RealPoint(p1[ 0 ], p0[ 1 ], p1[ 2 ]),
                       DGtal::Z3i::RealPoint(p0[ 0 ], p0[ 1 ], p1[ 2 ]),
                       DGtal::Z3i::RealPoint(p0[ 0 ], p1[ 1 ], p1[ 2 ]) );
        viewer.setFillColor(XZ_COLOR);
        viewer.addQuad(DGtal::Z3i::RealPoint(p1[ 0 ], p1[ 1 ], p1[ 2 ]),
                       DGtal::Z3i::RealPoint(p0[ 0 ], p1[ 1 ], p1[ 2 ]),
                       DGtal::Z3i::RealPoint(p0[ 0 ], p1[ 1 ], p0[ 2 ]),
                       DGtal::Z3i::RealPoint(p1[ 0 ], p1[ 1 ], p0[ 2 ]));
        viewer.setFillColor(YZ_COLOR);
        viewer.addQuad(DGtal::Z3i::RealPoint(p1[ 0 ], p1[ 1 ], p1[ 2 ]),
                       DGtal::Z3i::RealPoint(p1[ 0 ], p0[ 1 ], p1[ 2 ]),
                       DGtal::Z3i::RealPoint(p1[ 0 ], p0[ 1 ], p0[ 2 ]),
                       DGtal::Z3i::RealPoint(p1[ 0 ], p1[ 1 ], p0[ 2 ]));
    }
}

template<typename Point, typename RealPoint, typename space, typename kspace, typename Point3D>
void displayPoint2D(DGtal::Viewer3D<space, kspace> & viewer,const DGtal::Z3i::KSpace & ks, std::vector<Point3D> sequence, std::string mode){
    typedef typename DGtal::Z3i::KSpace::Cell cell;
    typedef typename DGtal::Z3i::KSpace::Point Point3d;
    auto it =sequence.begin();
    typedef typename DGtal::Z3i::KSpace::Point Point3d;
    typedef typename DGtal::Z3i::KSpace::Cell cell;
    Point3d b = ks.lowerBound();
    for (;it != sequence.end(); ++it ) {
        Point xy = convert3Dto2DAXIS<Point, Point3D>(*it, AXIS::XY);
        Point xz = convert3Dto2DAXIS<Point, Point3D>(*it, AXIS::XZ);
        Point yz = convert3Dto2DAXIS<Point, Point3D>(*it, AXIS::YZ);
        if (mode =="Point"){
            Point3d q1(xy[ 0 ],  xy[ 1 ],  b [ 2 ] );
            Point3d q2(xz[ 0 ],  b [ 1 ],  xz[ 1 ] );
            Point3d q3(b [ 0 ],  yz[ 0 ],  yz[ 1 ]);
            viewer <<  q1;
            viewer <<  q2;
            viewer <<  q3;

        }
        if (mode =="Cell"){
            Point3d q1(2*xy[ 0 ]+1,  2*xy[ 1 ]+1,2*b[ 2 ] );
            Point3d q2(2*xz[ 0 ]+1,  2*b[ 1 ],2*xz[ 1 ]+1 );
            Point3d q3(2*b[ 0 ],2*yz[ 0 ]+1,  2*yz[ 1 ]+1 );
            cell c1 = ks.uCell( q1 );
            cell c2 = ks.uCell( q2 );
            cell c3 = ks.uCell( q3 );
            viewer <<  c1;
            viewer <<  c2;
            viewer <<  c3;
        }


    }
}



template<typename Point, typename RealPoint, typename space, typename kspace, typename Point3D>
void displayPoint2DNoisesColor(DGtal::Viewer3D<space, kspace> & viewer,const DGtal::Z3i::KSpace & ks,const std::vector<std::vector<double>> vecMT, std::vector<Point3D> sequence, std::string mode){
    std::unordered_set<double> my_set_noises;
    for(int i = 0 ; i < vecMT.at(0).size(); i++){
        my_set_noises.insert(vecMT.at(0).at(i));
        my_set_noises.insert(vecMT.at(1).at(i));
        my_set_noises.insert(vecMT.at(2).at(i));
    }

    std::map<int,double> nbTypeSegment;
    int i = 0;
    for(std::unordered_set<double>::const_iterator it = my_set_noises.begin(); it != my_set_noises.end(); it++)
    {
        double m = (*it);
        nbTypeSegment[i] = m ;
        i++;
    }

    HueShadeColorMap<float,1>  hueShade(0,my_set_noises.size());
    typedef typename DGtal::Z3i::KSpace::Cell cell;
    typedef typename DGtal::Z3i::KSpace::Point Point3d;
    auto it =sequence.begin();
    typedef typename DGtal::Z3i::KSpace::Point Point3d;
    typedef typename DGtal::Z3i::KSpace::Cell cell;
    Point3d b = ks.lowerBound();
    i = 0;
    for (;it != sequence.end(); ++it ) {
        auto idx = nbTypeSegment.find(vecMT.at(0).at(i));
        int idxColor = idx->first;
        Color colXy= hueShade(idxColor);

        idx = nbTypeSegment.find(vecMT.at(1).at(i));
        idxColor = idx->first;
        Color colXz = hueShade(idxColor);

        idx = nbTypeSegment.find(vecMT.at(2).at(i));
        idxColor = idx->first;
        Color colYz = hueShade(idxColor);
        Point xy = convert3Dto2DAXIS<Point, Point3D>(*it, AXIS::XY);
        Point xz = convert3Dto2DAXIS<Point, Point3D>(*it, AXIS::XZ);
        Point yz = convert3Dto2DAXIS<Point, Point3D>(*it, AXIS::YZ);
        if (mode =="Point"){

            Point3d q1(xy[ 0 ],  xy[ 1 ],  b [ 2 ] );
            Point3d q2(xz[ 0 ],  b [ 1 ],  xz[ 1 ] );
            Point3d q3(b [ 0 ],  yz[ 0 ],  yz[ 1 ]);
            viewer <<CustomColors3D(colXy,colXy);
            viewer <<  q1;
            viewer <<CustomColors3D(colXz,colXz);
            viewer <<  q2;
            viewer <<CustomColors3D(colYz,colYz);
            viewer <<  q3;

        }
        if (mode =="Cell"){
            Point3d q1(2*xy[ 0 ]+1,  2*xy[ 1 ]+1,2*b[ 2 ] );
            Point3d q2(2*xz[ 0 ]+1,  2*b[ 1 ],2*xz[ 1 ]+1 );
            Point3d q3(2*b[ 0 ],2*yz[ 0 ]+1,  2*yz[ 1 ]+1 );
            cell c1 = ks.uCell( q1 );
            cell c2 = ks.uCell( q2 );
            cell c3 = ks.uCell( q3 );
            viewer <<CustomColors3D(colXy,colXy);
            viewer <<  c1;
            viewer <<CustomColors3D(colXz,colXz);
            viewer <<  c2;
            viewer <<CustomColors3D(colYz,colYz);
            viewer <<  c3;
        }
        i++;
    }
}

template<typename Point,typename space,typename kspace,typename AlphaThickSegmentComputer2D>
void displayAlphathickSemgent(DGtal::Viewer3D<space, kspace> & viewer,const DGtal::Z3i::KSpace & ks,AlphaThickSegmentComputer2D line,const Line3D &l,const AXIS &a){
    typedef typename DGtal::Z3i::KSpace::Point Point3d;
    typedef typename DGtal::PointVector<3,double> Point3D;
    Point pt0, pt1, pt2, pt3;
    line.getBoundingBox(pt0, pt1, pt2, pt3);
    Point3D fp = *l.Points3D.begin();
    Point3D fe = l.Points3D.back();

    Point3D p0, p1, p2, p3;
    Point3d b = ks.lowerBound();
    viewer.setLineColor(AXIS_COLOR);
    switch (a) {
        case AXIS::XY:
            p0 = DGtal::PointVector<3,double>(pt0[0], pt0[1],b[2]);
            p1 = DGtal::PointVector<3,double>(pt1[0], pt1[1],b[2]);
            p2 = DGtal::PointVector<3,double>(pt2[0], pt2[1],b[2]);
            p3 = DGtal::PointVector<3,double>(pt3[0], pt3[1],b[2]);
            break;
        case AXIS::XZ:
            p0 = DGtal::PointVector<3,double>(pt0[0], b[1],pt0[1]);
            p1 = DGtal::PointVector<3,double>(pt1[0],b[1], pt1[1]);
            p2 = DGtal::PointVector<3,double>(pt2[0],b[1], pt2[1]);
            p3 = DGtal::PointVector<3,double>(pt3[0],b[1], pt3[1]);
            break;
        case AXIS::YZ:
            p0 = DGtal::PointVector<3,double>( b[0],pt0[0],pt0[1]);
            p1 = DGtal::PointVector<3,double>(b[0],pt1[0], pt1[1]);
            p2 = DGtal::PointVector<3,double>(b[0],pt2[0], pt2[1]);
            p3 = DGtal::PointVector<3,double>(b[0],pt3[0], pt3[1]);
            break;
        default:
            assert("AXIS error displayAlphathickSemgent");
    }

    std::vector<Point3D> listPoint ={p0,p1,p2,p3};
    viewer.addLine( DGtal::Z3i::RealPoint(p0),
                    DGtal::Z3i::RealPoint(p1),  AXIS_LINESIZE );

    viewer.addLine( DGtal::Z3i::RealPoint(p1),
                    DGtal::Z3i::RealPoint(p2),  AXIS_LINESIZE );

    viewer.addLine( DGtal::Z3i::RealPoint(p2),
                    DGtal::Z3i::RealPoint(p3),  AXIS_LINESIZE );

    viewer.addLine( DGtal::Z3i::RealPoint(p3),
                    DGtal::Z3i::RealPoint(p0),  AXIS_LINESIZE );
}


template<typename Point,typename space,typename kspace>
void drawAsBoundingBox(DGtal::Viewer3D<space, kspace> & viewer,const DGtal::Z3i::KSpace & ks,const Line3D &l, int plan){
    typedef DGtal::PointVector<3,double> PointD3d;
    typedef DGtal::PointVector<3,double> VectorD3d;
    PointD3d mu;
    PointD3d omega;
        mu[0] = (l.intercept[0].first) / (l.intercept[0].second);
        mu[1] = (l.intercept[1].first) / (l.intercept[1].second);
        mu[2] = (l.intercept[2].first) / (l.intercept[2].second);

        omega[0] = (l.thickness[0].first) / (l.thickness[0].second);
        omega[1] = (l.thickness[1].first) / (l.thickness[1].second);
        omega[2] = (l.thickness[2].first) / (l.thickness[2].second);


    //casting coordinates of v in double
    VectorD3d u = VectorD3d((l.direction[0]),
                            (l.direction[1]),
                            (l.direction[2]));




    //Squared L2 norm of u
    double n = u[0]*u[0] + u[1]*u[1] + u[2]*u[2];
    //first and last points
    PointD3d first = *l.Points3D.begin();
    PointD3d last = l.extremity.second;
    PointD3d f = PointD3d((first[0]),
                          (first[1]),
                          (first[2]) );
    PointD3d l1= PointD3d((last[0]),
                          (last[1]),
                          (last[2]));

    if (n != 0) {

        //last coefficient of the normal plane to the direction
        //passing trough f and l
        double df = -u[0]*f[0] -u[1]*f[1] -u[2]*f[2];
        double dl = -u[0]*l1[0] -u[1]*l1[1] -u[2]*l1[2];
        //omega masks
        PointD3d omega1, omega2;
        if (omega[0] == 0) {
            omega1 = PointD3d(0,omega[1],0);
            omega2 = PointD3d(0,0,omega[2]);
        } else {
        if (omega[1] == 0) {
            omega1 = PointD3d(omega[0],0,0);
            omega2 = PointD3d(0,0,omega[2]);
        } else {//omega[2] == 0
            omega1 = PointD3d(omega[0],0,0);
            omega2 = PointD3d(0,omega[1],0);
        }
    }

    double m1 = u[0]*mu[0] + u[1]*mu[1] + u[2]*mu[2];
    double m2 = u[0]*(mu[0]+omega1[0]) + u[1]*(mu[1]+omega1[1]) + u[2]*(mu[2]+omega1[2]);
    double m3 = u[0]*(mu[0]+omega2[0]) + u[1]*(mu[1]+omega2[1]) + u[2]*(mu[2]+omega2[2]);
    double m4 = u[0]*(mu[0]+omega[0]) + u[1]*(mu[1]+omega[1]) + u[2]*(mu[2]+omega[2]);

    if(plan == 0 ) viewer.setLineColor(DGtal::Color( 0, 0, 255, 255 ));
    if(plan == 1 ) viewer.setLineColor(DGtal::Color( 0, 255, 0, 255 ));
    if(plan == 2 ) viewer.setLineColor(DGtal::Color( 255, 0, 0, 255 ));

    Point pt0, pt1, pt2, pt3;
    l.line3D.first.alphaSeg.getBoundingBox(pt0, pt1, pt2, pt3);

    Point pt0p, pt1p, pt2p, pt3p;
    l.line3D.second.alphaSeg.getBoundingBox(pt0p, pt1p, pt2p, pt3p);
        //4 lines
    PointD3d pf = PointD3d( mu[0] - ( (m1+df)*u[0] )/n,
                            mu[1] - ( (m1+df)*u[1] )/n,
                            mu[2] - ( (m1+df)*u[2] )/n );
    PointD3d pl = PointD3d( mu[0] - ( (m1+dl)*u[0] )/n,
                            mu[1] - ( (m1+dl)*u[1] )/n,
                            mu[2] - ( (m1+dl)*u[2] )/n );

    viewer.addLine(pf, pl);
    PointD3d pf2 = PointD3d((mu[0]+omega1[0]) - ( (m2+df)*u[0] )/n,
                            (mu[1]+omega1[1]) - ( (m2+df)*u[1] )/n,
                            (mu[2]+omega1[2]) - ( (m2+df)*u[2] )/n );
    PointD3d pl2 = PointD3d((mu[0]+omega1[0]) - ( (m2+dl)*u[0] )/n,
                            (mu[1]+omega1[1]) - ( (m2+dl)*u[1] )/n,
                            (mu[2]+omega1[2]) - ( (m2+dl)*u[2] )/n );

    viewer.addLine(pf2, pl2);

    PointD3d pf3 = PointD3d((mu[0]+omega2[0]) - ( (m3+df)*u[0] )/n,
                            (mu[1]+omega2[1]) - ( (m3+df)*u[1] )/n,
                            (mu[2]+omega2[2]) - ( (m3+df)*u[2] )/n );

    PointD3d pl3 = PointD3d((mu[0]+omega2[0]) - ( (m3+dl)*u[0] )/n,
                            (mu[1]+omega2[1]) - ( (m3+dl)*u[1] )/n,
                            (mu[2]+omega2[2]) - ( (m3+dl)*u[2] )/n );

    viewer.addLine(pf3, pl3);

    PointD3d pf4 = PointD3d((mu[0]+omega[0]) - ( (m4+df)*u[0] )/n,
                            (mu[1]+omega[1]) - ( (m4+df)*u[1] )/n,
                            (mu[2]+omega[2]) - ( (m4+df)*u[2] )/n );
    PointD3d pl4 = PointD3d((mu[0]+omega[0]) - ( (m4+dl)*u[0] )/n,
                            (mu[1]+omega[1]) - ( (m4+dl)*u[1] )/n,
                            (mu[2]+omega[2]) - ( (m4+dl)*u[2] )/n );

    viewer.addLine(pf4, pl4);

//    two end facets

    viewer.addLine(pf, pf2);
    viewer.addLine(pf2,pf4);
    viewer.addLine(pf4, pf3);
    viewer.addLine(pf3, pf);

    viewer.addLine(pl, pl2);
    viewer.addLine(pl2, pl4);
    viewer.addLine(pl4, pl3);
    viewer.addLine(pl3, pl);
    }
    viewer.setLineColor(DGtal::Color( 0, 0, 0, 100 ));
}

template<typename Point,typename space,typename kspace>
void drawAsBoundingBox(DGtal::Viewer3D<space, kspace> & viewer,const DGtal::Z3i::KSpace & ks,const Line3D &l, DGtal::Color color ){
    typedef DGtal::PointVector<3,double> PointD3d;
    typedef DGtal::PointVector<3,double> VectorD3d;
    viewer.setLineColor(color);
    PointD3d mu;
    PointD3d omega;
    mu[0] = (l.intercept[0].first) / (l.intercept[0].second);
    mu[1] = (l.intercept[1].first) / (l.intercept[1].second);
    mu[2] = (l.intercept[2].first) / (l.intercept[2].second);

    omega[0] = (l.thickness[0].first) / (l.thickness[0].second);
    omega[1] = (l.thickness[1].first) / (l.thickness[1].second);
    omega[2] = (l.thickness[2].first) / (l.thickness[2].second);


    //casting coordinates of v in double
    VectorD3d u = VectorD3d((l.direction[0]),
                            (l.direction[1]),
                            (l.direction[2]));
    //Squared L2 norm of u
    double n = u[0]*u[0] + u[1]*u[1] + u[2]*u[2];
    //first and last points
    PointD3d first = *l.Points3D.begin();
    PointD3d last = l.extremity.second;
    PointD3d f = PointD3d((first[0]),
                          (first[1]),
                          (first[2]) );
    PointD3d l1= PointD3d((last[0]),
                          (last[1]),
                          (last[2]));

    if (n != 0) {

        //last coefficient of the normal plane to the direction
        //passing trough f and l
        double df = -u[0]*f[0] -u[1]*f[1] -u[2]*f[2];
        double dl = -u[0]*l1[0] -u[1]*l1[1] -u[2]*l1[2];
        //omega masks
        PointD3d omega1, omega2;
        if (omega[0] == 0) {
            omega1 = PointD3d(0,omega[1],0);
            omega2 = PointD3d(0,0,omega[2]);
        } else {
            if (omega[1] == 0) {
                omega1 = PointD3d(omega[0],0,0);
                omega2 = PointD3d(0,0,omega[2]);
            } else {//omega[2] == 0
                omega1 = PointD3d(omega[0],0,0);
                omega2 = PointD3d(0,omega[1],0);
            }
        }

        double m1 = u[0]*mu[0] + u[1]*mu[1] + u[2]*mu[2];
        double m2 = u[0]*(mu[0]+omega1[0]) + u[1]*(mu[1]+omega1[1]) + u[2]*(mu[2]+omega1[2]);
        double m3 = u[0]*(mu[0]+omega2[0]) + u[1]*(mu[1]+omega2[1]) + u[2]*(mu[2]+omega2[2]);
        double m4 = u[0]*(mu[0]+omega[0]) + u[1]*(mu[1]+omega[1]) + u[2]*(mu[2]+omega[2]);



        Point pt0, pt1, pt2, pt3;
        l.line3D.first.alphaSeg.getBoundingBox(pt0, pt1, pt2, pt3);

        Point pt0p, pt1p, pt2p, pt3p;
        l.line3D.second.alphaSeg.getBoundingBox(pt0p, pt1p, pt2p, pt3p);
        //4 lines
        PointD3d pf = PointD3d( mu[0] - ( (m1+df)*u[0] )/n,
                                mu[1] - ( (m1+df)*u[1] )/n,
                                mu[2] - ( (m1+df)*u[2] )/n );
        PointD3d pl = PointD3d( mu[0] - ( (m1+dl)*u[0] )/n,
                                mu[1] - ( (m1+dl)*u[1] )/n,
                                mu[2] - ( (m1+dl)*u[2] )/n );

        viewer.addLine(pf, pl);
        PointD3d pf2 = PointD3d((mu[0]+omega1[0]) - ( (m2+df)*u[0] )/n,
                                (mu[1]+omega1[1]) - ( (m2+df)*u[1] )/n,
                                (mu[2]+omega1[2]) - ( (m2+df)*u[2] )/n );
        PointD3d pl2 = PointD3d((mu[0]+omega1[0]) - ( (m2+dl)*u[0] )/n,
                                (mu[1]+omega1[1]) - ( (m2+dl)*u[1] )/n,
                                (mu[2]+omega1[2]) - ( (m2+dl)*u[2] )/n );

        viewer.addLine(pf2, pl2);

        PointD3d pf3 = PointD3d((mu[0]+omega2[0]) - ( (m3+df)*u[0] )/n,
                                (mu[1]+omega2[1]) - ( (m3+df)*u[1] )/n,
                                (mu[2]+omega2[2]) - ( (m3+df)*u[2] )/n );

        PointD3d pl3 = PointD3d((mu[0]+omega2[0]) - ( (m3+dl)*u[0] )/n,
                                (mu[1]+omega2[1]) - ( (m3+dl)*u[1] )/n,
                                (mu[2]+omega2[2]) - ( (m3+dl)*u[2] )/n );

        viewer.addLine(pf3, pl3);

        PointD3d pf4 = PointD3d((mu[0]+omega[0]) - ( (m4+df)*u[0] )/n,
                                (mu[1]+omega[1]) - ( (m4+df)*u[1] )/n,
                                (mu[2]+omega[2]) - ( (m4+df)*u[2] )/n );
        PointD3d pl4 = PointD3d((mu[0]+omega[0]) - ( (m4+dl)*u[0] )/n,
                                (mu[1]+omega[1]) - ( (m4+dl)*u[1] )/n,
                                (mu[2]+omega[2]) - ( (m4+dl)*u[2] )/n );

        viewer.addLine(pf4, pl4);

//    two end facets

        viewer.addLine(pf, pf2);
        viewer.addLine(pf2,pf4);
        viewer.addLine(pf4, pf3);
        viewer.addLine(pf3, pf);

        viewer.addLine(pl, pl2);
        viewer.addLine(pl2, pl4);
        viewer.addLine(pl4, pl3);
        viewer.addLine(pl3, pl);
    }
    viewer.setLineColor(DGtal::Color( 0, 0, 0, 100 ));
}

template<typename Point,typename space,typename kspace>
void displayNormal3D(DGtal::Viewer3D<space, kspace> & viewer,const DGtal::Z3i::KSpace & ks,const Line3D &l){
    DGtal::PointVector<3,double> N = l.direction.getNormalized();

    viewer.setLineColor(DGtal::Color( 255, 0, 0, 255 ));
    viewer.addLine(l.Points3D[0],l.Points3D[0] + N*2,AXIS_LINESIZE);
    viewer.setLineColor(AXIS_COLOR);
}
template<typename Point3D,typename space,typename kspace>
void SequenceLine(DGtal::Viewer3D<space, kspace> & viewer,const DGtal::Z3i::KSpace & ks,std::vector<Point3D> sequence){
    std::vector<DGtal::Z3i::Point>::const_iterator it = sequence.begin();
    Point3D prevp = *it;
    DGtal::Z3i::RealPoint rprevp = viewer.embed( prevp);
    ++it;
    for ( ; it != sequence.end(); ++it) {
        Point3D p = *it;
        DGtal::Z3i::RealPoint rp = viewer.embed( p );
        viewer.addLine(rprevp,rp);
        rprevp = rp;
    }
}


template<typename Point3D,typename space,typename kspace>
void drawLambdaMSTestimator(DGtal::Viewer3D<space, kspace> & viewer,const DGtal::Z3i::KSpace & ks,const std::vector<Point3D> &sequence, const std::vector<DGtal::PointVector<3,double>> & V){
    for(int i = 0 ; i < sequence.size(); i++){
        viewer.setLineColor(Color(255,0,0));
        float normC = sqrt(pow(V.at(i)[0],2) + pow(V.at(i)[1],2) + pow(V.at(i)[2],2));
        DGtal::PointVector<3,double> p;
        p = (V.at(i))/normC + sequence.at(i);
        viewer.addLine(sequence.at(i), p  );
    }
}

template<typename Point3D,typename space,typename kspace>
void drawLambdaMSTestimatorNormale(DGtal::Viewer3D<space, kspace> & viewer,const DGtal::Z3i::KSpace & ks,const std::vector<Point3D> &sequence, const std::vector<DGtal::PointVector<3,double>> & V){
    for(int i = 0 ; i < sequence.size(); i++){
        viewer.setLineColor(Color(164,62,162));
        float normC = sqrt(pow(V.at(i)[0],2) + pow(V.at(i)[1],2) + pow(V.at(i)[2],2));
        DGtal::PointVector<3,double> p ,p1, p2,v1,v2,n,b;
        p = sequence.at(i);
        p1 = sequence.at(i);
        p1[2] = sequence.at(i)[2] -1 ;
        p2 = (V.at(i))/normC + sequence.at(i);
        v1 = p1 - p ;
        v2 = p2 -p ;
        n = crossProduct(v1,v2);
        float normC2 = sqrt(pow(n[0],2) + pow(n[1],2) + pow(n[2],2));
        b = n/normC2 + sequence.at(i);
        viewer.addLine(sequence.at(i), b );

    }
}


#endif //ALPHATEST2_DISPLAY_H

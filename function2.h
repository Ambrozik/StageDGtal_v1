//
// Created by ambrozik on 16/04/2021.
//

#ifndef STAGEDGTAL_FUNCTION2_H
#define STAGEDGTAL_FUNCTION2_H
#include <fstream>
#include  "math.h"

#include <iostream>
#include <string>
#include <regex>

using namespace DGtal;
template<typename Point3D, typename Point2D>
Point2D convert3Dto2DAXIS(const Point3D &p3D,AXIS AXIS);

/// \brief creation of a File with information de build segments 3D
/// \tparam Point3D point3D (x,y,z)
/// \param sequence curve 3D ( Point 3D list)
/// \param listSegment3D  (blurred segment list)
/// \param fileName file name
/// \param commentary comment about the file
template<typename Point3D>
void toFile(std::vector<Point3D> sequence,std::vector<Line3D>  listSegment3D,std::string fileName, std::string commentary);


/// \brief function to convert  axis index to enum AXIS
/// \param a index of combination plan
/// \return pair of plan
AXIS getAxes(std::string a);


/// \brief Got  all segment3D from file
/// \tparam Point3D point 3D (x,y,z)
/// \tparam AlphaThickSegmentComputer2D pair of blurred segment 2D to create blurred segment 3D
/// \param fileName  name of the file with the description of a 3D segmentation
/// \param listAllPoint3D list of point3D of curve
/// \return
template<typename Point3D,typename AlphaThickSegmentComputer2D>
std::vector<Line3D>  createBDSfromFile(std::string fileName, std::vector<Point3D> *listAllPoint3D);

/// \brief recover noise from file
/// \param fileName name of the file with the noise list by plans and by point
/// \return noisy list
std::vector<std::vector<double>> getNoisy(std::string fileName);


/// \brief convert point3D to point2D depending on the axis
/// \tparam Point3D point 3D (x,y,z)
/// \tparam Point2D point 2D (x,y)
/// \param p3D  point 3D to convert
/// \param AXIS projection plan
/// \return point 2D converted
template<typename Point3D, typename Point2D>
Point2D convert3Dto2DAXIS(const Point3D &p3D,AXIS AXIS);



/// \brief compute next point
/// \tparam Point3D point 3D (x,y,z)
/// \tparam Point2D point 2D (x,y)
/// \param sequence list point
/// \param p point 3D
/// \param AXIS  plan
/// \param indice index
/// \return next point
template<typename Point3D, typename Point2D>
Point2D nextPoint(const  std::vector<Point3D> & sequence,const Point2D &p, AXIS AXIS, int &indice);

/// \brief compute gcd
/// \param a
/// \param b
/// \return gcd
long long gcd(long long int a, long long int b);

/// \brief function to return LCM of two numbers
/// \param a
/// \param b
/// \return lcm
long long lcm(int a, int b);


///\brief euclidean distance 2 points2D
/// \tparam Point2D point 2D (x,y)
/// \param point2Da first point
/// \param point2Db second point
/// \return euclidean distance
template <typename Point2D>
double distance(Point2D point2Da, Point2D point2Db);

/// \brief euclidean distance 2 points3D
/// \tparam Point3D point 3D (x,y,z)
/// \param point3Da first point
/// \param point3Db second point
/// \return euclidean distance
template<typename Point3D>
double distance_3D(Point3D point3Da, Point3D point3Db);



template<typename Point3D>
void toFile(std::vector<Point3D> sequence,std::vector<Line3D>  listSegment3D,std::string fileName, std::string commentary){
    std::ofstream f(fileName + ".txt");
    //whrite all points 3D | x y z |
    for(int j = 0 ; j < sequence.size(); j++){
        f << sequence.at(j)[0] << " "
        <<sequence.at(j)[1]<< " "
        <<sequence.at(j)[2] << "|" ;
    }
    f << std::endl;
    //commentary
    f << "#" << commentary << std::endl;
    for( int i = 0 ; i < listSegment3D.size(); i++){
        //projection
        f << listSegment3D.at(i).axes.first <<" " << listSegment3D.at(i).axes.second <<std::endl;
        //point3D list
        for(int j = 0 ; j < listSegment3D.at(i).Points3D.size(); j++){
            f << listSegment3D.at(i).Points3D.at(j)[0] << " "
            <<listSegment3D.at(i).Points3D.at(j)[1]<< " "
            <<listSegment3D.at(i).Points3D.at(j)[2] << "|" ;
        }
        f << std::endl;
        //extremity
        f << listSegment3D.at(i).indexExtremity.first <<" "<<  listSegment3D.at(i).indexExtremity.second ;
        f << std::endl;
        //definition
        f << listSegment3D.at(i).D[0] << " " <<
        listSegment3D.at(i).D[1] << " " <<
        listSegment3D.at(i).D[2] << " " <<
        listSegment3D.at(i).D[3] << " " <<
        listSegment3D.at(i).D[4] << " " <<
        listSegment3D.at(i).D[5] << " " <<
        listSegment3D.at(i).D[6] <<
        std::endl;
        //thicknesses
        f << listSegment3D.at(i).thickness3D.at(0) << " " << listSegment3D.at(i).thickness3D.at(1) << " " << listSegment3D.at(i).thickness3D.at(2) ;
        f << std::endl;
        //separation segment
        f << "------------";
        f << std::endl;
    }
}

//function to convert  axis index to enum AXIS
AXIS getAxes(std::string a){
    if (a =="0"){
        return AXIS::XY;
    }
    if (a == "1"){
        return AXIS::XZ;
    }
    if(a== "2"){
        return AXIS::YZ;
    }
}

//Got  all segment3D from file
template<typename Point3D,typename AlphaThickSegmentComputer2D>
std::vector<Line3D>  createBDSfromFile(std::string fileName, std::vector<Point3D> *listAllPoint3D){
    std::vector<Line3D>  listSegment3D ;
    std::ifstream file(fileName);
    if (file.is_open()) {
        std::string line;
        std::getline(file,line);
        std::istringstream ss0(line.c_str());
        std::istringstream sss0;
        std::string token0;
        while(std::getline(ss0, token0, '|')) {
            sss0 =  std::istringstream( token0 ) ;
            std::vector<int> point ;
            while(std::getline(sss0, token0, ' ')) {
                point.push_back(std::stoi( token0));
            }
            listAllPoint3D->push_back(Point3D(point[0],point[1],point[2]));
        }
        std::getline(file,line);
        while (std::getline(file, line)) {
            //commentary
            //projection
            std::stringstream parser(line.c_str());
            std::string s1, s2;
            parser >> s1 >> s2;
            std::pair<AXIS,AXIS> ax = std::make_pair(getAxes(s1), getAxes(s2));
            std::cout<<ax<<std::endl;
            std::getline(file,line);
            //point3D
            std::string l = line.c_str();
            std::istringstream ss(l);
            std::istringstream sss;
            std::string token;
            std::vector<Point3D> listPoint ;
            while(std::getline(ss, token, '|')) {
                sss =  std::istringstream( token ) ;
                std::vector<int> point ;
                while(std::getline(sss, token, ' ')) {
                    point.push_back(std::stoi( token));
                }
                listPoint.push_back(Point3D(point[0],point[1],point[2]));
            }
            std::getline(file,line);
            std::stringstream parser2(line.c_str());
            parser2 >> s1 >> s2;
            std::pair<int,int> indexExtremity = std::make_pair(std::atoi(s1.c_str()), std::atoi(s2.c_str()));;
            std::getline(file,line);
            DGtal::PointVector<7,double> d;
            std::istringstream ss2(line.c_str());
            int i = 0;
            while(std::getline(ss2, token, ' ')) {
                d[i] = (std::stod(token));
                i++;
            }
            std::getline(file,line);
            std::istringstream ss3(line.c_str());
            std::vector<double> t;
            while(std::getline(ss3, token, ' ')) {
                t.push_back(std::stod(token));
            }

            Line3D line3D;
            init<Point3D,AlphaThickSegmentComputer2D>(&line3D,ax,d,t,listPoint,indexExtremity);
            listSegment3D.push_back(line3D);
            std::getline(file, line);
        }
        file.close();
    }
    return listSegment3D;
}

std::vector<std::vector<double>> getNoisy(std::string fileName){
    std::vector<std::vector<double>> VecTM(3);
    std::ifstream file(fileName);
    if(file.is_open()){
        std::string line;
        while(std::getline(file,line)){
            std::stringstream parser(line.c_str());
            int i1, i2, i3;
            parser >> i1 >> i2 >> i3 ;
            VecTM[0].push_back(i1);
            VecTM[1].push_back(i2);
            VecTM[2].push_back(i3);
        }
    }
    file.close();
    return VecTM;
}



//convert point3D to point2D depending on the axis
template<typename Point3D, typename Point2D>
Point2D convert3Dto2DAXIS(const Point3D &p3D,AXIS AXIS){
    Point2D nP; // (X,Y,Z)
    switch (AXIS){
        case AXIS::XY :
            nP =  Point2D(*(p3D.begin()),*(p3D.begin() + 1)); // (X,Y)
            break;
        case AXIS::XZ:
            nP = Point2D(*(p3D.begin()),*(p3D.begin() + 2)); // (X,Z)
            break;

        case AXIS::YZ:
            nP = Point2D(*(p3D.begin() + 1),*(p3D.begin() + 2)); // (Y,Z)
            break;
    }
    return nP;
}

//compute next Point
template<typename Point3D, typename Point2D>
Point2D nextPoint(const  std::vector<Point3D> & sequence,const Point2D &p, AXIS AXIS, int &indice){
    bool PointFind = false;
    Point2D nP;
    while(!PointFind && indice < sequence.size()){
        nP = convert3Dto2DAXIS<Point3D,Point2D>(sequence.at(indice + 1),AXIS);
        if(nP!=p){
            PointFind = true ;
        }else{
            indice++;
        }
    }
    return nP ;
}

// Function to return GCD of two numbers
long long gcd(long long int a, long long int b){
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

// Function to return LCM of two numbers
long long lcm(int a, int b){
    return std::abs((a / gcd(a, b)) * b);
}

//function distance 2 points2D
template <typename Point2D>
double distance(Point2D point2Da, Point2D point2Db){
    return sqrt(pow(point2Db[0]- point2Da[0],2) + pow(point2Db[1]- point2Da[1],2));
}


//function distance 2 point3D
template<typename Point3D>
double distance_3D(Point3D point3Da, Point3D point3Db){
    auto t = sqrt(pow(point3Db[0]- point3Da[0],2) + pow(point3Db[1]- point3Da[1],2) + pow(point3Db[2]- point3Da[2],2) );
    return t;
}

#endif //STAGEDGTAL_FUNCTION2_H

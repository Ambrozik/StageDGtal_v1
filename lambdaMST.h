//
// Created by ambrozik on 23/08/2021.
//
#include "function2.h"


//-------------function to compute lambdaMST-------------//

//---function to compute lambda for lambdaMST---//

//lambda Exponential

/// \brief lambda Exponential
/// \param x
/// \return lambda  EXponential
double lambdaExponentialFunction(double x){
    return 2.0 / ( std::exp ( 15.0 * ( x - 0.5 ) ) + std::exp ( -15.0 * ( x - 0.5 ) ) );
}

/// \brief lambda sin
/// \param x
/// \return lambda  sin
double lambdaSin(double x){
    auto t = sin(M_PI * x);
    return t;
}

/// \brief lambda 64
/// \param x
/// \return lambda  64
double lambda64(double x){
    return 64 * (pow(-x,6) + 3 * pow(x,5) - 3 * pow(x,4) + pow(x,3));
}

/// \brief lambda triangle
/// \param x
/// \return lambda  triangle
double lambdaTriangle(double x){
    if (x <= 0.5) return 2 * x ;
    if (x > 0.5 ) return  -2. * x + 2. ;
    return x;
}
//---function to compute lambda for lambdaMST---//


/// \brief compute eccentricity
/// \tparam Point3D point 3D (x,y,z)
/// \param point3D  fulcrum
/// \param m pencil list
/// \return eccentricity
template <typename Point3D>
double e(Point3D point3D, Line3D m){
    if( m.findPoint(point3D)){
        auto t =  distance_3D(point3D,m.Points3D.at(0))/m.len;
        return t;
    }else{
        return 0 ;
    }
}


/// \brief get all maximum blurred segment passing through the point x
/// \tparam Point3D point 3D (x,y,z)
/// \tparam Line3D segment 3D
/// \param x fulcrum
/// \param CT segment list
/// \return segment list
template<typename Point3D, typename Line3D>
std::vector<Line3D> pencil(Point3D x ,std::vector<Line3D> CT ){
    std::vector<Line3D> p ;
    for(int i = 0 ; i < CT.size(); i++){
        auto t  = std::find(CT.at(i).Points3D.begin(), CT.at(i).Points3D.end(),x);
        if(t != CT.at(i).Points3D.end()){
            p.push_back(CT.at(i));
        }
    }
    return p;
}

/// \brief compute the tangent direction  of a contour point k of the sequence
/// \tparam Point3D point 3D (x,y,z)
/// \param k
/// \param M
/// \param lambda function lambda
/// \return
template< typename Point3D>
DGtal::PointVector<3,double> estimator_of_tetha(Point3D k ,std::vector<Line3D> M,std::string lambda){
    DGtal::PointVector<3,double>  sumTop ;
    double sumBottom = 0 ;
    std::vector<Line3D> pen = pencil(k,M);
    for(int i = 0 ; i < pen.size();  i++){
        double l = lambdaExponentialFunction(e(k,pen[i]));
        DGtal::PointVector<3,double>  t_p = pen[i].V.getNormalized() ;
        t_p = t_p * l;
        sumTop = sumTop + t_p;
        if(lambda=="Exponential"){
            sumBottom = sumBottom+ lambdaExponentialFunction(e(k, pen[i]));
        }
        if(lambda=="Triangle"){
            sumBottom = sumBottom+ lambdaTriangle(e(k, pen[i]));
        }
        if(lambda=="64"){
            sumBottom = sumBottom+ lambda64(e(k, pen[i]));
        }else{ //default lambdaSin
            sumBottom = sumBottom+ lambdaSin(e(k, pen[i]));
        }
    }
    if(sumBottom == 0){
        return Point3D(0,0,0);
    }
    return sumTop/sumBottom;
}


/// \brief function 3D lambda-mst estimator
/// \tparam Point3D point 3D (x,y,z)
/// \param sequence curve 3D
/// \param tangentCoverSet tangent cover
/// \param lambda_type function lambda
/// \return lambda mst 3D
template <typename Point3D>
std::vector<DGtal::PointVector<3,double> > lambda_mst_estimator_3D(const  std::vector<Point3D> & sequence, std::vector<Line3D>  tangentCoverSet, std::string lambda_type){
    std::vector<DGtal::PointVector<3,double>> m_mst_estimator ;
    for(int ind = 0 ; ind < sequence.size(); ind++){
        auto p = sequence.at(ind);
        m_mst_estimator.push_back(estimator_of_tetha<Point3D>(p,tangentCoverSet,lambda_type));
    }
    return m_mst_estimator;
}

//-------------function to compute lambdaMST-------------//


#ifndef STAGEDGTAL_LAMBDAMST_H
#define STAGEDGTAL_LAMBDAMST_H

#endif //STAGEDGTAL_LAMBDAMST_H

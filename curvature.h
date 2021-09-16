//
// Created by ambrozik on 23/08/2021.
//
#include "function2.h"

//-------------function to compute curvature-----------------//

/// \brief curvature rayon  \f$s_1 =  \Vert \overrightarrow{C_kC_r}  \Vert  ,\ s_2 =  \Vert\overrightarrow{C_kC_l}  \Vert \ et \ s_3=  \Vert \overrightarrow{C_lC_r}  \Vert \f$

/// \param s1 s_1
/// \param s2 s_2
/// \param s3 s_3
/// \return curvature rayon
double Rwidth(const double  &s1, const double &s2, const double &s3 ){
    return (s1 * s2 * s3)/(sqrt((s1 + s2 + s3)*(s1 - s2 + s3) * (s1 + s2 - s3)*(s2+s3-s1)));
}


/// \brief compute curvature \f$ R_\upsilon(C_k) = \frac{s_1s_2s_3}{\sqrt{(s_1 + s_2 + s_3)(s_1 + s_2 - s_3)(s_1 + s_2 -s_3)(s_2 + s_3 - s_1) }} \f$
/// \tparam Point3D point 3D (x,y,z)
/// \param tangentCoverSet tangent cover
/// \param sequence curve 3D
/// \return curvature list by point
template< typename Point3D>
std::vector<double> curvatureOfWidth(std::vector<Line3D>  tangentCoverSet, std::vector<Point3D> sequence){
    int m = tangentCoverSet.size();
    std::vector<int> L(sequence.size()); //list index L
    std::vector<int> R(sequence.size());
    for(int i = 0 ; i < m - 1; i++) {
        auto E_i_1 = (i - 1 < 0) ? -1 : tangentCoverSet.at(i - 1).indexExtremity.second;
        auto E_i = tangentCoverSet.at(i).indexExtremity.second;
        for (int k = E_i_1 + 1 ; k <= E_i; k++) {
            L.at(k) = tangentCoverSet.at(i).indexExtremity.first;
        }

        auto bi = tangentCoverSet.at(i).indexExtremity.first;
        auto bi_1 = tangentCoverSet.at(i+1).indexExtremity.first;
        for (int k = bi ;
        k <= bi_1 - 1; k++) {
            R.at(k) = tangentCoverSet.at(i).indexExtremity.second;
        }
    }
    auto l = 1;
    auto n = sequence.size();
    std::vector<double> Rv(n);
    std::vector<double> Cv(n);

    for(int i = l ; i < n - 1 - l; i++){
        DGtal::PointVector<3,double> CkCr = sequence.at(R.at(i)) - sequence.at(i);
        DGtal::PointVector<3,double> CkCl = sequence.at(L.at(i)) - sequence.at(i);
        double s1 = distance_3D(sequence.at(i), sequence.at(R.at(i)));
        double s2 = distance_3D(sequence.at(i),sequence.at(L.at(i)));
        double s3 = distance_3D(sequence.at(L.at(i)), sequence.at(R.at(i)));
        Rv.at(i) = (Rwidth(s1,s2,s3));
        int s = (dotProduct(CkCr,CkCl) >= 0) ? 1 : -1 ;
        Cv.at(i) =(s / Rv.at(i));
    }
    return Cv;
}

//-------------function to compute curvature-----------------//
/*
template< typename Point3D>
std::vector<double> torsionEstimatator(std::vector<Line3D>  tangentCoverSet, std::vector<Point3D> sequence){
    double m = tangentCoverSet.size();
    std::vector<int> L(m);
    std::vector<int> R(m);
    for(int i = 0 ; i < m-1; i++) {
        auto E_i_1 = (i - 1 < 0) ? -1 : tangentCoverSet.at(i - 1).indexExtremity.second + 1;
        for (int k = E_i_1 + 1; k < tangentCoverSet.at(i).indexExtremity.second; k++) {
            L.at(k) = tangentCoverSet.at(i).indexExtremity.first;
        }
        for (int k = tangentCoverSet.at(i).indexExtremity.first;
             k < tangentCoverSet.at(i + 1).indexExtremity.first - 1; k++) {
            R.at(k) = tangentCoverSet.at(i).indexExtremity.second;
        }
    }
    auto l = 0;
    auto n = sequence.size();
    std::vector<DGtal::PointVector<3, double>> b(n);
    for (int i = l; i < n - 1 - l; i++) {
        DGtal::PointVector<3, double> t1, t2;
        t1 = (sequence.at(L.at(i)) - sequence.at(i)) / distance_3D(sequence.at(L.at(i)), sequence.at(i));
        t2 = (sequence.at(R.at(i)) - sequence.at(i)) / distance_3D(sequence.at(R.at(i)), sequence.at(i));
        b[i] = crossProduct(t1, t2);
    }
    std::vector<DGtal::PointVector<3,double>> P;
    P.push_back(DGtal::PointVector<3,double>(0,0,0));
    for(int i = l ; i < n - 1 - l; i++){
        P.push_back(P.back() + b[i]);
    }
        return std::vector<double> () ;
     //   return curvatureOfWidth(P,sequence);
}
*/

#ifndef STAGEDGTAL_CURVATURE_H
#define STAGEDGTAL_CURVATURE_H

#endif //STAGEDGTAL_CURVATURE_H

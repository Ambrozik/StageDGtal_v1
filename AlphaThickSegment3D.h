//
// Created by ambrozik on 16/04/2021.
//

#ifndef STAGEDGTAL_ALPHATHICKSEGMENT3D_H
#define STAGEDGTAL_ALPHATHICKSEGMENT3D_H
#include "DGtal/io/DrawWithDisplay3DModifier.h"
#include "DGtal/io/readers/PointListReader.h"
#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/boards/Board2D.h"


typedef DGtal::PointVector<3,double> Point;
typedef DGtal::PointVector<2,double> PointD;
typedef DGtal::PointVector<2,int> Point2D;
typedef DGtal::AlphaThickSegmentComputer<Point2D> AlphaThickSegmentComputer2D;

// struct of a AlphaThickSegment2D
/**
 * \struct AlphaSegment
 * \brief structure used to describe a BS 2D
 */
struct AlphaSegment {
    AlphaThickSegmentComputer2D alphaSeg;  /* <! AlphaThickSegmentComputer2D */
    DGtal::PointVector<4,double> D;  /* <! definition of a AlphaThinkSegment line 2D */
    double t ; /* <! maximum thicness of blurred segment 2D */
};


// struct of a AlphaThickSegment3D
/**
 * \struct AlphaThickSegment3D
 *  \brief structure to describe a blurred  segment 3d as mentioned in the internship report, with all the information necessary to be able to use the segment
 */
struct Line3D{
    typedef DGtal::PointVector<3,int>Point3D;
    std::pair<AXIS,AXIS> axes; /* <[in] projection plane of the blurred segment 3D  */
    std::pair<AlphaSegment,AlphaSegment> line3D; /* <! the 2 AlphaThickSegmentComputer2D used to create the blurred segment 3D */
    DGtal::PointVector<3,double> V;  /* <! director vector 3D */
    DGtal::PointVector<7,double> D;  /* <! definition of a AlphaThinkSegment line 3D D(a,b,c,mu,mu',e,e') */
    std::vector<Point3D> Points3D; /* <! Point 3D list present in the blurred segment */
    DGtal::PointVector<3,double> direction;  /* <! director vector 3D */
    std::vector<std::pair<double,double>>intercept;
    std::vector<std::pair<double,double>>thickness;
    std::pair<Point3D,Point3D> extremity ; /* <! the 2 ends points of the blurred segment 3D */
    std::pair<int,int> indexExtremity; /* <! the 2 ends point  index compared to curve */
    std::vector<double> thickness3D; /* <! thinckness of 3 plans to build the blurred segment */
    int indexA ; /* <! index of the plans used : 1 (XY XZ) 2 (XY YZ) 3 (XZ YZ) */
    double len; /* <! length of blurred segment 3D */
    /*!
    *  \brief find a point 3D in the curve
    *  \param x : point 3D to find
    *  \return true if the point belongs to curve 3D
    *  false else
    */
    bool findPoint(Point3D x){
        int i = 0 ;
        bool find = false;
        while( i < Points3D.size() && !find ){
            if(x== Points3D.at(i)) find = true;
            i++;
        }
        return find;
    }
};

/// \brief initialization of a blurred segment 3D
///  allows from the different information to create a blurred segment 3D
/// \tparam Point3D (x,y,z)
/// \tparam AlphaThickSegmentComputer2D (pair of 2 blurred segment 2D)
/// \param l  blurred segment 3D that will be initialized
/// \param axes pair of 2 plans used
/// \param D Definition of BS 3D
/// \param thickness3D   maximum thickness list
/// \param points3D  point3D list in blurred segment 3D
/// \param indexExtremity pair of 2 ends point index to compare to curve
template<typename Point3D,typename AlphaThickSegmentComputer2D>
void init(Line3D *l ,std::pair<AXIS,AXIS> axes,DGtal::PointVector<7,double> D,  std::vector<double> thickness3D,std::vector<Point3D> points3D,std::pair<Point3D,Point3D> indexExtremity);


#include "function2.h"

/// \brief function to compute 3D line defintion  D(a,b,c,mu,mu_p,nu, nu_p)
/// \param l segment 3D to compute
/// \param axis 2 plans used
void D3DComputer(Line3D &l, const int &axis);


/// \brief function to compute 2D line definition (a,b,mu,nu)
/// \tparam PointD Point2D (x,y)
/// \tparam AlphaSegment  structure of blurred segment 2D
/// \param anSegment  segment 2D to compute
template<typename PointD, typename AlphaSegment>
void computeSegmentDefinition (AlphaSegment &anSegment);


/// \brief check if a point is same as the last point add
/// \tparam Point2D Point2D (x,y)
/// \tparam AlphaThickSegmentComputer2D  pair of 2 blurred segment 2D
/// \param p point to check
/// \param alphaSegment segment to check
/// \return true if is the same point
/// false else
template<typename Point2D, typename AlphaThickSegmentComputer2D>
bool is_same_Point2D(const Point2D &p, AlphaThickSegmentComputer2D  alphaSegment);

/// \brief
/// \tparam Point2D
/// \tparam AlphaThickSegmentComputer2D
template<typename Point2D, typename AlphaThickSegmentComputer2D>
class ProductAlphaThickSegment3D {
public:
    virtual ~ProductAlphaThickSegment3D() {}
    virtual bool isProjectionValid(Point2D p, std::unordered_set<Point2D>,AlphaThickSegmentComputer2D t) const = 0;
};

template<typename Point2D, typename AlphaThickSegmentComputer2D>
class ConcreteProductAlphaThickSegmentBijectiveProjection : public ProductAlphaThickSegment3D<Point2D,AlphaThickSegmentComputer2D>{
public:
    bool isProjectionValid(Point2D p, std::unordered_set<Point2D> my_set,AlphaThickSegmentComputer2D t) const override {
        auto idx = my_set.find(p);
        return idx!=my_set.end() ;
    }

};
template<typename Point2D, typename AlphaThickSegmentComputer2D>
class ConcreteProductAlphaThickSegmentFurtherAway : public ProductAlphaThickSegment3D<Point2D,AlphaThickSegmentComputer2D> {
public:
    bool isProjectionValid(Point2D p, std::unordered_set<Point2D> my_set,AlphaThickSegmentComputer2D alphaThickSegmentComputer2D) const override {
        auto idx = my_set.find(p);
        return idx!=my_set.end() && !is_same_Point2D(p,alphaThickSegmentComputer2D);
    }
};

template<typename Point2D, typename AlphaThickSegmentComputer2D>
class CreatorAlphaThickSegment {

public:
    virtual ~CreatorAlphaThickSegment(){};
    virtual ProductAlphaThickSegment3D<Point2D,AlphaThickSegmentComputer2D>* FactoryMethod() const = 0;
    bool SomeOperation(Point2D p, std::unordered_set<Point2D> my_set,AlphaThickSegmentComputer2D alphaThickSegmentComputer2D) const {
        // Call the factory method to create a Product object.
        ProductAlphaThickSegment3D<Point2D,AlphaThickSegmentComputer2D>* product = this->FactoryMethod();
        // Now, use the product.
        bool res = product->isProjectionValid(p, my_set,alphaThickSegmentComputer2D);
        delete product;
        return res;
    }
    void setMaxThickness(const std::vector<double> &t){
        this->maxThicknesslist.push_back(t[0]);
        this->maxThicknesslist.push_back(t[1]);
        this->maxThicknesslist.push_back(t[2]);
    }

    double getThickNess(int dimension) const{
        return this->maxThicknesslist[dimension];
    }

    void choseMod(int mod){
        if (mod == 0){
            this->nbProjectionValid = 2;
        }else if(mod == 1){
            this->nbProjectionValid = 3;
        }
    }

    int getNbProjectionValid() const{
        return nbProjectionValid;
    }
private :
    std::vector<double>maxThicknesslist;
    int nbProjectionValid ;
};




/// Concrete Creators override the factory method in order to change the
/// \tparam Point2D
/// \tparam AlphaThickSegmentComputer2D
template<typename Point2D, typename AlphaThickSegmentComputer2D>
class AlphaThickSegmentBijectiveProjection : public CreatorAlphaThickSegment<Point2D,AlphaThickSegmentComputer2D> {
public:
    ProductAlphaThickSegment3D<Point2D,AlphaThickSegmentComputer2D>* FactoryMethod() const override {
        return new ConcreteProductAlphaThickSegmentBijectiveProjection<Point2D,AlphaThickSegmentComputer2D>();
    }
};

template<typename Point2D, typename AlphaThickSegmentComputer2D>
class AlphaThickSegmentFurtherAway : public CreatorAlphaThickSegment <Point2D,AlphaThickSegmentComputer2D>{
public:
    ProductAlphaThickSegment3D<Point2D,AlphaThickSegmentComputer2D>* FactoryMethod() const override {
        return new ConcreteProductAlphaThickSegmentFurtherAway<Point2D,AlphaThickSegmentComputer2D>();
    }
};

/// \brief retrieve the index of plans from the 2 plans
/// \param a pair of plan
/// \return index of 2 plans
int getA(std::pair<AXIS,AXIS> a);



/// \bref compute a blurred segment maximum
/// \tparam PointD point double
/// \tparam Point2D point 2D (x,y)
/// \tparam Point3D point 3D (x,y,z)
/// \tparam AlphaThickSegmentComputer2D pair of 2 blurred segment 2D
/// \tparam List3DLine blurred segment 3D list
/// \param creatorAlphaThickSegment creator factory type of  blurred segment 3D
/// \param sequence curve 3D
/// \param start index of first point to create the line
/// \param Oxyz combination of plan (OxyOxz) (OxyOyz) and (OxzOyz)
/// \return blurred segment
template<typename PointD, typename Point2D, typename  Point3D, typename AlphaThickSegmentComputer2D, typename List3DLine>
Line3D computerALine(const CreatorAlphaThickSegment<Point2D,AlphaThickSegmentComputer2D>& creatorAlphaThickSegment, std::vector<Point3D> sequence,int start,  std::vector<List3DLine> &Oxyz );


/// \brief compute all blurred segment maximum  of a curve
/// \tparam PointD point double
/// \tparam Point2D point 2D (x,y)
/// \tparam Point3D point 3D (x,y,z)
/// \tparam AlphaThickSegmentComputer2D pair of 2 blurred segment 2D
/// \tparam List3DLine blurred segment 3D list
/// \param creatorAlphaThickSegment creator factory type of  blurred segment 3D
/// \param sequence curve 3D
/// \param start index of first point to create the line
/// \param Oxyz combination of plan (OxyOxz) (OxyOyz) and (OxzOyz)
/// \return blurred segment list
template<typename PointD,typename Point2D , typename  Point3D , typename AlphaThickSegmentComputer2D , typename List3DLine>
List3DLine computerLine3D(const CreatorAlphaThickSegment<Point2D,AlphaThickSegmentComputer2D>& creatorAlphaThickSegment,std::vector<Point3D> sequence, std::vector<List3DLine> *Oxyz);


/// compute tangent cover of a cruve
/// \tparam PointD point double
/// \tparam Point2D point 2D (x,y)
/// \tparam Point3D point 3D (x,y,z)
/// \tparam AlphaThickSegmentComputer2D pair of 2 blurred segment 2D
/// \tparam List3DLine blurred segment 3D list
/// \param creatorAlphaThickSegment creator factory type of  blurred segment 3D
/// \param sequence curve 3D
/// \param start index of first point to create the line
/// \param Oxyz combination of plan (OxyOxz) (OxyOyz) and (OxzOyz)
/// \return blurred segment list belonging to the tangent

template<typename PointD,typename Point2D , typename  Point3D , typename AlphaThickSegmentComputer2D , typename List3DLine>
List3DLine tgtCover(const CreatorAlphaThickSegment<Point2D,AlphaThickSegmentComputer2D>& creatorAlphaThickSegment,std::vector<Point3D> sequence,std::vector<List3DLine> &Oxyz );

/// compute tangent cover of a cruve
/// \tparam PointD point double
/// \tparam Point2D point 2D (x,y)
/// \tparam Point3D point 3D (x,y,z)
/// \tparam AlphaThickSegmentComputer2D pair of 2 blurred segment 2D
/// \tparam List3DLine blurred segment 3D list
/// \param creatorAlphaThickSegment creator factory type of  blurred segment 3D
/// \param sequence curve 3D
/// \param start index of first point to create the line
/// \return blurred segment list belonging to the tangent
template<typename PointD,typename Point2D , typename  Point3D , typename AlphaThickSegmentComputer2D , typename List3DLine>
List3DLine tgtCoverLine(const CreatorAlphaThickSegment<Point2D,AlphaThickSegmentComputer2D>& creatorAlphaThickSegment,std::vector<Point3D> sequence);

///compute a  greedy segmentation 3D
/// \tparam PointD point double
/// \tparam Point2D point 2D (x,y)
/// \tparam Point3D point 3D (x,y,z)
/// \tparam AlphaThickSegmentComputer2D pair of 2 blurred segment 2D
/// \tparam List3DLine blurred segment 3D list
/// \param creatorAlphaThickSegment creator factory type of  blurred segment 3D
/// \param sequence curve 3D
/// \param start index of first point to create the line
/// \param Oxyz combination of plan (OxyOxz) (OxyOyz) and (OxzOyz)
/// \return blurred segment list belonging to the greedysegmentation
template<typename PointD,typename Point2D , typename  Point3D , typename AlphaThickSegmentComputer2D , typename List3DLine>
List3DLine greedyAlphaThickDecomposition(const CreatorAlphaThickSegment<Point2D,AlphaThickSegmentComputer2D>& creatorAlphaThickSegment,std::vector<Point3D> sequence, std::vector<Point3D> &Point3DExtremity,std::vector<List3DLine> &Oxyz);


#include "AlphaThickSegment3D.ih"
#endif //STAGEDGTAL_ALPHATHICKSEGMENT3D_H

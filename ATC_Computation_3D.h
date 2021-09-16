//

// Created by ambrozik on 02/06/2021.
//

/*********************************************************/
/********* Adaptive Tangent Cover computation ************/
/*********************************************************/





/// \brief function which calculates the 3D ATC with the first method mentioned,
///  that is to say, for a given point,
///  we take the maximum noise level of the 3 plans.
/// (it will not be retained for the future)
/// \tparam Point3D point 3D (x,y,z)
/// \param aContour curve 3D
/// \param vecMT3D list of noise by plan
/// \param ThicknessType list of different thickness
/// \param mode 0 for maximum 1 for minimum
/// \return adaptive tangential cover
template<typename Point3D>
std::vector<Line3D> adaptiveTangentCoverDecomposition3D(const vector<Point3D>& aContour, const std::vector<std::vector<double>>& vecMT3D, std::map<int,double> &ThicknessType, int mode);

/// \brief function which calculates the 3D ATC with the second method mentioned,
///  that is to say, for a given point,
///  we take the  3 noise levels.
/// (it will not be retained for the future)
/// \tparam Point3D point 3D (x,y,z)
/// \param aContour curve 3D
/// \param vecMT3D list of noise by plan
/// \param ThicknessType list of different thickness
/// \return adaptive tangential cover
template<typename Point3D>
std::vector<Line3D> adaptiveTangentCoverDecomposition3D_v2(const vector<Point3D>& aContour, const std::vector<std::vector<double>>& vecMT3D, std::map<std::vector<double>,int> &ThicknessType);


template<typename Point3D>
std::vector<Line3D>
adaptiveTangentCoverDecomposition3D(const vector<Point3D> &aContour, const vector<std::vector<double>> &vecMT3D,
                                    map<int, double> &ThicknessType, int mode) {

    typedef PointVector<2,double> PointD;
    typedef PointVector<2,int> Point2D;
    typedef AlphaThickSegmentComputer<Point2D> AlphaThickSegmentComputer2D;
    typedef std::vector<DGtal::Z3i::Point>::iterator Iterator;
    typedef std::vector<Line3D> List3DLine;
    //0. Find the max MT by point
    std::vector<double> vecMT;

    for(int i = 0 ; i < vecMT3D.at(0).size(); i++){

        if (mode == 0){
            double max = std::max(std::max(vecMT3D.at(0).at(i), vecMT3D.at(1).at(i)), vecMT3D.at(2).at(i));
            vecMT.push_back(max);
        }
        if(mode == 1 ){
            double min = std::min(std::min(vecMT3D.at(0).at(i), vecMT3D.at(1).at(i)), vecMT3D.at(2).at(i));
            vecMT.push_back(min);
        }

    }
    //1. Find vector of thickness elements
    vector<double> meaningThicknessElement;
    meaningThicknessElement.push_back(vecMT.front());
    for(vector<double>::const_iterator it = vecMT.begin()+1; it != vecMT.end(); it++)
    {
        double m = (*it);
        if (std::find(meaningThicknessElement.begin(), meaningThicknessElement.end(),m)==meaningThicknessElement.end())
            meaningThicknessElement.push_back(m);
    }
    std::sort(meaningThicknessElement.begin(),meaningThicknessElement.end(),sort_increase);
    for(int i = 0 ; i < meaningThicknessElement.size(); i++){
        std::cout<<meaningThicknessElement.at(i)<<std::endl;
    }

    //2. Compute different thickness tangent covers (blurred segments)
    vector<vector<Line3D>> meaningThicknessTangentCover(meaningThicknessElement.size());
    int index = 0;
    for(vector<double>::const_iterator it = meaningThicknessElement.begin(); it != meaningThicknessElement.end(); it++)
    {
        double thickness = (*it)*sqrt(2);
        ThicknessType[index] = thickness;
        cout<<"thickness="<<thickness<<endl;
        CreatorAlphaThickSegment<Point2D, AlphaThickSegmentComputer2D>*creator;
        creator = new AlphaThickSegmentFurtherAway<Point2D, AlphaThickSegmentComputer2D>();
        creator->setMaxThickness({thickness,thickness,thickness});
        creator->choseMod(1);
        vector<Line3D> fuzzySegmentSet = tgtCoverLine<PointD,Point2D,Point3D,AlphaThickSegmentComputer2D,List3DLine>(*creator ,aContour);
        cout<<"===> Num of seg decomposed is "<<fuzzySegmentSet.size()<<endl;
        for (std::vector<Line3D>::const_iterator it_bis = fuzzySegmentSet.begin();it_bis != fuzzySegmentSet.end();it_bis++)
            meaningThicknessTangentCover[index].push_back(*it_bis);
        index++;
    }

    //3. Update thickness of points with tangent covers
    vector<double> vecMTmodified;
    for(vector<double>::const_iterator it = vecMT.begin(); it != vecMT.end(); it++)
        vecMTmodified.push_back(*it);
    for(int it=meaningThicknessTangentCover.size()-1; it>=0; it--)
    {
        vector<Line3D> fuzzySegmentSet = meaningThicknessTangentCover.at(it);//*it;
        double thickness = meaningThicknessElement.at(it);
        for (std::vector<Line3D>::const_iterator it_bis = fuzzySegmentSet.begin();it_bis != fuzzySegmentSet.end();it_bis++)
        {
            int idStart = (*it_bis).indexExtremity.first;
            int idEnd = (*it_bis).indexExtremity.second;
//            int idStart = findElement(aContour,getStartPoint(*it_bis));
//            int idEnd = findElement(aContour,getEndPoint(*it_bis),idStart);
            if(idStart != -1 && idEnd != -1)
            {
                double maxThickness = -1;
                for(int i=idStart; i<=idEnd; i++)
                {
                    double thicknessPoint = vecMT.at(i);
                    if(thicknessPoint > maxThickness)
                        maxThickness = thicknessPoint;
                }
                for(int i=idStart; i<=idEnd; i++)
                {
                    if(maxThickness==thickness)//vecMTmodified.at(i) < maxThickness &&
                        vecMTmodified.at(i) = maxThickness;
                }
            }
            else
                cout<<"negatif"<<endl;
        }
    }

    //4. Travel over the tangent covers and select the segments w.r.t the associated thickness of points
    vector<vector<Line3D> > adaptiveMeaningThicknessTangentCover;
    int idCover = 0;
    for(vector<vector<Line3D> >::const_iterator it = meaningThicknessTangentCover.begin(); it != meaningThicknessTangentCover.end(); it++)
    {
        adaptiveMeaningThicknessTangentCover.push_back(vector<Line3D>());
        vector<Line3D> fuzzySegmentSet = *it;
        vector<Line3D> AdaptiveFuzzySegmentSet;
        int idSeg=0;
        double thickness = meaningThicknessElement.at(idCover);
        for (vector<Line3D>::const_iterator it_bis = fuzzySegmentSet.begin();it_bis != fuzzySegmentSet.end();it_bis++)
        {
            int idStart = (*it_bis).indexExtremity.first;
            int idEnd = (*it_bis).indexExtremity.second;
//            int idStart = findElement(aContour,getStartPoint(*it_bis));
//            int idEnd = findElement(aContour,getEndPoint(*it_bis));
            if(idStart != -1 && idEnd != -1)
            {
                bool isGoodMTmodif = false, isGoodMT = false;//true
                for(int i=idStart; i<=idEnd; i++)
                {
                    double thicknessMT = vecMT.at(i); //all elt have same meaningful thickness value (dont contain other meaningful thickness)
                    if(thicknessMT == thickness)
                        isGoodMT = true;
                    double thicknessMTmodif = vecMTmodified.at(i);
                    if(thicknessMTmodif == thickness) //there exist at least one elt in modif having meaningful thickness value
                        isGoodMTmodif = true;
                }
                if(isGoodMTmodif && isGoodMT)
                    AdaptiveFuzzySegmentSet.push_back(*it_bis);
            }
            idSeg++;
        }
        for (vector<Line3D>::const_iterator it_bis = AdaptiveFuzzySegmentSet.begin();it_bis != AdaptiveFuzzySegmentSet.end();it_bis++)
            adaptiveMeaningThicknessTangentCover[idCover].push_back(*it_bis);
        idCover++;
    }
    for(vector<vector<Line3D> >::reverse_iterator it1 = adaptiveMeaningThicknessTangentCover.rbegin(); it1 != adaptiveMeaningThicknessTangentCover.rend(); ++it1)
    {
        vector<Line3D>& segmentSet1 = *it1;
        for(vector<vector<Line3D> >::reverse_iterator it2 = it1+1; it2 != adaptiveMeaningThicknessTangentCover.rend(); ++it2)
        {
            vector<Line3D>& segmentSet2 = *it2;
            for (vector<Line3D>::iterator itSeg1 = segmentSet1.begin();itSeg1 != segmentSet1.end();itSeg1++)
            {
                int idCurrentStart = (*itSeg1).indexExtremity.first;
                int idCurrentEnd = (*itSeg1).indexExtremity.second;

//                int idCurrentStart = findElement(aContour,getStartPoint(*itSeg1));
//                int idCurrentEnd = findElement(aContour,getEndPoint(*itSeg1),idCurrentStart);
                for (vector<Line3D>::iterator itSeg2 = segmentSet2.begin();itSeg2 != segmentSet2.end();itSeg2++)
                {
                    int idStart = (*itSeg2).indexExtremity.first;
                    int idEnd = (*itSeg2).indexExtremity.second;

//                    int idStart = findElement(aContour,getStartPoint(*itSeg2));
//                    int idEnd = findElement(aContour,getEndPoint(*itSeg2),idStart);
                    if(idCurrentStart<=idStart && idCurrentEnd>=idEnd)
                    {
                        segmentSet2.erase(itSeg2);
                        itSeg2--;
                    }
                }
            }
        }
    }

    //5. Reorder the multi-thickness tangent cover
    vector<Line3D> adaptiveTangentCover;
    int seg=0,nbSeg=0;
    vector<int> idThicknessCover; //stock idSeg of the last seg at idThicknessCover
    for(int it=0; it<meaningThicknessElement.size();it++)
        idThicknessCover.push_back(0);
    for(int it = 0; it < adaptiveMeaningThicknessTangentCover.size(); it++)
        nbSeg += (adaptiveMeaningThicknessTangentCover.at(it)).size();

    while (seg<nbSeg)
    {
        int idMinStart = aContour.size(), idMinEnd = aContour.size();
        int idMin=-1, idSeg=-1;
        //scan adaptiveMeaningThicknessTangentCover
        for(int it = 0; it < adaptiveMeaningThicknessTangentCover.size(); it++)//thickness level = it
        {
            //current seg of thickness level idThicknessCover.at(i)
            int idCurrentSeg = idThicknessCover.at(it);
            if(idCurrentSeg<(adaptiveMeaningThicknessTangentCover.at(it)).size())
            {
                //get idStart and idEnd of seg
                int idCurrentStart = adaptiveMeaningThicknessTangentCover.at(it).at(idCurrentSeg).indexExtremity.first;
                int idCurrentEnd =  adaptiveMeaningThicknessTangentCover.at(it).at(idCurrentSeg).indexExtremity.second;
//                int idCurrentStart = findElement(aContour,getStartPoint((adaptiveMeaningThicknessTangentCover.at(it)).at(idCurrentSeg)));
//                int idCurrentEnd = findElement(aContour,getEndPoint((adaptiveMeaningThicknessTangentCover.at(it)).at(idCurrentSeg)),idCurrentStart);
                if(idThicknessCover.at(it)<(adaptiveMeaningThicknessTangentCover.at(it)).size())
                {
                    //find min idCurrentStart
                    if(idMinStart==idCurrentStart && idMinEnd<idCurrentEnd)
                    {
                        if(idThicknessCover.at(it)<(adaptiveMeaningThicknessTangentCover.at(it)).size()-1)
                        {
                            idThicknessCover.at(idMin) = idThicknessCover.at(idMin) + 1;
                            seg++;
                        }
                        idSeg = idCurrentSeg;
                        idMin = it;
                        idMinStart = idCurrentStart;
                        idMinEnd = idCurrentEnd;
                    }
                    else if(idMinStart>idCurrentStart && idMinEnd>=idCurrentEnd)
                    {
                        idSeg = idCurrentSeg;
                        idMin = it;
                        idMinStart = idCurrentStart;
                        idMinEnd = idCurrentEnd;
                    }
                }
            }
        }
        adaptiveTangentCover.push_back((adaptiveMeaningThicknessTangentCover.at(idMin)).at(idSeg));
        idThicknessCover.at(idMin) = idThicknessCover.at(idMin) + 1;
        seg++;
    }
    return adaptiveTangentCover;
}

template<typename Point3D>
std::vector<Line3D> adaptiveTangentCoverDecomposition3D_v2(const vector<Point3D>& aContour, const std::vector<std::vector<double>>& vecMT3D, std::map<std::vector<double>,int> &ThicknessType)
{

    typedef PointVector<2,double> PointD;
    typedef PointVector<2,int> Point2D;
    typedef AlphaThickSegmentComputer<Point2D> AlphaThickSegmentComputer2D;
    typedef std::vector<DGtal::Z3i::Point>::iterator Iterator;
    typedef std::vector<Line3D> List3DLine;
    //0. Find the max MT by point
    std::vector<std::vector<double>> vecMT;

    for(int i = 0 ; i < vecMT3D.at(0).size(); i++){
        std::vector<double> t;
        t.push_back(vecMT3D.at(0).at(i));
        t.push_back(vecMT3D.at(1).at(i));
        t.push_back(vecMT3D.at(2).at(i));
        vecMT.push_back(t);
    }
    //1. Find vector of thickness elements
    std::vector<std::vector<double>> meaningThicknessElement;
    meaningThicknessElement.push_back(vecMT.front());
    for(std::vector<std::vector<double>>::const_iterator it = vecMT.begin()+1; it != vecMT.end(); it++)
    {
        auto m = (*it);
        if (std::find(meaningThicknessElement.begin(), meaningThicknessElement.end(),m)==meaningThicknessElement.end())
            meaningThicknessElement.push_back(m);
    }
    //std::sort(meaningThicknessElement.begin(),meaningThicknessElement.end(),sort_increase);

        //2. Compute different thickness tangent covers (blurred segments)
    vector<vector<Line3D>> meaningThicknessTangentCover(meaningThicknessElement.size());
    int index = 0;
    for(vector<vector<double>>::const_iterator it = meaningThicknessElement.begin(); it != meaningThicknessElement.end(); it++)
    {
        std::cout<<(*it).at(0) << " " << (*it).at(1) << " " << (*it).at(2)<<std::endl;
        std::vector<double> thickness;
        thickness.push_back((*it).at(0)*sqrt(2));
        thickness.push_back((*it).at(1)*sqrt(2));
        thickness.push_back((*it).at(2)*sqrt(2));
        ThicknessType[thickness] = index;
        cout<<"thickness="<<thickness.at(0) << " " << thickness.at(1)<< " " << thickness.at(2) <<endl;
        CreatorAlphaThickSegment<Point2D, AlphaThickSegmentComputer2D>*creator;
        creator = new AlphaThickSegmentFurtherAway<Point2D, AlphaThickSegmentComputer2D>();
        creator->setMaxThickness(thickness);
        creator->choseMod(0);
        vector<Line3D> fuzzySegmentSet = tgtCoverLine<PointD,Point2D,Point3D,AlphaThickSegmentComputer2D,List3DLine>(*creator ,aContour);
        cout<<"===> Num of seg decomposed is "<<fuzzySegmentSet.size()<<endl;
        for (std::vector<Line3D>::const_iterator it_bis = fuzzySegmentSet.begin();it_bis != fuzzySegmentSet.end();it_bis++)
            meaningThicknessTangentCover[index].push_back(*it_bis);
        index++;
    }

    //3. Update thickness of points with tangent covers
    vector<vector<double>> vecMTmodified;
    for(vector<vector<double>>::const_iterator it = vecMT.begin(); it != vecMT.end(); it++)
        vecMTmodified.push_back(*it);
    for(int it=meaningThicknessTangentCover.size()-1; it>=0; it--)
    {
        vector<Line3D> fuzzySegmentSet = meaningThicknessTangentCover.at(it);//*it;
        vector<double> thickness = meaningThicknessElement.at(it);
        for (std::vector<Line3D>::const_iterator it_bis = fuzzySegmentSet.begin();it_bis != fuzzySegmentSet.end();it_bis++)
        {
            int idStart = (*it_bis).indexExtremity.first;
            int idEnd = (*it_bis).indexExtremity.second;
//            int idStart = findElement(aContour,getStartPoint(*it_bis));
//            int idEnd = findElement(aContour,getEndPoint(*it_bis),idStart);
            if(idStart != -1 && idEnd != -1)
            {
                std::vector<double>  maxThickness = {-1,-1,-1};
                for(int i=idStart; i<=idEnd; i++)
                {
                    vector<double> thicknessPoint = vecMT.at(i);
                    if(thicknessPoint.at(0) > maxThickness.at(0))
                        maxThickness.at(0) = thicknessPoint.at(0);
                    if(thicknessPoint.at(1) > maxThickness.at(1))
                        maxThickness.at(1) = thicknessPoint.at(1);
                    if(thicknessPoint.at(2) > maxThickness.at(2))
                        maxThickness.at(2) = thicknessPoint.at(2);
//                    if(thicknessPoint.at(0) >= maxThickness.at(0) && thicknessPoint.at(1) >= maxThickness.at(1) && thicknessPoint.at(2) >= maxThickness.at(2))
//                        maxThickness = thicknessPoint;
                }
                for(int i=idStart; i<=idEnd; i++)
                {
                    if(maxThickness==thickness)//vecMTmodified.at(i) < maxThickness &&
                        vecMTmodified.at(i) = maxThickness;
                }
            }
            else
                cout<<"negatif"<<endl;
        }
    }

    //4. Travel over the tangent covers and select the segments w.r.t the associated thickness of points
    vector<vector<Line3D> > adaptiveMeaningThicknessTangentCover;
    int idCover = 0;
    for(vector<vector<Line3D> >::const_iterator it = meaningThicknessTangentCover.begin(); it != meaningThicknessTangentCover.end(); it++)
    {
        adaptiveMeaningThicknessTangentCover.push_back(vector<Line3D>());
        vector<Line3D> fuzzySegmentSet = *it;
        vector<Line3D> AdaptiveFuzzySegmentSet;
        int idSeg=0;
        vector<double> thickness = meaningThicknessElement.at(idCover);
        for (vector<Line3D>::const_iterator it_bis = fuzzySegmentSet.begin();it_bis != fuzzySegmentSet.end();it_bis++)
        {
            int idStart = (*it_bis).indexExtremity.first;
            int idEnd = (*it_bis).indexExtremity.second;
//            int idStart = findElement(aContour,getStartPoint(*it_bis));
//            int idEnd = findElement(aContour,getEndPoint(*it_bis));
            if(idStart != -1 && idEnd != -1)
            {
                bool isGoodMTmodif = false, isGoodMT = false;//true
                for(int i=idStart; i<=idEnd; i++)
                {
                    vector<double> thicknessMT = vecMT.at(i); //all elt have same meaningful thickness value (dont contain other meaningful thickness)
                    if(thicknessMT == thickness)
                        isGoodMT = true;
                    vector<double> thicknessMTmodif = vecMTmodified.at(i);
                    if(thicknessMTmodif == thickness) //there exist at least one elt in modif having meaningful thickness value
                        isGoodMTmodif = true;
                }
                if(isGoodMTmodif && isGoodMT)
                    AdaptiveFuzzySegmentSet.push_back(*it_bis);
            }
            idSeg++;
        }
        for (vector<Line3D>::const_iterator it_bis = AdaptiveFuzzySegmentSet.begin();it_bis != AdaptiveFuzzySegmentSet.end();it_bis++)
            adaptiveMeaningThicknessTangentCover[idCover].push_back(*it_bis);
        idCover++;
    }
    for(vector<vector<Line3D> >::reverse_iterator it1 = adaptiveMeaningThicknessTangentCover.rbegin(); it1 != adaptiveMeaningThicknessTangentCover.rend(); ++it1)
    {
        vector<Line3D>& segmentSet1 = *it1;
        for(vector<vector<Line3D> >::reverse_iterator it2 = it1+1; it2 != adaptiveMeaningThicknessTangentCover.rend(); ++it2)
        {
            vector<Line3D>& segmentSet2 = *it2;
            for (vector<Line3D>::iterator itSeg1 = segmentSet1.begin();itSeg1 != segmentSet1.end();itSeg1++)
            {
                int idCurrentStart = (*itSeg1).indexExtremity.first;
                int idCurrentEnd = (*itSeg1).indexExtremity.second;

//                int idCurrentStart = findElement(aContour,getStartPoint(*itSeg1));
//                int idCurrentEnd = findElement(aContour,getEndPoint(*itSeg1),idCurrentStart);
                for (vector<Line3D>::iterator itSeg2 = segmentSet2.begin();itSeg2 != segmentSet2.end();itSeg2++)
                {
                    int idStart = (*itSeg2).indexExtremity.first;
                    int idEnd = (*itSeg2).indexExtremity.second;

//                    int idStart = findElement(aContour,getStartPoint(*itSeg2));
//                    int idEnd = findElement(aContour,getEndPoint(*itSeg2),idStart);
                    if(idCurrentStart<=idStart && idCurrentEnd>=idEnd)
                    {
                        segmentSet2.erase(itSeg2);
                        itSeg2--;
                    }
                }
            }
        }
    }

    //5. Reorder the multi-thickness tangent cover
    vector<Line3D> adaptiveTangentCover;
    int seg=0,nbSeg=0;
    vector<int> idThicknessCover; //stock idSeg of the last seg at idThicknessCover
    for(int it=0; it<meaningThicknessElement.size();it++)
        idThicknessCover.push_back(0);
    for(int it = 0; it < adaptiveMeaningThicknessTangentCover.size(); it++)
        nbSeg += (adaptiveMeaningThicknessTangentCover.at(it)).size();

    while (seg<nbSeg)
    {
        int idMinStart = aContour.size(), idMinEnd = aContour.size();
        int idMin=-1, idSeg=-1;
        //scan adaptiveMeaningThicknessTangentCover
        for(int it = 0; it < adaptiveMeaningThicknessTangentCover.size(); it++)//thickness level = it
        {
            //current seg of thickness level idThicknessCover.at(i)
            int idCurrentSeg = idThicknessCover.at(it);
            if(idCurrentSeg<(adaptiveMeaningThicknessTangentCover.at(it)).size())
            {
                //get idStart and idEnd of seg
                int idCurrentStart = adaptiveMeaningThicknessTangentCover.at(it).at(idCurrentSeg).indexExtremity.first;
                int idCurrentEnd =  adaptiveMeaningThicknessTangentCover.at(it).at(idCurrentSeg).indexExtremity.second;
//                int idCurrentStart = findElement(aContour,getStartPoint((adaptiveMeaningThicknessTangentCover.at(it)).at(idCurrentSeg)));
//                int idCurrentEnd = findElement(aContour,getEndPoint((adaptiveMeaningThicknessTangentCover.at(it)).at(idCurrentSeg)),idCurrentStart);
                if(idThicknessCover.at(it)<(adaptiveMeaningThicknessTangentCover.at(it)).size())
                {
                    //find min idCurrentStart
                    if(idMinStart==idCurrentStart && idMinEnd<idCurrentEnd)
                    {
                        if(idThicknessCover.at(it)<(adaptiveMeaningThicknessTangentCover.at(it)).size()-1)
                        {
                            idThicknessCover.at(idMin) = idThicknessCover.at(idMin) + 1;
                            seg++;
                        }
                        idSeg = idCurrentSeg;
                        idMin = it;
                        idMinStart = idCurrentStart;
                        idMinEnd = idCurrentEnd;
                    }
                    else if(idMinStart>idCurrentStart && idMinEnd>=idCurrentEnd)
                    {
                        idSeg = idCurrentSeg;
                        idMin = it;
                        idMinStart = idCurrentStart;
                        idMinEnd = idCurrentEnd;
                    }
                }
            }
        }
        adaptiveTangentCover.push_back((adaptiveMeaningThicknessTangentCover.at(idMin)).at(idSeg));
        idThicknessCover.at(idMin) = idThicknessCover.at(idMin) + 1;
        seg++;
    }
    return adaptiveTangentCover;
}


#include "ATC_Computation.h"

void drawCanvas(Board2D &aBoard, unsigned int w, unsigned int h){
    aBoard.setLineWidth(0);
    aBoard.drawLine(0, 0, w, 0);
    aBoard.drawLine(w, 0, w, h);
    aBoard.drawLine(w, h, 0, h);
    aBoard.drawLine(0, h, 0, 0);
}

/*************************************/
/*** Burred segments decomposition ***/
/*************************************/
vector<AlphaThickSegmentComputer2DD> blurredSegmentDecomposition(const vector<RealPoint>& aContour, double thickness, const char* filename)
{
    vector<AlphaThickSegmentComputer2DD> fuzzySegmentSet;
    //run over the points on the contours
    for(vector<RealPoint>::const_iterator it=aContour.begin();it!=aContour.end();it++)
    {
        AlphaThickSegmentComputer2DD aSegment(thickness);
        aSegment.init(it);
        /* travel over the contour points and add to the seg */
        while (aSegment.end()!=aContour.end() && aSegment.extendFront()){;}
        if(it==aContour.begin())
            fuzzySegmentSet.push_back(aSegment);
        else if(findElement(aContour,getEndPoint(aSegment))>findElement(aContour,getEndPoint(fuzzySegmentSet.back())))
            fuzzySegmentSet.push_back(aSegment);
        if(getEndPoint(aSegment)==aContour.back() || it==aContour.end())
            break;
    }

    if(filename!=NULL)
    {
        std::string n (filename);
        std::string outputExt = n.substr(n.find_last_of(".")+1);

        Board2D aBoard;
        /* display the boundary */
        aBoard << SetMode("PointVector", "Both");
        for(vector<RealPoint>::const_iterator it=aContour.begin(); it!=aContour.end(); it++)
            aBoard << *it;
        /* display the boundary */
        /* Display boundingbox */
        for(vector<AlphaThickSegmentComputer2DD>::const_iterator it=fuzzySegmentSet.begin();it!=fuzzySegmentSet.end();it++)
            aBoard << SetMode((*it).className(), "BoundingBox")
                   <<(*it);
        /* Display boundingbox */
        if(outputExt=="svg"){
            aBoard.saveSVG(filename);
        }
        else if (outputExt == "eps"){
            aBoard.saveEPS(filename);
        }
    }
    return fuzzySegmentSet;
}
/*************************************/
/*** Burred segments decomposition ***/
/*************************************/

/*********************************************************/
/********* Adaptive Tangent Cover computation ************/
/*********************************************************/
vector<AlphaThickSegmentComputer2DD> adaptiveTangentCoverDecomposition(const vector<RealPoint>& aContour, const vector<double>& vecMT)
{
    //1. Find vector of thickness element
    vector<double> meaningThicknessElement;
    meaningThicknessElement.push_back(vecMT.front());
    for(vector<double>::const_iterator it = vecMT.begin()+1; it != vecMT.end(); it++)
    {
        double m = (*it);
        if (std::find(meaningThicknessElement.begin(), meaningThicknessElement.end(),m)==meaningThicknessElement.end())
            meaningThicknessElement.push_back(m);
    }
    std::sort(meaningThicknessElement.begin(),meaningThicknessElement.end(),sort_increase);

    //2. Compute different thickness tangent covers (blurred segments)
    vector<vector<AlphaThickSegmentComputer2DD> > meaningThicknessTangentCover(meaningThicknessElement.size());
    int index = 0;
    for(vector<double>::const_iterator it = meaningThicknessElement.begin(); it != meaningThicknessElement.end(); it++)
    {
        double thickness = (*it)*sqrt(2);
        cout<<"thickness="<<thickness<<endl;
        vector<AlphaThickSegmentComputer2DD> fuzzySegmentSet = blurredSegmentDecomposition(aContour,thickness,NULL);
        cout<<"===> Num of seg decomposed is "<<fuzzySegmentSet.size()<<endl;
        for (vector<AlphaThickSegmentComputer2DD>::const_iterator it_bis = fuzzySegmentSet.begin();it_bis != fuzzySegmentSet.end();it_bis++)
            meaningThicknessTangentCover[index].push_back(*it_bis);
        index++;
    }

    //3. Update thickness of points with tangent covers
    vector<double> vecMTmodified;
    for(vector<double>::const_iterator it = vecMT.begin(); it != vecMT.end(); it++)
        vecMTmodified.push_back(*it);
    for(int it=meaningThicknessTangentCover.size()-1; it>=0; it--)
    {
        vector<AlphaThickSegmentComputer2DD> fuzzySegmentSet = meaningThicknessTangentCover.at(it);//*it;
        double thickness = meaningThicknessElement.at(it);
        for (vector<AlphaThickSegmentComputer2DD>::const_iterator it_bis = fuzzySegmentSet.begin();it_bis != fuzzySegmentSet.end();it_bis++)
        {
            int idStart = findElement(aContour,getStartPoint(*it_bis));
            int idEnd = findElement(aContour,getEndPoint(*it_bis),idStart);
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
    vector<vector<AlphaThickSegmentComputer2DD> > adaptiveMeaningThicknessTangentCover;
    int idCover = 0;
    for(vector<vector<AlphaThickSegmentComputer2DD> >::const_iterator it = meaningThicknessTangentCover.begin(); it != meaningThicknessTangentCover.end(); it++)
    {
        adaptiveMeaningThicknessTangentCover.push_back(vector<AlphaThickSegmentComputer2DD>());
        vector<AlphaThickSegmentComputer2DD> fuzzySegmentSet = *it;
        vector<AlphaThickSegmentComputer2DD> AdaptiveFuzzySegmentSet;
        int idSeg=0;
        double thickness = meaningThicknessElement.at(idCover);
        for (vector<AlphaThickSegmentComputer2DD>::const_iterator it_bis = fuzzySegmentSet.begin();it_bis != fuzzySegmentSet.end();it_bis++)
        {
            int idStart = findElement(aContour,getStartPoint(*it_bis));
            int idEnd = findElement(aContour,getEndPoint(*it_bis));
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
        for (vector<AlphaThickSegmentComputer2DD>::const_iterator it_bis = AdaptiveFuzzySegmentSet.begin();it_bis != AdaptiveFuzzySegmentSet.end();it_bis++)
            adaptiveMeaningThicknessTangentCover[idCover].push_back(*it_bis);
        idCover++;
    }
    for(vector<vector<AlphaThickSegmentComputer2DD> >::reverse_iterator it1 = adaptiveMeaningThicknessTangentCover.rbegin(); it1 != adaptiveMeaningThicknessTangentCover.rend(); ++it1)
    {
        vector<AlphaThickSegmentComputer2DD>& segmentSet1 = *it1;
        for(vector<vector<AlphaThickSegmentComputer2DD> >::reverse_iterator it2 = it1+1; it2 != adaptiveMeaningThicknessTangentCover.rend(); ++it2)
        {
            vector<AlphaThickSegmentComputer2DD>& segmentSet2 = *it2;
            for (vector<AlphaThickSegmentComputer2DD>::iterator itSeg1 = segmentSet1.begin();itSeg1 != segmentSet1.end();itSeg1++)
            {
                int idCurrentStart = findElement(aContour,getStartPoint(*itSeg1));
                int idCurrentEnd = findElement(aContour,getEndPoint(*itSeg1),idCurrentStart);
                for (vector<AlphaThickSegmentComputer2DD>::iterator itSeg2 = segmentSet2.begin();itSeg2 != segmentSet2.end();itSeg2++)
                {
                    int idStart = findElement(aContour,getStartPoint(*itSeg2));
                    int idEnd = findElement(aContour,getEndPoint(*itSeg2),idStart);
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
    vector<AlphaThickSegmentComputer2DD> adaptiveTangentCover;
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
                int idCurrentStart = findElement(aContour,getStartPoint((adaptiveMeaningThicknessTangentCover.at(it)).at(idCurrentSeg)));
                int idCurrentEnd = findElement(aContour,getEndPoint((adaptiveMeaningThicknessTangentCover.at(it)).at(idCurrentSeg)),idCurrentStart);
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

vector<vector<AlphaThickSegmentComputer2DD> > adaptiveTangentCoverDecomposition(const vector<vector<RealPoint> >& aContour, const vector<vector<double> >& vecMT, std::string filename, unsigned int w, unsigned int h)
{
    vector<vector<AlphaThickSegmentComputer2DD> > vecATC;
    for(size_t it_contour=0; it_contour<aContour.size(); it_contour++)
        vecATC.push_back(adaptiveTangentCoverDecomposition(aContour.at(it_contour),vecMT.at(it_contour)));

    Board2D aBoard;
    if(w!=0 && h!=0)
        drawCanvas(aBoard, w, h);
    GradientColorMap<int> cmap_grad( 0, aContour.size()+1 );
    cmap_grad.addColor( Color( 50, 50, 255 ) );
    cmap_grad.addColor( Color( 255, 0, 0 ) );
    cmap_grad.addColor( Color( 255, 255, 10 ) );
    cmap_grad.addColor( Color( 25, 255, 255 ) );
    cmap_grad.addColor( Color( 255, 25, 255 ) );
    cmap_grad.addColor( Color( 25, 25, 25 ) );
    for(size_t it_contour=0; it_contour<aContour.size(); it_contour++)
        for (vector<AlphaThickSegmentComputer2DD>::const_iterator it=vecATC.at(it_contour).begin(); it != vecATC.at(it_contour).end(); ++it)
            aBoard << SetMode((*it).className(), "BoundingBox")
                   << CustomStyle("AlphaThickSegment/BoundingBox",  new CustomPenColor(cmap_grad(it_contour)))
                   << *it;
    std::string outputExt = filename.substr(filename.find_last_of(".")+1);
    if(outputExt=="svg")
        aBoard.saveSVG(filename.c_str());
    else if (outputExt == "eps")
        aBoard.saveEPS(filename.c_str());

    return vecATC;
}
/*********************************************************/
/********* Adaptive Tangent Cover computation ************/
/*********************************************************/

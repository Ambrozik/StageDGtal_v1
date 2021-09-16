//
// Created by ambrozik on 02/06/2021.
//

//void testSeg(){
//    typedef PointVector<2,double> PointD;
//    typedef PointVector<3,int> Point3D;
//    typedef PointVector<2,int> Point2D;
//    typedef AlphaThickSegmentComputer<Point2D> AlphaThickSegmentComputer2D;
//    typedef std::vector<DGtal::Z3i::Point>::iterator Iterator;
//    typedef std::vector<Line3D> List3DLine;
//    string inputFilename = "../samples/currentFigure.dat"; // line Default
//    vector<Point3D> sequence = PointListReader<Point3D>::getPointsFromFile(inputFilename);
//
//    AlphaThickSegmentComputer2D XY(1);
//    AlphaThickSegmentComputer2D XZ(1);
//    AlphaThickSegmentComputer2D YZ(1);
//    typename std::vector<Point3D>::const_iterator it_p = sequence.begin();
//    for(int i = 0 ; i < sequence.size(); i++){
//        XY.extendFront(convert3Dto2DAXIS<Point3D, Point2D>(*it_p, AXIS::XY));
//        XZ.extendFront(convert3Dto2DAXIS<Point3D, Point2D>(*it_p, AXIS::XZ));
//        YZ.extendFront(convert3Dto2DAXIS<Point3D, Point2D>(*it_p, AXIS::YZ));
//        it_p++;
//    }
//
//    AlphaSegment Oxy , Oyz, Oxz;
//    Oxy.alphaSeg =XY; Oxz.alphaSeg =XZ; Oyz.alphaSeg =YZ;
//    computeSegmentDefinition<PointD, AlphaSegment>(Oxy);
//    computeSegmentDefinition<PointD, AlphaSegment>(Oxz);
//    computeSegmentDefinition<PointD, AlphaSegment>(Oyz);
//
//    Line3D OxyOxz ,OxyOyz ,OxzOyz ;
//    OxyOxz.line3D = make_pair(Oxy,Oxz);
//    OxyOyz.line3D = make_pair(Oxy,Oyz);
//    OxzOyz.line3D = make_pair(Oxz,Oyz);
//
//    D3DComputer(OxyOxz,0);
//    D3DComputer(OxyOyz,1);
//    D3DComputer(OxzOyz,2);
//    std::cout<<"Oxy : " << Oxy.D << " Oxz : " << Oxz.D << " Oyz : " << Oyz.D << std::endl;
//    std::cout<<" OxyOxz : " << OxyOxz.D<<std::endl;
//    std::cout<<" OxyOyz : " << OxyOyz.D<<std::endl;
//    std::cout<<" OxzOyz : " << OxzOyz.D<<std::endl;
//}
int testSeg(int argc, char** argv){
    typedef PointVector<2,double> PointD;
    typedef PointVector<3,int> Point3D;
    typedef PointVector<2,int> Point2D;
    typedef AlphaThickSegmentComputer<Point2D> AlphaThickSegmentComputer2D;
    typedef std::vector<DGtal::Z3i::Point>::iterator Iterator;
    typedef std::vector<Line3D> List3DLine;
    std::vector<Line3D> tangentCorverSet;
    std::vector<Point3D> sequence;
    tangentCorverSet  = createBDSfromFile<Point3D,AlphaThickSegmentComputer2D>("../Results/BNS/test.txt", &sequence);
    std::vector<vector<Line3D>> Oxyz(3);
    for(int i = 0 ; i < tangentCorverSet.size(); i++){
        int idx = tangentCorverSet.at(i).indexA;
        Oxyz[idx].push_back(tangentCorverSet.at(i));
    }
    bool flag = true;
    DGtal::Z3i::Point p;
    vector<Point3D> sequenceExtremity;
    QApplication application(argc,argv);
    DGtal::Z3i::Point lowerBound = sequence[ 0 ];
    DGtal::Z3i::Point upperBound = sequence[ 0 ];

    for ( int j = 1; j < sequence.size(); ++j ){
        lowerBound = lowerBound.inf( sequence[ j ] );
        upperBound = upperBound.sup( sequence[ j ] );
    }
    lowerBound -= DGtal::Z3i::Point::diagonal( 5);
    upperBound += DGtal::Z3i::Point::diagonal( 5 + 1 );
    K3 ks; ks.init( lowerBound, upperBound, true );
    Viewer3D<Z3,K3> viewer( ks );
    viewer.show();
    viewer  << SetMode3D(p.className(), "Grid");
    displayAXISs<DGtal::Z3i::Point,DGtal::Z3i::RealPoint, Z3i::Space, Z3i::KSpace>(viewer, lowerBound,upperBound,"COLORED");
    displayPoint2D<DGtal::Z3i::Point,DGtal::Z3i::RealPoint, Z3i::Space, Z3i::KSpace, Point3D>(viewer,ks,sequence,"Point");
    viewer.setFillColor(Color::Red);


    for(int i = 0 ; i < tangentCorverSet.size(); i++){
        displayAlphathickSemgent<PointD,Z3i::Space,Z3i::KSpace,AlphaThickSegmentComputer2D>(viewer,ks,tangentCorverSet.at(i).line3D.first.alphaSeg,tangentCorverSet.at(i),tangentCorverSet.at(i).axes.first);
        displayAlphathickSemgent<PointD,Z3i::Space,Z3i::KSpace,AlphaThickSegmentComputer2D>(viewer,ks,tangentCorverSet.at(i).line3D.second.alphaSeg,tangentCorverSet.at(i),tangentCorverSet.at(i).axes.second);
        drawAsBoundingBox<PointD,Z3i::Space,Z3i::KSpace>(viewer,ks,tangentCorverSet.at(i),tangentCorverSet.at(i).indexA);
    }


    //display Point3D
    viewer << CustomColors3D( viewer.getLineColor(), viewer.getFillColor() );
    for ( vector<DGtal::Z3i::Point>::const_iterator it = sequence.begin(), itE = sequence.end();it != itE; ++it ){
        viewer << *it;
    }

    SequenceLine<Point3D ,Z3i::Space,Z3i::KSpace>(viewer,ks,sequence);

    viewer << CustomColors3D( Color( 127, 0, 255, 255 ), Color( 127, 0, 255, 255 ));
    vector<Point3D> seq2 ;

    viewer << Viewer3D<>::updateDisplay;
    flag = application.exec();
    return flag;
}
//
// Created by ambrozik on 02/06/2021.
//

int FTC(int argc, char** argv){
    typedef PointVector<2,double> PointD;
    typedef PointVector<3,int> Point3D;
    typedef PointVector<2,int> Point2D;
    typedef AlphaThickSegmentComputer<Point2D> AlphaThickSegmentComputer2D;
    typedef std::vector<DGtal::Z3i::Point>::iterator Iterator;
    typedef std::vector<Line3D> List3DLine;

    //all option
    /* Read program parameters */
    double angleTol=10.0;
    po::options_description general_opt("Allowed options are: ");
    general_opt.add_options()
            ("help,h", "display this message")
            ("input,i", po::value<std::string>(), "input filename")
            ("ThicknessMax,t", po::value<std::vector<double>>()->multitoken(), "thicknessMax");
    string inputFilename;
    po::variables_map vm;
    po::store(po::parse_command_line(argc,argv,general_opt),vm);
    po::notify(vm);
    if(vm.count("input")){
        inputFilename = vm["input"].as<string>();
        struct stat buffer;
        try{
            if(stat(inputFilename.c_str(),&buffer) != 0){
                throw std::string("File doesn't exist");
            }
        }catch(std::string const& chaine){
            std::cerr << chaine <<endl;
            return 0;
        }
    }else{
        inputFilename = "../samples/currentFigure.dat"; // line Default
    }
    std::vector<double> thicknesslist ;
    if (vm.count("ThicknessMax") && vm["ThicknessMax"].as<vector<double>>().size() == 3) {
        thicknesslist = vm["ThicknessMax"].as<std::vector<double>>();
    }else{
        thicknesslist = std::vector<double>{1,1,1};
    }
    /* Read contour */
    vector<Point3D> sequence = PointListReader<Point3D>::getPointsFromFile(inputFilename);
    std::vector<List3DLine> Oxyz(3);
    vector<Point3D> sequenceExtremity;
    CreatorAlphaThickSegment<Point2D, AlphaThickSegmentComputer2D>*creator;
    creator = new AlphaThickSegmentFurtherAway<Point2D, AlphaThickSegmentComputer2D>();
    creator->setMaxThickness(thicknesslist);
    creator->choseMod(1);
    Oxyz = tgtCover<PointD,Point2D,Point3D,AlphaThickSegmentComputer2D,List3DLine>(*creator ,sequence);
    bool flag = true;
    DGtal::Z3i::Point p;

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


    //display Alphasegment on OxyOxz
    std::cout<<"OxyOxz"<<std::endl;
    for(std::vector<Line3D>::const_iterator it = Oxyz.at(0).begin(), itE = Oxyz.at(0).end(); it != itE; it++){
        displayAlphathickSemgent<PointD,Z3i::Space,Z3i::KSpace,AlphaThickSegmentComputer2D>(viewer,ks,(*it).line3D.first.alphaSeg,*it,AXIS::XY);
        displayAlphathickSemgent<PointD,Z3i::Space,Z3i::KSpace,AlphaThickSegmentComputer2D>(viewer,ks,(*it).line3D.second.alphaSeg,*it,AXIS::XZ);
        drawAsBoundingBox<PointD,Z3i::Space,Z3i::KSpace>(viewer,ks,*it,0);

    }
    //display AlphaSegments on OxyOyz
    for(std::vector<Line3D>::const_iterator it = Oxyz.at(1).begin(), itE = Oxyz.at(1).end(); it != itE; it++){
        displayAlphathickSemgent<PointD,Z3i::Space,Z3i::KSpace,AlphaThickSegmentComputer2D>(viewer,ks,(*it).line3D.first.alphaSeg,*it,AXIS::XY);
        displayAlphathickSemgent<PointD,Z3i::Space,Z3i::KSpace,AlphaThickSegmentComputer2D>(viewer,ks,(*it).line3D.second.alphaSeg,*it,AXIS::YZ);
        drawAsBoundingBox<PointD,Z3i::Space,Z3i::KSpace>(viewer,ks,*it,1);
    }

    //display AlphaSegments on OxzOyx
    for(std::vector<Line3D>::const_iterator it = Oxyz.at(2).begin(), itE = Oxyz.at(2).end(); it != itE; it++){
        drawAsBoundingBox<PointD,Z3i::Space,Z3i::KSpace>(viewer,ks,*it,2);
        displayAlphathickSemgent<PointD,Z3i::Space,Z3i::KSpace,AlphaThickSegmentComputer2D>(viewer,ks,(*it).line3D.first.alphaSeg,*it,AXIS::XZ);
        displayAlphathickSemgent<PointD,Z3i::Space,Z3i::KSpace,AlphaThickSegmentComputer2D>(viewer,ks,(*it).line3D.second.alphaSeg,*it,AXIS::YZ);
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
    delete creator;
    return flag;
}

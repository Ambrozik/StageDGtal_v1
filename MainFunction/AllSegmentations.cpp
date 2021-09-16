//
// Created by ambrozik on 02/06/2021.
//

int program( int argc, char** argv )
{
    typedef PointVector<2,double> PointD;
    typedef PointVector<3,int> Point3D;
    typedef PointVector<2,int> Point2D;
    typedef AlphaThickSegmentComputer<Point2D> AlphaThickSegmentComputer2D;
    typedef std::vector<DGtal::Z3i::Point>::iterator Iterator;
    typedef std::vector<Line3D> List3DLine;
    //all option
    po::options_description general_opt("Allowed options are: ");
    general_opt.add_options()
            ("help,h","display this message")
            ("input,i", po::value<std::string>(), "input segment 3D")
            ("BijS,b", po::bool_switch()->default_value(false), "Bijective segmentation")
            ("FurtherAway,a", po::bool_switch()->default_value(false),"Further away segmentation")
            ("FullSfs,f", po::bool_switch()->default_value(false), "Full segmentation")
            ("tgtCover", po::bool_switch()->default_value(false), "tengencielMaxSegment segmentation")
            ("GreedyS,g", po::bool_switch()->default_value(false), "Greddy segmentation")
            ("Aline,l", po::value<int>(), "specify first point")
            ("closedFigure,c", po::bool_switch()->default_value(false), "first point is the last point")
            ("ThicknessMax,t", po::value<std::vector<double>>()->multitoken(), "thicknessMax")
            ("mod,m", po::value<int>()->default_value(0), "mod = 0 (tq 2prj), mod =1 (tq 3prj)");

    po::variables_map vm;
    po::store(po::parse_command_line(argc,argv,general_opt),vm);
    po::notify(vm);

    //display help
    if(vm.count("help")){
        std::cout << general_opt<<std::endl;
        return 0;
    }
    //input line3D
    string inputFilename;
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

    vector<Point3D> sequence = PointListReader<Point3D>::getPointsFromFile(inputFilename);
    if(vm["closedFigure"].as<bool>()) {
        sequence.push_back(sequence[0]);
    }

    std::vector<List3DLine> Oxyz(3);
    vector<Point3D> sequenceExtremity;
    CreatorAlphaThickSegment<Point2D, AlphaThickSegmentComputer2D>*creator;
    try {
        if (vm["FurtherAway"].as<bool>() && vm["BijS"].as<bool>()) {
            throw std::string("must choose only one option 1 ");
        }else{
            if(vm["FurtherAway"].as<bool>()){
                creator = new AlphaThickSegmentFurtherAway<Point2D, AlphaThickSegmentComputer2D>();
            }else { //if(vm["BijectiveProjection"].as<bool>()){
                creator = new AlphaThickSegmentBijectiveProjection<Point2D, AlphaThickSegmentComputer2D>(); //default segmentation
            }
        }
    }catch(std::string const& chaine){
        std::cerr << chaine <<endl;
        return 0;
    }

    std::vector<double> thicknesslist ;
    //setThickness
    if (vm.count("ThicknessMax") && vm["ThicknessMax"].as<vector<double>>().size() == 3) {
        thicknesslist = vm["ThicknessMax"].as<std::vector<double>>();
    }else{
        thicknesslist = std::vector<double>{1,1,1};
    }
    creator->setMaxThickness(thicknesslist);

    //chose nbprojectionValid
    if(vm["mod"].as<int>() == 0 ){
        creator->choseMod(0);
    }else{
        creator->choseMod(1);
    }

    //mode of segmentation
    try {
        if ((vm["FullSfs"].as<bool>()+ vm["GreedyS"].as<bool>() + vm["tgtCover"].as<bool>()+ vm.count("Aline")) > 1) {
            throw std::string("must choose only one option 2");
        }else{
            if(vm["tgtCover"].as<bool>()){
                Oxyz = tgtCover<PointD,Point2D,Point3D,AlphaThickSegmentComputer2D,List3DLine>(*creator ,sequence);
            }else
            if(vm["FullSfs"].as<bool>()){
                Oxyz = computerLine3D<PointD,Point2D,Point3D,AlphaThickSegmentComputer2D,List3DLine>(*creator ,sequence);
            }else if(vm["GreedyS"].as<bool>()){
                Oxyz = greedyAlphaThickDecomposition<PointD,Point2D,Point3D,AlphaThickSegmentComputer2D,List3DLine>(*creator,sequence, sequenceExtremity);
            }else{ //default option
                int start = 0 ;
                if (vm.count("Aline")){
                    start = vm["Aline"].as<int>();
                }
                try {
                    if (start > sequence.size() - 1  || start < 0) {
                        throw std::string("index is out of range");
                    }else{
                        computerALine<PointD,Point2D,Point3D,AlphaThickSegmentComputer2D,List3DLine>(*creator,sequence,start,Oxyz);
                    }
                }catch(std::string const& chaine){
                    std::cerr << chaine <<endl;
                    return 0;
                }
            }
        }
    }catch(std::string const& chaine){
        std::cerr << chaine <<endl;
        return 0;
    }

    //whrite segmentation
    vector<Line3D> line;
    for(int i = 0 ; i < Oxyz.size(); i++){
        for(int j = 0 ; j < Oxyz.at(i).size(); j++){
            line.push_back(Oxyz.at(i).at(j));
        }
    }
   // toFile(sequence,line, "../Results/BNS/test", "segmentation de machin truc");
    //createBDSfromFile<Point3D,AlphaThickSegmentComputer2D>("../Results/BNS/test.txt");
    //display environment
    bool flag = true;
    DGtal::Z3i::Point p;

    QApplication application(argc,argv);
    DGtal::Z3i::Point lowerBound = sequence[ 0 ];
    DGtal::Z3i::Point upperBound = sequence[ 0 ];

    for ( unsigned int j = 1; j < sequence.size(); ++j ){
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

    //display greedysegmentation
    for(vector<DGtal::Z3i::Point>::const_iterator it = sequenceExtremity.begin(), itE = sequenceExtremity.end();it != itE; ++it){
        viewer <<*it;
    }

    //display Alphasegment on OxyOxz
    std::cout<<"OxyOxz"<<std::endl;
    for(std::vector<Line3D>::const_iterator it = Oxyz.at(0).begin(), itE = Oxyz.at(0).end(); it != itE; it++){
        displayAlphathickSemgent<PointD,Z3i::Space,Z3i::KSpace,AlphaThickSegmentComputer2D>(viewer,ks,(*it).line3D.first.alphaSeg,*it,AXIS::XY);
        displayAlphathickSemgent<PointD,Z3i::Space,Z3i::KSpace,AlphaThickSegmentComputer2D>(viewer,ks,(*it).line3D.second.alphaSeg,*it,AXIS::XZ);
        drawAsBoundingBox<PointD,Z3i::Space,Z3i::KSpace>(viewer,ks,*it,0);
        std::cout<<(*it).line3D.first.D<<" " <<(*it).line3D.second.D<<std::endl;
        std::cout<<(*it).D<<std::endl;
    }
    //display AlphaSegments on OxyOyz
    std::cout<<"OxyOyz"<<std::endl;
    for(std::vector<Line3D>::const_iterator it = Oxyz.at(1).begin(), itE = Oxyz.at(1).end(); it != itE; it++){
        displayAlphathickSemgent<PointD,Z3i::Space,Z3i::KSpace,AlphaThickSegmentComputer2D>(viewer,ks,(*it).line3D.first.alphaSeg,*it,AXIS::XY);
        displayAlphathickSemgent<PointD,Z3i::Space,Z3i::KSpace,AlphaThickSegmentComputer2D>(viewer,ks,(*it).line3D.second.alphaSeg,*it,AXIS::YZ);
        drawAsBoundingBox<PointD,Z3i::Space,Z3i::KSpace>(viewer,ks,*it,1);
    }

    //display AlphaSegments on OxzOyx
    std::cout<<"OxzOyz"<<std::endl;
    for(std::vector<Line3D>::const_iterator it = Oxyz.at(2).begin(), itE = Oxyz.at(2).end(); it != itE; it++){
        drawAsBoundingBox<PointD,Z3i::Space,Z3i::KSpace>(viewer,ks,*it,2);
        displayAlphathickSemgent<PointD,Z3i::Space,Z3i::KSpace,AlphaThickSegmentComputer2D>(viewer,ks,(*it).line3D.first.alphaSeg,*it,AXIS::XZ);
        displayAlphathickSemgent<PointD,Z3i::Space,Z3i::KSpace,AlphaThickSegmentComputer2D>(viewer,ks,(*it).line3D.second.alphaSeg,*it,AXIS::YZ);
    }

    viewer << CustomColors3D(DGtal::Color( 255, 0, 0, 255 ),DGtal::Color( 0,0,0,0 ));
    viewer << Point3D(3, 0, 0);
    viewer << CustomColors3D(DGtal::Color( 0, 0, 0, 150 ),DGtal::Color( 0, 0, 0, 150 ));


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


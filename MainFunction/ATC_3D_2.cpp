//
// Created by ambrozik on 02/06/2021.
//
#include "../ATC_Computation_3D.h"
#include "../lambdaMST.h"
#include "../curvature.h"
int Meaningful_v2(int argc, char *argv[])
{
    typedef PointVector<2,double> PointD;
    typedef PointVector<3,int> Point3D;
    typedef PointVector<2,int> Point2D;
    /* Read program parameters */
    double angleTol=10.0;
    po::options_description general_opt("Allowed options are: ");
    general_opt.add_options()
            ("help,h", "display this message")
            ("input,i", po::value<std::string>(), "input filename")
            ("imageneDir,d", po::value<std::string>(), "specify the ImaGene directory")
            ("sourceImageWidth", po::value<unsigned int>(), "source image width")
            ("sourceImageHeight", po::value<unsigned int>(), "source image height")
            ("output,o", po::value<std::string>()->default_value("./"), "output filename")
            ("eps,e","set output with eps format (default svg)")
            ("maxScale",  po::value<double>()->default_value(15.0), "set the maximal thickness for meaningful thickness detection")
            ("mode,m",  po::value<unsigned int>()->default_value(0), " 0 to max , 1 to min ")
            ("samplingStep",  po::value<double>()->default_value(1.0), "set the threshold of admissible angle in the tangent space");

    bool parseOK=true;
    po::variables_map vm;
    try{
        po::store(po::parse_command_line(argc, argv, general_opt), vm);
    }catch(const std::exception& ex){
        trace.info()<< "Error checking program options: "<< ex.what()<< std::endl;
        parseOK=false;
    }
    po::notify(vm);
    if(vm.count("help")||argc<=1|| !parseOK || !vm.count("input") || !vm.count("output") || !vm.count("imageneDir"))
    {
        trace.info()<< "Contour decomposition." <<std::endl << "Options: "<<std::endl
                    << general_opt << "\n";
        return 0;
    }
    string ImaGeneDIR = vm["imageneDir"].as<std::string>();
    double maxMT = vm["maxScale"].as<double>();
    double stepMT = vm["samplingStep"].as<double>();
    double mode = vm["mode"].as<unsigned int>();
    string inputFile=vm["input"].as<std::string>();
    string outputFile=vm["output"].as<std::string>();
    bool displayImageCanvas=vm.count("sourceImageWidth") && vm.count("sourceImageHeight");
    bool eps = vm.count("eps");
    unsigned int widthCanvas, heightCanvas=0;
    if (displayImageCanvas){
        widthCanvas=vm["sourceImageWidth"].as<unsigned int>();
        heightCanvas=vm["sourceImageHeight"].as<unsigned int>();
    }
    /* Read program parameters */

    /* Read contour */

    vector<Point3D> sequence = PointListReader<Point3D>::getPointsFromFile(inputFile);
    std::vector<std::vector<DGtal::Z2i::RealPoint>> vecPts(3);
    for(int i = 0 ; i < sequence.size(); i++){
        for ( const auto e : All ){
            vecPts.at(e).push_back(convert3Dto2DAXIS<Point3D,DGtal::Z2i::RealPoint>(sequence.at(i),e));
        }
    }

    std::stringstream instruction;
    vector<vector<double> > vecMT;
    for ( int i = 0 ; i < 3 ; i++ ){
        std::string contourFile=outputFile+"_"+std::to_string(i)+".sdp";
        writeFile(vecPts.at(i),contourFile.c_str());
        std::string noiseLevelMTFile=outputFile+"MeanThickness_"+std::to_string(i)+".txt";
        instruction << ImaGeneDIR << "/build/tests/TestCompNoiseDetect/displayNoiseBS -srcPolygon " << contourFile
                    << " 0 1 CLOSED -setSampling "<<maxMT<<" "<<stepMT
                    << " -exportNoiseLevel "<< noiseLevelMTFile.c_str();
        std::system(instruction.str().c_str());
        vector<double> oneVecMT=readMeanindfulThicknessFile(noiseLevelMTFile.c_str());
        vecMT.push_back(oneVecMT);
    }

    map<vector<double>,int> nbTypeSegment;

    vector<Line3D>  tangentCorverSet=adaptiveTangentCoverDecomposition3D_v2<Point3D>(sequence,vecMT,nbTypeSegment);
    vector<DGtal::PointVector<3,double>> lambdaMSTEstimator = lambda_mst_estimator_3D<Point3D >(sequence,tangentCorverSet,"Exponential");

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
    displayPoint2DNoisesColor<DGtal::Z3i::Point,DGtal::Z3i::RealPoint, Z3i::Space, Z3i::KSpace, Point3D>(viewer,ks,vecMT,sequence,"Point");

    for(int i = 0 ; i < lambdaMSTEstimator.size(); i++){
        drawLambdaMSTestimator<DGtal::Z3i::Point,Z3i::Space,Z3i::KSpace>(viewer,ks,sequence,lambdaMSTEstimator);
        drawLambdaMSTestimatorNormale<DGtal::Z3i::Point,Z3i::Space,Z3i::KSpace>(viewer,ks,sequence,lambdaMSTEstimator);
    }

    std::vector<double> c = curvatureOfWidth(tangentCorverSet,sequence);
    std::string curvatureFile=outputFile+"_curve.txt";
    std::string curvatureFilePlot=outputFile+"_curve_plot.png";
    ofstream f(curvatureFile);
    for(int i = 0 ; i < c.size(); i++){
        f << c.at(i) << std::endl;
    }
    std::stringstream instruction2;

    HueShadeColorMap<float,1>  hueShade(0,nbTypeSegment.size());
    for(int i = 0 ; i < tangentCorverSet.size(); i++){
        displayAlphathickSemgent<PointD,Z3i::Space,Z3i::KSpace,AlphaThickSegmentComputer2D>(viewer,ks,tangentCorverSet.at(i).line3D.first.alphaSeg,tangentCorverSet.at(i),tangentCorverSet.at(i).axes.first);
        displayAlphathickSemgent<PointD,Z3i::Space,Z3i::KSpace,AlphaThickSegmentComputer2D>(viewer,ks,tangentCorverSet.at(i).line3D.second.alphaSeg,tangentCorverSet.at(i),tangentCorverSet.at(i).axes.second);
        auto idx = nbTypeSegment.find({tangentCorverSet.at(i).thickness3D.at(0),tangentCorverSet.at(i).thickness3D.at(1),tangentCorverSet.at(i).thickness3D.at(2)});
        int idxColor = idx->second;
        Color col = hueShade(idxColor);

        std::cout << "thickness : " << tangentCorverSet.at(i).thickness3D.at(0)<< " " << tangentCorverSet.at(i).thickness3D.at(1) << " " << tangentCorverSet.at(i).thickness3D.at(2)<< endl;
        drawAsBoundingBox<PointD,Z3i::Space,Z3i::KSpace>(viewer,ks,tangentCorverSet.at(i),col);
    }

    for(std::map<vector<double>,int>::iterator it=nbTypeSegment.begin(); it!=nbTypeSegment  .end(); ++it){
        Color col = hueShade(it->second);
        std::cout<< col << "==>" << it->first.at(0) << ","
        << it->first.at(1) << "," << it->first.at(2)<<std::endl;
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


    instruction2 <<" python3 ../analyses.py -i " << curvatureFile <<" -o " <<curvatureFilePlot ;
    std::cout<<curvatureFile<<std::endl;
    std::system(instruction2.str().c_str());
    return flag;
}
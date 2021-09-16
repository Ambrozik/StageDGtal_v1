#include "../ATC_Computation_3D.h"

//
// Created by ambrozik on 16/08/2021.
//
int test_1(int argc, char **argv){
    using namespace std;
    using namespace DGtal;
    typedef PointVector<3,int> Point3D;
    typedef PointVector<2,int> Point2D;
    typedef PointVector<2,double> PointD;
    typedef AlphaThickSegmentComputer<Point2D> AlphaThickSegmentComputer2D;
    typedef std::vector<DGtal::Z3i::Point>::iterator Iterator;
    typedef std::vector<Line3D> List3DLine;
    CreatorAlphaThickSegment<Point2D, AlphaThickSegmentComputer2D>*creator;

    // specify command line ----------------------------------------------
    QApplication application(argc,argv); // remove Qt arguments.

    // parse command line using CLI ----------------------------------------------
    std::string inputnoise3D;
    std::string outputFileName {"ATC_3D"};

    bool curvature = {false};
    bool lambda_mst = {false};
    bool save = {false};
    CLI::App app("my program");
    std::string file;
    std::string output_curve;
    std::string output_mst;
    app.description("This program allows you to build the adaptative tengencial conver 3D of a 3D noisy curve ");
    app.set_help_all_flag("--help-all", "Expand all help");
    app.add_option("-f,--file", file, "File name")->required()->check(CLI::ExistingFile);
    auto curvat = app.add_option_group("save_curvature");
    curvat->add_flag("-c,--curvature", curvature);
    curvat->add_option("--outfileCurv", output_curve);

    auto lambda = app.add_option_group("save_lambdaMST");
    lambda->add_flag("-l,--lambdaMst", lambda_mst);
    lambda->add_option("--outfileLambda", output_mst);

    auto save_groupe = app.add_option_group("save_groupe");
    std::string  outfile;
    std::string  commentary;
    save_groupe->add_flag("-s,--save", save);
    save_groupe->add_option("-o, --outfile", outfile);
    save_groupe->add_option("--com", commentary);

    CLI::App *segApp= app.add_subcommand("segmentation", "segmentation mode");
    CLI::App *ccApp = app.add_subcommand("creatcurve", "create curve from a file with segment 3D");
    app.require_subcommand(1);  // segment or creatcurve

    CLI::App *ATCAPP = segApp->add_subcommand("ATC");
    CLI::App *segSimple = segApp->add_subcommand("SimpleSegmentation");
    segApp->require_subcommand(1);
        //ATC
        ATCAPP->add_option("-n, --noisyEstimator", inputnoise3D, "the name of the text file containing the list of noisy level of a point per line( Oxy Oxz Oyz)");


        //if Segmentation
            //if simple segmentation
            bool AT = {false};
            bool GS = {false};
            bool FS = {false};
            vector<double> thickness = {1,1,1};

            auto segmen = segSimple->add_option_group("output_format", "formatting type for output");
            segmen->add_flag("--AT", AT);
            segmen->add_flag("--GS",GS);
            segmen->add_flag("--FS",FS);
            segmen->require_option(1);
            segSimple->add_option("-t",thickness, "thicknessMax")->expected(3);

            //if ATC


    app.get_formatter()->column_width(40);

       CLI11_PARSE(app, argc, argv);
   // END parse command line using CLI ----------------------------------------------

   //sequence
   vector<Point3D> sequence;
   fstream inputStream;
   inputStream.open ( file.c_str(), ios::in);
   std::vector<Line3D> seg;
   std::vector<vector<Line3D>> Oxyz(3);
   vector<Point3D> sequenceExtremity;
   // create curve 3D from file
   if(*ccApp){
       seg  = createBDSfromFile<Point3D,AlphaThickSegmentComputer2D>(file, &sequence);
       for(int i = 0 ; i < seg.size(); i++){
           int idx = seg.at(i).indexA;
           Oxyz[idx].push_back(seg.at(i));
       }
   }
   //segmentation from curve 3D
   if(*segApp){
       try
       {
           sequence = PointListReader<Point3D>::getPointsFromInputStream( inputStream );
           if ( sequence.size() == 0) throw IOException();
       }
       catch (DGtal::IOException & ioe)
       {
           trace.error() << "Size is null." << endl;
       }
       if(*segSimple){
            creator = new AlphaThickSegmentFurtherAway<Point2D, AlphaThickSegmentComputer2D>();
           creator->setMaxThickness(thickness);
           creator->choseMod(0);
           if(GS){
                seg = greedyAlphaThickDecomposition<PointD,Point2D,Point3D,AlphaThickSegmentComputer2D,List3DLine>(*creator,sequence, sequenceExtremity,Oxyz);
           }
           if(FS){
                seg = computerLine3D<PointD,Point2D,Point3D,AlphaThickSegmentComputer2D,List3DLine>(*creator ,sequence,&Oxyz);
           }
           if(AT){
                seg = tgtCover<PointD,Point2D,Point3D,AlphaThickSegmentComputer2D,List3DLine>(*creator ,sequence, Oxyz);
           }
       }
       if(*ATCAPP){
           vector<vector<double>> vecMT;
           map<vector<double>,int> nbTypeSegment;
           try{
               struct stat buffer;
               try{
                   if(stat(inputnoise3D.c_str(),&buffer) != 0){
                       throw std::string("File doesn't exist");
                   }
               }catch(std::string const& chaine){
                   std::cerr << chaine <<endl;
                   return 0;
               }
               vecMT = getNoisy(inputnoise3D);
               if ( vecMT[0].size() != sequence.size()) throw IOException();
           }   catch (DGtal::IOException & ioe)
           {
               trace.error() << "Size of noise file doesn't have the same number of point." << endl;
           }
           seg = adaptiveTangentCoverDecomposition3D_v2<Point3D>(sequence,vecMT,nbTypeSegment);
       }
   }

   if(save){
       toFile(sequence,seg, outfile, commentary);
   }

   if(curvature){
       std::vector<double> c = curvatureOfWidth(seg,sequence);
       std::string curvatureFile=output_curve+"_curve.txt";
       ofstream f(curvatureFile);
       for(int i = 0 ; i < c.size(); i++){
           f << c.at(i) << std::endl;
       }
   }

   //print
   bool flag = true;
   DGtal::Z3i::Point p;

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

   for(int i = 0 ; i < seg.size(); i++){
       displayAlphathickSemgent<PointD,Z3i::Space,Z3i::KSpace,AlphaThickSegmentComputer2D>(viewer,ks,seg.at(i).line3D.first.alphaSeg,seg.at(i),seg.at(i).axes.first);
       displayAlphathickSemgent<PointD,Z3i::Space,Z3i::KSpace,AlphaThickSegmentComputer2D>(viewer,ks,seg.at(i).line3D.second.alphaSeg,seg.at(i),seg.at(i).axes.second);
       drawAsBoundingBox<PointD,Z3i::Space,Z3i::KSpace>(viewer,ks,seg.at(i),seg.at(i).indexA);
   }
   if(lambda_mst){
      vector<DGtal::PointVector<3,double>> lambdaMSTEstimator = lambda_mst_estimator_3D<Point3D >(sequence,seg);
      for(int i = 0 ; i < lambdaMSTEstimator.size(); i++){
          drawLambdaMSTestimator<DGtal::Z3i::Point,Z3i::Space,Z3i::KSpace>(viewer,ks,sequence,lambdaMSTEstimator);
          drawLambdaMSTestimatorNormale<DGtal::Z3i::Point,Z3i::Space,Z3i::KSpace>(viewer,ks,sequence,lambdaMSTEstimator);
      }
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
   //delete creator;
   return flag;
}
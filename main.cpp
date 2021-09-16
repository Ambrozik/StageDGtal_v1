#include <iostream>
#include "DGtal/io/viewers/Viewer3D.h"
#ifdef WITH_CAIRO
#include <DGtal/io/Display3D.h>
#endif
enum AXIS {XY = 0,XZ = 1,YZ = 2};
static const AXIS All[] = { XY, XZ, YZ };


#include "DGtal/io/DrawWithDisplay3DModifier.h"
#include "DGtal/io/readers/PointListReader.h"
#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/boards/Board2D.h"
#include <map>
#include "AlphaThickSegment3D.h"
#include "Display.h"
#include <sys/stat.h>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include "AdaptiveTangentCover/functions.h"

using namespace std;
using namespace DGtal;
using namespace Z3i;
const double MS3D_LINESIZE = 0.05;
namespace  po  = boost::program_options;

#include "CLI11.hpp"
//#include "MainFunction/tgtCover.cpp"
//#include "MainFunction/AllSegmentations.cpp"
//#include "MainFunction/TestDefintion.cpp"
//#include "MainFunction/ATC_3D.cpp"
#include "MainFunction/ATC_3D_2.cpp"
//#include "MainFunction/V1.cpp"

int main(int argc, char** argv){
//    testSeg(argc,argv);
   // program(argc,argv);
   // FTC(argc,argv);
  //  Meaningful(argc,argv);
    Meaningful_v2(argc,argv);
   //test_1(argc,argv);
    return 0;

}





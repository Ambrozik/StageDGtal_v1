#ifndef TESTFUNCTION
#define TESTFUNCTION

#include <iostream>
#include <exception>
#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/boards/Board2D.h"
#include "DGtal/io/colormaps/HueShadeColorMap.h"

#include "DGtal/geometry/curves/ArithmeticalDSSComputer.h"
#include "DGtal/geometry/curves/SaturatedSegmentation.h"
#include <DGtal/geometry/curves/AlphaThickSegmentComputer.h>

#include "functions.h"

using namespace std;
using namespace DGtal;
using namespace Z2i;

#define FILENAMESIZE 200

/*** Adaptative tangent cover decomposition ***/
vector<vector<AlphaThickSegmentComputer2DD> > adaptiveTangentCoverDecomposition(const vector<vector<DGtal::Z2i::RealPoint> >& aContour, const vector<vector<double> >& vecMT, string filename, unsigned int w=200, unsigned int h=200);
/*** Adaptative tangent cover decomposition ***/

#endif // TESTFUNCTION


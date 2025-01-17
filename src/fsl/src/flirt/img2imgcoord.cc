/*  img2imgcoord.cc

    Mark Jenkinson, FMRIB Image Analysis Group

    Copyright (C) 2000 University of Oxford  */

/*  Part of FSL - FMRIB's Software Library
    http://www.fmrib.ox.ac.uk/fsl
    fsl@fmrib.ox.ac.uk
    
    Developed at FMRIB (Oxford Centre for Functional Magnetic Resonance
    Imaging of the Brain), Department of Clinical Neurology, Oxford
    University, Oxford, UK
    
    
    LICENCE
    
    FMRIB Software Library, Release 3.3 (c) 2006, The University of
    Oxford (the "Software")
    
    The Software remains the property of the University of Oxford ("the
    University").
    
    The Software is distributed "AS IS" under this Licence solely for
    non-commercial use in the hope that it will be useful, but in order
    that the University as a charitable foundation protects its assets for
    the benefit of its educational and research purposes, the University
    makes clear that no condition is made or to be implied, nor is any
    warranty given or to be implied, as to the accuracy of the Software,
    or that it will be suitable for any particular purpose or for use
    under any specific conditions. Furthermore, the University disclaims
    all responsibility for the use which is made of the Software. It
    further disclaims any liability for the outcomes arising from using
    the Software.
    
    The Licensee agrees to indemnify the University and hold the
    University harmless from and against any and all claims, damages and
    liabilities asserted by third parties (including claims for
    negligence) which arise directly or indirectly from the use of the
    Software or the sale of any products based on the Software.
    
    No part of the Software may be reproduced, modified, transmitted or
    transferred in any form or by any means, electronic or mechanical,
    without the express permission of the University. The permission of
    the University is not required if the said reproduction, modification,
    transmission or transference is done without financial return, the
    conditions of this Licence are imposed upon the receiver of the
    product, and all original and amended source code is included in any
    transmitted product. You may be held legally responsible for any
    copyright infringement that is caused or encouraged by your failure to
    abide by these terms and conditions.
    
    You are not permitted under this Licence to use this Software
    commercially. Use for which any financial return is received shall be
    defined as commercial use, and includes (1) integration of all or part
    of the source code or the Software into a product for sale or license
    by or on behalf of Licensee to third parties or (2) use of the
    Software or any derivative of it for research with the final aim of
    developing software products for sale or license to a third party or
    (3) use of the Software or any derivative of it for research with the
    final aim of developing non-software products for sale or license to a
    third party, or (4) use of the Software to provide any service to an
    external organisation for which payment is received. If you are
    interested in using the Software commercially, please contact Isis
    Innovation Limited ("Isis"), the technology transfer company of the
    University, to negotiate a licence. Contact details are:
    innovation@isis.ox.ac.uk quoting reference DE/1112. */

#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#define WANT_STREAM
#define WANT_MATH

#include "newmatap.h"
#include "newmatio.h"
#include "miscmaths/miscmaths.h"
#include "newimage/newimageall.h"

#ifndef NO_NAMESPACE
 using namespace MISCMATHS;
 using namespace NEWMAT;
 using namespace NEWIMAGE;
#endif

////////////////////////////////////////////////////////////////////////////
// the real defaults are provided in the function parse_command_line

class globaloptions {
public:
  string destfname;
  string srcfname;
  string xfmfname;
  string coordfname;
  bool mm;
  int verbose;
public:
  globaloptions();
  ~globaloptions() {};
};

globaloptions globalopts;


globaloptions::globaloptions()
{
  // set up defaults
  destfname = "";
  srcfname = "";
  coordfname = "";
  xfmfname = "";
  mm = false;
}


////////////////////////////////////////////////////////////////////////////

// Parsing functions for command line parameters

void print_usage(int argc, char *argv[])
{
  cout << "Usage: " << argv[0] << " [options] <filename containing coordinates>\n\n"
       << "  Options are:\n"
       << "        -src <Source Image filename>         (NB: volume, not timeseries)\n"
       << "        -dest <Destination Image filename>   (NB: volume, not timeseries)\n"
       << "        -xfm <Source to Destination transform filename>\n"
       << "        -vox                                 (all coordinates in voxels - default)\n"
       << "        -mm                                  (all coordinates in mm)\n"
       << "        -help\n\n"
       << " Note that the first three options are compulsory\n";
}


void parse_command_line(int argc, char* argv[])
{
  if(argc<2){
    print_usage(argc,argv);
    exit(1);
  }


  int n=1;
  string arg;
  char first;

  while (n<argc) {
    arg=argv[n];
    if (arg.size()<1) { n++; continue; }
    first = arg[0];
    if (first!='-') {
      globalopts.coordfname = arg;
      n++;
      continue;
    }
    
    // put options without arguments here
    if ( arg == "-help" ) {
      print_usage(argc,argv);
      exit(0);
    } else if ( arg == "-vox" ) {
      globalopts.mm = false;
      n++;
      continue;
    } else if ( arg == "-mm" ) {
      globalopts.mm = true;
      n++;
      continue;
    } else if ( arg == "-flirt" ) {
      // do nothing anymore - this is all you can ever do!
      n++;
      continue;
    } else if ( arg == "-v" ) {
      globalopts.verbose = 5;
      n++;
      continue;
    }

    if (n+1>=argc) 
      { 
	cerr << "Lacking argument to option " << arg << endl;
	break; 
      }

    // put options with 1 argument here
    if ( arg == "-dest") {
      globalopts.destfname = argv[n+1];
      n+=2;
      continue;
    } else if ( arg == "-src") {
      globalopts.srcfname = argv[n+1];
      n+=2;
      continue;
    } else if ( arg == "-xfm") {
      globalopts.xfmfname = argv[n+1];
      n+=2;
      continue;
    } else { 
      cerr << "Unrecognised option " << arg << endl;
      exit(-1);
    } 

  }  // while (n<argc)

//    if (globalopts.coordfname.size()<1) {
//      cerr << "Input coordinate file not found\n\n";
//      print_usage(argc,argv);
//      exit(2);
//    }
  if ((globalopts.srcfname.size()<1)) {
    cerr << "ERROR:: Source volume filename not found\n\n";
  }
  if ((globalopts.destfname.size()<1)) {
    cerr << "ERROR:: Destination volume filename not found\n\n";
  }
}

////////////////////////////////////////////////////////////////////////////

void print_info(const volume<float>& vol, const string& name) {
  cout << name << ":: SIZE = " << vol.xsize() << " x " << vol.ysize() 
       << " x " << vol.zsize() << endl;
  cout << name << ":: DIMS = " << vol.xdim() << " x " << vol.ydim() 
       << " x " << vol.zdim() << " mm" << endl << endl;
}  

////////////////////////////////////////////////////////////////////////////

int main(int argc,char *argv[])
{
  parse_command_line(argc,argv);


  volume<float> srcvol, destvol;
    // read volumes
  if (read_volume_hdr_only(srcvol,globalopts.srcfname)<0) {
    cerr << "Cannot read Source volume" << endl;
    return -1;
  }
  if (read_volume_hdr_only(destvol,globalopts.destfname)<0) {
    cerr << "Cannot read Destination volume" << endl;
    return -1;
  }
    
  if (globalopts.verbose>3) {
    print_info(destvol,"Destination Volume");
    print_info(srcvol,"Source Volume");
  }

  // read matrices
  Matrix affmat(4,4);
  int returnval;
  returnval = read_matrix(affmat,globalopts.xfmfname,srcvol,destvol);
  if (returnval<0) {
    cerr << "Cannot read transform file" << endl;
    return -2;
  }
    
  if (globalopts.verbose>3) {
    cout << " affmat =" << endl << affmat << endl << endl;
  }


  // Let Volume 2 be Source and Volume 1 be Destination
  //  notate variables as (v=vox, w=world, f=flirt, t=dest)
  
  // the swap matrices are now defunct
  Matrix swapy1(4,4), swapy2(4,4);
  Identity(swapy1);  Identity(swapy2);
  
  Matrix destvox2world, srcvox2world;
  destvox2world = destvol.sampling_mat() * swapy1;
  srcvox2world = srcvol.sampling_mat() * swapy2;
  if (globalopts.verbose>3) {
    cout << " destvox2world =" << endl << destvox2world << endl << endl;
    cout << " srcvox2world =" << endl << srcvox2world << endl;
  }
  
  ColumnVector srccoord(4), destcoord(4), oldsrc(4);
  srccoord = 0;
  destcoord = 0;
  srccoord(4)=1;
  destcoord(4)=1;
  oldsrc = 0;  // 4th component set to 0, so that initially oldsrc -ne srccoord
  
  cout << "Coordinates in Destination volume";
  if (globalopts.mm) { 
    cout << " (in mm) :" << endl;
  } else { 
    cout << " (in voxels) :" << endl; 
  }
  
  if (globalopts.coordfname.size()>1) {
    ifstream matfile(globalopts.coordfname.c_str());
    if (!matfile) { 
      cerr << "Could not open matrix file " << globalopts.coordfname << endl;
      return -1;
    }
    
    while (!matfile.eof()) {
      for (int j=1; j<=3; j++) {
	matfile >> srccoord(j);
      }
      if (globalopts.mm) {  // in mm
	destcoord = destvol.sampling_mat() * destvox2world.i() * affmat * srcvox2world * srcvol.sampling_mat().i() * srccoord;
      } else { // in voxels
	destcoord = destvox2world.i() * affmat * srcvox2world * srccoord; 
      }
      cout << destcoord(1) << "  " << destcoord(2) << "  " << destcoord(3) << endl;
    }
    
    matfile.close();
  } else {
    cout << "Please type in Source coordinates";
    if (globalopts.mm) { 
      cout << " (in mm) :" << endl;
    } else { 
      cout << " (in voxels) :" << endl; 
    }
    while (!cin.eof()) {
      for (int j=1; j<=3; j++) {
	cin >> srccoord(j);
      }
      if (oldsrc == srccoord)  return 0;
      oldsrc = srccoord;
      if (globalopts.mm) {  // in mm
	destcoord = destvol.sampling_mat() * destvox2world.i() * affmat * srcvox2world * srcvol.sampling_mat().i() * srccoord;
      } else { // in voxels
	destcoord = destvox2world.i() * affmat * srcvox2world * srccoord; 
      }
      cout << destcoord(1) << "  " << destcoord(2) << "  " << destcoord(3) << endl;
    }
  }


  return 0;
}









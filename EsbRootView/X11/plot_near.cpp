// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file exlib.license for terms.

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#include "../EsbRootView/read_event"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#include <inlib/args>
#include <inlib/fileis>

#include <inlib/rroot/file>
#include <inlib/rroot/fac>

#include <inlib/rroot/rall>

#include <exlib/zlib>

////////////////////////////////////
////////////////////////////////////
#include <inlib/histo/h2d>
#include <exlib/X11/plotter>
#include <inlib/sg/plotter_style>
#include <exlib/xml/xml_style>
#include <inlib/xml/wrap_viewplot_fonts_microsoft_style> // inlib/xml/viewplot.style file embeded in an inline function.

inline bool load_embedded_styles(inlib::xml::styles& a_styles) {
  std::string ss;
  unsigned int linen;
  const char** lines = viewplot_fonts_microsoft_style(linen);
  for(unsigned int index=0;index<linen;index++) {
    std::string s = lines[index];
    inlib::replace(s,"@@double_quote@@","\"");
    inlib::replace(s,"@@back_slash@@","\\");
    ss += s + "\n";
  }
  return exlib::xml::load_style_string(a_styles,ss);
}

////////////////////////////////////
////////////////////////////////////

#include <cstdlib>
#include <iostream>

int main(int argc,char** argv) {

#ifdef INLIB_MEM
  inlib::mem::set_check_by_class(true);{
#endif

  /////////////////////////////////////////////
  //// arguments : ////////////////////////////
  /////////////////////////////////////////////
  inlib::args args(argc,argv);

  std::string file;
  if(!args.file(file)) {
    std::cout << "give a esb root file. (For example evetest.root)." << std::endl;
    return EXIT_FAILURE;
  }

  bool verbose = args.is_arg("-verbose");

 {bool is;
  inlib::file::is_root(file,is);
  if(!is) {
    std::cout << "file is not a root file." << std::endl;
    return EXIT_FAILURE;
  }}

  unsigned int ievent;
  args.find<unsigned int>("-event",ievent,0);
 
  /////////////////////////////////////////////////
  //// open the data file and read events : ///////
  /////////////////////////////////////////////////
  inlib::rroot::file rfile(std::cout,file,verbose);
  rfile.add_unziper('Z',exlib::decompress_buffer);

  if(args.is_arg("-ls")) {
    const std::vector<inlib::rroot::key*>& keys = rfile.dir().keys();
    inlib::rroot::dump(std::cout,rfile,keys,true,0);
    bool ls = args.is_arg("-ls");
    bool dump = args.is_arg("-dump");
    if(ls||dump) inlib::rroot::read(std::cout,rfile,keys,true,ls,dump,0);
    //if(sinfos) inlib::rroot::read_sinfos(rfile);
  }
 
  std::string stree("cbmsim");
  inlib::rroot::fac fac(std::cout);
  bool map_objs = true;
  //map_objs = false;
  inlib::rroot::tree* tree = inlib::rroot::find_tree(rfile,fac,stree,map_objs);      
  if(!tree) {
    std::cout << "tree " << stree << " not found." << std::endl;
    return EXIT_FAILURE;
  }

 {inlib::uint64 number = tree->entries();
  if(!number) {
    std::cout << "tree " << stree << " empty." << std::endl;
    return EXIT_SUCCESS;
  }
  std::cout << "entries = " << number << "." << std::endl;}

  //tree->dump(std::cout);
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////
  std::vector<EsbMCTrack*> tracks;
  if(!EsbRootView::read_MCTracks(*tree,ievent,tracks,verbose)) {
    std::cout << "can't read EsbMCTracks." << std::endl;
    return EXIT_FAILURE;
  }    
  std::vector<EsbDetectorPoint*> wc_points;
  if(!EsbRootView::read_WCDetectorPoints(*tree,ievent,wc_points,verbose)) {
    std::cout << "can't read EsbWCDetectorPoints." << std::endl;
    return EXIT_FAILURE;
  }
  /*
  std::vector<EsbFgdDetectorPoint*> fgd_points;
  if(!EsbRootView::read_EsbFgdDetectorPoints(*tree,ievent,fgd_points)) {
    std::cout << "can't read EsbFgdDetectorPoints." << std::endl;
    return EXIT_FAILURE;
  }
  */
  //////////////////////////////////////////////////////////
  /// plotting : ///////////////////////////////////////////
  //////////////////////////////////////////////////////////
  // from EsbRoot/display_event_ND.C.
 
  if(verbose) std::cout << "plot ..."<< std::endl;

  exlib::X11::session session(std::cout);
  if(!session.is_valid()) return EXIT_FAILURE;

  int event = 0;
  std::string title;
  inlib::sprintf(title,80,"Event #%d",event);
    
  double rND = 300;
  double lND = 1000;
  inlib::histo::h2d hEvent(title,
                           100, -rND-50, rND + lND + 2*rND + 50,
                           100, -rND*inlib::pi()-50, rND*inlib::pi()+300);

  size_t nPhotons = wc_points.size();

  std::cout << "nPhotons: " << nPhotons << std::endl;
  for(size_t n = 0; n < nPhotons; n++) {
    EsbDetectorPoint* photon = wc_points[n];
    
    double xP = photon->GetX();
    double yP = photon->GetY();
    double zP = photon->GetZ();
    double rP = ::sqrt(xP*xP + yP*yP);
    
    // Find the location of the photon hit
    int location = -1;
    if(::fabs(zP - lND/2.0) < 1) { // front
      location = 1;
    }
    else if(::fabs(zP + lND/2) < 1) { // back
      location = 3;
    }
    else if (::fabs(rP - rND) < 1) { // side
      
      location = 2;
    } else {
    
      std::cout << "Warning: hit was not on detector edge" << std::endl;
      continue;
    }

    // Now we need to find out where to fill the hit
    double x=0, y=0;

    if(location == 3) { // back. Ellipse at left.
      
      x = -xP;
      y =  yP;
    } else if (location == 1) { // front. Ellipse at right.
      
      x = xP + lND + 2*rND;
      y = yP; 
    } else if (location == 2) { // frontside
      
      x = zP + rND + lND/2.0; 
      y = ::atan2(yP, -xP) * rND; 
    }
    
    hEvent.Fill(x, y);
  }

  if(!inlib::is_env("EXLIB_RES_DIR")) {
    std::string path = ".."+inlib::sep()+"comres";
    if(inlib::dir::is_a(path)) inlib::put_env("EXLIB_RES_DIR",path);
  }
  
  exlib::X11::plotter plotter(session,1,1,0,0,650,470);
  if(!plotter.window()) return EXIT_FAILURE;

  inlib::xml::styles styles(std::cout);
  styles.add_colormap("default",inlib::sg::style_default_colormap());
  styles.add_colormap("ROOT",inlib::sg::style_ROOT_colormap());
  load_embedded_styles(styles);
  inlib::sg::style_from_res(styles,"ROOT_default",plotter.plots().current_plotter(),true);

  inlib::sg::plotter& sgp = plotter.plots().current_plotter();
  
  sgp.title_box_style().visible = false;
  sgp.infos_style().visible = false;
  
  sgp.title_to_axis = 0.03;
  sgp.title_style().visible = true;
  sgp.title_style().scale = 3;
  sgp.title_style().font = inlib::sg::font_arialbd_ttf();
  sgp.title_style().font_modeling = inlib::sg::font_pixmap;

  //hEvent->Draw("COLZ");
  sgp.bins_style(0).color = inlib::colorf_red();
  sgp.bins_style(0).modeling = inlib::sg::modeling_solid();
  sgp.bins_style(0).painting = inlib::sg::painting_violet_to_red;
  
  //TEllipse* ellipse = new TEllipse();
  //ellipse->SetLineWidth(2);
  //ellipse->SetFillStyle(0);
  //ellipse->DrawEllipse(0, 0, rND, 0, 0, 360, 0);
  //ellipse->DrawEllipse(rND+lND+rND, 0, rND, 0, 0, 360, 0);

  sgp.add_primitive
    (new inlib::sg::plottable_ellipse(0,0, rND,rND,
                                      inlib::sg::plottable_ellipse::HOLLOW,inlib::colorf_white(),0,0,inlib::colorf_black(),2));
  sgp.add_primitive
    (new inlib::sg::plottable_ellipse(rND+lND+rND,0, rND, rND,
                                      inlib::sg::plottable_ellipse::HOLLOW,inlib::colorf_white(),0,0,inlib::colorf_black(),2));
  //TBox* box = new TBox();
  //box->SetLineWidth(2);
  //box->SetFillStyle(0);
  //box->DrawBox(rND, -rND*TMath::Pi(), rND+lND, rND*TMath::Pi());

  sgp.add_primitive
    (new inlib::sg::plottable_box(rND, -rND*inlib::pi(), rND+lND, rND*inlib::pi(),
                                  inlib::sg::plottable_box::HOLLOW,inlib::colorf_white(),0,0,inlib::colorf_black(),2));
  
  //TLatex* latex = new TLatex();
  //latex->SetTextSize(0.06);
  //latex->SetTextAlign(22);
  //latex->DrawLatex(0          , rND*TMath::Pi()+200, "BACK");
  //latex->DrawLatex(rND+lND/2  , rND*TMath::Pi()+200, "SIDE");
  //latex->DrawLatex(rND+lND+rND, rND*TMath::Pi()+200, "FRONT");

  // latex->SetTextAlign(h*10+v);
  // horizontal :
  //  2    TA_CENTER
  //  3    TA_RIGHT
  //  else TA_LEFT
  // vertical :
  //  1    TA_BASELINE
  //  2    TA_TOP
  //  3    TA_TOP
  //  else TA_BASELINE

  float text_height = (2*float(rND*inlib::pi()))*0.09f;

  sgp.add_primitive
    (new inlib::sg::plottable_text("BACK",0,rND*inlib::pi()+200,
                                   text_height,inlib::sg::text_enforce_height,
                                   0,'C','M',   //22 in ROOT is for horizontal/center and vertical/top.
                                   inlib::sg::font_arialbd_ttf(),inlib::colorf_black(),1,true,true,1,inlib::sg::font_pixmap,false,inlib::colorf_white(),0));
  sgp.add_primitive
    (new inlib::sg::plottable_text("SIDE",rND+lND/2,rND*inlib::pi()+200,
                                   text_height,inlib::sg::text_enforce_height,
                                   0,'C','M',   //22 in ROOT is for horizontal/center and vertical/top.
                                   inlib::sg::font_arialbd_ttf(),inlib::colorf_black(),1,true,true,1,inlib::sg::font_pixmap,false,inlib::colorf_white(),0));
  sgp.add_primitive
    (new inlib::sg::plottable_text("FRONT",rND+lND+rND,rND*inlib::pi()+200,
                                   text_height,inlib::sg::text_enforce_height,
                                   0,'C','M',   //22 in ROOT is for horizontal/center and vertical/top.
                                   inlib::sg::font_arialbd_ttf(),inlib::colorf_black(),1,true,true,1,inlib::sg::font_pixmap,false,inlib::colorf_white(),0));

  sgp.set_axes_font_modeling(inlib::sg::font_pixmap);
  
  plotter.plot(hEvent);

    
  plotter.show();

  //////////////////////////////////////////////////////////
  /// windowing steering : /////////////////////////////////
  //////////////////////////////////////////////////////////
  session.steer();
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////
  inlib::raw_clear(tracks);
  inlib::raw_clear(wc_points);
//inlib::raw_clear(fgd_points);
  
  delete tree;

#ifdef INLIB_MEM
  std::cout << "exit (mem)..." << std::endl;
  }inlib::mem::balance(std::cout);
#else  
  if(verbose) std::cout << "exit..." << std::endl;
#endif

  return EXIT_SUCCESS;
}


  //      if(((*it).second=="pt")&&((*it).first->class_name()=="vector<float>") ) {
  //	pts = inlib::rroot::find_vec<float>(rfile,*((*it).first),(inlib::uint32)ievent);
      
  /*
        cout << "entries " << simtree->GetEntries() << endl;  //1

        TClonesArray *mctrack_arr = new TClonesArray("EsbMCTrack");
        simtree->SetBranchAddress("MCTrack", &mctrack_arr);
        TClonesArray *detpoint_arr = new TClonesArray("EsbWCDetectorPoint");
        simtree->SetBranchAddress("EsbWCDetectorPoint", &detpoint_arr);

        simtree->GetEntry(0);
        cout << mctrack_arr->GetEntries() << endl;  //214495
        cout << detpoint_arr->GetEntries() << endl; //147233

	for(int a = 0; a < mctrack_arr->GetEntries(); ++a) {
		EsbMCTrack *mctrack = (EsbMCTrack*) mctrack_arr->At(a);
		if(mctrack->GetPdgCode() == 50000050) continue; //cherenkov photon
  */

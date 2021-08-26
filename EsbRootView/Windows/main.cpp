// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file EsbRootView.license for terms.

#include "../EsbRootView/main"

#include <exlib/app/Windows/main_cpp>

int main(int argc,char** argv) {return exlib_main<EsbRootView::main>("EsbRootView",argc,argv);}

//exlib_build_use inlib expat
//exlib_build_use exlib png jpeg zlib inlib_glutess freetype
//exlib_build_use WGL Windows kernel
//exlib_build_bigobj


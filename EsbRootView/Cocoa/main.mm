// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file EsbRootView.license for terms.

#include "../EsbRootView/main"

typedef EsbRootView::main app_main_t;

#import <exlib/app/Cocoa/main.mm>

int main(int argc,char** argv) {return exlib_main<EsbRootView::context,EsbRootView::main>("EsbRootView",argc,argv);}
 

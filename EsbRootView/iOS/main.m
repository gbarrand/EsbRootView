// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file EsbRootView.license for terms.

#import <UIKit/UIKit.h>

int main(int argc, char *argv[]) {
  NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
  int retVal = UIApplicationMain(argc, argv, nil, nil);
  [pool release];
  return retVal;
}

/* Copyright (C) 2010, Guy Barrand. All rights reserved. */
/* See the file inlib.license for terms.                 */

#ifndef gl2ps_gldef_h
#define gl2ps_gldef_h

/* OpenGL typedefs and cpp macros used in gl2ps. */

typedef unsigned int   GLenum;
typedef unsigned int   GLuint;
typedef float          GLfloat;
typedef int            GLsizei;
typedef int            GLint;
typedef unsigned char  GLboolean;
typedef short          GLshort;
typedef unsigned short GLushort;

#define GL_TRUE                           1
#define GL_FALSE                          0

#define GL_FLOAT				0x1406
#define GL_BLEND				0x0BE2

#define GL_SRC_ALPHA				0x0302
#define GL_ONE_MINUS_SRC_ALPHA                  0x0303

#define GL_RGB					0x1907
#define GL_RGBA                                 0x1908

#define GL_POINTS                               0x0000

#define GL_CURRENT_RASTER_POSITION_VALID	0x0B08
#define GL_CURRENT_RASTER_POSITION		0x0B07
#define GL_CURRENT_RASTER_COLOR			0x0B04
#define GL_ZERO					0
#define GL_ONE					1
#define GL_COLOR_INDEX				0x1900

#define GL_POINT_TOKEN				0x0701
#define GL_LINE_TOKEN				0x0702
#define GL_LINE_RESET_TOKEN			0x0707
#define GL_POLYGON_TOKEN			0x0703
#define GL_BITMAP_TOKEN				0x0704
#define GL_DRAW_PIXEL_TOKEN			0x0705

#define GL_COPY_PIXEL_TOKEN			0x0706
#define GL_PASS_THROUGH_TOKEN			0x0700

#define GL_FEEDBACK				0x1C01
#define GL_COLOR_CLEAR_VALUE			0x0C22
#define GL_INDEX_CLEAR_VALUE			0x0C20
#define GL_RENDER				0x1C00
#define GL_VIEWPORT				0x0BA2
#define GL_BLEND_SRC				0x0BE1
#define GL_BLEND_DST				0x0BE0
#define GL_3D_COLOR				0x0602

#define GL_POLYGON_OFFSET_FACTOR		0x8038
#define GL_POLYGON_OFFSET_UNITS			0x2A00
#define GL_LINE_STIPPLE_PATTERN			0x0B25
#define GL_LINE_STIPPLE_REPEAT			0x0B26

#endif




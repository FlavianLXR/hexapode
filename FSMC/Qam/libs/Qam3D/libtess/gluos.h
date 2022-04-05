/*
** gluos.h - operating system dependencies for GLU
**
** $Header: //depot/main/gfx/lib/glu/include/gluos.h#4 $
*/

// QTGLAM
#ifndef __gluos_h_
#define __gluos_h_
//

//#ifdef _WIN32

//#define WIN32_LEAN_AND_MEAN
//#define NOGDI
//#define NOIME
//#include <windows.h>

// /* Disable warnings */
//#pragma warning(disable : 4101)
//#pragma warning(disable : 4244)
//#pragma warning(disable : 4761)

//#else

// /* Disable Microsoft-specific keywords */
//#define GLAPI
//#define WINGDIAPI

//#endif

// QTGLAM

typedef unsigned int GLenum ;
typedef unsigned char GLboolean ;
typedef float GLfloat ;
//typedef GLfloat GLdouble ;
typedef double GLdouble ;

/* BeginMode */
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006

/* ErrorCode */
#define GLU_INVALID_ENUM                     100900
#define GLU_INVALID_VALUE                    100901
#define GLU_OUT_OF_MEMORY                    100902
#define GLU_INCOMPATIBLE_GL_VERSION          100903
#define GLU_INVALID_OPERATION                100904

/* TessCallback */
#define GLU_TESS_BEGIN                       100100
#define GLU_BEGIN                            100100
#define GLU_TESS_VERTEX                      100101
#define GLU_VERTEX                           100101
#define GLU_TESS_END                         100102
#define GLU_END                              100102
#define GLU_TESS_ERROR                       100103
#define GLU_TESS_EDGE_FLAG                   100104
#define GLU_EDGE_FLAG                        100104
#define GLU_TESS_COMBINE                     100105
#define GLU_TESS_BEGIN_DATA                  100106
#define GLU_TESS_VERTEX_DATA                 100107
#define GLU_TESS_END_DATA                    100108
#define GLU_TESS_ERROR_DATA                  100109
#define GLU_TESS_EDGE_FLAG_DATA              100110
#define GLU_TESS_COMBINE_DATA                100111

/* TessContour */
#define GLU_CW                               100120
#define GLU_CCW                              100121
#define GLU_INTERIOR                         100122
#define GLU_EXTERIOR                         100123
#define GLU_UNKNOWN                          100124

/* TessProperty */
#define GLU_TESS_WINDING_RULE                100140
#define GLU_TESS_BOUNDARY_ONLY               100141
#define GLU_TESS_TOLERANCE                   100142

/* TessError */
#define GLU_TESS_ERROR1                      100151
#define GLU_TESS_ERROR2                      100152
#define GLU_TESS_ERROR3                      100153
#define GLU_TESS_ERROR4                      100154
#define GLU_TESS_ERROR5                      100155
#define GLU_TESS_ERROR6                      100156
#define GLU_TESS_ERROR7                      100157
#define GLU_TESS_ERROR8                      100158
#define GLU_TESS_MISSING_BEGIN_POLYGON       100151
#define GLU_TESS_MISSING_BEGIN_CONTOUR       100152
#define GLU_TESS_MISSING_END_POLYGON         100153
#define GLU_TESS_MISSING_END_CONTOUR         100154
#define GLU_TESS_COORD_TOO_LARGE             100155
#define GLU_TESS_NEED_COMBINE_CALLBACK       100156

/* TessWinding */
#define GLU_TESS_WINDING_ODD                 100130
#define GLU_TESS_WINDING_NONZERO             100131
#define GLU_TESS_WINDING_POSITIVE            100132
#define GLU_TESS_WINDING_NEGATIVE            100133
#define GLU_TESS_WINDING_ABS_GEQ_TWO         100134

#define GLU_TESS_MAX_COORD					1.0e150

#endif // __gluos_h_
//

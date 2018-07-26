// //////////////////////////////////////////////////////////////////////////////////////////
//  Module Interface  parent interface head file IUnknownEx.h
//  Other all interface parent interface
//  Author : deirlym
//  time:   2008-08-13
//  Version: 2.0
// ////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DEIRLYM_WIN_DATA_STRUCT_HEAD_FILE__
#define __DEIRLYM_WIN_DATA_STRUCT_HEAD_FILE__

#ifndef WIN32

typedef DWORD   COLORREF;
typedef DWORD   *LPCOLORREF;

#define HFILE_ERROR ((HFILE)-1)

typedef struct tagRECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} RECT, *PRECT;

typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT, *PPOINT;

typedef struct tagSIZE
{
    LONG        cx;
    LONG        cy;
} SIZE, *PSIZE, *LPSIZE;

typedef SIZE               SIZEL;
typedef SIZE               *PSIZEL, *LPSIZEL;


typedef struct tagPOINTS
{
#ifndef _MAC
    SHORT   x;
    SHORT   y;
#else
    SHORT   y;
    SHORT   x;
#endif
} POINTS, *PPOINTS, *LPPOINTS;

#define RGB(r,g,b)          (((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16))))

#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))

#endif  //WIN32

#endif //__DEIRLYM_WIN_DATA_STRUCT_HEAD_FILE__

#include "main.h"

/*0x76666667*/
unsigned long __cdecl WWKSX6A(void)
{
    /* Application handles WM_LBUTTONDBLCLK message. 
    *  If mouse coordinates fit the rectangle containing
    *  ESET logo, this function will be called.
    *  0x14, 0x14 +--------+
    *             |  ESET  |
    *             +--------+ 0x55, 0x2D
    *  The return value of this function must be less than or equal 
    *  to GetTickCount()+10. Obviously, GetTickCount() must be returned.
    */
    return GetTickCount();
}

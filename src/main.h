#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>

/* Stage 2. Export name CRC32 = 0xDEADCODE */
__declspec(dllexport) void __stdcall Z1IBF(unsigned short *, unsigned int);

/* Stage 3. Export name CRC32 = 0xDEADBEEF */
__declspec(dllexport) void __stdcall WPPD5FA(char *, unsigned int);

/* Stage 4. Export name CRC32 = 0xDEADCAFE */
__declspec(dllexport) void __stdcall IH5PME0(char *, unsigned int, unsigned int);

/* Stage 5. Export name is time-based */
__declspec(dllexport) int __stdcall FFFFFFFFFFFFFFFF(void);
__declspec(dllexport) int __stdcall DllMain(HINSTANCE, DWORD, LPVOID);

/* Stage hidden. Export name CRC32 = 0x76666667 */
__declspec(dllexport) unsigned long __cdecl WWKSX6A(void);

#endif

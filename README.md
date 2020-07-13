## Preface

The application contains 5 stages related to checkboxes and 1 hidden stage. The solution is the dll file exporting specific functions.

This document describes the solution. Each stage is described in similar style and contains information about how it can be completed: what kind of object is searched by the application (can be Function by name CRC32, Function by name or File by name CRC32), its CRC32 value, samples of object's name that have correct CRC32 values and some additional details.

## Stage 1

The application will search for a dynamic library nearby. The CRC32 of the name of the library must be `0x35370000`, thus the library name may be like `FMJ8FR`, `5Q0NUB`, `BIW9G1`.  The application must be able to load that library with `LdrLoadDll()` call to pass Stage 1.

## Stage 2

Within the library loaded at Stage 1, the application will try to locate a function with certain name - the CRC32 of the name of that function must be  `0xDEADC0DE`, i.e. `Z1IBF`, `SIFDGA0`, `LHBQVM9`.
The application will call that function, passing a pointer to an array of 2-byte integers and the size of that array. The function must sort that array in ascending order to pass the Stage 2.

## Stage 3

The application will try to locate a function with certain name - the CRC32 of the name of that function must be `0xDEADBEEF`, i.e. `WPPD5FA`, `C6XSAM5`, `W84WOU`.
The application will call that function, passing a `char *` and an `int` arguments to it. The application will expect that the function returns the encrypted name of the current user, and then decrypt it using RSA algorithm using private key pair `{d,n}` = `{0x10001, 0xE2D797F9}`.

The function must do encryption using public key pair `{e,n}` = `{0x4296E23D, 0xE2D797F9}` to pass Stage 3.

## Stage 4

The application will try to locate a function with certain name - the CRC32 of the name of that function must be `0xDEADCAFE`, i.e. `IH5PME0`, `T8TKU11`, `RL0J`.
The function must compute coefficients (A, B, C, D, E) for 5th degree equation `x^5 - Ax^4 + Bx^3 - Cx^2 + Dx - E = 0`. The application calls GetVolumeInformationW() to obtain the serial number of the currentvolume. The serial number is passed as argument to calculate the roots of the equation as 5 lowest bytes of the product `(CRC32(GetUserNameW()) * volume_serial)`.
Coefficients can be computed with Vieta's formulas and must be stored to dest buffer as 10-characters long hexadecimal strings (no more than n characters to be stored).

## Stage 5

The application will try to locate a function with certain name - based on current date, it's format is `YYYYDDDDHHHHMMMM`.

Function must return `0x35371337` to pass Stage 5.

## Hidden Stage

Only reachable if all five Stages has completed.

The application will try to locate a function with certain name - the CRC32 of the name of that function must be `0x76666667`, i.e. `WWKSX6A`, `HG3AWPB`, `SSVRYUA`.

The application handles WM_LBUTTONDBLCLK message so that if mouse coordinates fit the rectangle {{0x14, 0x14}, {0x55, 0x2D}} containing ESET logo, this function will be called. The function must return the `GetTickCount()` result. Double-click the ESET logo and the flames of the main window will change color to yellow.

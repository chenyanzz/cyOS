#pragma once


///向上取整
#define ceil(x) (int)((x) == (int)(x) ? (x) : (x + 1))

///向下取整
#define floor(x) (int)(x)

///较大者
#define max(a,b) ((a)>(b)?(a):(b))

///较小者
#define min(a,b) ((a)<(b)?(a):(b))


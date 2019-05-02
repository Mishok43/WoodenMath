# WoodenMath
WoodenMath - math library which contains vectors, quaternions, matrix and other mathematical constuctions and operations. 
It supports SSE SIMD operations - SSE, AVX-2. C++17 with meta-programming. Usings GTest library for unit-testing components.

<b>Implemented</b>:
1. DVector with support up to 4 elements and float, double, int32, uint32 types. Supporting SSE, AVX-2
2. DMatrix4x4 with support: double, int32, uint32 types. Supporting SSE, AVX-2 and data batching for 32-bit types (float, int)

<b>Not implemented yet</b>:
1. DQuaternions
2. Lerping functions (spherical, linear)
3. Cache-friendly big matrix multiplication

<b>Auto-testing</b>:
1. DVector
2. DMatrix

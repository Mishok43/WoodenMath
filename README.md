# WoodenMath
WoodenMath - math library which contains vectors, quaternions, matrix and other mathematical constuctions and operations. 
It supports SSE SIMD operations - SSE, AVX-2. C++17 with meta-programming. Usings GTest library for unit-testing components.

<b>Implemented</b>:
1. DVector with support up to 4 elements and float, double, int32, uint32 types. 
2. DVector supports SSE, AVX-2

<b>Not implemented yet</b>:
1. DMatrix 
2. Batching data for using 256 bit and 512 bit SSE operations
3. DQuaternions
4. Lerping functions (spherical, linear)
5. Cache-friendly big matrix multiplication


<b>Auto-testing<b>:
1. DVector

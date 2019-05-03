# WoodenMath
WoodenMath - math library which contains vectors, quaternions, matrix and other mathematical constuctions and operations. 
It supports SSE SIMD operations - SSE, AVX-2. C++17 with meta-programming. Usings GTest library for unit-testing components.

<b>Implemented</b>:
1. DVector with support up to 4 elements and float, double, int32, uint32 types. Supporting SSE, AVX-2
2. DMatrix4x4 with support: double, int32, uint32 types. Supporting SSE, AVX-2 and data batching for 32-bit types (float, int)
3. DQuaternions with support: float, double types. Supporting SSE, AVX-2 operations for 32-bit and 64-bit types without batching.
4. Lerping functions (spherical, linear)

<b>Not implemented yet</b>:
1. Cache-friendly big matrix multiplication

<b>Unit-testing</b>:
1. DVector
2. DMatrix
3. DQuaternion

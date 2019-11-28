# WoodenMath
WoodenMath - math library which contains vectors, quaternions, matrix and other mathematical constuctions and operations. 
It supports SSE SIMD operations - SSE, AVX-2. C++17 with meta-programming. Usings GTest library for unit-testing components.

<b>Implemented</b>:
1. DVector with support up to 8 elements and float, double, int32, uint32 types. Supporting SSE, AVX-2
2. DMatrix4x4 with support: double, int32, uint32 types. Supporting SSE, AVX-2 and data batching for 32-bit types (float, int)
3. DQuaternions with support: float, double types. Supporting SSE, AVX-2 operations for 32-bit and 64-bit types without batching.
4. DRays, DBounds
5. Distrubitions
6. Uniform sampling of disk, cone and etc
7. DTransform = Matrix + Inverse Matrix. DAnimatedTransform - transform between two transformation based on a normalized t
8. Lerping functions (spherical, linear)

<b>Not implemented yet</b>:
1. Cache-friendly big matrix multiplication
2. Computational algorithms
3. Structures for computational geometry 
4. Benchmarks

<b>Unit-testing</b>:
1. DVector
2. DMatrix
3. DQuaternion


<h1> Examples </h1>
<h2> Vectors </h2>

<h3> Initialization </h3>

<code>
  DVector<float, 12> v1(1.0);   
	float r[12] = { 2.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 4.0, 5.0 };   
	DVector<float, 12> v2(r);   
</code>

<h3> Addition </h3>

<code>
	DVector<float, 12> v3 = v1 + v2;   
</code>
  
<h3> Dot </h3>

<code>
	DVector<float, 26> v1(1.0f);   
  DVector<float, 26> v2(2.0f);   
  DVector<float, 26> v3 = v1+v2;   
</code>

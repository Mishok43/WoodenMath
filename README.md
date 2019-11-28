# WoodenMath
WoodenMath - math library which contains vectors, quaternions, matrix and other mathematical constuctions and operations. 
It supports SSE SIMD operations - SSE, AVX-2. C++17 with meta-programming. Usings GTest library for unit-testing components.
Used as the main math library in [pbr raytracer](https://github.com/Mishok43/WoodenPBREngine)

<b>Implemented</b>:
1. DVector with support: float, double, int32, uint32 types. Supporting SSE, AVX-2, and their combination: several registers of AVX2 and several registers of SSE in one vector!
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

C++:   

```
  DVector<float, 12> v1(1.0);   
  float r[12] = { 2.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 4.0, 5.0 };   
  DVector<float, 12> v2(r);   
```   
Assembler:   

```
00007FF6F52FDBA9  vmovups     ymm0,ymmword ptr [__ymm@3f8000003f8000003f8000003f8000003f8000003f8000003f8000003f800000 
00007FF6F52FDBB1  vmovups     xmm1,xmmword ptr [__xmm@3f8000003f8000003f8000003f800000 (07FF6F531B710h)]  
00007FF6F52FDBB9  vmovups     ymmword ptr [rbp],ymm0  
00007FF6F52FDBBE  vmovups     xmmword ptr [rbp+20h],xmm1 
```   

<i>(MSVC [generate](https://developercommunity.visualstudio.com/content/problem/19160/regression-from-vs-2015-in-ssseavx-instructions-ge.html) vmovups for an aligned memory data, too)</i>   

<h3> Addition </h3>

```
  DVector<float, 12> v3 = v1 + v2;   
```
```
00007FF71765DCB6  vaddps      xmm2,xmm3,xmm0  
00007FF71765DCBA  vaddps      ymm1,ymm4,ymm4 
```
<h3> Dot </h3>

```
  DVector<float, 19> v1(1.0f);   
  DVector<float, 19> v2(2.0f);   
  DVector<float, 19> v3 = v1+v2;   
```
```
00007FF79062DD74  vmovups     ymm2,ymmword ptr [rbp+0C0h]  
00007FF79062DD7C  vmulps      ymm0,ymm4,ymmword ptr [r8]  
00007FF79062DD81  vmulps      xmm7,xmm2,xmmword ptr [r8+40h]  
00007FF79062DD87  vmulps      ymm1,ymm5,ymmword ptr [r8+20h]  
00007FF79062DD8D  vmovups     ymmword ptr [rbp+60h],ymm2  
00007FF79062DD92  vmovups     ymmword ptr [rbp+20h],ymm0  
00007FF79062DD97  vmovups     xmmword ptr [rbp+60h],xmm7  
00007FF79062DD9C  vmovups     ymmword ptr [rbp+40h],ymm1  
00007FF79062DDA1  vxorps      xmm0,xmm0,xmm0  
00007FF79062DDA5  lea         rax,[rbp+20h]  
00007FF79062DDA9  mov         ecx,2  
00007FF79062DDAE  xchg        ax,ax  
00007FF79062DDB0  vaddps      ymm0,ymm0,ymmword ptr [rax]  
00007FF79062DDB4  lea         rax,[rax+20h]  
00007FF79062DDB8  sub         rcx,1  
00007FF79062DDBC  jne         main+230h (07FF79062DDB0h)  
00007FF79062DDBE  mov         rax,qword ptr gs:[58h]  
00007FF79062DDC7  vhaddps     ymm0,ymm0,ymm0  
00007FF79062DDCB  vextractf128 xmm1,ymm0,1  
00007FF79062DDD1  vextractf128 xmm0,ymm0,0  
00007FF79062DDD7  mov         rcx,qword ptr [rax]  
00007FF79062DDDA  vaddps      xmm2,xmm1,xmm0  
00007FF79062DDDE  mov         edx,4  
00007FF79062DDE3  vextractps  dword ptr [rbp],xmm2,1  
00007FF79062DDEA  vmovss      xmm0,dword ptr [rbp]  
00007FF79062DDEF  vextractps  dword ptr [rbp+4],xmm2,0  
00007FF79062DDF6  mov         eax,dword ptr [rdx+rcx]  
00007FF79062DDF9  cmp         dword ptr [TSS0<`template-parameter-7',wml::SIMDLines<float,16,3,1,4,__m128>::$02::prod, ?? :: ?? ::HA::wml::MXZ const * __ptr64 const> (07FF790658E00h)],eax  
00007FF79062DDFF  vaddss      xmm8,xmm0,dword ptr [rbp+4]  
00007FF79062DE04  vxorps      xmm6,xmm6,xmm6  
00007FF79062DE08  jle         main+2B6h (07FF79062DE36h)  
00007FF79062DE0A  lea         rcx,[TSS0<`template-parameter-7',wml::SIMDLines<float,16,3,1,4,__m128>::$02::prod, ?? :: ?? ::HA::wml::MXZ const * __ptr64 const> (07FF790658E00h)]  
00007FF79062DE11  vzeroupper  
00007FF79062DE19  cmp         dword ptr [TSS0<`template-parameter-7',wml::SIMDLines<float,16,3,1,4,__m128>::$02::prod, ?? :: ?? ::HA::wml::MXZ const * __ptr64 const> (07FF790658E00h)],0FFFFFFFFh  
00007FF79062DE20  jne         main+2B6h (07FF79062DE36h)  
00007FF79062DE22  lea         rcx,[TSS0<`template-parameter-7',wml::SIMDLines<float,16,3,1,4,__m128>::$02::prod, ?? :: ?? ::HA::wml::MXZ const * __ptr64 const> (07FF790658E00h)]  
00007FF79062DE29  vmovups     xmmword ptr [`wml::SIMDLines<float,16,3,1,4,__m128>::inprod<3>'::`7'::zero (07FF790658E10h)],xmm6  
00007FF79062DE36  vmovups     xmm0,xmmword ptr [`wml::SIMDLines<float,16,3,1,4,__m128>::inprod<3>'::`7'::zero (07FF790658E10h)]  
00007FF79062DE45  vblendps    xmm1,xmm0,xmm7,7  
00007FF79062DE4B  vhaddps     xmm0,xmm1,xmm1  
00007FF79062DE4F  vextractps  dword ptr [rbp],xmm0,0  
00007FF79062DE56  vextractps  dword ptr [rbp+4],xmm0,1  
00007FF79062DE5D  vmovss      xmm0,dword ptr [rbp+4]  
00007FF79062DE62  vaddss      xmm1,xmm0,dword ptr [rbp]  
00007FF79062DE67  vaddss      xmm1,xmm1,xmm8  
```
<h2> Transformations </h2>

```
DTransformf world = DTransformf::makeScale(1.0f, 5.0f, 4.0f)*
					DTransformf::makeRotateX(2.5)*
					DTransformf::makeTranslate(1.0f, 0.0f, 10.0f);
DPoint3f pos(5.0f, 2.5f, 5.4);
DPoint3f posW = world(pos);
DPoint3f posL = world(pos, INV_TRANFORM);
```

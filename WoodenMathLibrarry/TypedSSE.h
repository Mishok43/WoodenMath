#pragma once

#include <immintrin.h>
#include <smmintrin.h>
#include "stdafx.h"

namespace TypedSSE
{

	template<typename T>
	struct sse_alignment_size
	{
	};

	template<>
	struct sse_alignment_size<__m128>
	{
		static constexpr uint8_t size = 16;
	};

	template<>
	struct sse_alignment_size<__m128i>
	{
		static constexpr uint8_t size = 16;
	};


	template<>
	struct sse_alignment_size<__m256>
	{
		static constexpr uint8_t size = 32;
	};

	template<>
	struct sse_alignment_size<__m256i>
	{
		static constexpr uint8_t size = 32;
	};

	template<>
	struct sse_alignment_size<__m256d>
	{
		static constexpr uint8_t size = 32;
	};


	template<typename T>
	constexpr uint8_t sse_alignment_size_v = sse_alignment_size<T>::size;

	template<typename T, uint8_t factor>
	struct __m_type
	{
	};

	template<>
	struct __m_type<double, 16>
	{
		using type = typename __m128d;
	};

	template<>
	struct __m_type<float, 16>
	{
		using type = typename __m128;
	};

	template<>
	struct __m_type<float, 32>
	{
		using type = typename __m256;
	};

	template<>
	struct __m_type<double, 32>
	{
		using type = typename __m256d;
	};

	template<>
	struct __m_type<int32_t, 16>
	{
		using type = typename __m128i;
	};

	template<>
	struct __m_type<int32_t, 32>
	{
		using type = typename __m256i;
	};

	template<>
	struct __m_type<uint32_t, 16>
	{
		using type = typename __m128i;
	};

	template<>
	struct __m_type<uint32_t, 32>
	{
		using type = typename __m256i;
	};

	template<typename T, uint8_t width>
	using __m_t = typename __m_type<T, width>::type;

	template<typename T>
	struct __m256_type
	{
	};

	template<>
	struct __m256_type<float>
	{
		using type = typename __m256;
	};

	template<>
	struct __m256_type<double>
	{
		using type = typename __m256d;
	};

	template<>
	struct __m256_type<int32_t>
	{
		using type = typename __m256i;
	};

	template<>
	struct __m256_type<uint32_t>
	{
		using type = typename __m256i;
	};

	template<typename T>
	using __m256_t = typename __m256_type<T>::type;


	template<typename T>
	struct __m128_type
	{
	};

	template<>
	struct __m128_type<float>
	{
		using type = typename __m128;
	};

	template<>
	struct __m128_type<double>
	{
		using type = typename __m128d;
	};

	template<>
	struct __m128_type<int32_t>
	{
		using type = typename __m128i;
	};

	template<>
	struct __m128_type<uint32_t>
	{
		using type = typename __m128i;
	};

	template<typename T>
	using __m128_t = typename __m128_type<T>::type;


	template<typename __mT>
	struct __mT_type_extract
	{
	};

	template<>
	struct __mT_type_extract<__m128>
	{
		using type = typename float;
	};

	template<>
	struct __mT_type_extract<__m128d>
	{
		using type = typename double;
	};

	template<>
	struct __mT_type_extract<__m128i>
	{
		using type = typename uint32_t;
	};


	template<>
	struct __mT_type_extract<__m256>
	{
		using type = typename float;
	};

	template<>
	struct __mT_type_extract<__m256d>
	{
		using type = typename double;
	};

	template<>
	struct __mT_type_extract<__m256i>
	{
		using type = typename int32_t;
	};

	
	template<typename T>
	using __mT_type_extract_t = typename __mT_type_extract<T>::type;





	// _MM_LOADU_T
	template<typename __mT,  typename T>
	inline auto _mm_loadu_t(const T* data)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_loadu_t<__m128, float>(const float* data)
	{
		return _mm_loadu_ps(data);
	}

	template<>
	inline auto _mm_loadu_t<__m128i, int32_t>(const int32_t* data)
	{
		return _mm_loadu_si128((__m128i*)data);
	}

	template<>
	inline auto _mm_loadu_t<__m128i, uint32_t>(const uint32_t* data)
	{
		return _mm_loadu_si128((__m128i*)data);
	}

	template<>
	inline auto _mm_loadu_t<__m256, float>(const float* data)
	{
		return _mm256_loadu_ps(data);
	}

	template<>
	inline auto _mm_loadu_t<__m256i, int32_t>(const int32_t* data)
	{
		return _mm256_loadu_si256((__m256i*)data);
	}

	template<>
	inline auto _mm_loadu_t<__m256i, uint32_t>(const uint32_t* data)
	{
		return _mm256_loadu_si256((__m256i*)data);
	}

	template<>
	inline auto _mm_loadu_t<__m256d, double>(const double* data)
	{
		return _mm256_loadu_pd(data);
	}




	// _MM_loada_T
	template<typename __mT, typename T>
	inline auto _mm_loada_t(const T* data)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_loada_t<__m128, float>(const float* data)
	{
		return _mm_load_ps(data);
	}

	template<>
	inline auto _mm_loada_t<__m128i, int32_t>(const int32_t* data)
	{
		return _mm_load_si128((__m128i*)data);
	}

	template<>
	inline auto _mm_loada_t<__m128i, uint32_t>(const uint32_t* data)
	{
		return _mm_load_si128((__m128i*)data);
	}

	template<>
	inline auto _mm_loada_t<__m256, float>(const float* data)
	{
		return _mm256_load_ps(data);
	}

	template<>
	inline auto _mm_loada_t<__m256i, int32_t>(const int32_t* data)
	{
		return _mm256_load_si256((__m256i*)data);
	}

	template<>
	inline auto _mm_loada_t<__m256i, uint32_t>(const uint32_t* data)
	{
		return _mm256_load_si256((__m256i*)data);
	}

	template<>
	inline auto _mm_loada_t<__m256d, double>(const double* data)
	{
		return _mm256_load_pd(data);
	}


	// _MM_SET1_T
	template<typename __mT, typename T>
	inline __mT _mm_set1_t(T data)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline __m256d _mm_set1_t<__m256d, double>(double scalar)
	{
		return _mm256_set1_pd(scalar);
	}

	template<>
	inline __m128 _mm_set1_t<__m128, float>(float scalar)
	{
		return _mm_set1_ps(scalar);
	}


	template<>
	inline __m128i _mm_set1_t<__m128i, int32_t>(int32_t scalar)
	{
		return _mm_set1_epi32(scalar);
	}

	template<>
	inline __m128i _mm_set1_t<__m128i, uint32_t>(uint32_t scalar)
	{
		return _mm_set1_epi32(scalar);
	}

	template<>
	inline __m256 _mm_set1_t<__m256, float>(float scalar)
	{
		return _mm256_set1_ps(scalar);
	}

	template<>
	inline __m256i _mm_set1_t<__m256i, int32_t>(int32_t scalar)
	{
		return _mm256_set1_epi32(scalar);
	}

	template<>
	inline __m256i _mm_set1_t<__m256i, uint32_t>(uint32_t scalar)
	{
		return _mm256_set1_epi32(scalar);
	}

	template<typename T>
	inline auto _mm_setr_t(T m0, T m1, T m2, T m3)
	{
		static_assert(false, "The type is not supported");
	}

	template<>
	inline auto _mm_setr_t<float>(float m0, float m1, float m2, float m3)
	{
		return _mm_setr_ps(m0, m1, m2, m3);
	}

	template<>
	inline auto _mm_setr_t<double>(double m0, double m1, double m2, double m3)
	{
		return _mm256_setr_pd(m0, m1, m2, m3);
	}

	template<>
	inline auto _mm_setr_t<int32_t>(int32_t m0, int32_t m1, int32_t m2, int32_t m3)
	{
		return _mm_setr_epi32(m0, m1, m2, m3);
	}

	template<>
	inline auto _mm_setr_t<uint32_t>(uint32_t m0, uint32_t m1, uint32_t m2, uint32_t m3)
	{
		return _mm_setr_epi32(m0, m1, m2, m3);
	}

	template<typename T>
	inline auto _mm_setr_t(T m0, T m1, T m2, T m3, 
						   T m4, T m5, T m6, T m7)
	{
		static_assert(false, "The type is not supported");
	}

	template<>
	inline auto _mm_setr_t<float>(float m0, float m1, float m2, float m3,
								  float m4, float m5, float m6, float m7)
	{
		return _mm256_setr_ps(m0, m1, m2, m3, m4, m5, m6, m7);
	}

	template<>
	inline auto _mm_setr_t<int32_t>(int32_t m0, int32_t m1, int32_t m2, int32_t m3,
									int32_t m4, int32_t m5, int32_t m6, int32_t m7)
	{
		return _mm256_setr_epi32(m0, m1, m2, m3, m4, m5, m6, m7);
	}

	template<>
	inline auto _mm_setr_t<uint32_t>(uint32_t m0, uint32_t m1, uint32_t m2, uint32_t m3,
									 uint32_t m4, uint32_t m5, uint32_t m6, uint32_t m7)
	{
		return _mm256_setr_epi32(m0, m1, m2, m3, m4, m5, m6, m7);
	}



	// _MM_STORE_T
	template<typename T, typename __mT>
	inline auto _mm_storea_t(T* data, __mT vec)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_storea_t<float, __m128>(float* data, __m128 vec)
	{
		return _mm_store_ps(data, vec);
	}

	template<>
	inline auto _mm_storea_t<float, __m256>(float* data, __m256 vec)
	{
		return _mm256_store_ps(data, vec);
	}

	template<>
	inline auto _mm_storea_t<double, __m256d>(double* data, __m256d vec)
	{
		return _mm256_store_pd(data, vec);
	}

	template<>
	inline auto _mm_storea_t<int32_t, __m128i>(int32_t* data, __m128i vc)
	{
		return _mm_store_si128((__m128i*)data, vc);
	}

	template<>
	inline auto _mm_storea_t<int32_t, __m256i>(int32_t* data, __m256i vc)
	{
		return _mm256_store_si256((__m256i*)data, vc);
	}

	template<>
	inline auto _mm_storea_t<uint32_t, __m128i>(uint32_t* data, __m128i vc)
	{
		return _mm_store_si128((__m128i*)data, vc);
	}

	template<>
	inline auto _mm_storea_t<uint32_t, __m256i>(uint32_t* data, __m256i vc)
	{
		return _mm256_store_si256((__m256i*)data, vc);
	}

	template<typename T>
	inline auto _mm256_broadcast_t(const T* data)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm256_broadcast_t<float>(const float* data)
	{
		return _mm256_broadcast_ps((__m128 const*)data);
	}

	template<>
	inline auto _mm256_broadcast_t<double>(const double* data)
	{ 
		return _mm256_loadu_pd(data);
	}

	template<>
	inline auto _mm256_broadcast_t<int32_t>(const int32_t* data)
	{
		__m128i xmm = _mm_loadu_si128((__m128i*)data);
		return _mm256_broadcastsi128_si256(xmm);
	}

	template<>
	inline auto _mm256_broadcast_t<uint32_t>(const uint32_t* data)
	{
		__m128i xmm = _mm_loadu_si128((__m128i*)data);
		return _mm256_broadcastsi128_si256(xmm);
	}


	// _MM_STORE_T
	template<typename T, typename __mT = __m_t<T>>
	inline auto _mm_storeu_t(T* data, __mT vec)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_storeu_t<float>(float* data, __m128 vec)
	{
		return _mm_storeu_ps(data, vec);
	}

	template<>
	inline auto _mm_storeu_t<double>(double* data, __m256d vec)
	{
		return _mm256_storeu_pd(data, vec);
	}

	template<>
	inline auto _mm_storeu_t<int32_t>(int32_t* data, __m128i vc)
	{
		return _mm_storeu_si128((__m128i*)data, vc);
	}

	template<>
	inline auto _mm_storeu_t<uint32_t>(uint32_t* data, __m128i vc)
	{
		return _mm_storeu_si128((__m128i*)data, vc);
	}



	template<uint8_t mask = 0b11110001>
	struct _mm_dp_ts
	{
		template<typename T, typename __mT>
		inline auto f(__mT a, __mT b)
		{
			static_assert(false, "The type is not supported");
			return;
		}

		template<> inline auto f<__m128>(__m128 a, __m128 b)
		{
			return _mm_dp_ps(a, b, mask);
		}

		template<> inline auto f<__m128d>(__m128d a, __m128d b)
		{
			return _mm_dp_pd(a, b, mask);
		}

		template<> inline auto f<__m256>(__m256 a, __m256 b)
		{
			auto k = _mm256_dp_ps(a, b);
			return _mm256_hadd_ps(k, k);
		}
	};

	template<uint8_t N>
	inline constexpr unsigned mask_gen()
	{
		static_assert(false);
		return;
	}

	template<> inline constexpr unsigned mask_gen<0>(){return 0b00000000;}
	template<> inline constexpr unsigned mask_gen<1>(){return 0b00000001;}
	template<> inline constexpr unsigned mask_gen<2>(){return 0b00000011;}
	template<> inline constexpr unsigned mask_gen<3>(){return 0b00000111;}
	template<> inline constexpr unsigned mask_gen<4>(){return 0b00001111;}
	template<> inline constexpr unsigned mask_gen<5>(){return 0b00011111;}
	template<> inline constexpr unsigned mask_gen<6>(){return 0b00111111;}
	template<> inline constexpr unsigned mask_gen<7>(){return 0b01111111;}
	template<> inline constexpr unsigned mask_gen<8>(){return 0b11111111;}
	
	// _MM_ADD_T
	template<typename __mT>
	inline auto _mm_add_t(__mT a, __mT b)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_add_t<__m128>(__m128 a, __m128 b)
	{
		return _mm_add_ps(a, b);
	}

	template<>
	inline auto _mm_add_t<__m256>(__m256 a, __m256 b)
	{
		return _mm256_add_ps(a, b);
	}

	template<>
	inline auto _mm_add_t<__m128i>(__m128i a, __m128i b)
	{
		return _mm_add_epi32(a, b);
	}

	template<>
	inline auto _mm_add_t<__m256i>(__m256i a, __m256i b)
	{
		return _mm256_add_epi32(a, b);
	}

	template<>
	inline auto _mm_add_t<__m256d>(__m256d a, __m256d b)
	{
		return _mm256_add_pd(a, b);
	}

	// _MM_ADD_T
	template<typename __mT>
	inline auto _mm_hadd_t(__mT a, __mT b)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_hadd_t<__m128>(__m128 a, __m128 b)
	{
		return _mm_hadd_ps(a, b);
	}

	template<>
	inline auto _mm_hadd_t<__m256>(__m256 a, __m256 b)
	{
		return _mm256_hadd_ps(a, b);
	}

	template<>
	inline auto _mm_hadd_t<__m128i>(__m128i a, __m128i b)
	{
		return _mm_hadd_epi32(a, b);
	}

	template<>
	inline auto _mm_hadd_t<__m256i>(__m256i a, __m256i b)
	{
		return _mm256_hadd_epi32(a, b);
	}

	template<>
	inline auto _mm_hadd_t<__m256d>(__m256d a, __m256d b)
	{
		return _mm256_hadd_pd(a, b);
	}


	// _MM_ADD_T
	template<typename __mT>
	inline auto _mm_movemask_t(__mT a)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_movemask_t<__m128>(__m128 a)
	{
		return _mm_movemask_ps(a);
	}

	template<>
	inline auto _mm_movemask_t<__m256>(__m256 a)
	{
		return _mm256_movemask_ps(a);
	}

	template<>
	inline auto _mm_movemask_t<__m128i>(__m128i a)
	{
		return _mm_movemask_epi8(a);
	}

	template<>
	inline auto _mm_movemask_t<__m256i>(__m256i a)
	{
		return _mm256_movemask_epi8(a);
	}

	template<>
	inline auto _mm_movemask_t<__m256d>(__m256d a)
	{
		return _mm256_movemask_pd(a);
	}



	// _MM_ADD_T
	template<typename T, typename __mT>
	inline auto _mm_sqrt_t(__mT a)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_sqrt_t<float, __m128>(__m128 a)
	{
		return _mm_sqrt_ps(a);
	}

	template<>
	inline auto _mm_sqrt_t<float, __m256>(__m256 a)
	{
		return _mm256_sqrt_ps(a);
	}

	template<>
	inline auto _mm_sqrt_t<double, __m256d>(__m256d a)
	{
		return _mm256_sqrt_pd(a);
	}

	template<typename T, typename __mT>
	inline auto _mm_abs_t(__mT a)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_abs_t<float, __m128i>(__m128i a)
	{
		return _mm_abs_epi32(a);
	}

	template<>
	inline auto _mm_abs_t<float, __m256i>(__m256i a)
	{
		return _mm256_abs_epi32(a);
	}

	template<>
	inline auto _mm_abs_t<float, __m128>(__m128 a)
	{
		const static __m128 mask = _mm_castsi128_ps(_mm_srli_epi32(_mm_set1_epi32(-1), 1));
		return _mm_and_ps(a, mask);
	}

	template<>
	inline auto _mm_abs_t<float, __m128d>(__m128d a)
	{
		const static __m128d mask = _mm_castsi128_pd(_mm_srli_epi32(_mm_set1_epi32(-1), 1));
		return _mm_and_pd(a, mask);
	}

	template<>
	inline auto _mm_abs_t<float, __m256>(__m256 a)
	{
		const static __m256 mask = _mm256_castsi256_ps(_mm256_srli_epi32(_mm256_set1_epi32(-1), 1));
		return _mm256_and_ps(a, mask);
	}

	template<>
	inline auto _mm_abs_t<double, __m256d>(__m256d a)
	{
		const static __m256d mask = _mm256_castsi256_pd(_mm256_srli_epi32(_mm256_set1_epi32(-1), 1));
		return _mm256_and_pd(a, mask);
	}

	template<typename __mT>
	inline auto _mm_ceil_t(__mT a)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	
	template<>
	inline auto _mm_ceil_t<__m128>(__m128 a)
	{
		return _mm_ceil_ps(a);
	}

	template<>
	inline auto _mm_ceil_t<__m128d>(__m128d a)
	{
		return _mm_ceil_pd(a);
	}

	template<>
	inline auto _mm_ceil_t<__m256>(__m256 a)
	{
		return _mm256_ceil_ps(a);
	}

	template<>
	inline auto _mm_ceil_t<__m256d>(__m256d a)
	{
		return _mm256_ceil_pd(a);
	}

	template<typename __mT>
	inline auto _mm_floor_t(__mT a)
	{
		static_assert(false, "The type is not supported");
		return;
	}


	template<>
	inline auto _mm_floor_t<__m128>(__m128 a)
	{
		return _mm_floor_ps(a);
	}

	template<>
	inline auto _mm_floor_t<__m128d>(__m128d a)
	{
		return _mm_floor_pd(a);
	}

	template<>
	inline auto _mm_floor_t<__m256>(__m256 a)
	{
		return _mm256_floor_ps(a);
	}

	template<>
	inline auto _mm_floor_t<__m256d>(__m256d a)
	{
		return _mm256_floor_pd(a);
	}

	template<typename __mT>
	inline auto _mm_permute8x32_var_t(__mT a, __m256i idx)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_permute8x32_var_t<__m256i>(__m256i a, __m256i idx)
	{
		return _mm256_permutevar8x32_epi32(a, idx);
	}

	template<>
	inline auto _mm_permute8x32_var_t<__m256>(__m256 a, __m256i idx)
	{
		return _mm256_permutevar8x32_ps(a, idx);
	}


	// _MM_MIN_T
	template<typename T, typename __mT = __m_t<T>>
	inline auto _mm_min_t(__mT a, __mT b)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_min_t<float>(__m256 a, __m256 b)
	{
		return _mm256_min_ps(a, b);
	}

	template<>
	inline auto _mm_min_t<float>(__m128 a, __m128 b)
	{
		return _mm_min_ps(a, b);
	}

	template<>
	inline auto _mm_min_t<int32_t>(__m128i a, __m128i b)
	{
		return _mm_min_epi32(a, b);
	}

	template<>
	inline auto _mm_min_t<uint32_t>(__m128i a, __m128i b)
	{
		return _mm_min_epu32(a, b);
	}

	template<>
	inline auto _mm_min_t<double>(__m256d a, __m256d b)
	{
		return _mm256_min_pd(a, b);
	}

	// _MM_MAX_T
	template<typename T, typename __mT = __m_t<T>>
	inline auto _mm_max_t(__mT a, __mT b)
	{
		static_assert(false, "The type is not supported");
		return;
	}


	template<>
	inline auto _mm_max_t<float>(__m128 a, __m128 b)
	{
		return _mm_max_ps(a, b);
	}

	template<>
	inline auto _mm_max_t<int32_t>(__m128i a, __m128i b)
	{
		return _mm_max_epi32(a, b);
	}

	template<>
	inline auto _mm_max_t<uint32_t>(__m128i a, __m128i b)
	{
		return _mm_max_epu32(a, b);
	}

	template<>
	inline auto _mm_max_t<double>(__m256d a, __m256d b)
	{
		return _mm256_max_pd(a, b);
	}


	// _MM_SUB_T

	template<typename T, typename __mT>
	inline auto _mm_sub_t(__mT a, __mT b)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_sub_t<float, __m128>(__m128 a, __m128 b)
	{
		return _mm_sub_ps(a, b);
	}

	template<>
	inline auto _mm_sub_t<float, __m256>(__m256 a, __m256 b)
	{
		return _mm256_sub_ps(a, b);
	}

	template<>
	inline auto _mm_sub_t<int32_t, __m128i>(__m128i a, __m128i b)
	{
		return _mm_sub_epi32(a, b);
	}

	template<>
	inline auto _mm_sub_t<int32_t, __m256i>(__m256i a, __m256i b)
	{
		return _mm256_sub_epi32(a, b);
	}

	template<>
	inline auto _mm_sub_t<uint32_t, __m128i>(__m128i a, __m128i b)
	{
		return _mm_sub_epi32(a, b);
	}

	template<>
	inline auto _mm_sub_t<uint32_t, __m256i>(__m256i a, __m256i b)
	{
		return _mm256_sub_epi32(a, b);
	}

	template<>
	inline auto _mm_sub_t<double>(__m256d a, __m256d b)
	{
		return _mm256_sub_pd(a, b);
	}

	// _MM_MUL_T
	template<typename __mT> inline auto _mm_mul_t(__mT a, __mT b)
	{
		static_assert(false, "The type is not supported"); return;
	}

	template<> inline auto _mm_mul_t<__m128>(__m128 a, __m128 b){ return _mm_mul_ps(a, b); }
	template<> inline auto _mm_mul_t<__m128i>(__m128i a, __m128i b) { return _mm_mullo_epi32(a, b); }
	template<> inline auto _mm_mul_t<__m256>(__m256 a, __m256 b){ return _mm256_mul_ps(a, b); }
	template<> inline auto _mm_mul_t<__m256i>(__m256i a, __m256i b){ return _mm256_mullo_epi32(a, b); }
	template<> inline auto _mm_mul_t<__m256d>(__m256d a, __m256d b){ return _mm256_mul_pd(a, b); }

	// _mm_cmp_eq_t
	template<typename __mT >
	inline auto _mm_cmp_eq_t(__mT a, __mT b)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_cmp_eq_t<__m128>(__m128 a, __m128 b)
	{
		return _mm_cmpeq_ps(a, b);
	}

	template<>
	inline auto _mm_cmp_eq_t<__m128i>(__m128i a, __m128i b)
	{
		return _mm_cmpeq_epi32(a, b);
	}

	template<>
	inline auto _mm_cmp_eq_t<__m256d>(__m256d a, __m256d b)
	{
		return _mm256_cmp_pd(a, b, 0);
	}

	template<>
	inline auto _mm_cmp_eq_t<__m256>(__m256 a, __m256 b)
	{
		return _mm256_cmp_ps(a, b, 0);
	}

	template<typename __mT>
	inline auto _mm_predicate_load_t(__mT mask, __mT trueV, __mT falseV)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_predicate_load_t<__m128>(__m128 mask, __m128 trueV, __m128 falseV)
	{
		return _mm_blendv_ps(falseV, trueV, mask);
	}

	template<>
	inline auto _mm_predicate_load_t<__m256d>(__m256d mask, __m256d trueV, __m256d falseV)
	{
		return _mm256_blendv_pd(falseV, trueV, mask);
	}

	template<>
	inline auto _mm_predicate_load_t<__m256>(__m256 mask, __m256 trueV, __m256 falseV)
	{
		return _mm256_blendv_ps(falseV, trueV, mask);
	}

	// _MM_DIV_T

	template<typename T, typename __mT = __m_t<T>>
	inline auto _mm_div_t(__mT a, __mT b)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_div_t<float>(__m128 a, __m128 b)
	{
		return _mm_div_ps(a, b);
	}

	template<>
	inline auto _mm_div_t<float>(__m256 a, __m256 b)
	{
		return _mm256_div_ps(a, b);
	}


	template<>
	inline auto _mm_div_t<double>(__m256d a, __m256d b)
	{
		return _mm256_div_pd(a, b);
	}

	// _MM_MADD_T
	template<typename T, typename __mT = __m_t<T>>
	inline auto _mm_madd_t(__mT a, __mT b, __mT c)
	{
		static_assert(false, "The type is not supported");
		return;
	}


	template<>
	inline auto _mm_madd_t<float>(__m128 a, __m128 b, __m128 c)
	{
		return _mm_fmadd_ps(a, b, c);
	}

	template<>
	inline auto _mm_madd_t<double>(__m256d a, __m256d b, __m256d c)
	{
		return _mm256_fmadd_pd(a, b, c);
	}

	template<>
	inline auto _mm_madd_t<int32_t>(__m128i a, __m128i b, __m128i c)
	{
		__m128i buffer = _mm_mullo_epi32(a, b);
		return _mm_add_epi32(buffer, c);
	}

	


	template<>
	inline auto _mm_madd_t<uint32_t>(__m128i a, __m128i b, __m128i c)
	{
		__m128i buffer = _mm_mullo_epi32(a, b);
		return _mm_add_epi32(buffer, c);
	}
	
	template<uint8_t controlValue>
	struct _mm_permute_ts
	{

		template<typename __mT>
		static inline auto f(__mT a)
		{
			static_assert(false, "The type is not supported");
			return;
		}

		template<> static inline auto f<__m128>(__m128 a){ return _mm_permute_ps(a, controlValue); }
		template<> static inline auto f<__m256d>(__m256d a){ return _mm256_permute_pd(a, controlValue); }
		template<> static inline auto f<__m256>(__m256 a){ return _mm256_permute_ps(a, controlValue); }
		template<> static inline auto f<__m128i>(__m128i a){ return *reinterpret_cast<__m128i*>(&_mm_permute_ps(*((__m128*)(&a)), controlValue));}
	};


	template<uint8_t controlValue>
	struct _mm_cmp_ts
	{

		template<typename __mT>
		static inline auto f(__mT a, __mT b)
		{
			static_assert(false, "The type is not supported");
			return;
		}

		template<> static inline auto f<__m256d>(__m256d a, __m256d b){ return _mm256_cmp_pd(a, b, controlValue); }
		template<> static inline auto f<__m256>(__m256 a, __m256 b){ return _mm256_cmp_ps(a,b, controlValue); }
		template<> static inline auto f<__m128>(__m128 a, __m128 b){ return _mm_cmp_ps(a, b, controlValue);}
	};



	/*template<typename __mT, typename T = __mT_type_extract_t<__mT>, typename __mT128 = __m128_t<T>>
	inline auto dot_t(const __mT& x0, const __mT& y0)
	{
		__mT xy = _mm_mul_t(x0, y0);
		__mT temp = _mm_hadd_t(xy, xy);
		__mT128 hi128 = _mm_extractf128_ts<1>::(temp);
		__mT128 dotproduct = _mm_add_pd((__m128d)temp, hi128);
		return reinterpret_cast<T*>(&dotproduct)[0];
	}*/



	template<uint8_t controlValue>
	struct _mm_permute2f128_ts
	{

		template<typename __mT>
		static inline auto f(__mT a, __mT b)
		{
			static_assert(false, "The type is not supported");
			return;
		}

		template<> static inline auto f<__m256>(__m256 a, __m256 b){ return _mm256_permute2f128_ps(a, b, controlValue); }
		template<> static inline auto f<__m256i>(__m256i a, __m256i b){ return _mm256_permute2f128_si256(a, b, controlValue); }
		template<> static inline auto f<__m256d>(__m256d a, __m256d b){ return _mm256_permute2f128_pd(a, b, controlValue); }
	};


	template<uint8_t controlValue>
	struct _mm_extractf128_ts
	{

		template<typename __mT>
		static inline auto f(__mT a)
		{
			static_assert(false, "The type is not supported");
			return;
		}

		template<> static inline auto f<__m256>(__m256 a){ return _mm256_extractf128_ps(a, controlValue); }
		template<> static inline auto f<__m256i>(__m256i a){ return _mm256_extractf128_si256(a, controlValue); }
		template<> static inline auto f<__m256d>(__m256d a){ return _mm256_extractf128_pd(a, controlValue); }
	};

	template<uint8_t controlValue>
	struct _mm_blend_ts
	{

		template<typename __mT>
		static inline auto f(__mT a, __mT b)
		{
			static_assert(false, "The type is not supported");
			return;
		}

		template<> static inline auto f<__m128>(__m128 f, __m128 t){ return _mm_blend_ps(f,t, controlValue); }
		template<> static inline auto f<__m256>(__m256 f, __m256 t){ return _mm256_blend_ps(f, t, controlValue); }
		template<> static inline auto f<__m256d>(__m256d f, __m256d t){ return _mm256_blend_pd(f, t, controlValue); }
		template<> static inline auto f<__m128i>(__m128i f, __m128i t){ return _mm_blend_epi32(f, t, controlValue); }
	};



	// _MM_INSERT_T insert 1 value in a corresponding position

	//template<uint8_t controlValue, typename T, typename __mT = __m_t<T>>
	//struct _mm_insert_ts
	//{
	//	static inline auto f(__mT a, T value, uint8_t pos)
	//	{
	//		static_assert(false, "The type is not supported");
	//		return;
	//	}
	//};


	template<typename T, typename __mT>
	inline auto _mm_insert_t(__mT a, T b, uint8_t pos)
	{
		static_assert(false, "The type is not supported");
		return;
	}



	template<>
	inline auto _mm_insert_t<float, __m128>(__m128 a, float value, uint8_t pos)
	{
		/*__m128 sVector = _mm_set1_ps(value);
		return _mm_insert_ps(a, sVector, controlValue << 4);*/

		__m128 sVector = _mm_set1_ps(value);
		switch (pos)
		{
			case 0:
				return _mm_insert_ps(a, sVector, 0 << 4);
			case 1:
				return _mm_insert_ps(a, sVector, 1 << 4);
			case 2:
				return _mm_insert_ps(a, sVector, 2 << 4);
			case 3:
				return _mm_insert_ps(a, sVector, 3 << 4);
		}
	}

	template<>
	inline auto _mm_insert_t<double, __m256d>(__m256d a, double value, uint8_t pos)
	{
			/*__m256d sVector = _mm256_set1_pd(value);
			return _mm256_blend_pd(a, sVector, 1 << controlValue);*/
		a.m256d_f64[pos] = value;
		return a;
	}

	template<>
	inline auto _mm_insert_t<float, __m256>(__m256 a, float value, uint8_t pos)
	{
		/*__m256d sVector = _mm256_set1_pd(value);
		return _mm256_blend_pd(a, sVector, 1 << controlValue);*/
		a.m256_f32[pos] = value;
		return a;
		/*__m256 sVector = _mm256_set1_ps(value);
		switch (pos)
		{
			case 0:
				return _mm256_blend_ps(a, sVector, 1);
			case 1:
				return _mm256_blend_ps(a, sVector, 2);
			case 2:
				return _mm256_blend_ps(a, sVector, 3);
			case 3:
				return _mm256_blend_ps(a, sVector, 4);
		}*/
	}

	template<>
	inline auto _mm_insert_t<int32_t, __m128i>(__m128i a, int32_t value, uint8_t pos)
	{
		/*return _mm_insert_epi32(a, value, controlValue);*/

		switch (pos)
		{
			case 0:
				return _mm_insert_epi32(a, value, 0);
			case 1:
				return _mm_insert_epi32(a, value, 1);
			case 2:
				return _mm_insert_epi32(a, value, 2);
			case 3:
				return _mm_insert_epi32(a, value, 3);
		}
	}

	template<>
	inline auto _mm_insert_t<uint32_t, __m128i>(__m128i a, uint32_t value, uint8_t pos)
	{
			/*return _mm_insert_epi32(a, value, 0);*/
		switch (pos)
		{
			case 0:
				return _mm_insert_epi32(a, value, 0);
			case 1:
				return _mm_insert_epi32(a, value, 1);
			case 2:
				return _mm_insert_epi32(a, value, 2);
			case 3:
				return _mm_insert_epi32(a, value, 3);
		}
	}

	template<typename __mT>
	inline auto dot_t(const __mT& x0, const __mT& x1, const __mT& x2, const __mT& x3,
					  const __mT& y0, const __mT& y1, const __mT& y2, const __mT& y3)
	{
		__mT xy0 = _mm_mul_t(x0, y0);
		__mT xy1 = _mm_mul_t(x1, y1);
		__mT xy2 = _mm_mul_t(x2, y2);
		__mT xy3 = _mm_mul_t(x3, y3);

		// low to high: xy00+xy01 xy10+xy11 xy02+xy03 xy12+xy13
		__mT temp01 = _mm_hadd_t(xy0, xy1);

		// low to high: xy20+xy21 xy30+xy31 xy22+xy23 xy32+xy33
		__mT temp23 = _mm_hadd_t(xy2, xy3);

		// low to high: xy02+xy03 xy12+xy13 xy20+xy21 xy30+xy31
		__mT swapped = _mm_permute2f128_ts<0x21>::f(temp01, temp23);

		// low to high: xy00+xy01 xy10+xy11 xy22+xy23 xy32+xy33
		__mT blended = _mm_blend_ts<0b1100>::f(temp01, temp23);

		return _mm_add_t(swapped, blended);
	}

	template<typename __mT, typename __mT128 = __m128_t<__mT_type_extract_t<__mT>>>
	inline auto dot_t(const __mT& x0, const __mT& x1,
					  const __mT& y0, const __mT& y1)
	{

		__mT xy = _mm_mul_t(x0, y0);
		__mT zw = _mm_mul_t(x1, y1);
		__mT temp = _mm_hadd_t(xy, zw);
		__mT128 hi128 = _mm_extractf128_ts<1>::f(temp);
		return _mm_add_t((__mT128)temp, hi128);
	}

	template<typename __mT, typename __mT128 = __m128_t<__mT_type_extract_t<__mT>>>
	inline __mT128 dot2in1_t(const __mT& x0, const __mT& x1,
						  const __mT& y0, const __mT& y1)
	{

		// 0: x0.0.x*y0.0.x | 1: x0.0.y*y0.0.y | 2: x0.0.z*y0.0.z | 3: x0.0.w*y0.0.w |
		// 4: x0.1.x*y0.1.x | 5: x0.1.y*y0.1.y | 6: x0.1.z*y0.1.z | 7: x0.1.w*y0.1.w |
		__mT xy = _mm_mul_t(x0, y0);

		// 0: x0.0.x*y0.0.x | 1: x0.0.y*y0.0.y | 2: x0.0.z*y0.0.z | 3: x0.0.w*y0.0.w |
		// 4: x0.1.x*y0.1.x | 5: x0.1.y*y0.1.y | 6: x0.1.z*y0.1.z | 7: x0.1.w*y0.1.w |
		__mT zw = _mm_mul_t(x1, y1);

		// 0: x0.0.x*y0.0.x + x0.0.y*y0.0.y | 1: x0.0.z*y0.0.z+x0.0.w*y0.0.w |
		// 2: x1.0.x*y1.0.x + x1.0.y*y1.0.y | 3: x1.0.z*y1.0.z+x1.0.w*y1.0.w |
		// 4: x0.1.x*y0.1.x + x0.1.y*y0.0.y | 5: x0.1.z*y0.1.z+x0.1.w*y0.1.w |
		// 6: x1.1.x*y1.1.x + x1.1.y*y1.0.y | 7: x1.1.z*y1.1.z+x1.1.w*y1.1.w |
		__mT temp = _mm_hadd_t(xy, zw);

	/*	uint32_t d[8] = {
			0, 4, 2, 6, 1, 5, 3, 7
		};*/

		static __m256i mask;
		mask.m256i_i32[0] = 0;
		mask.m256i_i32[1] = 4;
		mask.m256i_i32[2] = 2;
		mask.m256i_i32[3] = 6;
		mask.m256i_i32[4] = 1;
		mask.m256i_i32[5] = 5;
		mask.m256i_i32[6] = 3;
		mask.m256i_i32[7] = 7;

		// 0: x0.0.x*y0.0.x + x0.0.y*y0.0.y | 4: x0.1.x*y0.1.x + x0.1.y*y0.0.y | 2: x1.0.x*y1.0.x + x1.0.y*y1.0.y | 6: x1.1.x*y1.1.x + x1.1.y*y1.0.y| 
		// 1: x0.0.z*y0.0.z+x0.0.w*y0.0.w   | 5: x0.1.z*y0.1.z+x0.1.w*y0.1.w   | 3: x1.0.z*y1.0.z+x1.0.w*y1.0.w   | 7: x1.1.z*y1.1.z+x1.1.w*y1.1.w 
		temp = _mm_permute8x32_var_t(temp, mask);

		__mT128 hi128 = _mm_extractf128_ts<1>::f(temp);
		__mT128 low128 = _mm_extractf128_ts<0>::f(temp);

		return _mm_add_t(low128, hi128);
	}


	template<typename __mT, typename T>
	constexpr __mT zeroXMM()
	{
		__mT xmm;
		xmm = _mm_set1_t<__mT, T>((T)(0));
		return xmm;
	}
}




#pragma once

#include <immintrin.h>
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
	
	template<typename T>
	struct __m_type
	{};

	template<>
	struct __m_type<float>
	{
		using type = typename __m128;
	};

	template<>
	struct __m_type<double>
	{
		using type = typename __m256d;
	};

	template<>
	struct __m_type<int32_t>
	{
		using type = typename __m128i;
	};

	template<>
	struct __m_type<uint32_t>
	{
		using type = typename __m128i;
	};

	template<typename T>
	using __m_t = typename __m_type<T>::type;

	template<typename T>
	struct __m256_type
	{};

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


	// _MM_LOADA_T
	template<typename T>
	inline auto _mm_loada_t(const T* data)
	{
		static_assert(false, "The type is not supported");
		return;
	}




	template<>
	inline auto _mm_loada_t<float>(const float* data)
	{
		return _mm_load_ps(data);
	}

	template<>
	inline auto _mm_loada_t<int32_t>(const int32_t* data)
	{
		return _mm_load_si128((__m128i*)data);
	}

	template<>
	inline auto _mm_loada_t<uint32_t>(const uint32_t* data)
	{
		return _mm_load_si128((__m128i*)data);
	}


	template<>
	inline auto _mm_loada_t<double>(const double* data)
	{
		return _mm256_load_pd(data);
	}

	// _MM_LOADU_T
	template<typename T>
	inline auto _mm_loadu_t(const T* data)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_loadu_t<float>(const float* data)
	{
		return _mm_loadu_ps(data);
	}

	template<>
	inline auto _mm_loadu_t<int32_t>(const int32_t* data)
	{
		return _mm_loadu_si128((__m128i*)data);
	}

	template<>
	inline auto _mm_loadu_t<uint32_t>(const uint32_t* data)
	{
		return _mm_loadu_si128((__m128i*)data);
	}


	template<>
	inline auto _mm_loadu_t<double>(const double* data)
	{
		return _mm256_loadu_pd(data);
	}
	
	


	// _MM_SET1_T
	template<typename T, typename __mT>
	inline __mT _mm_set1_t(T data)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline __m256d _mm_set1_t<double, __m256d>(double scalar)
	{
		return _mm256_set1_pd(scalar);
	}

	template<>
	inline __m128 _mm_set1_t<float, __m128>(float scalar)
	{
		return _mm_set1_ps(scalar);
	}

	template<>
	inline __m256 _mm_set1_t<float, __m256>(float scalar)
	{
		return _mm256_set1_ps(scalar);
	}

	template<>
	inline __m256i _mm_set1_t<int32_t, __m256i>(int32_t scalar)
	{
		return _mm256_set1_epi32(scalar);
	}

	template<>
	inline __m256i _mm_set1_t<uint32_t, __m256i>(uint32_t scalar)
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
	inline auto _mm256_broadcast_t(T* data)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm256_broadcast_t<float>(float* data)
	{
		return _mm256_broadcast_ps((__m128 const*)data);
	}

	template<>
	inline auto _mm256_broadcast_t<double>(double* data)
	{ 
		return _mm256_loadu_pd(data);
	}

	template<>
	inline auto _mm256_broadcast_t<int32_t>(int32_t* data)
	{
		__m128i xmm = _mm_loadu_si128((__m128i*)data);
		return _mm256_broadcastsi128_si256(xmm);
	}

	template<>
	inline auto _mm256_broadcast_t<uint32_t>(uint32_t* data)
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



	// _MM_ADD_T
	template<typename T, typename __mT>
	inline auto _mm_add_t(__mT a, __mT b)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_add_t<float, __m128>(__m128 a, __m128 b)
	{
		return _mm_add_ps(a, b);
	}

	template<>
	inline auto _mm_add_t<float, __m256>(__m256 a, __m256 b)
	{
		return _mm256_add_ps(a, b);
	}

	template<>
	inline auto _mm_add_t<int32_t, __m128i>(__m128i a, __m128i b)
	{
		return _mm_add_epi32(a, b);
	}

	template<>
	inline auto _mm_add_t<int32_t, __m256i>(__m256i a, __m256i b)
	{
		return _mm256_add_epi32(a, b);
	}

	template<>
	inline auto _mm_add_t<uint32_t, __m128i>(__m128i a, __m128i b)
	{
		return _mm_add_epi32(a, b);
	}

	template<>
	inline auto _mm_add_t<uint32_t, __m256i>(__m256i a, __m256i b)
	{
		return _mm256_add_epi32(a, b);
	}

	template<>
	inline auto _mm_add_t<double, __m256d>(__m256d a, __m256d b)
	{
		return _mm256_add_pd(a, b);
	}

	// _MM_MIN_T
	template<typename T, typename __mT = __m_t<T>>
	inline auto _mm_min_t(__mT a, __mT b)
	{
		static_assert(false, "The type is not supported");
		return;
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
	template<typename T, typename __mT>
	inline auto _mm_mul_t(__mT a, __mT b)
	{
		static_assert(false, "The type is not supported");
		return ;
	}

	template<>
	inline auto _mm_mul_t<float, __m128>(__m128 a, __m128 b)
	{
		return _mm_mul_ps(a, b);
	}

	template<>
	inline auto _mm_mul_t<int32_t, __m128i>(__m128i a, __m128i b)
	{
		return _mm_mul_epi32(a, b);
	}

	template<>
	inline auto _mm_mul_t<uint32_t, __m128i>(__m128i a, __m128i b)
	{
		return _mm_mul_epu32(a, b);
	}

	template<>
	inline auto _mm_mul_t<float, __m256>(__m256 a, __m256 b)
	{
		return _mm256_mul_ps(a, b);
	}

	template<>
	inline auto _mm_mul_t<int32_t, __m256i>(__m256i a, __m256i b)
	{
		return _mm256_mul_epi32(a, b);
	}

	template<>
	inline auto _mm_mul_t<uint32_t, __m256i>(__m256i a, __m256i b)
	{
		return _mm256_mul_epu32(a, b);
	}


	template<>
	inline auto _mm_mul_t<double, __m256d>(__m256d a, __m256d b)
	{
		return _mm256_mul_pd(a, b);
	}

	// _mm_cmp_eq_t
	template<typename T, typename __mT = __m_t<T>>
	inline auto _mm_cmp_eq_t(__mT a, __mT b)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_cmp_eq_t<float>(__m128 a, __m128 b)
	{
		return _mm_cmpeq_ps(a, b);
	}

	template<>
	inline auto _mm_cmp_eq_t<int32_t>(__m128i a, __m128i b)
	{
		return _mm_cmpeq_epi32(a, b);
	}

	template<>
	inline auto _mm_cmp_eq_t<uint32_t>(__m128i a, __m128i b)
	{
		return _mm_cmpeq_epi32(a, b);
	}

	template<>
	inline auto _mm_cmp_eq_t<double>(__m256d a, __m256d b)
	{
		return _mm256_cmp_pd(a, b, 0);
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
		__m128i buffer = _mm_mul_epi32(a, b);
		return _mm_add_epi32(buffer, c);
	}

	template<>
	inline auto _mm_madd_t<uint32_t>(__m128i a, __m128i b, __m128i c)
	{
		__m128i buffer = _mm_mul_epu32(a, b);
		return _mm_add_epi32(buffer, c);
	}
	
	template<uint8_t controlValue, typename T, typename __mT = __m_t<T>>
	struct _mm_permute_ts
	{
		static inline auto f(__mT a)
		{
			static_assert(false, "The type is not supported");
			return;
		}
	};

	template<uint8_t controlValue>
	struct _mm_permute_ts<controlValue, float>
	{
		static inline auto f(__m128 a)
		{
			return _mm_permute_ps(a, controlValue);
		}

	};


	template<uint8_t controlValue>
	struct _mm_permute_ts<controlValue, double>
	{
		static inline auto f(__m256d a)
		{
			return _mm256_permute_pd(a, controlValue);
		}

	};


	template<uint8_t controlValue>
	struct _mm_permute_ts<controlValue, int32_t>
	{
		static inline auto f(__m128i a)
		{
			return _mm_permute_ps(*((__m128*)(&a)), controlValue);
		}

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


	template<typename T, typename __mT = __m_t<T>>
	inline auto _mm_insert_t(__mT a, T b, uint8_t pos)
	{
		static_assert(false, "The type is not supported");
		return;
	}



	template<>
	inline auto _mm_insert_t<float>(__m128 a, float value, uint8_t pos)
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
	inline auto _mm_insert_t<double>(__m256d a, double value, uint8_t pos)
	{
			/*__m256d sVector = _mm256_set1_pd(value);
			return _mm256_blend_pd(a, sVector, 1 << controlValue);*/
		__m256d sVector = _mm256_set1_pd(value);
		switch (pos)
		{
			case 0:
				return _mm256_blend_pd(a, sVector, 1);
			case 1:
				return _mm256_blend_pd(a, sVector, 2);
			case 2:
				return _mm256_blend_pd(a, sVector, 3);
			case 3:
				return _mm256_blend_pd(a, sVector, 4);
		}
	}

	template<>
	inline auto _mm_insert_t<int32_t>(__m128i a, int32_t value, uint8_t pos)
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
	inline auto _mm_insert_t<uint32_t>(__m128i a, uint32_t value, uint8_t pos)
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


	

}




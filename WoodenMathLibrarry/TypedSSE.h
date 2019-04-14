#pragma once

#include <immintrin.h>
#include "stdafx.h"

namespace TypedSSE
{

	
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


	// _MM_LOAD_T
	template<typename T>
	inline auto _mm_load_t(const T* data)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<typename T>
	using __m_t = typename __m_type<T>::type;


	template<>
	inline auto _mm_load_t<float>(const float* data)
	{
		return _mm_load_ps(data);
	}

	template<>
	inline auto _mm_load_t<int32_t>(const int32_t* data)
	{
		return _mm_load_si128((__m128i*)data);
	}

	template<>
	inline auto _mm_load_t<uint32_t>(const uint32_t* data)
	{
		return _mm_load_si128((__m128i*)data);
	}


	template<>
	inline auto _mm_load_t<double>(const double* data)
	{
		return _mm256_load_pd(data);
	}


	// _MM_SET1_T
	template<typename T>
	inline auto _mm_set1_t(T data)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_set1_t<float>(float scalar)
	{
		return _mm_set1_ps(scalar);
	}

	template<>
	inline auto _mm_set1_t<double>(double scalar)
	{
		return _mm256_set1_pd(scalar);
	}

	template<>
	inline auto _mm_set1_t<int32_t>(int32_t scalar)
	{
		return _mm_set1_epi32(scalar);
	}

	template<>
	inline auto _mm_set1_t<uint32_t>(uint32_t scalar)
	{
		return _mm_set1_epi32(scalar);
	}

	// _MM_STORE_T
	template<typename T, typename __mT = __m_t<T>>
	inline auto _mm_store_t(T* data, __mT vec)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_store_t<float>(float* data, __m128 vec)
	{
		return _mm_store_ps(data, vec);
	}

	template<>
	inline auto _mm_store_t<double>(double* data, __m256d vec)
	{
		return _mm256_store_pd(data, vec);
	}

	template<>
	inline auto _mm_store_t<int32_t>(int32_t* data, __m128i vc)
	{
		return _mm_store_si128((__m128i*)data, vc);
	}

	template<>
	inline auto _mm_store_t<uint32_t>(uint32_t* data, __m128i vc)
	{
		return _mm_store_si128((__m128i*)data, vc);
	}



	// _MM_ADD_T
	template<typename T, typename __mT = __m_t<T>>
	inline auto _mm_add_t(__mT a, __mT b)
	{
		static_assert(false, "The type is not supported");
		return;
	}


	template<>
	inline auto _mm_add_t<float>(__m128 a, __m128 b)
	{
		return _mm_add_ps(a, b);
	}

	template<>
	inline auto _mm_add_t<int32_t>(__m128i a, __m128i b)
	{
		return _mm_add_epi32(a, b);
	}

	template<>
	inline auto _mm_add_t<uint32_t>(__m128i a, __m128i b)
	{
		return _mm_add_epi32(a, b);
	}

	template<>
	inline auto _mm_add_t<double>(__m256d a, __m256d b)
	{
		return _mm256_add_pd(a, b);
	}

	// _MM_SUB_T

	template<typename T, typename __mT = __m_t<T>>
	inline auto _mm_sub_t(__mT a, __mT b)
	{
		static_assert(false, "The type is not supported");
		return;
	}


	template<>
	inline auto _mm_sub_t<float>(__m128 a, __m128 b)
	{
		return _mm_sub_ps(a, b);
	}

	template<>
	inline auto _mm_sub_t<int32_t>(__m128i a, __m128i b)
	{
		return _mm_sub_epi32(a, b);
	}

	template<>
	inline auto _mm_sub_t<uint32_t>(__m128i a, __m128i b)
	{
		return _mm_sub_epi32(a, b);
	}

	template<>
	inline auto _mm_sub_t<double>(__m256d a, __m256d b)
	{
		return _mm256_sub_pd(a, b);
	}

	// _MM_MUL_T

	template<typename T, typename __mT = __m_t<T>>
	inline auto _mm_mul_t(__mT a, __mT b)
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<>
	inline auto _mm_mul_t<float>(__m128 a, __m128 b)
	{
		return _mm_mul_ps(a, b);
	}

	template<>
	inline auto _mm_mul_t<int32_t>(__m128i a, __m128i b)
	{
		return _mm_mul_epi32(a, b);
	}

	template<>
	inline auto _mm_mul_t<uint32_t>(__m128i a, __m128i b)
	{
		return _mm_mul_epu32(a, b);
	}

	template<>
	inline auto _mm_mul_t<double>(__m256d a, __m256d b)
	{
		return _mm256_mul_pd(a, b);
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

	template<typename T, uint8_t controlValue, typename __mT = __m_t<T>>
	struct _mm_permute_ts
	{
		inline auto _mm_permute_t(__mT a)
		{
			static_assert(false, "The type is not supported");
			return;
		}
	};

	template<uint8_t controlValue>
	struct _mm_permute_ts<float, controlValue>
	{
		inline auto _mm_permute_t(__m128 a)
		{
			return _mm_permute_ps(a, controlValue);
		}

	};



	template<uint8_t controlValue>
	struct _mm_permute_ts<double, controlValue>
	{
		inline auto _mm_permute_t(__m256d a)
		{
			return _mm256_permute_pd(a, controlValue);
		}

	};


	template<uint8_t controlValue>
	struct _mm_permute_ts<int32_t, controlValue>
	{
		inline auto _mm_permute_t(__m128i a)
		{
			return _mm_permute_ps(*((__m128*)(&a)), controlValue);
		}

	};

	/*
	// _MM_PERMUT_T
	template<typename T, uint8_t controlValue, typename __mT = __m_t<T>>
	inline auto _mm_permute_t(__mT a, std::integral_constant<uint8_t, )
	{
		static_assert(false, "The type is not supported");
		return;
	}

	template<uint8_t controlValue>
	inline auto _mm_permute_t<controlValue, float>(__m128 a)
	{
		return _mm_permute_ps(a, controlValue);
	}

	template<uint8_t controlValue>
	inline auto _mm_permute_t<controlValue, double>(__m256d a)
	{
		return _mm256_permute_pd(a, controlValue);
	}

	template<uint8_t controlValue>
	inline auto _mm_permute_t<int32_t, controlValue>(__m128i a)
	{
		return _mm_permute_ps(*((__m128*)(&a)), controlValue);
	}*/

}




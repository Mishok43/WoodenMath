#pragma once

#include <stdint.h>
#include <immintrin.h>


namespace TypedSSE
{
	template<typename T = float>
	struct __m_type
	{
		using type = typename __m128;
	};


	template<typename T = double>
	struct __m_type
	{
		using type = typename __m256d;
	};

	template<typename T = std::int32_t>
	struct __m_type
	{
		using type = typename __m128i;
	};

	template<typename T = std::int32_t>
	struct __m_type
	{
		using type = typename __m128i;
	};

	
	template<typename T>
	using __m_t = __m_type<T>::type;

	// _MM_LOAD_T
	template<typename T, typename T_t = std::enable_if_t<std::is_same_v<float, T>>>
	inline auto _mm_load_t(const T* data)
	{
		return _mm_load_ps(data);
	}

	template<typename T = std::int32_t>
	inline auto _mm_load_t(const T* data)
	{
		return _mm_load_si128(data);
	}

	template<typename T = std::uint32_t>
	inline auto _mm_load_t(const T* data)
	{
		return _mm_load_si128(data);
	}

	template<typename T, typename T_t = std::enable_if_t<std::is_same_v<double, T>>>
	inline auto _mm_load_t(const T* data)
	{
		return _mm256_load_pd(data);
	}

	// _MM_ADD_T

	template<typename T = float>
	inline auto _mm_add_t(const T* a, const T* b)
	{
		return _mm_add_ps(a, b);
	}

	template<typename T = std::int32_t>
	inline auto _mm_add_t(const T* a, const T* b)
	{
		return _mm_add_epi32(a, b);
	}

	template<typename T = std::uint32_t>
	inline auto _mm_add_t(const T* a, const T* b)
	{
		return _mm_add_epi32(a, b);
	}

	template<typename T = double>
	inline auto _mm_add_t(const T* a, const T* b)
	{
		return _mm256_add_pd(a, b);
	}

	// _MM_SUB_T

	template<typename T = float>
	inline auto _mm_sub_t(const T* a, const T* b)
	{
		return _mm_sub_ps(a, b);
	}

	template<typename T = std::int32_t>
	inline auto _mm_sub_t(const T* a, const T* b)
	{
		return _mm_sub_epi32(a, b);
	}

	template<typename T = std::uint32_t>
	inline auto _mm_sub_t(const T* a, const T* b)
	{
		return _mm_sub_epi32(a, b);
	}

	template<typename T = double>
	inline auto _mm_sub_t(const T* a, const T* b)
	{
		return _mm256_sub_pd(a, b);
	}

	// _MM_MUL_T

	template<typename T = float>
	inline auto _mm_mul_t(const T* a, const T* b)
	{
		return _mm_mul_ps(a, b);
	}

	template<typename T = std::int32_t>
	inline auto _mm_mul_t(const T* a, const T* b)
	{
		return _mm_mul_epi32(a, b);
	}

	template<typename T = std::uint32_t>
	inline auto _mm_mul_t(const T* a, const T* b)
	{
		return _mm_mul_epu32(a, b);
	}

	template<typename T = double>
	inline auto _mm_mul_t(const T* a, const T* b)
	{
		return _mm256_mul_pd(a, b);
	}

	// _MM_MADD_T

	template<typename T = float>
	inline auto _mm_madd_t(const T* a, const T* b, const T* c)
	{
		return _mm_fmadd_ps(a, b, c);
	}

	template<typename T = double>
	inline auto _mm_madd_t(const T* a, const T* b, const T* c)
	{
		return _mm_fmadd_pd(a, b, c);
	}

	template<typename T = std::int32_t>
	inline auto _mm_madd_t(const T* a, const T* b, const T* c)
	{
		__m128i buffer = _mm_mul_epi32(a, b);
		return _mm_add_epi32(buffer, c);
	}

	template<typename T = std::uint32_t>
	inline auto _mm_madd_t(const T* a, const T* b, const T* c)
	{
		__m128i buffer = _mm_mul_epu32(a, b);
		return _mm_add_epi32(buffer, c);

	}




}




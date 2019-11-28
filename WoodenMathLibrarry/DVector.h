#pragma once
#include "stdafx.h"
#include "TypedSSE.h"
#include <array>

WML_BEGIN

#define ALIGN_AS_VEC(T, SIZE) alignas(alignof(DVector<T, SIZE>))

// TODO: fork some methods for different sizes 

using namespace TypedSSE;

template<typename T, uint32_t Size>
class DVectorPacked;

using DVectorPacked1f = typename DVectorPacked<float, 1>;
using DVectorPacked2f = typename DVectorPacked<float, 2>;
using DVectorPacked3f = typename  DVectorPacked<float, 3>;
using DVectorPacked4f = typename  DVectorPacked<float, 4>;

struct V16
{
	static constexpr uint32_t v = 16;
};

struct V32
{
	static constexpr uint32_t v = 32;
};

template<typename T, uint32_t Width, uint32_t N, uint32_t NLines, uint32_t NPerLane = Width/sizeof(T), typename __mT = __m_t<T, Width>>
struct alignas(Width) SIMDLines
{
public:
	using xmm_type = typename __mT;
	__mT lines[NLines];

	template<uint32_t NV>
	SIMDLines(const SIMDLines<T, Width, NV, NLines>& _l)
	{
		std::memcpy(lines, _l.lines, NLines*widthBytes());
	}

	SIMDLines() = default;

	SIMDLines(__mT l[NLines])
	{
		std::memcpy(lines, l, NLines*widthBytes());
	}

	template<TTNumbrEqual(NLines, 1)>
	SIMDLines(__mT l) 
	{
		lines[0] = l;
	}

	const T& at(uint32_t iLine, uint32_t jElement) const
	{
		const T* line = reinterpret_cast<const T*>(&lines[iLine]);
		return line[jElement];
	}

	T& at(uint32_t iLine, uint32_t jElement)
	{
		T* line = reinterpret_cast<T*>(&lines[iLine]);
		return line[jElement];
	}

	//T& operator[](uint32_t i)
	//{
	//	T* line = reinterpret_cast<T*>(&lines[i / width()]);
	//	return line[i%width()];
	//}

	//const T& operator[](uint32_t i) const
	//{
	//	const T* line = reinterpret_cast<const T*>(&lines[i / width()]);
	//	return line[i%width()];
	//}

	void loadu(const T* d)
	{
		for (uint32_t i = 0; i < size(); i++, d += width())
		{
			lines[i] = _mm_loadu_t<__mT>(d);
		}
	}

	void loada(const T* d)
	{
		for (uint32_t i = 0; i < size(); i++, d += width())
		{
			lines[i] = _mm_loada_t<__mT>(d);
		}
	}

	template<uint32_t NMask>
	T inprod() const
	{
		constexpr uint32_t nUnMaskedLines = NMask/ NPerLane;

		__mT r;
		if constexpr (nUnMaskedLines == 1)
		{
			r = lines[0];
		}
		else
		{
			r = zeroXMM<__mT, T>();
			for (uint32_t i = 0; i < nUnMaskedLines; i++)
			{
				r = _mm_add_t(r, lines[i]);
			}
		}
		

		if constexpr (nUnMaskedLines != NLines)
		{
			static  __mT zero = zeroXMM<__mT, T>();
			constexpr uint32_t NLaneMask = (NMask%NPerLane == 0) ? NPerLane : NMask%NPerLane;
			static constexpr uint32_t mask = mask_gen<NLaneMask>();

			__mT residualLine = _mm_blend_ts<mask>::f(zero, lines[NLines - 1]);
			r = _mm_add_t(r, residualLine);
		}

		T res = 0;
		T* d = reinterpret_cast<T*>(&r);
		for (uint32_t i = 0; i < width(); i++)
		{
			res += d[i];	
		}
		//T res = _mm_inprod_t(r);
		return res;
	}

	template<typename K>
	void loadElementWise(K* d)
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			for (uint32_t k = 0; k < width(); k++, d++)
			{
				at(i, k) = static_cast<T>(*d);
			}
		}
	}

	void storea(T* d) const
	{
		for (uint32_t i = 0; i < size(); i++, d += widthBytes())
		{
			_mm_storea_t<T, __mT>(d, lines[i]);
		}
	}

	void storeu(T* d) const
	{
		for (uint32_t i = 0; i < size(); i++, d += widthBytes())
		{
			_mm_storeu_t<T, __mT>(d, lines[i]);
		}
	}

	void set1(T v)
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			lines[i] = _mm_set1_t<__mT,T>(v);
		}
	}

	void setr(uint32_t i, __mT xmm)
	{
		lines[i] = xmm;
	}

	template<TTNumbrEqual(NPerLane, 4)>
	void setr(uint32_t i, T m0, T m1, T m2, T m3)
	{
		lines[i] = _mm_setr_t(m0, m1, m2, m3);
	}

	template<TTNumbrEqual(NPerLane, 8)>
	void setr(uint32_t i, T m0, T m1, T m2, T m3, T m4, T m5, T m6, T m7)
	{
		lines[i] = _mm_setr_t(m0, m1, m2, m3, m4, m5, m6, m7);
	}

	template<TTNumbrEqual(NPerLane, 4)>
	void setr(T m0, T m1, T m2, T m3)
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			lines[i] = _mm_setr_t(m0, m1, m2, m3);
		}
	}

	template<TTNumbrEqual(NPerLane, 8)>
	void setr(T m0, T m1, T m2, T m3, T m4, T m5, T m6, T m7)
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			lines[i] = _mm_setr_t(m0, m1, m2, m3, m4, m5, m6, m7);
		}
	}

	const __mT& operator[](uint32_t i) const
	{
		return lines[i];
	}

	__mT& operator[](uint32_t i)
	{
		return lines[i];
	}

	void cmpEql(const SIMDLines& l, const SIMDLines& r)
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			lines[i] = _mm_cmp_eq_t(l[i], r[i]);
		}
	}

	template<uint32_t cmpType>
	void cmp(const SIMDLines& l, const SIMDLines& r)
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			lines[i] = _mm_cmp_ts<cmpType>::f(l[i], r[i]);
		}
	}

	void cndLoad(const SIMDLines& predicate, const SIMDLines& trueV, const SIMDLines& falseV)
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			lines[i] = _mm_predicate_load_t(predicate[i], trueV[i], falseV[i]);
		}
	}

	bool isEql(const SIMDLines& r) const
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			__mT mask  = _mm_cmp_eq_t(lines[i], r[i]);
			unsigned bitmask = _mm_movemask_t(mask);
			if (i == size() - 1)
			{
				constexpr unsigned m = ((~mask_gen<N%NPerLane>())&mask_gen<NPerLane>());
				bitmask |= m;
			}
			if (bitmask != mask_gen<NPerLane>())
			{
				return false;
			}
		}

		return true;
	}

	void minv(const SIMDLines& r)
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			lines[i] = _mm_min_t<T, __mT>(lines[i], r[i]);
		}
	}

	void maxv(const SIMDLines& r)
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			lines[i] = _mm_max_t<T, __mT>(lines[i], r[i]);
		}
	}

	void ceil()
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			lines[i] = _mm_ceil_t<__mT>(lines[i]);
		}
	}

	void floor()
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			lines[i] = _mm_floor_t<__mT>(lines[i]);
		}
	}

	void sqrt()
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			lines[i] = _mm_sqrt_t<T, __mT>(lines[i]);
		}
	}

	void exp()
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			for (uint32_t j = 0; j < width(); j++)
			{
				at(i, j) = std::exp(at(i, j));
			}
		}
	}


	void abs()
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			lines[i] = _mm_abs_t<T, __mT>(lines[i]);
		}
	}

	void add(const SIMDLines& r)
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			lines[i] = _mm_add_t(lines[i], r[i]);
		}
	}

	void mad(const SIMDLines& m, const SIMDLines& a)
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			lines[i] = _mm_madd_t<T, __mT>(lines[i], m[i], a[i]);
		}
	}

	void sub(const SIMDLines& r)
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			lines[i] = _mm_sub_t<T, __mT>(lines[i], r[i]);
		}
	}

	void mul(const SIMDLines& r)
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			lines[i] = _mm_mul_t(lines[i], r[i]);
		}
	}


	template<TTIsFloating(T)>
	void div(const SIMDLines& r)
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			lines[i] = _mm_div_t<T, __mT>(lines[i], r[i]);
		}
	}	

	template<TTIsInteger(T)>
	void div(const SIMDLines& r)
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			for (uint32_t k = 0; k < width(); k++)
			{
				at(i, k) /= r.at(i, k);
			}
		}
	}

	void insert(uint32_t iLine, uint32_t pos, T value)
	{
		lines[iLine] = _mm_insert_t(lines[iLine], value, pos);
	}

	template<uint32_t controlValue>
	void permuteEach()
	{
		for (uint32_t i = 0; i < size(); i++)
		{
			lines[i] = _mm_permute_ts<controlValue>::f(lines[i]);
		}
	}

	template<uint32_t controlValue>
	inline void permute(uint32_t i)
	{
		lines[i] = _mm_permute_ts<controlValue>::f(lines[i]);
	}
	
	inline T* data(){ return reinterpret_cast<T*>(lines);}
	inline const T* data() const{ return reinterpret_cast<const T*>(lines);}


	static constexpr uint32_t widthBytes() { return Width; } 

	static constexpr uint32_t width()  { return Width/sizeof(T); }

	static constexpr uint32_t size() { return NLines; }
};


template<bool Exist32, bool Exist16, typename T, uint32_t N32, uint32_t N32Lines, uint32_t N16, uint32_t N16Lines>
struct SIMDLinesUnion
{
};

template<typename T, uint32_t N32, uint32_t N32Lines, uint32_t N16, uint32_t N16Lines>
struct SIMDLinesUnion<true, true, typename T, N32, N32Lines, N16, N16Lines>
{
	using SIMDLines32W = typename SIMDLines<T, 32, N32, N32Lines>;
	using SIMDLines16W = typename SIMDLines<T, 16, N16, N16Lines>;

	SIMDLines32W l32;
	SIMDLines16W l16;
};

template<typename T, uint32_t N32, uint32_t N32Lines, uint32_t N16, uint32_t N16Lines>
struct SIMDLinesUnion<false, true, typename T, N32, N32Lines, N16, N16Lines>
{
	using SIMDLines16W = typename SIMDLines<T, 16, N16, N16Lines>;
	SIMDLines16W l16;
};

template<typename T, uint32_t N32, uint32_t N32Lines, uint32_t N16, uint32_t N16Lines>
struct SIMDLinesUnion<true, false, typename T, N32, N32Lines, N16, N16Lines>
{
	using SIMDLines32W = typename SIMDLines<T, 32, N32, N32Lines>;
	SIMDLines32W l32;
};


template<
	typename T,
	uint32_t Size,
	uint32_t N32Lines = (Size + (32 / sizeof(T))/2-1) / (32/sizeof(T)),
	uint32_t N32 = (N32Lines == 0) ? 0 : Size / (32 / sizeof(T))*(32 / sizeof(T)) + (Size - Size / (32 / sizeof(T))*(32 / sizeof(T)) + (32 / sizeof(T)) / 2 - 1) / (32 / sizeof(T))*(Size - Size / (32 / sizeof(T))*(32 / sizeof(T))),
	uint32_t N16Lines = (Size - N32 + (16 / sizeof(T)) - 1) / (16/sizeof(T)),
	uint32_t N16 = Size - N32,
	bool Exist32 = N32Lines != 0,
	bool Exist16 = N16Lines != 0,
	uint32_t N32Width = 32/sizeof(T),
	uint32_t N16Width = 16/sizeof(T),
	uint32_t ALIGN = (Exist32) ? 32 : 16>
class alignas(ALIGN) DVector
{
public:
	using SIMDLinesUnionB = typename SIMDLinesUnion<Exist32, Exist16, T, N32, N32Lines, N16, N16Lines>;

	mutable SIMDLinesUnionB lines;

#define EXIST_L32_ONLY if constexpr (Exist32 && !Exist16)
#define EXIST_L16_ONLY if constexpr (Exist16 && !Exist32)
#define EXIST_L32 if constexpr (Exist32)
#define EXIST_L16 if constexpr (Exist16)
#define EXIST_L32_L16 if constexpr (Exist16 && Exist32)

	template<uint32_t NMask>
	T inprod() const
	{
		EXIST_L32_L16
		if constexpr (NMask > N32 && NMask <= N16+N32)
		{
			return lines.l32.inprod<N32>() + lines.l16.inprod<NMask-N32>();
		}

		EXIST_L32
		if constexpr (NMask <= N32)
		{
			return lines.l32.inprod<NMask>();
		}

		EXIST_L16_ONLY
		if constexpr (NMask <= N16)
		{
			return lines.l16.inprod<NMask>();
		}
		assert(false);
	}

	void add(const DVector& v)
	{
		EXIST_L32
		lines.l32.add(v.lines.l32);

		EXIST_L16
		lines.l16.add(v.lines.l16);
	}

	template<TTNumbrEqual(N32Width, 8)>
	void set(uint32_t i, T m0, T m1, T m2, T m3, T m4, T m5, T m6, T m7)
	{
		lines.l32.setr(i, m0, m1, m2, m3, m4, m5, m6, m7);
	}


	template<typename __mTv>
	void set(uint32_t i, __mTv xmm)
	{
		EXIST_L32
		{
		if constexpr (std::is_same_v<decltype(lines.l32)::xmm_type, __mTv>)
		{
			lines.l32.setr(i, xmm);
			return;
		}
		}

		EXIST_L16
		{
			if constexpr(std::is_same_v<decltype(lines.l16)::xmm_type, __mTv>)
			{
				lines.l16.setr(i, xmm);
				return;
			}
		}
	}

	void set(uint32_t i, T x, T y, T z, T w)
	{
		EXIST_L32
		{
			lines.l32.setr(i, x, y, z, w);
			return;
		}

		EXIST_L16
		{
			lines.l16.setr(i, x, y, z, w);
			return;
		}
	}



	void sub(const DVector& v)
	{
		EXIST_L32
		lines.l32.sub(v.lines.l32);

		EXIST_L16
		lines.l16.sub(v.lines.l16);
	}

	void mul(const DVector& v)
	{
		EXIST_L32
		lines.l32.mul(v.lines.l32);

		EXIST_L16
		lines.l16.mul(v.lines.l16);
	}

	void div(const DVector& v)
	{
		EXIST_L32
		lines.l32.div(v.lines.l32);

		EXIST_L16
		lines.l16.div(v.lines.l16);
	}

	void set1(T broadcastValue)
	{
		EXIST_L32
		lines.l32.set1(broadcastValue);

		EXIST_L16
		lines.l16.set1(broadcastValue);
	}

	template<typename K>
	void loadElementWise(const K* d)
	{
		EXIST_L32
		{
			lines.l32.loadElementWise(d);
			d += lines.l32.size()*lines.l32.width();
		}

		EXIST_L16
		lines.l16.loadElementWise(d);
	}

	void loadu(const T* d)
	{
		EXIST_L32
		{
			lines.l32.loadu(d);
			d += lines.l32.size()*lines.l32.width();
		}

		EXIST_L16
		lines.l16.loadu(d);
	}

	void loada(const T* d)
	{
		EXIST_L32
		{
			lines.l32.loada(d);
			d += lines.l32.size()*lines.l32.width();
		}

		EXIST_L16
		lines.l16.loada(d);
	}

	void storeu(T* d) const
	{

		EXIST_L32
		{
			lines.l32.storeu(d);
			d += lines.l32.size()*lines.l32.width();
		}

		EXIST_L16
		lines.l16.storeu(d);
	}

	void storea(T* d) const
	{
		EXIST_L32
		{
			lines.l32.storea(d);
			d += lines.l32.size()*lines.l32.width();
		}

		EXIST_L16
		lines.l16.storea(d);
	}


	void insert(uint32_t i, T v) const
	{
		EXIST_L32_ONLY
		{
			uint32_t iLine = i / lines.l32.width();
			uint32_t offset = i % lines.l32.width();
			lines.l32.insert(iLine, offset, v);
		}

		EXIST_L16_ONLY
		{
			uint32_t iLine = i / lines.l16.width();
			uint32_t offset = i % lines.l16.width();
			lines.l16.insert(iLine, offset, v);
		}

		EXIST_L32_L16
		{
			uint32_t iLine = i / lines.l32.width();
			if (iLine > lines.l32.size())
			{
				i -= lines.l32.size()*lines.l32.width();
				uint32_t iLine = i / lines.l16.width();
				uint32_t offset = i % lines.l16.width();
				lines.l16.insert(iLine, offset, v);
			}
			else
			{
				uint32_t offset = i % lines.l32.width();
				lines.l32.insert(iLine, offset, v);
			}
		}	
	}

	DVector(const DVectorPacked<T, Size>& vPacked)
	{
		loadu(vPacked.data());
	}

	DVector(T* d)
	{
		loadu(d);
	}

	DVector(T broadcastValue = 0)
	{
		set1(broadcastValue);
	}
	
	template<typename T2>
	inline operator DVector<T2, Size>() const
	{
		DVector<T2, Size> res;
		res.loadElementWise(data());
		return res;
	}


	inline T* data()
	{
		EXIST_L32
		return lines.l32.data();
		EXIST_L16
		return lines.l16.data();
	}

	inline const T* data() const
	{
		EXIST_L32
		return lines.l32.data();
		EXIST_L16
		return lines.l16.data();
	}

	inline bool operator==(T value) const
	{
		DVector s = DVector(value);
		return (*this == s);
	}

	inline bool operator==(const DVector& v) const
	{

		bool r = true;
		EXIST_L32
		{
			r = lines.l32.isEql(v.lines.l32);
			if (!r)
			{
				return r;
			}
		}

		EXIST_L16
		r = lines.l16.isEql(v.lines.l16);
		return r;
	}

	inline bool operator!=(const DVector& v2) const
	{
		return !((*this) == v2);
	}

	inline DVector operator+(const DVector& v) const
	{
		DVector r = *this;
		r.add(v);
		return r;
	}

	inline DVector & operator+=(const DVector& v)
	{
		add(v);
		return *this;
	}

	inline DVector operator-(const DVector& v) const
	{
		DVector r = *this;
		r.sub(v);
		return r;
	}

	inline DVector& operator-=(const DVector& v)
	{
		sub(v);
		return *this;
	}

	inline DVector operator*(T s) const
	{
		DVector sVector = DVector(s);
		return (*this)*sVector;
	}

	inline DVector& operator*=(T s)
	{
		DVector sVector = DVector(s);
		*this *= sVector;
		return *this;
	}

	inline DVector operator*(const DVector& v) const
	{
		DVector r = *this;
		r.mul(v);
		return r;
	}

	inline DVector& operator*=(const DVector& v)
	{
		mul(v);
		return *this;
	}

	inline DVector operator/(T s) const
	{
		assert(s != 0);
		return (*this) / DVector<T, Size>(s);
	}

	inline DVector& operator/=(T s)
	{
		assert(s != 0);
		*this /= DVector<T, Size>(s);
		return *this;
	}

	inline DVector operator/(const DVector& v) const
	{
		DVector r = *this;
		r.div(v);
		return r;
	}

	inline DVector& operator/=(const DVector& v)
	{
		div(v);
		return *this;
	}

	inline DVector operator-() const
	{
		return (*this)*(DVector(-1));
	}


	inline T operator[](uint32_t i) const
	{
		return (data())[i];
	}

	inline T& operator[](uint32_t i)
	{
		return (data())[i];
	}



	
	//template<TTNumbrEqual(N16Lines, 0)>
	//DVector(SIMDLinesUnionB::SIMDLines32W lines32):
	//	lines.l32(std::move(lines32))
	//{}

	//template<TTNumbrEqual(N32Lines, 0)>
	//DVector(SIMDLinesUnionB::SIMDLines16W lines16):
	//	lines.l16(std::move(lines32))
	//{}

	template<TTNumbrEqual(Size, 4)>
	DVector(T x, T y, T z, T w)
	{
		set(0, x, y, z, w);
	}

	template<TTNumbrEqual(Size, 8)>
	DVector(T m0, T m1, T m2, T m3, T m4, T m5, T m6, T m7)
	{
		set(0, m0, m1, m2, m3, m4, m5, m6, m7);
	}


	template<TTNumbrEqual(Size, 3)>
	DVector(T x, T y, T z)
	{
		set(0, x, y, z, 0);
	}



	template<TTNumbrEqual(Size, 2)>
	DVector(T x, T y)
	{
		set(0, x, y, 0, 0);
	}

	template<TTNumbrEqual(Size, 4)>
	explicit DVector(const DVector<T, 3>& v, T w=0)
	{
		EXIST_L32
		lines.l32 = v.lines.l32;
		EXIST_L16
		lines.l16 = v.lines.l16;
		insert(3, w);
	}

	template<TTNumbrEqual(Size, 4)>
	explicit DVector(DVector<T, 3>&& v, T w = 0)
	{
		EXIST_L32
		lines.l32 = std::move(v.lines.l32);
		EXIST_L16
		lines.l16 = std::move(v.lines.l16);
		insert(3, w);
	}


	template<TTNumbrEqual(Size, 3)>
	explicit DVector(const DVector<T, 4>& v)
	{
		EXIST_L32
		lines.l32 = v.lines.l32;
		EXIST_L16
		lines.l16 = v.lines.l16;
		insert(3, 0);
	}


	template<TTNumbrEqual(Size, 3)>
	explicit DVector(DVector<T, 4>&& v)
	{
		EXIST_L32
		lines.l32 = std::move(v.lines.l32);
		EXIST_L16
		lines.l16 = std::move(v.lines.l16);
		insert(3, 0);
	}

	
	inline void setX(T value) { insert(0, value); }

	template<typename check = std::enable_if_t<Size >= 2>>
	inline void setY(T value) { insert(1, value); }

	template<typename check = std::enable_if_t<Size >= 3>>
	inline void setZ(T value) { insert(2, value); }

	template<typename check = std::enable_if_t<Size >= 4>>
	inline void setW(T value) { insert(3, value); }

	inline T x() const
	{
		return (*this)[0];
	}

	template<typename = std::enable_if_t<Size >= 2>>
	inline T y() const
	{
		return (*this)[1];
	}

	template<typename = std::enable_if_t<Size >= 3>>
	inline T z() const
	{
		return (*this)[2];
	}

	template<typename = std::enable_if_t<Size >= 4>>
	inline T w() const
	{
		return (*this)[3];
	}

	inline T& x()
	{
		return (*this)[0];
	}

	template<typename = std::enable_if_t<Size >= 2>>
	inline T& y()
	{
		return (*this)[1];
	}

	template<typename = std::enable_if_t<Size >= 3>>
	inline T& z()
	{
		return (*this)[2];
	}

	template<typename = std::enable_if_t<Size >= 4>>
	inline T& w()
	{
		return (*this)[3];
	}

	inline constexpr uint32_t size() const
	{
		return Size;
	}

	void insertUnsafe(uint32_t pos, T v) const
	{
		insert(pos, v);
	}

	template<TTNumbrEqual(Size, 3)>
	inline T area() const
	{
		DVector d2 = this->permuteEach<0b11001001>(); // y z x w
		return 2 * dot(*this, d2);
	}

	template<TTNumbrEqual(Size, 2)>
	inline T area() const
	{
		return this->x()*this->y();
	}

	template<uint32_t LSize = Size>
	inline T length2() const
	{
		return dot<LSize>(*this, *this);
	}

	template<uint32_t LSize = Size>
	inline T length() const
	{
		return std::sqrt(length2());
	}


	inline void normalize()
	{
		(*this) /= this->length();
	}

	template<uint32_t controlValue>
	inline DVector permuteEach() const
	{
		DVector v = *this;

		EXIST_L32
		v.lines.l32.permuteEach<controlValue>();

		EXIST_L16
		v.lines.l16.permuteEach<controlValue>();

		return v;
	}

	bool isZero() const
	{
		return (*this == 0);
	}


public:
	inline static DVector abs(const DVector& v)
	{
		DVector res = v;

		EXIST_L32
		res.lines.l32.abs();

		EXIST_L16
		res.lines.l16.abs();

		return res;
	}


	template<uint32_t CMPT>
	inline static DVector cmp(const DVector& l, const DVector& r)
	{
		DVector res;

		EXIST_L32
		res.lines.l32.cmp<CMPT>(l.lines.l32, r.lines.l32);

		EXIST_L16
		res.lines.l16.cmp<CMPT>(l.lines.l16, r.lines.l16);

		return res;
	}

	
	inline static DVector cndLoad(const DVector& predicate, const DVector& trueV, const DVector& falseV)
	{
		DVector r;

		EXIST_L32
		r.lines.l32.cndLoad(predicate.lines.l32, trueV.lines.l32, falseV.lines.l32);

		EXIST_L16
		r.lines.l16.cndLoad(predicate.lines.l16, trueV.lines.l16, falseV.lines.l16);

		return r;
	}


	inline static DVector clamp(const DVector& v, DVector low, DVector high)
	{
		
		DVector maskGreaterLow = cmp<_CMP_GE_OQ>(v, low);
		DVector maskLessHigh = cmp<_CMP_LE_OQ>(v, high);

		DVector res = cndLoad(maskGreaterLow, v, low);
		res = cndLoad(maskLessHigh, res, high);
		return res;
	}

	inline static T absDot(const DVector& v1, const DVector& v2)
	{
		return std::abs(dot(v1, v2));
	}


	//template<TTNumbrEqual(Size, 3)>
	inline static DVector cross(const DVector& v1, const DVector& v2)
	{
		DVector v1p = v1.permuteEach<0b11001001>(); // Y Z X W
		DVector v2p = v2.permuteEach<0b11010010>(); // Z X Y W

		DVector v3 = v1p * v2p;
		v1p = v1p.permuteEach<0b11001001>(); // Z X Y W
		v2p = v2p.permuteEach<0b11010010>(); // Y Z X W

		DVector v4 = v1p * v2p;
		return v3 - v4;
	}

	inline static DVector lerp(const DVector& v1, const DVector& v2, const DVector& t)
	{
		DVector tInv = DVector(1) - t;
		return mad(v1, tInv, v2 * t);
	}

	inline static DVector lerp(const DVector& v1, const DVector& v2, T t)
	{
		return lerp(v1, v2, DVector(t));
	}

	
	inline static T minComponent(const DVector& v)
	{
		T m = v[0];
		for (uint32_t i = 1; i < Size; ++i)
		{
			if (m > v[i])
			{
				m = v[i];
			}
		}
		return m;
	}


	inline static bool isSameHemisphere(const DVector &w, const DVector &wp)
	{
		return w.z() * wp.z() > 0;
	}

	inline static DVector permuteEach(const DVector &v, const DVector& indices)
	{
		DVector res;
		for (uint32_t i = 0; i < Size; i++)
		{
			res[i] = v[indices[i]];
		}
		return res;
	}

	inline static uint32_t maxDimension(const DVector& v)
	{
		T m = v[0];
		uint32_t iM = 0;
		for (uint32_t i = 1; i < Size; ++i)
		{
			if (m < v[i])
			{
				m = v[i];
				iM = i;
			}
		}
		return iM;
	}

	
	inline static T maxComponent(const DVector& v)
	{
		T m = v[0];
		for (uint32_t i = 1; i < Size; ++i)
		{
			if (m < v[i])
			{
				m = v[i];
			}
		}
		return m;
	}

	
	inline static DVector ceil(const DVector& v)
	{
		DVector r = v;

		EXIST_L32
		r.lines.l32.ceil();
		EXIST_L16
		r.lines.l16.ceil();
		return r;
	}

	inline static DVector exp(const DVector& v)
	{
		DVector r = v;
		EXIST_L32
		r.lines.l32.exp();
		EXIST_L16
		r.lines.l16.exp();
		return r;
	}

	
	inline static DVector floor(const DVector& v)
	{
		DVector r = v;
		EXIST_L32
		r.lines.l32.floor();
		EXIST_L16
		r.lines.l16.floor();
		return r;
	}
	
	inline static DVector reflect(const DVector& wo, const DVector& n)
	{
		return -wo +  n*(2* dot(wo, n));
	}

	
	inline static DVector sqrt(const DVector& v)
	{
		DVector r = v;
		EXIST_L32
		r.lines.l32.sqrt();
		EXIST_L16
		r.lines.l16.sqrt();
		return r;
	}


	
	inline static DVector mad(const DVector& v1, float s, const DVector& v2)
	{
		return mad(v1, DVector(s), v2);
	}
	
	inline static DVector mad(const DVector& v1, const DVector& v2, const DVector& v3)
	{
		DVector r = v1;
		EXIST_L32
		r.lines.l32.mad(v2.lines.l32, v3.lines.l32);
		EXIST_L16
		r.lines.l16.mad(v2.lines.l16, v3.lines.l16);
		return r;
	}
	
	inline static DVector normalize(const DVector& v)
	{
		return v / v.length();
	}
	
	inline static DVector minv(const DVector& v1, const DVector& v2)
	{
		DVector r = v1;
		EXIST_L32
		r.lines.l32.minv(v2.lines.l32);
		EXIST_L16
		r.lines.l16.minv(v2.lines.l16);
		return r;
	}
	
	inline static DVector maxv(const DVector& v1, const DVector& v2)
	{
		DVector r = v1;
		EXIST_L32
		r.lines.l32.maxv(v2.lines.l32);
		EXIST_L16
		r.lines.l16.maxv(v2.lines.l16);
		return r;
	}

	template<TTNumbrEqual(Size, 3)>
	inline static void makeBasisByVector(const DVector& v1, DVector& v2, DVector& v3)
	{

		if (std::abs(v1.x()) > std::abs(v1.y()))
		{
			v2 = DVector(-v1.z(), 0, v1.x());
		}
		else
		{
			v2 = DVector(0, v1.z(), -v1.y());
		}

		v2 = v2.normalize(v2);;
		v3 = cross(v1, v2);
	}

	template<uint32_t DotSize=Size>
	inline static T dot(const DVector& v1, const DVector& v2) 
	{
		DVector resV = v1* v2;
		T res = resV.inprod<DotSize>();
		return res;
	}
};



template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> abs(const DVector<VT, VSize>& v)
{
	return DVector<VT, VSize>::abs(v);
}

template<uint32_t DotSize, typename T>
inline auto dot(const T& v1, const T& v2)
{
	return T::dot<DotSize>(v1, v2);
}

template<typename VT, uint32_t VSize>
inline VT dot(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2)
{
	return DVector<VT, VSize>::dot(v1, v2);
}

template<typename VT, uint32_t VSize, typename T2, typename T3>
inline auto clamp(const DVector<VT, VSize>& v, const T2& low, const T3& high)
{
	return  DVector<VT, VSize>::clamp(v, (DVector<VT, VSize>)low, (DVector<VT, VSize>)high);
}
//
//template<typename VT, uint32_t VSize>
//inline DVector<VT, VSize> clamp(const DVector<VT, VSize>& v, DVector<VT, VSize> low, DVector<VT, VSize> high = DVector<VT, VSize>(std::numeric_limits<float>::infinity()))
//{
//	return DVector<VT, VSize>::clamp(v, low, high);
//}
//
//template<typename VT, uint32_t VSize>
//inline DVector<VT, VSize> clamp(const DVector<VT, VSize>& v, VT low, DVector<VT, VSize> high = DVector<VT, VSize>(std::numeric_limits<float>::infinity()))
//{
//	return clamp(v, DVector<VT, VSize>(low), high);
//}
//
//template<typename VT, uint32_t VSize>
//inline DVector<VT, VSize> clamp(const DVector<VT, VSize>& v, DVector<VT, VSize> low, VT high = DVector<VT, VSize>(std::numeric_limits<float>::infinity()))
//{
//	return clamp(v, low, DVector<VT, VSize>(high));
//}
//
//template<typename VT, uint32_t VSize>
//inline DVector<VT, VSize> clamp(const DVector<VT, VSize>& v, VT low, VT high = std::numeric_limits<float>::infinity())
//{
//	return clamp(v, DVector<VT, VSize>(low), DVector<VT, VSize>(high));
//}

template<typename VT, uint32_t VSize>
inline VT absDot(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2)
{
	return DVector<VT, VSize>::absDot(v1, v2);
}

template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> cross(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2)
{
	return DVector<VT, VSize>::cross(v1, v2);
}

template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> lerp(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2, const DVector<VT, VSize>& t)
{
	return DVector<VT, VSize>::lerp(v1, v2, t);
}


template<typename VT, uint32_t VSize>
inline VT minComponent(const DVector<VT, VSize>& v)
{
	return DVector<VT, VSize>::minComponent(v);
}

template<typename VT, uint32_t VSize>
inline bool isSameHemisphere(const DVector<VT, VSize> &w, const DVector<VT, VSize> &wp)
{
	return DVector<VT, VSize>::isSameHemisphere(w, wp);
}

//template<typename VT, uint32_t VSize>
//inline DVector<VT, VSize> permuteEach(const DVector<VT, VSize> &v, const DVector<uint32_t, VSize>& indices)
//{
//	return DVector<VT, VSize>::permuteEach(v, indices);
//}

template<typename T, typename K>
inline T permuteEach(const T &v, const K& indices)
{
	return T::permuteEach(v, indices);
}


template<typename VT, uint32_t VSize>
inline uint32_t maxDimension(const DVector<VT, VSize>& v)
{
	return DVector<VT, VSize>::maxDimension(v);
}

template<typename VT, uint32_t VSize>
inline VT maxComponent(const DVector<VT, VSize>& v)
{
	return DVector<VT, VSize>::maxComponent(v);
}

template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> ceil(const DVector<VT, VSize>& v)
{
	return DVector<VT, VSize>::ceil(v);
}

template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> floor(const DVector<VT, VSize>& v)
{
	return DVector<VT, VSize>::floor(v);
}


template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> exp(const DVector<VT, VSize>& v)
{
	return DVector<VT, VSize>::exp(v);
}

template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> reflect(const DVector<VT, VSize>& wo, const DVector<VT, VSize>& n)
{
	return DVector<VT, VSize>::reflect(wo, n);
}



template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> sqrt(const DVector<VT, VSize>& v)
{
	return DVector<VT, VSize>::sqrt(v);
}

template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> lerp(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2, VT t)
{
	return lerp(v1, v2, DVector<VT, VSize>(t));
}

template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> mad(const DVector<VT, VSize>& v1, float s, const DVector<VT, VSize>& v2)
{
	return DVector<VT, VSize>::mad(v1, s, v2);
}
template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> mad(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2, const DVector<VT, VSize>& v3)
{
	return DVector<VT, VSize>::mad(v1, v2, v3);
}
template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> normalize(const DVector<VT, VSize>& v)
{
	return DVector<VT, VSize>::normalize(v);
}
template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> minv(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2)
{
	return DVector<VT, VSize>::minv(v1, v2);
}
template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> maxv(const DVector<VT, VSize>& v1, const DVector<VT, VSize>& v2)
{
	return DVector<VT, VSize>::maxv(v1, v2);
}



template<typename VT, uint32_t VSize>
inline void makeBasisByVector(const DVector<VT, VSize>& v1, DVector<VT, VSize>& v2, DVector<VT, VSize>& v3)
{

	if (std::abs(v1.x()) > std::abs(v1.y()))
	{
		v2 = DVector<VT, VSize>(-v1.z(), 0, v1.x());
	}
	else
	{
		v2 = DVector<VT, VSize>(0, v1.z(), -v1.y());
	}

	v2 = v2.normalize(v2);;
	v3 = cross(v1, v2);
}


template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> cndLoad(const DVector<VT, VSize>& predicate, const DVector<VT, VSize>& trueV, const DVector<VT, VSize>& falseV)
{
	return DVector<VT, VSize>::cndLoad(predicate, trueV, falseV);
}

template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> cndLoad(const DVector<VT, VSize>& predicate, VT trueV, const DVector<VT, VSize>& falseV)
{
	return cndLoad(predicate, DVector<VT, VSize>(trueV), falseV);
}

template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> cndLoad(const DVector<VT, VSize>& predicate, const DVector<VT, VSize>&  trueV, VT falseV)
{
	return cndLoad(predicate, trueV, DVector<VT, VSize>(falseV));
}

template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> cndLoad(const DVector<VT, VSize>& predicate, VT trueV, VT falseV)
{
	return cndLoad(predicate, DVector<VT, VSize>(trueV), DVector<VT, VSize>(falseV));
}

template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> cmpEQL(const DVector<VT, VSize>& l, const DVector<VT, VSize>& r)
{
	return DVector<VT, VSize>::cmp<0>(l, r);
}

template<typename VT, uint32_t VSize>
inline DVector<VT, VSize> cmpEQL(const DVector<VT, VSize>& l, VT r)
{
	return cmpEQL(l, DVector<VT, VSize>(r));
}


using DVector2f = typename DVector<float, 2>;
using DVector3f = typename  DVector<float, 3>;
using DVector4f = typename  DVector<float, 4>;
using DVector8f = typename  DVector<float, 8>;

using DVector2d = typename  DVector<double, 2>;
using DVector3d = typename  DVector<double, 3>;
using DVector4d = typename  DVector<double, 4>;

using DVector2i = typename  DVector<int32_t, 2>;
using DVector3i = typename  DVector<int32_t, 3>;
using DVector4i = typename  DVector<int32_t, 4>;

using DVector2u = typename  DVector<uint32_t, 2>;
using DVector3u = typename  DVector<uint32_t, 3>;
using DVector4u = typename  DVector<uint32_t, 4>;

template<typename T, uint32_t Size>
class DVectorPacked
{
public:
	std::array<T, Size> arr;

	using UnpackedT = typename DVector<T, Size>;

	DVectorPacked(T v = 0)
	{
		for (uint32_t i = 0; i < Size; i++)
		{
			arr[i] = v;
		}
	}

	DVectorPacked(const DVector<T, Size>& vec)
	{
		vec.storeu(arr.data());
	}



	DVector<T, Size> unpack() const
	{
		return DVector<T, Size>(*this);
	}

	inline T x() const
	{
		return (*this)[0];
	}

	template<typename = std::enable_if_t<Size >= 2>>
	inline T y() const
	{
		return (*this)[1];
	}

	template<typename = std::enable_if_t<Size >= 3>>
	inline T z() const
	{
		return (*this)[2];
	}

	template<typename = std::enable_if_t<Size >= 4>>
	inline T w() const
	{
		return (*this)[3];
	}

	inline T& x()
	{
		return (*this)[0];
	}

	template<typename = std::enable_if_t<Size >= 2>>
	inline T& y()
	{
		return (*this)[1];
	}

	template<typename = std::enable_if_t<Size >= 3>>
	inline T& z()
	{
		return (*this)[2];
	}

	template<typename = std::enable_if_t<Size >= 4>>
	inline T& w()
	{
		return (*this)[3];
	}

	constexpr uint32_t size() const
	{
		return Size;
	}

	const T* data() const
	{
		return &arr[0];
	}

	T* data()
	{
		return &arr[0];
	}

	inline T operator[](uint32_t i) const
	{
		return arr[i];
	}

	inline T& operator[](uint32_t i)
	{
		return arr[i];
	}
};

WML_END
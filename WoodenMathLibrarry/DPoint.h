#pragma once

#include "stdafx.h"
#include "DVector.h"

WML_BEGIN

using namespace TypedSSE;

template<typename T, uint8_t Size>
class DPoint : public DVector<T, Size>
{
public:
	using base = typename DVector<T, Size>;
	
	//using base::operator-();
	//using base::operator+();
	//using base::operator-=();
	//using base::operator+=();
	//using base::operator*();
	//using base::operator*=();
	//using base::operator/();
	//using base::operator/=();


	operator DVector<T, Size>() const
	{
		return *this;
	}



	template<TTNumbrEqual(Size, 3)>
	DPoint(T x=0, T y=0, T z=0) :
		base(x, y, z)
	{
		this->insert(3, (T)1);
	}

	template<TTNumbrEqual(Size, 3)>
	 DPoint(const DVector<T, Size>& v)
	{
		_mm_loada_t<T>(&v.xmm);
		this->insert(3, (T)1);
	}

	template<TTNumbrEqual(Size, 3)>
	 DPoint(DVector<T, 3>&& v) :
		 base(std::move(v.xmm))
	{
		 this->insert(3, (T)1);
	}


	template<TTNumbrEqual(Size, 2)>
	DPoint(T x = 0, T y = 0) :
		base(x, y)
	{
		this->insert(3, (T)1);
	}

	template<TTNumbrEqual(Size, 2)>
	 DPoint(const DVector<T, Size>& v)
	{
		_mm_loada_t<T>(&v.xmm);
		this->insert(3, (T)1);
	}

	template<TTNumbrEqual(Size, 2)>
	 DPoint(DVector<T, 2>&& v) :
		 base(std::move(v.xmm))
	{
		 this->insert(3, (T)1);
	}
};

using DPoint3f = typename DPoint<float, 3>;
using DPoint3i = typename DPoint<int, 3>;
using DPoint3d = typename DPoint<double, 3>;
using DPoint2f = typename DPoint<float, 2>;
using DPoint2i = typename DPoint<int, 2>;
using DPoint2d = typename DPoint<double, 2>;

WML_END


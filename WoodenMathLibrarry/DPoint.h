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


	DPoint(T broadcastValue=0.0f) :
		base(broadcastValue)
	{
		//this->insert(3, (T)1);

	}

	template<typename T2>
	operator DPoint<T2, Size>() const
	{
		DPoint<T2, Size> res;
		for (uint8_t i = 0; i < Size; i++)
		{
			res[i] = (T2)((*this)[i]);
		}
		return res;
	}

	template<TTNumbrEqual(Size, 3)>
	DPoint(T x, T y, T z) :
		base(x, y, z)
	{
		//this->insert(3, (T)1);
	}

	template<TTNumbrEqual(Size, 3)>
	 DPoint(const DVector<T, Size>& v):
		 base(v)
	{
		//this->insert(3, (T)1);
	}

	template<TTNumbrEqual(Size, 3)>
	 DPoint(DVector<T, 3>&& v) :
		 base(std::move(v.xmm))
	{
		 //this->insert(3, (T)1);
	}


	template<TTNumbrEqual(Size, 2)>
	DPoint(T x = 0, T y = 0) :
		base(x, y)
	{
		//this->insert(3, (T)1);
	}

	template<TTNumbrEqual(Size, 2)>
	 DPoint(const DVector<T, Size>& v):
		 base(v)
	{		
		 //this->insert(3, (T)1);
	}

	template<TTNumbrEqual(Size, 2)>
	 DPoint(DVector<T, 2>&& v) :
		 base(std::move(v.xmm))
	{
		 //this->insert(3, (T)1);
	}
};

using DPoint3f = typename DPoint<float, 3>;
using DPoint3i = typename DPoint<int32_t, 3>;
using DPoint3d = typename DPoint<double, 3>;
using DPoint2f = typename DPoint<float, 2>;
using DPoint2i = typename DPoint<int32_t, 2>;
using DPoint2d = typename DPoint<double, 2>;

WML_END


/*
* Phoenix 3D ���� Version 2.0
*
* Copyright (C) 2009-2011 http://www.Phoenix3d.org/
*
* �ļ�����	��	PX2Any.hpp
*
* �汾		:	1.0 (2011/03/19)
*
* ����		��	more
*
*/

#ifndef PX2ANY_HPP
#define PX2ANY_HPP

#include "PX2CorePrerequisites.hpp"

namespace PX2
{

	class Any
	{
	public:
		// ���죬����
		Any()
			:
		mContent(0)
		{
		}

		template<typename ValueType>
		Any (const ValueType &value)
			: 
		mContent(new Holder<ValueType>(value))
		{
		}

		Any (const Any &other)
			:
		mContent(other.mContent ? other.mContent->Clone() : 0)
		{
		}

		~Any()
		{
			delete mContent;
		}

		// ����
		Any &Swap(Any & rhs)
		{
			std::swap(mContent, rhs.mContent);
			return *this;
		}

		template<typename ValueType>
		Any &operator=(const ValueType &rhs)
		{
			Any(rhs).Swap(*this);
			return *this;
		}

		Any & operator=(Any rhs)
		{
			rhs.Swap(*this);
			return *this;
		}

		bool IsEmpty() const
		{
			return !mContent;
		}

		const std::type_info &type() const
		{
			return mContent ? mContent->type() : typeid(void);
		}

	public_internal:
		// types
		class PlaceHolder
		{
		public:
			virtual ~PlaceHolder()
			{
			}

		public:
			virtual const std::type_info & type() const = 0;
			virtual PlaceHolder * Clone() const = 0;
		};

		template<typename ValueType>
		class Holder : public PlaceHolder
		{
		public:
			Holder(const ValueType &value)
				: mHeld(value)
			{
			}

			virtual const std::type_info &type() const
			{
				return typeid(ValueType);
			}

			virtual PlaceHolder *Clone() const
			{
				return new Holder(mHeld);
			}

		public:
			ValueType mHeld;

		private:
			// ��������ʵ��
			Holder & operator=(const Holder &);
		};

	public_internal:
		PlaceHolder * mContent;
	};

	template<typename ValueType>
	ValueType *Any_Cast(Any * operand)
	{
		return operand && operand->type() == typeid(ValueType) ?
			&static_cast<Any::Holder<ValueType> *>(operand->mContent)->mHeld
			: 0;
	}

	template<typename ValueType>
	inline const ValueType *Any_Cast(const Any *operand)
	{
		return Any_Cast<ValueType>(const_cast<Any *>(operand));
	}

}

#endif
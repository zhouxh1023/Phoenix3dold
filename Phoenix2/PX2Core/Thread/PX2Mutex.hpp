/*
* Phoenix 3D 引擎 Version 2.0
*
* Copyright (C) 2009-2011 http://www.Phoenix3d.org/
*
* 文件名称	：	PX2Mutex.hpp
*
* 版本		:	1.0 (2011/01/30)
*
* 作者		：	more
*
*/

#ifndef PX2MUTEX_HPP
#define PX2MUTEX_HPP

#include "PX2CorePrerequisites.hpp"
#include "PX2MutexType.hpp"

namespace PX2
{
	
	/// 线程锁类
	class PX2_CORE_ENTRY Mutex
	{
	public:
		Mutex ();
		~Mutex ();

		void Enter ();
		void Leave ();

	private:
		MutexType mMutex;
	};

}
#endif
///<------------------------------------------------------------------------------
//< @file:   server\basic\singleton.h
//< @author: hongkunan
//< @date:   2015年1月13日 16:2:11
//< @brief:	 单例模式
//< Copyright (c) 2015 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _singleton_h_
#define _singleton_h_

#include "lock.h"

// 单例类
// 比如：若希望某个类A拥有单例方法，则可定义class A : public Singleton<A>，便可调用A::instance()
template <typename T>
class Singleton
{
private:
	struct born {
		born() { Singleton<T>::Instance(); }
	};

public:
	// 该单例方法是线程安全的
	static T& Instance()
	{
		if (NULL == instance) {
			lock_guard_t<> lock(s_lock);
			if (NULL == instance) {
				T *t = new T;
				instance = t;
			}
		}

		return *instance;
	}

private:
	static born s_born;
	static mutex_t s_lock;

public:
	static T *instance;
};

template <typename T>
typename Singleton<T>::born Singleton<T>::s_born;

template <typename T>
T* Singleton<T>::instance = NULL;

template <typename T>
mutex_t Singleton<T>::s_lock;

#endif //_singleton_h_


/* 线程安全版

///<------------------------------------------------------------------------------
//< @file:   server\basic\singleton.h
//< @author: hongkunan
//< @date:   2015年1月13日 16:2:11
//< @brief:
//< Copyright (c) 2015 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _singleton_h_
#define _singleton_h_

// 单例<线程安全>
template <typename T>
class Singleton
{
struct object_creator {
object_creator() { Singleton<T>::instance(); }
};

static object_creator create_object;

public:
static T* instance()
{
static T obj;
return &obj;
}

static void init()
{
t = new T();
}

#ifdef WIN
static T& instance()
{
pthread_once(&ponce_, &Singleton::init);
if(InterlockedCompareExchange((uint32_t*)once, 1, 0) == 0) {
init();
}

return *t;
}

private:
static uint32 ponce;
static T* t;
#else
static T& instance()
{
pthread_once(&ponce_, &Singleton::init);
return *t;
}

private:
static pthread_once_t ponce;
static T* t;
#endif
};

template <typename T>
typename Singleton<T>::object_creator Singleton<T>::create_object;

#ifdef WIN
template<typename T>
uint32 Singleton<T>::ponce = 1;

template<typename T>
T* Singleton<T>::value_ = NULL;
#else
template<typename T>
pthread_once_t Singleton<T>::ponce = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::value_ = NULL;
#endif

#endif //_singleton_h_

*/
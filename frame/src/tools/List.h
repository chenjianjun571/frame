///************************************************************
/// @Copyright (C), 1993-2012, bafei  Information Technologies Co., Ltd.
/// @URL            
/// @file           src/frame/tools/List.h
/// @brief          对STL的list进行封装，以解决在使用size成员函数时，cpu高的问题
/// @attention      
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2013年01月08日
/// @Description    
/// @History        
///************************************************************

#ifndef __LIST_H_FILE__
#define __LIST_H_FILE__

#include <list>


/// @class CList
/// @brief 对stl的list进行封装
template<class T>
class CList :	public std::list<T>
{
public:
    typedef std::list<T> father;
    typedef typename father::iterator iterator;

private:
    unsigned int m_num;
public:
    CList(void) { m_num=0; }

    ~CList(void) {}

    void push_back(const T& _Val)
    {
        father::push_back (_Val);
        ++m_num;
    }

	void push_front(const T& _Val)
	{
		father::push_front(_Val);
		++m_num;
	}

	void pop_front()
	{
        father::pop_front();
		--m_num;
	}
	
	void pop_back()
	{
		father::pop_back();
		--m_num;
	}
	
	iterator insert(iterator _Where, const T& _Val)
	{
		++m_num;
		return father::insert(_Where,_Val);
	}

	iterator erase(iterator _Where)
	{
		--m_num;
		return father::erase(_Where);
	}
	
	iterator erase(iterator _First, iterator _Last)
	{
		--m_num;
		return father::erase(_First,_Last);
	}

	void clear()
	{
		father::clear();
		m_num = 0;
	}

	unsigned int size()const { return m_num; }
};

#endif//__LIST_H_FILE__



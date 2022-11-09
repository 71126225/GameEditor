﻿#ifndef _MAP_H_
#define _MAP_H_

#include "mySTL.h"
#include "Vector.h"
#include <map>

using std::pair;
using std::initializer_list;
using std::map;

template<typename Key, typename Value>
class Map : public mySTL
{
public:
	typedef typename map<Key, Value>::iterator iterator;
	typedef typename map<Key, Value>::reverse_iterator reverse_iterator;
	typedef typename map<Key, Value>::const_iterator const_iterator;
public:
	Map() = default;
	explicit Map(initializer_list<pair<const Key, Value>> list)
	{
		mMap.insert(list);
	}
	~Map() override { clear(); }
	reverse_iterator rbegin()			{ return mMap.rbegin(); }
	reverse_iterator rend()				{ return mMap.rend(); }
	iterator begin()					{ return mMap.begin(); }
	iterator end()						{ return mMap.end(); }
	const_iterator cbegin() const		{ return mMap.cbegin(); }
	const_iterator cend() const			{ return mMap.cend(); }
	// 获取列表中指定顺序的值,如果获取失败,则返回设置的defaultValue,适用于value是指针类型或者整数类型的列表
	const Value& getAtIndex(const int index, const Value& defaultValue) const
	{
		int curIndex = 0;
		for(const auto& iter : mMap)
		{
			if (index == curIndex)
			{
				return iter->second;
			}
			++curIndex;
		}
		return defaultValue;
	}
	// 尝试获取值的指针,适用于value是非指针类型的列表,由于返回出去的值允许被修改,所以返回值不能添加const,此函数也不是const
	Value* get(const Key& key)
	{
		auto iter = mMap.find(key);
		if (iter != mMap.end())
		{
			return &(iter->second);
		}
		return nullptr;
	}
	// 尝试获取值的指针,适用于value是非指针类型的列表,返回出去的值不允许被修改
	const Value* getConst(const Key& key) const
	{
		auto iter = mMap.find(key);
		if (iter != mMap.end())
		{
			return &(iter->second);
		}
		return nullptr;
	}
	// 获取值,如果获取失败,则返回设置的defaultValue,适用于value是指针类型或者整数类型的列表
	const Value& get(const Key& key, const Value& defaultValue) const
	{
		auto iter = mMap.find(key);
		return iter != mMap.end() ? iter->second : defaultValue;
	}
	// 获取值,如果获取失败,则返回默认值,适用于value是类对象,可以自动调用构造的
	const Value& getValue(const Key& key) const
	{
		auto iter = mMap.find(key);
		return iter != mMap.end() ? iter->second : mEmptyValue;
	}
	// 如果key存在,则更新key的值为value,如果key不存在,则插入key和value
	void set(const Key& key, const Value& value)
	{
		auto iter = mMap.find(key);
		if (iter != mMap.end())
		{
			iter->second = value;
		}
		else
		{
			insert(key, value);
		}
	}
	const_iterator find(const Key& key) const { return mMap.find(key); }
	iterator find(const Key& key) { return mMap.find(key); }
	bool contains(const Key& key) const { return mMap.find(key) != mMap.end(); }
	// 尝试修改列表中的值,如果key存在则执行成功,key不存在则执行失败
	void trySet(const Key& key, const Value& value, bool* success = nullptr)
	{
		auto iter = mMap.find(key);
		const bool result = iter != mMap.end();
		if (result)
		{
			iter->second = value;
		}
		if (success != nullptr)
		{
			*success = result;
		}
	}
	// 尝试向列表中插入值,插入成功则返回插入值的引用,如果key已经存在,则返回对应的value的引用
	Value& tryInsert(const Key& key, const Value& value, bool* success = nullptr)
	{
		auto iter = mMap.find(key);
		if (iter == mMap.end())
		{
			if (success != nullptr)
			{
				*success = true;
			}
			return mMap.insert(make_pair(key, value)).first->second;
		}
		if (success != nullptr)
		{
			*success = false;
		}
		return iter->second;
	}
	// 尝试向列表中插入值,插入成功则返回插入值的引用,如果key已经存在,则返回对应的value的引用
	Value& tryInsert(const Key& key, bool* success = nullptr)
	{
		auto iter = mMap.find(key);
		if (iter == mMap.end())
		{
			if (success != nullptr)
			{
				*success = true;
			}
			return mMap.insert(make_pair(key, Map<Key, Value>::mEmptyValue)).first->second;
		}
		if (success != nullptr)
		{
			*success = false;
		}
		return iter->second;
	}
	// 插入key和value,如果key存在则插入失败
	bool insert(const Key& key, const Value& value)
	{
#if _DEBUG
		checkLock();
#endif
		auto iter = mMap.find(key);
		if (iter == mMap.end())
		{
			mMap.insert(make_pair(key, value));
			return true;
		}
		return false;
	}
	// 插入key和value,如果key存在则插入失败
	bool insertNoCheck(const Key& key, const Value& value)
	{
		auto iter = mMap.find(key);
		if (iter == mMap.end())
		{
			mMap.insert(make_pair(key, value));
			return true;
		}
		return false;
	}
	// 插入key,value为默认值,如果key存在则插入失败
	bool insert(const Key& key)
	{
#if _DEBUG
		checkLock();
#endif
		auto iter = mMap.find(key);
		if (iter == mMap.end())
		{
			mMap.insert(make_pair(key, Map<Key, Value>::mEmptyValue));
			return true;
		}
		return false;
	}
	// 插入key,value为默认值,如果key存在则插入失败
	bool insertNoCheck(const Key& key)
	{
		auto iter = mMap.find(key);
		if (iter == mMap.end())
		{
			mMap.insert(make_pair(key, Map<Key, Value>::mEmptyValue));
			return true;
		}
		return false;
	}
	void erase(const iterator& iter)
	{
#if _DEBUG
		checkLock();
#endif
		mMap.erase(iter);
	}
	void eraseNoCheck(const iterator& iter)
	{
		mMap.erase(iter);
	}
	// 将key从列表中移除,如果移除成功,则将value设置为被移除的元素的second,一般用于指针类型
	bool erase(const Key& key, Value& value)
	{
		auto iter = mMap.find(key);
		if (iter != mMap.end())
		{
			value = iter->second;
#if _DEBUG
			erase(iter);
#else
			mMap.erase(iter);
#endif
			return true;
		}
		return false;
	}
	// 将key从列表中移除,如果移除成功,则将value设置为被移除的元素的second,一般用于指针类型
	bool eraseNoCheck(const Key& key, Value& value)
	{
		auto iter = mMap.find(key);
		if (iter != mMap.end())
		{
			value = iter->second;
			mMap.erase(iter);
			return true;
		}
		return false;
	}
	// 返回值表示移除成功或失败
	bool erase(const Key& key)
	{
		auto iter = mMap.find(key);
		if (iter != mMap.end())
		{
#if _DEBUG
			erase(iter);
#else
			mMap.erase(iter);
#endif
			return true;
		}
		return false;
	}
	// 返回值表示移除成功或失败
	bool eraseNoCheck(const Key& key)
	{
		auto iter = mMap.find(key);
		if (iter != mMap.end())
		{
			mMap.erase(iter);
			return true;
		}
		return false;
	}
	// 因为clear本身会执行元素的析构,即使列表为空,也会执行较多的指令,所以先排除空列表的不必要的清空操作
	void clear()
	{
#if _DEBUG
		checkLock();
#endif
		if (mMap.size() > 0)
		{
			mMap.clear();
		}
	}
	bool merge(const Map<Key, Value>& other)
	{
		for (const auto& iter : mMap)
		{
#if _DEBUG
			const bool ret = insert(iter->first, iter->second);
#else
			const bool ret = mMap.insert(make_pair(iter->first, iter->second)).second;
#endif
			if (!ret)
			{
				return false;
			}
		}
		return true;
	}
	int size() const { return (int)mMap.size(); }
	Value& operator[](const Key& k) { return mMap[k]; }
	void clone(Map<Key, Value>& target) const
	{
		target.mMap.clear();
		for (const auto& iter : mMap)
		{
			target.mMap.insert(make_pair(iter->first, iter->second));
		}
#if _DEBUG
		target.resetLock();
#endif
	}
	void keyToList(Vector<Key>& keyList) const
	{
		for (const auto& iter : mMap)
		{
			keyList.push_back(iter->first);
		}
	}
	bool keyToList(Value* keyList, const int maxCount) const
	{
		int index = 0;
		for (const auto& iter : mMap)
		{
			if (index >= maxCount)
			{
				return false;
			}
			keyList[index++] = iter->first;
		}
		return true;
	}
	void valueToList(Vector<Value>& valueList) const
	{
		for (const auto& iter : mMap)
		{
			valueList.push_back(iter->second);
		}
	}
	void valueToListFilter(Vector<Value>& valueList, const Value& exceptValue) const
	{
		for (const auto& iter : mMap)
		{
			if (iter->second != exceptValue)
			{
				valueList.push_back(iter->second);
			}
		}
	}
	template<size_t Length>
	int valueToListFilter(Array<Length, Value>& valueList, const Value& exceptValue, const int startIndex = 0) const
	{
		if (startIndex >= (int)mMap.size())
		{
			return 0;
		}
		int indexInMap = 0;
		int curDataCount = 0;
		for (const auto& iter : mMap)
		{
			if (++indexInMap <= startIndex)
			{
				continue;
			}
			if (curDataCount >= (int)Length)
			{
				break;
			}
			if (iter->second != exceptValue)
			{
				valueList[curDataCount++] = iter->second;
			}
		}
		return curDataCount;
	}
	template<size_t Length>
	int valueToList(Array<Length, Value>& valueList, const int startIndex = 0) const
	{
		if (startIndex >= (int)mMap.size())
		{
			return 0;
		}
		int indexInMap = 0;
		int curDataCount = 0;
		for (const auto& iter : mMap)
		{
			if (++indexInMap <= startIndex)
			{
				continue;
			}
			if (curDataCount >= (int)Length)
			{
				break;
			}
			valueList[curDataCount++] = iter->second;
		}
		return curDataCount;
	}
	int valueToList(Value* valueList, const int maxCount, const int startIndex = 0) const
	{
		if (startIndex >= (int)mMap.size())
		{
			return 0;
		}
		int indexInMap = 0;
		int curDataCount = 0;
		for (const auto& iter : mMap)
		{
			if (++indexInMap <= startIndex)
			{
				continue;
			}
			if (curDataCount >= maxCount)
			{
				break;
			}
			valueList[curDataCount++] = iter->second;
		}
		return curDataCount;
	}
public:
	map<Key, Value> mMap;
	static Value mEmptyValue;
};

template<typename Key, typename Value>
Value Map<Key, Value>::mEmptyValue;

#endif
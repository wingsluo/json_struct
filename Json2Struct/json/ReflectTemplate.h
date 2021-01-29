/*******************************************************************************
* @file     ReflectTemplate.h
* @brief    Json
* @author   luoyu
* @date:    2019-12-06
* @note
* Modify history:
******************************************************************************/
#ifndef REFLECT_TEMPLATE_H
#define REFLECT_TEMPLATE_H

#include "os/os.h"
#include "json/CJsonObject.hpp"
#include "ContainerType.h"
#include "JsonBase.h"


template<class T>
class reflect_base
{
public:
    static string GetTString(intptr_t p)
    {
        T& t = *reinterpret_cast<T*>(p);
		std::stringstream stream;
		stream << t;
		return stream.str();
	}
	static T GetStringT(string _str)
	{
		T t;
		std::stringstream stream(_str);
		stream >> t;
		return t;
	}

    static void init_base_type( void* p)
	{
		*(T*)p = T(0);
	}

	static bool CompareObj(void* p1, void*p2)
	{
		T& t1 = *reinterpret_cast<T*>(p1);
		T& t2 = *reinterpret_cast<T*>(p2);
		return t1 == t2;
	}

	static bool to_json_enum(neb::CJsonObject& oJson, string key, void* p)
	{
		T& t = *reinterpret_cast<T*>(p);
		int ti = static_cast<int>(t);
		oJson.Add(key, ti);
		return true;
	}

	static bool load_json_enum(neb::CJsonObject& oJson, string key, void* p)
	{
		T& t = *reinterpret_cast<T*>(p);
		int ti = 0;
		if (oJson.Get(key, ti))
		{
			t = static_cast<T>(ti);
			return true;
		}
		return false;
	}

	static bool to_json_base_type(neb::CJsonObject& oJson, string key, void* p)
    {
        T& t = *reinterpret_cast<T*>(p);
		oJson.Add(key, t);
		return true;
	}
	static bool load_json_base_type(neb::CJsonObject& oJson, string key, void* p)
    {
        T& t = *reinterpret_cast<T*>(p);
		return oJson.Get(key, t);
	}
	static bool to_json_vector(neb::CJsonObject& oJson, void* p)
	{
		vector<T>& containerT = *reinterpret_cast<vector<T>*>(p);
		typename vector<T>::iterator iter = containerT.begin();
		for (; iter != containerT.end(); ++iter)
		{
			//oJson.AddAsFirst(iter->to_json());
			oJson.Add(iter->to_json());
		}
		return true;
	}
	static bool load_json_vector(neb::CJsonObject& oJson, void* p)
	{
		vector<T>& containerT = *reinterpret_cast<vector<T>*>(p);
		int i = 0;
		for (; i < oJson.GetArraySize(); ++i)
		{
			T t(oJson[i]);
			containerT.push_back(t);
		}
		return true;
	}
	static bool to_json_set(neb::CJsonObject& oJson, void* p)
	{
		set<T>& containerT = *reinterpret_cast<set<T>*>(p);
		auto iter = containerT.begin();
		for (; iter != containerT.end(); ++iter)
        {
            const T& t = static_cast<T>(*iter);
			//oJson.AddAsFirst(t.to_json());
			oJson.Add(iter->to_json());
		}
		return true;
	}
	static bool load_json_set(neb::CJsonObject& oJson, void* p)
	{
		set<T>& containerT = *reinterpret_cast<set<T>*>(p);
		int i = 0;
		for (; i < oJson.GetArraySize(); ++i)
		{
			T t(oJson[i]);
			containerT.insert(t);
		}
		return true;
	}
	static bool to_json_base_vector(neb::CJsonObject& oJson, void* p)
	{
		vector<T>& containerT = *reinterpret_cast<vector<T>*>(p);
		typename vector<T>::iterator iter = containerT.begin();
		for (; iter != containerT.end(); ++iter)
		{
			//oJson.AddAsFirst(*iter);
			oJson.Add(*iter);
		}
		return true;
	}
	static bool load_json_base_vector(neb::CJsonObject& oJson, void* p)
	{
		vector<T>& containerT = *reinterpret_cast<vector<T>*>(p);
		int i = 0;
		for (; i < oJson.GetArraySize(); ++i)
		{
			T t;
			oJson.Get(i, t);
			containerT.push_back(t);
		}
		return true;
	}

	static bool to_json_list(neb::CJsonObject& oJson, void* p)
	{
		list<T>& containerT = *reinterpret_cast<list<T>*>(p);
		typename list<T>::iterator iter = containerT.begin();
		for (; iter != containerT.end(); ++iter)
		{
			//oJson.AddAsFirst(iter->to_json());
			oJson.Add(iter->to_json());
		}
		return true;
	}
	static bool load_json_list(neb::CJsonObject& oJson, void* p)
	{
		list<T>& containerT = *reinterpret_cast<list<T>*>(p);
		int i = 0;
		for (; i < oJson.GetArraySize(); ++i)
		{
			T t(oJson[i]);
			containerT.push_back(t);
		}
		return true;
	}

	static bool to_json_base_list(neb::CJsonObject& oJson, void* p)
	{
		list<T>& containerT = *reinterpret_cast<list<T>*>(p);
		typename list<T>::iterator iter = containerT.begin();
		for (; iter != containerT.end(); ++iter)
		{
			//oJson.AddAsFirst(*iter);
			oJson.Add(*iter);
		}
		return true;
	}
	static bool load_json_base_list(neb::CJsonObject& oJson, void* p)
	{
		list<T>& containerT = *reinterpret_cast<list<T>*>(p);
		int i = 0;
		for (; i < oJson.GetArraySize(); ++i)
		{
			T t;
			oJson.Get(i, t);
			containerT.push_back(t);
		}
		return true;
	}
	static bool to_json_base_set(neb::CJsonObject& oJson, void* p)
	{
		set<T>& containerT = *reinterpret_cast<set<T>*>(p);
		typename set<T>::iterator iter = containerT.begin();
		for (; iter != containerT.end(); ++iter)
		{
			//oJson.AddAsFirst(*iter);
			oJson.Add(*iter);
		}
		return true;
	}
	static bool load_json_base_set(neb::CJsonObject& oJson, void* p)
	{
		set<T>& containerT = *reinterpret_cast<set<T>*>(p);
		int i = 0;
		for (; i < oJson.GetArraySize(); ++i)
		{
			T t;
			oJson.Get(i, t);
			containerT.insert(t);
		}
		return true;
	}

#ifdef JSON_MAP_STRUCT

#define TO_JSON_MAP(KeyType, ValueType) \
	map<KeyType, ValueType>& _map = *reinterpret_cast<map<KeyType, ValueType>*>(p);\
	typename map<KeyType, ValueType>::iterator iter = _map.begin();\
	for (; iter != _map.end(); ++iter){\
        string _str_key = reflect_base<KeyType>::GetTString(reinterpret_cast<intptr_t>(&iter->first));\
		oJson.Add(_str_key, iter->second.to_json());\
	}

#define LOAD_JSON_MAP(KeyType, ValueType) \
	map<KeyType, ValueType>& _map = *reinterpret_cast<map<KeyType, ValueType>*>(p);\
	while (oJson.GetKey(_str_key)){\
		KeyType _key = reflect_base<KeyType>::GetStringT(_str_key);\
		oJson.Get(_str_key, oJsonSub);\
		ValueType _value(oJsonSub);\
		_map.insert(make_pair(_key, _value));\
	}



	static bool load_json_map(neb::CJsonObject& oJson, void* p, st_container_type _ct)
	{
		string _str_key;
		neb::CJsonObject oJsonSub;
		if (_ct._type_name_key._base_type == bt_uchar)
		{
            LOAD_JSON_MAP(uchar, T)
		}
		else if (_ct._type_name_key._base_type == bt_char)
		{
            LOAD_JSON_MAP(char, T)
		}
		else if (_ct._type_name_key._base_type == bt_ushort)
		{
            LOAD_JSON_MAP(ushort, T)
		}
		else if (_ct._type_name_key._base_type == bt_short)
		{
            LOAD_JSON_MAP(short, T)
		}
		else if (_ct._type_name_key._base_type == bt_uint)
		{
            LOAD_JSON_MAP(uint, T)
		}
		else if (_ct._type_name_key._base_type == bt_int)
		{
            LOAD_JSON_MAP(int, T)
		}
		else if (_ct._type_name_key._base_type == bt_ulong)
		{
            LOAD_JSON_MAP(ulong, T)
		}
		else if (_ct._type_name_key._base_type == bt_long)
		{
            LOAD_JSON_MAP(long, T)
		}
		else if (_ct._type_name_key._base_type == bt_uint64)
		{
            LOAD_JSON_MAP(uint64, T)
		}
		else if (_ct._type_name_key._base_type == bt_int64)
		{
            LOAD_JSON_MAP(int64, T)
		}
		else if (_ct._type_name_key._base_type == bt_float)
		{
            LOAD_JSON_MAP(float, T)
		}
		else if (_ct._type_name_key._base_type == bt_double)
		{
            LOAD_JSON_MAP(double, T)
		}
		else if (_ct._type_name_key._base_type == bt_string)
		{
            LOAD_JSON_MAP(string, T)
		}
		return true;
	}

	static bool to_json_map(neb::CJsonObject& oJson, void* p, st_container_type _ct)
	{
		if (_ct._type_name_key._base_type == bt_uchar)
		{
            TO_JSON_MAP(uchar, T)
		}
		else if (_ct._type_name_key._base_type == bt_char)
		{
            TO_JSON_MAP(uchar, T)
		}
		else if (_ct._type_name_key._base_type == bt_ushort)
		{
            TO_JSON_MAP(ushort, T)
		}
		else if (_ct._type_name_key._base_type == bt_short)
		{
            TO_JSON_MAP(short, T)
		}
		else if (_ct._type_name_key._base_type == bt_uint)
		{
            TO_JSON_MAP(uint, T)
		}
		else if (_ct._type_name_key._base_type == bt_int)
		{
            TO_JSON_MAP(int, T)
		}
		else if (_ct._type_name_key._base_type == bt_ulong)
		{
            TO_JSON_MAP(ulong, T)
		}
		else if (_ct._type_name_key._base_type == bt_long)
		{
            TO_JSON_MAP(long, T)
		}
		else if (_ct._type_name_key._base_type == bt_uint64)
		{
            TO_JSON_MAP(uint64, T)
		}
		else if (_ct._type_name_key._base_type == bt_int64)
		{
            TO_JSON_MAP(int64, T)
		}
		else if (_ct._type_name_key._base_type == bt_float)
		{
            TO_JSON_MAP(float, T)
		}
		else if (_ct._type_name_key._base_type == bt_double)
		{
            TO_JSON_MAP(double, T)
		}
		else if (_ct._type_name_key._base_type == bt_string)
		{
            TO_JSON_MAP(string, T)
		}
		return true;
	}
#endif


#ifdef JSON_UNORDERED_MAP_STRUCT

#define TO_JSON_UNORDERED_MAP(KeyType, ValueType) \
    unordered_map<KeyType, ValueType>& _map = *reinterpret_cast<unordered_map<KeyType, ValueType>*>(p);\
    typename unordered_map<KeyType, ValueType>::iterator iter = _map.begin();\
    for (; iter != _map.end(); ++iter){\
        string _str_key = reflect_base<KeyType>::GetTString(reinterpret_cast<intptr_t>(&iter->first));\
        oJson.Add(_str_key, iter->second.to_json());\
    }

#define LOAD_JSON_UNORDERED_MAP(KeyType, ValueType) \
    unordered_map<KeyType, ValueType>& _map = *reinterpret_cast<unordered_map<KeyType, ValueType>*>(p);\
    while (oJson.GetKey(_str_key)){\
        KeyType _key = reflect_base<KeyType>::GetStringT(_str_key);\
        oJson.Get(_str_key, oJsonSub);\
        ValueType _value(oJsonSub);\
        _map.insert(make_pair(_key, _value));\
    }
    static bool load_json_unordered_map(neb::CJsonObject& oJson, void* p, st_container_type _ct)
	{
        string _str_key;
        neb::CJsonObject oJsonSub;
        if (_ct._type_name_key._base_type == bt_uchar)
        {
            LOAD_JSON_UNORDERED_MAP(uchar, T)
        }
        else if (_ct._type_name_key._base_type == bt_char)
        {
            LOAD_JSON_UNORDERED_MAP(char, T)
        }
        else if (_ct._type_name_key._base_type == bt_ushort)
        {
            LOAD_JSON_UNORDERED_MAP(ushort, T)
        }
        else if (_ct._type_name_key._base_type == bt_short)
        {
            LOAD_JSON_UNORDERED_MAP(short, T)
        }
        else if (_ct._type_name_key._base_type == bt_uint)
        {
            LOAD_JSON_UNORDERED_MAP(uint, T)
        }
        else if (_ct._type_name_key._base_type == bt_int)
        {
            LOAD_JSON_UNORDERED_MAP(int, T)
        }
        else if (_ct._type_name_key._base_type == bt_ulong)
        {
            LOAD_JSON_UNORDERED_MAP(ulong, T)
        }
        else if (_ct._type_name_key._base_type == bt_long)
        {
            LOAD_JSON_UNORDERED_MAP(long, T)
        }
        else if (_ct._type_name_key._base_type == bt_uint64)
        {
            LOAD_JSON_UNORDERED_MAP(uint64, T)
        }
        else if (_ct._type_name_key._base_type == bt_int64)
        {
            LOAD_JSON_UNORDERED_MAP(int64, T)
        }
        else if (_ct._type_name_key._base_type == bt_float)
        {
            LOAD_JSON_UNORDERED_MAP(float, T)
        }
        else if (_ct._type_name_key._base_type == bt_double)
        {
            LOAD_JSON_UNORDERED_MAP(double, T)
        }
        else if (_ct._type_name_key._base_type == bt_string)
        {
            LOAD_JSON_UNORDERED_MAP(string, T)
		}
        return true;
    }

    static bool to_json_unordered_map(neb::CJsonObject& oJson, void* p, st_container_type _ct)
	{
        if (_ct._type_name_key._base_type == bt_uchar)
        {
            TO_JSON_UNORDERED_MAP(uchar, T)
        }
        else if (_ct._type_name_key._base_type == bt_char)
        {
            TO_JSON_UNORDERED_MAP(uchar, T)
        }
        else if (_ct._type_name_key._base_type == bt_ushort)
        {
            TO_JSON_UNORDERED_MAP(ushort, T)
        }
        else if (_ct._type_name_key._base_type == bt_short)
        {
            TO_JSON_UNORDERED_MAP(short, T)
        }
        else if (_ct._type_name_key._base_type == bt_uint)
        {
            TO_JSON_UNORDERED_MAP(uint, T)
        }
        else if (_ct._type_name_key._base_type == bt_int)
        {
            TO_JSON_UNORDERED_MAP(int, T)
        }
        else if (_ct._type_name_key._base_type == bt_ulong)
        {
            TO_JSON_UNORDERED_MAP(ulong, T)
        }
        else if (_ct._type_name_key._base_type == bt_long)
        {
            TO_JSON_UNORDERED_MAP(long, T)
        }
        else if (_ct._type_name_key._base_type == bt_uint64)
        {
            TO_JSON_UNORDERED_MAP(uint64, T)
        }
        else if (_ct._type_name_key._base_type == bt_int64)
        {
            TO_JSON_UNORDERED_MAP(int64, T)
        }
        else if (_ct._type_name_key._base_type == bt_float)
        {
            TO_JSON_UNORDERED_MAP(float, T)
        }
        else if (_ct._type_name_key._base_type == bt_double)
        {
            TO_JSON_UNORDERED_MAP(double, T)
        }
        else if (_ct._type_name_key._base_type == bt_string)
        {
            TO_JSON_UNORDERED_MAP(string, T)
		}
        return true;
	}
#endif
};


template<class KeyType, class ValueType>
class reflect_map
{
public:
#ifdef JSON_MAP_STRUCT
	static bool to_json_base(neb::CJsonObject& oJson, void* p)
	{
        map<KeyType, ValueType>& _map = *reinterpret_cast<map<KeyType, ValueType>*>(p);
        typename map<KeyType, ValueType>::iterator iter = _map.begin();
		for (; iter != _map.end(); ++iter)
		{
            string _str_key = reflect_base<KeyType>::GetTString(reinterpret_cast<intptr_t>(&iter->first));
			oJson.Add(_str_key, iter->second);
		}
		return true;
	}
	static bool load_json_base(neb::CJsonObject& oJson, void* p)
	{
        map<KeyType, ValueType>& _map = *reinterpret_cast<map<KeyType, ValueType>*>(p);
		string _str_key;
		while (oJson.GetKey(_str_key))
		{
			KeyType _key = reflect_base<KeyType>::GetStringT(_str_key);
			ValueType _value;
			if (oJson.Get(_str_key, _value))
			{
				_map.insert(make_pair(_key, _value));
			}
		}
		return true;
	}
#endif

#ifdef JSON_UNORDERED_MAP_STRUCT
    static bool to_json_base_unordered(neb::CJsonObject& oJson, void* p)
	{
        unordered_map<KeyType, ValueType>& _map = *reinterpret_cast<unordered_map<KeyType, ValueType>*>(p);
        typename unordered_map<KeyType, ValueType>::iterator iter = _map.begin();
        for (; iter != _map.end(); ++iter)
        {
            string _str_key = reflect_base<KeyType>::GetTString(reinterpret_cast<intptr_t>(&iter->first));
            oJson.Add(_str_key, iter->second);
		}
        return true;
    }
    static bool load_json_base_unordered(neb::CJsonObject& oJson, void* p)
	{
        unordered_map<KeyType, ValueType>& _map = *reinterpret_cast<unordered_map<KeyType, ValueType>*>(p);
        string _str_key;
        while (oJson.GetKey(_str_key))
        {
            KeyType _key = reflect_base<KeyType>::GetStringT(_str_key);
            ValueType _value;
            if (oJson.Get(_str_key, _value))
            {
                _map.insert(make_pair(_key, _value));
            }
		}
        return true;
	}
#endif
};


#endif // __REFLECT_TYPE_H

/*******************************************************************************
* @file     ContainerType.cpp
* @brief    Json
* @author   luoyu
* @date:    2019-12-06
* @note
* Modify history:
******************************************************************************/
#include "ContainerType.h"
#include "ReflectTemplate.h"
#include "StringHelper.h"


#if defined(WIN32) || defined(_WIN64)
const string _str_vector_header = "class std::vector<";
const string _str_vector_header2 = "class std::vector<";
const string _str_list_header = "class std::list<";
const string _str_list_header2 = "class std::list<";
const string _str_set_header = "class std::set<";
const string _str_enum_header = "enum ";
const string _str_struct_header = "struct ";
const string _str_class_header = "class ";
const string _str_map_head = "class std::map<";
const string _str_hash_map_head = "class std::hash_map<";
const string _str_unordered_map_head = "class std::unordered_map<";
#else
const string _str_vector_header = "std::vector<";
const string _str_vector_header2 = "std::__cxx11::vector<";
const string _str_list_header = "std::list<";
const string _str_list_header2 = "std::__cxx11::list<";
const string _str_set_header = "std::set<";
const string _str_enum_header = "";
const string _str_struct_header = "";
const string _str_class_header = "";
const string _str_map_head = "std::map<";
const string _str_hash_map_head = "std::hash_map<";
const string _str_unordered_map_head = "std::unordered_map<";
#endif

//_g_base_type_info[bt_##Type]._type_name = type_oper<Type>::type_id_name(); 

#define GET_BASE_TYPE_INFO(Type) \
	_g_base_type_info[bt_##Type]._type_name = TYPE_ID_NAME(Type);\
	_g_base_type_info[bt_##Type].init_base_type_fn = reflect_base<Type>::init_base_type;\
	_g_base_type_info[bt_##Type].t_string_fn = reflect_base<Type>::GetTString;\
	_g_base_type_info[bt_##Type].to_json_fn = reflect_base<Type>::to_json_base_type;\
	_g_base_type_info[bt_##Type].load_json_fn = reflect_base<Type>::load_json_base_type;\
	_g_base_type_info[bt_##Type].to_json_vector_fn = reflect_base<Type>::to_json_base_vector;\
	_g_base_type_info[bt_##Type].load_json_vector_fn = reflect_base<Type>::load_json_base_vector;\
	_g_base_type_info[bt_##Type].to_json_list_fn = reflect_base<Type>::to_json_base_list;\
	_g_base_type_info[bt_##Type].load_json_list_fn = reflect_base<Type>::load_json_base_list;\
	_g_base_type_info[bt_##Type].to_json_set_fn = reflect_base<Type>::to_json_base_set;\
	_g_base_type_info[bt_##Type].load_json_set_fn = reflect_base<Type>::load_json_base_set;

st_base_type_fn _g_base_type_info[bt_struct];

static bool _s_json_base_init = false;
void json_base_init()
{
    if (!_s_json_base_init)
	{
        GET_BASE_TYPE_INFO(bool)
        GET_BASE_TYPE_INFO(uchar)
        GET_BASE_TYPE_INFO(char)
        GET_BASE_TYPE_INFO(ushort)
        GET_BASE_TYPE_INFO(short)
        GET_BASE_TYPE_INFO(uint)
        GET_BASE_TYPE_INFO(int)
        GET_BASE_TYPE_INFO(uint64)
        GET_BASE_TYPE_INFO(int64)
        GET_BASE_TYPE_INFO(ulong)
        GET_BASE_TYPE_INFO(long)
        GET_BASE_TYPE_INFO(float)
        GET_BASE_TYPE_INFO(double)
        GET_BASE_TYPE_INFO(string)
        _s_json_base_init = true;
	}
}


bool st_type_name::is_base_type(string _type_id_name, bool _b_can_struct)
{
	int i = 0;
	string _str;
	utility::trim(_type_id_name);
	for (; i < bt_struct; ++i)
	{
		if (!_type_id_name.compare(0, _g_base_type_info[i]._type_name.length(), _g_base_type_info[i]._type_name))
		{
            _base_type = static_cast<e_base_type>(i);
			return true;
		}
		if (bt_char == i)
		{
			string signedchar = TYPE_ID_NAME(signed char);
			if (!_type_id_name.compare(0, signedchar.length(), signedchar))
			{
				_base_type = static_cast<e_base_type>(i);
				return true;
			}
		}
	}
	if (_b_can_struct)
	{
		if (!_type_id_name.compare(0, _str_struct_header.length(), _str_struct_header))
		{
			_base_type = bt_struct;
            size_t idx = _type_id_name.find_first_of(",");
            if(idx == string::npos) idx = _type_id_name.length();
			_struct_name = _type_id_name.substr(_str_struct_header.length(), idx - _str_struct_header.length());
			idx = _struct_name.find_last_of(":");
			if (idx != string::npos)
			{
				_struct_name = _struct_name.substr(idx+1);
			}
			return true;
		}
		if (!_type_id_name.compare(0, _str_class_header.length(), _str_class_header))
		{
			_base_type = bt_struct;
			size_t idx = _type_id_name.find_first_of(",");
			if (idx == string::npos) idx = _type_id_name.length();
			_struct_name = _type_id_name.substr(_str_class_header.length(), idx - _str_class_header.length());
			idx = _struct_name.find_last_of(":");
			if (idx != string::npos)
			{
				_struct_name = _struct_name.substr(idx+1);
			}
			return true;
		}
	}
	return false;
}


bool st_type_name::is_bool()
{
	switch (_base_type)
	{
	case bt_bool:
		return true;
    default:
        return false;
    }
}
bool st_type_name::is_int()
{
	switch (_base_type)
	{
	case bt_char:
	case bt_uchar:
	case bt_short:
	case bt_ushort:
	case bt_int:
    case bt_uint:
	case bt_int64:
	case bt_uint64:
    case bt_long:
    case bt_ulong:
		return true;
    default:
        return false;
    }
}
bool st_type_name::is_double()
{
	switch (_base_type)
	{
	case bt_float:
	case bt_double:
		return true;
    default:
        return false;
    }
}
bool st_type_name::is_string()
{
	switch (_base_type)
	{
	case bt_string:
		return true;
    default:
        return false;
    }
}

//是否容器类型
bool st_container_type::is_container(string _type_id_name, string _str_stl_head)
{
	utility::trim(_type_id_name);
	if (!_type_id_name.compare(0, _str_stl_head.length(), _str_stl_head))
	{
		string _str = _type_id_name.substr(_str_stl_head.length());
		if (_type_name_key.is_base_type(_str))// , _str_stl_head != _str_set_header))
		{
			return true;
		}
	}
	return false;
}


#ifdef JSON_MAP_STRUCT
#define SET_MAP_FUN(KeyType,ValueType) {\
	to_json_map_fn = reflect_map<KeyType,ValueType>::to_json_base;\
	load_json_map_fn = reflect_map<KeyType,ValueType>::load_json_base;\
}
#else
#define SET_MAP_FUN(KeyType,ValueType) {\
    to_json_map_fn = nullptr;\
    load_json_map_fn = nullptr;\
}
#endif



#ifdef JSON_UNORDERED_MAP_STRUCT
#define SET_UNORDERED_MAP_FUN(KeyType,ValueType) {\
    to_json_map_fn = reflect_map<KeyType,ValueType>::to_json_base_unordered;\
    load_json_map_fn = reflect_map<KeyType,ValueType>::load_json_base_unordered;\
}
#else 
#define SET_UNORDERED_MAP_FUN(KeyType,ValueType) {\
    to_json_map_fn = nullptr;\
    load_json_map_fn = nullptr;\
}
#endif

#define SET_MAP_BASE_(MAP_FUN,KeyType) \
	if(_type_name_value._base_type == bt_bool) \
        MAP_FUN(KeyType, bool)\
	else if(_type_name_value._base_type == bt_uchar) \
        MAP_FUN(KeyType, uchar)\
	else if (_type_name_value._base_type == bt_char) \
        MAP_FUN(KeyType, char) \
	else if (_type_name_value._base_type == bt_ushort) \
        MAP_FUN(KeyType, ushort) \
	else if (_type_name_value._base_type == bt_short) \
        MAP_FUN(KeyType, short) \
	else if (_type_name_value._base_type == bt_uint) \
        MAP_FUN(KeyType, uint) \
	else if (_type_name_value._base_type == bt_int) \
        MAP_FUN(KeyType, int) \
	else if (_type_name_value._base_type == bt_ulong) \
        MAP_FUN(KeyType, ulong) \
	else if (_type_name_value._base_type == bt_long) \
        MAP_FUN(KeyType, long) \
	else if (_type_name_value._base_type == bt_uint64) \
        MAP_FUN(KeyType, uint64) \
	else if (_type_name_value._base_type == bt_int64) \
        MAP_FUN(KeyType, int64) \
	else if (_type_name_value._base_type == bt_float) \
        MAP_FUN(KeyType, float) \
	else if (_type_name_value._base_type == bt_double) \
        MAP_FUN(KeyType, double) \
	else if (_type_name_value._base_type == bt_string) \
        MAP_FUN(KeyType, string)




#define SET_MAP_BASE(KeyType) \
    if(_base_container == bc_map){\
        SET_MAP_BASE_(SET_MAP_FUN,KeyType)\
    }\
    else if(_base_container == bc_unordered_map){\
        SET_MAP_BASE_(SET_UNORDERED_MAP_FUN,KeyType)\
    }

bool st_container_type::set_map_fun()
{
	if (_type_name_key._base_type == bt_uchar)
    {
        SET_MAP_BASE(uchar)
	}
	else if (_type_name_key._base_type == bt_char)
	{
        SET_MAP_BASE(char)
	}
	else if (_type_name_key._base_type == bt_ushort)
	{
        SET_MAP_BASE(ushort)
	}
	else if (_type_name_key._base_type == bt_short)
	{
        SET_MAP_BASE(short)
	}
	else if (_type_name_key._base_type == bt_uint)
	{
        SET_MAP_BASE(uint)
	}
	else if (_type_name_key._base_type == bt_int)
	{
        SET_MAP_BASE(int)
	}
	else if (_type_name_key._base_type == bt_long)
	{
        SET_MAP_BASE(long)
	}
	else if (_type_name_key._base_type == bt_ulong)
	{
        SET_MAP_BASE(ulong)
	}
	else if (_type_name_key._base_type == bt_long)
	{
        SET_MAP_BASE(long)
	}
	else if (_type_name_key._base_type == bt_uint64)
	{
        SET_MAP_BASE(uint64)
	}
	else if (_type_name_key._base_type == bt_int64)
	{
        SET_MAP_BASE(int64)
	}
	else if (_type_name_key._base_type == bt_float)
	{
        SET_MAP_BASE(float)
	}
	else if (_type_name_key._base_type == bt_double)
	{
        SET_MAP_BASE(double)
	}
	else if (_type_name_key._base_type == bt_string)
	{
        SET_MAP_BASE(string)
	}
	return true;
}

bool st_container_type::is_map_container(string _type_id_name)
{
	utility::trim(_type_id_name);
#ifdef JSON_MAP_STRUCT
	if (!_type_id_name.compare(0, _str_map_head.length(), _str_map_head))
	{
		string _str_key = _type_id_name.substr(_str_map_head.length());
		//key必须是basetype
		if (_type_name_key.is_base_type(_str_key, false))
		{
			int i = 0;
			for (; i < bt_struct; ++i)
			{
				if (_type_name_key._base_type == i)
				{
					string _str_value = _str_key.substr(_g_base_type_info[i]._type_name.length() + 1);
					if (_type_name_value.is_base_type(_str_value))
					{
						_base_container = bc_map;
						set_map_fun();
						return true;
					}
				}
			}
		}
	}
#endif
	return false;
}

bool st_container_type::is_unordered_map_container(string _type_id_name)
{
	utility::trim(_type_id_name);
#ifdef JSON_UNORDERED_MAP_STRUCT
    if (!_type_id_name.compare(0, _str_unordered_map_head.length(), _str_unordered_map_head))
    {
        string _str_key = _type_id_name.substr(_str_unordered_map_head.length());
        //key必须是basetype
        if (_type_name_key.is_base_type(_str_key, false))
        {
            int i = 0;
            for (; i < bt_struct; ++i)
            {
                if (_type_name_key._base_type == i)
                {
                    string _str_value = _str_key.substr(_g_base_type_info[i]._type_name.length() + 1);
                    if (_type_name_value.is_base_type(_str_value))
					{
						_base_container = bc_unordered_map;
                        set_map_fun();
                        return true;
                    }
                }
            }
        }
    }
#endif
    return false;
}

bool st_container_type::set_container_type(string _type_id_name)
{
	if (_type_name_key.is_base_type(_type_id_name,false))
	{
		_base_container = bc_base;
		return true;
	}
    //vector
	else if (is_container(_type_id_name, _str_vector_header))
	{
		_base_container = bc_vector;
		return true;
	}
    //list
	else if (is_container(_type_id_name, _str_list_header))
	{
		_base_container = bc_list;
		return true;
	}
    else if (is_container(_type_id_name, _str_list_header2))
    {
        _base_container = bc_list;
        return true;
    }
    //set
	else if (is_container(_type_id_name, _str_set_header))
	{
		_base_container = bc_set;
		return true;
	}
#ifdef JSON_MAP_STRUCT
    //map
	else if (is_map_container(_type_id_name))
	{
		return true;
	}
#endif //JSON_MAP_STRUCT

#ifdef JSON_UNORDERED_MAP_STRUCT
    else if (is_unordered_map_container(_type_id_name))
    {
        return true;
    }
#endif
	else if(_type_name_key.is_base_type(_type_id_name))
	{
		_base_container = bc_base;
		return true;
	}
	return false;
}


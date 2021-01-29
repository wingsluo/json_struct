/*******************************************************************************
* @file     ContainerType.h
* @brief    Json
* @author   luoyu
* @date:    2019-12-06
* @note
* Modify history:
******************************************************************************/
#ifndef CONTAINER_TYPE_H
#define CONTAINER_TYPE_H

#include "os/os.h"
#include "CJsonObject.hpp"
using std::string;

typedef string(*T2StringFun)(intptr_t p);
typedef bool(*json_base_type_fun)(neb::CJsonObject& oJson, string key, void* p);
typedef bool(*json_base_container_fun)(neb::CJsonObject& oJsonSub, void* vec);
typedef bool(*json_base_map_fun)(neb::CJsonObject& oJsonSub, void* vec);
typedef bool(*struct_compare_fun)(void* p1, void*p2);

enum e_base_type
{
	bt_bool = 0,
	bt_uchar,
	bt_char,
	bt_ushort,
	bt_short,
	bt_uint,
    bt_int,
	bt_uint64,
	bt_int64,
    bt_ulong,
    bt_long,
	bt_float,
	bt_double,
	bt_string,
	bt_struct
};

enum e_base_container
{
	bc_base,
	bc_vector,
	bc_list,
	bc_set,
    bc_map,
    bc_unordered_map
};

template <typename T>
class type_oper
{
public:
	static string type_id_name()
	{

#if defined(WIN32) || defined(_WIN64)
		return typeid(T).name();
#else
		T t;
		int status = 0;
        return abi::__cxa_demangle(typeid(t).name(), nullptr, nullptr, &status);
#endif
	}
};



#if defined(WIN32) || defined(_WIN64)
#define TYPE_ID_NAME(Type) typeid(Type).name()
#else
string inline type_name(string type_id_name)
{
	int status = 0;
	return abi::__cxa_demangle(type_id_name.c_str(), 0, 0, &status);
}
#define TYPE_ID_NAME(Type) type_name(typeid(Type).name())
#endif

struct st_type_name
{
	e_base_type _base_type;
	string _struct_name;
	st_type_name() {
		_base_type = bt_int;
	}
	bool is_base_type(string _type_id_name, bool _b_can_struct = true);
	bool is_bool();
	bool is_int();
	bool is_double();
	bool is_string();
};


struct st_container_type
{
	e_base_container _base_container;
	st_type_name _type_name_key;
	st_type_name _type_name_value;
	struct_compare_fun compare_fn;
	json_base_map_fun to_json_map_fn;
	json_base_map_fun load_json_map_fn;
	st_container_type() {
		_base_container = bc_base;
        to_json_map_fn = nullptr;
		load_json_map_fn = nullptr;
		compare_fn = nullptr;
	}
	bool is_container(string _type_id_name, string _str_stl_head);
	bool set_map_fun();
	bool is_map_container(string _type_id_name);
    bool is_unordered_map_container(string _type_id_name);
	bool set_container_type(string _type_id_name);
};


typedef void(*init_base_type)(void* p);

struct st_base_type_fn
{
	string _type_name;
	init_base_type init_base_type_fn;
	T2StringFun t_string_fn;
	json_base_type_fun to_json_fn;
	json_base_type_fun load_json_fn;
	json_base_container_fun to_json_vector_fn;
	json_base_container_fun load_json_vector_fn;
	json_base_container_fun to_json_list_fn;
	json_base_container_fun load_json_list_fn;
	json_base_container_fun to_json_set_fn;
	json_base_container_fun load_json_set_fn;
};

extern st_base_type_fn _g_base_type_info[bt_struct];


#endif // __CONTAINER_TYPE_H


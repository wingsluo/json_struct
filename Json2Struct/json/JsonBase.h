/*******************************************************************************
* @file     JsonBase.h
* @brief    Json
* @author   luoyu
* @date:    2019-12-06
* @note
* Modify history:
******************************************************************************/
#ifndef JSON_BASE_H
#define JSON_BASE_H

#include "os/os.h"
#include "CJsonObject.hpp"
#include "ContainerType.h"
#include "ReflectMacro.h"
#include "JsonException.h"

using std::map;
using std::string;

void json_base_init();

typedef bool(*ReflectTemplateFn)(neb::CJsonObject& oJsonSub, void* vec); 
typedef bool(*ReflectMapTemplateFn)(neb::CJsonObject& oJson, void* p, st_container_type _ct);

struct st_field_info
{
	bool _necessary;
	string _field_name;
	string _reflect_name;
	intptr_t _offset;
	st_container_type _container_type;
};
struct st_struct_info
{
	ReflectTemplateFn to_json_vector_fn;
	ReflectTemplateFn load_json_vector_fn;
	ReflectTemplateFn to_json_list_fn;
	ReflectTemplateFn load_json_list_fn;
	ReflectTemplateFn to_json_set_fn;
	ReflectTemplateFn load_json_set_fn;
	ReflectTemplateFn to_json_enum_fn;
	ReflectTemplateFn load_json_enum_fn;
	ReflectMapTemplateFn to_json_map_fn;
	ReflectMapTemplateFn load_json_map_fn;
    ReflectMapTemplateFn to_json_unordered_map_fn;
	ReflectMapTemplateFn load_json_unordered_map_fn;
	vector<st_field_info> _vec_field_info;
	st_struct_info()
	{
		to_json_vector_fn = nullptr;
		load_json_vector_fn = nullptr;
		to_json_list_fn = nullptr;
		load_json_list_fn = nullptr;
		to_json_set_fn = nullptr;
		load_json_set_fn = nullptr;
		to_json_enum_fn = nullptr;
		load_json_enum_fn = nullptr;
		to_json_map_fn = nullptr;
		load_json_map_fn = nullptr;
		to_json_unordered_map_fn = nullptr;
		load_json_unordered_map_fn = nullptr;
	}
};


struct st_json_base
{
protected:
    neb::CJsonObject to_json_base(neb::CJsonObject& oJson, st_struct_info _si) const;
    neb::CJsonObject to_json_struct(neb::CJsonObject& oJson, st_struct_info _si) const;
    neb::CJsonObject to_json_array(neb::CJsonObject& oJson, st_struct_info _si) const;
    neb::CJsonObject to_json_map(neb::CJsonObject& oJson, st_struct_info _si) const;
	st_struct_info* get_struct_info();
	void init_base();
public:
	static map<string, st_struct_info> _s_st_map;
	string _struct_name;
    string to_string() const;
    neb::CJsonObject to_json() const;
	bool load_json(const string strJson);
	bool load_json(neb::CJsonObject& oJson);
};

string GetJsonBaseVersion();

#endif

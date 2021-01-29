/*******************************************************************************
* @file     ReflectMacro.h
* @brief    Json
* @author   luoyu
* @date:    2019-12-06
* @note
* Modify history:
******************************************************************************/
#ifndef REFLECT_MARCO_H
#define REFLECT_MARCO_H
#include "os/os.h"
#include "ReflectTemplate.h"

#define STRUCT_INSTANCE(StructName) StructName();

#ifdef JSON_MAP_STRUCT
#define JSON_MAP_STRUCT_FN(StructName) \
		_si.to_json_map_fn = reflect_base<StructName>::to_json_map; \
		_si.load_json_map_fn = reflect_base<StructName>::load_json_map; 
#else
#define JSON_MAP_STRUCT_FN(StructName) 
#endif

#ifdef JSON_UNORDERED_MAP_STRUCT
#define JSON_UNORDERED_MAP_STRUCT_FN(StructName) \
		_si.to_json_unordered_map_fn = reflect_base<StructName>::to_json_unordered_map; \
		_si.load_json_unordered_map_fn = reflect_base<StructName>::load_json_unordered_map; 
#else
#define JSON_UNORDERED_MAP_STRUCT_FN(StructName) 
#endif

#define REFLECT_STRUCT_INIT(StructName) \
		json_base_init(); \
		_struct_name = #StructName; \
		if (_s_st_map.find(_struct_name) == _s_st_map.end()) { \
		st_struct_info _si; \
		_si.to_json_vector_fn = reflect_base<StructName>::to_json_vector; \
		_si.load_json_vector_fn = reflect_base<StructName>::load_json_vector; \
		_si.to_json_list_fn = reflect_base<StructName>::to_json_list; \
		_si.load_json_list_fn = reflect_base<StructName>::load_json_list; \
		_si.to_json_set_fn = reflect_base<StructName>::to_json_set; \
		_si.load_json_set_fn = reflect_base<StructName>::load_json_set; \
		JSON_MAP_STRUCT_FN(StructName) \
		JSON_UNORDERED_MAP_STRUCT_FN(StructName) \
		declare_field(_si); \
		_s_st_map[_struct_name] = _si; \
		} \
		init_base();

#define DECLARE_STRUCT(StructName) \
	StructName() {\
		REFLECT_STRUCT_INIT(StructName);\
	}\
	StructName(neb::CJsonObject& oJson) {\
		REFLECT_STRUCT_INIT(StructName);\
		load_json(oJson);\
	}\
	StructName(const string strJson) {\
		REFLECT_STRUCT_INIT(StructName);\
		load_json(strJson);\
	}\
	void declare_field(st_struct_info& _si)


#define DECLARE_ENUM(EnumName) \
	if (_s_st_map.find(EnumName) == _s_st_map.end()) {\
		st_struct_info _si; \
		_si.to_json_enum_fn = reflect_base<EnumName>::to_json_enum; \
		_si.load_json_enum_fn = reflect_base<EnumName>::load_json_enum; \
		_s_st_map[_struct_name] = _si; \
	}
		

#define INSERT_FIELD(Field,MapName,Necessary) {\
	st_field_info _fi; \
	_fi._necessary = Necessary; \
	if (_fi._container_type.set_container_type(TYPE_ID_NAME(Field))) {\
			_fi._field_name = #Field; \
			_fi._reflect_name = #MapName; \
            _fi._offset = reinterpret_cast<intptr_t>(&Field) - reinterpret_cast<intptr_t>(this); \
			_si._vec_field_info.push_back(_fi); \
	}\
}\

#define DECLARE_FIELD_NULL(Field)	INSERT_FIELD(Field,Field,false)

#define DECLARE_FIELD_NEEDED(Field)	INSERT_FIELD(Field,Field,true)

#define DECLARE_FIELD_NULL_M(Field,MappingName)	INSERT_FIELD(Field,MappingName,false)

#define DECLARE_FIELD_NEEDED_M(Field,MappingName)	INSERT_FIELD(Field,MappingName,true)



#endif // __REFLECT_MARCO_H

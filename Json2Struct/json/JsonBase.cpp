/*******************************************************************************
* @file     JsonBase.cpp
* @brief    Json
* @author   luoyu
* @date:    2019-12-06
* @note
* Modify history:
******************************************************************************/
#include "os/os.h"
#include "StringHelper.h"
#include "JsonBase.h"
using namespace std;
map<string, st_struct_info> st_json_base::_s_st_map;


bool st_json_base::load_json(neb::CJsonObject& oJson)
{
	if (_s_st_map.find(_struct_name) != _s_st_map.end())
	{
		st_struct_info _si = _s_st_map[_struct_name];
		unsigned int i = 0;
		for (; i < _si._vec_field_info.size(); ++i)
		{
            st_field_info _fi = _si._vec_field_info[i];
			neb::CJsonObject oJsonSub;
			st_container_type _ct = _fi._container_type;
			e_base_type _bt = _ct._type_name_key._base_type;
			e_base_container _bc = _ct._base_container;
			void* p = reinterpret_cast<void*>(reinterpret_cast<intptr_t>(this) + _fi._offset);
			if (_bc == bc_base && _bt != bt_struct)
			{
				if (!_g_base_type_info[_bt].load_json_fn(oJson, _fi._reflect_name, p) && _fi._necessary)
				{
					char _sz_msg[1000];
					sprintf(_sz_msg, "Json field %s:%s is necessary", _struct_name.c_str(), _fi._reflect_name.c_str());
					throw JsonException(_sz_msg);
				}
			}
			else if (oJson.Get(_fi._reflect_name, oJsonSub))
			{
				if (bc_base == _bc)
				{
                    reinterpret_cast<st_json_base*>(p)->load_json(oJsonSub);
				}
#ifdef JSON_MAP_STRUCT
				else if (bc_map == _bc)
				{
					string _str_key;
					e_base_type _bt_value = _ct._type_name_value._base_type;
					if (_bt_value == bt_struct)
					{
						if (_s_st_map.find(_ct._type_name_value._struct_name) != _s_st_map.end())
						{
							st_struct_info _si = _s_st_map[_ct._type_name_value._struct_name];
							_si.load_json_map_fn(oJsonSub, p, _ct);
						}
					}
					else
					{
						_ct.load_json_map_fn(oJsonSub, p);
					}
				}
#endif
#ifdef JSON_UNORDERED_MAP_STRUCT
				else if (bc_unordered_map == _bc)
				{
					string _str_key;
					e_base_type _bt_value = _ct._type_name_value._base_type;
					if (_bt_value == bt_struct)
					{
						if (_s_st_map.find(_ct._type_name_value._struct_name) != _s_st_map.end())
						{
							st_struct_info _si = _s_st_map[_fi._container_type._type_name_value._struct_name];
							_si.load_json_unordered_map_fn(oJsonSub, p, _ct);
						}
					}
					else
					{
						_ct.load_json_map_fn(oJsonSub, p);
					}
				}
#endif
				else if (bt_struct != _bt)
				{
					if (_bc == bc_vector)
					{
						_g_base_type_info[_bt].load_json_vector_fn(oJsonSub, p);
					}
					else if (_bc == bc_list)
					{
						_g_base_type_info[_bt].load_json_list_fn(oJsonSub, p);
					}
					else if (_bc == bc_set)
					{
						_g_base_type_info[_bt].load_json_set_fn(oJsonSub, p);
					}
				}
				else
				{
					if (_s_st_map.find(_ct._type_name_key._struct_name) != _s_st_map.end())
					{
						st_struct_info bs = _s_st_map[_fi._container_type._type_name_key._struct_name];
						if (_bc == bc_vector)
						{
							bs.load_json_vector_fn(oJsonSub, p);
						}
						else if (_bc == bc_list)
						{
							bs.load_json_list_fn(oJsonSub, p);
						}
						else if (_bc == bc_set)
						{
							bs.load_json_set_fn(oJsonSub, p);
						}
					}
				}
			}
			else if (_fi._necessary)
			{
				char _sz_msg[1000];
				sprintf(_sz_msg, "field %s:%s is necessary", _struct_name.c_str(), _fi._reflect_name.c_str());
				throw JsonException(_sz_msg);
			}
		}
	}
	return true;
}


bool st_json_base::load_json(const string strJson)
{
	neb::CJsonObject oJson(strJson);
	return load_json(oJson);
}

neb::CJsonObject st_json_base::to_json_base(neb::CJsonObject& oJson, st_struct_info _si) const
{
	unsigned int i = 0;
	for (; i < _si._vec_field_info.size(); ++i)
	{
		st_field_info _fi = _si._vec_field_info[i];
		string _rn = _fi._reflect_name;
		st_container_type _ct = _fi._container_type;
		e_base_type _bt = _ct._type_name_key._base_type;
		e_base_container _bc = _ct._base_container;
		void* p = reinterpret_cast<void*>(reinterpret_cast<intptr_t>(this) + _fi._offset);
		if (_bc == bc_base && _bt != bt_struct)
		{
			_g_base_type_info[_bt].to_json_fn(oJson, _rn, p);
		}
	}
	return oJson;
}

neb::CJsonObject st_json_base::to_json_struct(neb::CJsonObject& oJson, st_struct_info _si) const
{
	unsigned int i = 0;
	for (; i < _si._vec_field_info.size(); ++i)
	{
		st_field_info _fi = _si._vec_field_info[i];
		string _rn = _fi._reflect_name;
		st_container_type _ct = _fi._container_type;
		e_base_type _bt = _ct._type_name_key._base_type;
		e_base_container _bc = _ct._base_container;
        void* p = reinterpret_cast<void*>(reinterpret_cast<intptr_t>(this) + _fi._offset);
		if (_bc == bc_base && _bt == bt_struct)
		{
            oJson.Add(_rn, (reinterpret_cast<st_json_base*>(p))->to_json());
		}
	}
	return oJson;
}

neb::CJsonObject st_json_base::to_json_array(neb::CJsonObject& oJson, st_struct_info _si) const
{
	unsigned int i = 0;
	for (; i < _si._vec_field_info.size(); ++i)
	{
		st_field_info _fi = _si._vec_field_info[i];
		string _rn = _fi._reflect_name;
		st_container_type _ct = _fi._container_type;
		e_base_type _bt = _ct._type_name_key._base_type;
		e_base_container _bc = _ct._base_container;
		string _fsn = _ct._type_name_key._struct_name;
		void* p = reinterpret_cast<void*>(reinterpret_cast<intptr_t>(this) + _fi._offset);
		if (_bc != bc_map && _bc != bc_unordered_map && _bc != bc_base)
		{
			oJson.AddEmptySubArray(_rn);
			neb::CJsonObject& oJsonSub = oJson[_rn];
			if (_bt != bt_struct)
			{
				if (_bc == bc_vector)
				{
					_g_base_type_info[_bt].to_json_vector_fn(oJsonSub, p);
				}
				else if (_bc == bc_list)
				{
					_g_base_type_info[_bt].to_json_list_fn(oJsonSub, p);
				}
				else if (_bc == bc_set)
				{
					_g_base_type_info[_bt].to_json_set_fn(oJsonSub, p);
				}
			}
			else
			{
				if (_s_st_map.find(_fsn) != _s_st_map.end())
				{
					st_struct_info bs = _s_st_map[_fsn];
					if (_bc == bc_vector)
					{
						bs.to_json_vector_fn(oJsonSub, p);
					}
					else if (_bc == bc_list)
					{
						bs.to_json_list_fn(oJsonSub, p);
					}
					else if (_bc == bc_set)
					{
						bs.to_json_set_fn(oJsonSub, p);
					}
				}
			}
		}
	}
	return oJson;
}


neb::CJsonObject st_json_base::to_json_map(neb::CJsonObject& oJson, st_struct_info _si) const
{
	unsigned int i = 0;
	for (; i < _si._vec_field_info.size(); ++i)
	{
		st_field_info _fi = _si._vec_field_info[i];
		string _rn = _fi._reflect_name;
		st_container_type _ct = _fi._container_type;
		e_base_container _bc = _ct._base_container;
		void* p = reinterpret_cast<void*>(reinterpret_cast<intptr_t>(this) + _fi._offset);
		if (_bc == bc_map || _bc == bc_unordered_map)
		{
			e_base_type _bt_value = _ct._type_name_value._base_type;
			neb::CJsonObject oJsonSub("{}");
			if (_bt_value == bt_struct)
			{
				if (_s_st_map.find(_ct._type_name_value._struct_name) != _s_st_map.end())
				{
					st_struct_info _si = _s_st_map[_ct._type_name_value._struct_name];
					if (bc_map == _bc)
					{
						if(_si.to_json_map_fn)
							_si.to_json_map_fn(oJsonSub, p, _ct);
					}
					else
					{
						if(_si.to_json_unordered_map_fn)
							_si.to_json_unordered_map_fn(oJsonSub, p, _ct);
					}
				}
			}
			else
			{
				if(_ct.to_json_map_fn)
					_ct.to_json_map_fn(oJsonSub, p);
			}
			oJson.Add(_rn, oJsonSub);
		}
	}
	return oJson;
}


neb::CJsonObject st_json_base::to_json() const
{
	neb::CJsonObject oJson;
	if (_s_st_map.find(_struct_name) != _s_st_map.end())
	{
		st_struct_info _si = _s_st_map[_struct_name];
		to_json_base(oJson, _si);
		to_json_struct(oJson, _si);
		to_json_array(oJson, _si);
		to_json_map(oJson, _si);
		return oJson;
		//return to_json_map(to_json_array(to_json_struct(to_json_base(oJson, _si), _si), _si), _si);
	}
	return oJson;
}


string st_json_base::to_string() const
{
	return to_json().ToString();
}


void st_json_base::init_base()
{
	neb::CJsonObject oJson;
	if (_s_st_map.find(_struct_name) != _s_st_map.end())
	{
		st_struct_info _si = _s_st_map[_struct_name];
		unsigned int i = 0;
		for (; i < _si._vec_field_info.size(); ++i)
		{
			st_field_info _fi = _si._vec_field_info[i];
            void* p = reinterpret_cast<void*>(reinterpret_cast<intptr_t>(this) + _fi._offset);
			st_container_type _ct = _fi._container_type;
			if (_ct._base_container == bc_base)
			{
				st_type_name _tn = _ct._type_name_key;
				if (_tn._base_type == bt_struct)
				{
                    st_json_base& _st = *reinterpret_cast<st_json_base*>(p);
					_st.init_base();
				}
				else if (_tn._base_type != bt_string)
				{
                    _g_base_type_info[_tn._base_type].init_base_type_fn(p);
				}
			}
		}
	}
}


st_struct_info* st_json_base::get_struct_info()
{
	if (_s_st_map.find(_struct_name) != _s_st_map.end())
	{
		st_struct_info& _si = _s_st_map[_struct_name];
		return &_si;
	}
    return nullptr;
}


string GetJsonBaseVersion()
{
	return "1.0";
}


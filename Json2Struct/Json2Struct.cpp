// Json2Struct.cpp : 定义控制台应用程序的入口点。
//

#include "json/JsonBase.h"

struct st_course : public st_json_base
{
	uint32 id;
	string name;
	float64 score;
	DECLARE_STRUCT(st_course)
	{
		DECLARE_FIELD_NULL(id)
		DECLARE_FIELD_NULL(name)
		DECLARE_FIELD_NULL(score)
	}
	//所有结构必须能够比较
	bool operator<(const st_course& s) const
	{
		return this->id < s.id;
	}
};

struct st_student : public st_json_base
{
	uint32 id;
	string name;
	bool m_sex;
	int m_age;
	list<uint32> check_day_list;
	vector<st_course> st_course_vector;
	DECLARE_STRUCT(st_student)
	{
		//必须字段
		DECLARE_FIELD_NEEDED(id)
		//不必须字段
		DECLARE_FIELD_NULL(name)
		//必须字段映射
		DECLARE_FIELD_NEEDED_M(m_sex, sex)
		//不必须字段映射
		DECLARE_FIELD_NULL_M(m_age, age)
		//普通列表数组结合
		DECLARE_FIELD_NULL(check_day_list)
		//数组结构体字段
		//必须先声明结构体变量
		STRUCT_INSTANCE(st_course)
		DECLARE_FIELD_NULL(st_course_vector)
	}
	//所有结构必须能够比较
	bool operator<(const st_student& s) const
	{
		return this->id < s.id;
	}
};


void test_2_struct(string& str)
{
	try
	{
		st_student stu(str);
		cout << stu.id << "\t" << stu.name << "\t" << stu.m_sex << "\t" << stu.m_sex << endl;
		for (auto it = stu.check_day_list.begin(); it != stu.check_day_list.end(); ++it)
		{
			cout << *it << " ";
		}
		cout << endl;
		for (auto it = stu.st_course_vector.begin(); it != stu.st_course_vector.end(); ++it)
		{
			cout << it->id << "\t" << it->name << "\t" << it->score << endl;
		}
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}
}


string test_2_json()
{
	try
	{
		st_student stu;
		stu.id = 1002;
		stu.name = "张三";
		stu.m_sex = false;
		stu.m_age = 16;
		for (uint32 i = 0; i < 30; ++i)
		{
			stu.check_day_list.push_back(i);
		}
		st_course cour;
		cour.id = 1;
		cour.name = "语文";
		cour.score = 90;
		stu.st_course_vector.push_back(cour);
		cour.id = 2;
		cour.name = "数学";
		cour.score = 95;
		stu.st_course_vector.push_back(cour);
		cour.id = 3;
		cour.name = "英语";
		cour.score = 100;
		stu.st_course_vector.push_back(cour);
		return stu.to_string();

	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}
	return "";
}


int main()
{
	string json = test_2_json();
	cout << json << endl;
	test_2_struct(json);
    return 0;
}


// Json2Struct.cpp : �������̨Ӧ�ó������ڵ㡣
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
	//���нṹ�����ܹ��Ƚ�
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
		//�����ֶ�
		DECLARE_FIELD_NEEDED(id)
		//�������ֶ�
		DECLARE_FIELD_NULL(name)
		//�����ֶ�ӳ��
		DECLARE_FIELD_NEEDED_M(m_sex, sex)
		//�������ֶ�ӳ��
		DECLARE_FIELD_NULL_M(m_age, age)
		//��ͨ�б�������
		DECLARE_FIELD_NULL(check_day_list)
		//����ṹ���ֶ�
		//�����������ṹ�����
		STRUCT_INSTANCE(st_course)
		DECLARE_FIELD_NULL(st_course_vector)
	}
	//���нṹ�����ܹ��Ƚ�
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
		stu.name = "����";
		stu.m_sex = false;
		stu.m_age = 16;
		for (uint32 i = 0; i < 30; ++i)
		{
			stu.check_day_list.push_back(i);
		}
		st_course cour;
		cour.id = 1;
		cour.name = "����";
		cour.score = 90;
		stu.st_course_vector.push_back(cour);
		cour.id = 2;
		cour.name = "��ѧ";
		cour.score = 95;
		stu.st_course_vector.push_back(cour);
		cour.id = 3;
		cour.name = "Ӣ��";
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


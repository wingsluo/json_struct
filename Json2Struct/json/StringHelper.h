#ifndef  STRING_HELPER_H
#define  STRING_HELPER_H

#include "os/os.h"

using std::string;
using std::vector;
using std::wstring;

#define STR_TRANS(_Str,_Value) \
    std::stringstream stream(_Str);\
	stream >> _Value;


template<typename T>
string inline TRANS_STR(T _val)
{
    std::stringstream stream;
	stream << _val;
	return stream.str();
};


namespace utility
{
	// trim from start
	string inline &ltrim(string &s)
	{
		//' '	(0x20)	space (SPC)
		//	'\t'	(0x09)	horizontal tab (TAB)
		//	'\n'	(0x0a)	newline (LF)
		//	'\v'	(0x0b)	vertical tab (VT)
		//	'\f'	(0x0c)	feed (FF)
		//	'\r'	(0x0d)	carriage return (CR)

		string::iterator itr = s.begin();
		for (; itr != s.end(); ++itr)
		{
			if (*itr == ' ' || *itr == '\t' || *itr == '\n'
				|| *itr == '\v' || *itr == '\f' || *itr == '\r')
			{
				continue;
			}
			break;
		}
		s.erase(s.begin(), itr);
		return s;
	}

	// trim from end
	string inline &rtrim(string &s)
	{
		if (!s.empty())
		{
			string::iterator itr = s.end();
			size_t i = s.length();
			for (; i > 0; --i)
			{
				if (s[i - 1] == ' ' || s[i - 1] == '\t' || s[i - 1] == '\n'
					|| s[i - 1] == '\v' || s[i - 1] == '\f' || s[i - 1] == '\r')
				{
					--itr;
					continue;
				}
				break;
			}
			s.erase(itr, s.end());
		}
		return s;
	}

	// trim from both ends
	string inline &trim(string &s)
	{
		return ltrim(rtrim(s));
	}

	size_t inline split_string(const string& str,
		const string& split,
		vector<string>& ls,
		bool hasEmpty = false)
	{
		ls.clear();
		if (str.empty())	return 0;

		string::size_type pos1, pos2;
		pos2 = str.find(split);
		pos1 = 0;
		while (string::npos != pos2)
		{
			string tmp = str.substr(pos1, pos2 - pos1);
			pos1 = pos2 + 1;
			pos2 = str.find(split, pos1);
			if (hasEmpty)
			{
				ls.push_back(tmp);
			}
			else
			{
				trim(tmp);
				if (!tmp.empty())
				{
					ls.push_back(tmp);
				}
			}
		}
		string tmp = str.substr(pos1);
		if (hasEmpty)
		{
			ls.push_back(tmp);
		}
		else
		{
			trim(tmp);
			if (!tmp.empty())
			{
				ls.push_back(tmp);
			}
		}
		return ls.size();
	}

}
#endif

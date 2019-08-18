#include "utilites.h"
#include <algorithm>

using namespace std;

#ifdef _DEBUG
#include <test_runner.h>

void SpitString_test()
{
	{
		string str("first second third");
		vector<string> expected = { "first", "second", "third" };
		ASSERT(expected == SplitString(str));
		str = "first second third   ";
		ASSERT(expected == SplitString(str));
		str = "   first   second  third   ";
		ASSERT(expected == SplitString(str));
	}
	{
		string str("first-second-third");
		vector<string> expected = { "first", "second", "third" };
		ASSERT(expected == SplitString(str, '-'));
		str = "first-second-third----";
		ASSERT(expected == SplitString(str, '-'));
		str = "----first---second-third----";
		ASSERT(expected == SplitString(str, '-'));
	}
}
//-----------------------------------------------
void GetPrefix_test()
{
	{
		string test_str("test string");
		auto result = GetPrefix(test_str);
		ASSERT_EQUAL(result, "test");
		ASSERT_EQUAL(test_str, "string");

		test_str = " test string";
		result = GetPrefix(test_str);
		ASSERT_EQUAL(result, "");
		ASSERT_EQUAL(test_str, "test string");

		test_str = "test_string";
		result = GetPrefix(test_str);
		ASSERT_EQUAL(result, "");
		ASSERT_EQUAL(test_str, "test_string");
	}

}
//-----------------------------------------------
void ClearPrefixSuffix_test()
{
	string str = "  test string  ";
	ClearPrefix(str, ' ');
	ASSERT_EQUAL(str, "test string  ");
	ClearSufix(str, ' ');
	ASSERT_EQUAL(str, "test string");
}
//-----------------------------------------------
void GetStopesMap_test()
{
	//  55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam
	unordered_map<string, int> expected =
	{
		{"Rossoshanskaya ulitsa", 7500},
		{"Biryusinka", 1800},
		{"Universam", 2400}
	};
	auto test_vector = SplitString("55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam", ',');
	ASSERT_EQUAL(expected, GetStopesMap(test_vector));

}

#endif // _DEBUG

vector<string> SplitString(const string &source, char delim)
{
	vector<string> result;
	if (source.empty())
		return result;
	size_t index = 0;
	do
	{
		auto pos = source.find(delim, index);
		if (pos - index > 0)
			result.push_back(source.substr(index, pos - index));
		index = pos + 1;

	} while (index != 0);

	if (result.back().empty())
		result.pop_back();

	return result;
}
//-------------------------------------------------------------------
string GetPrefix(string &source, char delim)
{
	auto index = source.find(delim, 0);
	if (index < source.size())
	{
		auto result = source.substr(0, index);
		source.erase(0, index + 1);
		return result;
	}
	return "";
}
//-------------------------------------------------------------------
void ClearSufix(string &str, char ch)
{
	if (str.empty())
		return;
	while (str.back() == ch)
		str.pop_back();
}
//-------------------------------------------------------------------
void ClearPrefix(string &str, char ch)
{
	if (str.empty())
		return;
	auto it = find_if(str.begin(), str.end(), [=](char c) {return c != ch; });
	if (it != str.end())
		str.erase(str.begin(), it);
}
//-------------------------------------------------------------------
unordered_map<string, int> GetStopesMap(vector<string> &vec)
{
	static const string find_template = "m to ";
	unordered_map<string, int> result;
	for (auto &str : vec)
	{
		auto it = search(str.begin(), str.end(), find_template.begin(), find_template.end());
		if (it != str.end())
		{
			string number(str.begin(), it);
			int dist = stoi(number.c_str());
			str.erase(str.begin(), it + find_template.size());
			result[str] = dist;
		}
	}
	return result;
}
//-----------------------------------------------------------------------------
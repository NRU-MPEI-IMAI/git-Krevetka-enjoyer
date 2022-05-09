#pragma once
#include "string"
#include "vector"
#include "unordered_map"
#include "fstream"
#include <exception>
using std::string;
using std::vector;
using std::make_pair;
typedef std::unordered_map<char, string> map;
class Perehod
{
	string x;
	map y;
	string lambda;
	public:
	Perehod(const string x, const vector<char> a) :x(x),lambda("")
	{
		for (int i = 0; i < a.size(); ++i)
		{
			string z = "";
			y.insert(make_pair(a[i], z)); 
		}
	}
	Perehod()
	{}
	const string& Shag(char a) const
	{
		if (a != '~')
		{

			string w;
			try
			{
				return y.at(a);
			}
			catch (const std::exception& e)
			{
				return "";
			}
		}
		else
		{
			if (lambda.size() > 0)
				return lambda;
			else
				return "";
		}
	}
	void Dob(const char a, const string b)
	{
		if (a != '~')
			y[a] = b;
		else
		{
			if (lambda == "")
				lambda = b;
			else
				lambda += ',' + b;
		}
	}
	string Naz() const { return x; }

};


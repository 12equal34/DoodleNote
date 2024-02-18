#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <cassert>

namespace Combination1
{
	using namespace std;

	vector<string> F(const string& Str,map<string,vector<string>>& Memo)
	{
		if (Str.size() == 0) return {};
		else if (Str.size() == 1) return {string{},string(1,Str[0])};

		const string Substr = Str.substr(1);
		if (!Memo.contains(Substr)) Memo[Substr] = F(Substr, Memo);
		vector<string> f = Memo[Substr];
		vector<string> result = f;
		for (const string& s : f)
		{
			result.push_back(Str[0] + s);
		}
		return result;
	}

	void Test()
	{
		map<string,vector<string>> Memo;
		vector<string> SortedCombination = F("ABCDE",Memo);
		sort(SortedCombination.begin(),SortedCombination.end());

		for (const string& str : SortedCombination)
		{
			cout << str << endl;
		}
		cout << "number of combination: " << SortedCombination.size();

		cout << endl;

		for (const auto& [key,value] : Memo)
		{
			cout << "key: " << key << "-> ";
			for (const string& str : value) {
				cout << str << " ";
			}
			cout << endl << endl;
		}
	}
}

namespace Combination2
{
	using namespace std;
	using namespace std::string_literals;

	void F(string src, string output)
	{
		cout << output << endl;

		for (int i = 0; i < src.size(); i++)
		{
			F(src.substr(i+1), output + src[i]);
		}
	}

	void G(const string& src,int f, int l, string output)
	{
		cout << output << endl;

		for (int i = f; i < l; i++)
		{
			G(src, i+1, l, output + src[i]);
		}
	}

	void H(const string& src,int f, int l, vector<string>& outputs, int b)
	{
		assert(outputs.size() >= 1);

		for (int i = f; i < l; i++)
		{
			outputs.push_back(outputs[b] + src[i]);
			H(src, i+1, l, outputs, static_cast<int>(outputs.size()) - 1);
		}
	}

	void Test()
	{
		F("ABCDE"s, ""s);
		cout << endl;
		G("ABCDE"s, 0, 5, ""s);
		cout << endl;

		const string source = "ABCDE"s;
		vector<string> outputs(1, ""s);
		H(source, 0, source.size(), outputs, 0);
		for (auto& o : outputs) cout << o << endl;
	}
}


int main()
{
	std::cout << -26 / 5 << ","<< -26 % 5 << std::endl;
}
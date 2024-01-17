#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>

using namespace std;

map<string, vector<string>> Memo;

vector<string> F(const string& Str)
{
	if (Str.size() == 0) return {};
	else if (Str.size() == 1) return { string{}, string(1, Str[0]) };

	const string Substr = Str.substr(1);
	if (!Memo.contains(Substr)) Memo[Substr] = F(Substr);
	vector<string> f = Memo[Substr];
	vector<string> result = f;
	for (const string& s : f)
	{
		result.push_back(Str[0] + s);
	}
	return result;
}

int main()
{
	vector<string> SortedCombination = F("ABCDE");
	sort(SortedCombination.begin(), SortedCombination.end());

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
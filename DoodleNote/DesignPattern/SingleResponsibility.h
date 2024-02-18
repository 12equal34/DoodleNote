#pragma once

// Single Responsibility Principle (SRP): states "A module should be responsible to one, and only one, actor."
// The term actor refers to a group (consisting of one or more stakeholders or users) that requires a change in the module.
// "A class should have only one reason to change."
// "Gather together the things that change for the same reasons. Separate those things that change for different reasons."
// 하나의 클래스는 반드시 변경되는 이유가 오직 하나여야 한다.
// 같은 이유로 변경되는 것들을 함께 모으고, 다른 이유로 변경되는 것들은 분리해야 한다.
// 그래서 각각의 모듈은 각자의 역할을 책임져야 한다.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Journal
{
    string title;
    vector<string> entries;

    explicit Journal(const string& title)
        : title{title}
    {
    }

    void add(const string& entry);

    // persistence is a separate concern.. This violates the SRP.
    void save(const string& filename);
};

void Journal::add(const string& entry)
{
    static int count = 1;
    entries.push_back(to_string(count++) + ": " + entry);
}

void Journal::save(const string& filename)
{
    ofstream ofs(filename);
    for (auto& s : entries)
        ofs << s << endl;
}

struct PersistenceManager
{
    static void save(const Journal& j, const string& filename)
    {
        ofstream ofs(filename);
        for (auto& s : j.entries)
            ofs << s << endl;
    }
};

void main()
{
    Journal journal{"Dear Diary"};
    journal.add("I ate a bug");
    journal.add("I cried today");

    // This violates the SRP.
    //journal.save("diary.txt");

    PersistenceManager pm;
    pm.save(journal, "diary.txt");
}
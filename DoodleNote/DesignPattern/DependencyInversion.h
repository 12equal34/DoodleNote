#pragma once

// Dependency Inversion Principle (DIP): is a specific methodology for
// loosely coupled software modules.
// When following this principle, the conventional dependency relationships
// established from high-level, policy-setting modules to low-level,
// dependency modules are reversed, thus rendering high-level modules
// independent of the low-level module implementation details.
// 통상적으로 고수준 모듈은 저수준 모듈에 의존한다. 하지만 이러한 의존관계를 뒤집어서
// 고수준 모듈이 저수준 모듈의 구현 디테일과 무관하게 만들어야 한다.
// 이러한 원칙을 의존관계 역전 원칙이라 한다. 
// 원칙은 다음과 같다.
// The principle states:
// A: High-level modules should not import anything from low-level modules.
//    Both should depend on abstractions (e.g., interfaces).
//    고수준 모듈은 저수준 모듈의 어떠한 것도 가져와서 이용하면 안된다.
//    고수준/저수준 모듈은 모두 추상화(즉, 인터페이스)에 의존해야 한다.
// B: Abstractions should not depend on details.
//    Details (concrete implementations) should depend on abstractions.
//    추상화는 구현 디테일과 무관해야 한다. 반대로 구현 디테일은 추상화에 의존해야 한다.
//    여기서 구현 디테일은 concrete implementations을 의미한다.
// 
// 고수준 모듈과 저수준 모듈의 Interaction을 디자인할 때,
// the interaction을 모듈 사이의 abstract interaction로 생각해야한다.
// 이는 고수준 모듈뿐만 아니라 저수준 모듈 또한 이를 염두에 두고 디자인해야한다.

#include <vector>
#include <iostream>

using namespace std;

enum class ERelationship
{
	Parent,
	Child,
	Sibling
};

struct Person
{
	string Name;
};

struct RelationshipBrowser
{
	virtual vector<Person> FindAllChildrenOf(const Person& InPerson) const = 0;
};

// a low-level module
struct Relations : RelationshipBrowser
{
	using RelationType = tuple<Person, ERelationship, Person>;
	vector<RelationType> Relations;

	void AddParentAndChild(Person Parent,Person Child)
	{
		Relations.push_back(RelationType{Parent, ERelationship::Parent, Child});
		Relations.push_back(RelationType{Child, ERelationship::Child, Parent});
	}

	virtual vector<Person> FindAllChildrenOf(const Person& InPerson) const override
	{
		vector<Person> Result;
		for (auto&& [Parent,Relation,Child] : Relations)
			if (Parent.Name == InPerson.Name)
				Result.push_back(Child);
		return Result;
	}
};

// a high-level module
struct Reserch
{
	// Interaction이 저수준 모듈에 의존하고 있다.
	/*void ReserchBy(const Relations& InRelations)
	{
		auto&& Relations = InRelations.Relations;
		for (auto&& [Parent,Relation,Child] : Relations)
		{
			if (Parent.Name == "John" && Relation == ERelationship::Parent) {
				cout << Parent.Name << " has a child called " << Child.Name << endl;
			}
		}
	}*/

	// Interaction이 추상화에 의존하고 있다.
	void ReserchBy(const RelationshipBrowser& Browser)
	{
		Person Parent {"John"};
		for (auto&& Child : Browser.FindAllChildrenOf(Parent))
		{
			cout << Parent.Name << " has a child called " << Child.Name << endl;
		}
	}
};

void Test()
{
	Relations relations;
	relations.AddParentAndChild(Person{"John"},Person{"James"});
	relations.AddParentAndChild(Person{"John"},Person{"Kim"});
	relations.AddParentAndChild(Person{"Mon"}, Person{"Su"});
	relations.AddParentAndChild(Person{"Mon"}, Person{"Yu"});

	Reserch r;
	r.ReserchBy(relations);
}
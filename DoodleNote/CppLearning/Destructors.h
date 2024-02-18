#pragma once

// Implicitly-declared destructor:
// 사용자 정의 소멸자가 없다면, 컴파일러는 항상 inline public 맴버 소멸자를 선언한다.
// 모든 암시적 선언 특별 맴버함수들처럼 잠재적으로 베이스의 소멸자가 예외를 던지지 않는 한 noexcept이다.
// Implicitly-defined destructor:
// 암시적 선언 소멸자를 delete하지 않았다면, 암시적으로 empty body로 정의된다.

// Guidline:
// 소멸자는 반드시 public and virtual 이거나 protected and nonvirtual 이어야 한다.

// 직접 다음과 같이 호출할 수도 있지만 거의 사용하지 않는다.
// this->~ClassName();
// obj->~ClassName();
// 보통은 delete를 사용하여 소멸자를 자동호출한다.

namespace Nonpublic
{
	// 만약 소멸자가 public이 아니면,
	class A
	{
	protected:
		~A() {}

		void internal_method()
		{
			A a;
			A* pa = new A;
			delete pa;
		}
	};

	void external_func()
	{
		// 다음 스택 할당은 접근제한 오류난다.
		// A a;

		// 다음 힙 할당은 가능하다.
		A* a = new A;

		// 그러나 delete는 접근제한 오류난다.
		// delete a;
		// 결과적으로 포인터를 잃어버린다.
	}
}

#include <iostream>

using namespace std;

// base 포인터를 통해 polymorhic deletion을 할 때 사용한다.
namespace Public_And_Virtual
{
	class Base
	{
		string* Name;
	public:
		Base()
		{
			Name = new string { "base" };
		}
		virtual ~Base()
		{
			cout << "Base dtor" << endl;
			delete Name;
		}
	};

	class Derived: public Base
	{
		int* Integer;
	public:
		Derived()
		{
			Integer = new int { 5 };
		}
		virtual ~Derived() override
		{
			cout << "Derived dtor" << endl;
			delete Integer;
		}
	};

	void Heap_Alloc()
	{
		Base* var = new Derived;
		// delete는 소멸자를 호출하므로 컴파일러가 접근제한 유무를 판단한다.
		delete var;
		// 만약 Base가 protected 소멸자를 가지면, delete는 접근제한 오류가 난다.
	}

	void Stack_Alloc()
	{
		{
			Base var;    // Ok
		}
		cout << endl;
		{
			Derived foo; // Ok
		}
	}
}

namespace Public_And_Nonvirtual // 문제가 발생
{
	class Base
	{
		string* Name;
	public:
		Base()
		{
			Name = new string { "base" };
		}
		~Base()
		{
			cout << "Base dtor" << endl;
			delete Name;
		}
	};

	class Derived: public Base
	{
		int* Integer;
	public:
		Derived()
		{
			Integer = new int { 5 };
		}
		~Derived()
		{
			cout << "Derived dtor" << endl;
			delete Integer;
		}
	};

	void Heap_Alloc()
	{
		Base* var = new Derived;
		// Name은 정리되지만,
		// Derived 소멸자가 호출되지 않으므로 Integer가 정리되지 않는다.
		delete var;
	}

	void Stack_Alloc()
	{
		{
			Base var;    // Ok
		}
		cout << endl;
		{
			Derived foo; // Ok
			// 스택 할당 해제는 nonvirtual이어도 상속된 소멸자 모두 호출하므로
			// 문제없이 Name, Integer 모두 정리된다.
		}
	}
}

// 위 문제가 발생하는 것을 방지한다. 
namespace Protected_And_Nonvirtual 
{
	// 다형적 소멸을 원하지 않는 경우에 사용해야 한다.
	
	// 만약 다형적 소멸을 해야할 때 사용한다면,
	class Base
	{
		string* Name;
	public:
		Base()
		{
			Name = new string { "base" };
		}
	protected:
		~Base()
		{
			cout << "Base dtor" << endl;
			delete Name;
		}
	};

	class Derived: public Base
	{
		int* Integer;
	public:
		Derived()
		{
			Integer = new int { 5 };
		}
		~Derived()
		{
			cout << "Derived dtor" << endl;
			delete Integer;
		}
	};

	void Heap_Alloc()
	{
		Base* var = new Derived;
		// 접근 제한 오류이므로 사전에 방지한다.
		//delete var;

		// 여기서는 할당한 객체를 가리키는 포인터를 잃어버린다.
		// 하지만 이는 깜빡하고 포인터를 잃어버리는 경우이므로 별개의 일이다.
		// 포인터를 수동으로 delete하거나 스마트포인터의 내부 코드에서 delete를 호출하므로
		// 언젠가 delete를 작성하면 컴파일 에러가 발생할 것이다.
		// 그러므로 위의 문제가 사전 방지된 것이다.
	}

	void Stack_Alloc()
	{
		// Base var; 접근 오류
		Derived foo; // Ok
	}
}

// 어떠한 순수 가상매서드도 갖지 않는 추상 클래스가 필요할 때 사용한다.
namespace Public_Pure_Virtual
{
	class AbstractBase
	{
	public:
		virtual ~AbstractBase() = 0;
	};
	AbstractBase::~AbstractBase() {} // 순수 가상 소멸자는 반드시 한 개의 정의를 가져야 한다.

	class Derived : public AbstractBase {};

	// AbstractBase obj; // compiler error
	Derived obj;         // OK
}
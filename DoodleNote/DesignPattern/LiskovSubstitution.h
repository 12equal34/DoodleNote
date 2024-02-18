#pragma once

// Category: Subtyping

// -리스코프 치환 원칙 (LSP, Liskov Substitution Principle)-
// "자료형 S가 자료형 T의 서브타입이라면,
// 필요한 프로그램의 속성의 변경 없이,
// 자료형 T의 객체를 자료형 S의 객체로 치환할 수 있어야 한다."
// 정의는 다음과 같다.
// "Let p(x) be a property provable about objects x of type T.
// Then p(x) should be true for objects y of type S where S is a subtype of T."
// Symbolically:
// "S<=T -> ∀x:T.p(x) -> ∀y:S.p(y)"

#include <vector>
#include <iostream>

using namespace std;

namespace Problematic_Case_In_LSP
{
	class Rectangle
	{
		// postcondition: 너비와 높이를 독립적으로 변경할 수 있다.
	protected:
		int width;
		int height;

	public:
		Rectangle(int w, int h) : width(w), height(h) {}

		int Area() const { return width * height; }

		int GetWidth() const { return width; }
		int GetHeight() const { return height; }
		virtual void SetWidth(int w) { width = w; }
		virtual void SetHeight(int h) { height = h; }
	};

	class Square: public Rectangle
	{
		// invariant: 너비와 높이는 항상 같다.
	public:
		Square(int size) : Rectangle(size,size) {}

		virtual void SetWidth(int w) override  { width = height = w;}
		virtual void SetHeight(int h) override { width = height = h;}
	};

	void Process(Rectangle& r)
	{
		const int h = r.GetHeight();
		const int changed_w = 10;
		r.SetWidth(changed_w);
		cout << "Area: " << r.Area() << endl;
		cout << "Expected: " << changed_w * h << endl;
	}

	void Test()
	{
		Rectangle r{5,5};
		Process(r);
		Square s{5};
		Process(s);
	}
	// Area: 50
	// Expected: 50
	// Area: 100
	// Expected: 50

	// If you uses this factory, then you can avoid Problematic cass.
	struct RectangleFactory
	{
		static Rectangle CreateRectangle(int w, int h) { return Rectangle{w,h};}
		static Square	 CreateSquare(int size) { return Square(size);}
	};
	// But, It does not solve the problem fundamentally.
}
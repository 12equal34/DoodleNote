#pragma once

// Open–Closed Principle (OCP): states that software entities (classes, modules, functions, etc.) 
// should be open for extension, but closed for modification.
// Such an entity can allow its behaviour to be extended without modifying its source code.

#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum class EColor { Red, Green, Blue };
enum class ESize  { Large, Medium, Small };

#define COLOR_CASE(X,...) case EColor::X: {ColorStr = #X; break;}
#define SIZE_CASE(X,...) case ESize::X: {SizeStr = #X; break;}

ostream& operator<<(ostream& os,EColor Color)
{
	string ColorStr;
	switch (Color)
	{
		COLOR_CASE(Red);
		COLOR_CASE(Green);
		COLOR_CASE(Blue);
	}
	return os << ColorStr;
}

ostream& operator<<(ostream& os,ESize Size)
{
	string SizeStr;
	switch (Size)
	{
		SIZE_CASE(Large);
		SIZE_CASE(Medium);
		SIZE_CASE(Small);
	}
	return os << SizeStr;
}

#undef COLOR_CASE
#undef SIZE_CASE

struct Product
{
	string Name;
	EColor Color;
	ESize  Size;
};

ostream& operator<<(ostream& os,Product& InProduct)
{
	return os << InProduct.Name << " has " << InProduct.Color << " color and " << InProduct.Size << " size.";
}

namespace Problematic_Case_OCP
{
	struct ProductFilter
	{
		using Items = vector<Product*>;

		static Items FilterByColor(const Items& Products,EColor Color)
		{
			Items Result;
			for (Product* Product : Products)
				if (Product->Color == Color)
					Result.push_back(Product);
			return Result;
		}

		static Items FilterBySize(const Items& Products,ESize Size)
		{
			Items Result;
			for (Product* Product : Products)
				if (Product->Size == Size)
					Result.push_back(Product);
			return Result;
		}

		static Items FilterByColorAndSize(const Items& Products,EColor Color,ESize Size)
		{
			Items Result;
			for (Product* Product : Products)
				if (Product->Color == Color && Product->Size == Size)
					Result.push_back(Product);
			return Result;
		}
	};

	void Test()
	{
		Product Apple{"Apple", EColor::Green, ESize::Small};
		Product Tree{"Tree", EColor::Green, ESize::Large};
		Product House{"House", EColor::Blue, ESize::Large};

		const vector<Product*> All { &Apple, &Tree, &House };
		for (auto product : All) {
			cout << *product << endl;
		}
		cout << endl;

		ProductFilter PF;
		auto FilteredByGreen = PF.FilterByColor(All, EColor::Green);
		for (auto product : FilteredByGreen) {
			cout << *product << endl;
		}
		cout << endl;

		auto FilteredByLarge = PF.FilterBySize(All, ESize::Large);
		for (auto product : FilteredByLarge) {
			cout << *product << endl;
		}
		cout << endl;
	}
}

namespace Open_Closed_Case
{
	template<class T>
	struct Specification
	{
		virtual ~Specification() {}
		virtual bool IsSatisfied(const T* Item) const = 0;
	};

	template<class T>
	struct AndSpecification: Specification<T>
	{
		const Specification<T>& First;
		const Specification<T>& Second;
		AndSpecification(const Specification<T>& InFirst, const Specification<T>& InSecond) : First(InFirst), Second(InSecond) {}

		virtual bool IsSatisfied(const T* Item) const override
		{
			return First.IsSatisfied(Item) && Second.IsSatisfied(Item);
		}
	};

	template<class T>
	AndSpecification<T> operator&&(const Specification<T>& First, const Specification<T>& Second)
	{
		return {First,Second};
	};

	template<class T>
	struct Filter
	{
		using Items = vector<T*>;
		virtual Items Apply(const Items& InItems, const Specification<T>& Spec) = 0;
	};

	struct ProductFilter: Filter<Product>
	{
		struct ColorSpecification: Specification<Product>
		{
			EColor Color;
			ColorSpecification(EColor InColor) : Color(InColor) {}

			virtual bool IsSatisfied(const Product* InProduct) const override
			{
				return InProduct->Color == Color;
			}
		};
		struct SizeSpecification: Specification<Product>
		{
			ESize Size;
			SizeSpecification(ESize InSize) : Size(InSize) {}

			virtual bool IsSatisfied(const Product* InProduct) const override
			{
				return InProduct->Size == Size;
			}
		};

		static ColorSpecification ByColor(EColor Color) { return {Color}; }
		static SizeSpecification  BySize(ESize Size)    { return {Size}; }

		virtual Items Apply(const Items& InProducts, const Specification<Product>& Spec) override
		{
			Items Result;
			for (Product* InProduct : InProducts)
				if (Spec.IsSatisfied(InProduct))
					Result.push_back(InProduct);
			return Result;
		}
	};

	void Test()
	{
		Product Apple{"Apple", EColor::Green, ESize::Small};
		Product Tree{"Tree", EColor::Green, ESize::Large};
		Product House{"House", EColor::Blue, ESize::Large};

		const vector<Product*> All { &Apple, &Tree, &House };
		for (auto product : All) {
			cout << *product << endl;
		}
		cout << endl;

		ProductFilter PF;
		auto ColorSpec = PF.ByColor(EColor::Green);
		auto FilteredByGreen = PF.Apply(All, ColorSpec);
		for (auto product : FilteredByGreen) {
			cout << *product << endl;
		}
		cout << endl;
		
		auto SizeSpec = PF.BySize(ESize::Large);
		auto FilteredByLarge = PF.Apply(All, SizeSpec);
		for (auto product : FilteredByLarge) {
			cout << *product << endl;
		}
		cout << endl;

		auto FilteredByGreenAndLarge = PF.Apply(All, ColorSpec && SizeSpec);
		for (auto product : FilteredByGreenAndLarge) {
			cout << *product << endl;
		}
		cout << endl;
		
		auto Green = ProductFilter::ColorSpecification(EColor::Green);
		auto Large = ProductFilter::SizeSpecification(ESize::Large);
		auto GreenAndLarge = Green && Large;
		auto FilteredByGreenAndLarge2 = PF.Apply(All, GreenAndLarge);
		for (auto product : FilteredByGreenAndLarge2) {
			cout << *product << endl;
		}
		cout << endl;
	}
}

#pragma once
// Interface segregation principle (ISP): states that no code should be forced 
// to depend on methods it does not use.
// ISP splits interfaces that are very large into smaller and
// more specific ones so that clients will only have to know 
// about the methods that are of interest to them.
// 사용하지 않는 메서드를 강요하면 안된다. 인터페이스들을 쪼개야 한다.

namespace Problematic_Case_ISP
{
	struct Document;

	struct IMachine
	{
		virtual void Print(Document& Doc) = 0;
		virtual void Scan(Document& Doc) = 0;
		virtual void Fax(Document& Doc) = 0;
	};

	struct MFP: IMachine
	{
		virtual void Print(Document& Doc) override;
		virtual void Scan(Document& Doc) override;
		virtual void Fax(Document& Doc) override;
	};
	// Problems
	// 1. Recompile
	// 2. Client does not need this
	// 3. Forcing implementors to implement too much
}

namespace Case_ISP_Applied
{
	struct Document;

	struct IPrinter
	{
		virtual void Print(Document& Doc) = 0;
	};

	struct IScanner
	{
		virtual void Scan(Document& Doc) = 0;
	};

	struct IFaxer
	{
		virtual void Fax(Document& Doc) = 0;
	};

	struct IMachine : IPrinter, IScanner
	{
		virtual void Print(Document& Doc) = 0;
		virtual void Scan(Document& Doc) = 0;
	};

	struct Printer: IPrinter
	{
		virtual void Print(Document& Doc) override;
	};

	struct Scanner: IScanner
	{
		virtual void Scan(Document& Doc) override;
	};

	struct Machine: IMachine
	{
		IPrinter& Printer;
		IScanner& Scanner;

		Machine(IPrinter& InPrinter, IScanner& InScaner) : Printer(InPrinter), Scanner(InScaner) {}

		virtual void Print(Document& Doc) override
		{
			Printer.Print(Doc);
		}

		virtual void Scan(Document& Doc) override
		{
			Scanner.Scan(Doc);
		}
	};
}
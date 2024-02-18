#pragma once

#include <cassert>

// Hoare triple: A triple describes how the execution of a piece of
// code changes the state of the computation.
// A Hoare triple is of the form: {P}C{Q}
// where P and Q are assertions and C is a command.
// P is named the precondition and
// Q is the postcondition:
// when the precondtion is met, executing the command C establishes
// the postcondition.
// C의 실행 이전에 P가 성립한다면, C의 종료 이후에 Q가 성립하거나 또는
// C의 종료가 존재하지 않는다.
// 참고로 프로그램에서 실행이 성공적으로 종료되었는지의 여부는 따로 판단해야한다.
// 무한루프를 하거나 구현 정의 제한(숫자범위, 저장크기, 또는 운영체제 시간위반)을
// 위반할 수 있다.

// Precondition: a condition that must always be true just prior to 
// the execution of some section of code or 
// before an operation in a formal specification.
// 만약 선행조건이 틀리면, 해당 코드조각의 행동은 미정이다.
// 선행조건을 놓치거나 불충분하거나 공식적으로 증명되지 않거나, 정적 혹은 동적으로
// 검사하지않았다면, 문제를 일으킬 수 있다.

// Postcondition: ac condition that must always be true just after
// the execution of some section of code or after
// an operation in a formal specification.
// 예를 들어, 팩토리얼은 항상 1 이상의 정수형을 리턴한다.
// 제곱근을 계산하는 프로그램이 있는데, 리턴한 값을 제곱하면
// 입력값과 반드시 동일해야 한다는 후행조건을 만족해야 한다.

// Invariant: a logical assertion that is always held to be true during
// a certain phase of execution of a computer program.
// 예를 들어, 루프 불변성(loop invariant)는 루프의 모든 반복에서 시작과 끝에
// 반드시 만족해야하는 조건이다.
// Programmers often use assertions in their code to make invariants explicit.
// 몇몇 객체지향언어는 class invariants 문법을 갖는다.

namespace Examples::Eiffel
{
	class Time
	{
		int hour;
	public:
		void set_hour(int a_hour)
		{
			//1.required valid_argument(or precondition): 
			assert(0 <= a_hour && a_hour <= 23);

			//2.executions:
			hour = a_hour;

			//3.ensure(or postcondition):
			assert(hour == a_hour);
		}
	};
}

namespace Examples::MU_Puzzle
{
	void MUPuzzle()
	{
		volatile int RandomRule;
		int ICount = 1;
		int UCount = 0;
		while (ICount % 3 != 0) // Non-terminating loop
		{
			switch (RandomRule) {
			case 1:				UCount += 1; break;
			case 2: ICount *=2; UCount *= 2; break;
			case 3: ICount -=3; UCount += 1; break;
			case 4:				UCount -= 2; break;
			}
			// Computed invariant:
			assert(ICount % 3 == 1 || ICount % 3 == 2);
		}
	}
}
#pragma once

#include <iostream>
#include <cassert>
#include <vector>
#include <initializer_list>

namespace Monomorphic
{
    class ArrayOfInt
    {
        int Data[5] ={1,2,3,4,5};
    public:
        int Size() const { return 5; }

        int& operator[](int i) 
        { 
            return const_cast<int&>((*static_cast<const ArrayOfInt*>(this))[i]);
        }

        const int& operator[](int i) const
        { 
            assert(0 <= i && i < 5);
            return Data[i]; 
        }
    };

    int Sum(const ArrayOfInt& Array)
    {
        int Result = 0;
        for (int i = 0; i < Array.Size(); i++)
        {
            Result += Array[i];
        }
        return Result;
    }

    void Multiply(ArrayOfInt& ArrayRef, int Factor)
    {
        for (int i = 0; i < ArrayRef.Size(); i++)
        {
            ArrayRef[i] *= Factor;
        }
    }

    void Test()
    {
        ArrayOfInt Array;

        auto PrintArray = [&] (const char* cstr) {
            std::cout << cstr;
            for (int i = 0; i < Array.Size(); i++) {
                std::cout << Array[i] << " ";
            }
            std::cout << std::endl;
        };

        PrintArray("Array: ");
        std::cout << "Sum: " << Sum(Array) << std::endl;
        Multiply(Array, 2);
        PrintArray("Array Multiplied: ");
    }
}

namespace Polymorphic
{
    class ContainerOfInt
    {
    public:
        virtual int Size() const = 0;
        int& operator[](int i)
        {
            return const_cast<int&>((*static_cast<const ContainerOfInt*>(this))[i]);
        }
        virtual const int& operator[](int) const = 0;
        virtual void Add(int) = 0;
    };

    class ArrayOfInt: public ContainerOfInt
    {
        int* Datas = nullptr;
        int Capacity = 10;
        int Length = 0;
    public:
        ArrayOfInt() { Datas = new int[Capacity]; }
        ~ArrayOfInt()
        {
            delete[] Datas;
            Datas = nullptr;
        }

        //~ Begin ContainerOfInt Interface.
        virtual int Size() const override { return Length; }
        using ContainerOfInt::operator[];
        virtual const int& operator[](int i) const override { return Datas[i]; }
        virtual void Add(int InData) override
        {
            if (Capacity <= Length)
            {
                const int NewCapacity = Capacity * 2;
                if (int* NewDatas = new(std::nothrow) int[NewCapacity])
                {
                    for (int i = 0; i < Length; i++)
                    {
                        NewDatas[i] = Datas[i];
                    }
                    delete[] Datas;
                    Datas = NewDatas;
                    Capacity = NewCapacity;
                }
            }
            Datas[Length++] = InData;
        }
        //~ End ContainerOfInt Interface.
    };

    class ListOfInt: public ContainerOfInt
    {
        struct Node
        {
            int Data;
            Node* Next;
        };
        Node* Root = nullptr;
        Node* Last = nullptr;
        int NumNode = 0;

    public:
        ~ListOfInt()
        {
            Node* Temp;
            while (Root)
            {
                Temp = Root;
                Root = Root->Next;
                delete Temp;
            }
            Temp = nullptr;
        }

        //~ Begin ContainerOfInt Interface.
        virtual int Size() const override { return NumNode; }
        using ContainerOfInt::operator[];
        virtual const int& operator[](int i) const override
        {
            assert(0 <= i && i < NumNode);
            Node* Node = Root;
            for (int Count = 0; Count < i; Count++)
            {
                Node = Node->Next;
            }
            return Node->Data;
        }
        virtual void Add(int InData) override
        {
            if (Node* NewNode = new(std::nothrow) Node{InData, nullptr})
            {
                if (Last) Last = (Last->Next = NewNode);
                else Root = Last = NewNode;
                NumNode++;
            }
        }
        //~ End ContainerOfInt Interface.
    };

    int Sum(const ContainerOfInt& Container)
    {
        // ArrayOfInt: T(n) = Theta(n)
        // ListOfInt:  T(n) = Theta(n^2)
        int Result = 0;
        for (int i = 0; i < Container.Size(); i++)
        {
            Result += Container[i];
        }
        return Result;
    }

    void Multiply(ContainerOfInt& ContainerRef, int Factor)
    {
        // ArrayOfInt: T(n) = Theta(n)
        // ListOfInt:  T(n) = Theta(n^2)
        for (int i = 0; i < ContainerRef.Size(); i++)
        {
            ContainerRef[i] *= Factor;
        }
    }

    void Test()
    {
        auto Print = [] (const char* cstr, ContainerOfInt& ContainerRef){
            std::cout << cstr;
            for (int i = 0; i < ContainerRef.Size(); i++) {
                std::cout << ContainerRef[i] << " ";
            }
            std::cout << std::endl;
        };

        ArrayOfInt Array;
        ListOfInt List;
        for (int i = 1; i <= 5; i++) {
            Array.Add(i);
            List.Add(5 - i + 1);
        }

        Print("Array: ", Array);
        Print("List: ", List);

        std::cout << "Sum of Array: " << Sum(Array) << std::endl;
        std::cout << "Sum of List: " << Sum(List) << std::endl;

        Multiply(Array, 2);
        Multiply(List, -1);

        Print("Array: ", Array);
        Print("List: ", List);

        for (int i = 0; i < 5; i++) {
            Array[i] = List[i] = 0;
        }
        Print("Array: ", Array);
        Print("List: ", List);
    }
}

namespace Generic
{
    template <class InContainerType, class InFactorType>
    void Multiply(InContainerType& Container,InFactorType&& Factor)
    {
        for (int i = 0; i < Container.Size(); i++)
        {
            Container[i] *= Factor;
        }
    }

    class DamageArray: public std::vector<float>
    {
    public:
        DamageArray(std::initializer_list<float> Init) : std::vector<float>(Init) {}
        int Size() const { return static_cast<int>(size()); }
    };

    void Test()
    {
        using Polymorphic::ArrayOfInt;
        using Polymorphic::ListOfInt;

        auto Print = [] (const char* cstr, auto&& Container){
            std::cout << cstr;
            for (int i = 0; i < Container.Size(); i++) {
                std::cout << Container[i] << " ";
            }
            std::cout << std::endl;
        };

        ArrayOfInt Array;
        ListOfInt List;
        for (int i = 1; i <= 5; i++) {
            Array.Add(i);
            List.Add(5 - i + 1);
        }
        DamageArray DamagesToApply { 1.f, 2.f, 3.f, 4.f, 5.f };

        Multiply(Array, 2);
        Multiply(List, -1);
        Multiply(DamagesToApply, 10.f);

        Print("Array: ", Array);
        Print("List: ", List);
        Print("std::vector: ", DamagesToApply);
    }
}
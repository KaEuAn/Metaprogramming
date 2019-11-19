#include <stdio.h>
#include <iostream>

#include "common_interface.h"

struct A{};
struct B: public A{
    B() {std::cout << "B\n";}
};
struct C: public B{
    C() {std::cout << "C\n";}
};
struct C1: public B{
    C1() {std::cout << "C1\n";}
};
struct D: public C{
    D() {std::cout << "D\n";}
};

struct BB: public A{
    BB() {std::cout << "BB\n";}
};
struct CC: public BB{
    CC() {std::cout << "CC\n";}
};
struct CB: public BB{
    CB() {std::cout << "CB\n";}
};

struct BBB: public A{
    BBB() {std::cout << "BBB\n";}
};
struct CCC: public BBB{
    CCC() {std::cout << "CCC\n";}
};
struct CCB: public BBB{
    CCB() {std::cout << "CCB\n";}
};
struct CCCB: public CCB{
    CCCB() {std::cout << "CCCB\n";}
};


template <class T>
struct Unit1{
    T value_;
};
template <class T, class Base>
struct Unit2: public Base{
    T value_;
};

int main() {

    ////typelist Tests
    typedef TypeList<float, double, long double> floating_point_types;
    typedef TypeList<int> TL1;
    std::cout << "start unit tests\n";
    std::cout << std::boolalpha << IsEmpty<TL1>::value << " " << IsEmpty<EmptyTypeList>() << std::endl;
    typedef TypeList<double, float, float, double, int, char, char, int, char> TL;
    std::cout << std::boolalpha << Contains<char, TL>::value << " " << Contains<float, TypeList<double>>() << std::endl;
    std::cout << Length<TL>::value << " " << Length<EmptyTypeList>() << std::endl;

    std::cout << Contains<double, RemoveAll<double, TL>::type>() << ' ' << Contains<int, RemoveAll<double, TL>::type>() << std::endl;
    std::cout << AddFront<char, TL1>::type() << std::endl;
    std::cout << TL() << std::endl;
    std::cout << Replace<TL, float, int>::Result() << std::endl;
    typedef TypeList<A, A> TL5;
    std::cout << SuperSubClass<A, B>::value << ' ' << SuperSubClass<char*, int>::value << std::endl;
    std::cout << typeid(MostDerived<TypeList<B, C, A ,D>, B>::Result).name() << std::endl;
    std::cout << DerivedToFront<TypeList<B, C, A ,D>>::Result() << std::endl;
    std::cout << SortTypeList<TypeList<B, C, A ,D>>::Result() << std::endl;
    std::cout << Reverse<TypeList<int, char, double, float>>::Result() << std::endl;
    std::cout << TypeList<GetSuper<CCC, TypeList<CCC, BBB, CCB, BB, B, A>>::Result>() << std::endl;
    std::cout << GetLinear<TypeList<CCC, CCB, BBB, BB, B, A>, CCB>::Result() << std::endl;
    std::cout << "end unit tests\n";
    
    using OC = GenScatterHierarchy<TypeList<int, double, char>, Unit1>;
    using LH = GenLinearHierarchy<TypeList<int, char, bool>, Unit2>;
    OC oc;
    (static_cast<Unit1<int>&>(oc)).value_ = 78;
    std::cout << (static_cast<Unit1<int>&>(oc)).value_ << std::endl;

    std::cout << FabricGeneratorHelper<TypeList<double, float, int>, TypeList<char, char, float>, TypeList<double>>::Types() << std::endl;
    using MyFactoryHierarchy = FabricGenerator<3, 5, TypeList<B, BB>>;
    auto* CurrentFactory = new MyFactoryHierarchy::GetConcreteFactory<BB>::result;
    BB* x = CurrentFactory->Create<BB>();
    std::cout << typeid(*x).name() << std::endl;
    delete x;

    
    using MyFactoryHierarchy1 = FabricGenerator<3, 5, TypeList<B, BB, BBB>, TypeList<C, C1>, TypeList<CC, CB>, TypeList<CCC, CCB>>;
    auto* CurrentFactory1 = new MyFactoryHierarchy1::GetConcreteFactory<CB>::result;
    BB* xx = CurrentFactory1->Create<BB>();
    std::cout << typeid(*xx).name() << std::endl;
    delete xx;

    using MyFactoryHierarchy2 = FabricGenerator<3, 5, TypeList<B, BB, BBB>, TypeList<C, C1>, TypeList<CC, CB>, TypeList<CCC, CCB>, TypeList<CCCB>>;
    auto* CurrentFactory2 = new MyFactoryHierarchy1::GetConcreteFactory<CCCB>::result;
    BBB* xxx = CurrentFactory2->Create<BBB>();
    std::cout << typeid(*xxx).name() << std::endl;
    delete xxx;
    return 0;
}
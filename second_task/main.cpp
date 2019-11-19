#include <stdio.h>
#include <iostream>

#include "common_interface.h"

class A{};
class B: public A{};
class C: public B{};
class C1: public B{};
class D: public C{};

class BB: public A{};
class CC: public BB{};
class CB: public BB{};

class BBB: public A{};
class CCC: public BBB{};
class CCB: public BBB{};


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

    
    using OC = GenScatterHierarchy<TypeList<int, double, char>, Unit1>;
    using LH = GenLinearHierarchy<TypeList<int, char, bool>, Unit2>;
    OC oc;
    (static_cast<Unit1<int>&>(oc)).value_ = 78;
    std::cout << (static_cast<Unit1<int>&>(oc)).value_ << std::endl;

    std::cout << FabricGeneratorHelper<TypeList<double, float, int>, TypeList<char, char, float>, TypeList<double>>::Types();
    using MyFactoryHierarchy = FabricGenerator<3, 5, TypeList<B, BB>>;
    //using MyFactoryHierarchy = FabricGenerator<3, 5, TypeList<B, BB, BBB>, TypeList<C, C1>, TypeList<CC, CB>, TypeList<CCC, CCB>>;
    auto* CurrentFactory = new MyFactoryHierarchy::GetConcreteFactory<BB>::result;
    BBB* x = CurrentFactory->Create<BBB>();

    return 0;
}
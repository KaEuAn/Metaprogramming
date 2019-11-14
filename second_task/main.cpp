#include <stdio.h>
#include <iostream>

#include "something.h"

class A{};
class B: public A{};
class C: public B{};
class D: public C{};

template <class T>
struct Unit {
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

    
    using OC = GenScatterHierarchy<TypeList<int, double, char>, Unit>;
    OC oc;
    (static_cast<Unit<int>&>(oc)).value_ = 78;
    std::cout << (static_cast<Unit<int>&>(oc)).value_;
    return 0;
}
#include "TypeList.h"


//check if T can be casted to U

template<typename T, typename U> 
struct Conversion{
    using small = char;
    class big{char a[2];};

    static small Test(U);
    static big Test(...);
    static T MakeT();
    const static bool exists = (sizeof(Test(MakeT())) == sizeof(small));
};

//check if T is super to U
template<typename T, typename U>
struct SuperSubClass{
    const static bool value = (Conversion<const U*, const T*>::exists && !std::is_same<const T*, const void*>::value);
};


template <typename TL, typename T> struct MostDerived{};

template <typename T>
struct MostDerived<EmptyTypeList, T> {
    using Result = T;
};

template<typename Head, typename ...Args, typename T>
struct MostDerived<TypeList<Head, Args...>, T> {
    using Candidate = typename MostDerived<TypeList<Args...>, T>::Result;
    using Result = typename std::conditional<
        SuperSubClass<Candidate, Head>::value, 
        Head,
        Candidate>::type;
};

template<typename T> struct DerivedToFront;

template<>
struct DerivedToFront<EmptyTypeList> {
    using Result = EmptyTypeList;
};

template<typename Head, typename ...Args>
struct DerivedToFront<TypeList<Head, Args...>> {
    using Tail = TypeList<Args...>;
    using TheMostDerived = typename MostDerived<Tail, Head>::Result;
    using Left = typename Replace<Tail, TheMostDerived, Head>::Result;
    using Result = typename AddFront<TheMostDerived, Left>::type;
};

/////////////

template <typename TL, template<class> class Unit>
class GenScatterHierarchy;

template<template<class> class Unit>
class GenScatterHierarchy<EmptyTypeList, Unit>{};

template <typename Head, typename ...Tail, template <class> class Unit>
class GenScatterHierarchy <TypeList<Head, Tail...>, Unit>
    : public Unit<Head>, public GenScatterHierarchy<TypeList<Tail...>, Unit>
{
public:
    using TList = TypeList<Head, Tail...>;
    using LeftBase = GenScatterHierarchy<TypeList<Head>, Unit>;
    using RightBase = GenScatterHierarchy<TypeList<Tail...>, Unit>;
};

template <class AtomicType, template<class> class Unit>
class GenScatterHierarchy<TypeList<AtomicType>, Unit>: public Unit<AtomicType> {
    using LeftBase = Unit<AtomicType>;
};

//////////

template<typename T>
struct Type2Type{
    using type = T;
};

template<int v>
struct Int2Type{
    enum {value = v};
};


template <typename T>
class AbstractFactoryUnit
{
public:
    virtual T* DoCreate(Type2Type<T>) = 0;
    virtual ~AbstractFactoryUnit() {}
};

template<typename TL, template<class> class Unit = AbstractFactoryUnit> 
struct AbstractFactory: public GenScatterHierarchy<TL, Unit> {
    using Types = TL;

    template<typename T>
    T* Create() {
        Unit<T>& unit = *this;
        return unit.DoCreate(Type2Type<T>());
    }
};
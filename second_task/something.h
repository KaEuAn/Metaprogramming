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

///////

template<typename TL>
struct SortTypeList{};

template<>
struct SortTypeList<EmptyTypeList>
{
    using Result = EmptyTypeList;
};

template<typename T>
struct SortTypeList<TypeList<T>>
{
    using Result = TypeList<T>;
};

template<typename Head, typename ...Args> struct SortTypeList<TypeList<Head, Args...>> {
    using Tail = TypeList<Args...>;
    using TheMostDerived = typename MostDerived<Tail, Head>::Result;
    using Left = typename Replace<Tail, TheMostDerived, Head>::Result;
    using Result = typename AddFront<TheMostDerived, typename SortTypeList<Left>::Result>::type;
};

/////////////

template<typename T, typename TL> struct GetSuper;

template<typename T, typename Head, typename ...Tail> 
struct GetSuper<T, TypeList<Head, Tail...>>{
    using Result = typename std::conditional<
        SuperSubClass<Head, T>::value,
        Head,
        typename GetSuper<T, TypeList<Tail...>>::Result
    >::type;
};

template<typename T>
struct GetSuper<T, EmptyTypeList>{
    using Result = internal::Void;
};

template<typename TL, typename T>
struct GetLinear;

template<typename Head, typename ...Tail, typename T>
struct GetLinear<TypeList<Head, Tail...>, T> {
    using SuperClass = typename GetSuper<T, TypeList<Head, Tail...>>::Result;
    using Result = typename std::conditional<
        std::is_same<SuperClass, internal::Void>::value,
        EmptyTypeList,
        typename std::conditional<
            std::is_same<Head, SuperClass>::value,
            typename AddFront<Head, typename GetLinear<TypeList<Tail...>, SuperClass>::Result>::type,
            typename GetLinear<TypeList<Tail...>, T>::Result
        >::type
    >::type;
};


template<typename T>
struct GetLinear<EmptyTypeList, T> {
    using Result = EmptyTypeList;
};

////////////

template <typename TL, template<class> class Unit>
class GenScatterHierarchy;

template<template<class> class Unit>
class GenScatterHierarchy<EmptyTypeList, Unit>{};

template <typename Head, typename ...Tail, template <class> class Unit>
class GenScatterHierarchy <TypeList<Head, Tail...>, Unit>
    : public Unit<Head>, public GenScatterHierarchy<TypeList<Tail...>, Unit>
{
};

template <class AtomicType, template<class> class Unit>
class GenScatterHierarchy<TypeList<AtomicType>, Unit>: public Unit<AtomicType> {
};

////////////////////////


template<typename TL, template<class AtomicType, class Base> class Unit, typename Root = internal::Void>
struct GenLinearHierarchy;

template<typename Head, typename ...Tail, template<class, class> class Unit, typename Root>
struct GenLinearHierarchy<TypeList<Head, Tail...>, Unit, Root> : public Unit<Head, GenLinearHierarchy<TypeList<Tail...>, Unit, Root>>{};

template<template<class, class> class Unit, typename Root> 
struct GenLinearHierarchy<EmptyTypeList, Unit, Root> : public Root{};



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
    virtual T* DoCreate(Type2Type<T>);
    virtual ~AbstractFactoryUnit() {}
};

template<typename TL, template<class> class Unit = AbstractFactoryUnit> 
struct AbstractFactory: public GenScatterHierarchy<TL, Unit> {
    using Types = TL;

    template<typename T>
    T* Create() {
        Unit<T>* unit = this;
        return unit->DoCreate(Type2Type<T>());
    }
};

template<typename Product, typename Base>
struct NewFactoryUnit: public Base {
private:
    using BaseTypes = typename Base::Types;
protected:
    using Types = typename BaseTypes::Tail;
public:
    using AbstractProduct = typename BaseTypes::Head;
    Product* DoCreate(Type2Type<Product>) {
        return new AbstractProduct;
    }   
};

template<typename AbFactory, template<class, class> class Creator = NewFactoryUnit, typename TL = typename AbFactory::Types>
struct ConcreteFactory : public GenLinearHierarchy<TL, Creator, AbFactory> {
    using Types = typename AbFactory::Types;
    using ConcreteTypes = TL;
};


//template<int classNum, int TLNum, typename ...>
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


template<typename TL> struct GetLast;

template<typaname Head, typename ...Tail>
struct GetLast<TypeList<Head, Tail...>>{
    using Result = template GetLast<TypeList<Tail...>>::Result;
};

template<typaname Head>
struct GetLast<TypeList<Head>>{
    using Result = Head;
};


template<typename TL1, typename TL2> struct GetBaseTL;

template<typename Head1, typename ...Tail1, typename Head2, typename ...Tail2> 
struct GetBaseTL<TypeList<Head1, Tail1...>, TypeList<Head2, Tail2...>>{
    using Result = typename std::conditional<Conversion<Head1, Head2>::exists, TypeList<Head2, Tail2...>, TypeList<Head1, Tail1...>>::type;
};

template<typename T, typename TL> struct GetBase;

template<typename T, typename Head> 
struct GetBase<T, TypeList<Head>>{
    using Result = typename std::conditional<Conversion<Head, T>::exists, internal::Void, Head>::type;
};

template<typename T, typename Head, typename ...Tail> 
struct GetBase<T, TypeList<Head, Tail...>>{
    using Prev = typename GetBase<T, TypeList<Tail...>>::Result;
    using Result = typename std::conditional<Conversion<Head, T>::exists, internal::Void, Head>::type;
};
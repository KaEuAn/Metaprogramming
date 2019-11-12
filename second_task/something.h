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
    using Result = internal::Void;
};

template<typename Head, typename ...Args>
struct DerivedToFront<TypeList<Head, Args...>> {
    using Tail = TypeList<Args...>;
    using TheMostDerived = typename MostDerived<Tail, Head>::Result;
    using Left = typename Replace<Tail, TheMostDerived, Head>::Result;
    using Result = typename AddFront<TheMostDerived, Left>::type;
};
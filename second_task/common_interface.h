#include "something.h"

template<typename ...Types> struct FabricGeneratorHelper;

template<typename Tl> struct RemoveBad;

template<typename Head, typename ...Tail>
struct RemoveBad<TypeList<Head, Tail...>> {
    using Result = typename std::conditional<
    std::is_constructible<Head>::value,
    TypeList<Head, Tail...>,
    typename RemoveBad<TypeList<Tail...>>::Result
    >::type;
};

template<>
struct RemoveBad<EmptyTypeList> {
    using Result = EmptyTypeList;
};

template<int x, int y, typename ...TLs>
struct FabricGenerator{
    using Types = typename SortTypeList<typename FabricGeneratorHelper<TLs...>::Types>::Result;
    template<typename T> struct GetConcreteFactory{
        //maybe extract only necessary from Types?
        using CurrentTypes = typename RemoveBad<typename GetLinear<Types, T>::Result>::Result;
        using AbFabric = AbstractFactory<CurrentTypes>;
        using result = ConcreteFactory<typename CurrentTypes::Head, AbFabric>;
    };

};


template<typename Head, typename ...Tail>
struct FabricGeneratorHelper<Head, Tail...>{
    using Types = typename Append<typename FabricGeneratorHelper<Tail...>::Types, Head>::type;
};

template<typename Head>
struct FabricGeneratorHelper<Head>{
    using Types = Head;
};

template<>
struct FabricGeneratorHelper<EmptyTypeList>{
    using Types = EmptyTypeList;
};
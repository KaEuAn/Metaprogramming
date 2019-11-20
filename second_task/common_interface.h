#include "something.h"

template<typename ...Types> struct FabricGeneratorHelper;

template<int x, int y, typename ...TLs>
struct FabricGenerator{
    using Types = typename SortTypeList<typename FabricGeneratorHelper<TLs...>::Types>::Result;
    template<typename T> struct GetConcreteFactory{
        //maybe extract only necessary from Types?
        using CurrentTypes = typename GetLinear<Types, T>::Result;
        using AbFabric = AbstractFactory<CurrentTypes>;
        using result = ConcreteFactory<T, AbFabric>;
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
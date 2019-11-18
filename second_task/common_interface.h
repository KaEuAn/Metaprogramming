#include "something.h"

template<typename ...Types> struct FabricGeneratorHelper;

template<int x, int y, typename ...TLs>
struct FabricGenerator{
    using Types = typename SortTypeList<typename FabricGeneratorHelper<TLs...>::Types>::Result;
    using AbFabric = AbstractFactory<Types>;
    template<typename T> class GetConcreteFactory{
        //maybe extract only necessary from Types?
        using result = ConcreteFactory<AbFabric>;
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
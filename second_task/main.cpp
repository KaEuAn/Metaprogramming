
namespace internal {
struct Void {};
} // internal

template<typename ...Args>
struct TypeList
{
    using Head = internal::Void;
    using Tail =  internal::Void;
};

typedef TypeList<> EmptyTypeList;

template<typename H, typename ...T>
struct TypeList<H, T...>
{
    using Head = H;
    using Tail = TypeList<T...>;
};

template <typename H, typename... T>
struct Typelist<H, T...> {
    using Head = H;
    using Tail = Typelist<T...>;

}

template <typename H, typename... T>
struct Typelist<H> {
    ...
}

// without variadic
template <typename H, typename T>
struct Typelist {
    ...
}

Typelist<H, Typelist<N, Typelist<M, P> > > t;

// Loki

template <typename H, typename... T>
struct Typelist {
    using Head = H;
    using Tail = Typelist<T...>;

    ...
}

struct Emptylist {
    ...
}

///////////////////////////////////////////////////////////

TypeAt<Typelist<...>, 2>::Result t;

template <typename Typelist<typename H, typename... T>, size_t i>
class TypeAt<Typelist<H, T...>, i> {
    using Result = TypeAt<Typelist<T...>, i - 1>;
}

template <typename Typelist<typename H, typename... T>, size_t i>
class TypeAt<Typelist<H, T...>, 0> {
    using Result = Typelist<H, T...>::Head;
}

template <typename Typelist<typename H, typename... T>, size_t i>
class TypeAt<Emptylist, i> {
    using Result = Emptylist;
}

///////////////////////////////////////////////////////////

SetAt<2, float, Typelist<int, int, int, int>>::Result t;

// имхо херня
template<size_t pos, typename Type, typename Typelist<typename H, typename... T>>
struct SetAt<pos, Type, Typelist<H, T...>> {
    using Result = Typelist<H, SetAt<pos - 1, Type, Typelist<T...>>::Result>;
}

template<size_t pos, typename Type, typename Typelist<typename H, typename... T>>
struct SetAt<0, Type, Typelist<H, T...>> {
    using Result = Typelist<Type, H, T...>;
}

template<size_t pos, typename Type, typename Typelist<typename H, typename... T>>
struct SetAt<pos, Type, Emptylist> {
    using Result = Typelist<Type>;
}

///////////////////////////////////////////////////////////

EraseAll<float, Typelist<int, float, int, int>>::Result t;

template <typename EType, typename Typelist<typename H, typename... T>>
struct EraseAll<EType, Typelist<H, T...>> {
    using Result = Typelist<EType, H, EraseAll<EType, Typelist<T...>>>;
}

template <typename EType, typename Typelist<typename H, typename... T>>
struct EraseAll<EType, Typelist<EType, T...>> {
    using Result = Typelist<EraseAll<EType, Typelist<T...>>>;
}



int main() {
    typedef TypeList<float, double, long double> floating_point_types;
    return 0;
}
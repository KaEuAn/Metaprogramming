#include <type_traits>
#include <typeinfo>



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

template<typename H, typename... T>
struct TypeList<H, T...>
{
    using Head = H;
    using Tail = TypeList<T...>;
};

////////////////

template<typename TL>
struct IsEmpty : std::true_type{};

template<>
struct IsEmpty<TypeList<internal::Void, internal::Void>> : std::true_type{};

template<typename ...Args>
struct IsEmpty<TypeList<Args...>> : std::integral_constant<bool,
        std::is_same<typename TypeList<Args...>::Head, internal::Void>::value &&
        IsEmpty<typename TypeList<Args...>::Tail>::value> {};

//////////////////////////////////

template<typename T, typename TL>
struct Contains : std::false_type {};

template<typename ...Args>
struct Contains<internal::Void, Args...> : std::false_type {};

template<typename T, typename ...Args>
struct Contains<T, TypeList<Args...>> : std::integral_constant<bool,
        std::is_same<typename TypeList<Args...>::Head, T>::value ||
        Contains<T, typename TypeList<Args...>::Tail>::value> {};

////////////////////////////

template<typename TL>
struct Length : std::integral_constant<unsigned int, 0> {};

template<typename ...Args>
struct Length<TypeList<Args...>> : std::integral_constant<unsigned int,
        IsEmpty<TypeList<Args...>>::value
            ? 0 : 1 + Length<typename TypeList<Args...>::Tail>::value> {};


///////////////////////////

template<unsigned int N, typename TL>
struct TypeAt
{
    using type = internal::Void;
};

template<typename ...Args>
struct TypeAt<0, TypeList<Args...>>
{
    using type = typename TypeList<Args...>::Head;
};

template<unsigned int N, typename ...Args>
struct TypeAt<N, TypeList<Args...>>
{
    //static_assert(N < Length<TypeList<Args...>>::value, "N is too big");
    
    using type = typename TypeAt<N - 1, typename TypeList<Args...>::Tail>::type;
};

//////////////

template<typename T, typename TL>
struct Append {};

template<typename T, typename ...Args>
struct Append<T, TypeList<Args...>>
{
    using type = TypeList<Args..., T>;
};

template<typename ...Args1, typename ...Args2>
struct Append<TypeList<Args1...>, TypeList<Args2...>>
{
    using type = TypeList<Args2..., Args1...>;
};

template<typename T, typename TL>
struct Add{};

template<typename T, typename ...Args>
struct Add<T, TypeList<Args...>>
{
	using type = TypeList<Args..., T>;
};

template<typename T, typename TL>
struct AddFront{};

template<typename T>
struct AddFront<T, EmptyTypeList>
{
	using type = TypeList<T>;
};

template<typename T, typename ...Args>
struct AddFront<T, TypeList<Args...>>
{
	using type = TypeList<T, Args...>;
};

//////////////////////////////


template<typename T, typename TL>
struct RemoveAll {};

template<typename T, typename ...Args>
struct RemoveAll<T, TypeList<Args...>>
{
private:
    using Removed = typename RemoveAll<T, typename TypeList<Args...>::Tail>::type;
    using Head = typename TypeList<Args...>::Head;
    
public:
    using type = typename std::conditional<
        std::is_same<Head, T>::value,
        Removed,
        typename Append<Removed, TypeList<Head>>::type
        >::type;
};

template<typename T, typename Head>
struct RemoveAll<T, TypeList<Head>>
{
    using type = typename std::conditional<
        std::is_same<Head, T>::value,
        EmptyTypeList,
        TypeList<Head>>::type;
};

template<typename T>
struct RemoveAll<T, EmptyTypeList>
{
    using type = EmptyTypeList;
};

///////////////////

template<typename TL, typename T, typename U>
struct Replace{};

template<typename T, typename U>
struct Replace<EmptyTypeList, T, U> {
    using Result = EmptyTypeList;
};

template<typename T, typename U, typename ...Args>
struct Replace<TypeList<T, Args...>, T, U> {
    using Result = TypeList<U, Args...>;
};

template<typename Head, typename T, typename U, typename ...Args>
struct Replace<TypeList<Head, Args...>, T, U>{
    using prev = typename Replace<TypeList<Args...>, T, U>::Result;
    using Result = typename AddFront<Head, prev>::type;
};

//////////////////




// Пустой список
std::ostream& operator<<(std::ostream& ostr, EmptyTypeList) {
	ostr << "{}";
	return ostr;
}

template<typename TL>
void PrintTypeListHelper(TL, std::ostream& ostr){}

template<typename T>
void PrintTypeListHead(T, std::ostream& ostr)
{
	ostr << typeid(T).name();
}

template<typename ...Args>
void PrintTypeListHead(TypeList<Args...> tl, std::ostream& ostr)
{
	ostr << tl;
}

template<typename Head, typename ...Args>
void PrintTypeListHelper(TypeList<Head, Args...>, std::ostream& ostr)
{
	PrintTypeListHead(Head(), ostr);
	if(!IsEmpty<TypeList<Args...>>::value)
	{
		ostr << ' ';
		PrintTypeListHelper<Args...>(TypeList<Args...>(), ostr);
	}
}

template<typename ...Args>
std::ostream& operator<<(std::ostream& ostr, TypeList<Args...> tl)
{
	ostr << '{';
	PrintTypeListHelper(tl, ostr);
	ostr << '}';
	return ostr;
}

////////////

template<typename TL> struct Reverse;

template<> struct Reverse<EmptyTypeList> {
    using Result = EmptyTypeList;
};

template<typename Head, typename ...Tail>
struct Reverse<TypeList<Head, Tail...>> {
    using Result = typename Append<Head, typename Reverse<TypeList<Tail...>>::Result>::type;
};

template<typename Head>
struct Reverse<TypeList<Head>> {
    using Result = TypeList<Head>;
};


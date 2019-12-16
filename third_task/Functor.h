#include <memory>
#include <iostream>

template<typename T, typename... Args>
struct VirtualFunctor {
    virtual ~VirtualFunctor() = default;
    virtual T operator()(Args...) = 0;
};

template<typename T, typename Method, typename... Args> 
class ConcreteFunctor: public VirtualFunctor<T, Args...> {
    Method* method;
public:
    explicit ConcreteFunctor(const Method& method): method(&method) {}
    T operator()(Args... args) override {
        return (*method)(args...);
    }
};


template<typename T>
class Functor;

template<typename T, typename... Args>
class Functor<T(Args...)>: public VirtualFunctor<T, Args...> {
    std::shared_ptr<VirtualFunctor<T, Args...>> functor;
public:
    template<typename FType>
    Functor(const FType& func): functor(std::make_shared<ConcreteFunctor<T, FType, Args...>>(func)) {}
    Functor(const Functor& a): functor(a.functor) {}

    T operator()(Args... args) override {
        return functor->operator()(args...);
    }
};
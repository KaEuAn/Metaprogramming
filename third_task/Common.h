#include "TypeList.h"
#include "Functor.h"
#include "Heroes.h"

//traits
void PrintAll() {}

template<typename Head>
void PrintAll(const Head& head) {
    std::cout << head << '\n';
}

template<typename Head, typename... Args>
void PrintAll(const Head& head, const Args&... args) {
    std::cout << head << ' ';
    PrintAll(args...);
}

template<bool A, typename B, typename C> struct CD {
    using type = B;
};

template<typename B, typename C> struct CD<false, B, C> {
    using type = C;
};




template <typename T, typename Visitor, typename Acceptor, typename... Args>
class VFunctor : VirtualFunctor<T, Visitor&, Acceptor&, Args...> {
public:
   
    T operator()(Visitor& visitor, Acceptor& acceptor, Args... args) override {
        std::cout << "Bad args for vistor " << typeid(visitor).name() << " and acceptor " << typeid(acceptor).name() << " with arguments ";
        PrintAll(std::cout, args...);
    }
};

template<>
struct VFunctor<std::string, Buff, Genji, bool>: public  VirtualFunctor<std::string, Buff&, Genji&, bool> {
    std::string operator()(Buff& buff, Genji& genji, bool print) override {
        std::cout << "Genji Buffed ";
        if (print) {
            std::cout << genji.GetStats() << '\n';
        }
        return genji.GetStats();
    }
};
template<>
struct VFunctor<std::string, Buff, Brigitte, bool>: public  VirtualFunctor<std::string, Buff&, Brigitte&, bool> {
    std::string operator()(Buff& buff, Brigitte& brig, bool print) override {
        std::cout << "Brigitte Buffed ";
        if (print) {
            std::cout << brig.GetStats() << '\n';
        }
        return brig.GetStats();
    }
};
template<>
struct VFunctor<std::string, Nerf, Brigitte, bool>: public  VirtualFunctor<std::string, Nerf&, Brigitte&, bool> {
    std::string operator()(Nerf& nerf, Brigitte& brig, bool print) override {
        std::cout << "Brigitte Nerfed ";
        if (print) {
            std::cout << brig.GetStats() << '\n';
        }
        return brig.GetStats();
    }
};

template<>
struct VFunctor<std::string, Nerf, Genji, bool>: public  VirtualFunctor<std::string, Nerf&, Genji&, bool> {
    std::string operator()(Nerf& nerf, Genji& genji, bool print) override {
        std::cout << "Genji Nerfed ";
        if (print) {
            std::cout << genji.GetStats() << '\n';
        }
        return genji.GetStats();
    }
};





template <typename Visitor, typename VTL, typename Acceptor, typename AcceptorsTL, typename Result, typename... Args>
struct VisitFactory;


template <typename Visitor, typename VHead, typename... VTail, typename Acceptor, typename AHead, typename... ATail, typename Result, typename... Args>
struct VisitFactory<Visitor, TypeList<VHead, VTail...>, Acceptor, TypeList<AHead, ATail...>, Result, Args...> {
    static Result make(Visitor& visitor, Acceptor& acceptor, Args... args) {
        auto VCheck = dynamic_cast<VHead*>(&visitor);
        auto ACheck = dynamic_cast<AHead*>(&acceptor);
        if(ACheck && VCheck) {
            acceptor.Accept(visitor);
            VFunctor<Result, VHead, AHead, Args...> cur_functor;
            Result ans = cur_functor(static_cast<VHead&>(visitor), static_cast<AHead&>(acceptor), args...);
            return ans;
        } else if (! ACheck) {
            return VisitFactory<Visitor, TypeList<VHead, VTail...>, Acceptor, TypeList<ATail...>, Result, Args...>::make(visitor, acceptor, args...);
        } else {
            return VisitFactory<Visitor, TypeList<VTail...>, Acceptor, TypeList<AHead, ATail...>, Result, Args...>::make(visitor, acceptor, args...);
        }
    }
};

template <typename Visitor, typename VHead, typename Acceptor, typename AHead, typename... ATail, typename Result, typename... Args>
struct VisitFactory<Visitor, TypeList<VHead>, Acceptor, TypeList<AHead, ATail...>, Result, Args...> {
    static Result make(Visitor& visitor, Acceptor& acceptor, Args... args) {
        auto VCheck = dynamic_cast<VHead*>(&visitor);
        auto ACheck = dynamic_cast<AHead*>(&acceptor);
        if(ACheck && VCheck) {
            acceptor.Accept(visitor);
            VFunctor<Result, VHead, AHead, Args...> cur_functor;
            Result ans = cur_functor(static_cast<VHead&>(visitor), static_cast<AHead&>(acceptor), args...);
            return ans;
        } else if (! ACheck) {
            return VisitFactory<Visitor, TypeList<VHead>, Acceptor, TypeList<ATail...>, Result, Args...>::make(visitor, acceptor, args...);
        } else {
            return Result();
        }
    }
};

template <typename Visitor, typename VHead, typename... VTail, typename Acceptor, typename AHead, typename Result, typename... Args>
struct VisitFactory<Visitor, TypeList<VHead, VTail...>, Acceptor, TypeList<AHead>, Result, Args...> {
    static Result make(Visitor& visitor, Acceptor& acceptor, Args... args) {
        auto VCheck = dynamic_cast<VHead*>(&visitor);
        auto ACheck = dynamic_cast<AHead*>(&acceptor);
        if(ACheck && VCheck) {
            acceptor.Accept(visitor);
            VFunctor<Result, VHead, AHead, Args...> cur_functor;
            Result ans = cur_functor(static_cast<VHead&>(visitor), static_cast<AHead&>(acceptor), args...);
            return ans;
        } else if (! ACheck) {
            return Result();
        } else {
            return VisitFactory<Visitor, TypeList<VTail...>, Acceptor, TypeList<AHead>, Result, Args...>::make(visitor, acceptor, args...);
        }
    }
};

template <typename Visitor, typename VHead, typename Acceptor, typename AHead, typename Result, typename... Args>
struct VisitFactory<Visitor, TypeList<VHead>, Acceptor, TypeList<AHead>, Result, Args...> {
    static Result make(Visitor& visitor, Acceptor& acceptor, Args... args) {
        auto VCheck = dynamic_cast<VHead*>(&visitor);
        auto ACheck = dynamic_cast<AHead*>(&acceptor);
        if(ACheck && VCheck) {
            acceptor.Accept(visitor);
            VFunctor<Result, VHead, AHead, Args...> cur_functor;
            Result ans = cur_functor(static_cast<VHead&>(visitor), static_cast<AHead&>(acceptor), args...);
            return ans;
        } else {
            return Result();
        }
    }
};
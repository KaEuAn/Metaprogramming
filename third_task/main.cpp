#include "Common.h"

void simple_print(int a) {
    std::cout << a << '\n';
}


int smth(int& x, char* b) {
    return ++x;
}

int main() {

    //functor tests
    Functor<void(int)> test1 = simple_print;
    test1(2);
    Functor<int(int&, char*)> test2 = smth;
    int hh = 2;
    int hhh = test2(hh, nullptr);
    std::cout << hh << ' ' << hhh<< '\n';

    //test classes
    Brigitte brig;
    std::cout << brig.GetStats() << '\n';
    Genji gen;
    std::cout << gen.GetStats() << '\n';

    //test PrintAll
    PrintAll(5, "dsafdas", 5.6);

    VisitFactory<Visitor, TypeList<Nerf, Buff>, Hero, TypeList<Brigitte, Genji>, std::string, bool> factory;
    Nerf nerf(0.1, false);
    Buff buff(0.1, false);
    std::cout << brig.ult_range;
    std::cout << dynamic_cast<Brigitte*>(&brig);
    //factory.make(nerf, brig, true);
    //factory.make(buff, brig, true);
    factory.make(nerf, gen, true);
    factory.make(buff, gen, false);

    return 0;
}
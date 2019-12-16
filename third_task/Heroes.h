#include <string>


class Hero;
class Genji;
class Brigitte;
class Visitor;
class Buff;
class Nerf;



struct Hero {
    double basic_attack;
    double melee_attack;
    int max_hp;
    int ult_duration;

    Hero(): basic_attack(40), melee_attack(30), max_hp(200), ult_duration(10) {}
    virtual ~Hero() = default;
    virtual std::string Accept(Visitor&) = 0;

    std::string GetStats() const {
        return "basic_attack: " + std::to_string(basic_attack) + ", melee_attack: " + std::to_string(melee_attack) + 
            ", max_hp: " + std::to_string(max_hp) + ", ult_duration: " + std::to_string(ult_duration);
    }

    friend class Visitor;
};

struct Genji: public Hero{
    double ult_attack;
    double dash_attack;

public:
    Genji(): ult_attack(120), dash_attack(75) {
        ult_duration = 8;
    }

    std::string Accept(Visitor&) override;

    std::string GetStats() const {
        return Hero::GetStats() + ", ult_attack: " + std::to_string(ult_attack) + ", dash_attack: " + std::to_string(dash_attack);
    }
};


struct Brigitte: public Hero {
    int shield;
    int ult_range;
  
public:
    Brigitte(): shield(300), ult_range(7) {
        basic_attack = melee_attack;
    }

    std::string Accept(Visitor&) override;
    
    std::string GetStats() const {
        return Hero::GetStats() + ", shield: " + std::to_string(shield) + ", ult_range: " + std::to_string(ult_range);
    }
};





class Visitor {
protected:
    double GetBasicAttackChange(Hero& hero, double impact) const {
        return hero.basic_attack * impact / 100;
    }
public:
    virtual std::string Visit(Genji&);
    virtual std::string Visit(Brigitte&);
};


class Buff: public Visitor {
    bool change_ult;
public:
    Buff(bool change): change_ult(change) {}

    std::string Visit(Genji& genji) override {
        double impact = 1;
        //genji.basic_attack += GetBasicAttackChange(dynamic_cast<Hero&>(genji), impact);
        if (change_ult) {
            genji.ult_attack += 20;
        }
        return genji.GetStats();
    }

    std::string Visit(Brigitte& brig) override {
        double impact = 1;
        //brig.basic_attack -= GetBasicAttackChange(dynamic_cast<Hero&>(brig), impact);
        brig.melee_attack = brig.basic_attack;
        if (change_ult) {
            brig.ult_range += 2;
        }
        return brig.GetStats();
    }
};



class Nerf: public Visitor {
    bool change_shields;

public:
    Nerf(double imp, bool change): change_shields(change) {}

    std::string Visit(Genji& genji) override {
        double impact = 1;
        //genji.basic_attack -= GetBasicAttackChange(dynamic_cast<Hero&>(genji), impact);
        return genji.GetStats();
    }

    std::string Visit(Brigitte& brig) override {
        double impact = 1;
        //basic_attack -= GetBasicAttackChange(dynamic_cast<Hero&>(brig), impact);
        brig.melee_attack = brig.basic_attack;
        if (change_shields) {
            brig.shield = 100;
        }
        return brig.GetStats();
    }
    
};



std::string Brigitte::Accept(Visitor& visitor) {
    return visitor.Visit(*this);
}

std::string Genji::Accept(Visitor& visitor) {
    return visitor.Visit(*this);
}
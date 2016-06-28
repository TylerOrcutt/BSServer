#ifndef __WEAPON_PISTOL_H_
#define __WEAPON_PISTOL_H_
#include "../Weapon.hpp"
#include <string>
class Pistol:public Weapon {
    float damage=1;
    std::string type="Pistol";
    virtual std::string getType(){
        return type;
    }
   
};


#endif
#ifndef __WEAPON_H_
#define __WEAPON_H_
#include <string>
class Weapon {
    float damage=0;
    std::string type="Default Weapon";
    virtual std::string getType(){
        return type;
    }
   
};


#endif
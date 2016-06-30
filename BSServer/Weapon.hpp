#ifndef __WEAPON_H_
#define __WEAPON_H_
#include <string>
#include "Helper.hpp"
class Weapon {
    private: 
    float damage=0;
    float fireRate=10.f; // 10 times ever mili
    std::string type="Default Weapon";
    long lastTimeFired=0;
    public:
    void Weapon(_damage=1.f,_fireRate=10.f,std::string _type="Default Weapon"){
        damage=_damage;
        fireRate=_fireRate;
        type=_type;
        
    }
    virtual std::string getType(){
        return type;
    }
    float getFireRate(){
        return fireRate;
    } 
    void setFireRate(float _rate){
        fireRate=_rate;
    }
    bool canFIre(){
        if(Helper::getTime()-lastTimeFired>=1000.f/fireRate){
            return true;
        }
        return false;
    }
    virtual bool fire(){
        if(Helper::getTime()-lastTimeFired>=1000.f/fireRate){
            lastTimeFired=::Helper::getTime();
            return true;
        }
        return false;
    
    }
   
};


#endif
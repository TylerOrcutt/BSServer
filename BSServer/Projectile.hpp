#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__
#include "Client.hpp"
class Projectile{
  private:
  unsigned long lastUpdate=0;
   float x=0,y=0;
   float lifespawn = 5.f;
   float timeAlive=10.f;
   float width=20,height=20;
   float speed=.5f;
   float angle=0;
   Client * owner;
  public:
    
    Projectile(){}
    Projectile(float _x, float _y,float _angle){
        x=_x;
        y=_y;
        angle =_angle;
    }
    
    
    Client * getOwner(){
        return owner;
    }
    void setOwner(Client *cli){
        owner=cli;
    }
    void move(float _x, float _y){
        x=_x;
        y=_y;
    }
    float getX(){
        return x;
    }
    float getY(){
        return y;
    }
    float getSpeed(){
        return speed;
    }
    void setLastUpdate(unsigned long time){
        lastUpdate=time;
    }
    unsigned long getLastUpdate(){
        return lastUpdate;
    }
    float getWidth(){
        return width;
    }
    float getHeight(){
        return height;
    }
    void resize(float _w , float _h){
        width=_w;
        height=_h;
    }
    float getTimeAlive(){
        return timeAlive;
    }
   void setTimeAlive(float time){
       timeAlive=time;
   }
    
    float getAngle(){
        return angle;
    }
    
    
};
#endif
#pragma once
#include <unistd.h>
#include "BSServer.hpp"
#include "Client.hpp"
#include "Helper.hpp"
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "Map.hpp"
#include "Dictionary.hpp"
#include "JSONParser.hpp"
#include <math.h>
#include <mutex>  
#include <thread>
#include "Projectile.hpp"
class Engine{
    private:
    BSServer *serv;
   Map map;
   std::mutex mtx;  
   std::list<Projectile> projectiles;
  public:
  Engine(BSServer *server){
      serv=server;
      if(!map.loadMap("map2")){
          std::cout<<"Map failed to load\n";
      }
      
  }
  
  void run(){
      while(serv->isRunning()){
      mtx.lock();
          std::vector<Client*>*clients= serv->getClients();
  mtx.unlock();
          for(int i=0;i<clients->size();i++){
           mtx.lock();
              Client* cli = (*clients)[i];
            
                        float x=cli->getX(),y=cli->getY(),angle=cli->getAngle();
                        
                       bool moving =cli->isMoving();
         mtx.unlock();
              if(Helper::getTime()-cli->getLastResponce() >30000){
                  clients->erase(clients->begin()+i);
                  std::cout<<"Kicked client "<<i<<" Hasnt responed for awhile\n";
                  continue;
              }
              
                  if(Helper::getTime()-cli->getLastPing() >10000){
                      cli->setLastPing(Helper::getTime());
                      serv->sendData("Ping",cli->cli_addr,cli->clilen);
                      std::cout<<"Sent ping\n";
                      
                  }
          
                  if(cli->commands_size()>0){
                      CommandMessage lst = cli->popCommand();
                      std::string cmd = lst.cmd;
                    //  std::cout<<"ts: "<<lst.timestamp<<"   "<<cmd<<std::endl;
                    Dictionary * data = JSONParser::parseJson(lst.cmd);
                 //  data->printDictionary();
                    DictionaryItem * player;
                    if((player=data->getItem("Player"))!=nullptr){
                     
                       if(player->getItem("x")!=nullptr){
                            x=atof(player->getItem("x")->value.c_str());      
                          //  cli->setX(x);                   
                       }
                       
                       if(player->getItem("y")!=nullptr){
                            y=atof(player->getItem("y")->value.c_str());                           
                        //  cli->setY(y);
                       }
                       
                          if(player->getItem("angle")!=nullptr){
                             angle=atof(player->getItem("angle")->value.c_str());                          
                         mtx.lock();
                       //  cli->setAngle(angle);
                          mtx.unlock();
                       }
            
                      if(player->getItem("moving")!=nullptr){
                    
                     //  moving=false;
                       mtx.lock();
                       //cli->move(x,y);
                   std::cout<<i<<"  STOPPED:  X:"<<x<<"  Y:"<<y<<"  Angle:"<<angle<<std::endl;
                      
                
                       // cli->setLastUpdate(Helper::getTimeNano());
                      cli->setMoving(false);
                //     cli->setLastUpdate(Helper::getTime());
         
                   mtx.unlock(); 
                       std::cout<<i<<" STOPED MOVING\n";
               std::cout<<"X:"<<x<<"  Y:"<<y<<"  Angle:"<<angle<<std::endl;
               
                //    continue;
                      }else{
                          if(!moving){
                       std::cout<<i<<" Started MOVING\n";
                       mtx.lock();
                       cli->move(x,y);
                         std::cout<<i<<"  STARTED:  X:"<<x<<"  Y:"<<y<<"  Angle:"<<cli->getAngle()<<std::endl;
                      
                
                        cli->setAngle(angle);
                          cli->setLastUpdate(Helper::getTime()-((cli->getLatency()/2)));
                        cli->setMoving(true);
                       mtx.unlock();
                   
                          }
                          moving=true;
                        //  cli->setMoving(true);
                          
                           
                      }
                      
                   
                   ///    cli->setAngle(angle);
                          
                    
                       }
                       
                    //check for projectile data   
             
                     if((player=data->getItem("Projectile"))!=nullptr){
                               if(player->getItem("angle")!=nullptr){
                               float projectileAngle=atof(player->getItem("angle")->value.c_str());    
                               mtx.lock();
                               Projectile prj(cli->getX()+32, cli->getY()+32,projectileAngle);
                               prj.setLastUpdate(Helper::getTime()- (cli->getLatency()/2));
                               prj.setOwner(cli);
                               mtx.unlock();
                               projectiles.push_back(prj);
                               std::cout<<"Added Projectile\n";
                                     
                       }
                     }
                    
                           std::stringstream id;
                     id<<"id:"<<cli->cli_addr.sin_addr.s_addr<<":"<<cmd;
                  //   std::cout<<id.str()<<std::endl;
                      serv->broadcastPlayerData(cli->cli_addr,id.str());
                     
                       
                       
                       delete data;
                       
             
                  }             
                  //    std::cout<<i<<"  Player X:"<<cli->getX()<<"  Y:"<<cli->getY()<<"  Angle:"<<angle<<std::endl;
                      
                 
                      if(moving ){
                      //    std::cout<<"PLAYER IS MOVING\n";
               
                 //    std::cout<<dt<<std::endl;
                   
                     mtx.lock();
                            unsigned long ct=  Helper::getTime();
                            if(!cli->isMoving() || angle!=cli->getAngle()){
                                ct-=(cli->getLatency()/2);
                             
                            }else{
                                
                            }
                       
                          long dt =  (ct-cli->getLastUpdate());
                  
               
                 //    std::cout<<dt<<"\n";
                     
                  //     angle = cli->getAngle();
                     float cx = cli->getX()+ (((float)cos(cli->getAngle()))*(dt*0.5f));
                     float cy = cli->getY()+(((float)sin(cli->getAngle()))*(dt*0.5f));
                    //   cli->setLastUpdate(ct);
                    cli->setAngle(angle);
                      cli->setLastUpdate(ct);
                    if(map.checkCollision(cx,cy,64,64)){
                        std::cout<<"COLLISION\n";
                       cli->setMoving(false);
                      
                      }else{
                      cli->move(cx,cy);
                      }
                
                   std::cout.precision(9);
             //  std::cout<<i<<"  Caculated X:"<<cx<<"  Y:"<<cy<<"  Angle:"<<angle<<std::endl;
               
                 
                      
                         mtx.unlock();
                
                  }
       
               
               
          }
          
          //update projectiles
      //    std::cout<<projectiles.size()<<"\n";
          for(auto projectile = projectiles.begin();projectile!=projectiles.end();projectile++){
              Projectile prj = *projectile;
                unsigned long ct=  Helper::getTime();
                    long dt =  (ct-prj.getLastUpdate());
            
                  float cx = prj.getX()+ (((float)cos(prj.getAngle()))*(dt*prj.getSpeed()));
                  float cy = prj.getY()+(((float)sin(prj.getAngle()))*(dt*prj.getSpeed()));
                  prj.setTimeAlive(prj.getTimeAlive()-(float)(dt/100.f));
                      prj.move(cx,cy);
                   prj.setLastUpdate(ct);
                  if(prj.getTimeAlive()<=0){
                       std::cout<<"Projectile REMOVED - TIMEOUT\n";
                        projectiles.erase(projectile);
                        projectile--;
                        continue;
                  }
              //check collision
             
                    if(map.checkCollision(cx,cy,20,20)){
                        std::cout<<"Projectile Map collision\n";
                        projectiles.erase(projectile);
                        projectile--;
                      //  std::cout<<"Removed PRojectile\n";
                    }
                    mtx.lock();
                          std::vector<Client*>*clients= serv->getClients();
                  for(int i=0;i<clients->size();i++){
                          Client * cli = (*clients)[i];
                        if(prj.getOwner() == cli){
                            continue;
                        }
                      
                       if(prj.getX()+20>=cli->getX() && prj.getX()<= cli->getX()+64 
                       && prj.getY()+20 >=cli->getY() && prj.getY()<=cli->getY()+64){
                      
                         //kill player
                         std::cout<<i<<" was killed\n";
                                 projectiles.erase(projectile);
                                   projectile--;
                          }
                    }
                    mtx.unlock();
              
          }
          
       
         usleep(100);
          
      }
  }
};
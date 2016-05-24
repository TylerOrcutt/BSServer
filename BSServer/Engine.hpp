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
class Engine{
    private:
    BSServer *serv;
   Map map;
   std::mutex mtx;  
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
         
                    
                             std::stringstream id;
                     id<<"id:"<<cli->cli_addr.sin_addr.s_addr<<":"<<cmd;
                  //   std::cout<<id.str()<<std::endl;
                      serv->broadcastPlayerData(cli->cli_addr,id.str());
                     // delete player;
                       
                       
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
                   
                      
                      }else{
                      cli->move(cx,cy);
                      }
                     mtx.unlock();
                   std::cout.precision(9);
               std::cout<<i<<"  Caculated X:"<<cx<<"  Y:"<<cy<<"  Angle:"<<angle<<std::endl;
                      
                
                 
                      
                    
                
                  }
       
               
               
          }
       
         usleep(100);
          
      }
  }
};
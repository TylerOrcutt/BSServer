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
class Engine{
    private:
    BSServer *serv;
   Map map;
   
  public:
  Engine(BSServer *server){
      serv=server;
      if(!map.loadMap("map2")){
          std::cout<<"Map failed to load\n";
      }
      
  }
  
  void run(){
      while(true){
          std::vector<Client*>*clients= serv->getClients();
          for(int i=0;i<clients->size();i++){
              Client* cli = (*clients)[i];
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
                       float x=cli->getX(),y=cli->getY(),angle=cli->getAngle();
                       bool moving =true;
                       if(player->getItem("x")!=nullptr){
                            x=atof(player->getItem("x")->value.c_str());                         
                       }
                       
                       if(player->getItem("y")!=nullptr){
                            y=atof(player->getItem("y")->value.c_str());                           
                       }
                       
                          if(player->getItem("angle")!=nullptr){
                            angle=atof(player->getItem("angle")->value.c_str());                          
                       }
                       
                   std::cout<<"X:"<<x<<"  Y:"<<y<<"  Angle:"<<angle<<std::endl;
                    
                      if(player->getItem("moving")!=nullptr){
                       moving=false;
                        cli->move(x,y);
                        
                      }
                       cli->setAngle(angle);
                      cli->setMoving(moving);
                       } 
                      if(cli->isMoving()){
                            
                     unsigned long dt = Helper::getTime()-lst.timestamp;
                  //   std::cout<<"DT:"<<dt<<std::endl;
                  
                   //  dt/=10000000;
                    // std::cout<<"DT:"<<dt<<std::endl;
                  //     cli->move(x,y);
                //       std::cout<<"X:"<<x<<"  Y:"<<y<<"  Angle:"<<angle<<std::endl;
                  if(dt>0){
                      float angle=cli->getAngle();
                     float cx = cli->getX()+ (float)cos(angle)*(dt*7);
                     float cy = cli->getX()+(float)sin(angle)*(dt*7);
                     std::cout<<"Caculated X:"<<cx<<"  Y:"<<cy<<"  Angle:"<<angle<<std::endl;
                      }
                    //  if(map.checkCollision(cx,cy,64,64)){
                      //    std::cout<<"COLLISION\n";
                      }
                      
                      //s}
                      
                    
                      std::stringstream id;
                     id<<"id:"<<cli->cli_addr.sin_addr.s_addr<<":"<<cmd;
                  //   std::cout<<id.str()<<std::endl;
                      serv->broadcastPlayerData(cli->cli_addr,id.str());
                  }
          }
          
         usleep(1000);
          
      }
  }
};
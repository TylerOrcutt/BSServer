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
                   //   std::cout<<cmd<<std::endl;
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
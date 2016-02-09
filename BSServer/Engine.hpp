#pragma once
#include <unistd.h>
#include "BSServer.hpp"
#include "Client.hpp"
#include "Helper.hpp"
#include <vector>
#include <iostream>
class Engine{
    private:
    BSServer *serv;
  public:
  Engine(BSServer *server){
      serv=server;
  }
  
  void run(){
      while(true){
          std::vector<Client*>*clients= serv->getClients();
          for(int i=0;i<clients->size();i++){
              Client* cli = (*clients)[i];
              if(Helper::getTime()-cli->getLastResponce() >30000){
                  clients->erase(clients->begin()+i);
                  std::cout<<"Kicked client "<<i<<" Hasnt responed for awhile\n";
              }
          }
          
         usleep(1000);
          
      }
  }
};
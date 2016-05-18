#ifndef _CONFIG__H_
#define _CONFIG__H_
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
struct configuration{
    int max_clients=10;
    std::vector<std::string> maplist;
    std::string ServerName;
    std::string ip;
    int port = 9898;
    std::string user;
    std::string passwd;
   
   
    
};

class Config{
    
    public:
    static configuration* LoadConfig(){
      //  std::cout<<"Loading config \n";
        std::fstream file;
        file.open("config",std::fstream::in);
        if(file.is_open()){
           file.close();
        }
        //defaults 
        std::cout<<"Failed to load config file, using defaults.\n";
        configuration * config = new configuration;
        config->max_clients = 300;
        config->ServerName= "Develop - internal";
        config->ip="10.0.0.3";
        config->port=9898;
        config->user="twittumz";
        config->passwd="asd";
         
        
        return config;
    }
    static std::string genParamString(configuration * config){
        std::stringstream ss;
        ss<<"MaxClients="<<config->max_clients;
        ss<<"&ServerName="<<config->ServerName;
        ss<<"&IP="<<config->ip;
        ss<<"&port="<<config->port;
        ss<<"&username="<<config->user;
        ss<<"&passwd="<<config->passwd;
        return ss.str();
    }
    
};



#endif
#ifndef _CONFIG__H_
#define _CONFIG__H_
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include<dirent.h>
struct configuration{
    int max_clients=10;
    std::vector<std::string> maplist;
    std::string ServerName;
    std::string ip;
    int port = 9898;
    long gameLength = 300000; //5minutes
    std::string user;
    std::string passwd;
    std::vector<std::string> maps;
   
   
    
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
        config->ServerName= "Development";
        config->ip="10.0.0.3";
        config->port=9898;
        config->user="twittumz";
        config->passwd="asd";
        config->maps = *Config::getMaps(); 
        config->gameLength=300000;
        for(int i=0;i<config->maps.size();i++){
            std::cout<<"'"<<config->maps[i]<<"'"<<std::endl;
        }
        return config;
    }
    
    static std::vector<std::string> * getMaps(){
        std::vector<std::string> *maps = new std::vector<std::string>();
        DIR * dir;
        struct dirent *ent;
        if((dir=opendir("maps/"))!= NULL){
            while((ent = readdir(dir))!=NULL){
                if(std::string(ent->d_name) == "." ||std::string(ent->d_name)==".."){
                    continue;
                }
                maps->push_back(ent->d_name);
            }
            
            closedir(dir);
            return maps;
        }
        
         std::cout<<"Error: Could not find maps directory";
        return maps;
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
#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include <vector>
struct Block{
    
    float x;
    float y;
    bool blocked=true;
    float width=64;
    float height=64;
    bool isSpawn=false;
  };

class Map{
    int mapScaleX=2;
    int mapScaleY=2;
    private:
    std::vector<Block>Blocks;
    std::vector<Block> spawnPoints;
    
    
   void handleLine(std::string line){
      Block blk;
       for(int i=0;i<line.length();i++){
           if(line.substr(i,5)=="posx="){
               blk.x = atof(getValue(line,i+5).c_str())*mapScaleX;
               }
           if(line.substr(i,5)=="posy="){
               blk.y = atof(getValue(line,i+5).c_str())*mapScaleY;
               }
              if(line.substr(i,8)=="blocked="){
                  std::string v = getValue(line,i+8);
                  if(v=="false"){
                    //  std::cout<<"blocked\n";
                      blk.blocked=false;
                  }
              }
              
                  if(line.substr(i,11)=="spawnPoint="){
                  std::string v = getValue(line,i+11);
                  if(v=="true"){
                    //  std::cout<<"blocked\n";
                      blk.isSpawn=true;
                  }
              }
       }
      Blocks.push_back(blk);
      if(blk.isSpawn){
          spawnPoints.push_back(blk);
      }
   
   }
   std::string getValue(std::string line,int x){
       std::string value="";
       for(int i=x;i<line.length() && line.substr(i,1)!=";";i++){
           value+=line.substr(i,1);
       }
       return value;
   }
    public:
   bool loadMap(std::string map){
       std::cout<<"loading map "<< map<<"\n";
       spawnPoints.clear();
       Blocks.clear();
       std::string line;
       map="./maps/"+map;
        std::ifstream file(map.c_str());
        if(file.is_open()){
            while(std::getline(file,line)){
                handleLine(line);
               
                 
            }
        }else{
            return false;
        }
       
 
       return true; 
    }
    
    
    bool checkCollision(float x,float y, float width,float height){
        
          for(int i=0;i<Blocks.size();i++){
              if(!Blocks[i].blocked){ continue;}
         if(x+width>=Blocks[i].x && x<= Blocks[i].x+Blocks[i].width 
           && y+height >=Blocks[i].y && y<=Blocks[i].y+Blocks[i].height){
               return true;
           }
       }
        return false;
    }
    
    Block *getSpawnPoint(){
        
        return (spawnPoints.size()>0)? &spawnPoints[0]:nullptr;
    }
    
};

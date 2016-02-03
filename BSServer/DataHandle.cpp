#include "BSServer.hpp"
#include "JSONParser.hpp"
#include <string>
#include <thread>
#include <future>
int BSServer::handle_data(Client * con){
    char buffer[512];
  int bytes;
 
  bytes=read(con->getConnection(),buffer,sizeof(buffer));
  buffer[bytes-2]='\0';
  std::stringstream ss;
  if(bytes>0){
   //    cout<<"data?\n";
    
    ss<<buffer;
  }else{
     // std::cout<<"closing connection\n";
      return bytes;
  }
  
   std::string retData = ss.str();
     if(retData=="\n"){
        return bytes;
     }

for(int i=0;i<retData.length();i++){
   if(retData.substr(i,1)=="\n" ){
       std::cout<<"removed return char   " <<i<< "\n";
     retData.erase(i,1);  
 //  i--;
   //return bytes;
}
}
  /*  for(int i=0;i<retData.length();i++){
        if(retData.substr(i,1)=="["){
            retData.replace(i,1,"{");
        }
        if(retData.substr(i,1)=="]"){
            retData.replace(i,1,"}");
        }
    }*/
    std::thread t(&BSServer::static_broadcastPlayerData,this, con->getConnection(),retData);
  t.detach();
 //broadcastPlayerData(con->getConnection(),retData);

//  std::cout<<retData<<std::endl;
 //cout<<ss.str()<<endl;
  //sendData(con->getConnection(),"Hello world");
 /* Dictionary * dict = JSONParser::parseJson(ss.str());
 // dict->printDictionay();
  
  if(dict->getItem("Player")!=nullptr){
      DictionaryItem *player = dict->getItem("Player");
    //Ehhhh this rounds it to 3 decimals
     float x = atof(  player->getItem("x")->value.c_str());
     float y = atof(  player->getItem("y")->value.c_str());
     double angle = atof(  player->getItem("angle")->value.c_str());
      con->move(x,y);
      con->setAngle(angle);
      std::string retData = ss.str();
    for(int i=0;i<retData.length()-1;i++){
        if(retData.substr(i,1)=="["){
            retData.replace(i,1,"{");
        }
        if(retData.substr(i,1)=="]"){
            retData.replace(i,1,"}");
        }
    }
      broadcastPlayerData(con->getConnection(),retData);
      
       //cout<<"X:"<<x<<"  Y:"<<y<<"  Angle:"<<angle<<endl;
  }
    if(dict->getItem("Projectile")!=nullptr){
    std::string retData = ss.str();
    for(int i=0;i<retData.length()-1;i++){
        if(retData.substr(i,1)=="["){
            retData.replace(i,1,"{");
        }
        if(retData.substr(i,1)=="]"){
            retData.replace(i,1,"}");
        }
    }
      broadcastPlayerData(con->getConnection(),retData);
    }
  delete dict;*/
  return bytes;
}
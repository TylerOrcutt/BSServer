#pragma once
#include <iostream>
#include <openssl/ssl.h>
#include <sys/socket.h>
#include <list>
#include <vector>
#include <string>
struct CommandMessage{
       CommandMessage(std::string c,unsigned long t){
           cmd = c;
           timestamp=t;
       }
    std::string cmd;
    unsigned long timestamp;
};
class Client{
  private: 
  //SSL * _ssl;

  int socket;
  int packets_sent=0;
  int packet_sequence=0;
  float x=0,y=0;
  double angle=0;
  unsigned long lastResponce = 0;
  unsigned long lastPing = 0;
  unsigned long pingTimeSent=0;
  unsigned long pongTimeReceived=0;
  std::list<CommandMessage> * commands;
  public:  
  Client(int nsock){
     // _ssl=s;
      socket=nsock;
  commands = new std::list<CommandMessage>();
  }  
    Client(){
          commands = new std::list<CommandMessage>();
    }
 /* SSL* ssl(){
      return _ssl;
  }*/
    int getConnection(){
        return socket;
    }
    void move(float _x,float _y){
        x=_x;
        y=_y;
    }
    void setAngle(double _angle){
        angle=_angle;
    }
    float getX(){
        return x;
    }
     float getY(){
        return y;
    }
    double getAngle(){
        return angle;
    }
    void incPackets_sent(){
        packets_sent++;
    }
    int getPacketsSent(){
        return packets_sent;
    }
    void setSequenceId(int id){
       packet_sequence=id;
    }
    int getSequenceId(){
        return packet_sequence;
    }
    unsigned long getLastResponce(){
        return lastResponce;
    }
    void setLastResponce(unsigned long rp){
        lastResponce=rp;
    }
    
    unsigned long getLastPing(){
        return lastPing;
    }
    void setLastPing(unsigned long rp){
        lastPing=rp;
    }
    
    unsigned long getPingTimeSent(){
        return pingTimeSent;
    }
    void setPingTimeSent(unsigned long rp){
        pingTimeSent=rp;
    }

       unsigned long getPongTimeReceived(){
        return pongTimeReceived;
    }
    void setPongTimeReceived(unsigned long rp){
        pongTimeReceived=rp;
    }
    
    void pushCommand(std::string cmd){
      //  commands->push_back(cmd);
        CommandMessage  cm(cmd,1);
        commands->push_back(cm);
     
        
    }
    int commands_size(){
        return commands->size();
    }
    
   CommandMessage popCommand(){
          CommandMessage cmd = commands->front();
        commands->pop_front();
        return cmd;
    }
    socklen_t clilen;
  struct sockaddr_in  cli_addr;
    
};
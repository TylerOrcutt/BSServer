#pragma once
#include <iostream>
#include <openssl/ssl.h>
#include <sys/socket.h>
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
  public:  
  Client(int nsock){
     // _ssl=s;
      socket=nsock;
  }  
    Client(){
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
    socklen_t clilen;
  struct sockaddr_in  cli_addr;
    
};
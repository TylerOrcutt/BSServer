#pragma once
#include <iostream>
#include <openssl/ssl.h>

class Client{
  private: 
  //SSL * _ssl;
  int socket;
  
  public:  
  Client(int nsock){
     // _ssl=s;
      socket=nsock;
  }  
 /* SSL* ssl(){
      return _ssl;
  }*/
    int getConnection(){
        return socket;
    }
};
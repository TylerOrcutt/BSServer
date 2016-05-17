#pragma once
//TODO: android sockets are wierd fix to work with them..

#define MAX_CLIENTS 16

#include <iostream>
#include <string.h>
#include <sstream>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <resolv.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <vector>
#include <unistd.h>
#include <thread>
#include <future>

#include "Client.hpp"
#include "Helper.hpp"
#include "Config.hpp"
#include "HttpRequest.hpp"
#include <mutex>  
using namespace std;
class BSServer{
 private:
  
  configuration  * config;
  
   int serv;//port;
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;
    struct sockaddr_in serv_addr_out;
  SSL_CTX *ctx;
  bool running=true;

  fd_set master;
fd_set read_fds;
int fdmax;

 std::vector<Client*> *clients;
 std::string idenity = "96e419b4ec90d1409f5d";
 std::string map = "map2";
 public:  
BSServer(){
 //InitServerCTX();
// LoadCertificates( "BS_ALPHA_CERT.pem", "BS_ALPHA_CERT.pem");

 clients= new vector<Client*>();
 config =Config::LoadConfig();
 std::string paramstr = Config::genParamString(config);
 //std::cout<<paramstr<<std::endl;
std::string responce=HTTPRequest::HTTPPostRequest("rustednail.ddns.net","updateserver",paramstr);
if(responce.substr(0,12)=="unauthorized" || responce==""){
    std::cout<<"Exiting.\n";
    running=false;
}
}

/* *** Function definations */
static void static_broadcastPlayerData(BSServer * serv, int con, std::string data){
    serv->broadcastPlayerData(con,data);

}
int handle_data(std::string data, sockaddr_in cli, socklen_t clen);


/*  ***** */ 
void start(){
    
  /*  if(!isRoot()){
      cout<<"Must be ran as root\n";
      return;
    }
*/
serv=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
if(serv<0){ cout<<"sock error\n"; return;}
memset(&serv_addr,sizeof serv_addr,0);

//port=9899;
serv_addr.sin_family=AF_INET;
serv_addr.sin_addr.s_addr=INADDR_ANY;
serv_addr.sin_port =htons(config->port);



if(bind(serv,(struct sockaddr*)&serv_addr,sizeof serv_addr)<0){
  cout<<"error binding\n"; 
  running=false;
  return;
  }
  
  //set to non blocking socket
  /*
 int nonblock=1;
 if(fcntl(serv,F_SETFL,O_NONBLOCK,nonblock)==-1){
     cout<<"failed to set non blocking\n";
     return;
 }*/

while(running){
   //usleep(1000);
    int bytes=0;
     char buffer[512];
    if((bytes=recvfrom(serv,buffer,sizeof(buffer),0,(struct sockaddr*)&cli_addr,&clilen))<=0){
        continue;
    }
    if(cli_addr.sin_addr.s_addr==0){
        continue;
    }
    
     buffer[bytes-2]='\0';
    std::stringstream ss;
     ss<<buffer;
        std::string retData = ss.str();
        if(retData.substr(0,idenity.length()) != idenity){
            continue;//packet doesnt belong to us - invalid idenity
        }
       retData= retData.erase(0,idenity.length()+1);
        

    if(retData=="Identify"){
        //return idenity 
        cout<<"Sent Identify\n";
        sendIdentity(cli_addr,clilen);
        continue;
    }
    
//std::cout<<retData<<endl;
Client *cli;

if((cli = getClient(cli_addr))==nullptr){

    if(clients->size()>= config->max_clients){
        //need to report this back to the client trying to connect
        continue;//server is already at max capacity
    }
 std::cout<< "new connection from "<< cli_addr.sin_addr.s_addr<<std::endl;
    Client *client = new Client();
    client->cli_addr =  cli_addr;
    client->clilen = clilen;
   
    clients->push_back(client);
    cli = client;
    std::stringstream idjson ;
    idjson<< "{\"id\":\""<<cli_addr.sin_addr.s_addr<<"\"}";
    sendData(idjson.str(),cli_addr,clilen);
    
 
 }
 
 
  cli->setLastResponce(Helper::getTime());
//pull sequence id off packet
std::string strid="";

for(int i=0;i<retData.length();i++){
    if(retData.substr(i,1)==":"){
        retData=retData.erase(0,i+1);
        break;
    }else{
        strid+=retData.substr(i,1);
    }
}
  int id = atoi(strid.c_str());
  if(id<cli->getSequenceId()){
      continue; //packet is old, throw it out for now
  }else{
  cli->setSequenceId(id);
  }
  
    if(retData=="Disconnect"){
        std::cout<<"Received disconnect\n";
        int cli_id=-1;
        if((cli_id=getClientId(cli_addr))>=0){
         clients->erase(clients->begin()+cli_id);
        std::cout<<"Client "<<cli_id<<" sent dc\n";
        }
           continue;
    }
  if(retData=="Pong"){
      
       cli->setPongTimeReceived(Helper::getTime());
       std::cout<<"Pong Received, Latency:"<<cli->getPongTimeReceived()-cli->getLastPing()<<"\n";
       continue;
  }
  //  cout<<ss.str()<<endl;
   //  sendto(serv, retData.c_str(), strlen(retData.c_str()), 0, (struct sockaddr*) &cli_addr,clilen);
         
  //  broadcastPlayerData(cli_addr, retData);
  cli->pushCommand(retData);
    
}//
close(serv);
}
bool cliExist(sockaddr_in addr){
     for(int i=0;i<clients->size();i++){
      if((addr.sin_addr.s_addr == (*clients)[i]->cli_addr.sin_addr.s_addr) ){
         return true;
      }
 }
    return false;
}


void broadcastPlayerData(sockaddr_in addr, std::string data){
        for(int i=0;i<clients->size();i++){
     if((addr.sin_addr.s_addr != (*clients)[i]->cli_addr.sin_addr.s_addr) ){
         struct sockaddr_in cli = (*clients)[i]->cli_addr;
           
       //   cli.sin_port =htons(port+1);
      (*clients)[i]->incPackets_sent();
       std::stringstream id;
       id<<(*clients)[i]->getPacketsSent()<<":";
     //  data = id.str() + data;
           
         sendto(serv, data.c_str(), strlen(data.c_str()), 0, (struct sockaddr*) &(*clients)[i]->cli_addr,(*clients)[i]->clilen);
         
     }
 }
}

Client * getClient(sockaddr_in addr){     
    for(int i=0;i<clients->size();i++){
      if((addr.sin_addr.s_addr == (*clients)[i]->cli_addr.sin_addr.s_addr) ){
         return (*clients)[i];
      }
 }
 return nullptr;
    
}

int getClientId(sockaddr_in addr){     
    for(int i=0;i<clients->size();i++){
      if((addr.sin_addr.s_addr == (*clients)[i]->cli_addr.sin_addr.s_addr) ){
         return i;
      }
 }
 return -1;
    
}
void sendIdentity(sockaddr_in addr, socklen_t addrlen){
    std::stringstream json;
    json<<"{\"identity\":{\"Map\":\""<<map<<"\",\"players\":\""<<clients->size()<<"\",\"maxplayers\":\""<<MAX_CLIENTS<<"\"}}";
    sendData(json.str(),addr,addrlen);
    
    
}
void sendData(std::string data, sockaddr_in addr, socklen_t alen){
 //   serv_addr.sin_port=htons(9898);
      sendto(serv, data.c_str(), strlen(data.c_str()), 0, (struct sockaddr*) &addr,alen);
         
}
std::vector<Client*> * getClients(){
    return clients;
}
void setRunning(bool run){
    running=run;
}
bool isRunning(){
    return running;
}
/*serv=socket(AF_INET,SOCK_STREAM,0);
if(serv<0){ cout<<"sock error\n"; return;}

memset(&serv_addr,sizeof serv_addr,0);

port=9898;
serv_addr.sin_family=AF_INET;
serv_addr.sin_addr.s_addr=INADDR_ANY;
serv_addr.sin_port =htons(port);


if(bind(serv,(struct sockaddr*)&serv_addr,sizeof serv_addr)<0){
  cout<<"error binding\n"; return;
  }

listen(serv,1);
FD_SET(serv,&master);
fdmax=serv;
clilen=sizeof cli_addr;
int newsock;
//vector<SSL*> ssl(100);
std::cout<<"Listening for connections\n";
while(true){
read_fds=master;
//gameTime.update();
  if(select(fdmax+1,&read_fds,NULL,NULL,NULL)==-1){
    cout<<"select error\n";
    return;
  }
  for(int i=0;i<=fdmax;i++){
   if(FD_ISSET(i,&read_fds)){//data to be red
    if(i==serv){//new connection

    if((newsock=accept(serv,(struct sockaddr*)&cli_addr,&clilen))!=-1){


      if(newsock >fdmax){
      fdmax=newsock;
      }

      cout<<"new connection, "<<newsock<<endl;
      /*  SSL *ssl;
         ssl = SSL_new(ctx);
        
         SSL_set_fd(ssl,newsock);
        if( SSL_accept(ssl)==-1){
        cout<<"SSL ACcept fail reverting to plain socket.\n";
       // close(newsock);
        //ssl[newsock]=nullptr;
        //delete(ssl);
        FD_CLR(newsock,&master);
      }else{


     
        cout<<"Accepted SSL connection\n";

      }*/
      /*
         Client *cli = new Client(newsock);
        clients->push_back(cli);
        FD_SET(newsock,&master);

      //send_welcome(newsock);
    }else{
      cout<<"error accepting\n";
      close(newsock);
      FD_CLR(newsock,&master);
    }
    }else{ //handle data from client
      for(int con=0;con<clients->size();con++){
        if(i==(*clients)[con]->getConnection()){

              if(handle_data((*clients)[con])<=0){ //connection lost?
               
              //broadcast disconnect here
              //clients[con]->setLogedin(false);
             //   db.savePlayer(clients[con]);
              stringstream ss;
              //ss<<"{\"Disconnected\":\""<<clients[con]->getUsername()<<"\"}";
             // broadcast(ss.str());
              //
              //kickPlayer(i);
              std::cout<<"Connection closed\n";

             // cout<<"connection "<<clients[con]->getConnection()<<" closed.\n";
              close((*clients)[con]->getConnection());
              FD_CLR(i,&master);
              delete((*clients)[con]);
              clients->erase(clients->begin()+con);

              }
         break;
        }
      }

}

  }
}//for

}//while

close(serv);
SSL_CTX_free(ctx);
}*/


  void sendData(int ssl, std::string data){
data=data+" \n";
  int bytes_sent=write(ssl,data.c_str(),strlen(data.c_str()));
    if(bytes_sent==0){
    std::cout<<">>>failed to send message\n";
  }
}
  void sendData(SSL* ssl, std::string data){
//data=data+" \n";
  int bytes_sent=SSL_write(ssl,data.c_str(),strlen(data.c_str()));
    if(bytes_sent==0){
    std::cout<<">>>failed to send message\n";
  }
}

void broadcastPlayerData(int sck, std::string data){
    for(int i=0;i<clients->size();i++){
        if(sck != (*clients)[i]->getConnection()){
            sendData((*clients)[i]->getConnection(),data);
        }
    }
}
//#MARK SSL shit
void LoadCertificates( char* CertFile, char* KeyFile){
    if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
      }
    if ( !SSL_CTX_check_private_key(ctx) )
    {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}
void InitServerCTX()
{
 /* SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    const SSL_METHOD * meth = SSLv3_server_method();
    ctx = SSL_CTX_new(meth);
   if ( ctx == NULL ) {
     ERR_print_errors_fp(stderr);
       abort();
    }*/

}

 bool isRoot() {
      if (getuid() != 0){
          return false;
      }else{
        return true;
      }
    }
  


};
#ifndef _HTTP_REQUEST_HPP_
#define _HTTP_REQUEST_HPP_
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <openssl/ssl.h>
class HTTPRequest{
    public:
  static std::string HTTPPostRequest(std::string webhost,std::string page,std::string params=""){
       // std::cout<<"Positng "<<url<<std::endl;
        
        int sock;
        struct sockaddr_in server;
        int port=80;
        struct hostent *host = gethostbyname(webhost.c_str());
        if(host== NULL || host->h_addr==NULL){
            std::cout<<"host not found\n";
            return "";
        }
        bzero(&server, sizeof(server));
        memset(&server,0,sizeof(struct sockaddr_in));
        server.sin_family=AF_INET;
        server.sin_port = htons(port);
        bcopy((char*)host->h_addr,(char*)&server.sin_addr.s_addr,host->h_length);
        
        sock = socket(AF_INET,SOCK_STREAM,0);
        if(sock<0){
            std::cout<<"Connection to host failed\n";
            return "";
        }
        if(connect(sock,(struct sockaddr*)&server,sizeof(server))<0){
            close(sock);
            std::cout<<"Connection failed\n";
            return "";
        }
        std::stringstream ss;
        ss<<"POST /"<<page<<" HTTP/1.1\r\n";
        ss<<"Host: "<<webhost<<"\r\n";
        ss<<"Accept: */*\r\n";
        ss<<"Connection: close\r\n";
        ss<<"Content-Type: application/x-www-form-urlencoded\r\n";
        ss<<"Content-Length: "<<params.length()<<"\r\n\r\n";
        ss<<params;
        
        if(send(sock,ss.str().c_str(),ss.str().length(),0)<0){
            std::cout<<"error making request\n";
            close(sock);
            return "";
        }
        char cur;
        std::stringstream htmlstream;
         while(read(sock,&cur,1)>0){
           htmlstream<<cur;
         }
     
       std::string html = htmlstream.str();
         
       for(int i=0;i<html.length();i++){
           if(html.substr(i,4)=="\r\n\r\n"){
                     html = html.substr(i+4,html.length()-i-4);  
           }
       }
       
       //std::cout<<"\n\n";
       std::cout<<html<<std::endl;
        close(sock);
      return html;
  }
      static std::string HTTPSPostRequest(std::string webhost,std::string page,std::string params=""){
       // std::cout<<"Positng "<<url<<std::endl;
        SSL_load_error_strings();
        SSL_library_init();
        OpenSSL_add_all_algorithms();
        SSL_CTX * ctx = SSL_CTX_new(SSLv23_client_method());
       
       
       
       
        int sock;
        struct sockaddr_in server;
        int port=443;
        struct hostent *host = gethostbyname(webhost.c_str());
        if(host== NULL || host->h_addr==NULL){
            std::cout<<"host not found\n";
            return "";
        }
        bzero(&server, sizeof(server));
        memset(&server,0,sizeof(struct sockaddr_in));
        server.sin_family=AF_INET;
        server.sin_port = htons(port);
        bcopy((char*)host->h_addr,(char*)&server.sin_addr.s_addr,host->h_length);
        
        sock = socket(AF_INET,SOCK_STREAM,0);
        if(sock<0){
            std::cout<<"Connection to host failed\n";
            return "";
        }
        if(connect(sock,(struct sockaddr*)&server,sizeof(server))<0){
            close(sock);
            std::cout<<"Connection failed\n";
            return "";
        }
        
        std::stringstream ss;
        ss<<"POST /"<<page<<" HTTP/1.1\r\n";
        ss<<"Host: "<<webhost<<"\r\n";
        ss<<"Accept: */*\r\n";
        ss<<"Connection: close\r\n";
        ss<<"Content-Type: application/x-www-form-urlencoded\r\n";
        ss<<"Content-Length: "<<params.length()<<"\r\n\r\n";
        ss<<params;
        
        
        SSL*ssl = SSL_new(ctx);
        SSL_set_fd(ssl,sock);
        SSL_connect(ssl);
      ShowCerts(ssl);
        if(SSL_write(ssl,ss.str().c_str(),ss.str().length())<0){
     //   if(send(sock,ss.str().c_str(),ss.str().length(),0)<0){
            std::cout<<"error making request\n";
            close(sock);
            return "";
        }
        char cur;
        std::stringstream htmlstream;
         while(SSL_read(ssl,&cur,1)>0){
           htmlstream<<cur;
         }
     
       std::string html = htmlstream.str();
         
       for(int i=0;i<html.length();i++){
           if(html.substr(i,4)=="\r\n\r\n"){
                     html = html.substr(i+4,html.length()-i-4);  
           }
       }
       
       //std::cout<<"\n\n";
       std::cout<<html<<std::endl;
        close(sock);
      return html;
  }
  
  static void ShowCerts(SSL* ssl){
      X509 *cert;
      char *line;
      cert =   SSL_get_peer_certificate(ssl);
      if(cert!=NULL){
          std::cout<<"Server Certs:\n";
           line = X509_NAME_oneline(X509_get_subject_name(cert),0,0);
           std::cout<<line<<std::endl;
           delete line;
          X509_free(cert);
      }
      
  }
};

#endif 
#include <iostream>
#include "BSServer.hpp"
#include "Engine.hpp"
#include <thread>
static void engineThread(BSServer *serv){
    Engine *eng;
    eng = new Engine(serv);
    eng->run();
}
int main(){
    BSServer *  serv;
    serv = new BSServer();
  
    
    std::thread t(&engineThread,serv);
 
      serv->start();
      t.join();
 
    return 0;
}
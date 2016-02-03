#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "Dictionary.hpp"

class JSONParser {
public:
static Dictionary * parseJson(std::string data){
  Dictionary * a = new Dictionary();
  for(int i=0;i<data.length();i++){
    if(data.substr(i,1)=="\""){
      DictionaryItem  n;
      n.key = getValue(data,i+1);
      i+=n.key.length();
      //ignore data till we find the value pair
      for(int k=i;k<data.length()-1 && data.substr(k,1)!=":";k++){i=k;}
     i++;
      for(int k=i;k<data.length()-1 && data.substr(k,1)!="\"" && data.substr(k,1)!="[";k++ ){i=k;}
      //value pair
      i++;
      if(data.substr(i,1)=="\""){
      //  std::cout<<"value pair\n";
          n.value = getValue(data,i+1);
          i+=n.value.length();
            for(int k=i;k<data.length() && data.substr(k,1)==",";k++){i=k;}
            i++;
      }
        if(data.substr(i,1)=="["){
          i++;
      //    std::cout<<"array pair\n";
        n.items=  getSubItems(data, &i);
          }
             a->push_back(n);
    }
    //  a->push_back(n);
    }

  return a;
}

static std::vector<DictionaryItem> getSubItems(std::string data, int *index){
std::vector<DictionaryItem> a;
//std::cout<<"getting sub items\n\n";

for(int i=(*index);i<data.length() && data.substr(i,1) !="]";i++){
  if(data.substr(i,1)=="\""){
    DictionaryItem  n;
    n.key = getValue(data,i+1);
    i+=n.key.length();
    //ignore data till we find the value pair
    for(int k=i;k<data.length() && data.substr(k,1)!=":";k++){i=k;}
    i++;
    for(int k=i;k<data.length() && data.substr(k,1)!="\"" && data.substr(k,1)!="[";k++ ){i=k;}
    //value pair
    i++;
    if(data.substr(i,1)=="\""){
    //  std::cout<<"value pair\n";
        n.value = getValue(data,i+1);
        i+=n.value.length();
          for(int k=i;k<data.length() && data.substr(k,1)==",";k++){i=k;}
          i++;
    }else
      if(data.substr(i,1)=="["){
        i++;
        std::cout<<"array pair\n";
      n.items=  getSubItems(data, &i);
        }
   a.push_back(n);
  }
  (*index)=i;
  }

  return a;

}

 static std::string getValue(std::string data, int index){
  std::string value="";
  for(int i=index; i<data.length();i++){
    if(data.substr(i,1) == "\""){
      break;
    }
    value+=data.substr(i,1);
  }
  return value;
}
};
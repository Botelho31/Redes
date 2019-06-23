#include "../include/HTTPRequest.h"
HTTPRequest::HTTPRequest(std::map<std::string,std::vector<std::string>> httpParams){
    method = "";
    connection = "";
    host = "";
    for(auto it = httpParams.cbegin(); it != httpParams.cend(); ++it){
        for(int i = 0;i < it->second.size();i++){
            if((it->first == "ResponseHeader") && (i == 0)){
                this->method = it->second[i];
            }
            if((it->first == "Host") && (i == 0)){
                this->host = it->second[i];
            }
            if((it->first == "Connection") && (i == 0)){
                this->connection = it->second[i];
            }
        }
    }
}
#include "../include/HTTPRequest.h"
HTTPRequest::HTTPRequest(std::map<std::string,std::vector<std::string>> httpParams){
    method = "";
    connection = "";
    host = "";
    html = "";
    this->params = httpParams;
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
            if((it->first == "HTML") && (i == 0)){
                this->html = it->second[i];
            }
        }
    }
}

std::string HTTPRequest::GetCleanedRequest(){
    std::stringstream cleanedrequest;

    for(int i = 0;i < params["ResponseHeader"].size();i++){
        cleanedrequest << params["ResponseHeader"][i] << " ";
    }
    cleanedrequest << "\r\n";
    for(auto it = params.cbegin(); it != params.cend(); ++it){

        if((it->first != "ResponseHeader") && (it->first != "Accept-Encoding") && (it->first != "Connection") && (it->first != "HTML")){
            cleanedrequest << it->first << ": ";
            for(int i = 0;i < it->second.size();i++){
                cleanedrequest << it->second[i] << " ";
            }
            cleanedrequest << "\r\n";
        }
    }
    cleanedrequest << "Connection: close \r\n";
    cleanedrequest << "\r\n";
    std::cout << cleanedrequest.str() << std::endl;
    return cleanedrequest.str();
}
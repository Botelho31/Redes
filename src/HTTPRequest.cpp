#include "../include/HTTPRequest.h"

//Recebe um dicionario de parametros do HTTP e pega os dados dele de forma segura para a leitura
HTTPRequest::HTTPRequest(std::map<std::string,std::vector<std::string>> httpParams){
    method = "";
    connection = "";
    host = "";
    html = "";
    accept = "";
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
            if(it->first == "Accept" && (i == 0)){
                std::stringstream acceptstream;
                acceptstream << it->second[i];
                getline(acceptstream,this->accept,'/');
            }
        }
    }
}

//Refaz a request com alguns itens diferentes para facilitar a interpretação da response
std::string HTTPRequest::GetCleanedRequest(){
    std::stringstream cleanedrequest;

    for(int i = 0;i < params["ResponseHeader"].size();i++){
        cleanedrequest << params["ResponseHeader"][i];
        if(i != (params["ResponseHeader"].size() - 1)){
            cleanedrequest << " ";
        }
    }
    cleanedrequest << "\r\n";
    for(auto it = params.cbegin(); it != params.cend(); ++it){

        if((it->first != "ResponseHeader") && (it->first != "Connection") && (it->first != "Accept-Encoding") && (it->first != "HTML") && (it->first.size() > 2)){
            cleanedrequest << it->first << ": ";
            for(int i = 0;i < it->second.size();i++){
                cleanedrequest << it->second[i];
                if(i !=  (it->second.size() - 1)){
                    cleanedrequest << " ";
                }
            }
            cleanedrequest << "\r\n";
        }
    }
    cleanedrequest << "Connection: close \r\n";
    cleanedrequest << "\r\n";
    std::cout << cleanedrequest.str() << std::endl;
    return cleanedrequest.str();
}
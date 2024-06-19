//#ifndef API_CLIENT_CPP
//#define API_CLIENT_CPP
//
//#include <iostream>
//#include <string>
//#include "../../external/restclient/include/restclient.h"
//#include "../../external/json/json.hpp"
//#include "../../external/restclient/include/connection.h"
//#include "models.cpp"
//#include "../../logger/logger.h"
//
//using json = nlohmann::json;
//
//class ApiClient
//{
//private:
//
//    std::string api_url = API_URL;
//    RestClient::Connection* connection;
//
//    json genericGet(const std::string& endpoint)
//    {
//        RestClient::Response r = connection->get(endpoint);
//        if(r.code != 200)
//        {
//            Logger::logError("Error getting from API: " + r.body);
//            throw std::runtime_error("Error getting from API");
//        }
//        return json::parse(r.body);
//    }
//
//
//public:
//
//    ApiClient ()
//    {
//        RestClient::init();
//        connection = new RestClient::Connection(api_url);
//        RestClient::HeaderFields headers;
//        //headers["Authorization"] = "Bearer " + api_token;
//        connection->SetHeaders(headers);
//        connection->SetTimeout(5);
//    }
//
//    ApiModels::Text getText(unsigned int id)
//    {
//        json j = genericGet("texts/" + std::to_string(id));
//        return j.template get<ApiModels::Text>();
//    }
//
//
////    void getActiveSlideshow ()
////    {
////        RestClient::Response r = connection->get("slideshows/active");
////
////        // Try to parse the response
////        json j = json::parse(r.body);
//////        std::cout << j["id"] << std::endl;
//////        std::cout << j["slideshow_widgets"][0] << std::endl;
////        return;
////    }
//
//    void test ()
//    {
//        RestClient::Response r = RestClient::get(api_url + "test2");
//        std::cout << r.body  << std::endl;
//
//        // Post check
//        RestClient::Response r2 = RestClient::post(api_url + "test", "application/json", "{\"test\": \"test\"}");
//        std::cout << r2.body  << std::endl;
//
//        // Try to parse the response
//        json j = json::parse(r2.body);
//        std::cout << j["test"] << std::endl;
//
//        return;
//    }
//};
//
//#endif
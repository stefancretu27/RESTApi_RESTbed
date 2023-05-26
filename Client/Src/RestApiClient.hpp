#pragma once

#include <memory>
#include <future>
#include <iostream>
#include <string>
#include <fstream>

//3rd party
#include <restbed>
#include <nlohmann/json.hpp>

class RestApiClient
{
    public:
    enum class RequestType
    {
        POST,
        GET,
        PUT,
        PATCH,
        DELETE
    };

    static void PrintResponse(const std::shared_ptr<restbed::Response>& );
    static void PrintRequest(const std::shared_ptr<restbed::Request>& );

    void SendAsyncRequest(const RequestType& reqType, const std::string& messageBody = {});
    void SendSyncRequest(const RequestType& reqType, const std::string& messageBody = {});


    private:
    std::string ConvertRequestTypeToString(const RequestType& reqType);
    std::shared_ptr<restbed::Response> mSpResponse;
    std::shared_ptr<restbed::Request> mSpRequest;
};
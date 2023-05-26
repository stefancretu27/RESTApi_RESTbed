#include "RestApiClient.hpp"

using namespace std;
using namespace restbed;
using json = nlohmann::json;


int main( const int, const char** )
{
    RestApiClient client{};

    std::ifstream inputJsonFile("Client/example.json");

    json jsonPOST{};

    if(inputJsonFile.is_open())
    {
        jsonPOST = json::parse(inputJsonFile);
    }

    client.SendAsyncRequest(RestApiClient::RequestType::POST, jsonPOST.dump());

    //--------------------------------------------------------------------------------
    
    client.SendAsyncRequest(RestApiClient::RequestType::GET);

    //--------------------------------------------------------------------------------

    json jsonPUT = json::parse(R"({"Id":1, 
                                    "Name":"FirstResourceChanged",
                                    "Qty":5}
                                )");

    client.SendSyncRequest(RestApiClient::RequestType::PUT, jsonPUT.dump());

    //--------------------------------------------------------------------------------

    client.SendSyncRequest(RestApiClient::RequestType::GET);

    //--------------------------------------------------------------------------------

    json jsonPATCH = //R"({"Name":"ResourceNamePatched","NewEntry":"patchValue"})"_json;
                    {
                        {"Name", "ResourceNamePatched"},
                        {"NewEntry", "patchValue"},
                        {"wyh", 3.14159}
                    };

    client.SendAsyncRequest(RestApiClient::RequestType::PATCH, jsonPATCH.dump());

    //--------------------------------------------------------------------------------

    client.SendAsyncRequest(RestApiClient::RequestType::GET);

    //--------------------------------------------------------------------------------

    client.SendSyncRequest(RestApiClient::RequestType::DELETE);


    return EXIT_SUCCESS;
}
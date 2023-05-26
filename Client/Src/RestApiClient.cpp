#include "RestApiClient.hpp"

using namespace std;
using namespace restbed;
using json = nlohmann::json;

/*
 * Resource: Represents a HTTP response with additional helper methods for manipulating data, and improving code readability. 
 */
void RestApiClient::PrintResponse(const std::shared_ptr<restbed::Response>& spResponse)
{
    cout<<"[CLIENT][ServerResponse] Status Code: "<< spResponse->get_status_code()<<endl;
    cout<<"[CLIENT][ServerResponse] Status Message: "<< spResponse->get_status_message().data()<<endl;
    cout<<"[CLIENT][ServerResponse] HTTP Version: "<< spResponse->get_version()<<endl;
    cout<<"[CLIENT][ServerResponse] HTTP Protocol: "<< spResponse->get_protocol().data()<<endl;

    for ( const auto header : spResponse->get_headers( ) )
    {
        cout<<"[CLIENT][ServerResponse][Header]: "<<header.first.data()<<" "<<header.second.data()<<endl;
    }

    auto length = spResponse->get_header( "Content-Length", 0 );

    if(length > 0)
    {
        Http::fetch( length, spResponse );

        cout<<"[CLIENT][ServerResponse][Body]: "<<spResponse->get_body().data()<<endl;
    }
}

void RestApiClient::PrintRequest(const std::shared_ptr<restbed::Request>& spRequest)
{
    //parse some header entries
    size_t content_length = spRequest->get_header( "Content-Length", 0);
    string host = spRequest->get_header( "Host");
    string origin = spRequest->get_header( "Origin");
    string content_type = spRequest->get_header( "Content-Type");

    cout<<endl<<"--------------------------------------------------------------------------------"<<endl;
    cout<<"[CLIENT][RequestToServer] New request sent with the following method: "<<spRequest->get_method()<<endl;
    cout<<"[CLIENT][RequestToServer] protocol: "<<spRequest->get_protocol()<<endl;
    cout<<"[CLIENT][RequestToServer] port: "<<spRequest->get_port()<<endl;
    cout<<"[CLIENT][RequestToServer] version: "<<spRequest->get_version()<<endl;
    cout<<"[CLIENT][RequestToServer][Header] request content type: "<<content_type<<endl;
    cout<<"[CLIENT][RequestToServer][Header] request content length: "<<content_length<<endl;
    cout<<"[CLIENT][RequestToServer][Header] path: "<<spRequest->get_path()<<endl;
    cout<<"[CLIENT][RequestToServer][Header] host: "<<host<<" "<<spRequest->get_host()<<endl;
    cout<<"[CLIENT][RequestToServer][Header] accept: "<<spRequest->get_header("Accept")<<endl;

    if(content_length > 0)
    {
        cout<<"[CLIENT][RequestToServer][Body]: "<<spRequest->get_body().data()<<endl;
    }
}

std::string RestApiClient::ConvertRequestTypeToString(const RequestType& reqType)
{
    string result{"GET"};

    switch (reqType)
    {
        case RequestType::POST:
            result = "POST";
            break;

        case RequestType::PUT:
            result = "PUT";
            break;

        case RequestType::PATCH:
            result = "PATCH";
            break;

        case RequestType::DELETE:
            result = "DELETE";
            break;
        
        default:
            result = "GET";
            break;
    }

    return result;
}

void RestApiClient::SendAsyncRequest(const RestApiClient::RequestType& reqType, const std::string& messageBody)
{
    mSpRequest = make_shared< Request >(Uri("http://localhost:8899/root_resource"));
    mSpRequest->set_header("Accept", "*/*" );
    mSpRequest->set_header("Host", "localhost" );
    mSpRequest->set_header("Content-Type" , "application/json");
    mSpRequest->set_protocol("HTTP");
    mSpRequest->set_port(8899);
    mSpRequest->set_method(ConvertRequestTypeToString(reqType));

    if(size_t msgSize = messageBody.size())
    {
        mSpRequest->set_header("Content-Length" , to_string(msgSize));
        mSpRequest->set_body(messageBody);
    }

    future<shared_ptr<Response>> futureAsyncResult = Http::async( mSpRequest, [](const shared_ptr<Request>& request, const shared_ptr<Response>& response)
                                                                                {
                                                                                    RestApiClient::PrintRequest(request);
                                                                                    RestApiClient::PrintResponse(response);
                                                                                }
                                                                );
    futureAsyncResult.wait();

    mSpResponse = futureAsyncResult.get();
}

void RestApiClient::SendSyncRequest(const RestApiClient::RequestType& reqType, const std::string& messageBody)
{
    mSpRequest = make_shared< Request >(Uri("http://localhost:8899/root_resource"));
    mSpRequest->set_header("Accept", "*/*" );
    mSpRequest->set_header("Host", "localhost" );
    mSpRequest->set_header("Content-Type" , "application/json");
    mSpRequest->set_protocol("HTTP");
    mSpRequest->set_port(8899);
    mSpRequest->set_method(ConvertRequestTypeToString(reqType));

    if(size_t msgSize = messageBody.size())
    {
        mSpRequest->set_header("Content-Length" , to_string(msgSize));
        mSpRequest->set_body(messageBody);
    }

    mSpResponse = Http::sync(mSpRequest);

    RestApiClient::PrintRequest(mSpRequest);
    RestApiClient::PrintResponse(mSpResponse);
}

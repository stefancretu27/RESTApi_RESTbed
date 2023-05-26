#include "BusinessLogic.hpp"

#include <utility>

//3rd party
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;
using namespace restbed;
using namespace placeholders;


BusinessLogic& BusinessLogic::GetInstance()
{
    static BusinessLogic instance{};

    return instance;
}

void BusinessLogic::PrintRequestDetails(const shared_ptr<const restbed::Request >& request)
{
    //parse some header entries
    size_t content_length = request->get_header( "Content-Length", 0);
    string host = request->get_header( "Host");
    string origin = request->get_header( "Origin");
    string content_type = request->get_header( "Content-Type");

    cout<<"--------------------------------------------------------------------------------"<<endl;
    cout<<"[SERVER][ClientRequest] New request received with the following method: "<<request->get_method()<<endl;
    cout<<"[SERVER][ClientRequest] protocol: "<<request->get_protocol()<<endl;
    cout<<"[SERVER][ClientRequest] port: "<<request->get_port()<<endl;
    cout<<"[SERVER][ClientRequest] version: "<<request->get_version()<<endl;
    cout<<"[SERVER][ClientRequest][Header] request content type: "<<content_type<<endl;
    cout<<"[SERVER][ClientRequest][Header] request content length: "<<content_length<<endl;
    cout<<"[SERVER][ClientRequest][Header] path: "<<request->get_path()<<endl;
    cout<<"[SERVER][ClientRequest][Header] host: "<<host<<" "<<request->get_host()<<endl;
    cout<<"[SERVER][ClientRequest][Header] accept: "<<request->get_header("Accept")<<endl;
}

bool BusinessLogic::CreateHttpResource(const string& path)
{
    shared_ptr<Resource> newSpResource{make_shared<Resource>()};

    newSpResource->set_path(path);
    newSpResource->set_method_handler( "GET", bind(&BusinessLogic::GETHandler, this, _1 ));
    newSpResource->set_method_handler( "POST", bind(&BusinessLogic::POSTHandler, this, _1 ));
    newSpResource->set_method_handler( "PUT", bind(&BusinessLogic::PUTHandler, this, _1 ));
    newSpResource->set_method_handler( "DELETE", bind(&BusinessLogic::DELETEHandler, this, _1 ));
    newSpResource->set_method_handler( "PATCH", bind(&BusinessLogic::PATCHHandler, this, _1 ));

    pair<map<string, shared_ptr<Resource>>::iterator, bool> mapEmplaceResult = mSpResources.emplace(path, newSpResource);

    return mapEmplaceResult.second;
}

shared_ptr<Resource> BusinessLogic::GetHttpResource(const string& path)
{
    shared_ptr<Resource> retrievedSpResource{nullptr};

    map<string, shared_ptr<Resource>>::iterator mapFindResult = mSpResources.find(path);

    if(mapFindResult != mSpResources.end())
    {
        retrievedSpResource = mapFindResult->second;
    }

    return retrievedSpResource;
}

/* 
* POST is used to create a new resource. A POST request requires a body in which you define the data of the entity to be created.
* A successful POST request would be a 200 response code.
*/
void BusinessLogic::POSTHandler( const shared_ptr< Session >& session )
{
    auto&& request = session->get_request( );

    //get the length of the request's body
    if(request)
    {
        PrintRequestDetails(request);

        size_t content_length = request->get_header( "Content-Length", 0 );

        //the callback is executed after the request's body is fetched
        session->fetch( content_length, [ request, self = this ]( const shared_ptr< Session >& session, const Bytes & body )
                                        {
                                            string closing_message{"Session closed!"};

                                            string requestBody{reinterpret_cast<const char*>(body.data())};

                                            auto resultPair = self->mPersistence.emplace(make_pair(request->get_path(), requestBody));

                                            if(resultPair.second)
                                            {
                                                cout<<"[SERVER][ClientRequest][Body] fetched body size: "<< body.size()<<" "<<" fetched data: ";
                                                cout<<requestBody<< endl;

                                                closing_message = "Session closed after inserting new entry with the following data";
                                                closing_message += "    " + requestBody;
                                            }

                                            session->close( OK, closing_message, { { "Content-Length", to_string(closing_message.size()) }} );
                                        } 
		);
    }
}

// A successful GET returns a response containing the information you requested. 
void BusinessLogic::GETHandler( const shared_ptr< Session >& session )
{
    auto&& request = session->get_request( );

    if(request)
    {
        PrintRequestDetails(request);

        size_t content_length = request->get_header( "Content-Length", 0 );

        auto&& resource = session->get_resource();

        session->fetch( content_length, [ request, self = this]( const shared_ptr< Session >& session, const Bytes& body )
                                        {
                                            string closing_message{"Session closed without retrieving the entry!"};

                                            auto&& iterator = self->mPersistence.find(request->get_path());

                                            if(iterator != self->mPersistence.end())
                                            {
                                                cout<<"Data found at persistency level: "<<iterator->second<<endl;

                                                closing_message = "Session closed after the entry was retrieved!";
                                                closing_message += "    " + iterator->second;
                                            }

                                            session->close( OK, 
                                                            closing_message, 
                                                            { { "Content-Length", to_string(closing_message.size()) }});
                                        } 
                        );
    }
}

/*
* PUT updates the entire resource with data that is passed in the body payload. 
*/
void BusinessLogic::PUTHandler( const shared_ptr< Session >& session )
{
    auto&& request = session->get_request( );

    //get the length of the request's body
    if(request)
    {
        PrintRequestDetails(request);

        size_t content_length = request->get_header( "Content-Length", 0 );
    
        //the callback is executed after the request's body is fetched
        session->fetch( content_length, [ request, self = this ]( const shared_ptr< Session >& session, const Bytes & body )
                                        {
                                            string closing_message{"Session closed without changing the entry!"};

                                            string requestBody{reinterpret_cast<const char*>(body.data())};

                                            auto&& iterator = self->mPersistence.find(request->get_path());

                                            if(iterator != self->mPersistence.end())
                                            {
                                                cout<<"[SERVER][ClientRequest][Header] fetched body size: "<< body.size()<<" "<<" fetched data: ";
                                                cout<<requestBody<< endl;

                                                cout<<" Before PUT: "<<iterator->second<<endl;
                                                iterator->second = requestBody;
                                                cout<<" After PUT: "<<iterator->second<<endl;

                                                closing_message = "Session closed after an existing entry was updated with the following data";
                                                closing_message += "    " + requestBody;
                                            }

                                            session->close( OK, closing_message, { { "Content-Length", to_string(closing_message.size()) } });
                                        } 
		);
    }
}

void BusinessLogic::DELETEHandler( const shared_ptr< Session >& session )
{
    auto&& request = session->get_request();

    if(request)
    {
        PrintRequestDetails(request);

        size_t content_length = request->get_header( "Content-Length", 0 );

        auto&& resource = session->get_resource();

        session->fetch( content_length, [ request, self = this]( const shared_ptr< Session >& session, const Bytes& body )
                                        {
                                            string closing_message{"Session closed without deleting the entry!"};

                                            auto&& iterator = self->mPersistence.find(request->get_path());

                                            if(iterator != self->mPersistence.end())
                                            {
                                                self->mPersistence.erase(iterator);

                                                closing_message = "Session closed after deleting the entry!";
                                            }

                                            session->close( OK, closing_message, { { "Content-Length", to_string(closing_message.size()) } });
                                        } 
                    );
    }
}

/*
* PATCH to modify a part of a resource. With PATCH, you only need to pass in the data that you want to update. 
*/
void BusinessLogic::PATCHHandler( const shared_ptr< Session >& session )
{
    auto&& request = session->get_request( );

    //get the length of the request's body
    if(request)
    {
        PrintRequestDetails(request);

        size_t content_length = request->get_header( "Content-Length", 0 );
    
        //the callback is executed after the request's body is fetched
        session->fetch( content_length, [ request, self = this ]( const shared_ptr< Session >& session, const Bytes & body )
                                        {
                                            string closing_message{"Session closed without changing the entry!"};

                                            string requestBody{reinterpret_cast<const char*>(body.data())};

                                            auto&& iterator = self->mPersistence.find(request->get_path());

                                            if(iterator != self->mPersistence.end())
                                            {
                                                cout<<"[SERVER][ClientRequest][Header] fetched body size: "<< body.size()<<" "<<" fetched data: ";
                                                cout<<requestBody<< endl;

                                                json initialValue = json::parse(iterator->second);
   
                                                json j_patch = json::parse(requestBody);

                                                // apply the patch
                                                cout<<" Before patch: "<<initialValue.dump()<<endl;
                                                initialValue.merge_patch(j_patch);
                                                cout<<" After patch: "<<initialValue.dump()<<endl;

                                                closing_message = "Session closed after an existing entry was patched with the following data";
                                                closing_message += "    " + initialValue.dump();

                                                self->mPersistence[request->get_path()] = initialValue.dump();
                                            }

                                            session->close( OK, closing_message, { { "Content-Length", to_string(closing_message.size()) }} );
                                        } 
		);
    }
}

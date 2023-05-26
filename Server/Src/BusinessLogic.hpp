#pragma once

#include <iostream>
#include <memory>
#include <map> 
#include <array>

//3rd party
#include <restbed>

class BusinessLogic
{
    public:
        ~BusinessLogic() = default;

        //Singleton
        static BusinessLogic& GetInstance();

        //copy and move semantics are useless for singletons
        BusinessLogic(const BusinessLogic&) = delete;
        BusinessLogic& operator=(const BusinessLogic&) = delete;
        BusinessLogic(BusinessLogic&&) = delete;
        BusinessLogic& operator=(BusinessLogic&&) = delete;
        
        bool CreateHttpResource(const std::string& path);
        void CreateHttpServer(const std::shared_ptr<restbed::Settings>& settings);

        //getters
        std::shared_ptr<restbed::Resource> GetHttpResource(const std::string& path);

    private:
        
        BusinessLogic() = default;

        //HTTP requests handlers
        void POSTHandler( const std::shared_ptr< restbed::Session >& session );
        void GETHandler( const std::shared_ptr< restbed::Session >& session );
        void PUTHandler( const std::shared_ptr< restbed::Session >& session );
        void DELETEHandler( const std::shared_ptr< restbed::Session >& session );
        void PATCHHandler( const std::shared_ptr< restbed::Session >& session );

        //server logging
        void PrintRequestDetails(const std::shared_ptr<const restbed::Request >& request);

        //store each created resource that is identifiable by its path (string)
        std::map<std::string, std::shared_ptr<restbed::Resource>> mSpResources;

        //persistence emulator used for storing a http request's body data (JSON like strings) mapped to resource's path
        std::map<std::string, std::string> mPersistence;

};
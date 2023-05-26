#include "BusinessLogic.hpp"

using namespace std;
using namespace restbed;
using namespace placeholders;


int main( const int, const char** )
{
    bool isRootResourceCreated = BusinessLogic::GetInstance().CreateHttpResource("/root_resource" );

    if(isRootResourceCreated)
    {
        auto&& rootResource = BusinessLogic::GetInstance().GetHttpResource("/root_resource" );
        
        auto&& httpSettings = make_shared<Settings>();
        httpSettings->set_port(8899);
        httpSettings->set_default_header( "Connection", "close" );

        Service httpServer{};
        //all resources must be published before the server's start, as it enters in a listening loop
        httpServer.publish(rootResource);
        httpServer.start(httpSettings);      
    }
    else
    {
        std::cout<<"Root resource couldn't be created!"<<std::endl;
    }

    return EXIT_SUCCESS;
}

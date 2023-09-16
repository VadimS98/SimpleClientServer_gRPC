#include <myproto/error.pb.h>
#include <myproto/print.grpc.pb.h>
#include "google/protobuf/empty.pb.h"

#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>

#include <iostream>

// Service to process requests

class PrintService final : public greetandprint::Print::Service {
public:
    virtual ::grpc::Status Print(::grpc::ServerContext* context, const ::greetandprint::Command* request, ::greetandprint::Error* response)
    {
        if (request->command() == "hello") {
            std::cout << "hello" << std::endl;
        }
        else if (request->command() == "print") {
            std::cout << request->text() << std::endl;
        }
        else if (request->command() == "exit") {
            exit(0);
        }
        else {
            std::cout << "Server: GotCommand for \"" << request->command() << "\"." << std::endl;

            response->set_answer("Sorry, currently I'm just a young server who cant handle such requests. Come back later!");
        }

        return grpc::Status::OK;
    }
};

int main(int argc, char* argv[])
{
    std::string server_address("0.0.0.0:50051");

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    PrintService printService;
    builder.RegisterService(&printService);

    // Initialize server

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
    
    return 0;
}
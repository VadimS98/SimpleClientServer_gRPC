#include <myproto/error.pb.h>
#include <myproto/print.grpc.pb.h>
#include "google/protobuf/empty.pb.h"

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>

#include <iostream>
#include <string>

// Client that manages what func to call

class MyServiceClient {
public:
    MyServiceClient(std::shared_ptr<grpc::Channel> printChannel)
        : _printStub(greetandprint::Print::NewStub(printChannel)) {}

    void SendCommand(const std::string& command, const std::string& message) \
    {
        if (command == "hello") {
            SendGreeting(command);
        }
        else if (command == "print")
        {
            SendPrinting(command, message);
        }
        else
        {
            UnknownCommand(command);
        }
    }

private:

    // Just an error responce

    void UnknownCommand(const std::string& command) 
    {
        greetandprint::Command request;
        request.set_command(command);

        greetandprint::Error response = SendRequest(request);

        std::cout << response.answer() << std::endl;
    }

    // To create 'hello' request

    void SendGreeting(const std::string& command)
    {
        greetandprint::Command request;
        request.set_command(command);

        SendRequest(request);
    }

    // To create 'print' request with message

    void SendPrinting(const std::string& command, const std::string& message)
    {
        greetandprint::Command request;
        request.set_command(command);
        request.set_text(message);

        SendRequest(request);
    }

    // To send premade requestes && get responce 

    greetandprint::Error SendRequest(greetandprint::Command request)
    {
        greetandprint::Error response;
        grpc::ClientContext context;

        grpc::Status status = _printStub->Print(&context, request, &response);

        if (status.ok())
        {
            std::cout << "Request sent successfully." << std::endl;
        }
        else
        {
            std::cerr << "RPC failed: " << status.error_message() << std::endl;
        }

        return response;
    }

    std::unique_ptr<greetandprint::Print::Stub> _printStub;
};

int main(int argc, char* argv[])
{   
    // Initialize ClientObj && connect to server via server address
    std::string server_address("localhost:50051");
    MyServiceClient printClient(grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));

    // Pick && parse command from command line to form msg

    std::string input = "";
    while (1) {
        getline(std::cin, input);
        std::string firstWord = input.substr(0, input.find(" "));
        
        std::string message = "";

        if (firstWord == "print") {
            message = input.substr(input.find(" ") + 1);
        }

        // To exit
        else if (firstWord == "exit")
        {
            return 0;
        }
        else
        {
            // Do nothing
        }

        printClient.SendCommand(firstWord, message);
    }

    return 0;
}
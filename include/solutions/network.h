#include <stdio.h>
#include <czmq.h>
#include <string.h>
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <sstream>

#define SetConnection(x) printf("Connecting to the server…\n");void *context = zmq_ctx_new ();void *requester = zmq_socket (context, ZMQ_REQ);zmq_connect (requester, x);
#define EndConnection() zmq_close(requester);zmq_ctx_destroy(context);return 0;

struct Nvec3
{
    float x,y,z;
};
struct Nvec2
{
    float Theta,Phi;
};

struct NetworkOutPut
{
    Nvec3 Positions;
    Nvec2 Direction;
    float skin;
};
struct resivedData
{
    std::string x;
    std::string y;
    std::string z;

    std::string Theta;
    std::string Phi;

    std::string ID;
};

//SetConnection("tcp://localhost:5555");
//EndConnection();
NetworkOutPut NetworkConunication (const char* Input,const char* Address)
{
    printf("Connecting to the server…\n");
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, Address);
        char buffer [10];
        printf ("Sending Input %d…\n");
        zmq_send (requester, Input, 5, 0);//send message
        zmq_recv (requester, buffer, 10, 0);
        printf ("Received Buffer %d\n",buffer);
    
}

char* IOformat=
"Pos: x: y: z:\n"
"Di: t: p:\n"
"TID: i:";

static resivedData reado(char* input)
{
    enum class ShaderType
    {
        NONE = -1, X = 0, Y = 1, Z =2 , T=3, P=4 , I=5
    };
    
    std::string line = input;
    std::stringstream ss[6];
    ShaderType type = ShaderType::NONE;

    while (strcmp(input, nullptr))
    {
        if (line.find("Pos:") != std::string::npos)
        {
            if (line.find("x:") != std::string::npos)
                type = ShaderType::X;
            else if (line.find("y:") != std::string::npos)
                type = ShaderType::Y;
            else if (line.find("z:") != std::string::npos)
                type = ShaderType::Z;
        }
        else if (line.find("Di:") != std::string::npos)
        {
            if (line.find("t:") != std::string::npos)
                type = ShaderType::T;
            else if (line.find("p:") != std::string::npos)
                type = ShaderType::P;
        }
        else if (line.find("TID:") != std::string::npos)
        {
            if (line.find("t:") != std::string::npos)
                type = ShaderType::I;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str(), ss[2].str(), ss[3].str(), ss[4].str(), ss[5].str() };
}
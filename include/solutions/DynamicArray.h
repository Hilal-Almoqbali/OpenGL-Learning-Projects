#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <array>

struct vec4
{
    float x,y,z,w;
};
struct vec3
{
    float x,y,z;
};
struct vec2
{
    float x,y;
};


struct vertex
{
    vec3 Positions;
    vec4 Color;
    vec2 TexCoord;
    float TexID;
};

static std::array<vertex,8> CreatQuad(float x,float y,float z,float r,float g,float b,float a,float TC1,float TC2,float TexID);

vertex DynamicArray(float x,float y,float z ,float r,float g,float b,float a,float TC1,float TC2,float TexID)
{
                      
    auto q0 = CreatQuad(x,y,z,r,g,b,a,TC1,TC2,TexID);
    vertex vertices[8];
    memcpy(vertices,q0.data(),q0.size()*sizeof(vertex));
    return *vertices;
}


static std::array<vertex,8> CreatQuad(float x,float y,float z,float r,float g,float b,float a,float TC1,float TC2,float TexID)
        {

            vertex v0;
            v0.Positions = {x+0.5f,y-0.25f,z+0.25f};
            v0.Color = {r,g,b,a};
            v0.TexCoord ={TC1,TC2};
            v0.TexID = {TexID};

            vertex v1;
            v1.Positions = {x+0.5f,y+0.25f,z+0.25f};
            v1.Color = {r,g,b,a};
            v1.TexCoord ={TC1,TC2};
            v1.TexID = {TexID};

            vertex v2;
            v2.Positions = {x-0.5f,y+0.25f,z+0.25f};
            v2.Color = {r,g,b,a};
            v2.TexCoord ={TC1,TC2};
            v2.TexID = {TexID};

            vertex v3;
            v3.Positions = {x-0.5f,y-0.25f,z+0.25f};
            v3.Color = {r,g,b,a};
            v3.TexCoord ={TC1,TC2};
            v3.TexID = {TexID};

            vertex v4;
            v4.Positions = {x+0.5f,y-0.5f,z-0.25f};
            v4.Color = {r,g,b,a};
            v4.TexCoord ={TC1,TC2};
            v4.TexID = {TexID};

            vertex v5;
            v5.Positions = {x+0.5f,y+0.5f,z-0.25f};
            v5.Color = {r,g,b,a};
            v5.TexCoord ={TC1,TC2};
            v5.TexID = {TexID};

            vertex v6;
            v6.Positions = {x-0.5f,y+0.5f,z-0.25f};
            v6.Color = {r,g,b,a};
            v6.TexCoord ={TC1,TC2};
            v6.TexID = {TexID};

            vertex v7;
            v7.Positions = {x-0.5f,y-0.5f,z-0.25f};
            v7.Color = {r,g,b,a};
            v7.TexCoord ={TC1,TC2};
            v7.TexID = {TexID};

            return {v0,v1,v2,v3,v4,v5,v6,v7};
        }
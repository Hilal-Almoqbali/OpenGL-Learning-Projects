#include <stdio.h>
#include <iostream>

struct ShadderSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

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





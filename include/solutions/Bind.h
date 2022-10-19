#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vendor/stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "solutions/CompileShader.h"

class Shader
{
    private:
        unsigned int m_RendererID;
        std::string m_FilePath;
        std::unordered_map<std::string, int> m_UniformLocationCache;

    public:
        Shader(const std::string& filepath)
        {
            m_RendererID = CompileShadder(filepath);
        }
        ~Shader()
        {
            glDeleteProgram(m_RendererID);
        }

        void BindShader() const
        {
            glUseProgram(m_RendererID);
        }
        void UnbindShader() const
        {
            glUseProgram(0);
        }

};

class IndexBuffer
{
    private:
    unsigned int m_RendererID;
    unsigned int m_Count;
    int m_IndexID;

    public:
    IndexBuffer(const int ID, const unsigned int* data, unsigned int count)
      :m_Count(count), m_IndexID(ID)
    {
    sizeof(unsigned int) == sizeof(GLuint);
    glGenBuffers(ID, &m_RendererID) ;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    }
     

    ~IndexBuffer()
    {
        glDeleteBuffers(m_IndexID, &m_RendererID);
    }


    void Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }
    void Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    inline unsigned int GetCount() const {return m_Count;}
};

class VertexBuffer
{
    private:
        unsigned int m_RendererID;

    public:
        VertexBuffer(const void* data, unsigned int size)
        {
        glGenBuffers(1, &m_RendererID) ;
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID) ;
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW) ;
        }
        ~VertexBuffer()
        {
            glDeleteBuffers(1, &m_RendererID);
        }

        void Bind() const
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        }
        void Unbind() const
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
};

class VertexArray
{
    private:
        unsigned int m_RendererID;

    public:
        VertexArray()
        {
            glGenVertexArrays(1, &m_RendererID);
        }
        ~VertexArray()
        {
            glDeleteVertexArrays(1, &m_RendererID);
        }

        void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
        {
            Bind();
            vb.Bind();
            const std::vector <VertexBufferElement> elements = layout.GetElements();
            unsigned int offset = 0;
            for (unsigned int i = 0; i < elements.size() ; i++)
            {
                const VertexBufferElement element = elements[i];
                glEnableVertexAttribArray(i) ;
                glVertexAttribPointer(i, element.count, element.type, element.normalized,
                                              layout.GetStride(), INT2VOIDP(offset));
                offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
            }
        }
        void Bind() const
        {
            glBindVertexArray(m_RendererID);
        }
        void Unbind() const
        {
            glBindVertexArray(0);
        }



    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT         : return sizeof(GLfloat);
            case GL_UNSIGNED_INT  : return sizeof(GLuint);
            case GL_UNSIGNED_BYTE : return sizeof(GLbyte);
        }
        
        return 0;
    }
};

class VertexBufferLayout
{
    private:
        unsigned int m_Stride;
        std::vector<VertexBufferElement> m_Elements;

    public:
        VertexBufferLayout() :
            m_Stride(0) { }

        void AddFloat(unsigned int count)        { Push(GL_FLOAT, count, GL_FALSE);        }
        void AddUnsignedInt(unsigned int count)  { Push(GL_UNSIGNED_INT, count, GL_FALSE); }
        void AddUnsignedByte(unsigned int count) { Push(GL_UNSIGNED_BYTE, count, GL_TRUE); }

        inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; };
        inline unsigned int GetStride() const { return m_Stride; };

    private:
        void Push(unsigned int type, unsigned int count, unsigned char normalized)
        {
            struct VertexBufferElement vbe = {type, count, normalized};
            m_Elements.push_back(vbe);
            m_Stride += count * VertexBufferElement::GetSizeOfType(type);
        };
};



class 2DTextures
{
    private:
        unsigned int m_RendererID;
        std::string m_FilePath;
        unsigned char* m_LocalBuffer;
        int m_Width, m_Height, m_BPP;

    public:
        2DTextures(const std::string& path)
        : m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
        {
            stbi_set_flip_vertically_on_load(1);
            m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
            GLCall( glGenTextures(1, &m_RendererID) );
            GLCall( glBindTexture(GL_TEXTURE_2D, m_RendererID) ); // Bind without slot selection

            GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
            GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
            GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
            GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
        

            GLCall( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer) );
            Unbind();

            if (m_LocalBuffer)
                stbi_image_free(m_LocalBuffer);
        }
        ~2DTextures()
        {
            glDeleteTextures(1, &m_RendererID);
        }

        void Bind(unsigned int slot ) const
        {
                glActiveTexture(GL_TEXTURE0 + slot) ;
                glBindTexture(GL_TEXTURE_2D, m_RendererID) ;
        }
        void Unbind() const
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        inline int GetWidth() const { return m_Width; }
        inline int GetHeight() const { return m_Height; }
};

class renderer
{
        public:
        void Clear() const;
        void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};

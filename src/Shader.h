#pragma once

#include <fstream>
#include <unordered_map>

struct ShaderSource {
    std::string VertexShaderSource;
    std::string FragmentShaderSource;
};

class Shader {
private:
    unsigned int m_RendererID;
    std::unordered_map<std::string, unsigned int> m_UniformLocationCache;
public:
    Shader(const std::string& path);
    ~Shader();

    void Bind() const;
    static void UnBind();

    void setUniform4f(const char* name, float v1, float v2, float v3, float v4);
    void setUniform1i(const char* name, int v1);

private:
    unsigned int GetUniformLocation(const std::string& name);
    ShaderSource ParseShader(const std::string& path);
    unsigned int CompileShader(const unsigned int& type, const std::string& source);
    unsigned int CreateShader(const ShaderSource& source);
};
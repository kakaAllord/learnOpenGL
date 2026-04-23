#include "Shader.h"

#include <glad/glad.h>
#include <unordered_map>
#include <sstream>
#include <iostream>

enum class ShaderType {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
};

Shader::Shader(const std::string &path) {
    ShaderSource source = ParseShader(path);
    m_RendererID = CreateShader(source);
}

Shader::~Shader() {
    UnBind();
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const {
    glUseProgram(m_RendererID);
}

void Shader::UnBind() {
    glUseProgram(0);
}

void Shader::setUniform4f(std::string& name, float v1, float v2, float v3, float v4) {
    glUniform4f(GetUniformLocation(name), v1, v2, v3, v4);
}

unsigned int Shader::GetUniformLocation(const std::string& name) {
    unsigned int UniformLocation;
    if (m_UniformLocationCache.contains(name)) {
        UniformLocation = m_UniformLocationCache[name];
        return UniformLocation;
    }
    UniformLocation = glGetUniformLocation(m_RendererID, name.c_str());
    m_UniformLocationCache[name] = UniformLocation;

    return UniformLocation;
}

ShaderSource Shader::ParseShader(const std::string &path) {
    ShaderSource shader_source;
    std::ifstream file(path);
    std::stringstream ss[2];
    std::string line;
    auto type = ShaderType::NONE;

    if (!file.is_open()) {
        std::cerr << "Failed to load file at path: " << path << std::endl;
        return {"", ""};
    }

    while (std::getline(file, line)) {
        if (line.find("vertex") != std::string::npos) type = ShaderType::VERTEX;
        else if (line.find("fragment") != std::string::npos) type = ShaderType::FRAGMENT;
        else ss[static_cast<int>(type)] << line << std::endl;
    }

    shader_source.VertexShaderSource = ss[static_cast<int>(ShaderType::VERTEX)].str();
    shader_source.FragmentShaderSource = ss[static_cast<int>(ShaderType::FRAGMENT)].str();

    return shader_source;
}

unsigned int Shader::CompileShader(const unsigned int &type, const std::string &source) {

    const unsigned int id = glCreateShader(type);
    const char* shader_source = &source[0];
    glShaderSource(id, 1, &shader_source, nullptr);
    glCompileShader(id);

    //ERROR HANDLING
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
        int message_length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &message_length);

        const auto message = static_cast<char *>(alloca(message_length * sizeof(char)));
        glGetShaderInfoLog(id, message_length, nullptr, message);

        std::cerr << "Failed to compile " <<
                (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
                " : " << message << std::endl;
        glDeleteShader(id);

        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const ShaderSource& source) {

    const unsigned int ShaderProgram = glCreateProgram();
    const unsigned int VertexShaderProgram = CompileShader(GL_VERTEX_SHADER, source.VertexShaderSource);
    const unsigned int FragmentShaderProgram = CompileShader(GL_FRAGMENT_SHADER, source.FragmentShaderSource);

    glAttachShader(ShaderProgram, VertexShaderProgram);
    glAttachShader(ShaderProgram, FragmentShaderProgram);

    glLinkProgram(ShaderProgram);
    glValidateProgram(ShaderProgram);

    glDeleteShader(VertexShaderProgram);
    glDeleteShader(FragmentShaderProgram);

    return ShaderProgram;
}
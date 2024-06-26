//
// Copyright (c) 2021 Julian Hinxlage. All rights reserved.
//

#include "Shader.h"
#include "ShaderState.h"
#include "common/Log.h"
#include "util/strutil.h"
#include "GL/glew.h"
#include <fstream>
#include <filesystem>

using namespace tridot2d;

namespace tridot2d {
    
    Shader::Shader() {
        id = 0;
        state = std::make_shared<ShaderState>();
    }

    Shader::~Shader() {
        if(id != 0){
            glDeleteProgram(id);
            id = 0;
        }
    }

    void bindShader(uint32_t id){
        static uint32_t currentId = 0;
        if(id != currentId){
            glUseProgram(id);
            currentId = id;
        }
    }

    void Shader::bind() {
        bindShader(id);
    }

    void Shader::unbind() {
        bindShader(0);
    }

    uint32_t Shader::getId() {
        return id;
    }

    bool Shader::loadSourceFile(const std::string& file, std::string& source) {
        std::ifstream stream(file);
        if (!stream.is_open()) {
            Log::warning("shader: file %s not found", file.c_str());
            return false;
        }

        source.clear();
        std::string line;
        int lineNumber = 1;
        while (std::getline(stream, line, '\n')) {
            if (match(line, "#include") == 8) {
                auto parts = split(line, " ", false);
                if (parts.size() >= 2) {
                    parts = split(parts[1], "\"", false);
                    if (parts.size() >= 1) {
                        std::string str;
                        std::string includeFile = std::filesystem::path(file).parent_path().string() + "/" + parts[0];

                        if (loadSourceFile(includeFile, str)) {
                            source += str;
                            source += "\n";
                        }
                    }
                    else {
                        Log::warning("shader: error on line %i in file %s", lineNumber, file.c_str());
                    }
                }
                else {
                    Log::warning("shader: error on line %i in file %s", lineNumber, file.c_str());
                }
            }
            else {
                source += line;
                source += "\n";
            }
            lineNumber++;
        }

        return true;
    }

    bool Shader::load(const std::string& file) {
        std::string source;
        if (!loadSourceFile(file, source)) {
            return false;
        }
        this->file = file;
        return loadFromSource(source);
    }

    bool Shader::loadFromSource(const std::string& source)
    {
        sources.clear();
        for (auto& line : split(source, "\n")) {
            if (line == "#type vertex") {
                sources.push_back({ (uint32_t)GL_VERTEX_SHADER, "" });
            }
            else if (line == "#type fragment") {
                sources.push_back({ (uint32_t)GL_FRAGMENT_SHADER, "" });
            }
            else if (line == "#type geometry") {
                sources.push_back({ (uint32_t)GL_GEOMETRY_SHADER, "" });
            }
            else if (line == "#type compute") {
                sources.push_back({ (uint32_t)GL_COMPUTE_SHADER, "" });
            }
            else {
                if (!sources.empty()) {
                    sources.back().second += line;
                    sources.back().second += '\n';
                }
            }
        }
        if (sources.size() == 0) {
            return false;
        }

        std::vector<GLuint> shaderIds;
        shaderIds.reserve(sources.size());
        for(auto &source : sources){
            GLuint shaderId = glCreateShader((GLenum)source.first);
            const char *src = source.second.c_str();
            int size = source.second.size();
            glShaderSource(shaderId, 1, &src, &size);
            glCompileShader(shaderId);
            GLint isCompiled = 0;
            glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
            if(isCompiled == GL_FALSE){
                int logLength = 0;
                glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
                std::string log(logLength, '\0');
                glGetShaderInfoLog(shaderId, logLength, &logLength, log.data());
                if((GLenum)source.first == GL_VERTEX_SHADER){
                    Log::warning("vertex shader compilation:\n%s", log.c_str());
                }else if((GLenum)source.first == GL_FRAGMENT_SHADER){
                    Log::warning("fragment shader compilation:\n%s", log.c_str());
                }else if((GLenum)source.first == GL_GEOMETRY_SHADER){
                    Log::warning("geometry shader compilation:\n%s", log.c_str());
                }else if((GLenum)source.first == GL_COMPUTE_SHADER){
                    Log::warning("compute shader compilation:\n%s", log.c_str());
                }
                glDeleteShader(shaderId);
            }else{
                shaderIds.push_back(shaderId);
            }
        }

        if(sources.size() != shaderIds.size()){
            for(auto &shaderId : shaderIds){
                glDeleteShader(shaderId);
            }
            sources.clear();
            return false;
        }
        sources.clear();

        GLuint programId = glCreateProgram();
        for(auto &shaderId : shaderIds){
            glAttachShader(programId, shaderId);
        }
        glLinkProgram(programId);
        GLint isLinked = 0;
        glGetProgramiv(programId, GL_LINK_STATUS, &isLinked);
        if(isLinked == GL_FALSE){
            int logLength = 0;
            glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
            std::string log(logLength, '\0');
            glGetProgramInfoLog(programId, logLength, &logLength, log.data());
            Log::warning("shader linking:\n%s", log.c_str());

            for(auto &shaderId : shaderIds){
                glDetachShader(programId, shaderId);
                glDeleteShader(shaderId);
            }

            glDeleteProgram(programId);
            return false;
        }

        for(auto &shaderId : shaderIds){
            glDetachShader(programId, shaderId);
            glDeleteShader(shaderId);
        }
        id = programId;
        Log::trace("loaded shader %s", file.c_str());
        locations.clear();
        bufferLocations.clear();
        return true;
    }

    bool Shader::has(const std::string &uniform) {
        if(id == 0){
            return false;
        }else{
            return getLocation(uniform, false) != -1 || getBufferLocation(uniform, false) != -1;
        }
    }

    void Shader::set(const std::string &uniform, int value) {
        glUniform1i(getLocation(uniform), value);
        state->set(uniform, value);
    }

    void Shader::set(const std::string &uniform, float value) {
        glUniform1f(getLocation(uniform), value);
        state->set(uniform, value);
    }

    void Shader::set(const std::string &uniform, const glm::vec2 &value) {
        glUniform2f(getLocation(uniform), value.x, value.y);
        state->set(uniform, value);
    }

    void Shader::set(const std::string &uniform, const glm::vec3 &value) {
        glUniform3f(getLocation(uniform), value.x, value.y, value.z);
        state->set(uniform, value);
    }

    void Shader::set(const std::string &uniform, const glm::vec4 &value) {
        glUniform4f(getLocation(uniform), value.x, value.y, value.z, value.w);
        state->set(uniform, value);
    }

    void Shader::set(const std::string &uniform, const glm::mat2 &value) {
        glUniformMatrix2fv(getLocation(uniform), 1, GL_FALSE, (float*)&value);
        state->set(uniform, value);
    }

    void Shader::set(const std::string &uniform, const glm::mat3 &value) {
        glUniformMatrix3fv(getLocation(uniform), 1, GL_FALSE, (float*)&value);
        state->set(uniform, value);
    }

    void Shader::set(const std::string &uniform, const glm::mat4 &value) {
        glUniformMatrix4fv(getLocation(uniform), 1, GL_FALSE, (float*)&value);
        state->set(uniform, value);
    }

    void Shader::set(const std::string &uniform, int *values, int count) {
        glUniform1iv(getLocation(uniform), count, values);
        state->set(uniform, values, count);
    }

    void Shader::set(const std::string &uniform, float *values, int count) {
        glUniform1fv(getLocation(uniform), count, values);
        state->set(uniform, values, count);
    }

    void Shader::set(const std::string &uniform, glm::vec2 *values, int count) {
        glUniform2fv(getLocation(uniform), count, (float*)values);
        state->set(uniform, values, count);
    }

    void Shader::set(const std::string &uniform, glm::vec3 *values, int count) {
        glUniform3fv(getLocation(uniform), count, (float*)values);
        state->set(uniform, values, count);
    }

    void Shader::set(const std::string &uniform, glm::vec4 *values, int count) {
        glUniform4fv(getLocation(uniform), count, (float*)values);
        state->set(uniform, values, count);
    }

    void Shader::set(const std::string &uniform, glm::mat2 *values, int count) {
        glUniformMatrix2fv(getLocation(uniform), count, GL_FALSE, (float*)values);
        state->set(uniform, values, count);
    }

    void Shader::set(const std::string &uniform, glm::mat3 *values, int count) {
        glUniformMatrix3fv(getLocation(uniform), count, GL_FALSE, (float*)values);
        state->set(uniform, values, count);
    }

    void Shader::set(const std::string &uniform, glm::mat4 *values, int count) {
        glUniformMatrix4fv(getLocation(uniform), count, GL_FALSE, (float*)values);
        state->set(uniform, values, count);
    }

    void Shader::set(const std::string &uniform, Buffer *buffer) {
        uint32_t location = getBufferLocation(uniform);
        if (location != -1) {
            glUniformBlockBinding(id, location, location);
            glBindBufferBase(GL_UNIFORM_BUFFER, location, buffer->getId());
        }
    }

    uint32_t Shader::getLocation(const std::string &name, bool warn) {
        auto entry = locations.find(name);
        if(entry != locations.end()){
            return entry->second;
        }else{
            uint32_t location = glGetUniformLocation(id, name.c_str());
            if(location == -1 && warn){
                Log::warning("uniform %s not found in shader %s", name.c_str(), file.c_str());
            }
            locations[name] = location;
            return location;
        }
    }

    uint32_t Shader::getBufferLocation(const std::string &name, bool warn) {
        auto entry = bufferLocations.find(name);
        if(entry != bufferLocations.end()){
            return entry->second;
        }else{
            uint32_t location = glGetUniformBlockIndex(id, name.c_str());
            if(location == -1 && warn){
                Log::warning("uniform buffer %s not found in shader %s", name.c_str(), file.c_str());
            }
            bufferLocations[name] = location;
            return location;
        }
    }

}
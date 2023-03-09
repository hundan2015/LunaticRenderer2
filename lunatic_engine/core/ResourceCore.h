#pragma once
#include <iostream>
#include <sstream>
#include "RenderingCore.h"
#include "content/ShaderContent.h"
#include "fstream"
#include "glad/glad.h"
namespace lunatic_engine {

class ResourceCore {
   public:
    std::shared_ptr<RenderingCore> rendering_core;
    std::shared_ptr<lunatic_engine::ShaderContent> GetShaderContent(
        const std::string vertex_shader_dir,
        const std::string fragement_shader_dir) {
        std::ifstream vertex_shader_file;
        vertex_shader_file.open(vertex_shader_dir);
        std::string vertex_shader_source;
        std::stringstream vertex_shader_source_buffer;
        vertex_shader_source_buffer << vertex_shader_file.rdbuf();
        vertex_shader_source = vertex_shader_source_buffer.str();
        vertex_shader_file.close();

        std::ifstream fragment_shader_file;
        fragment_shader_file.open(fragement_shader_dir);
        std::string fragment_shader_source;
        std::stringstream fragment_shader_source_buffer;
        fragment_shader_source_buffer << fragment_shader_file.rdbuf();
        fragment_shader_source = fragment_shader_source_buffer.str();
        fragment_shader_file.close();

        std::shared_ptr<ShaderContent> shader_content =
            std::make_shared<ShaderContent>();
        bool isOK = false;
        unsigned int shaderProgram;
        auto create_shader_source = [=, &isOK, &shaderProgram]() mutable {
            unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
            const char* shit1 = vertex_shader_source.c_str();
            const char* shit2 = fragment_shader_source.c_str();
            glShaderSource(vertexShader, 1, &shit1, nullptr);
            glCompileShader(vertexShader);
            checkCompileErrors(vertexShader, "VERTEX");

            unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &shit2, nullptr);
            glCompileShader(fragmentShader);
            checkCompileErrors(fragmentShader, "FRAGMENT");

            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            // isOK like a future. Until isOK, the function wouldn't return a
            // content.
            isOK = true;
        };
        rendering_core->InsertResoureCommandGroup(create_shader_source);
        // Kind of barrier.
        while (!isOK) {
        }
        shader_content->shader_program = shaderProgram;
        return shader_content;
    }
    void checkCompileErrors(GLuint shader, std::string type) {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout
                    << "ERROR::SHADER_COMPILATION_ERROR of type: " << type
                    << "\n"
                    << infoLog
                    << "\n -- "
                       "--------------------------------------------------- -- "
                    << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout
                    << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                    << infoLog
                    << "\n -- "
                       "--------------------------------------------------- -- "
                    << std::endl;
            }
        }
    }
};

}  // namespace lunatic_engine

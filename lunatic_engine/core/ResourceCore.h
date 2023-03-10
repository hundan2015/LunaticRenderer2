#pragma once
#include <iostream>
#include <sstream>
#include "AssimpLoader.h"
#include "RenderingCore.h"
#include "content/MeshContent.h"
#include "content/ShaderContent.h"
#include "fstream"
#include "glad/glad.h"
namespace lunatic_engine {

class ResourceCore {
   public:
    std::shared_ptr<RenderingCore> rendering_core;
    /**
     *
     * @param vertex_shader_dir Vertex shader's dir.
     * @param fragment_shader_dir Fragment shader's dir.
     * @return The shader content created by core.
     * @brief Get shader context from the shader dir.
     */
    [[nodiscard]] std::shared_ptr<lunatic_engine::ShaderContent>
    GetShaderContent(const std::string& vertex_shader_dir,
                     const std::string& fragment_shader_dir) const {
        std::string vertex_shader_source;
        vertex_shader_source = GetShaderFileString(vertex_shader_dir);

        std::string fragment_shader_source;
        fragment_shader_source = GetShaderFileString(fragment_shader_dir);

        std::shared_ptr<ShaderContent> shader_content =
            std::make_shared<ShaderContent>();
        bool is_good = false;

        unsigned int shader_program;
        auto create_shader_source = [=, &is_good, &shader_program]() mutable {
            unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
            const char* shit1 = vertex_shader_source.c_str();
            const char* shit2 = fragment_shader_source.c_str();
            glShaderSource(vertex_shader, 1, &shit1, nullptr);
            glCompileShader(vertex_shader);
            CheckCompileErrors(vertex_shader, "VERTEX");

            unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment_shader, 1, &shit2, nullptr);
            glCompileShader(fragment_shader);
            CheckCompileErrors(fragment_shader, "FRAGMENT");

            shader_program = glCreateProgram();
            glAttachShader(shader_program, vertex_shader);
            glAttachShader(shader_program, fragment_shader);
            glLinkProgram(shader_program);

            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);
            // is_good like a future. Until is_good, the function wouldn't
            // return a content.
            is_good = true;
        };
        rendering_core->InsertResoureCommandGroup(create_shader_source);
        // Kind of barrier.
        while (!is_good) {
        }
        shader_content->shader_program = shader_program;
        return shader_content;
    }
    static std::string GetShaderFileString(const std::string& shader_dir) {
        std::ifstream shader_file;
        std::string res;
        shader_file.open(shader_dir);
        std::stringstream shader_source_buffer;
        shader_source_buffer << shader_file.rdbuf();
        res = shader_source_buffer.str();
        shader_file.close();
        return std::move(res);
    }
    /**
     *
     * @param shader The shader's handle. Including vertex shader and fragment
     * shader.
     * @param type A enum including {'PROGRAM','VERTEX','FRAGMENT'}
     * @brief Checking the errors of shader, and print them.
     */
    static void CheckCompileErrors(GLuint shader, const std::string& type) {
        GLint success;
        GLchar info_log[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, info_log);
                std::cout
                    << "ERROR::SHADER_COMPILATION_ERROR of type: " << type
                    << "\n"
                    << info_log
                    << "\n -- "
                       "--------------------------------------------------- -- "
                    << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, info_log);
                std::cout
                    << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                    << info_log
                    << "\n -- "
                       "--------------------------------------------------- -- "
                    << std::endl;
            }
        }
    }

    MeshContent GetMeshContent(model_loader::Mesh mesh) const {
        bool is_good = false;
        unsigned int vao;
        unsigned int vbo;
        unsigned int ebo;
        const auto vertices_count = mesh.vertices.size();
        std::vector<float> vertices_data;
        // The VBO data is just like this:
        //{position, normal, texcoord}
        for (auto i = 0; i < vertices_count; ++i) {
            vertices_data.emplace_back(mesh.vertices[i].x);
            vertices_data.emplace_back(mesh.vertices[i].y);
            vertices_data.emplace_back(mesh.vertices[i].z);
            vertices_data.emplace_back(mesh.normals[i].x);
            vertices_data.emplace_back(mesh.normals[i].y);
            vertices_data.emplace_back(mesh.normals[i].z);
            vertices_data.emplace_back(mesh.uvs[i].x);
            vertices_data.emplace_back(mesh.uvs[i].y);
        }

        unsigned int* indices = mesh.ebo_s.data();
        // unsigned int indices[] = {0, 1, 2};
        auto draw_mode = GL_STATIC_DRAW;
        auto get_mesh_vao_command = [&]() {
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            glGenBuffers(1, &ebo);
            // bind the Vertex Array Object first, then bind and set vertex
            // buffer(s), and then configure vertex attributes(s).
            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices_data.size(),
                         vertices_data.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         sizeof(float) * mesh.ebo_s.size(), indices, draw_mode);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                                  nullptr);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(
                1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                reinterpret_cast<const void*>(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(
                2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                reinterpret_cast<const void*>(6 * sizeof(float)));
            glEnableVertexAttribArray(2);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            is_good = true;
        };
        // The lambda is a kind of resource command, which must be pushed into
        // the rendering loop to interact with GLAD.
        rendering_core->InsertResoureCommandGroup(get_mesh_vao_command);
        // A kind of barrier, like future.
        while (!is_good)
            ;
        MeshContent res{};
        res.triangle_count = mesh.triangle_count;
        res.vao = vao;
        return res;
    }
};

}  // namespace lunatic_engine

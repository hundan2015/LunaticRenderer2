//
// Created by Symbolic on 2023/3/18.
//
// If we have lnk2005, try to make def into the cpp file, not the header file.
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "ResourceCore.h"
#include <atomic>
#include <thread>
#include "stb_image.h"
std::shared_ptr<lunatic_engine::ShaderContent>
lunatic_engine::ResourceCore::GetShaderContent(
    const std::string& vertex_shader_dir,
    const std::string& fragment_shader_dir) {
    std::string vertex_shader_source;
    std::string shader_id = vertex_shader_dir + "||" + fragment_shader_dir;
    {
        std::lock_guard lock_guard(shader_content_map_mutex_);
        auto result = shader_content_map_.find(shader_id);
        if (result != shader_content_map_.end()) {
            return result->second;
        }
    }
    vertex_shader_source = GetShaderFileString(vertex_shader_dir);

    std::string fragment_shader_source;
    fragment_shader_source = GetShaderFileString(fragment_shader_dir);

    std::shared_ptr<ShaderContent> shader_content =
        std::make_shared<ShaderContent>();
    std::atomic<bool> is_good = false;

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
        std::cout << "Shader Program is" << shader_program << std::endl;

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
        // TODO:Here should have a lock. Kind of PV operation. If we don't do
        // so, the content would be get earlier than it has been spawned.
        // std::this_thread::yield();
    }
    shader_content->shader_program = shader_program;
    {
        std::lock_guard lock_guard(shader_content_map_mutex_);
        shader_content_map_.insert(std::make_pair(shader_id, shader_content));
    }
    return shader_content;
}
std::string lunatic_engine::ResourceCore::GetShaderFileString(
    const std::string& shader_dir) {
    std::ifstream shader_file;
    std::string res;
    shader_file.open(shader_dir);
    std::stringstream shader_source_buffer;
    shader_source_buffer << shader_file.rdbuf();
    res = shader_source_buffer.str();
    shader_file.close();
    return std::move(res);
}
void lunatic_engine::ResourceCore::CheckCompileErrors(GLuint shader,
                                                      const std::string& type) {
    GLint success;
    GLchar info_log[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, info_log);
            std::cout
                << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
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
lunatic_engine::MeshContent lunatic_engine::ResourceCore::GetMeshContent(
    lunatic_engine::model_loader::Mesh mesh) {
    std::atomic<bool> is_good = false;
    // bool is_good = false;
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
    // TODO: Add some options to make Mesh support skeleton.
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
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh.ebo_s.size(),
                     indices, draw_mode);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        std::cout << "Created VAO:" << vao << std::endl;
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
    std::cout << "Having VAO:" << vao << std::endl;
    res.vao = vao;
    return res;
}
lunatic_engine::ImageContent lunatic_engine::ResourceCore::GetImageContent(
    const std::string& image_dir) {
    // The brace is necessary! To use the lock guard auto free the lock!
    {
        std::lock_guard lock_guard(image_content_map_mutex_);
        // If it has been loaded once, we should find it in the map.
        auto search_result = image_content_map_.find(image_dir);

        if (search_result != image_content_map_.end()) {
            return *(search_result->second);
        }
    }

    // The content's first load.
    unsigned int texture;
    std::atomic<bool> is_texture_ok = false;
    auto get_texture_content = [=, &texture, &is_texture_ok]() {
        int width;
        int height;
        int nr_channel;
        unsigned char* data = nullptr;
        const char* directory_str = image_dir.c_str();
        data = stbi_load(directory_str, &width, &height, &nr_channel, 0);
        assert(data != nullptr);
        if (data == nullptr) {
            std::cout << "The texture failed ot load!\n";
        } else {
            std::cout << "Loaded texture." << std::endl;
        }
        // Final I found this fucking mistake! In the previous version
        // it can't pass the value to the texture. So the texture is
        // fucking black!
        auto texture_plus = texture;
        // Don't try to get texture's address!
        glGenTextures(1, &texture_plus);
        std::cout << "Texture id" << texture_plus << std::endl;
        glBindTexture(GL_TEXTURE_2D, texture_plus);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);

        // glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        texture = texture_plus;
        is_texture_ok = true;
    };
    rendering_core->InsertResoureCommandGroup(get_texture_content);
    while (!is_texture_ok)
        ;
    ImageContent result(texture);
    std::shared_ptr<ImageContent> image_content_ptr =
        std::make_shared<ImageContent>(result);

    image_content_map_mutex_.lock();
    image_content_map_.insert(std::make_pair(image_dir, image_content_ptr));
    image_content_map_mutex_.unlock();
    return result;
}

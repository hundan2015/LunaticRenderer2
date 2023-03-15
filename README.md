# Lunatic Rendering Engine 2

LRE2 是一个简单的玩具渲染引擎。主要用来测试逻辑与渲染分离的OpenGL架构。
## Used Vcpkg packages
* glad
* glfw
* Imgui
* nlohmann_json
* assimp
* fmt
* stb_image

## TODO
* 修改cmakelists文件，解决linux平台中组件依赖的问题
* 修改cmakelists文件，使得assets相对路径化
* 替换掉CPP20的特性，包括format->fmt，barrier更换为cpp17的替代实现
* 去除MSVC库，增强跨平台性
* 整理目前所有系统完成状态，做好代码功能跟踪
```cmake
find_package(fmt CONFIG REQUIRED)
target_link_libraries(main PRIVATE fmt::fmt-header-only)
```

## 未来想要加入的功能

- [ ] PBR渲染管线
- [ ] ECS系统
- [ ] 文件管理系统
- [ ] 序列化
- [ ] 运行时编辑器
- [ ] 运行时控制台

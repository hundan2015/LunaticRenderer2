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

## 未来想要加入的功能

- [x] PBR渲染管线
- [x] ECS系统
- [x] 文件管理系统
- [x] 序列化
- [ ] 运行时编辑器
- [ ] 运行时控制台

## 关于这个项目的结束

这个项目最后还是决定放弃维护。这里已经写了很多的代码，无论是自己完成的ECS实现也好，还是渲染与逻辑分离的架构也好
这个项目至少也是初步实现了。但是由于水平问题，这个项目中还是有很多稚嫩的地方。而且
这个项目完全没有经过任何软件工程，很多地方都没有进行详细的设计，导致很多地方修改起来非常麻烦。
这也是这个项目结束的主要原因。不过这个项目也是我这个阶段的写照，至少对于这个项目我已经拼尽了全力，
用上了几乎所有学过的技术，当然它也让我学到了了很多。我相信未来的我一定会挑战LunaticEngine3，用更加规范的接口设计
更加良好的设计习惯以及更多的cpp技巧来完成它。希望未来的我能加油。
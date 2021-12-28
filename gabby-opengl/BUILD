cc_library(
	name = "Window",
	srcs = ["Window.h"],
	hdrs = ["Window.h"],
	deps = [
		"@GLFW//:GLFW",
		"//third_party/glad",
	],
)

cc_library(
	name = "Shader",
	srcs = ["Shader.cpp"],
	hdrs = ["Shader.h"],
	deps = [
		"@GLFW//:GLFW",
                "//third_party/glm",
		"//third_party/glad",
	],
)

cc_library(
	name = "Camera",
	srcs = ["Camera.h"],
	hdrs = ["Camera.h"],
	deps = [
		"@GLFW//:GLFW",
                "//third_party/glm",
		"//third_party/glad",
	],
)

cc_library(
	name = "Texture",
	srcs = ["Texture.h"],
	hdrs = ["Texture.h"],
	deps = [
		"@GLFW//:GLFW",
                "//third_party/glm",
		"//third_party/glad",
                "//third_party/stb",
	],
)

cc_library(
	name = "World",
	srcs = ["World.h"],
	hdrs = ["World.h"],
	deps = [
                ":Block",
                ":Util",
	],
)

cc_library(
	name = "State",
	srcs = ["State.h"],
	hdrs = ["State.h"],
	deps = [
                ":Window",
                ":Shader",
                ":BlockAtlas",
                ":World",
	],
)

cc_library(
	name = "BlockAtlas",
	srcs = ["BlockAtlas.h"],
	hdrs = ["BlockAtlas.h"],
	deps = [
		"@GLFW//:GLFW",
                "//third_party/glm",
		"//third_party/glad",
	],
)

cc_library(
	name = "VBO",
	srcs = ["vbo.h"],
	hdrs = ["vbo.h"],
	deps = [
		"@GLFW//:GLFW",
		"//third_party/glad",
	],
)

cc_library(
	name = "VAO",
	srcs = ["vao.h"],
	hdrs = ["vao.h"],
	deps = [
		"@GLFW//:GLFW",
		"//third_party/glad",
                ":VBO"
	],
)


cc_library(
	name = "Block",
	srcs = ["Block.h"],
	hdrs = ["Block.h"],
	deps = [
		"@GLFW//:GLFW",
		"//third_party/glad",
                "//third_party/glm",
                "//third_party/stb",
                ":Texture",
                ":VAO",
                ":VBO",
	],
)


cc_library(
	name = "Util",
	srcs = ["util.h"],
	hdrs = ["util.h"],
)

cc_binary(
	name = "opengl-app",
	srcs=["main.cpp"],
	deps = [
		"@GLFW//:GLFW",
                "//third_party/glm",
		"//third_party/glad",
                "//third_party/stb",
		":Window",
                ":Shader",
                ":Camera",
                ":VAO",
                ":VBO",
                ":Texture",
                ":BlockAtlas",
                ":World",
                ":State",
                ":Block",
                ":Util",
	],
)

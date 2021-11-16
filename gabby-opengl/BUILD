cc_library(
	name = "Window",
	srcs = ["Window.cpp"],
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
		"//third_party/glad",
	],
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
	],
)

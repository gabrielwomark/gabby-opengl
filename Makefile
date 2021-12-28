CXX = --cxxopt=-std=c++14

run:
	bazel run //gabby-opengl:opengl-app $(CXX)
build:
	bazel build //gabby-opengl:opengl-app $(CXX)
clean:
	bazel clean

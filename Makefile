NAME = scop

SRCS =	main.cpp \
		ObjectFactory.cpp \
		TetraedreFactory.cpp \
		Object.cpp \
		MeshData.cpp \
		MeshFactory.cpp \
		ModelToWorld.cpp \
		Transformation.cpp \
		BasicTransformation.cpp \
		Perspective.cpp \
		Scene.cpp \
		WorldToView.cpp \
		Bitmap.cpp \
		InputManager.cpp \
		Motion.cpp

SRCS_DIR = srcs

OBJS_DIR = .objs

INCLUDE_DIR = includes

CXXFLAGS = -Wall -Werror -Wextra -g3 -std=c++20

LDFLAGS = -lGLEW -lGLU -lGL -lglfw

CXX = clang++

include template_cpp.mk

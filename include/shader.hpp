// Copyright 2022 Canvas02 <Canvas02@protonmail.com>.
// SPDX-License-Identifier: MIT

#pragma once

#include "common/types.hpp"

#include <glad/gl.h>

#include <vector>

enum ShaderType {
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER,
	Geometry = GL_GEOMETRY_SHADER,
	TessControl = GL_TESS_CONTROL_SHADER,
	TessEvaluation = GL_TESS_EVALUATION_SHADER,
	Compute = GL_COMPUTE_SHADER
};

class Shader {
public:
	void destroy() { glDeleteShader(m_handle); }

	u32 getHandle() const { return m_handle; }

	static Shader FromShaderFile(const char* filepath, const ShaderType type);
	static Shader FromShaderText(const char* data, const ShaderType type);

	static Shader FromSpirvFile(const char* filepath, const ShaderType type);
	static Shader FromSpirvText(const std::vector<u8> data, const ShaderType type);

private:
	u32 m_handle;
	Shader(u32 handle)
		: m_handle(handle) {}

};
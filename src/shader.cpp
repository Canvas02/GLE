// Copyright 2022 Canvas02 <Canvas02@protonmail.com>.
// SPDX-License-Identifier: MIT

#include "shader.hpp"

#include <vector>
#include <sstream>
#include <fstream>

#include <spdlog/spdlog.h>

Shader Shader::FromShaderText(const char* data, const ShaderType type) {
	auto handle = glCreateShader(type);
	glShaderSource(handle, 1, &data, nullptr);
	glCompileShader(handle);

	int status;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		int info_log_length;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &info_log_length);

		std::vector<char> info_log_buf(info_log_length);
		glGetShaderInfoLog(handle, info_log_length, &info_log_length, info_log_buf.data());

		std::string info_log_str(info_log_buf.begin(), info_log_buf.end());

		spdlog::error("GL: Failed to compile");
		spdlog::error("GL: error log:\n{}", info_log_str);
	}

	return Shader(handle);
}

Shader Shader::FromShaderFile(const char* filepath, const ShaderType type) {
	std::ifstream file(filepath);
	std::stringstream buffer;
	std::string line;

	if (!file.is_open()) {
		spdlog::error("Program: Failed to read file: {}", filepath);
	}

	while(std::getline(file, line)) {
		buffer << line << '\n';
	}
	file.close();

	return FromShaderText(buffer.str().c_str(), type);
}

Shader Shader::FromSpirvText(const std::vector<u8> data, const ShaderType type) {
	auto handle = glCreateShader(type);
	glShaderBinary(1, &handle, GL_SHADER_BINARY_FORMAT_SPIR_V, data.data(), data.size());
	glSpecializeShader(handle, "main", 0, nullptr, nullptr);

	int status;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		int info_log_length;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &info_log_length);

		std::vector<char> info_log_buf(info_log_length);
		glGetShaderInfoLog(handle, info_log_length, &info_log_length, info_log_buf.data());

		std::string info_log_str(info_log_buf.begin(), info_log_buf.end());

		spdlog::error("GL: Failed to compile");
		spdlog::error("GL: error log:\n{}", info_log_str);
	}

	return Shader(handle);
}
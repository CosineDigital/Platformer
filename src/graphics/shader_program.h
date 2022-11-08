#ifndef SHADER_PROGRAM_H_
#define SHADER_PROGRAM_H_

#include <string>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include "shader.h"

class ShaderProgram
{
public:
    ShaderProgram() = default;

    // @constructor a regular shader program
    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    // @constructor a shader program with a geometry shader
    ShaderProgram(const char* vertexPath, const char* fragmentPath, const char* geometryPath);


    void use(void) const;
    const unsigned int& getID(void) const noexcept;

    void setBool (const char* uniformName, bool  value) const;
    void setInt  (const char* uniformName, int   value) const;
    void setFloat(const char* uniformName, float value) const;

    void setVec2(const char* uniformName, const glm::vec2& value)             const;
    void setVec2(const char* uniformName, float x, float y)                   const;
    void setVec3(const char* uniformName, const glm::vec3& value)             const;
    void setVec3(const char* uniformName, float x, float y, float z)          const;
    void setVec4(const char* uniformName, const glm::vec4 &value)             const;
    void setVec4(const char* uniformName, float x, float y, float z, float w) const;

    void setMat2(const char* uniformName, const glm::mat2 &mat) const;
    void setMat3(const char* uniformName, const glm::mat3 &mat) const;
    void setMat4(const char* uniformName, const glm::mat4 &mat) const;

private:
    mutable std::unordered_map<const char*, int> uniformLocations;
    int getUniformLocation(const char* uniformName) const;
public:
    unsigned int programID{ 0 };
};

#endif // SHADER_PROGRAM_H_

#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/ext/matrix_transform.hpp>


struct Transform
{
	glm::vec3 mPosition = { 0.0f, 0.0f, 0.0f };
	// in degrees - should start with no rotation unless specified
	glm::vec3 mRotation = { 0.0f, 0.0f, 0.0f };
	// in units - should be one to start of with
	glm::vec3 mScale = { 1.0f, 1.0f, 1.0f };

	/*Instance data to be sent to Model pointer mModelType*/
	glm::mat4 getViewMatrix(void) const
	{
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), mPosition);

		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0f), mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0f), mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), mScale);

		glm::mat4 matrix = translation * rotation * scale;
		return matrix;
	}

	glm::vec2 getPosition2D(void) const
	{
		return { mPosition.x, mPosition.y };
	}

	Transform(glm::vec2 pos)
	{
		mPosition = glm::vec3{ pos.x, pos.y, 0.0f };
		mRotation = { 0.0f, 0.0f, 0.0f };
		mScale = { 1.0f, 1.0f, 1.0f };
	}

	Transform(Transform&) = default;
	Transform& operator=(Transform&) = default;

	Transform() {}

	Transform(Transform&& other) noexcept
		: mPosition(std::move(other.mPosition)),
		mRotation(std::move(other.mRotation)),
		mScale(std::move(other.mScale)) {}

	Transform& operator=(Transform&& other) noexcept
	{
		mPosition = std::move(other.mPosition);
		mRotation = std::move(other.mRotation);
		mScale = std::move(other.mScale);

		return *this;
	}
};

#endif // TRANSFORM_H_

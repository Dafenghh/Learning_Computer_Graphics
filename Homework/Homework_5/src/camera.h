#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
using namespace std;
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
private: 
	// Camera Attributes
	glm::vec3 position_;
	glm::vec3 front_;
	glm::vec3 up_;
	glm::vec3 right_;
	glm::vec3 world_up_;
	// Euler Angles
	float yaw_;
	float pitch_;
	// Camera options
	float movement_speed_;
	float mouse_sensitivity_;
	float zoom_;

public:


	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		   glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
		   float yaw = YAW, 
		   float pitch = PITCH) : 
			front_(glm::vec3(0.0f, 0.0f, -1.0f)), 
			movement_speed_(SPEED), 
			mouse_sensitivity_(SENSITIVITY), 
			zoom_(ZOOM) {
		position_ = position;
		world_up_ = up;
		yaw_ = yaw;
		pitch_ = pitch;
		UpdateCameraVectors();
	}
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, 
		   float upX, float upY, float upZ, 
		   float yaw, float pitch) : 
			front_(glm::vec3(0.0f, 0.0f, -1.0f)), 
			movement_speed_(SPEED), 
			mouse_sensitivity_(SENSITIVITY), 
			zoom_(ZOOM)	{
		position_ = glm::vec3(posX, posY, posZ);
		world_up_ = glm::vec3(upX, upY, upZ);
		yaw_ = yaw;
		pitch_ = pitch;
		UpdateCameraVectors();
	}

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(position_, position_ + front_, up_);
	}
	float zoom() {
		return zoom_;
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
		float velocity = movement_speed_ * deltaTime;
		if (direction == FORWARD)
			position_ += front_ * velocity;
		if (direction == BACKWARD)
			position_ -= front_ * velocity;
		if (direction == LEFT)
			position_ -= right_ * velocity;
		if (direction == RIGHT)
			position_ += right_ * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
		xoffset *= mouse_sensitivity_;
		yoffset *= mouse_sensitivity_;

		yaw_ += xoffset;
		pitch_ += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (pitch_ > 89.0f)
				pitch_ = 89.0f;
			if (pitch_ < -89.0f)
				pitch_ = -89.0f;
		}

		// Update front_, right_ and up_ Vectors using the updated Euler angles
		UpdateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		if (zoom_ >= 1.0f && zoom_ <= 45.0f)
			zoom_ -= yoffset;
		if (zoom_ <= 1.0f)
			zoom_ = 1.0f;
		if (zoom_ >= 45.0f)
			zoom_ = 45.0f;
	}

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void UpdateCameraVectors()
	{
		// Calculate the new front_ vector
		glm::vec3 front;
		front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
		front.y = sin(glm::radians(pitch_));
		front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
		front_ = glm::normalize(front);
		// Also re-calculate the right_ and up_ vector
		right_ = glm::normalize(glm::cross(front_, world_up_));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		up_ = glm::normalize(glm::cross(right_, front_));
	}
};
#endif
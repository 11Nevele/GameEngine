#pragma once
#define GLM_ENABLE_EXPERIMENTAL  
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
namespace ac
{
	/**
	 * @brief Represents a transformation in 3D space.
	 * 
	 * The Transform class encapsulates position, rotation, and scale data
	 * for an object in 3D space, along with utility methods for manipulating
	 * these transformations.
	 */
	class Transform
	{
	public:
		glm::vec3 position; ///< The position in 3D space (x, y, z)
		glm::quat rotation; ///< The rotation as a quaternion
		glm::vec3 scale;    ///< The scale in 3D space (x, y, z)
		
		/**
		 * @brief Default constructor that initializes the transform.
		 * 
		 * Creates a transform with default values (position at origin,
		 * identity rotation, and unit scale).
		 */
		Transform();
		
		/**
		 * @brief Converts the transform to a 4x4 matrix.
		 * 
		 * @param invPosition If true, inverts the position component
		 * @return glm::mat4 A 4x4 transformation matrix
		 */
		glm::mat4 asMat4(bool invPosition = false) const;

		glm::vec3 getRotationAsDegrees()const;
		
		/**
		 * @brief Rotates around an arbitrary axis.
		 * 
		 * @param axis The axis of rotation (normalized vector)
		 * @param angle The angle of rotation in degrees
		 */
		void RotateAxis(glm::vec3 axis, float angle);
		
		/**
		 * @brief Rotates around the X axis.
		 * 
		 * @param angle The angle of rotation in degrees
		 */
		void RotateX(float angle);
		
		/**
		 * @brief Rotates around the Y axis.
		 * 
		 * @param angle The angle of rotation in degrees
		 */
		void RotateY(float angle);
		
		/**
		 * @brief Rotates around the Z axis.
		 * 
		 * @param angle The angle of rotation in degrees
		 */
		void RotateZ(float angle);
		
		/**
		 * @brief Sets the rotation using Euler angles.
		 * 
		 * @param eulerAngle The desired rotation in Euler angles (pitch, yaw, roll) in degrees
		 */
		void SetRotation(glm::vec3 eulerAngle);
		
		/**
		 * @brief Rotates the yaw component (around Y axis).
		 * 
		 * @param angle The angle of rotation in degrees
		 */
		void RotateYaw(float angle);
		
		/**
		 * @brief Rotates the pitch component (around X axis).
		 * 
		 * @param angle The angle of rotation in degrees
		 */
		void RotatePitch(float angle);
		
		/**
		 * @brief Rotates the roll component (around Z axis).
		 * 
		 * @param angle The angle of rotation in degrees
		 */
		void RotateRoll(float angle);
	};
}

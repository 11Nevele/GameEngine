#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define PI 3.1415926
namespace ac
{
	namespace util
	{
    
        /**
     * Creates a rotation matrix around the X axis
     * @param theta Rotation angle in degrees
     * @return X-axis rotation matrix
     */
        glm::mat4 RotationX(double theta)
        {
            theta = theta / 180 * PI;
            glm::mat4 res (
                    {1, 0, 0, 0},
                    {0, glm::cos(theta), -glm::sin(theta), 0},
                    {0, glm::sin(theta), glm::cos(theta), 0},
                    {0, 0, 0, 1}
            );
            return res;
        }

        /**
         * Creates a rotation matrix around the Y axis
         * @param theta Rotation angle in degrees
         * @return Y-axis rotation matrix
         */
        glm::mat4 RotationY(double theta)
        {
            theta = theta / 180 * PI;
            glm::mat4 res (
                    { glm::cos(theta), 0, glm::sin(theta), 0},
                    {0, 1, 0, 0},
                    {-glm::sin(theta), 0, glm::cos(theta), 0},
                    {0, 0, 0, 1}
            );
            return res;
        }

        /**
         * Creates a rotation matrix around the Z axis
         * @param theta Rotation angle in degrees
         * @return Z-axis rotation matrix
         */
        glm::mat4 RotationZ(double theta)
        {
            theta = theta / 180 * PI;
            glm::mat4 res(
                    { glm::cos(theta), -glm::sin(theta), 0, 0},
                    { glm::sin(theta), glm::cos(theta), 0, 0},
                    {0, 0, 1, 0},
                    {0, 0, 0, 1}
            );
            return res;
        }

        /**
         * Creates a translation matrix
         * @param tx X translation
         * @param ty Y translation
         * @param tz Z translation
         * @return Translation matrix
         */
        glm::mat4 Translation(glm::vec3 t)
        {
            return glm::translate(glm::mat4(1.0f), t);
        }
	}
}
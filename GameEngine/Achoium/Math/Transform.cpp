#include <acpch.h>  
#include "Transform.h"  
#include "util.h"  
#include <glm/gtc/matrix_transform.hpp>  
#define GLM_ENABLE_EXPERIMENTAL  
#include <glm/gtx/quaternion.hpp>  

namespace ac  
{  
   /// Converts the transform into a 4x4 matrix representation.  
   /// @param invPosition If true, the position is inverted in the transformation.  
   /// @return A 4x4 matrix representing the transform.  
   glm::mat4 Transform::asMat4(bool invPosition) const
   {  
       glm::mat4 transform = glm::mat4(1.0f);  

       if (!invPosition)  
       {  
           transform = glm::translate(transform, position);  
           transform *= glm::toMat4(rotation);  
           transform = glm::scale(transform, scale);  
       }  
       else  
       {  
           transform = glm::translate(transform, -position);  
           transform *= glm::toMat4(glm::inverse(rotation));  
           transform = glm::scale(transform, glm::vec3(1.0f) / scale);  
       }  

       return transform;  
   }

   glm::vec3 Transform::getRotationAsDegrees() const
   {
       return glm::eulerAngles(rotation);
   }

   /// Default constructor for the Transform class.  
   /// Initializes position, rotation, and scale to their default values.  
   Transform::Transform() : position(0, 0, 0), rotation(1.0f, 0.0f, 0.0f, 0.0f), scale(1, 1, 1)  
   {  
   }  

   /// Rotates the transform around a specified axis by a given angle.  
   /// @param axis The axis of rotation.  
   /// @param angle The angle of rotation in degrees.  
   void Transform::RotateAxis(glm::vec3 axis, float angle)  
   {  
       glm::quat q = glm::angleAxis(glm::radians(angle), glm::normalize(axis));  
       rotation = q * rotation;  
   }  

   /// Rotates the transform around the X-axis by a given angle.  
   /// @param angle The angle of rotation in degrees.  
   void Transform::RotateX(float angle)  
   {  
       RotateAxis(glm::vec3(1.0f, 0.0f, 0.0f), angle);  
   }  

   /// Rotates the transform around the Y-axis by a given angle.  
   /// @param angle The angle of rotation in degrees.  
   void Transform::RotateY(float angle)  
   {  
       RotateAxis(glm::vec3(0.0f, 1.0f, 0.0f), angle);  
   }  

   /// Rotates the transform around the Z-axis by a given angle.  
   /// @param angle The angle of rotation in degrees.  
   void Transform::RotateZ(float angle)  
   {  
       RotateAxis(glm::vec3(0.0f, 0.0f, 1.0f), angle);  
   }  

   /// Sets the rotation of the transform using Euler angles.  
   /// @param eulerAngle The Euler angles (in degrees) to set the rotation.  
   void Transform::SetRotation(glm::vec3 eulerAngle)  
   {  
       rotation = glm::quat(glm::radians(eulerAngle));  
   }  

   /// Rotates the transform around the Y-axis (yaw) relative to the current rotation.  
   /// @param angle The angle of rotation in degrees.  
   void Transform::RotateYaw(float angle)  
   {  
       glm::quat yawQuat = glm::angleAxis(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));  
       rotation = yawQuat * rotation;  
   }  

   /// Rotates the transform around the X-axis (pitch) relative to the current rotation.  
   /// @param angle The angle of rotation in degrees.  
   void Transform::RotatePitch(float angle)  
   {  
       glm::quat pitchQuat = glm::angleAxis(glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));  
       rotation = pitchQuat * rotation;  
   }  

   /// Rotates the transform around the Z-axis (roll) relative to the current rotation.  
   /// @param angle The angle of rotation in degrees.  
   void Transform::RotateRoll(float angle)  
   {  
       glm::quat rollQuat = glm::angleAxis(glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));  
       rotation = rollQuat * rotation;  
   }  
}
#include "acpch.h"
#include "RectCollider2D.h"
#include "CircleCollider2D.h"
#include "PolygonCollider2D.h"
#include "Math/Transform.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <numeric>
namespace ac
{
    RectCollider2D::RectCollider2D()
        : Collider2D()
        , halfSize(0.5f, 0.5f)
    {
    }
    
    RectCollider2D::RectCollider2D(float width, float height)
        : Collider2D()
        , halfSize(width * 0.5f, height * 0.5f)
    {
    }
    
    RectCollider2D::RectCollider2D(float width, float height, const glm::vec2& _offset, uint32_t _layer, bool _isTrigger)
        : Collider2D(_layer)
        , halfSize(width * 0.5f, height * 0.5f)
    {
        offset = _offset;
        isTrigger = _isTrigger;
    }
    
    std::vector<glm::vec2> RectCollider2D::GetLocalVertices() const
    {
        std::vector<glm::vec2> vertices(4);
        
        // Generate the vertices in counter-clockwise order
        vertices[0] = glm::vec2(-halfSize.x, -halfSize.y); // Bottom-left
        vertices[1] = glm::vec2(halfSize.x, -halfSize.y);  // Bottom-right
        vertices[2] = glm::vec2(halfSize.x, halfSize.y);   // Top-right
        vertices[3] = glm::vec2(-halfSize.x, halfSize.y);  // Top-left
        
        return vertices;
    }
    
    std::vector<glm::vec2> RectCollider2D::GetWorldVertices(const Transform& transform) const
    {
        std::vector<glm::vec2> localVertices = GetLocalVertices();
        std::vector<glm::vec2> worldVertices(4);
        
        // Create model matrix including offset
        glm::mat4 modelMatrix = transform.asMat4();
        
        // Apply offset to model matrix
        if (glm::length2(offset) > 0.0001f) {
            modelMatrix = glm::translate(modelMatrix, glm::vec3(offset,0));
        }
        
        for (size_t i = 0; i < 4; ++i)
        {
            // Apply transform to each vertex
            glm::vec4 worldPos = modelMatrix * glm::vec4(localVertices[i].x, localVertices[i].y, 0.0f, 1.0f);
            worldVertices[i] = glm::vec2(worldPos.x, worldPos.y);
        }
        
        return worldVertices;
    }
    
    bool RectCollider2D::CheckCollision(
        const Collider2D* other,
        const Transform& myTransform,
        const Transform& otherTransform,
        std::vector<CollisionPoint2D>& collisionPoints,
        glm::vec2& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Check what type of collider we're colliding with and delegate to specific function
        if (const RectCollider2D* otherRect = dynamic_cast<const RectCollider2D*>(other))
        {
            return RectVsRect(otherRect, myTransform, otherTransform, 
                collisionPoints, collisionNormal, penetrationDepth);
        }
        else if (const CircleCollider2D* otherCircle = dynamic_cast<const CircleCollider2D*>(other))
        {
            return RectVsCircle(otherCircle, myTransform, otherTransform, 
                collisionPoints, collisionNormal, penetrationDepth);
        }
        else if (const PolygonCollider2D* otherPolygon = dynamic_cast<const PolygonCollider2D*>(other))
        {
            return RectVsPolygon(otherPolygon, myTransform, otherTransform, 
                collisionPoints, collisionNormal, penetrationDepth);
        }
        
        // Unsupported collider type
        return false;
    }
    
    bool RectCollider2D::RectVsRect(
        const RectCollider2D* other,
        const Transform& myTransform,
        const Transform& otherTransform,
        std::vector<CollisionPoint2D>& collisionPoints,
        glm::vec2& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Get vertices in world space
        std::vector<glm::vec2> myVertices = GetWorldVertices(myTransform);
        std::vector<glm::vec2> otherVertices = other->GetWorldVertices(otherTransform);
        
        // Implement SAT for collision detection
        float minPenetration = std::numeric_limits<float>::max();
        glm::vec2 bestAxis;
        bool hasCollision = true;
        std::vector<glm::vec2> axes;
        
        // Get all potential axes to test (edges from both rectangles)
        // Collect axes from my edges
        for (size_t i = 0; i < myVertices.size(); ++i)
        {
            size_t j = (i + 1) % myVertices.size();
            glm::vec2 edge = myVertices[j] - myVertices[i];
            glm::vec2 axis(-edge.y, edge.x);
            float length = glm::length(axis);
            if (length < 0.0001f) continue;
            
            axes.push_back(axis / length); // Normalize
        }
        
        // Collect axes from other's edges
        for (size_t i = 0; i < otherVertices.size(); ++i)
        {
            size_t j = (i + 1) % otherVertices.size();
            glm::vec2 edge = otherVertices[j] - otherVertices[i];
            glm::vec2 axis(-edge.y, edge.x);
            float length = glm::length(axis);
            if (length < 0.0001f) continue;
            
            axes.push_back(axis / length); // Normalize
        }
        
        // Test all axes
        for (const auto& axis : axes)
        {
            // Project both shapes onto the axis
            float minA = std::numeric_limits<float>::max();
            float maxA = -std::numeric_limits<float>::max();
            float minB = std::numeric_limits<float>::max();
            float maxB = -std::numeric_limits<float>::max();
            
            // Project vertices of rectangle A
            for (const auto& vertex : myVertices)
            {
                float projection = glm::dot(axis, vertex);
                minA = std::min(minA, projection);
                maxA = std::max(maxA, projection);
            }
            
            // Project vertices of rectangle B
            for (const auto& vertex : otherVertices)
            {
                float projection = glm::dot(axis, vertex);
                minB = std::min(minB, projection);
                maxB = std::max(maxB, projection);
            }
            
            // Check for separation
            if (minA > maxB || minB > maxA)
            {
                // Separating axis found, no collision
                hasCollision = false;
                break;
            }
            
            // Calculate penetration depth
            float overlap = std::min(maxA - minB, maxB - minA);
            
            // Track minimum penetration for collision response
            if (overlap < minPenetration)
            {
                minPenetration = overlap;
                bestAxis = axis;
                
                // Ensure the normal points from A to B
                float centerA = (minA + maxA) * 0.5f;
                float centerB = (minB + maxB) * 0.5f;
                if (centerA > centerB)
                {
                    bestAxis = -bestAxis;
                }
            }
        }
        
        if (!hasCollision) return false;
        
        // Set collision normal and penetration depth
        collisionNormal = glm::vec3(bestAxis.x, bestAxis.y, 0.0f);
        penetrationDepth = minPenetration;
        
        // Find the contact points - this is what needs improvement
        
        // Get the reference and incident faces
        struct Face {
            glm::vec2 startPoint;
            glm::vec2 endPoint;
            glm::vec2 normal;
            float dot;
        };
        
		glm::vec2 collisionNormal2D = glm::vec2(collisionNormal.x, collisionNormal.y);

        Face referenceFace{};
        Face incidentFace{};
        bool flipNormal = false;
        
        // Find reference face (edge most perpendicular to collsion normal)
        float mnDot = std::numeric_limits<float>::max();;
		float mxProjectionOnNormal = -std::numeric_limits<float>::max();
		float mnProjectionOnNormal = std::numeric_limits<float>::max();
        
        // Check my faces
        for (size_t i = 0; i < myVertices.size(); i++) {
            size_t j = (i + 1) % myVertices.size();
            glm::vec2 edge = myVertices[j] - myVertices[i];
            glm::vec2 normal(-edge.y, edge.x);
			float myMxProjectionOnNormal = std::max(glm::dot(collisionNormal2D, myVertices[j]), glm::dot(collisionNormal2D, myVertices[i]));
            
            normal = glm::normalize(normal);
            float dot = abs(glm::dot(collisionNormal2D, glm::normalize(edge)));
            if(myMxProjectionOnNormal > mxProjectionOnNormal) {
                mxProjectionOnNormal = myMxProjectionOnNormal;
				referenceFace.startPoint = myVertices[i];
				referenceFace.endPoint = myVertices[j];
                referenceFace.dot = dot;
                referenceFace.normal = normal; // Use the normal as is
                mnDot = dot;

			}
            else if(abs(myMxProjectionOnNormal - mxProjectionOnNormal) < 0.0001f) {
                if (dot < mnDot)
                {
                    mxProjectionOnNormal = myMxProjectionOnNormal;
                    referenceFace.startPoint = myVertices[i];
                    referenceFace.endPoint = myVertices[j];
                    referenceFace.normal = normal; // Use the normal as is
                    referenceFace.dot = dot;
                    mnDot = dot;
                }
			}
        }
        mnDot = std::numeric_limits<float>::max();
        // Check other faces
        for (size_t i = 0; i < otherVertices.size(); i++) {
            size_t j = (i + 1) % otherVertices.size();
            glm::vec2 edge = otherVertices[j] - otherVertices[i];
            glm::vec2 normal(-edge.y, edge.x);
            normal = glm::normalize(normal);
            float myMnProjectionOnNormal = std::min(glm::dot(collisionNormal2D, otherVertices[j]), glm::dot(collisionNormal2D, otherVertices[i]));

            // Reverse the normal since we need it pointing from B to A for comparison
            //normal = -normal;
            
            float dot = abs(glm::dot(collisionNormal2D, glm::normalize(edge)));
            if (myMnProjectionOnNormal < mnProjectionOnNormal) {
                mnProjectionOnNormal = myMnProjectionOnNormal;
                incidentFace.startPoint = otherVertices[i];
                incidentFace.endPoint = otherVertices[j];
                incidentFace.normal = normal; // Use the normal as is
                incidentFace.dot = dot;
                mnDot = dot;
            }
            else if (abs(myMnProjectionOnNormal - mnProjectionOnNormal) < 0.0001f) {
                if (dot < mnDot)
                {
                    mnProjectionOnNormal = myMnProjectionOnNormal;
                    incidentFace.startPoint = otherVertices[i];
                    incidentFace.endPoint = otherVertices[j];
                    incidentFace.normal = normal; // Use the normal as is
                    incidentFace.dot = dot;
                    mnDot = dot;
                }
            }
        }
        
        if(incidentFace.dot < referenceFace.dot)
			std::swap(referenceFace, incidentFace);
        else if(abs(incidentFace.dot - referenceFace.dot) < 0.0001f) {
            // If they are equal, we can just use the one with the smaller start point
            if (glm::length2(incidentFace.startPoint - incidentFace.endPoint) >
                glm::length2(referenceFace.startPoint - referenceFace.endPoint)) {
                std::swap(referenceFace, incidentFace);
            }
		}
        // Clipping
        // Reference edge vector
        glm::vec2 refEdge = referenceFace.endPoint - referenceFace.startPoint;
        refEdge = glm::normalize(refEdge);
        
        

        // Compute reference face side normals (pointing outward)
        glm::vec2 refFaceNormal = referenceFace.normal;
        
        // Position relative to reference face start
        float offset1 = glm::dot(refFaceNormal, referenceFace.startPoint);
        float offset2 = glm::dot(refEdge, referenceFace.startPoint);
        float offset3 = glm::dot(-refEdge, referenceFace.endPoint);
        
        // Clip incident face to reference face side planes
        // Side 1
        std::vector<glm::vec2> clippedPoints;
        ClipSegmentToLine(incidentFace.startPoint, incidentFace.endPoint, refFaceNormal, offset1, clippedPoints);
        if (clippedPoints.size() > 1)
        {
            glm::vec2 v1 = clippedPoints[0], v2 = clippedPoints[1];
            clippedPoints.clear();
            ClipSegmentToLine(v1, v2, refEdge, offset2, clippedPoints);
        }
        if (clippedPoints.size() > 1)
        {
            glm::vec2 v1 = clippedPoints[0], v2 = clippedPoints[1];
            clippedPoints.clear();
            ClipSegmentToLine(v1, v2, -refEdge, offset3, clippedPoints);
        }

        
        if (clippedPoints.empty()) {
            // Use approximate method as above
            glm::vec3 myCenter(0.0f);
            for (const auto& vertex : myVertices) {
                myCenter += glm::vec3(vertex.x, vertex.y, 0.0f);
            }
            myCenter /= static_cast<float>(myVertices.size());
            
            glm::vec3 otherCenter(0.0f);
            for (const auto& vertex : otherVertices) {
                otherCenter += glm::vec3(vertex.x, vertex.y, 0.0f);
            }
            otherCenter /= static_cast<float>(otherVertices.size());
            
            glm::vec2 p = (myCenter + otherCenter) * 0.5f;
            collisionPoints.push_back({ p,p });
        } else {
			glm::vec2 averagePoint(0.0f);
            for(const auto& point : clippedPoints) {
				averagePoint += point;
			}
			averagePoint /= static_cast<float>(clippedPoints.size());
			collisionPoints.push_back({ averagePoint, averagePoint });
        }
        
        return true;
    }
    
    bool RectCollider2D::RectVsCircle(
        const CircleCollider2D* circle,
        const Transform& myTransform,
        const Transform& circleTransform,
        std::vector<CollisionPoint2D>& collisionPoints,
        glm::vec2& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Get world positions
        glm::vec2 rectCenter = GetWorldPosition(myTransform);
        glm::vec2 circleCenter = circle->GetWorldPosition(circleTransform);
        
        // Convert circle center to rectangle's local space
        // We need to invert the transform to get from world to local space
        glm::mat4 rectModelMatrix = myTransform.asMat4();
        if (glm::length2(offset) > 0.0001f) {
            rectModelMatrix = glm::translate(rectModelMatrix, glm::vec3(offset,0));
        }
        
        glm::mat4 rectWorldToLocal = glm::inverse(rectModelMatrix);
        glm::vec4 circleCenterLocal = rectWorldToLocal * glm::vec4(circleCenter, 0.0f, 1.0f);
        
        // Find the closest point on the rectangle to the circle center
        glm::vec3 closestPoint;
        closestPoint.x = glm::clamp(circleCenterLocal.x, -halfSize.x, halfSize.x);
        closestPoint.y = glm::clamp(circleCenterLocal.y, -halfSize.y, halfSize.y);
        closestPoint.z = circleCenterLocal.z; // Keep the same Z value
        
        // Calculate the distance from the closest point to the circle center
        glm::vec3 difference = glm::vec3(circleCenterLocal) - closestPoint;
        float distanceSq = glm::length2(difference);
        
        // If the squared distance is greater than the circle's radius squared, no collision
        if (distanceSq > circle->radius * circle->radius)
        {
            return false;
        }
        
        // Convert the closest point back to world space
        glm::vec4 closestPointWorld = rectModelMatrix * glm::vec4(closestPoint, 1.0f);
        
        // Calculate collision normal (from rectangle to circle)
        float distance = glm::sqrt(distanceSq);
        
        if (distance < 0.0001f)
        {
            // Circle center is inside the rectangle, need to find the shortest exit direction
            
            // Find the penetration depth along each axis
            float dRight = halfSize.x - circleCenterLocal.x;  // Distance to right edge
            float dLeft = circleCenterLocal.x + halfSize.x;   // Distance to left edge
            float dTop = halfSize.y - circleCenterLocal.y;    // Distance to top edge
            float dBottom = circleCenterLocal.y + halfSize.y; // Distance to bottom edge
            
            // Find the minimum penetration
            float minPenetration = dRight;
            glm::vec3 localNormal(1.0f, 0.0f, 0.0f); // Right direction
            
            if (dLeft < minPenetration)
            {
                minPenetration = dLeft;
                localNormal = glm::vec3(-1.0f, 0.0f, 0.0f); // Left direction
            }
            
            if (dTop < minPenetration)
            {
                minPenetration = dTop;
                localNormal = glm::vec3(0.0f, 1.0f, 0.0f); // Up direction
            }
            
            if (dBottom < minPenetration)
            {
                minPenetration = dBottom;
                localNormal = glm::vec3(0.0f, -1.0f, 0.0f); // Down direction
            }
            
            // Transform the normal from local to world space
            glm::vec4 worldNormal = glm::transpose(rectWorldToLocal) * glm::vec4(localNormal, 0.0f);
            collisionNormal = glm::normalize(glm::vec3(worldNormal));
            
            // Calculate the penetration depth (include circle radius)
            penetrationDepth = minPenetration + circle->radius;
            
            // Set collision point at circle center
            collisionPoints.push_back({ closestPointWorld, circleCenter });
        }
        else
        {
            // Normal conversion from local to world
            glm::vec3 localNormal = glm::normalize(difference);
            glm::vec4 worldNormal = glm::transpose(rectWorldToLocal) * glm::vec4(localNormal, 0.0f);
            collisionNormal = glm::normalize(glm::vec3(worldNormal));
            
            // Set collision point at the closest point on the rectangle
            collisionPoints.push_back({ closestPointWorld, circleCenter + -collisionNormal * circle->radius });
            
            // Calculate penetration depth
            penetrationDepth = circle->radius - distance;
        }
        
        return true;
    }
    
    bool RectCollider2D::RectVsPolygon(
        const PolygonCollider2D* polygon,
        const Transform& myTransform,
        const Transform& polygonTransform,
        std::vector<CollisionPoint2D>& collisionPoints,
        glm::vec2& collisionNormal,
        float& penetrationDepth
    ) const
    {
        // Convert rectangle to polygon and use polygon-polygon collision
        std::vector<glm::vec2> myVertices = GetWorldVertices(myTransform);
        std::vector<glm::vec2> polyVertices = polygon->GetWorldVertices(polygonTransform);
        
        // Generate edges for rectangle
        std::vector<glm::vec2> myEdges(myVertices.size());
        for (size_t i = 0; i < myVertices.size(); ++i)
        {
            size_t j = (i + 1) % myVertices.size();
            myEdges[i] = myVertices[j] - myVertices[i];
        }
        
        // Generate edges for polygon
        std::vector<glm::vec2> polyEdges(polyVertices.size());
        for (size_t i = 0; i < polyVertices.size(); ++i)
        {
            size_t j = (i + 1) % polyVertices.size();
            polyEdges[i] = polyVertices[j] - polyVertices[i];
        }
        
        // Implement the SAT for collision detection
        float minPenetration = std::numeric_limits<float>::max();
        glm::vec2 bestAxis;
        bool hasCollision = true;
        
        // Check rectangle's axes
        for (const auto& edge : myEdges)
        {
            // Calculate the perpendicular axis for projection
            glm::vec2 axis(-edge.y, edge.x);
            float length = glm::length(axis);
            if (length < 0.0001f) continue; // Skip degenerate edges
            
            axis = axis / length; // Normalize
            
            // Project both shapes onto the axis
            float minA = std::numeric_limits<float>::max();
            float maxA = -std::numeric_limits<float>::max();
            float minB = std::numeric_limits<float>::max();
            float maxB = -std::numeric_limits<float>::max();
            
            // Project rectangle vertices
            for (const auto& vertex : myVertices)
            {
                float projection = glm::dot(axis, vertex);
                minA = std::min(minA, projection);
                maxA = std::max(maxA, projection);
            }
            
            // Project polygon vertices
            for (const auto& vertex : polyVertices)
            {
                float projection = glm::dot(axis, vertex);
                minB = std::min(minB, projection);
                maxB = std::max(maxB, projection);
            }
            
            // Check for separation
            if (minA > maxB || minB > maxA)
            {
                // Separating axis found, no collision
                hasCollision = false;
                break;
            }
            
            // Calculate penetration depth
            float overlap = std::min(maxA - minB, maxB - minA);
            
            // Track minimum penetration for collision response
            if (overlap < minPenetration)
            {
                minPenetration = overlap;
                bestAxis = axis;
                
                // Ensure the normal points from rectangle to polygon
                float centerA = (minA + maxA) * 0.5f;
                float centerB = (minB + maxB) * 0.5f;
                if (centerA > centerB)
                {
                    bestAxis = -bestAxis;
                }
            }
        }
        
        if (!hasCollision) return false;
        
        // Check polygon's axes
        for (const auto& edge : polyEdges)
        {
            // Calculate the perpendicular axis for projection
            glm::vec2 axis(-edge.y, edge.x);
            float length = glm::length(axis);
            if (length < 0.0001f) continue; // Skip degenerate edges
            
            axis = axis / length; // Normalize
            
            // Project both shapes onto the axis
            float minA = std::numeric_limits<float>::max();
            float maxA = -std::numeric_limits<float>::max();
            float minB = std::numeric_limits<float>::max();
            float maxB = -std::numeric_limits<float>::max();
            
            // Project rectangle vertices
            for (const auto& vertex : myVertices)
            {
                float projection = glm::dot(axis, vertex);
                minA = std::min(minA, projection);
                maxA = std::max(maxA, projection);
            }
            
            // Project polygon vertices
            for (const auto& vertex : polyVertices)
            {
                float projection = glm::dot(axis, vertex);
                minB = std::min(minB, projection);
                maxB = std::max(maxB, projection);
            }
            
            // Check for separation
            if (minA > maxB || minB > maxA)
            {
                // Separating axis found, no collision
                hasCollision = false;
                break;
            }
            
            // Calculate penetration depth
            float overlap = std::min(maxA - minB, maxB - minA);
            
            // Track minimum penetration for collision response
            if (overlap < minPenetration)
            {
                minPenetration = overlap;
                bestAxis = axis;
                
                // Ensure the normal points from rectangle to polygon
                float centerA = (minA + maxA) * 0.5f;
                float centerB = (minB + maxB) * 0.5f;
                if (centerA > centerB)
                {
                    bestAxis = -bestAxis;
                }
            }
        }
        
        if (!hasCollision) return false;
        
        // If we get here, there is a collision
        collisionNormal = glm::vec3(bestAxis.x, bestAxis.y, 0.0f);
        penetrationDepth = minPenetration;
        
        // Calculate contact points
        // Use the centroid of the overlap region as an approximation
        glm::vec2 rectCenter = std::accumulate(myVertices.begin(), myVertices.end(), glm::vec2(0.0f)) / 
                               static_cast<float>(myVertices.size());
        glm::vec2 polyCenter = std::accumulate(polyVertices.begin(), polyVertices.end(), glm::vec2(0.0f)) / 
                               static_cast<float>(polyVertices.size());
        
        //collisionPoint = glm::vec3((rectCenter + polyCenter) * 0.5f, 0.0f);
        
        return true;
    }
    
    void RectCollider2D::ClipSegmentToLine(
        const glm::vec2& v1,
        const glm::vec2& v2,
        const glm::vec2& normal,
        float offset,
        std::vector<glm::vec2>& outPoints
    ) const
    {
        outPoints.clear();
        
        // Distance of vertices from the clipping line
        float distance1 = glm::dot(normal, v1) - offset;
        float distance2 = glm::dot(normal, v2) - offset;
        
        // If both points are behind the line, keep both
        if (distance1 >= 0.0f && distance2 >= 0.0f) {
            outPoints.push_back(v1);
            outPoints.push_back(v2);
            return;
        }
        
        // If only first point is behind the line, keep first and compute intersection
        if (distance1 >= 0.0f && distance2 < 0.0f) {
            outPoints.push_back(v1);
            float t = distance1 / (distance1 - distance2);
            outPoints.push_back(v1 + t * (v2 - v1));
            return;
        }
        
        // If only second point is behind the line, keep second and compute intersection
        if (distance1 < 0.0f && distance2 >= 0.0f) {
            float t = distance1 / (distance1 - distance2);
            outPoints.push_back(v2);
            outPoints.push_back(v1 + t * (v2 - v1));
            return;
        }
        
        // Both points are in front of the line, keep none
        return;
    }
}
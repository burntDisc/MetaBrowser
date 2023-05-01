
#pragma once

#include <vector>
#include <functional>
#include <glm/glm.hpp>

#include "SolidObject.h"

class MotionHandler
{
public:
	static void AddSolidObject(SolidObject* object);
private:
	static bool colliding;
	static const int maxRecursionDepth = 0;
	static glm::vec3 intersectionPoint;
	static glm::vec3 translation;
	static glm::vec3 travel;
	static glm::vec3 eTranslation;
	static glm::vec3 eTravel;

	static float nearestDistance;
	static float SignOfQuad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
	static std::vector<SolidObject*> solidObjects;

	class PLANE {
	public:
		float equation[4];
		glm::vec3 origin;
		glm::vec3 normal;
		PLANE(const glm::vec3& origin, const glm::vec3& normal);
		PLANE(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& normal);
		bool isFrontFacingTo(const glm::vec3& direction) const;
		float SignedDistanceTo(const glm::vec3& point) const;
	};


	const static glm::vec3 collisionEllipse; // ellipsoid hitbox radii

	const static glm::mat3 ellipseTransform;
	static void CheckTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& triNormal, glm::vec3& normal);

	static glm::vec3 CollideWithWorld(glm::vec3& normal, int collisionRecursionDepth = 0);

public:

	static void CheckCollision(glm::vec3& normal);
	static glm::vec3 CollideAndSlide(glm::vec3 translation, glm::vec3 velocity, double timeDelta, glm::vec3& normal);
};



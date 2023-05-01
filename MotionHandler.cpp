#include "MotionHandler.h"
#include <glm/gtx/projection.hpp>

#include <iostream> // REMOVE 

//***************************************************************
//TODO: http://www.peroxide.dk/papers/collision/collision.pdf
// **************************************************************

std::vector<SolidObject*> MotionHandler::solidObjects;
glm::vec3 MotionHandler::intersectionPoint;
glm::vec3 MotionHandler::translation;
glm::vec3 MotionHandler::travel;
glm::vec3 MotionHandler::eTranslation;
glm::vec3 MotionHandler::eTravel;
float MotionHandler::nearestDistance;
bool MotionHandler::colliding;

const glm::vec3 MotionHandler::collisionEllipse(1.0f, 1.0f, 1.0f);
const glm::mat3 MotionHandler::ellipseTransform =
{
	{1.0f / collisionEllipse.x, 0.0f, 0.0f},
	{0.0f, 1.0f / collisionEllipse.y, 0.0f},
	{0.0f, 0.0f, 1.0f / collisionEllipse.z}
};


void MotionHandler::AddSolidObject(SolidObject* object)
{
	solidObjects.push_back(object);
}

float MotionHandler::SignOfQuad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
	float quadVolume = glm::dot(glm::cross(b - a, c - a), d - a);
	return glm::sign(quadVolume);
}

//adjusts destination to avoid passing through triangles
void MotionHandler::CheckCollision(glm::vec3& normal)
{

	for (int objectIndex = 0; objectIndex < solidObjects.size(); ++objectIndex)
	{
		SolidObject* object = solidObjects[objectIndex];

		std::vector<glm::vec3> objectTrianglePositions = object->GetTriangleWorldPositions();
		std::vector<glm::vec3> objectTriangleNormals = object->GetTriangleWorldNormals();

		for (int triangleIndex = 0; triangleIndex < objectTrianglePositions.size(); triangleIndex += 3)
		{
			glm::vec3 modelVertexPositionA = ellipseTransform * objectTrianglePositions[triangleIndex + 0];
			glm::vec3 modelVertexPositionB = ellipseTransform * objectTrianglePositions[triangleIndex + 1];
			glm::vec3 modelVertexPositionC = ellipseTransform * objectTrianglePositions[triangleIndex + 2];

			glm::vec3 modelVertexNormalA = ellipseTransform * objectTriangleNormals[triangleIndex + 0];
			glm::vec3 modelVertexNormalB = ellipseTransform * objectTriangleNormals[triangleIndex + 1];
			glm::vec3 modelVertexNormalC = ellipseTransform * objectTriangleNormals[triangleIndex + 2];

			glm::vec3 triNormal = normalize((modelVertexNormalA + modelVertexNormalB + modelVertexNormalC) / 3.0f);
			CheckTriangle(modelVertexPositionA, modelVertexPositionB, modelVertexPositionC, triNormal, normal);
		}
	}
}

//----------------------------------------------------------------------------------

MotionHandler::PLANE::PLANE(const glm::vec3& origin, const glm::vec3& normal) {
	this->normal = normal;
	this->origin = origin;
	equation[0] = normal.x;
	equation[1] = normal.y;
	equation[2] = normal.z;
	equation[3] = -(normal.x * origin.x + normal.y * origin.y
		+ normal.z * origin.z);
}

// Construct from triangle:
MotionHandler::PLANE::PLANE(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& normal)
{
	this->normal = normal;
	origin = p1;
	equation[0] = normal.x;
	equation[1] = normal.y;
	equation[2] = normal.z;
	equation[3] = -(normal.x * origin.x + normal.y * origin.y
		+ normal.z * origin.z);
}

bool MotionHandler::PLANE::isFrontFacingTo(const glm::vec3& direction) const {
	double dot = glm::dot(normal, direction);
	return (dot <= 0);
}

float MotionHandler::PLANE::SignedDistanceTo(const glm::vec3& point) const {
	return glm::dot(point, normal) + equation[3];
}

typedef unsigned int uint32;
#define in(a) ((uint32&) a)

bool checkPointInTriangle(const glm::vec3& point,
	const glm::vec3& pa, const glm::vec3& pb, const glm::vec3& pc)
{
	glm::vec3 e10 = pb - pa;
	glm::vec3 e20 = pc - pa;
	float a = glm::dot(e10, e10);
	float b = glm::dot(e10, e20);
	float c = glm::dot(e20, e20);
	float ac_bb = (a * c) - (b * b);
	glm::vec3 vp(point.x - pa.x, point.y - pa.y, point.z - pa.z);
	float d = glm::dot(vp, e10);
	float e = glm::dot(vp, e20);
	float x = (d * c) - (e * b);
	float y = (e * a) - (d * b);
	float z = x + y - ac_bb;
	return ((in(z) & ~(in(x) | in(y))) & 0x80000000);
}

bool getLowestRoot(float a, float b, float c, float maxR,
	float* root) {
	// Check if a solution exists
	float determinant = b * b - 4.0f * a * c;
	// If determinant is negative it means no solutions.
	if (determinant < 0.0f) return false;
	// calculate the two roots: (if determinant == 0 then
	// x1==x2 but let’s disregard that slight optimization)
	float sqrtD = sqrt(determinant);
	float r1 = (-b - sqrtD) / (2 * a);
	float r2 = (-b + sqrtD) / (2 * a);
	// Sort so x1 <= x2
	if (r1 > r2) {
		float temp = r2;
		r2 = r1;
		r1 = temp;
	}
	// Get lowest root:
	if (r1 > 0 && r1 < maxR) {
		*root = r1;
		return true;
	}
	// It is possible that we want x2 - this can happen
	// if x1 < 0
	if (r2 > 0 && r2 < maxR) {
		*root = r2;
		return true;
	}
	// No (valid) solutions
	return false;
}

float SquaredLength(glm::vec3 vector)
{
	float length = glm::length(vector);
	return length * length;
}

void MotionHandler::CheckTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& triNormal, glm::vec3& normal)
{
	// Make the plane containing this triangle.
	PLANE trianglePlane(p1, p2, p3, triNormal);
	// Is triangle front-facing to the velocity vector?
	// We only check front-facing triangles
	// (your choice of course)
	if (trianglePlane.isFrontFacingTo(normalize(eTravel)))
	{
		// Get interval of plane intersection:
		float t0, t1;
		bool embeddedInPlane = false;
		// Calculate the signed distance from sphere
		// position to triangle plane
		float signedDistToTrianglePlane =
			trianglePlane.SignedDistanceTo(eTranslation);
		// cache this as we’re going to use it a few times below:
		float normalDotVelocity =
			glm::dot(trianglePlane.normal, eTravel);

		// if sphere is travelling parrallel to the plane:
		if (normalDotVelocity == 0.0f) {
			if (fabs(signedDistToTrianglePlane) >= 1.0f) {
				// Sphere is not embedded in plane.
				// No collision possible:
				return;
			}
			else {
				// sphere is embedded in plane.
				// It intersects in the whole range [0..1]
				embeddedInPlane = true;
				t0 = 0.0f;
				t1 = 1.0f;
			}
		}
		else {
			// N dot D is not 0. Calculate intersection interval:
			t0 = (-1.0f - signedDistToTrianglePlane) / normalDotVelocity;
			t1 = (1.0f - signedDistToTrianglePlane) / normalDotVelocity;
			// Swap so t0 < t1
			if (t0 > t1) {
				float temp = t1;
				t1 = t0;
				t0 = temp;
			}
			// Check that at least one result is within range:
			if (t0 > 1.0f || t1 < 0.0f) {
				// Both t values are outside values [0,1]
				// No collision possible:
				return;
			}
			// Clamp to [0,1]
			if (t0 < 0.0f) t0 = 0.0f;
			if (t1 < 0.0f) t1 = 0.0f;
			if (t0 > 1.0f) t0 = 1.0f;
			if (t1 > 1.0f) t1 = 1.0f;
		}
		// OK, at this point we have two time values t0 and t1
		// between which the swept sphere intersects with the
		// triangle plane. If any collision is to occur it must
		// happen within this interval.
		glm::vec3 collisionPoint;
		bool foundCollison = false;
		float t = 1.0;
		// First we check for the easy case - collision inside
		// the triangle. If this happens it must be at time t0
		// as this is when the sphere rests on the front side
		// of the triangle plane. Note, this can only happen if
		// the sphere is not embedded in the triangle plane.
		if (!embeddedInPlane) {
			glm::vec3 planeIntersectionPoint =
				(eTranslation - trianglePlane.normal)
				+ t0 * eTravel;
			if (checkPointInTriangle(planeIntersectionPoint,
				p1, p2, p3))
			{
				foundCollison = true;
				t = t0;
				collisionPoint = planeIntersectionPoint;
			}
		}
		// if we haven’t found a collision already we’ll have to
		// sweep sphere against points and edges of the triangle.
		// Note: A collision inside the triangle (the check above)
		// will always happen before a vertex or edge collision!
		// This is why we can skip the swept test if the above
		// gives a collision!
		if (foundCollison == false) {
			// some commonly used terms:
			float travelSquaredLength = SquaredLength(eTravel);
			float a, b, c; // Params for equation
			float newT;
			// For each vertex or edge a quadratic equation have to
			// be solved. We parameterize this equation as
			// a*t^2 + b*t + c = 0 and below we calculate the
			// parameters a,b and c for each test.
			// Check against points:
			a = travelSquaredLength;
			// P1
			b = 2.0f * glm::dot(eTravel, eTranslation - p1);
			c = SquaredLength(p1 - eTranslation) - 1.0f;
			if (getLowestRoot(a, b, c, t, &newT)) {
				t = newT;
				foundCollison = true;
				collisionPoint = p1;
			}
			// P2
			b = 2.0f * glm::dot(eTravel, eTranslation - p2);
			c = SquaredLength(p2 - eTranslation) - 1.0f;
			if (getLowestRoot(a, b, c, t, &newT)) {
				t = newT;
				foundCollison = true;
				collisionPoint = p2;
			}
			// P3
			b = 2.0f * glm::dot(eTravel, eTranslation - p3);
			c = SquaredLength(p3 - eTranslation) - 1.0f;
			if (getLowestRoot(a, b, c, t, &newT)) {
				t = newT;
				foundCollison = true;
				collisionPoint = p3;
			}
			// Check agains edges:
			// p1 -> p2:
			glm::vec3 edge = p2 - p1;
			glm::vec3 baseToVertex = p1 - eTranslation;
			float edgeSquaredLength = SquaredLength(edge);
			float edgeDotVelocity = glm::dot(edge, eTravel);
			float edgeDotBaseToVertex = glm::dot(edge, baseToVertex);
			// Calculate parameters for equation
			a = edgeSquaredLength * -travelSquaredLength +
				edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2.0f * glm::dot(eTravel, baseToVertex)) -
				2.0 * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength * (1.0f - SquaredLength(baseToVertex)) +
				edgeDotBaseToVertex * edgeDotBaseToVertex;
			// Does the swept sphere collide against infinite edge?
			if (getLowestRoot(a, b, c, t, &newT)) {
				// Check if intersection is within line segment:
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) /
					edgeSquaredLength;
				if (f >= 0.0f && f <= 1.0f) {
					// intersection took place within segment.
					t = newT;
					foundCollison = true;
					collisionPoint = p1 + f * edge;
				}
			}
			// p2 -> p3:
			edge = p3 - p2;
			baseToVertex = p2 - eTranslation;
			edgeSquaredLength = SquaredLength(edge);
			edgeDotVelocity = glm::dot(edge, eTravel);
			edgeDotBaseToVertex = glm::dot(edge, baseToVertex);
			a = edgeSquaredLength * -travelSquaredLength +
				edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2.0f * glm::dot(eTravel, baseToVertex)) -
				2.0 * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength * (1 - SquaredLength(baseToVertex)) +
				edgeDotBaseToVertex * edgeDotBaseToVertex;
			if (getLowestRoot(a, b, c, t, &newT)) {
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) /
					edgeSquaredLength;
				if (f >= 0.0 && f <= 1.0) {
					t = newT;
					foundCollison = true;
					collisionPoint = p2 + f * edge;
				}
			}
			// p3 -> p1:
			edge = p1 - p3;
			baseToVertex = p3 - eTranslation;
			edgeSquaredLength = SquaredLength(edge);
			edgeDotVelocity = glm::dot(edge, eTravel);
			edgeDotBaseToVertex = glm::dot(edge, baseToVertex);
			a = edgeSquaredLength * -travelSquaredLength +
				edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2.0f * glm::dot(eTravel, baseToVertex)) -
				2.0 * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength * (1 - SquaredLength(baseToVertex)) +
				edgeDotBaseToVertex * edgeDotBaseToVertex;
			if (getLowestRoot(a, b, c, t, &newT)) {
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) /
					edgeSquaredLength;
				if (f >= 0.0 && f <= 1.0) {
					t = newT;
					foundCollison = true;
					collisionPoint = p3 + f * edge;
				}
			}
		}
		// Set result:
		if (foundCollison == true) {
			// distance to collision: ’t’ is time of collision
			float distToCollision = t * (eTravel - eTranslation).length();
			// Does this triangle qualify for the closest hit?
			// it does if it’s the first hit or the closest
			if (!colliding ||
				distToCollision < nearestDistance) {
				normal = trianglePlane.normal;
				// Collision information nessesary for sliding
				nearestDistance = distToCollision;
				intersectionPoint = collisionPoint;
				colliding = true;
			}
		}
	} // if not backface
}

// Set this to match application scale..
glm::vec3 MotionHandler::CollideWithWorld(glm::vec3& normal, int collisionRecursionDepth)
{
	if (collisionRecursionDepth > maxRecursionDepth)
		return eTranslation;

	const float veryCloseDistance = 0.001;
	CheckCollision(normal);
	// If no collision we just move along the velocity
	glm::vec3 destinationPoint = eTranslation + eTravel;
	if (!colliding) {
		return destinationPoint;
	}
	// *** Collision occured ***
	// The original destination point
	glm::vec3 newBasePoint = eTranslation;

	// only update if we are not already very close
	// and if so we only move very close to intersection..not
	// to the exact spot.
	if (nearestDistance >= veryCloseDistance)
	{
		glm::vec3 V = normalize(eTravel) * (nearestDistance - veryCloseDistance);
		newBasePoint = eTranslation + V;
		// Adjust polygon intersection point (so sliding
		// plane will be unaffected by the fact that we
		// move slightly less than collision tells us)
		V = normalize(V);
		intersectionPoint -= veryCloseDistance * V;
	}
	// Determine the sliding plane
	glm::vec3 slidePlaneOrigin = intersectionPoint;
	glm::vec3 slidePlaneNormal = normalize(newBasePoint - intersectionPoint);
	PLANE slidingPlane(slidePlaneOrigin, slidePlaneNormal);

	// Again, sorry about formatting.. but look carefully ;)
	glm::vec3 newDestinationPoint = destinationPoint - slidingPlane.SignedDistanceTo(destinationPoint) * slidePlaneNormal;
	// Generate the slide vector, which will become our new

	// velocity vector for the next iteration
	glm::vec3 newVelocityVector = newDestinationPoint - intersectionPoint;

	// Recurse:
	// dont recurse if the new velocity is very small
	if (newVelocityVector.length() < veryCloseDistance) {
		return newBasePoint;
	}

	eTranslation = eTranslation;
	eTravel = newVelocityVector;
	return CollideWithWorld(normal, ++collisionRecursionDepth);
}

glm::vec3 MotionHandler::CollideAndSlide(glm::vec3 translation, glm::vec3 velocity, double timeDelta, glm::vec3& normal)
{
	glm::vec3 travel = velocity * (float)timeDelta;

	normal = glm::vec3(0.0f, 0.0f, 0.0f);

	// calculate position and velocity in eSpace

	eTranslation.x = translation.x / collisionEllipse.x;
	eTranslation.y = translation.y / collisionEllipse.y;
	eTranslation.z = translation.z / collisionEllipse.z;

	eTravel.x = travel.x / collisionEllipse.x;
	eTravel.y = travel.y / collisionEllipse.y;
	eTravel.z = travel.z / collisionEllipse.z;

	colliding = false;
	// Iterate until we have our final position.
	glm::vec3 newETranslation = CollideWithWorld(normal);

	// Convert final result back to R3:
	glm::vec3 newPosition;

	newPosition.x = newETranslation.x * collisionEllipse.x;
	newPosition.y = newETranslation.y * collisionEllipse.y;
	newPosition.z = newETranslation.z * collisionEllipse.z;

	// Move the entity (application specific function)
	return newPosition;
}

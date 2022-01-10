// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Collider Physics Class Implementation
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

// include specification
#include "collider.h"

// related constructs

// ===========================================================================
// vector2 mathematical operator implementations
// ===========================================================================

float Vector2Math::Length(const VECTOR2* v)
{ return D3DXVec2Length(v); }

float Vector2Math::Dot(const VECTOR2* v1, const VECTOR2* v2)
{ return D3DXVec2Dot(v1, v2); }

void Vector2Math::Normalize(VECTOR2* v)
{ D3DXVec2Normalize(v, v); }

VECTOR2* Vector2Math::Transform(VECTOR2* v, D3DXMATRIX* m)
{ return D3DXVec2TransformCoord(v, v, m); }


// class methods

// ===========================================================================
// default constructor
// ===========================================================================
Collider::Collider():

	// general
	colliderType	(COLLIDER_TYPE::CIRCLE),
	colliderEnabled	(TRUE),
	colliderCenter	(0, 0),
	colliderAngle	(0),
	colliderScale	(1),

	// circle collider
	colliderRadius	(1.0f),
	distSquared		(0, 0),
	sumRadiiSquared	(0),

	// box colliders
	edges			({-1, -1, 1, 1}),
	corners			(),
	edge1UV			(0, 0),
	edge3UV			(0, 0),
	edge1Min		(0),
	edge1Max		(0),
	edge3Min		(0),
	edge3Max		(0),
	rotatedBoxReady	(FALSE),
	cornersDirty	(TRUE)
{}

// ===========================================================================
// circle collider constructor
// ===========================================================================
Collider::Collider(
	float			centerX,
	float			centerY,
	float			_radius,
	bool			enabled
):
	// general
	colliderType	(COLLIDER_TYPE::CIRCLE),
	colliderEnabled	(enabled),
	colliderCenter	(centerX, centerY),
	colliderAngle	(0),
	colliderScale	(1),

	// circle collider
	colliderRadius	(_radius),
	distSquared		(0, 0),
	sumRadiiSquared	(0),

	// box colliders
	edges			(),
	corners			(),
	edge1UV			(0, 0),
	edge3UV			(0, 0),
	edge1Min		(0),
	edge1Max		(0),
	edge3Min		(0),
	edge3Max		(0),
	rotatedBoxReady	(FALSE),
	cornersDirty	(TRUE)
{
	// set edges to radius to get bounding box
	edges =
	{
		long(-_radius),
		long(-_radius),
		long(_radius),
		long(_radius)
	};
}

// ===========================================================================
// box collider constructor
// ===========================================================================
Collider::Collider(
	float			centerX,
	float			centerY,
	RECT			_edges,
	bool			enabled
) :
	// general
	colliderType	(COLLIDER_TYPE::BOX),
	colliderEnabled	(enabled),
	colliderCenter	(centerX, centerY),
	colliderAngle	(0),
	colliderScale	(1),

	// circle collider
	colliderRadius	(1.0f),
	distSquared		(0, 0),
	sumRadiiSquared	(0),

	// box colliders
	edges			(_edges),
	corners			(),
	edge1UV			(0, 0),
	edge3UV			(0, 0),
	edge1Min		(0),
	edge1Max		(0),
	edge3Min		(0),
	edge3Max		(0),
	rotatedBoxReady	(FALSE),
	cornersDirty(TRUE)
{}

// ===========================================================================
// destructor
// ===========================================================================
Collider::~Collider() {}


// internal methods

// ===========================================================================
// circular collision detection
// ===========================================================================
bool Collider::collideCircle(
	Collider&	oth,
	VECTOR2&	cV
) {
	// let this collider's center be OA
	// let other collider's center be OB

	// get distance between circle centers
	distSquared = colliderCenter - *oth.getColliderCenter();

	// by pythogoreas theorem: distance^2 = a^2 + b^2
	distSquared.x = distSquared.x * distSquared.x;
	distSquared.y = distSquared.y * distSquared.y;

	// calculate sum of radii (adjusted for collider scale)
	sumRadiiSquared =
		(colliderRadius				* colliderScale) +
		(oth.getColliderRadius()	* oth.getColliderScale());

	// square sum of radii to match squared distance
	sumRadiiSquared *= sumRadiiSquared;

	// if d < (r1 + r2), circles are intersecting -> collision occured
	// d < (r1 + r2)				<=>		sqrt(a^2 + b^2) < (r1 + r2)
	// sqrt(a^2 + b^2) < (r1 + r2)	<=>		(a^2 + b^2) < (r1 + r2)^2
	
	// if entities are not colliding, return no collision occured
	if (distSquared.x + distSquared.y > sumRadiiSquared) return false;

	// else calculate collision vector (AB = OB - OA)
	cV = *oth.getColliderCenter() - colliderCenter;

	// return collision occured
	return true;
}

// ===========================================================================
// box collision detection (axis-aligned bounding box [aabb] method)
// ===========================================================================
bool Collider::collideBox(
	Collider&	oth,
	VECTOR2&	cV
) {
	// let this collider's center be OA
	// let other collider's center be OB

	// check for collision using axis aligned bounding box check
	if (
		// the right edge is to the left of the other collider's left edge
		(colliderCenter.x + edges.right * colliderScale < 
			oth.getColliderCenterX() + 
			oth.getColliderEdges().left * oth.getColliderScale())	||

		// the left edge is to the right of the other collider's right edge
		(colliderCenter.x + edges.left * colliderScale >
			oth.getColliderCenterX() + 
			oth.getColliderEdges().right * oth.getColliderScale())	||

		// the bottom edge is above the other collider's top edge
		(colliderCenter.y + edges.bottom * colliderScale <
			oth.getColliderCenterY() + 
			oth.getColliderEdges().top * oth.getColliderScale())	||

		// the top edge is below the other collider's bottom edge
		(colliderCenter.y + edges.top * colliderScale >
			oth.getColliderCenterY() + 
			oth.getColliderEdges().bottom * oth.getColliderScale())
	)
		// if any of these are true, return no collision occured
		return false;

	// else calculate collision vector (AB = OB - OA)
	cV = *oth.getColliderCenter() - colliderCenter;

	// return collision occured
	return true;
}

// ===========================================================================
// rotated box collision detection (separating axis test [sat] method)
// ===========================================================================
bool Collider::collideRotatedBox(
	Collider&	oth,
	VECTOR2&	cV
) {
	// let this collider's center be OA
	// let other collider's center be OB

	// prepare rotated box for both colliders
	computeRotatedBox();
	oth.computeRotatedBox();

	// check if collider projections fail to overlap
	if (!projectionsOverlap(oth) || !oth.projectionsOverlap(*this))

		// return no collision occurred
		return false;

	// else calculate collision vector (AB = OB - OA)
	cV = *oth.getColliderCenter() - colliderCenter;

	// return collision occured
	return true;
}

// ===========================================================================
// rotated box and circle collision detection (sat method)
// ===========================================================================
bool Collider::collideRotatedBoxCircle(
	Collider&	oth,
	VECTOR2&	cV
) {
	// working states - temporary projection states for circle
	float center1, center3, cEdge1Min, cEdge1Max, cEdge3Min, cEdge3Max;

	// prepare rotated box for this collider
	computeRotatedBox();

	// project circle center onto edge 1
	center1	= Vector2Math::Dot(&edge1UV, oth.getColliderCenter());

	// calculate min and max projections for circle on edge 1
	cEdge1Min = center1 - oth.getColliderRadius() * oth.getColliderScale();
	cEdge1Max = center1 + oth.getColliderRadius() * oth.getColliderScale();

	// if projections do not overlap, no collision is possible
	if (cEdge1Min > edge1Max || cEdge1Max < edge1Min) return false;

	// project circle center onto edge3
	center3 = Vector2Math::Dot(&edge3UV, oth.getColliderCenter());

	// calculate min and max projections for circle on edge 3
	cEdge3Min = center3 - oth.getColliderRadius() * oth.getColliderScale();
	cEdge3Max = center3 + oth.getColliderRadius() * oth.getColliderScale();

	// if projections do not overlap, no collision is possible
	if (cEdge3Min > edge3Max || cEdge3Max < edge3Min) return false;


	// if circle projection overlaps box projection

	// if the circle center is outside the lines extended from the collision 
	// box edges (voronoi region), then the nearest box corner is checked for
	// collision using a distance check.
	
	//   voronoi0 |   | voronoi1
	//         ---0---1---
	//            |   |
	//         ---3---2---
	//   voronoi3 |   | voronoi2
	
	// use overlap test to determine nearest corner

	// if circle in voronoi0, compute corner checks for corner 0
	if (center1 < edge1Min && center3 < edge3Min)
		return collideCornerCircle(corners[0], oth, cV);

	// if circle in voronoi1, compute corner checks for corner 1
	if (center1 > edge1Max && center3 < edge3Min)
		return collideCornerCircle(corners[1], oth, cV);

	// if circle in voronoi2, compute corner checks for corner 2
	if (center1 > edge1Max && center3 > edge3Max)
		return collideCornerCircle(corners[2], oth, cV);

	// if circle in voronoi3, compute corner checks for corner 3
	if (center1 < edge1Min && center3 > edge3Max)
		return collideCornerCircle(corners[3], oth, cV);

	// if no overlaps, circle not in voronoi region, so it is colliding with
	// an edge of the box.

	// calculate collision vector (center of circle -> center of box)
	cV = *oth.getColliderCenter() - colliderCenter;

	// return collision occursed
	return true;
}

// ===========================================================================
// compute circle & box-corner collisions (voronoi regions) using a distance
// check
// ===========================================================================
bool Collider::collideCornerCircle(
	VECTOR2		corner,
	Collider&	oth,
	VECTOR2&	cV
) {
	// let the corner's position vector be OA
	// let other collider's center be OB
	
	// get distance between corner and circle center
	distSquared = corner - *oth.getColliderCenter();

	// by pythogoreas theorem: distance^2 = a^2 + b^2
	distSquared.x = distSquared.x * distSquared.x;
	distSquared.y = distSquared.y * distSquared.y;

	// calculate sum of radii (adjusted for collider scale)
	// note that corner has a radii of 0, therefore it is (0 + circle radii)
	sumRadiiSquared = (oth.getColliderRadius() * oth.getColliderScale());

	// square sum of radii to match squared distance
	sumRadiiSquared *= sumRadiiSquared;

	// if d < (r1 + r2), circles are intersecting -> collision occured
	// d < (r1 + r2)				<=>		sqrt(a^2 + b^2) < (r1 + r2)
	// sqrt(a^2 + b^2) < (r1 + r2)	<=>		(a^2 + b^2) < (r1 + r2)^2

	// if entities are not colliding, return no collision occured
	if (distSquared.x + distSquared.y > sumRadiiSquared) return false;

	// else calculate collision vector (AB = OB - OA)
	cV = *oth.getColliderCenter() - corner;

	// return collision occured
	return true;
}

// ===========================================================================
// perform pre-computations for rotated box collisions
// ===========================================================================
void Collider::computeRotatedBox()
{
	// corner numbers

	// 0---1
	// |   |
	// 3---2

	// exit early if current rotated box states are valid
	if (rotatedBoxReady) return;

	// working state - shared projection scalar container
	float projection;

	// recalculate corners for rotated box if necessary
	updateCorners();

	// use corners[0] as origin, calculate projection line vectors
	edge1UV = VECTOR2(
		corners[1].x - corners[0].x, 
		corners[1].y - corners[0].y
	);
	edge3UV = VECTOR2(
		corners[3].x - corners[0].x,
		corners[3].y - corners[0].y
	);

	// get unit (direction) vectors for each projection line vector
	Vector2Math::Normalize(&edge1UV);
	Vector2Math::Normalize(&edge3UV);

	// get minimum and maximum projection for each edge

	// get projections for edge 1
	projection = Vector2Math::Dot(&edge1UV, &corners[0]);
	edge1Min = edge1Max = projection;
	projection = Vector2Math::Dot(&edge1UV, &corners[1]);
	if		(projection < edge1Min) edge1Min = projection;
	else if (projection > edge1Max) edge1Max = projection;

	// get projections for edge 3
	projection = Vector2Math::Dot(&edge3UV, &corners[0]);
	edge3Min = edge3Max = projection;
	projection = Vector2Math::Dot(&edge3UV, &corners[3]);
	if		(projection < edge3Min)	edge3Min = projection;
	else if (projection > edge3Max) edge3Max = projection;

	// update rotated box ready state to true
	rotatedBoxReady = true;
}

// ===========================================================================
// check if projections overlap against another entity
// ===========================================================================
bool Collider::projectionsOverlap(
	Collider&	oth
) {
	// working state - shared projection scalar container
	float projection;

	// working states - temporary projection states for other collider
	float oEdge1Min, oEdge1Max, oEdge3Min, oEdge3Max;


	// project other box collider onto this collider's edge1 and edge3 axes

	// project corner 0
	projection = Vector2Math::Dot(&edge1UV, oth.getColliderCorner(0));
	oEdge1Min = oEdge1Max = projection;

	// for each remaining corner,
	for (int c = 1; c < 4; c++)
	{
		// project the corner onto edge1, update min/max values accordingly
		projection = Vector2Math::Dot(&edge1UV, oth.getColliderCorner(c));
		if		(projection < oEdge1Min) oEdge1Min = projection;
		else if (projection > oEdge1Max) oEdge1Max = projection;
	}

	// if projections do not overlap, no collision is possible
	if (oEdge1Min > edge1Max || oEdge1Max < edge1Min) return false;

	// project corner 3
	projection = Vector2Math::Dot(&edge3UV, oth.getColliderCorner(0));
	oEdge3Min = oEdge3Max = projection;

	// for each remaining corner,
	for (int c = 1; c < 4; c++)
	{
		// project the corner onto edge1, update min/max values accordingly
		projection = Vector2Math::Dot(&edge3UV, oth.getColliderCorner(c));
		if		(projection < oEdge3Min) oEdge3Min = projection;
		else if (projection > oEdge3Max) oEdge3Max = projection;
	}

	// if projections do not overlap, no collision is possible
	if (oEdge3Min > edge3Max || oEdge3Max < edge3Min) return false;


	// else, if both axes projections overlap, collision has occured
	return true;
}


// public methods

// ===========================================================================
// checks and calculates collisions between this and another collider
// ===========================================================================
bool Collider::collidesWith(
	Collider&	oth,
	VECTOR2&	cV
) {
	// if either collider is disabled, no collisions may occur
	if (!colliderEnabled || !oth.getColliderEnabled()) return false;

	// handle circle-circle collision
	if (
		colliderType			== COLLIDER_TYPE::CIRCLE	&&
		oth.getColliderType()	== COLLIDER_TYPE::CIRCLE
	)
		return collideCircle(oth, cV);

	// handle simple box-box collision (neither box is rotated)
	if (
		colliderType			== COLLIDER_TYPE::BOX		&& 
		colliderAngle			== 0						&&
		oth.getColliderType()	== COLLIDER_TYPE::BOX		&&
		oth.getColliderAngle()	== 0
	)
		return collideBox(oth, cV);

	// all other combinations should use separating axis test

	// handle box-rotatedbox / rotatedbox-box collision (neither is circle)
	if (
		colliderType			!= COLLIDER_TYPE::CIRCLE	&&
		oth.getColliderType()	!= COLLIDER_TYPE::CIRCLE
	)
		return collideRotatedBox(oth, cV);


	// handle rest of combinations (one of the colliders is a circle)

	// the other collider is a circle collider
	if (
		colliderType			!= COLLIDER_TYPE::CIRCLE
	)
		return collideRotatedBoxCircle(oth, cV);

	// this collider is a circle collider, so we check for collision from the
	// other collider with our circle collider

	// compute collision from other object
	bool collide = oth.collideRotatedBoxCircle(oth, cV);

	// reverse other collider's collision vector to get our collision vector
	cV *= -1;

	// return collision status
	return collide;
}

// ===========================================================================
// check if collider bounds are outside of the specified rectangle
//
// returns an integer corresponding to the side which the collider exceeded,
// else 0 if the collider is inside the rect.
// 
//   __2__
//  |     |
// 1|  0  |3
//  |_____|
//     4
// 
// ===========================================================================
int Collider::outsideRect(
	RECT	rect
) {
	// TODO: if circle, use radius check
	// TODO: if box, use sat check

	// if collider is a circle, check if axis-aligned edge projections of 
	// circle are completely within rect edges
	if (colliderType != COLLIDER_TYPE::BOX)
	{
		if (colliderCenter.x + colliderRadius < rect.left)		return 1;
		if (colliderCenter.x - colliderRadius > rect.right)		return 3;
		if (colliderCenter.y + colliderRadius < rect.top)		return 2;
		if (colliderCenter.y - colliderRadius > rect.bottom)	return 4;
	}

	// else, check if box corners are outside of rect
	else
	{
		// get updated corners
		updateCorners();

		// check if any corner is completely outside of rect edge projection
		for (int i = 0; i < 4; i++) {

			if (corners[i].x < rect.left)	return 1;
			if (corners[i].x > rect.right)	return 3;
			if (corners[i].y < rect.top)	return 2;
			if (corners[i].y > rect.bottom)	return 4;
		}
	}

	// else, collider is inside of the rect
	return 0;
}

// ===========================================================================
// calculate corners for rotated box
// ===========================================================================
void Collider::updateCorners()
{
	// if corners are clean, no need to recalculate them
	if (!cornersDirty) return;

	// calculate rotated axis based on current collider angle
	VECTOR2 rotatedX((float) cos(colliderAngle), (float) sin(colliderAngle));
	VECTOR2 rotatedY((float) -sin(colliderAngle), (float) cos(colliderAngle));

	// calculate corners for rotated box
	corners[0] = colliderCenter +
		rotatedX * ((float)edges.left	* colliderScale) +
		rotatedY * ((float)edges.top	* colliderScale);
	corners[1] = colliderCenter +
		rotatedX * ((float)edges.right	* colliderScale) +
		rotatedY * ((float)edges.top	* colliderScale);
	corners[2] = colliderCenter +
		rotatedX * ((float)edges.right	* colliderScale) +
		rotatedY * ((float)edges.bottom	* colliderScale);
	corners[3] = colliderCenter +
		rotatedX * ((float)edges.left	* colliderScale) +
		rotatedY * ((float)edges.bottom	* colliderScale);

	// update status
	cornersDirty = FALSE;
}

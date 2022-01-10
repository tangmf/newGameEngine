// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Collider Physics Class Specification
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

#ifndef _INKDOT_COLLIDER_H
#define _INKDOT_COLLIDER_H

// import necessary headers
#include "../common.h"

// include d3d9 header and libraries

// NOTE: If d3dx9.h is NOT FOUND, go to Properties > VC++ Directories and add
// $(DXSDK_DIR)Include and $(DXSDK_DIR)LIB\x86 into Include Directories and
// Library Directories respectively

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// directx interface aliases
#define VECTOR2 D3DXVECTOR2

// namespace aliases
#define COLLIDER_TYPE PhysicsConstants::ColliderType


// related constructs

// vector2 mathematical operators
namespace Vector2Math
{
	float		Length		(const VECTOR2* v);
	float		Dot			(const VECTOR2* v1, const VECTOR2* v2);
	void		Normalize	(VECTOR2* v);
	VECTOR2*	Transform	(VECTOR2* v, D3DXMATRIX* m);
};


// class specification

class Collider
{
protected:

	// general states

	// collider type
	COLLIDER_TYPE colliderType;

	// physics toggle
	bool colliderEnabled;

	// center of collider geometry
	VECTOR2 colliderCenter;

	// angle of rotation of collider geometry in radians
	float colliderAngle;

	// scale of collider geometry
	float colliderScale;


	// circle collider states

	// radius of collision circle
	float colliderRadius;

	// working state - distance ^2 between two colliders
	VECTOR2 distSquared;

	// working state - sum of both collider radii ^2
	float sumRadiiSquared;


	// box/rotated box collider states

	// position of edges relative to object center
	RECT edges;

	// object corners for Object Bounding Box collision detection
	VECTOR2 corners[4];

	// working state - unit vectors for edges used in projection
	VECTOR2 edge1UV, edge3UV;

	// working state - min and max scalar projections for projected edges
	float edge1Min, edge1Max, edge3Min, edge3Max;

	// specifies if rotated collision box is ready
	bool rotatedBoxReady;

	// specifies if current collider corners are dirty
	bool cornersDirty;


	// internal methods

	/// <summary>
	/// Circular collision detection
	/// </summary>
	/// <param name="oth">
	/// : The other collider object to check collision for
	/// </param>
	/// <param name="cV">
	/// : Vector output to store the resultant collision vector to
	/// </param>
	/// <returns>
	/// : True if a collision did occur, and false otherwise
	/// </returns>
	bool collideCircle(Collider& oth, VECTOR2& cV);

	/// <summary>
	/// Box collision detection
	/// </summary>
	/// <param name="oth">
	/// : The other collider object to check collision for
	/// </param>
	/// <param name="cV">
	/// : Vector output to store the resultant collision vector to
	/// </param>
	/// <returns>
	/// : True if a collision did occur, and false otherwise
	/// </returns>
	bool collideBox(Collider& oth, VECTOR2& cV);

	/// <summary>
	/// Rotated box collision detection
	/// </summary>
	/// <param name="oth">
	/// : The other collider object to check collision for
	/// </param>
	/// <param name="cV">
	/// : Vector output to store the resultant collision vector to
	/// </param>
	/// <returns>
	/// : True if a collision did occur, and false otherwise
	/// </returns>
	bool collideRotatedBox(Collider& oth, VECTOR2& cV);

	/// <summary>
	/// Rotated box and circle collision detection
	/// </summary>
	/// <param name="oth">
	/// : The other Circle collider object to check collision for
	/// </param>
	/// <param name="cV">
	/// : Vector output to store the resultant collision vector to
	/// </param>
	/// <returns>
	/// : True if a collision did occur, and false otherwise
	/// </returns>
	bool collideRotatedBoxCircle(Collider& oth, VECTOR2& cV);

	/// <summary>
	/// Compute circle & box-corner collisions (voronoi regions) using a
	/// distance check
	/// </summary>
	/// <param name="corner">
	/// : The box corner to check circle collision for
	/// </param>
	/// <param name="oth">
	/// : The other collider object to check collision for
	/// </param>
	/// <param name="cV">
	/// : Vector output to store the resultant collision vector to
	/// </param>
	/// <returns>
	/// : True if a collision did occur, and false otherwise
	/// </returns>
	bool collideCornerCircle(
		VECTOR2		corner,
		Collider&	oth,
		VECTOR2&	cV
	);

	// perform pre-computations for rotated box collisions
	void computeRotatedBox();

	// check if projections overlap against another entity
	bool projectionsOverlap(Collider& oth);

public:

	// default constructor
	Collider();

	// circle collider constructor
	Collider(
		float			centerX,
		float			centerY,
		float			_radius,
		bool			enabled		= TRUE
	);

	// box collider constructor
	Collider(
		float			centerX,
		float			centerY,
		RECT			_edges,
		bool			enabled		= TRUE
	);

	// destructor
	~Collider();


	// public methods

	/// <summary>
	/// Checks and calculates collisions between this and another collider
	/// </summary>
	/// <param name="oth">
	/// : The other collider object to check collision for
	/// </param>
	/// <param name="cV">
	/// : Vector output to store the resultant collision vector to
	/// </param>
	/// <returns>
	/// : True if a collision did occur, and false otherwise
	/// </returns>
	bool collidesWith(Collider& oth, VECTOR2& cV);

	// check if collider bounds are outside of the specified rectangle
	int outsideRect(RECT rect);

	// recalculate corners for rotated box
	void updateCorners();
	

	// getters

	// return collider colliderType (none / circle / box / rotated box)
	COLLIDER_TYPE getColliderType()
	{ return colliderType; }

	// return collider colliderEnabled state
	bool getColliderEnabled() const
	{ return colliderEnabled; }

	// return collider center according to screen position
	const VECTOR2* getColliderCenter()
	{ return &colliderCenter; }

	float getColliderCenterX() const
	{ return colliderCenter.x; }

	float getColliderCenterY() const
	{ return colliderCenter.y; }

	// return collider rotation angle in radians
	float getColliderAngle() const
	{ return colliderAngle; }

	// return collider scale
	float getColliderScale() const
	{ return colliderScale; }

	// return circle collider radius
	float getColliderRadius() const
	{ return colliderRadius; }

	// return collider edges relative to object colliderCenter
	const RECT& getColliderEdges() const
	{ return edges; }

	// return corner c for rotated boxes
	const VECTOR2* getColliderCorner(UINT c) const
	{
		// wrap c to a valid value
		if (c >= 4) c %= 4;

		return &corners[c];
	}

	
	// setters

	// update collider type to circle collider
	void changeToCircleCollider(float _radius)
	{
		colliderType = COLLIDER_TYPE::CIRCLE;
		colliderRadius = _radius;

		// set edges to radius to get bounding box
		edges =
		{
			long(-_radius),
			long(-_radius),
			long(_radius),
			long(_radius)
		};
	}

	// update collider type to box collider
	void changeToBoxCollider(RECT _edges)
	{
		colliderType = COLLIDER_TYPE::BOX;
		edges = _edges;

		// recalculate rotated box states
		rotatedBoxReady = false;
		cornersDirty = true;
	}

	// set collider enabled state
	void setColliderEnabled(bool e)
	{ colliderEnabled = e; }

	// set collider center
	void setColliderCenter(float x, float y)
	{
		colliderCenter.x = x;
		colliderCenter.y = y;
		
		// recalculate rotated box states
		rotatedBoxReady = false;
		cornersDirty = true;
	}

	// set collider center using vector
	void setColliderCenter(VECTOR2 pos)
	{
		colliderCenter = pos;

		// recalculate rotated box states
		rotatedBoxReady = false;
		cornersDirty = true;
	}

	// set collider rotation angle (in radians)
	void setColliderAngle(float r)
	{
		colliderAngle = r;

		// recalculate rotated box states
		rotatedBoxReady = false;
		cornersDirty = true;
	}

	void setColliderScale(float s)
	{
		colliderScale = s;

		// recalculate rotated box states
		rotatedBoxReady = false;
		cornersDirty = true;
	}

	// set radius of collider circle
	void setColliderRadius(float r)
	{ colliderRadius = r; }

};


// class aliases

#define Collidable Collider

#endif // !_INKDOT_COLLIDER_H
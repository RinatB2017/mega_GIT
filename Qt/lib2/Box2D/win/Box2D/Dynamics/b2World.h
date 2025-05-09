// MIT License

// Copyright (c) 2019 Erin Catto

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef B2_WORLD_H
#define B2_WORLD_H

#include <b2Api.h>
#include <b2BlockAllocator.h>
#include <b2ContactManager.h>
#include <b2Math.h>
#include <b2StackAllocator.h>
#include <b2TimeStep.h>
#include <b2WorldCallbacks.h>

struct b2AABB;
struct b2BodyDef;
struct b2Color;
struct b2JointDef;
class b2Body;
class b2Draw;
class b2Fixture;
class b2Joint;

/// The world class manages all physics entities, dynamic simulation,
/// and asynchronous queries. The world also contains efficient memory
/// management facilities.
class B2_API b2World
{
public:
	/// Construct a world object.
	/// @param gravity the world gravity vector.
	b2World(const b2Vec2& gravity);

	/// Destruct the world. All physics entities are destroyed and all heap memory is released.
	~b2World();

	/// Register a destruction listener. The listener is owned by you and must
	/// remain in scope.
	void SetDestructionListener(b2DestructionListener* listener);

	/// Register a contact filter to provide specific control over collision.
	/// Otherwise the default filter is used (b2_defaultFilter). The listener is
	/// owned by you and must remain in scope.
	void SetContactFilter(b2ContactFilter* filter);

	/// Register a contact event listener. The listener is owned by you and must
	/// remain in scope.
	void SetContactListener(b2ContactListener* listener);

	/// Register a routine for debug drawing. The debug draw functions are called
	/// inside with b2World::DebugDraw method. The debug draw object is owned
	/// by you and must remain in scope.
	void SetDebugDraw(b2Draw* debugDraw);

	/// Create a rigid body given a definition. No reference to the definition
	/// is retained.
	/// @warning This function is locked during callbacks.
	b2Body* CreateBody(const b2BodyDef* def);

	/// Destroy a rigid body given a definition. No reference to the definition
	/// is retained. This function is locked during callbacks.
	/// @warning This automatically deletes all associated shapes and joints.
	/// @warning This function is locked during callbacks.
	void DestroyBody(b2Body* body);

	/// Create a joint to constrain bodies together. No reference to the definition
	/// is retained. This may cause the connected bodies to cease colliding.
	/// @warning This function is locked during callbacks.
	b2Joint* CreateJoint(const b2JointDef* def);

	/// Destroy a joint. This may cause the connected bodies to begin colliding.
	/// @warning This function is locked during callbacks.
	void DestroyJoint(b2Joint* joint);

	/// Take a time step. This performs collision detection, integration,
	/// and constraint solution.
	/// @param timeStep the amount of time to simulate, this should not vary.
	/// @param velocityIterations for the velocity constraint solver.
	/// @param positionIterations for the position constraint solver.
	void Step(	float timeStep,
				int velocityIterations,
				int positionIterations);

	/// Manually clear the force buffer on all bodies. By default, forces are cleared automatically
	/// after each call to Step. The default behavior is modified by calling SetAutoClearForces.
	/// The purpose of this function is to support sub-stepping. Sub-stepping is often used to maintain
	/// a fixed sized time step under a variable frame-rate.
	/// When you perform sub-stepping you will disable auto clearing of forces and instead call
	/// ClearForces after all sub-steps are complete in one pass of your game loop.
	/// @see SetAutoClearForces
	void ClearForces();

	/// Call this to draw shapes and other debug draw data. This is intentionally non-const.
	void DebugDraw();

	/// Query the world for all fixtures that potentially overlap the
	/// provided AABB.
	/// @param callback a user implemented callback class.
	/// @param aabb the query box.
	void QueryAABB(b2QueryCallback* callback, const b2AABB& aabb) const;

	/// Ray-cast the world for all fixtures in the path of the ray. Your callback
	/// controls whether you get the closest point, any point, or n-points.
	/// The ray-cast ignores shapes that contain the starting point.
	/// @param callback a user implemented callback class.
	/// @param point1 the ray starting point
	/// @param point2 the ray ending point
	void RayCast(b2RayCastCallback* callback, const b2Vec2& point1, const b2Vec2& point2) const;

	/// Get the world body list. With the returned body, use b2Body::GetNext to get
	/// the next body in the world list. A nullptr body indicates the end of the list.
	/// @return the head of the world body list.
	b2Body* GetBodyList();
	const b2Body* GetBodyList() const;

	/// Get the world joint list. With the returned joint, use b2Joint::GetNext to get
	/// the next joint in the world list. A nullptr joint indicates the end of the list.
	/// @return the head of the world joint list.
	b2Joint* GetJointList();
	const b2Joint* GetJointList() const;

	/// Get the world contact list. With the returned contact, use b2Contact::GetNext to get
	/// the next contact in the world list. A nullptr contact indicates the end of the list.
	/// @return the head of the world contact list.
	/// @warning contacts are created and destroyed in the middle of a time step.
	/// Use b2ContactListener to avoid missing contacts.
	b2Contact* GetContactList();
	const b2Contact* GetContactList() const;

	/// Enable/disable sleep.
	void SetAllowSleeping(bool flag);
	bool GetAllowSleeping() const { return m_allowSleep; }

	/// Enable/disable warm starting. For testing.
	void SetWarmStarting(bool flag) { m_warmStarting = flag; }
	bool GetWarmStarting() const { return m_warmStarting; }

	/// Enable/disable continuous physics. For testing.
	void SetContinuousPhysics(bool flag) { m_continuousPhysics = flag; }
	bool GetContinuousPhysics() const { return m_continuousPhysics; }

	/// Enable/disable single stepped continuous physics. For testing.
	void SetSubStepping(bool flag) { m_subStepping = flag; }
	bool GetSubStepping() const { return m_subStepping; }

	/// Get the number of broad-phase proxies.
	int GetProxyCount() const;

	/// Get the number of bodies.
	int GetBodyCount() const;

	/// Get the number of joints.
	int GetJointCount() const;

	/// Get the number of contacts (each may have 0 or more contact points).
	int GetContactCount() const;

	/// Get the height of the dynamic tree.
	int GetTreeHeight() const;

	/// Get the balance of the dynamic tree.
	int GetTreeBalance() const;

	/// Get the quality metric of the dynamic tree. The smaller the better.
	/// The minimum is 1.
	float GetTreeQuality() const;

	/// Change the global gravity vector.
	void SetGravity(const b2Vec2& gravity);

	/// Get the global gravity vector.
	b2Vec2 GetGravity() const;

	/// Is the world locked (in the middle of a time step).
	bool IsLocked() const;

	/// Set flag to control automatic clearing of forces after each time step.
	void SetAutoClearForces(bool flag);

	/// Get the flag that controls automatic clearing of forces after each time step.
	bool GetAutoClearForces() const;

	/// Shift the world origin. Useful for large worlds.
	/// The body shift formula is: position -= newOrigin
	/// @param newOrigin the new origin with respect to the old origin
	void ShiftOrigin(const b2Vec2& newOrigin);

	/// Get the contact manager for testing.
	const b2ContactManager& GetContactManager() const;

	/// Get the current profile.
	const b2Profile& GetProfile() const;

	/// Dump the world into the log file.
	/// @warning this should be called outside of a time step.
	void Dump();

private:

	friend class b2Body;
	friend class b2Fixture;
	friend class b2ContactManager;
	friend class b2Controller;

	void Solve(const b2TimeStep& step);
	void SolveTOI(const b2TimeStep& step);

	void DrawShape(b2Fixture* shape, const b2Transform& xf, const b2Color& color);

	b2BlockAllocator m_blockAllocator;
	b2StackAllocator m_stackAllocator;

	b2ContactManager m_contactManager;

	b2Body* m_bodyList;
	b2Joint* m_jointList;

	int m_bodyCount;
	int m_jointCount;

	b2Vec2 m_gravity;
	bool m_allowSleep;

	b2DestructionListener* m_destructionListener;
	b2Draw* m_debugDraw;

	// This is used to compute the time step ratio to
	// support a variable time step.
	float m_inv_dt0;

	bool m_newContacts;
	bool m_locked;
	bool m_clearForces;

	// These are for debugging the solver.
	bool m_warmStarting;
	bool m_continuousPhysics;
	bool m_subStepping;

	bool m_stepComplete;

	b2Profile m_profile;
};

inline b2Body* b2World::GetBodyList()
{
	return m_bodyList;
}

inline const b2Body* b2World::GetBodyList() const
{
	return m_bodyList;
}

inline b2Joint* b2World::GetJointList()
{
	return m_jointList;
}

inline const b2Joint* b2World::GetJointList() const
{
	return m_jointList;
}

inline b2Contact* b2World::GetContactList()
{
	return m_contactManager.m_contactList;
}

inline const b2Contact* b2World::GetContactList() const
{
	return m_contactManager.m_contactList;
}

inline int b2World::GetBodyCount() const
{
	return m_bodyCount;
}

inline int b2World::GetJointCount() const
{
	return m_jointCount;
}

inline int b2World::GetContactCount() const
{
	return m_contactManager.m_contactCount;
}

inline void b2World::SetGravity(const b2Vec2& gravity)
{
	m_gravity = gravity;
}

inline b2Vec2 b2World::GetGravity() const
{
	return m_gravity;
}

inline bool b2World::IsLocked() const
{
	return m_locked;
}

inline void b2World::SetAutoClearForces(bool flag)
{
	m_clearForces = flag;
}

/// Get the flag that controls automatic clearing of forces after each time step.
inline bool b2World::GetAutoClearForces() const
{
	return m_clearForces;
}

inline const b2ContactManager& b2World::GetContactManager() const
{
	return m_contactManager;
}

inline const b2Profile& b2World::GetProfile() const
{
	return m_profile;
}

#endif

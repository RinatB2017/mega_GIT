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

#ifndef B2_BROAD_PHASE_H
#define B2_BROAD_PHASE_H

#include <b2Api.h>
#include <b2Settings.h>
#include <b2Collision.h>
#include <b2DynamicTree.h>

struct B2_API b2Pair
{
	int proxyIdA;
	int proxyIdB;
};

/// The broad-phase is used for computing pairs and performing volume queries and ray casts.
/// This broad-phase does not persist pairs. Instead, this reports potentially new pairs.
/// It is up to the client to consume the new pairs and to track subsequent overlap.
class B2_API b2BroadPhase
{
public:

	enum
	{
		e_nullProxy = -1
	};

	b2BroadPhase();
	~b2BroadPhase();

	/// Create a proxy with an initial AABB. Pairs are not reported until
	/// UpdatePairs is called.
	int CreateProxy(const b2AABB& aabb, void* userData);

	/// Destroy a proxy. It is up to the client to remove any pairs.
	void DestroyProxy(int proxyId);

	/// Call MoveProxy as many times as you like, then when you are done
	/// call UpdatePairs to finalized the proxy pairs (for your time step).
	void MoveProxy(int proxyId, const b2AABB& aabb, const b2Vec2& displacement);

	/// Call to trigger a re-processing of it's pairs on the next call to UpdatePairs.
	void TouchProxy(int proxyId);

	/// Get the fat AABB for a proxy.
	const b2AABB& GetFatAABB(int proxyId) const;

	/// Get user data from a proxy. Returns nullptr if the id is invalid.
	void* GetUserData(int proxyId) const;

	/// Test overlap of fat AABBs.
	bool TestOverlap(int proxyIdA, int proxyIdB) const;

	/// Get the number of proxies.
	int GetProxyCount() const;

	/// Update the pairs. This results in pair callbacks. This can only add pairs.
	template <typename T>
	void UpdatePairs(T* callback);

	/// Query an AABB for overlapping proxies. The callback class
	/// is called for each proxy that overlaps the supplied AABB.
	template <typename T>
	void Query(T* callback, const b2AABB& aabb) const;

	/// Ray-cast against the proxies in the tree. This relies on the callback
	/// to perform a exact ray-cast in the case were the proxy contains a shape.
	/// The callback also performs the any collision filtering. This has performance
	/// roughly equal to k * log(n), where k is the number of collisions and n is the
	/// number of proxies in the tree.
	/// @param input the ray-cast input data. The ray extends from p1 to p1 + maxFraction * (p2 - p1).
	/// @param callback a callback class that is called for each proxy that is hit by the ray.
	template <typename T>
	void RayCast(T* callback, const b2RayCastInput& input) const;

	/// Get the height of the embedded tree.
	int GetTreeHeight() const;

	/// Get the balance of the embedded tree.
	int GetTreeBalance() const;

	/// Get the quality metric of the embedded tree.
	float GetTreeQuality() const;

	/// Shift the world origin. Useful for large worlds.
	/// The shift formula is: position -= newOrigin
	/// @param newOrigin the new origin with respect to the old origin
	void ShiftOrigin(const b2Vec2& newOrigin);

private:

	friend class b2DynamicTree;

	void BufferMove(int proxyId);
	void UnBufferMove(int proxyId);

	bool QueryCallback(int proxyId);

	b2DynamicTree m_tree;

	int m_proxyCount;

	int* m_moveBuffer;
	int m_moveCapacity;
	int m_moveCount;

	b2Pair* m_pairBuffer;
	int m_pairCapacity;
	int m_pairCount;

	int m_queryProxyId;
};

inline void* b2BroadPhase::GetUserData(int proxyId) const
{
	return m_tree.GetUserData(proxyId);
}

inline bool b2BroadPhase::TestOverlap(int proxyIdA, int proxyIdB) const
{
	const b2AABB& aabbA = m_tree.GetFatAABB(proxyIdA);
	const b2AABB& aabbB = m_tree.GetFatAABB(proxyIdB);
	return b2TestOverlap(aabbA, aabbB);
}

inline const b2AABB& b2BroadPhase::GetFatAABB(int proxyId) const
{
	return m_tree.GetFatAABB(proxyId);
}

inline int b2BroadPhase::GetProxyCount() const
{
	return m_proxyCount;
}

inline int b2BroadPhase::GetTreeHeight() const
{
	return m_tree.GetHeight();
}

inline int b2BroadPhase::GetTreeBalance() const
{
	return m_tree.GetMaxBalance();
}

inline float b2BroadPhase::GetTreeQuality() const
{
	return m_tree.GetAreaRatio();
}

template <typename T>
void b2BroadPhase::UpdatePairs(T* callback)
{
	// Reset pair buffer
	m_pairCount = 0;

	// Perform tree queries for all moving proxies.
	for (int i = 0; i < m_moveCount; ++i)
	{
		m_queryProxyId = m_moveBuffer[i];
		if (m_queryProxyId == e_nullProxy)
		{
			continue;
		}

		// We have to query the tree with the fat AABB so that
		// we don't fail to create a pair that may touch later.
		const b2AABB& fatAABB = m_tree.GetFatAABB(m_queryProxyId);

		// Query tree, create pairs and add them pair buffer.
		m_tree.Query(this, fatAABB);
	}

	// Send pairs to caller
	for (int i = 0; i < m_pairCount; ++i)
	{
		b2Pair* primaryPair = m_pairBuffer + i;
		void* userDataA = m_tree.GetUserData(primaryPair->proxyIdA);
		void* userDataB = m_tree.GetUserData(primaryPair->proxyIdB);

		callback->AddPair(userDataA, userDataB);
	}

	// Clear move flags
	for (int i = 0; i < m_moveCount; ++i)
	{
		int proxyId = m_moveBuffer[i];
		if (proxyId == e_nullProxy)
		{
			continue;
		}

		m_tree.ClearMoved(proxyId);
	}

	// Reset move buffer
	m_moveCount = 0;
}

template <typename T>
inline void b2BroadPhase::Query(T* callback, const b2AABB& aabb) const
{
	m_tree.Query(callback, aabb);
}

template <typename T>
inline void b2BroadPhase::RayCast(T* callback, const b2RayCastInput& input) const
{
	m_tree.RayCast(callback, input);
}

inline void b2BroadPhase::ShiftOrigin(const b2Vec2& newOrigin)
{
	m_tree.ShiftOrigin(newOrigin);
}

#endif

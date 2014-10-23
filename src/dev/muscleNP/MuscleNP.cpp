/*
 * Copyright © 2012, United States Government, as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All rights reserved.
 * 
 * The NASA Tensegrity Robotics Toolkit (NTRT) v1 platform is licensed
 * under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0.
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
*/

/**
 * @file MuscleNP.cpp
 * @brief Definition of a massless cable with contact dynamics
 * $Id$
 */

// This object
#include "MuscleNP.h"

// NTRT
#include "tgcreator/tgUtil.h"
#include "core/MuscleAnchor.h"
#include "core/tgCast.h"
// The Bullet Physics library
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletCollision/BroadphaseCollision/btOverlappingPairCache.h"
#include "BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "BulletDynamics/Dynamics/btActionInterface.h"
#include "LinearMath/btDefaultMotionState.h"

// Classes for manual collision detection - will likely remove later
#include "BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h"
#include "BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h"
#include "BulletCollision/NarrowPhaseCollision/btPointCollector.h"
#include "BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h"
#include "BulletCollision/NarrowPhaseCollision/btConvexPenetrationDepthSolver.h"
#include "BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.h"

#include <iostream>

MuscleNP::MuscleNP(btPairCachingGhostObject* ghostObject,
btBroadphaseInterface* broadphase,
 btRigidBody * body1,
 btVector3 pos1,
 btRigidBody * body2,
 btVector3 pos2,
 double coefK,
 double dampingCoefficient) :
Muscle2P (body1, pos1, body2, pos2, coefK, dampingCoefficient),
m_ghostObject(ghostObject),
m_overlappingPairCache(broadphase)
{
	
}
         
MuscleNP::~MuscleNP()
{
	
}

btVector3 MuscleNP::calculateAndApplyForce(double dt)
{
	btManifoldArray	m_manifoldArray;
	btVector3 m_touchingNormal;
	
	//std::cout << m_ghostObject->getOverlappingPairCache()->getNumOverlappingPairs() << std::endl;

	// Only caches the pairs, they don't have a lot of useful information
	btBroadphasePairArray& pairArray = m_ghostObject->getOverlappingPairCache()->getOverlappingPairArray();
	int numPairs = pairArray.size();

	for (int i=0;i<numPairs;i++)
	{
		m_manifoldArray.clear();

		const btBroadphasePair& pair = pairArray[i];
		
		// The real broadphase's pair cache has the useful info
		btBroadphasePair* collisionPair = m_overlappingPairCache->getOverlappingPairCache()->findPair(pair.m_pProxy0,pair.m_pProxy1);

		btCollisionObject* obj0 = static_cast<btCollisionObject*>(collisionPair->m_pProxy0->m_clientObject);
                btCollisionObject* obj1 = static_cast<btCollisionObject*>(collisionPair->m_pProxy1->m_clientObject);

		if (collisionPair->m_algorithm)
			collisionPair->m_algorithm->getAllContactManifolds(m_manifoldArray);
		
		//std::cout  << m_manifoldArray.size() << std::endl;
		
		for (int j=0;j<m_manifoldArray.size();j++)
		{
			btPersistentManifold* manifold = m_manifoldArray[j];
			btScalar directionSign = manifold->getBody0() == m_ghostObject ? btScalar(-1.0) : btScalar(1.0);
			for (int p=0;p<manifold->getNumContacts();p++)
			{
				const btManifoldPoint&pt = manifold->getContactPoint(p);

				btScalar dist = pt.getDistance();
				
				if (dist < 0.0)
				{
					
					m_touchingNormal = pt.m_normalWorldOnB * directionSign;//??
					
					btRigidBody* rb = NULL;
					
					if (directionSign < 0)
					{
						rb = btRigidBody::upcast(obj1);
					}
					else
					{
						rb = btRigidBody::upcast(obj0);
					}
					if ( rb != anchor1->attachedBody || rb != anchor2->attachedBody)
					{
						//std::cout << pt.m_positionWorldOnB << " " << m_touchingNormal << std::endl;
					
						btScalar mass = rb->getInvMass() == 0 ? 0.0 : 1.0 / rb->getInvMass();
						btVector3 impulse = mass * dt * m_touchingNormal * getTension() / getActualLength() * dist;
						rb->applyImpulse(impulse, pt.m_positionWorldOnB);
					}
					
				}
			}
			
			//manifold->clearManifold();
		}
	
	}
	
	btVector3 from = anchor1->getWorldPosition();
	btVector3 to = anchor2->getWorldPosition();
	
	btTransform transform = tgUtil::getTransform(from, to);
	
	//std::cout << (to - from).length()/2.0 << std::endl;
	
	m_ghostObject->setWorldTransform(transform);
	
	btScalar radius = 0.01;
	
	btCylinderShape* shape = tgCast::cast<btCollisionShape,  btCylinderShape>(*m_ghostObject->getCollisionShape());
	/* Note that 1) this is listed as "use with care" in Bullet's documentation and
	 * 2) we had to remove the object from DemoApplication's render function in order for it to render properly
	 * changing from a non-contact object will break that behavior.
	 */ 
	shape->setImplicitShapeDimensions(btVector3(radius, (to - from).length()/2.0, radius));
	m_ghostObject->setCollisionShape(shape);
	
	Muscle2P::calculateAndApplyForce(dt);
}

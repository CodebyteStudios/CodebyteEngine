/*********************************************************************************
Codebyte Engine

Copyright (c) 2014 Cameron Kline

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*********************************************************************************/

#ifndef PHYSICS_H
#define PHYSICS_H
#pragma once

#include "Tools.h"
#include "Transform.h"

#include "Manager.h"
#include "Object.h"

class PhysicsManager;

class Physics{
public:
    static PhysicsManager manager;

public:
    Object object;
    btRigidBody* rigidbody;
    btCollisionShape* shape;
    btDefaultMotionState* motionState;
    Physics(String name, Transform transform, btScalar mass, std::string shapeType);
    ~Physics();
};

// A custom manager class
class PhysicsManager: public Manager<Physics>{
private:
    float timeStep;
    btBroadphaseInterface*                 broadphase;
    btDefaultCollisionConfiguration*       collisionConfiguration;
    btCollisionDispatcher*                 dispatcher;
    btSequentialImpulseConstraintSolver*   solver;
public:
    btDiscreteDynamicsWorld*               dynamicsWorld;
    void Setup(glm::vec3 gravity, float fixedTimeStep);
    void Update();
    void Cleanup();
};

#endif /* PHYSICS_H */

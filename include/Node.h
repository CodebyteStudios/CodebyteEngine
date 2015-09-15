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

#ifndef NODE_H
#define NODE_H
#pragma once

#include "Tools.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "Physics.h"

#include "Manager.h"
#include "Object.h"

/* Represents a drawable object in the scene */

class Node{
public:
    static Manager<Node> manager;

// Instance stuff
private:
	Node* parentNode;
public:
    /* Possible Components (all nodes have to have a Transform)*/
    Transform   transform;
    Mesh        * meshPtr;
    Material    * materialPtr;
    Camera      * cameraPtr;
    Physics     * physicsPtr;

    void SetMesh(Mesh* object);
    void SetMaterial(Material* object);
    void SetCamera(Camera* object);
    void SetPhysics(Physics* objects);

    /* These functions ensure that the Transform and
     * Physics components are in sync with each other,
     * always make sure to use them when moving an object! */
    void Translate(float x, float y, float z, bool global);
    void Rotate(float x, float y, float z, bool global);
    void Scale(float x, float y, float z, bool global);
    void ApplyForce(btVector3 force, bool global);
    void ApplyImpulse(btVector3 impulse, bool global);
    void ApplyTorque(btVector3 torque);

    /* false will sync the transform to the rigidbody, and true will do the opposite*/
    void SyncTransformAndRigidbody(bool transOrRig, uint partsToSync);

    /* End of Component stuff */

    std::vector<Node*> childNodes;
    Object object;
    Node(String name);
    Node(String name, Transform transform);
	~Node(void);
	void SetParent(Node* node);
    Node* GetParent();
	void AddChild(Node* node);
    void RemoveChild(ulong id);
    void Update(glm::mat4* combinedModelMatrix); // The combined matrix of all its ancestors
    void Render();

};

#endif /* NODE_H */

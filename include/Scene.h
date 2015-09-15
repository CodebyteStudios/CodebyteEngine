#ifndef SCENE_H
#define SCENE_H
#pragma once

#include "Tools.h"
#include "Node.h"

class Scene{
public:
    static Node* base;
    static Node* currentCameraNode;
    static void Setup();
    static void Cleanup();
    static void LoadMap(String filename);
    static void SaveMap(String filename);
    static void CreateNode(String name, Node* parent, Transform transform, Mesh* mesh, Material* material, Camera* camera, Physics* physics);
    static void RemoveNode(ulong id);
    static void Update();
    static void Render();
    static void Sort();
    static void SetCurrentCamera(Node* cameraNode);
};

#endif /* SCENE_H */

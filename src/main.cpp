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

#include "Tools.h"

#include "Script.h"
#include "Scene.h"
#include "GUI.h"

bool keys[512];
double  lastMouseX = 0.0f, lastMouseY = 0.0f,
        currentMouseX = 0.0f, currentMouseY = 0.0f,
        mouseDiffX = 0.0f, mouseDiffY = 0.0f;
float deltaTime = 0.0f;
bool mouseLocked = true;

GLFWwindow * window;
GUI::GUIManager * guiManager;

Camera   * camera;

Node     * player;
Node     * playerCamera;
Node     * boat;

Script   * testScript;

bool inBoat = true;

void Setup(){
    for(uint i = 0; i < 512; i++)
        keys[i] = false;

    Scene::LoadMap(CurrentWorkingDirectory() + "/Resources/Maps/test.map");
    Script::manager.Setup();
    Script::manager.LoadScript(CurrentWorkingDirectory() + "/Resources/Scripts/test.js");

    testScript = new Script( CurrentWorkingDirectory() + "/testScript", "Player");

    GUI::GUIManager::Setup(800, 600);

    Node * terrain = new Node("terrain", Transform(glm::vec3(0.0f, -20.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f)));
    Mesh * terrainMesh = new Mesh("terrain");
    terrainMesh->LoadTerrain(CurrentWorkingDirectory() + "/Resources/Textures/terrain.png");
    terrain->SetMesh(terrainMesh);
    terrain->SetMaterial(Material::manager.At("floor"));
    Node::manager.At("base")->AddChild(terrain);

    camera = Camera::manager.At("camera");
    player = Node::manager.At("player");
    playerCamera = Node::manager.At("playerCamera");
    boat = Node::manager.At("boat");
    Scene::SetCurrentCamera(playerCamera);

    if(player == NULL) std::cout << "player is null" << std::endl;
}

void Cleanup(){
    GUI::GUIManager::Cleanup();
    Scene::Cleanup();
}

void Update(){
    float moveSpeed = 0.01f * (deltaTime * 1000) * ((keys[GLFW_KEY_LEFT_SHIFT] == true) ? 2 : 1);
    float lookSpeed = 0.0025f * (deltaTime * 1000);

    if(keys[GLFW_KEY_LEFT_CONTROL] == true) player->transform.position.y = 2.5f;
    else player->transform.position.y = 5.0f;

    /* Move the player */
    if(keys[GLFW_KEY_A] == true)		player->transform.TranslateLocal(-moveSpeed, 0, 0);
    if(keys[GLFW_KEY_D] == true)		player->transform.TranslateLocal(moveSpeed, 0, 0);
    if(keys[GLFW_KEY_S] == true)		player->transform.TranslateLocal(0, 0, moveSpeed);
    if(keys[GLFW_KEY_W] == true)		player->transform.TranslateLocal(0, 0, -moveSpeed);

    /* Move the camera */
    //if(keys[GLFW_KEY_DOWN] == true)   playerCamera->transform.RotateLocal(-lookSpeed, 0, 0);
    //if(keys[GLFW_KEY_UP] == true)		playerCamera->transform.RotateLocal(lookSpeed, 0, 0);
    //if(keys[GLFW_KEY_LEFT] == true)	player->transform.RotateGlobal(0, lookSpeed, 0);
    //if(keys[GLFW_KEY_RIGHT] == true)	player->transform.RotateGlobal(0, -lookSpeed, 0);

    if(keys[GLFW_KEY_LEFT] == true)		boat->Rotate(0, 0.0025f * (deltaTime * 1000), 0, false);
    if(keys[GLFW_KEY_RIGHT] == true)	boat->Rotate(0, -0.0025f * (deltaTime * 1000), 0, false);
    if(keys[GLFW_KEY_UP] == true)		boat->ApplyForce(btVector3(1 * (deltaTime * 1000), 0, 0), false);
    if(keys[GLFW_KEY_DOWN] == true)   	boat->ApplyForce(btVector3(-1 * (deltaTime * 1000), 0, 0), false);
    if(keys[GLFW_KEY_SPACE] == true)  	boat->ApplyForce(btVector3(0, 2, 0), false);
    //if(keys[GLFW_KEY_UP] == true)		boat->Translate(0.01f * (deltaTime * 1000), 0.0f, 0.0f, false);
    //if(keys[GLFW_KEY_DOWN] == true)   boat->Translate(-0.01f * (deltaTime * 1000), 0.0f, 0.0f, false);

    if(mouseLocked == true){
        player->transform.RotateGlobal(0, mouseDiffX / 100.0f, 0);
        playerCamera->transform.RotateLocal(mouseDiffY / 100.0f, 0, 0);
    }

    //boat->transform.RotateGlobal(0, 0.0005f * (deltaTime * 1000), 0);
    //boat->transform.TranslateLocal(0.005f * (deltaTime * 1000), 0, 0);

    //std::cout << player->transform.position.x << ":" << player->transform.position.y << ":" << player->transform.position.z << std::endl;

    Script::manager.Update();
    Physics::manager.Update();
    Scene::Update();
}

void Render(){
    /* Update all nodes and render them, starting from the base node (which is zero) */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Scene::Render();
    mouseDiffX = 0; mouseDiffY = 0;
    GUI::GUIManager::Render();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_UNKNOWN){
        std::cout << "Unknown key detected!" << std::endl;
        return;
    }

    if(action == GLFW_PRESS) keys[key] = true;
    if(action == GLFW_RELEASE) keys[key] = false;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == GLFW_KEY_1 && action == GLFW_PRESS){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mouseLocked = true;
    }
    if(key == GLFW_KEY_2 && action == GLFW_PRESS){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        mouseLocked = false;
    }
}

static void mouse_callback(GLFWwindow* window, double x, double y){
    currentMouseX = x; currentMouseY = y;
    mouseDiffX = ((lastMouseX > currentMouseX) ? lastMouseX - currentMouseX : -(currentMouseX - lastMouseX));
    mouseDiffY = ((lastMouseY > currentMouseY) ? lastMouseY - currentMouseY : -(currentMouseY - lastMouseY));
    lastMouseX = currentMouseX; lastMouseY = currentMouseY;
}

static void onresize(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    GUI::GUIManager::Resize(width, height);
    Scene::currentCameraNode->cameraPtr->SetAspect((float)width / (float)height);
}

int main(void){
    /* Initialize the library */
    if (!glfwInit()) return -1;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    window = glfwCreateWindow(800, 600, "Hello World!", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Setup some glfw callbacks */
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetWindowSizeCallback(window, onresize);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* Initalize glew */
    glewExperimental = GL_TRUE; 
    if(glewInit() != GLEW_OK){
        std::cout << "Error loading glew" << std::endl;
    }

    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

    /* Set some OpenGL state values */
    glClearColor(0.4f, 0.6f, 0.9f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    /* Setup the game */
    Setup();

    double newTime = glfwGetTime();
    double lastTime = newTime;
    deltaTime = newTime - lastTime;
    std::string title = patch::to_string(deltaTime);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){
        newTime = glfwGetTime();
        if(((newTime - lastTime) * 1000) < (1000.0f / 60.0f)){
            // dont do anything
        }else{
            Update();
            Render();
            glfwSwapBuffers(window);
            glfwPollEvents();
            deltaTime = newTime - lastTime;
            lastTime = newTime;
        }
    }

    Cleanup();
    glfwTerminate();
    return 0;
}

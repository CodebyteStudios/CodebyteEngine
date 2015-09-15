#include "Scene.h"

Node* Scene::base = NULL;
Node* Scene::currentCameraNode = NULL;

//bool sortByShaderId(Node* firstNode, Node* secondNode){
//    if(firstNode->materialPtr != NULL && secondNode->materialPtr != NULL)
//        return (firstNode->materialPtr->shader->shaderID < secondNode->materialPtr->shader->shaderID);
//    else
//        return false;
//}

void Scene::Setup(){
    base = new Node("base");
}

void Scene::Cleanup(){
    Node::manager.Cleanup();
    Camera::manager.Cleanup();
    Shader::manager.Cleanup();
    Mesh::manager.Cleanup();
    Texture::manager.Cleanup();
    Material::manager.Cleanup();
    Physics::manager.Cleanup();
}

void Scene::LoadMap(String filename){
    // Make sure to cleanup before loading new stuff
    Cleanup();
    Physics::manager.Setup(glm::vec3(0, -9.81f, 0), 1.0f / 60.0f);

    // Make sure to add the base node
    base = new Node("base");

    StringArray fileContents = LoadFile(filename);
    String line = "";
    int currentResourceGroup = -1;

    for(uint i = 0; i < (uint)fileContents.size(); i++){
        line = fileContents.at(i);
        line = RemoveCharToChar(line, '#', '\n');
        if(line == "[models]")          currentResourceGroup = 0;
        else if(line == "[textures]")   currentResourceGroup = 1;
        else if(line == "[shaders]")    currentResourceGroup = 2;
        else if(line == "[materials]")  currentResourceGroup = 3;
        else if(line == "[cameras]")    currentResourceGroup = 4;
        else if(line == "[physics]")    currentResourceGroup = 5;
        else if(line == "[nodes]")      currentResourceGroup = 6;
        else if(line.size() > 0 || line.substr(0, 1) != "#"){ // Its probably a resource
            switch(currentResourceGroup){
                case 0: {
                    // [format] name : filelocation
                    StringArray tokens = SplitString(line, ":");
                    Mesh* temp = new Mesh(tokens.at(0));
                    temp->Load(CurrentWorkingDirectory() + "/" + tokens.at(1));
                } break;
                case 1: {
                    // [format] name : filelocation
                    StringArray tokens = SplitString(line, ":");
                    Texture* temp = new Texture(tokens.at(0));
                    temp->Load(CurrentWorkingDirectory() + "/" + tokens.at(1));
                } break;
                case 2: {
                    // [format] name : filelocation
                    StringArray tokens = SplitString(line, ":");
                    Shader* temp = new Shader(tokens.at(0));
                    temp->Load(CurrentWorkingDirectory() + "/" + tokens.at(1));
                } break;
                case 3: {
                    // [format] name : filelocation
                    StringArray tokens = SplitString(line, ":");
                    Material* temp = new Material(tokens.at(0));
                    temp->Load(CurrentWorkingDirectory() + "/" + tokens.at(1));
                } break;
                case 4: {
                    // [format] name : fov : aspect
                    StringArray tokens = SplitString(line, ":");
                    if((uint)tokens.size() == 3)
                        Camera* temp = new Camera(tokens.at(0), std::atof(tokens.at(1).c_str()), 800.0f / 600.0f);
                    else
                        std::cout << "Invalid camera format in map file '" << filename << "' on line " << (i + 1) << std::endl;
                } break;
                case 5: {
                    // [format] name : position : rotation : scale : mass : shape
                    StringArray tokens = SplitString(line, ":");
                    if((uint)tokens.size() == 6){
                        StringArray pos = SplitString(tokens.at(1), " ");
                        StringArray rot = SplitString(tokens.at(2), " ");
                        StringArray scale = SplitString(tokens.at(3), " ");
                        Physics* temp = new Physics(tokens.at(0), Transform(
                                glm::vec3(std::atof(pos.at(0).c_str()), std::atof(pos.at(1).c_str()), std::atof(pos.at(2).c_str())),
                                glm::quat(glm::vec3(std::atof(rot.at(0).c_str()), std::atof(rot.at(1).c_str()), std::atof(rot.at(2).c_str()))),
                                glm::vec3(std::atof(scale.at(0).c_str()), std::atof(scale.at(1).c_str()), std::atof(scale.at(2).c_str()))
                            ), std::atoi(tokens.at(4).c_str()), tokens.at(5)
                        );
                    }else{
                        std::cout << "Invalid physics object format in map file '" << filename << "' on line " << (i + 1) << std::endl;
                    }
                } break;
                case 6: {
                    // [format] name : parent : position : rotation : scale : meshId : materialId : cameraId : physicsId
                    StringArray tokens = SplitString(line, ":");
                    if(tokens.size() == 9){
                        StringArray pos = SplitString(tokens.at(2), " ");
                        StringArray rot = SplitString(tokens.at(3), " ");
                        StringArray scale = SplitString(tokens.at(4), " ");

                        CreateNode(tokens.at(0), Node::manager.At(tokens.at(1)),
                            Transform(
                                glm::vec3(std::atof(pos.at(0).c_str()), std::atof(pos.at(1).c_str()), std::atof(pos.at(2).c_str())),
                                glm::quat(glm::vec3(std::atof(rot.at(0).c_str()), std::atof(rot.at(1).c_str()), std::atof(rot.at(2).c_str()))),
                                glm::vec3(std::atof(scale.at(0).c_str()), std::atof(scale.at(1).c_str()), std::atof(scale.at(2).c_str()))
                            ),
                            ((tokens.at(5) == "none") ? NULL : Mesh::manager.At(tokens.at(5))),
                            ((tokens.at(6) == "none") ? NULL : Material::manager.At(tokens.at(6))),
                            ((tokens.at(7) == "none") ? NULL : Camera::manager.At(tokens.at(7))),
                            ((tokens.at(8) == "none") ? NULL : Physics::manager.At(tokens.at(8)))
                        );
                    }else{
                        std::cout << "Invalid node format in map file '" << filename << "' on line " << (i + 1) << std::endl;
                    }
                } break;
            }
        }
    }

}

void Scene::SaveMap(String filename){

}

// We use pointers here in case we dont want one of them (therefore it'd be NULL)
void Scene::CreateNode(String name, Node* parent, Transform transform, Mesh* mesh, Material* material, Camera* camera, Physics* physics){
    Node* temp = new Node(name, transform);
    parent->AddChild(temp);
    if(mesh != NULL)        temp->SetMesh(mesh);
    if(material != NULL)    temp->SetMaterial(material);
    if(camera != NULL)      temp->SetCamera(camera);
    if(physics != NULL)     temp->SetPhysics(physics);
    // Sort the list by shader (might speed up rendering)
    Scene::Sort();
}

void Scene::RemoveNode(ulong nodeId){

}

void Scene::Update(){
    glm::mat4 * mat = new glm::mat4(1.0f);
    base->Update(mat);
    delete mat;
}

void Scene::Render(){
    // This function will do some occlusion culling magic
    // Then only render things that need rendered
    GLuint lastShaderId = 0;
    for(uint i = 0; i < (uint)Node::manager.objects.size(); i++){
        if(Node::manager.objects.at(i)->meshPtr != NULL && Node::manager.objects.at(i)->materialPtr != NULL){
            // Make sure where using the right shader
            if(Node::manager.objects.at(i)->materialPtr->shader->shaderID != lastShaderId){
                Node::manager.objects.at(i)->materialPtr->shader->Bind();
                if(currentCameraNode != NULL){
                    if(currentCameraNode->cameraPtr != NULL){
                        glUniformMatrix4fv(Node::manager.objects.at(i)->materialPtr->shader->GetUniform("projectionMatrix"), 1, GL_FALSE, &currentCameraNode->cameraPtr->projectionMatrix[0][0]);
                        glUniformMatrix4fv(Node::manager.objects.at(i)->materialPtr->shader->GetUniform("viewMatrix"), 1, GL_FALSE, &glm::inverse(currentCameraNode->transform.globalModelMatrix)[0][0]);
                    }else{
                        std::cout << "currentCameraNode cameraPtr is NULL!" << std::endl;
                    }
                }
                lastShaderId = Node::manager.objects.at(i)->materialPtr->shader->shaderID;
            }

            // Upload material attributes
            Node::manager.objects.at(i)->materialPtr->Upload(Node::manager.objects.at(i)->materialPtr->shader);

            Node::manager.objects.at(i)->Render();
        }
    }
}

void Scene::Sort(){
    //std::cout << "--------------------SORTING NODES--------------------" << std::endl;

    // Old node order
    //for(uint i = 0; i < (uint)nodes.size(); i++){
    //    if(nodes.at(i)->materialPtr != NULL)
    //        std::cout << nodes.at(i)->materialPtr->shader->shaderID << " ";
    //    else
    //        std::cout << "0 ";
    //}std::cout << std::endl;

    //std::sort(nodes.begin(), nodes.end(), sortByShaderId);

    // New node order
    //for(uint i = 0; i < (uint)nodes.size(); i++){
    //    if(nodes.at(i)->materialPtr != NULL)
    //        std::cout << nodes.at(i)->materialPtr->shader->shaderID << " ";
    //    else
    //        std::cout << "0 ";
    //}std::cout << std::endl;
}

void Scene::SetCurrentCamera(Node* cameraNode){
    if(cameraNode->cameraPtr != NULL)
        currentCameraNode = cameraNode;
    else
        std::cout << "Node '" << cameraNode->object.name << "' does not have a camera attached to it!" << std::endl;
}

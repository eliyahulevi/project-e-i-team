#pragma once
#define _USE_MATH_DEFINES

#include "Trans.h"
#include "ImguiMenus.h"
#include "MeshModel.h"
#include "Utils.h"
#include <cmath>
#include <memory>
#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <nfd.h>
#include <random>
#include <iostream>
#include <list>
#include <conio.h>
#include <string>

static glm::vec4 backgroundColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
bool showAboutUsWindow = false;
bool showTransWindow = true;
bool showDemoWindow = false;
bool showSimpleWindow = false;

const glm::vec4& GetClearColor(){
	return backgroundColor;
}

const char* getModelNames(Scene* scene,int modelType=MESH_MODEL_TYPE) {
	int length = 0;
	if (modelType == MESH_MODEL_TYPE) {
		length = scene->GetModelCount();
	} else { /* CAMERA_MODEL_TYPE */
		length = scene->GetCameraCount();
	}
	if (length == 0) {
		char* empty = new char[1]{ '\0' };
		return empty;
	}
	std::string cStr = "";
	for (size_t i = 0; i < length; i++) {
		if (modelType == MESH_MODEL_TYPE) {
			std::string modelName = scene->GetModel(i)->GetModelName();
			cStr += modelName;
			cStr += '\0';
		} else { /* CAMERA_MODEL_TYPE */
			std::string cam(scene->GetCamera(i)->GetModelName());
			cStr += cam.substr(0, cam.length() - sizeof(".obj") + 1);
			cStr += " ";
			cStr += i;
			cStr += '\0';
		}
	}
	cStr += '\0';
	int listLength = cStr.length();
	char* comboList = new char[listLength];
	if (listLength == 1) { comboList[0] = cStr.at(0); }
	for (size_t i = 0; i < listLength; i++) {
		comboList[i] = cStr.at(i);
	}
	return comboList;
}

const char* getCamerasNames(int length) {
	string cStr = "";
	for (size_t i = 0; i < length; i++) {

		cStr += "Camera ";
		if (i < 10) {
			cStr += '0' + i;
		}
		else {
			cStr += '0' + int(i / 10);
			cStr += '0' + int(i % 10);
		}
		cStr += '\0';
	}
	cStr += '\0';
	int listLength = cStr.length();

	char* comboList = new char[listLength];
	for (size_t i = 0; i < listLength; i++) {
		comboList[i] = cStr.at(i);
	}
	return comboList;
}

void handleKeyboardInputs(std::shared_ptr<MeshModel> model) {
	if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_PageUp))) {
		if(model->fScale < MAX_SCALE_FACTOR){
			model->fScale += SCALE_OBJ_FACTOR;
		}
	}
	else if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_PageDown))){
		if(model->fScale > MIN_SCALE_FACTOR){
			model->fScale -= SCALE_OBJ_FACTOR;
		}
	}
	else if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_LeftArrow))){
		if (model->fTranslatex > MIN_TRANSLATION_LENGTH) {
			model->fTranslatex -= XTRANS_FACTOR;
		}
	}
	else if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_RightArrow))){
		if(model->fTranslatex < MAX_TRANSLATION_LENGTH){
			model->fTranslatex += XTRANS_FACTOR;
		}
	}
	else if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow))){
		if(model->fTranslatey < MAX_TRANSLATION_LENGTH){
			model->fTranslatey += YTRANS_FACTOR;
		}
	}
	else if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow))){
		if(model->fTranslatey > MIN_TRANSLATION_LENGTH){
			model->fTranslatey -= YTRANS_FACTOR;
		}
	}
}

void handleZoomByYscrolling(float* fScale,int y_scroll_offset) {
	if (y_scroll_offset > 1) {
		for (int i = 0; i < y_scroll_offset; i++) {
			*fScale += SCALE_OBJ_FACTOR;
		}
	}
	else if (y_scroll_offset < -1) {
		for (int i = 0; i < y_scroll_offset; i++) {
			*fScale -= SCALE_OBJ_FACTOR;
		}
	}
}

// it is important to use public variable for lite reading and writing values from object's fields
void buildTransformationsWindow(ImGuiIO& io,Scene* scene,int y_scroll_offset, const int frameBufferWidth, const int frameBufferHeight) {
	ImGui::Begin("Scene Menu", &showTransWindow);
	ImVec4 textColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	ImGui::TextColored(textColor,"Transformations Window:");

	ImGui::ColorEdit3("Background Color", (float*)&backgroundColor); // Edit 3 floats representing a color
	
	if (ImGui::CollapsingHeader("Cameras")) {
		
		if (ImGui::Button("Add camera")) {
			std::string path = Get_Root_Project_Dir("Data\\camera.obj");
			cout << "camera path = " << path << endl;
			scene->AddCamera(std::make_shared<MeshModel>(Utils::LoadMeshModel(path)) , frameBufferHeight);
		}
		const char* cameras = getCamerasNames(scene->activeCameraIndex);
		ImGui::Combo("Active Camera", &(scene->currentActiveCamera), cameras, IM_ARRAYSIZE(cameras));
		std::shared_ptr<Camera> currentCam = scene->GetCamera(scene->currentActiveCamera);
		if (currentCam != NULL) {
			ImGui::ColorEdit3("Cam Color", (float*)&(currentCam->color)); // Edit 3 floats representing a color
		}
		
		if (currentCam != NULL) {
			//float yoffset = y_scroll_offset;
			//glm::vec3 eye = glm::vec3(io.MousePos.x, io.MousePos.y, 0);
			//glm::vec3 at = glm::vec3(0,0,0);
			//glm::vec3 up = glm::vec3(io.MousePos.x, io.MousePos.y, 0);
			//currentCam->SetCameraLookAt(eye,at,up);
			
		
			ImGui::RadioButton("Perspective", &(currentCam->transType), 0);
			ImGui::RadioButton("Orthographic", &(currentCam->transType), 1);
			ImGui::SliderFloat("Fovy", &(currentCam->ffovy), MIN_FFOVY, MAX_FFOVY);
			ImGui::SliderFloat("Near", &(currentCam->fnear), MIN_FNEAR, MAX_FNEAR);
			ImGui::SliderFloat("Far", &(currentCam->ffar), MIN_FFAR, MAX_FFAR);
			float aspectratio = float(frameBufferWidth) / float(frameBufferHeight);
			if (currentCam->transType) {
				//Orthographic
				currentCam->SetOrthographicProjection(currentCam->ffovy, aspectratio, -1 *currentCam->fnear, -1 *currentCam->ffar);
			}
			else {
				//Perspective
				currentCam->SetPerspectiveProjection(currentCam->ffovy, aspectratio, -1 * currentCam->fnear, -1 *currentCam->ffar);
			}
			
			if (ImGui::Button("Focus on current model")) {
				glm::vec3 at;
				std::vector<Face> faces = scene->getModelfaces(scene->activeModelIndex);

				for (auto face = faces.begin(); face != faces.end(); ++face) {
					int v = face->GetVertexIndex(0) - 1;
					at = scene->getModelVertices(0, v);
					break;
				}
				
				

				glm::vec3 rand = glm::vec3(3, 2, 1);
				glm::vec3 vec_eye_at = at - currentCam->origin_eye;
				glm::vec3 vector_eye_rand = rand - currentCam->origin_eye;
				glm::vec3 up = glm::cross(vec_eye_at, vector_eye_rand) + currentCam->origin_eye;
				
				glm::vec4 up4 = glm::vec4(up.x, up.y, up.z, 1);
				glm::vec4 at4 = glm::vec4(at.x, at.y, at.z, 1);
				glm::vec4 eye4 = glm::vec4((currentCam->origin_eye).x, (currentCam->origin_eye).y, (currentCam->origin_eye).z, 1);
				
				currentCam->SetCameraLookAt(eye4, at4, up4);
			}
		}
	}
	if (ImGui::CollapsingHeader("Viewers")) {
		
		const char* items = getModelNames(scene);
		ImGui::Combo("Model Name", &(scene->activeModelIndex), items, IM_ARRAYSIZE(items));
		std::shared_ptr<MeshModel> m = scene->GetModel(scene->activeModelIndex);
		
		ImGui::ColorEdit3("Model Color", (float*)&(m->color)); // Edit 3 floats representing a color

		if (m != nullptr) {
			// determine the parameters initialize if required from the user: [changing scale graph online]
			handleKeyboardInputs(m);
			// each field is belonging to each mesh model object due to this issue, 
			// we need them public and to referenced always the app is running:

			// as response to y scrolling value we control the zoom in and zoom out world models:
			// handleZoomByYscrolling(&(m->fScale),y_scroll_offset);
			m->SetAllWorldTransformation(y_scroll_offset);

			ImGui::SliderFloat("Scale Object", &(m->fScale), 1.0f, 1000.0f);
			glm::mat4x4 scaling = Trans::getScale4x4(m->fScale);
			ImGui::SliderFloat("Rotate By X [-2PI,2PI]", &(m->fRotatex), -2.2f*M_PI, 2.2f*M_PI);
			glm::mat4x4 xRotateMat = Trans::getxRotate4x4(m->fRotatex);
			ImGui::SliderFloat("Translate By X", &(m->fTranslatex), -1000.0f, 1000.0f);
			glm::mat4x4 xTranslateMat = Trans::getTranslate4x4(0.0f, m->fTranslatex, 0.0f);
			ImGui::SliderFloat("Rotate By Y [-2PI,2PI]", &(m->fRotatey), -2.2f*M_PI, 2.2f*M_PI);
			glm::mat4x4 yRotateMat = Trans::getyRotate4x4(m->fRotatey);
			ImGui::SliderFloat("Translate By Y", &(m->fTranslatey), -1000.0f, 1000.0f);
			glm::mat4x4 yTranslateMat = Trans::getTranslate4x4(0.0f, m->fTranslatey, 0.0f);
			ImGui::SliderFloat("Rotate By Z [-2PI,2PI]", &(m->fRotatez), -2.2f*M_PI, 2.2f*M_PI);
			glm::mat4x4 zRotateMat = Trans::getzRotate4x4(m->fRotatez);
			ImGui::SliderFloat("Translate By Z", &(m->fTranslatez), -1000.0f, 1000.0f);
			glm::mat4x4 zTranslateMat = Trans::getTranslate4x4(0.0f, 0.0f, m->fTranslatez);
			ImGui::Checkbox("Show Face Normals", &(m->showFaceNormals));
			ImGui::ColorEdit3("Face Normal Color", (float*)&(m->fNcolor));
			ImGui::SliderFloat("Face Normal Length", &(m->fNlength), 1.0f, 4*NORMAL_LENGTH);
			ImGui::Checkbox("Show Vectex Normals", &(m->showVertexNormals));
			ImGui::ColorEdit3("Vertex Normal Color", (float*)&(m->vNcolor));
			ImGui::SliderFloat("Vertex Normal Length", &(m->vNlength), 1.0f, 4*NORMAL_LENGTH);
			ImGui::Checkbox("Show Bounding Box", &(m->showBoundingBox));
			ImGui::ColorEdit3("Bounding Box Color", (float*)&(m->BoundingBoxColor));
			

			// transformations to the space:
			glm::mat4x4 resetPosition = Trans::getTranslate4x4(0.0f, 0.0f, 0.0f);
			glm::mat4x4 nextPosition = Trans::getTranslate4x4(m->fTranslatex, m->fTranslatey, m->fTranslatez);
			m->SetWorldTransformation(nextPosition*zRotateMat*yRotateMat*xRotateMat*scaling*resetPosition); //TODO: scaling matrix doesn't effect!!!
		}
	}

	/*ImGui::Text("");
	if (ImGui::Button("Dismiss")) {
		showTransWindow = false;
	}*/
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void buildAboutUsWindow() {
	ImGui::Begin("About Us", &showAboutUsWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	ImVec4 textColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	ImGui::TextColored(textColor,"Student Names: Itay Guy And Elias Jadon");
	ImGui::TextColored(textColor,"Task Count: 1");
	ImGui::TextColored(textColor,"Title: Cameras VS. Viewers");
	ImGui::TextColored(textColor,"Course Semester: Winter 2018");
	ImGui::TextColored(textColor,"Lecturer: Dr. Roi Poranne");
	ImGui::TextColored(textColor,"Institute: University Of Haifa");
	if (ImGui::Button("Close")){ showAboutUsWindow = false; }
	ImGui::End();
}

void loadGrid(Scene& scene) {
	MeshModel k = Utils::LoadGridModel();
	scene.AddModel(std::make_shared<MeshModel>(k));
	glm::vec4 blackColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	scene.GetModel(0)->resetModel(1.0f, blackColor, blackColor, 0.0f, 0.0f);
	scene.gridCounter++;
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene,int y_scroll_offset, const int frameBufferWidth, const int frameBufferHeight){
	if (scene.gridCounter == 0) { loadGrid(scene); }
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showDemoWindow){ ImGui::ShowDemoWindow(&showDemoWindow); }
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	if(showSimpleWindow){
		ImGui::Begin("Task 1 - Cameras VS. Viewers");					// Create a window called "Task 1 - Cameras VS. Views" and append into it.
		ImGui::Checkbox("Transformations Window", &showTransWindow);
		
		ImGui::Checkbox("About Us", &showAboutUsWindow);
		ImGui::End();
	}

	// Show transformations window:
	// Itay's implementation:
	if (showTransWindow) { buildTransformationsWindow(io, &scene, y_scroll_offset ,frameBufferWidth, frameBufferHeight); }
	// Show about us window:
	if (showAboutUsWindow) { buildAboutUsWindow(); }

	// Demonstrate creating a fullscreen menu bar and populating it:
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
		if (ImGui::BeginMainMenuBar()){
			if (ImGui::BeginMenu("File")){
				if (ImGui::MenuItem("Load Model...", "CTRL+O")){
					nfdchar_t *outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
					if (result == NFD_OKAY) {
						MeshModel k = Utils::LoadMeshModel(outPath);
						scene.AddModel(std::make_shared<MeshModel>(Utils::LoadMeshModel(outPath)));
						free(outPath);
					}
					else if (result == NFD_CANCEL) {
					}
					else {
					}

				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}

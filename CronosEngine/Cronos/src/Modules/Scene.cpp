#include "Providers/cnpch.h"
#include "Scene.h"

#include "Application.h"

#include "Filesystem.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/GLRenderer3D.h"
#include "EngineCamera.h"
#include "Input.h"
#include "TextureManager.h"

#include "SDLWindow.h"

#include "GameObject/Components/TransformComponent.h"
#include "GameObject/Components/CameraComponent.h"

#include "mmgr/mmgr.h"

namespace Cronos {

	Scene::Scene(Application* app, bool start_enabled) : Module(app, "Module Scene", start_enabled), m_SceneName("NewScene")
	{}

	Scene::~Scene()
	{}

	// Load assets
	bool Scene::OnStart()
	{
		LOG("Loading Scene");
		bool ret = true;
		App->renderer3D->SetOpenGLSettings();

		//Water Simulation --------------------------------------------------------------------------------
		//Wave Object
		m_Wave = m_CNAssimp_Importer.LoadModel("res/models/waterPlane/waterPlaneOBJ.obj");
		m_Wave->GetComponent<TransformComponent>()->SetPosition({ 0.0f, 2.0f, 0.0f });

		//Water Shader &
		m_WaterShader = new Shader("res/shaders/WaterShader.glsl");

		//Water Material
		m_WaveMaterial = new Material();
		m_WaveMaterial->SetName("Water Wave Material");
		m_WaveMaterial->SetShader(*m_WaterShader);

		//Water Textures & Color
		m_WaveTexture = App->textureManager->CreateTexture("res/models/waterPlane/water1.jpg", TextureType::DIFFUSE);
		m_WaveSpecText = App->textureManager->CreateTexture("res/models/waterPlane/specwater.jpg", TextureType::SPECULAR);
		m_WaveMaterial->SetTexture(m_WaveTexture, TextureType::DIFFUSE);
		m_WaveMaterial->SetTexture(m_WaveSpecText, TextureType::SPECULAR);
		m_WaveMaterial->SetColor(glm::vec4(1.0f));

		(*m_Wave->m_Childs.begin())->GetComponent<MaterialComponent>()->SetMaterial(*m_WaveMaterial);

		//Water timer for movement calc
		m_WaveTimer.Start();

		// ------------------------------------------------------------------------------------------------

		//m_StreetModel = m_CNAssimp_Importer.LoadModel(std::string("res/models/street/stre.FBX"));
		//m_GameObjects.push_back(m_StreetModel);

		ToCopy = nullptr;
		return ret;
	}

	// CleanUp
	bool Scene::OnCleanUp()
	{
		m_Wave->CleanUp();
		m_WaveTexture->~Texture();
		m_WaveSpecText->~Texture();

		LOG("Unloading Intro scene");
		for (auto element : m_GameObjects)
		{
			element->CleanUp();
			RELEASE(element);
		}

		m_GameObjects.clear();

		std::list<Texture*>::iterator it = m_TexturesLoaded.begin();
		while (it != m_TexturesLoaded.end() && *it)
		{
			RELEASE(*it);
			it = m_TexturesLoaded.erase(it);
		}

		m_TexturesLoaded.clear();
		return true;
	}

	// PreUpdate
	update_status Scene::OnPreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	// Update: draw background
	update_status Scene::OnUpdate(float dt)
	{

		//------------------------------------------------------------------------------------------------------------------------------------
		//---------------------------------- WAVE UPDATE -------------------------------------------------------------------------------------

	/*	GameObject* WaveMesh = (*m_Wave->m_Childs.begin());
		m_WaterShader->Bind();

		// Wave Calculations ----------------
		float maxT = 180.0f;
		m_WaterShader->SetUniform1f("u_Time", m_WaveTimer.ReadSec());
		m_WaterShader->SetUniform1f("u_MaxTime", maxT);
		m_WaterShader->SetUniform1f("u_Amplitude", 5.0f);
		m_WaterShader->SetUniform1f("u_WaveLength", 35.0f);
		m_WaterShader->SetUniform1f("u_Velocity", 15.0f);
		m_WaterShader->SetUniform1f("u_FOAMVelocity", 5.0f);
		m_WaterShader->SetUniform1f("u_ColorGradingOffset", 0.1f); //Fragment Shader
		m_WaterShader->SetUniformVec2f("u_FOAMDirection", glm::vec2(1.0f, 1.0f));
		m_WaterShader->SetUniform1f("u_WaveMovementMultiplicator", 2.0f);

		if (m_WaveTimer.ReadSec() >= maxT)
			m_WaveTimer.Start();

		// Shader Stuff ---------------------
		m_WaterShader->SetUniformMat4f("u_View", App->engineCamera->GetViewMatrix());
		m_WaterShader->SetUniformMat4f("u_Proj", App->engineCamera->GetProjectionMatrix());
		m_WaterShader->SetUniformMat4f("u_Model", WaveMesh->GetComponent<TransformComponent>()->GetGlobalTranformationMatrix());

		MaterialComponent* material = WaveMesh->GetComponent<MaterialComponent>();
		VertexArray* VAO = WaveMesh->GetComponent<MeshComponent>()->GetVAO();

		//Binding ----------------------------
		if (material != nullptr)
			material->Bind();
		VAO->Bind();
		material->SetColor({ 1.0f, 1.0f, 1.0f, 0.8f });

		//Drawing ----------------------------
		glDrawElements(GL_TRIANGLES, VAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

		//Unbinding --------------------------
		if (material != nullptr)
			material->Unbind();
		VAO->UnBind();
		m_WaterShader->Unbind();

		//------------------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------------------
		*/
		//Game Objects update
		for (auto element : m_GameObjects)
			element->Update(dt);

		//Creating Camera
		if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN && App->EditorGUI->GetCurrentGameObject() != nullptr && App->EditorGUI->GetCurrentGameObject()->GetComponent<CameraComponent>() != nullptr)
			App->renderer3D->SetRenderingCamera(*App->EditorGUI->GetCurrentGameObject()->GetComponent<CameraComponent>()->GetCamera());
		else if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
			App->renderer3D->SetRenderingCamera(*App->engineCamera->GetCamera());

		//Copy & Paste
		if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN) {
			if (App->EditorGUI->GetCurrentGameObject() != nullptr) {
				ToCopy = App->EditorGUI->GetCurrentGameObject();
			}
		}

		//Axis Gizmo modification
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && App->input->GetMouseButton(1) != KEY_REPEAT) {
			guizmo_operation = ImGuizmo::TRANSLATE;
		}
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) {
			guizmo_operation = ImGuizmo::ROTATE;
		}
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) {
			guizmo_operation = ImGuizmo::SCALE;
		}

		if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN) {
			if (App->EditorGUI->GetCurrentGameObject() != nullptr && ToCopy != nullptr) {

				GameObject* NewGO = App->filesystem->Load(ToCopy->GetGOID());


				NewGO->SetNewID();
				if (App->EditorGUI->GetCurrentGameObject()->GetParentGameObject() == nullptr) {
					NewGO->SetParent(App->EditorGUI->GetCurrentGameObject());
					App->EditorGUI->GetCurrentGameObject()->m_Childs.push_back(NewGO);
				}
				else {
					NewGO->SetParent(App->EditorGUI->GetCurrentGameObject()->GetParentGameObject());
					App->EditorGUI->GetCurrentGameObject()->m_Childs.push_back(NewGO);
				}
				App->filesystem->SaveOwnFormat(App->EditorGUI->GetCurrentGameObject());
				//ToCopy = nullptr;
			}
		}

		return UPDATE_CONTINUE;
	}

	// PostUpdate
	update_status Scene::OnPostUpdate(float dt)
	{
		if (mustCleanScene) {
			if(App->EditorGUI->GetCurrentGameObject()!=nullptr)
				App->EditorGUI->CancelGameObject();
			if(ToCopy!=nullptr)
				ToCopy = nullptr;
			OnCleanUp();
			mustCleanScene = false;
		}
		if (mustSave) {
			SaveScene(m_SceneName.c_str());
			mustSave = false;
		}
		if (mustLoad) {
			LoadScene(m_SceneName.c_str());
			mustLoad = false;
		}
		return UPDATE_CONTINUE;
	}

	//----------------------------------------------------------------------------
	GameObject* Scene::CreateModel(const char* path)
	{
		return m_CNAssimp_Importer.LoadModel(path);
	}

	//----------------------------------------------------------------------------
	bool Scene::SaveScene(const char * SceneName)
	{
		std::string Path = App->filesystem->GetScenePath();
		Path += SceneName;
		Path += ".scene";

		json sceneFile;
		sceneFile["Name"] = SceneName;
		sceneFile["Path"] = Path;
		sceneFile["TotalofRootGO"] = m_GameObjects.size();
		int index=0;
		for (auto& Go : m_GameObjects) {
			App->filesystem->SaveOwnFormat(Go);
			sceneFile["ParentGOID"][index] = Go->GetGOID();
			index++;
		}

		std::ofstream OutputFile_Stream{ Path,std::ofstream::out };

		OutputFile_Stream << std::setw(2) << sceneFile;

		OutputFile_Stream.close();

		return false;
	}

	bool Scene::LoadScene(const char* SceneName)
	{
		std::string Path = App->filesystem->GetScenePath();
		Path += SceneName;
		Path += ".scene";
		bool exists = std::filesystem::exists(Path);

		if (exists)
		{
			App->renderer3D->ResetTree();

			App->EditorGUI->CancelGameObject();
			ToCopy = nullptr;
			OnCleanUp();

			std::ifstream file{ Path.c_str() };
			if (file.is_open())
			{
				json LoadSceneFile=json::parse(file);
				uint numofRootGo = LoadSceneFile["TotalofRootGO"].get<uint>();

				for (int i = 0; i < numofRootGo; i++)
				{
					int id = LoadSceneFile["ParentGOID"][i].get<int>();
					GameObject* ret = App->filesystem->Load(id);
					m_GameObjects.push_back(ret);
				}
			}

			return true;
		}

		LOG("Couldn't Found any Scene with that path! Scene not loaded", Path.c_str());
		return false;
	}

	void Scene::DrawGuizmo(Camera* camera, GameObject* go)
	{
		glm::mat4 ViewMatrix = camera->GetViewMatrix();
		glm::mat4 ProjMatrix = camera->GetProjectionMatrix();

		ImGuizmo::BeginFrame();
		ImGuizmo::Enable(true);

		glm::mat4 model = go->GetComponent<TransformComponent>()->GetGlobalTranformationMatrix();
		glm::mat4 delta;

		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(-200, -200, (float)App->window->GetWidth(), (float)App->window->GetHeight());


		ImGuizmo::SetDrawlist();
		ImGuizmo::Manipulate((const float*)&ViewMatrix, (const float*)&ProjMatrix, guizmo_operation, guizmo_mode, (float*)&model, (float*)&delta);

		glm::mat4 identity(1.0f);

		if (ImGuizmo::IsUsing() && delta != identity)
		{
			float matrixTranslation[3], matrixRotation[3], matrixScale[3];
			ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(model), matrixTranslation, matrixRotation, matrixScale);

			/*float matrixTranslation[3], matrixRotation[3], matrixScale[3];
			ImGuizmo::DecomposeMatrixToComponents(gizmoMatrix.m16, matrixTranslation, matrixRotation, matrixScale);
			ImGui::InputFloat3("Tr", matrixTranslation, 3);
			ImGui::InputFloat3("Rt", matrixRotation, 3);
			ImGui::InputFloat3("Sc", matrixScale, 3);*/

			glm::vec3 Rotation = glm::vec3(matrixRotation[0], matrixRotation[1], matrixRotation[2]);
			go->GetComponent<TransformComponent>()->SetPosition(glm::vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]));
			go->GetComponent<TransformComponent>()->SetScale(glm::vec3(matrixScale[0], matrixScale[1], matrixScale[2]));
			go->GetComponent<TransformComponent>()->Rotate(glm::vec3(matrixRotation[0], matrixRotation[1], matrixRotation[2]));

		}

		//Rick Code

		//if (ImGuizmo::IsUsing() && !delta.IsIdentity())
		//{
		//	model.Transpose();
		//	if (go->GetParent() == nullptr)
		//	{
		//		go->SetLocalTransform(model);
		//	}
		//	else
		//	{
		//		float4x4 parent = go->GetParent()->GetGlobalTransformation();
		//		parent.InverseOrthonormal();
		//		go->SetLocalTransform(parent*model);
		//	}
		//}

		//float3 points[8];
		//go->global_bbox.GetCornerPoints(points);
		//std::swap(points[2], points[5]);
		//std::swap(points[3], points[4]);
		//std::swap(points[4], points[5]);
		//std::swap(points[6], points[7]);
		//dd::box(points, dd::colors::Yellow);
	}
}

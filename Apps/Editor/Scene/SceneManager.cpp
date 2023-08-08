#include "SceneManager.h"
#include <fstream>
#include <Widgets/ProgressBar.h>
#include <iostream>
SceneManager g_sceneManager;
SceneManager::SceneManager(QObject* parent) {}

SceneManager::~SceneManager() {}

void SceneManager::slot_openMesh(const boost::filesystem::path& path_)
{
    soarscape::Mesh mesh;

    ProgressBar::ThreadFunction fnc = [&]() {
        std::cout << "loading " << path_.generic_string() << std::endl;
        mesh.read(path_);
        m_meshs.push_back(mesh);
        g_progress_percentage = 100;
        g_done                = true;
        std::cout << "loaded"<< std::endl;
    };
    ProgressBar progressBar(fnc);
    progressBar.exec();
    mesh.updateRenderData();
}

bool SceneManager::EditorStatement::s_CurrentState = false;

void SceneManager::EditorStatement::SetCurrentState(bool state)
{
    s_CurrentState = state;
}

bool SceneManager::EditorStatement::GetCurrentState()
{
    return s_CurrentState;
}
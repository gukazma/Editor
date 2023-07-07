#include "SceneManager.h"
#include <fstream>
SceneManager g_sceneManager;
SceneManager::SceneManager(QObject* parent) {}

SceneManager::~SceneManager() {}

void SceneManager::slot_openMesh(const boost::filesystem::path& path_)
{
    soarscape::Mesh mesh;
    mesh.read(path_);
    m_meshs.push_back(mesh);
}

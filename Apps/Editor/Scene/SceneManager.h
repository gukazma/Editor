#pragma once
#include <QObject>
#include <boost/filesystem/path.hpp>
#include <Datas/Mesh.h>
namespace soarscape
{
class Mesh;
}
class SceneManager : public QObject
{
	Q_OBJECT
public:
    explicit SceneManager(QObject* parent = nullptr);
    ~SceneManager();

    std::vector<::soarscape::Mesh> m_meshs;
public slots:
    void slot_openMesh(const boost::filesystem::path&);
};

extern SceneManager g_sceneManager;
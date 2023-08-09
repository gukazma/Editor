#pragma once
#include <QObject>
#include <boost/filesystem/path.hpp>
#include <Datas/Mesh.h>
#include <memory>
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

    std::vector<std::shared_ptr<::soarscape::Mesh>> m_meshs;

    class EditorStatement
    {
    public:
        static bool  s_CurrentState;
        static void  SetCurrentState(bool state);
        static bool  GetCurrentState();
    };

public slots:
    void slot_openMesh(const boost::filesystem::path&);
};

extern SceneManager g_sceneManager;
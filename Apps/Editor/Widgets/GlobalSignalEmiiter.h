#pragma once
#include <QObject>
#include <boost/filesystem.hpp>
#include <functional>
#include <string>
class GlobalSignalEmiiter : public QObject
{
    Q_OBJECT
public:
    explicit GlobalSignalEmiiter(QObject* parent = nullptr);
    ~GlobalSignalEmiiter() = default;

Q_SIGNALS:
    void signal_openMesh(const boost::filesystem::path&);
};

extern GlobalSignalEmiiter g_globalSignalEmitter;
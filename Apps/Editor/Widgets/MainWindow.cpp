#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "QRibbon.h"
#include "GlobalSignalEmiiter.h"
#include "Scene/SceneManager.h"
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QRibbon::install(this);
    initConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConnect() {
    connect(&g_globalSignalEmitter,
            SIGNAL(signal_openMesh(const boost::filesystem::path&)),
            &g_sceneManager,
            SLOT(slot_openMesh(const boost::filesystem::path&)));
    connect(ui->actionSelectFace,
            &QAction::triggered,
            this,
            &MainWindow::on_actionSelectFace_triggered);
}

void MainWindow::on_actionImportMesh_triggered()
{
    auto result = QFileDialog::getOpenFileName(this, tr("Open mesh"), "", "mesh (*.ply *.osgb)");
    if (!result.isEmpty()) {
        emit g_globalSignalEmitter.signal_openMesh(result.toLocal8Bit().constData());
    }
}

void MainWindow::on_actionSelectFace_triggered()
{
    if (ui->actionSelectFace->isChecked()) {
        SceneManager::EditorStatement::SetCurrentState(true);
        if (QEvent::MouseButtonPress == true) {

        }
    }
    else {
        SceneManager::EditorStatement::SetCurrentState(false);
    }
}
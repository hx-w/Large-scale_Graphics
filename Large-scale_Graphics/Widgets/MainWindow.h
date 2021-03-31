#pragma once
#include <QtWidgets/QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include "GLWidget.h"



QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMenuBar;
class QSessionManager;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
	void loadObj(const QString fileName);
protected:
	void closeEvent(QCloseEvent *event)override;
private slots:
	void importObj();
	// graphicsWasModified
private:
	void createActions();
	void createStatusBar();
	void setCurrentFile(const QString &fileName);

	QString curFile;
	GLWidget *glWidget;
};

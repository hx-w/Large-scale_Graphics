
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

	setMinimumSize(860, 860);
	
	createActions();
	createStatusBar();

	setCurrentFile(QString());
	
	glWidget = new GLWidget("Res/custom/plane.obj");
	setCentralWidget(glWidget);

	setCurrentFile(QString());
    
}
void MainWindow::importObj() {
	QString fileName = QFileDialog::getOpenFileName(this);
	qDebug ()<< fileName << endl;
	if (!fileName.isEmpty())
		loadObj(fileName);
	
}
void MainWindow::loadObj(const QString fileName) {
	if (!centralWidget()) {
		glWidget = new GLWidget(fileName);
		setCentralWidget(glWidget);

		qDebug() << "load model success! ";
	}
	else {
		qDebug() << "exist central widget!";
		//若加载新模型
	}
	
	
}

void MainWindow::closeEvent(QCloseEvent *event) {

}

void MainWindow::createActions() {
	//File Menu
	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	QAction *pObj = fileMenu->addAction("import obj");
	pObj->setStatusTip(tr("import obj"));

	connect(pObj, &QAction::triggered, this, &MainWindow::importObj);
}

void MainWindow::createStatusBar() {
	statusBar()->showMessage(tr("Ready"));
}

void MainWindow::setCurrentFile(const QString &fileName) {

}

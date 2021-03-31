#pragma once

#include <iostream>
#include <QWidget>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <qopengldebug.h>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>
#include "../Loaders/OBJLoader.h"


class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
	Q_OBJECT

public:
	GLWidget(const QString fileName,QWidget *parent = Q_NULLPTR);
	~GLWidget();
	 void GLClearError();
	 bool GLCheckError(const char* function, const char* file, int line);
	 QSize minimumSizeHint() const override;
	 QSize sizeHint() const override;

public slots:
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);
	void cleanup();

signals:
	void xRotationChanged(int angle);
	void yRotationChanged(int angle);
	void zRotationChanged(int angle);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
private:
	//static bool m_transparent;
	QVector3D cameraLocation, lightLocation;
	qreal angleX=0, angleY=0, anglZ=0;
	QMatrix4x4 vMatrix, pMatrix, mMatrix;
	QVector<float> vertPoints;
	QOpenGLShaderProgram *program;
	QString fileName;

	QPoint mouseLastPos;
};
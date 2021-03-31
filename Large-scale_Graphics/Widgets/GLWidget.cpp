#include "glWidget.h"

static GLuint VBO, VAO, EBO;

#define ASSERT(x) if (!x) __debugbreak();
#define GLCall(x) GLClearError();\
		x;\
		ASSERT(GLCheckError(#x,__FILE__,__LINE__))

QSize GLWidget::minimumSizeHint() const {
	return QSize(50, 50);
}

QSize GLWidget::sizeHint() const {
	return QSize(400, 400);
}

static void qNormalizeAngle(int &angle) {
	while (angle < 0)
		angle += 360 * 16;
	while (angle > 360 * 16)
		angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle) {
	qNormalizeAngle(angle);
	if (angle != angleX) {
		angleX = angle;
		emit xRotationChanged(angle);
		update();
	}
}
void GLWidget::setYRotation(int angle) {
	qNormalizeAngle(angle);
	if (angle != angleY) {
		angleY = angle;
		emit yRotationChanged(angle);
		update();
	}
}
void GLWidget::setZRotation(int angle) {
	qNormalizeAngle(angle);
	if (angle != anglZ) {
		anglZ = angle;
		emit zRotationChanged(angle);
		update();
	}
}

void GLWidget::cleanup() {
	if (program == nullptr)
		return;
	makeCurrent();
	GLCall(glDeleteVertexArrays(1, &VAO));
	GLCall(glDeleteBuffers(1, &VBO));
	delete program;
	program = 0;
	doneCurrent();
}



GLWidget::GLWidget(const QString fileName,QWidget *parent)
	: QOpenGLWidget(parent) {
	this->fileName = fileName;
}

GLWidget::~GLWidget() {
	cleanup();
}

void GLWidget::GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLWidget::GLCheckError(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error](0x" << std::hex << error << std::dec << ")" << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}


void GLWidget::initializeGL() {
	this->initializeOpenGLFunctions();
	program = new QOpenGLShaderProgram;
	bool success = program->addShaderFromSourceFile(QOpenGLShader::Vertex, "Res/vsrc.vert");
	if (!success) {
		qDebug() << "shaderProgram addShaderFromSourceFile failed!" << program->log();
		return;
	}
	success = program->addShaderFromSourceFile(QOpenGLShader::Fragment, "Res/fsrc.frag");
	if (!success) {
		qDebug() << "shaderProgram addShaderFromSourceFile failed!" << program->log();
		return;
	}
	success = program->link();
	if (!success) {
		qDebug() << "shaderProgram link failed!" << program->log();
	}
	qDebug() << "shaderProgram read success!!!";

	OBJLoader  *_objLoader = new OBJLoader();
	_objLoader->load(this->fileName, vertPoints);
	qDebug() << vertPoints.count();

	//get id
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &VBO));
	//1.bind vertex array object,
	GLCall(glBindVertexArray(VAO));

	//2.then bind and set vertex buffers
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, vertPoints.size() * sizeof(GL_FLOAT), &vertPoints[0], GL_STATIC_DRAW));//顶点数据复制到缓冲

	//3.and then configure vertex attributes
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));


	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));//取消VBO绑定
	GLCall(glBindVertexArray(0));   //取消VAO绑定
	GLCall(cameraLocation.setX(0));
	GLCall(cameraLocation.setY(0));
	GLCall(cameraLocation.setZ(98));
	GLCall(vMatrix.lookAt(cameraLocation, QVector3D(0.0, 0.0,0.0), QVector3D(0.0, -1.0, 0.0)));
	
}

void GLWidget::resizeGL(int w, int h) {
	GLCall(pMatrix.setToIdentity());
	GLCall(pMatrix.perspective(45, float(w) / h, 0.01f, 100.0f));

}

void GLWidget::paintGL() {
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	

	GLCall(program->bind());
	GLCall(glBindVertexArray(VAO));
	mMatrix.setToIdentity();
	mMatrix.rotate(180.0f - (angleX / 16.0f), 1, 0, 0);
	mMatrix.rotate(angleY / 16.0f, 0, 1, 0);
	mMatrix.rotate(anglZ / 16.0f, 0, 0, 1);
	GLCall(program->setUniformValue("uPMatrix", pMatrix));
	GLCall(program->setUniformValue("uVMatrix", vMatrix));
	GLCall(program->setUniformValue("uMMatrix", mMatrix));
	int vertexcount = vertPoints.size() / 3;
	
	GLCall(glDrawArrays(GL_LINES, 0, vertexcount));
	//glDrawArrays(GL_TRIANGLES, 0, 3);

}

void GLWidget::mousePressEvent(QMouseEvent *event) {
	mouseLastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
	int dx = event->x() - mouseLastPos.x();
	int dy = event->y() - mouseLastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		setXRotation(angleX + 8 * dy);
		setYRotation(angleY + 8 * dx);
	}
	else if (event->buttons() & Qt::RightButton) {
		setXRotation(angleX + 8 * dx);
		setZRotation(angleY + 8 * dy);
	}
	mouseLastPos = event->pos();
}

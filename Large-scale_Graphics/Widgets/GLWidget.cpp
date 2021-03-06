#include "glWidget.h"

#include <string>
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
		// emit xRotationChanged(angle);
		update();
	}
}
void GLWidget::setYRotation(int angle) {
	qNormalizeAngle(angle);
	if (angle != angleY) {
		angleY = angle;
		//emit yRotationChanged(angle);
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


	//--------------------- bsp works ---------------------
	BsplineMethod mybsp;
	mybsp.init(3, 4);
	auto xyz = mybsp.split(vertPoints);
	mybsp.dump("../../cpp/oorigin.txt", xyz[0]);
	vertPoints = mybsp.load("Res/models/4_little_plane_1.txt");
//	mybsp.dump("cache.txt", vertPoints);
	// vertPoints = mybsp.load("cache.txt");
	for (int idx = 0; idx < 3; idx++) {
//		xyz[idx] = mybsp.gauss_filter(xyz[idx]);
		xyz[idx] = mybsp.down_sampling(xyz[idx]);
		xyz[idx] = mybsp.up_sampling_bsp(xyz[idx]);
	}
	//mybsp.dump("../../cpp/bsp.txt", xyz[0]);
//	vertPoints = mybsp.join(xyz);
	/*
	for (int m = 2; m <= 8; m++) {
		mybsp.init(3, m);
		auto xyz = mybsp.split(vertPoints);
		// origin
		for (int idx = 0; idx < 3; idx++) {
			xyz[idx] = mybsp.down_sampling(xyz[idx]);
			xyz[idx] = mybsp.up_sampling_bsp(xyz[idx]);
		}
		auto origin = mybsp.join(xyz);
		
		std::string filename = std::string("Res/models/") + char('0' + m) + "_little_plane_1.txt";
		auto vs = mybsp.load(filename);
		auto xyz2 = mybsp.split(vs);
		for (int idx = 0; idx < 3; idx++) {
			xyz2[idx] = mybsp.down_sampling(xyz2[idx]);
			xyz2[idx] = mybsp.up_sampling_bsp(xyz2[idx]);
		}
		auto lowp = mybsp.join(xyz2);
		

		float LSD1 = mybsp.LSD(origin, vertPoints);
		float LSD2 = mybsp.LSD(lowp, vertPoints);
		float LSD = mybsp.LSD(lowp, origin);
		//qDebug() << "[m=" << m << "] " << LSD1 << "     " << LSD2;
		qDebug() << "[m=" << m << "] " << LSD;
	}
	*/
	qDebug() <<"total vertex number: " << vertPoints.count();
	//-------------------------------------------------------

	//get id
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &VBO));
	//1.bind vertex array object,
	GLCall(glBindVertexArray(VAO));

	//2.then bind and set vertex buffers
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, vertPoints.size() * sizeof(GL_FLOAT), &vertPoints[0], GL_STATIC_DRAW));//??????????????????

	//3.and then configure vertex attributes
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));


	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));//????VBO????
	GLCall(glBindVertexArray(0));   //????VAO????
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
	angleX = 1584;
	angleY = 8;
//	scale_factor = 0.09;
	x_trans = 5;
	y_trans = -299;

	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	GLCall(program->bind());
	GLCall(glBindVertexArray(VAO));
	mMatrix.setToIdentity();
	mMatrix.translate(0.1 * x_trans, 0.1 * y_trans, 0);
	mMatrix.rotate(angleX / 16.0f, 1, 0, 0); 
	mMatrix.rotate(angleY / 16.0f, 0, 1, 0);
	mMatrix.rotate(0, 0, 0, 1);
	//scale_factor = 0.2;
	mMatrix.scale(scale_factor);
	GLCall(program->setUniformValue("uPMatrix", pMatrix));
	GLCall(program->setUniformValue("uVMatrix", vMatrix));
	GLCall(program->setUniformValue("uMMatrix", mMatrix));
	int vertexcount = vertPoints.size() / 3;
	
	GLCall(glDrawArrays(GL_POINTS, 0, vertexcount));
	//glDrawArrays(GL_LINE_STRIP, 0, vertexcount);
	qDebug() << "angleX: " << angleX << " angleY: " << angleY << " scale: " << scale_factor << " xtrans: " << x_trans << " ytrans: " << y_trans;

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
		x_trans += dx;
		y_trans += dy;
		update();
		//setXRotation(angleX + 8 * dx);
		//setZRotation(angleY + 8 * dy);
	}
	mouseLastPos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
	QPoint numPixels, numDegrees;
	numPixels = event->pixelDelta();
	numDegrees = event->angleDelta() / 8;

	int step = 0;
	if (!numPixels.isNull()) {
		step = numPixels.y();
	}
	else if (!numDegrees.isNull()) {
		QPoint numSteps = numDegrees / 15;
		step = numSteps.y();
	}
	scale_factor += step * scale_transform_tick;
	if (scale_factor < 0) scale_factor = 0;
	update();
}

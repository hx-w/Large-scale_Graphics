#pragma once

#include <QObject>
#include <QVector>
class OBJLoader : public QObject {
	Q_OBJECT

public:
	OBJLoader(QObject *parent= Q_NULLPTR);
	~OBJLoader();
	bool load(const QString fileName, QVector<float> &vPoints);
};

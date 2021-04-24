#pragma once

#include <QObject>
#include <QVector>

#include "../utils/EulerGraph.h"


class OBJLoader : public QObject {
	Q_OBJECT

public:
	OBJLoader(QObject *parent= Q_NULLPTR);
	~OBJLoader();
	bool load(const QString fileName, QVector<float> &vPoints);
	bool load_new(const QString fileName, QVector<float> &vPoints);
};

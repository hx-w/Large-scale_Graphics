#include "OBJLoader.h"

#include <QFile>
#include <QDebug>
OBJLoader::OBJLoader(QObject *parent)
	: QObject(parent) {
}

OBJLoader::~OBJLoader() {
}

bool OBJLoader::load(const QString fileName, QVector<float> &vPoints) {
	if (fileName.mid(fileName.lastIndexOf('.')) != ".obj" && fileName.mid(fileName.lastIndexOf('.')) != ".OBJ") {
		qDebug() << "file is not a obj file!";
		return false;
	}
	QFile objfile(fileName);
	if (!objfile.open(QIODevice::ReadOnly)) {
		qDebug() << "open" << fileName << "failed";
		return false;
	}
	else {
		qDebug() << "open" << fileName << "success!";
	}
	QVector<float> vertextPoints;
	QVector<std::tuple< int, int>> facesIndexs;

	while (!objfile.atEnd()) {
		QByteArray lineData = objfile.readLine();

		lineData = lineData.remove(lineData.count() - 2, 2);
		// qDebug() << lineData;

		if (lineData == "")
			continue;
		QList<QByteArray> strValues = lineData.split(' ');
		strValues.removeAll("");
		QString dataType = strValues.takeFirst();
		if (dataType == "v") {
			for (int i = 0; i < strValues.count(); i++) {
				if (strValues[i] != "")
					vertextPoints.push_back(strValues[i].toFloat());
			}

		}

		else if (dataType == "f") {
			if (strValues.size() == 4) {
				continue;
				strValues.push_back(strValues.at(0));
				strValues.push_back(strValues.at(2));
			}
			std::transform(strValues.begin(), strValues.end(), std::back_inserter(facesIndexs), [](QByteArray &str) {
				QList<QByteArray> intStr = str.split('/');
				auto tmp = std::make_tuple(intStr.first().toInt(), intStr.last().toInt());
				return tmp;
			});

		}

	}
	if (vertextPoints.count() != 0) {
		qDebug() << "vertex points: " << vertextPoints.count();
	}
	else {
		qDebug() << "none vertex points";
		return false;
	}


	if (facesIndexs.count() != 0) {
		qDebug() << "face points: " << facesIndexs.count();
	}
	else {
		qDebug() << "none faces";
		return false;
	}
	
	/*
	for (auto &verFaceInfo : facesIndexs) {
		int vIndex = std::get<0>(verFaceInfo);

		// int vPointSizes = vertextPoints.count() / 3;
		//�������������
		vPoints << vertextPoints.at(vIndex * 3 - 3);
		vPoints << vertextPoints.at(vIndex * 3 - 2);
		vPoints << vertextPoints.at(vIndex * 3 - 1);
	}
	*/
	QMap<int, QSet<int>> stat;
	QVector<int> temp_index;
	for (auto &verFaceInfo : facesIndexs) {
		int vIndex = std::get<0>(verFaceInfo);
		temp_index.append(vIndex);
		if (temp_index.count() == 3) {
			stat[temp_index[0]].insert(temp_index[1]);
			stat[temp_index[0]].insert(temp_index[2]);
			stat[temp_index[1]].insert(temp_index[0]);
			stat[temp_index[1]].insert(temp_index[2]);
			stat[temp_index[2]].insert(temp_index[1]);
			stat[temp_index[2]].insert(temp_index[0]);


			vPoints << vertextPoints.at(temp_index[0] * 3 - 3);
			vPoints << vertextPoints.at(temp_index[0] * 3 - 2);
			vPoints << vertextPoints.at(temp_index[0] * 3 - 1);

			vPoints << vertextPoints.at(temp_index[1] * 3 - 3);
			vPoints << vertextPoints.at(temp_index[1] * 3 - 2);
			vPoints << vertextPoints.at(temp_index[1] * 3 - 1);

			vPoints << vertextPoints.at(temp_index[1] * 3 - 3);
			vPoints << vertextPoints.at(temp_index[1] * 3 - 2);
			vPoints << vertextPoints.at(temp_index[1] * 3 - 1);

			vPoints << vertextPoints.at(temp_index[2] * 3 - 3);
			vPoints << vertextPoints.at(temp_index[2] * 3 - 2);
			vPoints << vertextPoints.at(temp_index[2] * 3 - 1);

			vPoints << vertextPoints.at(temp_index[0] * 3 - 3);
			vPoints << vertextPoints.at(temp_index[0] * 3 - 2);
			vPoints << vertextPoints.at(temp_index[0] * 3 - 1);

			vPoints << vertextPoints.at(temp_index[2] * 3 - 3);
			vPoints << vertextPoints.at(temp_index[2] * 3 - 2);
			vPoints << vertextPoints.at(temp_index[2] * 3 - 1);
			temp_index.clear();
		}
	}

	vertextPoints.clear();
	facesIndexs.clear();

	objfile.close();
	return true;
}

bool OBJLoader::load_new(const QString fileName, QVector<float> &vPoints) {
	if (fileName.mid(fileName.lastIndexOf('.')) != ".obj" && fileName.mid(fileName.lastIndexOf('.')) != ".OBJ") {
		qDebug() << "file is not a obj file!";
		return false;
	}
	QFile objfile(fileName);
	if (!objfile.open(QIODevice::ReadOnly)) {
		qDebug() << "open" << fileName << "failed";
		return false;
	}
	else {
		qDebug() << "open" << fileName << "success!";
	}
	QVector<float> vertextPoints;

	while (!objfile.atEnd()) {
		QByteArray lineData = objfile.readLine();

		lineData = lineData.remove(lineData.count() - 2, 2);
		// qDebug() << lineData;

		if (lineData == "")
			continue;
		QList<QByteArray> strValues = lineData.split(' ');
		strValues.removeAll("");
		QString dataType = strValues.takeFirst();
		if (dataType == "v") {
			for (int i = 0; i < strValues.count(); i++) {
				if (strValues[i] != "")
					vertextPoints.push_back(strValues[i].toFloat());
			}

		}

		else if (dataType == "f") {
			if (strValues.count() != 3)
				continue;

			vPoints << vertextPoints.at(strValues[0].toInt() * 3 - 3);
			vPoints << vertextPoints.at(strValues[0].toInt() * 3 - 2);
			vPoints << vertextPoints.at(strValues[0].toInt() * 3 - 1);

			vPoints << vertextPoints.at(strValues[1].toInt() * 3 - 3);
			vPoints << vertextPoints.at(strValues[1].toInt() * 3 - 2);
			vPoints << vertextPoints.at(strValues[1].toInt() * 3 - 1);

			vPoints << vertextPoints.at(strValues[0].toInt() * 3 - 3);
			vPoints << vertextPoints.at(strValues[0].toInt() * 3 - 2);
			vPoints << vertextPoints.at(strValues[0].toInt() * 3 - 1);

			vPoints << vertextPoints.at(strValues[2].toInt() * 3 - 3);
			vPoints << vertextPoints.at(strValues[2].toInt() * 3 - 2);
			vPoints << vertextPoints.at(strValues[2].toInt() * 3 - 1);

			vPoints << vertextPoints.at(strValues[2].toInt() * 3 - 3);
			vPoints << vertextPoints.at(strValues[2].toInt() * 3 - 2);
			vPoints << vertextPoints.at(strValues[2].toInt() * 3 - 1);

			vPoints << vertextPoints.at(strValues[1].toInt() * 3 - 3);
			vPoints << vertextPoints.at(strValues[1].toInt() * 3 - 2);
			vPoints << vertextPoints.at(strValues[1].toInt() * 3 - 1);
		}

	}
	if (vertextPoints.count() != 0) {
		qDebug() << "vertex points: " << vertextPoints.count();
	}
	else {
		qDebug() << "none vertex points";
		return false;
	}

	vertextPoints.clear();

	objfile.close();
	return true;
}


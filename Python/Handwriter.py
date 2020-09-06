# This Python file uses the following encoding: utf-8
from PyQt5 import QtCore
from PyQt5 import QtWidgets
from PyQt5 import QtGui

from PyQt5.QtCore import Qt, QSize
from PyQt5.QtWidgets import QWidget
from PyQt5.QtGui import QImage, QTransform, QPainter, QPen


class Handwriter(QWidget):
    def __init__(self):
        QWidget.__init__(self)
        self.image = QImage(QSize(280, 280), QImage.Format_RGB32)
        self.clearImage()
        self.activate = False

    def isModified(self):
        return self.modified

    def clearImage(self):
        self.image.fill(Qt.black)
        self.update()
        self.modified = False

    def saveImage(self):
        if self.modified:
            resizedImage = self.image.scaled(28, 28, Qt.KeepAspectRatio)
            resizedImage.save("query.pgm", "PGM")

    def mousePressEvent(self, event):
        if event.button() == Qt.LeftButton:
            self.lastPoint = event.pos()
            self.activate = True

    def mouseMoveEvent(self, event):
        if self.activate:
            self.drawLineTo(event.pos())

    def mouseReleaseEvent(self, event):
        if event.button() == Qt.LeftButton and self.activate:
            self.drawLineTo(event.pos())
            self.activate = False

    def paintEvent(self, event):
        painter = QPainter(self)
        rect = self.rect()
        painter.drawImage(rect, self.image, rect)

    def drawLineTo(self, endPoint):
        painter = QPainter(self.image)
        painter.setPen(QPen(Qt.white, 30, Qt.SolidLine, Qt.RoundCap, Qt.RoundJoin))
        painter.drawLine(self.lastPoint, endPoint)
        self.modified = True
        self.update()
        self.lastPoint = endPoint

    def __del__(self):
        del self.image

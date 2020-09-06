# This Python file uses the following encoding: utf-8
from PyQt5 import QtWidgets

from PyQt5.QtWidgets import QApplication, QWidget

class Model:
    def __init__(self, parent):
        self.parent = parent
        self.queryPath = QApplication.applicationDirPath() + "/query.pgm"

    def query(self):
        pass

    def closeEvent(self, event):
        super(QWidget, self).closeEvent(event)

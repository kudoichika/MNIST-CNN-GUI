# This Python file uses the following encoding: utf-8
from PyQt5 import QtGui
from PyQt5 import QtWidgets

from Handwriter import Handwriter
from Model import Model

from PyQt5.QtCore import Qt
from PyQt5.QtGui import QPalette, QColor
from PyQt5.QtWidgets import QWidget
from PyQt5.QtWidgets import QPushButton, QHBoxLayout, QVBoxLayout
from PyQt5.QtWidgets import QSizePolicy, QLabel, QSpacerItem


class MainWindow(QWidget):
    def __init__(self):
        QtWidgets.QWidget.__init__(self)

        self.handwriter = Handwriter()
        self.model = Model(self)
        self.clearButton = QPushButton('Clear')
        self.doneButton = QPushButton('Interpret')

        self.clearButton.clicked.connect(self.handwriter.clearImage)
        self.doneButton.clicked.connect(self.imageRecognition)

        actionLayout = QHBoxLayout()
        actionLayout.addWidget(self.clearButton)
        actionLayout.addWidget(self.doneButton)

        leftLayout = QVBoxLayout()
        leftLayout.addWidget(self.handwriter)
        leftLayout.addLayout(actionLayout)

        rightLayout = QVBoxLayout()
        self.label = QLabel('Draw\n and I\'ll Guess!')
        self.label.setAlignment(Qt.AlignHCenter)
        self.label.setStyleSheet('*{color:#5A95B3;font-size:30px;font-weight:bold;}')
        self.stats = QLabel('Accuracy: 100%')
        self.stats.setAlignment(Qt.AlignHCenter)
        self.stats.setStyleSheet('*{font-size:20px;font-weight:bold;}')
        self.wrongButton = QPushButton('Report Error')
        self.correctButton = QPushButton('Report Accurate')
        rightLayout.addSpacerItem(QSpacerItem(0, 50, QSizePolicy.Minimum, QSizePolicy.Minimum))
        rightLayout.addWidget(self.label)
        rightLayout.addSpacerItem(QSpacerItem(0, 100, QSizePolicy.Minimum, QSizePolicy.Minimum))
        rightLayout.addWidget(self.stats)
        rightLayout.addWidget(self.wrongButton)
        rightLayout.addWidget(self.correctButton)
        rightLayout.setAlignment(Qt.AlignHCenter)

        layout = QHBoxLayout()
        layout.addLayout(leftLayout)
        layout.addLayout(rightLayout)

        self.setLayout(layout)
        self.setFixedSize(600, 350)

    def setLabel(self, text):
        self.label.setText(text)

    def imageRecognition(self):
        if self.handwriter.isModified():
            self.handwriter.saveImage()
            self.model.query()

    def __del__(self):
        del self.handwriter
        del self.clearButton
        del self.doneButton
        del self.label

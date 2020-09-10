# This Python file uses the following encoding: utf-8
import numpy as np
import pandas as pd
import tensorflow as tf

from PIL import Image

from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QApplication, QWidget

class Model:
    def __init__(self, parent):
        self.parent = parent
        self.modelPath = QApplication.applicationDirPath() + '/model.h5'
        self.queryPath = QApplication.applicationDirPath() + '/query.pgm'

    def query(self):
        im = np.array(Image.open(self.queryPath))
        model = tf.keras.models.load_model(self.modelPath)
        #model.summary()
        im = im.reshape((28, 28, 1))
        print(im)
        prediction = model.predict(im)
        print(prediction)
        self.parent.setLabel(prediction)

    def closeEvent(self, event):
        super(QWidget, self).closeEvent(event)

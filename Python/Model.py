# This Python file uses the following encoding: utf-8
import numpy as np
import tensorflow as tf

from PIL import Image

from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QApplication, QWidget

class Model:
    def __init__(self, parent):
        self.parent = parent
        self.modelPath = 'CNN/model.h5'
        self.queryPath = 'query.pgm'

    def query(self):
        im = np.array(Image.open(self.queryPath), dtype=np.float32)
        model = tf.keras.models.load_model(self.modelPath)
        im = im.reshape((1, 28, 28, 1))
        prediction = model.predict(im)
        clas = np.argmax(prediction)
        self.parent.setLabel("My Guess is a \n" + str(clas) + " !")

    def closeEvent(self, event):
        super(QWidget, self).closeEvent(event)

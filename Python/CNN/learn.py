import numpy as np
import pandas as pd
import tensorflow as tf
import matplotlib.pyplot as plt

din = ''
dout = '../'

train = pd.read_csv(din + 'train.csv')
train = train.to_numpy()

test = pd.read_csv(din + 'test.csv')
test = test.to_numpy()
test = test / 255.0
test = test.reshape((test.shape[0], 28, 28, 1))

trainY = train[:, 0]
trainX = train[:,1:] / 255.0
trainX = trainX.reshape((train.shape[0], 28, 28, 1))

del train

def create_model(target_size, num_classes):
    model = tf.keras.models.Sequential([
        tf.keras.layers.Conv2D(64, (3, 3), activation = 'relu'),
        tf.keras.layers.MaxPooling2D(2, 2),
        tf.keras.layers.Conv2D(32, (3, 3), activation = 'relu'),
        tf.keras.layers.MaxPooling2D(2, 2),
        tf.keras.layers.Flatten(),
        tf.keras.layers.Dense(128, activation = 'relu'),
        tf.keras.layers.Dropout(0.4),
        tf.keras.layers.Dense(64, activation = 'relu'),
        tf.keras.layers.Dense(32, activation = 'relu'),
        tf.keras.layers.Dense(16, activation = 'relu'),
        tf.keras.layers.Dense(num_classes, activation = 'softmax')
    ])

    model.compile(
        optimizer = 'adam',
        loss = 'sparse_categorical_crossentropy',
        metrics = ['acc']
    )

    model.build((None, target_size[0], target_size[1], 1))
    model.summary()

    return model

CLASSES = 10
SIZE = (28, 28)
BATCH = 32
EPOCHS = 20 #10-30

model = create_model(SIZE, CLASSES)

history = model.fit(trainX,
	trainY,
	batch_size = BATCH,
	epochs = EPOCHS,
	verbose = 2,
	validation_split = 0.005,
	shuffle = True)
)

model.save(dout + 'model.h5')

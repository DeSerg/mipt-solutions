import pandas as pd

import os
import shutil


data_dir = 'data'

train_dir = os.path.join(data_dir, 'train')
test_dir = os.path.join(data_dir, 'test')
validation_dir = os.path.join(data_dir, 'validation')

df_train = pd.read_csv('data/labels.csv')

for index, row in df_train[:10].iterrows():
    filename = row['id'] + '.jpg'
    cl = row['breed']

    train_path_orig = os.path.join(train_dir, filename)
    train_path_class = os.path.join(train_dir, cl)
    train_path_new = os.path.join(train_path_class, filename)
    print(train_path_orig, train_path_class, train_path_new)

    val_path_orig = os.path.join(validation_dir, filename)
    val_path_class = os.path.join(validation_dir, cl)
    val_path_new = os.path.join(val_path_class, filename)
    print(val_path_orig, val_path_class, val_path_new)

    if os.path.isfile(train_path_orig):
        print('yey')
        if not os.path.exists(train_path_class):
            os.makedirs(train_path_class)
        shutil.move(train_path_orig, train_path_new)

    if os.path.isfile(val_path_orig):
        print('yey')
        if not os.path.exists(val_path_class):
            os.makedirs(val_path_class)
        shutil.move(val_path_orig, val_path_new)

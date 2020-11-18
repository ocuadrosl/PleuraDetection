#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Nov  6 11:35:39 2020
@author: oscar
"""

import numpy as np
import matplotlib.pyplot as plt
from sklearn import svm
import pandas as pd
from sklearn.metrics import classification_report, plot_confusion_matrix
#from sklearn.pipeline import make_pipeline
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
from sklearn.feature_selection import RFECV
from sklearn.model_selection import StratifiedKFold
from sklearn.model_selection import GridSearchCV
from sklearn.metrics import confusion_matrix


dataPath = "/home/oscar/data/biopsy/tiff/test/csv/"
inputFileName = "fractal_lbp_coo_mome_200"
outputFileName = inputFileName+"_prediction"


data = pd.read_csv(dataPath+inputFileName+".csv") #data with headers and meta-data
#data = rawData.drop(['Image_Name','Col', 'Row'], axis=1);
data.dropna(inplace=True);


#clf = make_pipeline(StandardScaler(), svm.SVC(gamma='auto', C=10 ))

scaler = StandardScaler()
#X = data.loc[:, 'feature_1':'feature_66']# fractal + lbp + coo + moments
#X = data.loc[:, 'feature_2':'feature_60'] # lbp
#X = np.array(data.loc[:, 'feature_1']).reshape(-1,1) # fractal
#X = data.loc[:, 'feature_61':'feature_64']# Cooccurrence
X = data.loc[:, 'feature_2':'feature_64']# lbp + Cooccurrence
#X = data.loc[:, 'feature_65':'feature_66']# moments




Y = data['Label']
scaler.fit(X)
X = pd.DataFrame(scaler.transform(X))



X_train, X_test, y_train, y_test = train_test_split(X, Y, test_size=0.30, random_state=42)


paramGrid = {'C': [1, 10, 100, 1000, 10000], 'gamma': [0.0001, 0.0005, 0.001, 0.005, 0.01, 0.1, 'scale', 'auto'], }

clf = GridSearchCV(svm.SVC(kernel='rbf'), paramGrid)

#clf = svm.SVC(gamma='auto', C=10 , kernel="rbf") 
clf.fit(X_train, y_train);

print("Best estimator found by grid search:")
print(clf.best_estimator_)



#rfecv = RFECV(estimator=clf, step=1,  scoring='accuracy', cv=10)
#rfecv.fit(X_train,y_train);
# print("Optimal number of features : %d" % rfecv.n_features_)
# # Plot number of features VS. cross-validation scores
# plt.figure()
# plt.xlabel("Number of features selected")
# plt.ylabel("Cross validation score (nb of correct classifications)")
# plt.plot(range(1, len(rfecv.grid_scores_) + 1), rfecv.grid_scores_)
# plt.show()
# plt.get_current_fig_manager().show()


#prediction Test set
y_pred = clf.predict(X_test)

target_names = ['Non_pleura', 'Pleura']
n_classes = 2
print(classification_report(y_test, y_pred, target_names=target_names))
#print(confusion_matrix(y_test, y_pred, labels=range(n_classes)))

#Prediction all set
pred = clf.predict(X)
print(classification_report(Y, pred, target_names=target_names))

#writing results 
data['Prediction'] = pred
#print(data)
csv = data.to_csv(dataPath+outputFileName+".csv");



# disp = plot_confusion_matrix(clf, X_test, y_test)
# disp.figure_.suptitle("Confusion Matrix")
# plt.get_current_fig_manager().show()
# print("Confusion matrix:\n%s" % disp.confusion_matrix)


















import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3' 
####*IMPORANT*: Have to do this line *before* importing tensorflow
os.environ['PYTHONHASHSEED']=str(1)

import tensorflow as tf
import tensorflow.keras as keras
import tensorflow.keras.layers 
import matplotlib.pyplot as plt
import pandas as pd

import numpy as np
import random

all=[]
fitt = []
# The below is necessary in Python 3.2.3 onwards to
# have reproducible behavior for certain hash-based operations.
# See these references for further details:
# https://docs.python.org/3.4/using/cmdline.html#envvar-PYTHONHASHSEED
# https://github.com/fchollet/keras/issues/2280#issuecomment-306959926

def reset_random_seeds():
   os.environ['PYTHONHASHSEED']=str(1)
   tf.random.set_seed(1)
   np.random.seed(1)
   random.seed(1)


def most_frequent(List): 
    counter = 0
    num = List[0] 
      
    for i in List: 
        curr_frequency = List.count(i) 
        if(curr_frequency> counter): 
            counter = curr_frequency 
            num = i 
  
    return num

reset_random_seeds()
def Keras_training(N):
    
    selected_col = []
    one = 0
    for i in range(0,len(Chromo[N])):
        
        if Chromo[N][i] == '1':
            selected_col.append(i)
            
    
    print("\n||||||||||||||||||||||||||||||| ",len(selected_col))
    
    X = df.iloc[:,selected_col]
    Y = df.iloc[:,-1]

    col_num = len(selected_col)
    
    neuron = col_num + 1

    reset_random_seeds()
        
    avg=[]
    for i in range(1):
        
        train = df.sample(n=128, random_state=i)
        X_train = train.iloc[:,selected_col]
        Y_train = train.iloc[:,-1]
        
        X_test = df.iloc[:,selected_col]
        Y_test = df.iloc[:,-1]
        
        model = keras.Sequential([
                keras.layers.Dense(neuron, input_shape=(col_num,), activation='relu'),
                #keras.layers.Dense(neuron, activation='relu'),
                keras.layers.Dense(1, activation='sigmoid')
            ])

        model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])
        
        history = model.fit(X_train, Y_train, validation_data=(X_test, Y_test), verbose=0, epochs=100)
        
        #history = model.fit(X, Y, batch_size=100,validation_split = 0.2, epochs=100, verbose=0, shuffle=False)
        h = history.history['val_accuracy'][-1]
        #h = (history.history['val_accuracy'][-1] + history.history['accuracy'][-1])/2
        
        print(h)
        avg.append(h)
        
        tf.keras.backend.clear_session()
        del model
        
    h = sum(avg)/len(avg)
    weights = 60
    col = col_num
    f1 = 0.99*h
    f2 = 0.01*(weights-col)/(weights-1)
    f = f1+f2
    #f = h*(weights-col)/(weights-1)
    
    print("========================================================")
    print("C", N, " \nFitness  : ",f )
    print("Accuracy : ", h,"\nNumber of attributes : ",col_num,"\t fit+ :",f2)
    
    fitt.append(f)
    #print("Most frequency")
    #print(most_frequent(avg),"\n\n")
    
    
    fitness.append(f)
    
    # summarize history for accuracy
    plt.plot(history.history['accuracy'])
    plt.plot(history.history['val_accuracy'])
    plt.title('model accuracy')
    plt.ylabel('accuracy')
    plt.xlabel('epoch')
    
    plt.axhline(y=0.8317, color='green', linestyle='-')
    plt.axhline(y=h, color='lime', linestyle='--')
    
    #axes = plt.gca()
    #axes.set_ylim([0.0,1])
    
    plt.legend(['Training', 'Testing','Thersold','Last Testing Accuracy'], loc='lower right')
    
    plt.savefig('./t'+str(N)+'.png')
    plt.show()
    
    
file1 = open('./test_gene.txt', 'r') 
Chromo = file1.readlines()

csv_name = "../sonar_rock.xlsx"
df = pd.read_excel(csv_name)
print(df.iloc[0,-1])
#os.system("pause")
df.iloc[:,-1] = pd.factorize(df.iloc[:,-1])[0]
fitness=[]

    
for i in range(len(Chromo)):
    Keras_training(i)

print("===========\n",sum(fitness)/len(fitness))

writeResult = open("./result.txt", "w")
for value in fitness:
    writeResult.write(str(value)+"\n")
writeResult.close()

os.system("pause")







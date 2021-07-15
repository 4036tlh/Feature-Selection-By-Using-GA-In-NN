import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3' 
os.environ['PYTHONHASHSEED']=str(1)

import tensorflow as tf
import tensorflow.keras as keras
import tensorflow.keras.layers 
import matplotlib.pyplot as plt
import pandas as pd

all=[]
fitt = []

def reset_random_seeds():
   os.environ['PYTHONHASHSEED']=str(1)
   tf.random.set_seed(1)
    
def Keras_training(N):
    
    selected_col = []
    one = 0
    for i in range(0,len(Chromo[N])):
        
        if Chromo[N][i] == '1':
            selected_col.append(i)
            one += 2**i
    
    if one == 0:
        fitness.append(0)
        return 0
    
    if len(all)==0:
         all.append(one)
    else:
        for p in range(0,len(all)):
            if all[p] == str(one):
                fitness.append(fitt[p])
                print("============================================================================")
                print("c",N,"\t\t\t\t\t\t(Exist in history)")
                print("Fitness =", fitness[-1])
                print("Number of attributes : ", len(selected_col)) 
       
                return 0
        all.append(one)
            
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
                keras.layers.Dense(1, activation='sigmoid')
            ])

        model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])
        
        history = model.fit(X_train, Y_train, validation_data=(X_test, Y_test), verbose=0, epochs=100)
        
        h = history.history['val_accuracy'][-1]
        
        avg.append(h)
        
        tf.keras.backend.clear_session()
        del model
        
    h = sum(avg)/len(avg)
    weights = 60
    f1 = 0.99*h
    f2 = 0.01*(weights-col_num)/(weights-1)
    f = f1+f2
        
    print("============================================================================")
    print("C", N,"\nFitness =",f)
    print("Accuracy             : ", h      ,"    \t fit+ :",f1)
    print("Number of attributes : ", col_num,"\t\t\t fit+ :",f2)
        
    fitt.append(f)
    fitness.append(f)
    

#############################################
read_gene = open('./data/gene.txt', 'r') 
Chromo = read_gene.readlines()

csv_name = "sonar_rock.xlsx"
df = pd.read_excel(csv_name)
    
df.iloc[:,-1] = pd.factorize(df.iloc[:,-1])[0]
fitness=[]

#############################################
with open('./data/history.txt') as openHistory:
    for line in openHistory:
        his_fitness,his_chromo = line.strip().split('=')
        fitt.append(his_fitness)
        all.append(his_chromo)

#############################################        
print("\nStart training NN model...")
for i in range(len(Chromo)):
    Keras_training(i)

#############################################
writeResult = open("./data/result.txt", "w")
for value in fitness:
    writeResult.write(str(value)+"\n")
writeResult.close()

#############################################
writeHistory = open("./data/history.txt", "w")
for i in range(len(all)):
    writeHistory.write(str(fitt[i])+"="+str(all[i])+"\n")
writeHistory.close()






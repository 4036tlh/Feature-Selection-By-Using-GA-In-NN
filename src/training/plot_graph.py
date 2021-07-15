
import matplotlib.pyplot as plt

def ppp():
  
    x1=[]
    x2=[]
    with open('./data/bestFitness.txt') as f:
        for line1 in f:
            x1.append(float(line1))
    with open('./data/avgFitness.txt') as f:
        for line2 in f:
            x2.append(float(line2))
    #print(x1)
    #print(x2)
    
    #y=[i for i in range(4)]
    plt.plot(x1)
    plt.plot(x2)
    plt.title('Ftiness Over Generation')
    plt.ylabel('Fitness Values')
    plt.xlabel('Generations')
    plt.axhline(y=0.831, color='g', linestyle='-')
    plt.legend(['Best Fitness', 'Average Fitness','Thersold'], loc='lower right')
    
    plt.savefig('./data/Result_graph.png')
    plt.show()
    
    
ppp()
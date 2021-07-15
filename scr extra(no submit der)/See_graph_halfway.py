
import matplotlib.pyplot as plt

def plot_graph():
  
    x1=[]
    x2=[]
    
    #from folder
    dic = "../data"
    
    with open(dic + '/bestFitness.txt') as f:
        for line1 in f:
            x1.append(float(line1))
    with open(dic + '/avgFitness.txt') as f:
        for line2 in f:
            x2.append(float(line2))

    plt.plot(x1)
    plt.plot(x2)
    plt.title('Ftiness Over Generation')
    plt.ylabel('Fitness Values')
    plt.xlabel('Generations')
    plt.axhline(y=0.831, color='green', linestyle='-')
    axes = plt.gca()
    axes.set_ylim([0.7, 0.9])
    plt.legend(['Best Fitness', 'Average Fitness','Thersold'], loc ="lower right")
    plt.savefig(dic + '/Result_graph.png')
    plt.show()
    
plot_graph()

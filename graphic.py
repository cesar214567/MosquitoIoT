import matplotlib.pyplot as plt

def draw():
    filename = "results.txt"
    distances = []
    with open(filename, 'r') as f:
        for line in f:
            line = line.replace("\n", "")
            distances.append(float(line))
    time = [i for i in range(1, len(distances) + 1)]
    
    plt.plot(time, distances, color='blue')
    plt.pause(0.5)
    
if __name__ == '__main__':
    plt.show()
    while True:
        draw()
import os
import matplotlib.pyplot as plt

def generate_dse_plot():
    configs = ["1 Node (4 Ranks)", "2 Nodes (2 Ranks/Node, 1/Socket)", "2 Nodes (2 Ranks/Node, 2/Socket)", "4 Nodes (1 Rank/Node)"]
    files = [
        "dse_1node_4ranks.out",
        "dse_2nodes_1socket.out",
        "dse_2nodes_2sockets.out",
        "dse_4nodes_1rank.out"
    ]
    
    times = [0.015, 0.018, 0.020, 0.025] # Default mock data
    
    for i, file in enumerate(files):
        if os.path.exists(file):
            try:
                with open(file, 'r') as f:
                    for line in f:
                        if "Time elapsed" in line:
                            parts = line.split(":")
                            if len(parts) > 1:
                                times[i] = float(parts[1].split()[0].strip())
            except Exception as e:
                print(f"Error reading {file}: {e}")
        else:
            print(f"File {file} not found. Using mock data for {configs[i]}.")

    plt.figure(figsize=(10, 6))
    bars = plt.bar(configs, times, color=['green', 'blue', 'cyan', 'purple'])
    plt.ylabel('Execution Time (seconds)')
    plt.title('Sobel MPI+OpenMP DSE Performance (4 Ranks x 4 Threads)')
    plt.xticks(rotation=15, ha='right')
    plt.tight_layout()

    for bar in bars:
        yval = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2, yval + 0.001, round(yval, 4), ha='center', va='bottom')
    
    plt.savefig('dse_performance.png')
    print("Saved plot to dse_performance.png")

if __name__ == "__main__":
    generate_dse_plot()

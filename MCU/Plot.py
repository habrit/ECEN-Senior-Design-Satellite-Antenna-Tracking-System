import matplotlib.pyplot as plt

# Open the file "time.txt" in read mode
with open("time.txt", "r") as f:
    # Read the contents of the file and store them in a list
    times_str = f.readlines()

# Convert the list of strings to a list of floating-point numbers
times = [float(time.strip()) for time in times_str]

# Create a list of indices for the x-axis
indices = list(range(1, len(times) + 1))

# Create a scatter plot
plt.scatter(indices, times, marker='o')

# Customize the scatter plot with appropriate labels and titles
plt.title("Scatter Plot of Time Data")
plt.xlabel("Sample Index")
plt.ylabel("Time (seconds)")

# Show the scatter plot
plt.show()

# Calculate the average and print the number of samples
average = sum(times) / len(times)
print("Average: {:.4f}".format(average))
print("Number of samples: {}".format(len(times)))
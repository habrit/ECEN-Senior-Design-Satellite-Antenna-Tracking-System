import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

# Generate azimuth and elevation values for the full range
azimuths = np.linspace(0, 360, 360)  # 360 degrees in azimuth
elevations = np.linspace(0, 180, 180)  # 180 degrees in elevation

# Convert to radians
azimuths = np.radians(azimuths)
elevations = np.radians(elevations)

# Create a grid of points
azimuths, elevations = np.meshgrid(azimuths, elevations)

# Convert to Cartesian coordinates
x = np.sin(azimuths) * np.cos(elevations)
y = np.cos(azimuths) * np.cos(elevations)
z = np.sin(elevations)

# Create 3D plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot points
ax.scatter(x, y, z, label='Rotator Direction', s=1)  # s parameter controls the marker size

# Set labels
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_title('Rotator Direction Visualization')

# Show the plot
plt.show()
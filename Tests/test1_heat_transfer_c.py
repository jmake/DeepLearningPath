"""
reformulate it as an optimization problem.
"""
import os
os.environ['TF_ENABLE_ONEDNN_OPTS'] = '0'

import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

def create_matrix(Nx, Ny, alpha, delta_x, delta_y, delta_t):
    r_x = alpha * delta_t / (delta_x ** 2)
    r_y = alpha * delta_t / (delta_y ** 2)

    N = Nx * Ny
    A = tf.zeros((N, N), dtype=tf.float32)
    A = tf.Variable(A)

    for j in range(Ny):
        for i in range(Nx):
            index = i + j * Nx
            A[index, index].assign(1 + 2 * r_x + 2 * r_y)
            if i > 0:
                A[index, index - 1].assign(-r_x)
            if i < Nx - 1:
                A[index, index + 1].assign(-r_x)
            if j > 0:
                A[index, index - Nx].assign(-r_y)
            if j < Ny - 1:
                A[index, index + Nx].assign(-r_y)

    return A

def implicit_heat_equation(u_initial, alpha, delta_x, delta_y, delta_t, Nt):
    Nx, Ny = u_initial.shape
    N = Nx * Ny
    A = create_matrix(Nx, Ny, alpha, delta_x, delta_y, delta_t)

    u_old = tf.reshape(tf.convert_to_tensor(u_initial, dtype=tf.float32), (-1,))
    u_states = [u_old.numpy().copy()]

    tolerance = 1e-2
    learning_rate = 1e-0

    for n in range(Nt):
        b = u_old.numpy().copy()

        # Set up the right-hand side b for the next timestep
        for j in range(Ny):
            for i in range(Nx):
                index = i + j * Nx
                if i > 0:
                    b[index] += alpha * delta_t / (delta_x ** 2) * u_old[index - 1]
                if i < Nx - 1:
                    b[index] += alpha * delta_t / (delta_x ** 2) * u_old[index + 1]
                if j > 0:
                    b[index] += alpha * delta_t / (delta_y ** 2) * u_old[index - Nx]
                if j < Ny - 1:
                    b[index] += alpha * delta_t / (delta_y ** 2) * u_old[index + Nx]

        b_tensor = tf.convert_to_tensor(b, dtype=tf.float32)
        u_new = tf.Variable(tf.random.normal((N,), dtype=tf.float32))

        # Create a new optimizer instance for each u_new
        optimizer = tf.optimizers.SGD(learning_rate=learning_rate)

        # Define the loss function based on the difference between A*u_new and b
        def loss_function():
            return tf.reduce_mean(tf.square(tf.matmul(A, tf.reshape(u_new, (-1, 1))) - tf.reshape(b_tensor, (-1, 1))))

        # Gradient descent until convergence
        loss = None
        while True:
            with tf.GradientTape() as tape:
                loss = loss_function()
            if loss < tolerance: break
            gradients = tape.gradient(loss, [u_new])
            optimizer.apply_gradients(zip(gradients, [u_new]))

        print(n, loss)
        u_states.append(u_new.numpy().copy())
        u_old = u_new

    return u_states

if __name__ == "__main__":
    alpha = 0.01
    delta_x = 0.1
    delta_y = 0.1
    delta_t = 0.01
    Nt = 100

    Nx, Ny = 10, 10
    u_initial = np.zeros((Nx, Ny))
    u_initial[Nx // 2, Ny // 2] = 100

    u_states = implicit_heat_equation(u_initial, alpha, delta_x, delta_y, delta_t, Nt)

    # Visualization
    fig, ax = plt.subplots()
    cax = ax.imshow(u_states[0].reshape(Nx, Ny), cmap='hot', vmin=0, vmax=100)
    fig.colorbar(cax)

    def animate(frame):
        cax.set_array(u_states[frame].reshape(Nx, Ny))
        return [cax]

    ani = FuncAnimation(fig, animate, frames=Nt, interval=10, blit=True)
    plt.show()




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

    # Creating a tensor that can be modified
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
    #print( A.shape )

    u_old = tf.reshape(u_initial, (-1,))
    u_states = [u_old.numpy().copy()]

    for n in range(Nt):
        b = u_old.numpy().copy()
        
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

        _u_new = np.linalg.solve(A, b)
        _u_new = tf.convert_to_tensor(_u_new, dtype=tf.float32)

        b_tensor = tf.convert_to_tensor(b, dtype=A.dtype)
        b_tensor = tf.reshape(b_tensor, (-1, 1))

        u_new = tf.linalg.solve(A, b_tensor)

        u_states.append(u_new.numpy().copy())
        u_old = u_new

    return u_states

def animate_temperature_distribution(u_states, delta_t, Nx, Ny):
    fig, ax = plt.subplots()
    im = ax.imshow(u_states[0].reshape(Nx, Ny), cmap='hot', interpolation='nearest', vmin=0, vmax=100)
    plt.colorbar(im, ax=ax, label='Temperature')
    ax.set_title('2D Heat Distribution Over Time')
    ax.set_xlabel('X Coordinate')
    ax.set_ylabel('Y Coordinate')

    def update(frame):
        im.set_array(u_states[frame].reshape(Nx, Ny))
        ax.set_title(f'Time = {frame * delta_t:.2f} seconds')
        return [im]

    # Asignar la animación a una variable para evitar que se elimine
    ani = FuncAnimation(fig, update, frames=len(u_states), blit=False, repeat=False)
    
    # Mostrar la animación
    plt.show()



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
    animate_temperature_distribution(u_states, delta_t, Nx, Ny)

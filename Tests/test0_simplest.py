import os
os.environ['TF_ENABLE_ONEDNN_OPTS'] = '0'

import tensorflow as tf

# Define the matrix A and vector b
A = tf.constant([[3.0, 2.0], [1.0, 4.0]], dtype=tf.float32)
b = tf.constant([[5.0], [6.0]], dtype=tf.float32)

# Initialize x with random values and make it a variable so it can be optimized
x = tf.Variable(tf.random.normal([2, 1], dtype=tf.float32))

# Define the learning rate and optimizer
learning_rate = 0.01
optimizer = tf.optimizers.SGD(learning_rate)

# Define the loss function as the Mean Squared Error (MSE)
def loss_function():
    return tf.reduce_mean(tf.square(tf.matmul(A, x) - b))

# Set the tolerance level
tolerance = 1e-10

# Optimization loop with tolerance condition
step = 0
while True:
    with tf.GradientTape() as tape:
        loss = loss_function()
    # Check if the loss is below the tolerance
    if (loss < tolerance) or (step > 1000):
        break
    # Compute gradients and update x
    gradients = tape.gradient(loss, [x])
    optimizer.apply_gradients(zip(gradients, [x]))

    # Print loss every 100 steps for monitoring
    if step % 100 == 0:
        print(f"Step {step}: Loss = {loss.numpy()}")
    step += 1

# Print the final solution found by gradient descent
print("Converged Solution x:\n", x.numpy())
print(f"Final Loss: {loss.numpy()} after {step} steps (tolerance:{tolerance})")


## Classic 
x = tf.linalg.solve(A, b)
print("Solution x:\n", x.numpy())


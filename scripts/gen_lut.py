import numpy as np
import matplotlib.pyplot as plt

# generate sine to f32
sin_len = 256

t = np.linspace(0,1,sin_len)
s = (np.sin(2 * np.pi * t) + 1 ) * 0.5
# convert to q15
q = 15
q_sin = np.uint16(s * np.float32( 1 << q ) )

plt.figure(1)
plt.subplot(2,1,1)
plt.plot(s)
plt.subplot(2,1,2)
plt.plot(q_sin)
plt.show()
# print

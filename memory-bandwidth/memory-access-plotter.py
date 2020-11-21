#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
import struct
import sys

NS_TO_SEC = 1000 * 1000 * 1000

with open(sys.argv[1], mode='rb') as f:
    x = []
    y = []
    wall_start = 0;
    last_user = 0;
    for wc_s, wc_ns, user in struct.iter_unpack("LLL", f.read()):
        if (wall_start == 0):
            wall_start = (wc_s * NS_TO_SEC) + wc_ns
        x.append((wc_s * NS_TO_SEC) + wc_ns - wall_start)
        y.append(user - last_user)
        last_user = user
    plt.plot(x, y)
plt.show()

#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
import struct
import sys
import csv
import re

NS_TO_SEC = 1000.0 * 1000 * 1000
NS_TO_MS = 1000 * 1000

slot_divide = int(sys.argv[1])
slot_file = sys.argv[2]

virt_to_node = {}
with open(slot_file) as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        node = 0 if int(row["slot_no"]) > slot_divide else 1
        virt_to_node[row["virt_id"]] = node

start_ts = sorted([int(s.split("/")[-1].split("-")[0]) for s in sys.argv[3:]])[0]
print("Start ts: {}".format(start_ts))
wall_start = start_ts * 1000

f, axes = plt.subplots(2, 1, sharey=True, sharex=True)
for plot_idx, filename in enumerate(sys.argv[3:]):
    with open(filename, mode='rb') as f:
        plot_idx = 0
        x = []
        y = []
        wall_start = 0;
        last_user = 0;
        idx = 0;
        for wc_s, wc_ns, user in struct.iter_unpack("LLL", f.read()):
            x.append((((wc_s * NS_TO_SEC) + wc_ns) / NS_TO_MS) - wall_start)
            if last_user == 0:
                y.append(0)
            else:
                y.append(float(user - last_user) / 1000)
            last_user = user
            idx += 1;
            if idx == 100:
                #axes[plot_idx].axvline((((wc_s * NS_TO_SEC) + wc_ns) / NS_TO_MS) - wall_start, color = 'r')
                idx = 0
        virt_id = re.search("i-.*", filename)[0]
        color = 'g' if virt_to_node[virt_id] == 0 else 'b'
        axes[plot_idx].plot(x, y, color=color)
        axes[plot_idx].set_ylabel("milliseconds")
        axes[plot_idx].set_xlabel("milliseconds")
        axes[plot_idx].set_title(filename.split("/")[-1])
plt.show()

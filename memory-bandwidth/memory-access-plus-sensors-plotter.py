#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
import struct
import sys
import csv
import re
from dateutil import parser

NS_TO_SEC = 1000.0 * 1000 * 1000
NS_TO_MS = 1000 * 1000

slot_divide = int(sys.argv[1])
slot_file = sys.argv[2]
bmc_sensors_file = sys.argv[3]

virt_to_node = {}
with open(slot_file) as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        node = 0 if int(row["slot_no"]) > slot_divide else 1
        virt_to_node[row["virt_id"]] = node

f, axes = plt.subplots(2, 1, sharex=True)

temperature_0_measurements = {}
temperature_1_measurements = {}
fan_measurements = {}
timestamps = []
for line in open(bmc_sensors_file, mode='r').readlines():
    if re.search(" -------- ", line):
        human_readable = line.split(" -------- ")[-1].strip()
        timestamps.append(int(parser.parse(human_readable).timestamp()))
    container = None
    if re.match("^SYS_FAN_", line):
        container = fan_measurements
    if re.match("^CPU_0_DIMM", line):
        container = temperature_0_measurements
    if re.match("^CPU_1_DIMM", line):
        container = temperature_1_measurements
    if container != None:
        parts = [p.strip() for p in line.split("|")]
        if parts[0] not in container:
            container[parts[0]] = []
        container[parts[0]].append(float(parts[1]))

for name, vals in temperature_0_measurements.items():
    axes[0].plot(timestamps, vals, color='r')
for name, vals in temperature_1_measurements.items():
    axes[0].plot(timestamps, vals, color='orange')

axes[0].set_ylabel("DIMM temp degrees")
ax2 = axes[0].twinx()
for name, vals in fan_measurements.items():
    size = min(len(timestamps), len(vals))
    ax2.plot(timestamps[0:size], vals[0:size], color='b')
ax2.set_ylabel("Fan RPM")
axes[0].set_title("BMC sensors DIMM temp and fans")

for plot_idx, filename in enumerate(sys.argv[4:]):
    with open(filename, mode='rb') as f:
        plot_idx = 1
        x = []
        y = []
        wall_start = 0;
        last_user = 0;
        idx = 0;
        for wc_s, wc_ns, user in struct.iter_unpack("LLL", f.read()):
            x.append(((wc_s * NS_TO_SEC) + wc_ns) / NS_TO_SEC)
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

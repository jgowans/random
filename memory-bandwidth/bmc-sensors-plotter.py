#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
import struct
import sys
import re
from dateutil import parser

f, axes = plt.subplots(2, 1, sharey=True, sharex=True)

for plot_idx, filename in enumerate(sys.argv[1:]):
    temperature_0_measurements = {}
    temperature_1_measurements = {}
    fan_measurements = {}
    timestamps = []
    for line in open(filename, mode='r').readlines():
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
        axes[plot_idx].plot(timestamps, vals, color='r')
    for name, vals in temperature_1_measurements.items():
        axes[plot_idx].plot(timestamps, vals, color='orange')

    axes[plot_idx].set_ylabel("DIMM temp degrees")
    ax2 = axes[plot_idx].twinx()
    for name, vals in fan_measurements.items():
        size = min(len(timestamps), len(vals))
        ax2.plot(timestamps[0:size], vals[0:size], color='b')
    ax2.set_ylabel("Fan RPM")
    axes[plot_idx].set_title(filename.split("/")[-1])
plt.show()

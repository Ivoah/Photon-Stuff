#!/usr/local/bin/python3

import os
import sys
import glob
import subprocess

DEVICE = 'Joda'

projects = {
    'iTunes': {
        'files': ['iTunes.ino'],
        'libs': ['ILI9163']
    },
    'Joystick': {
        'files': ['Joystick.ino'],
        'libs': ['ILI9163']
    },
    'Noise': {
        'files': ['Noise.ino'],
        'libs': ['ILI9163', 'PerlinNoise']
    },
    'Print': {
        'files': ['Print.ino'],
        'libs': ['ILI9163']
    }
}

proj = sys.argv[1]
try:
    usb = sys.argv[2] == 'usb'
except IndexError:
    usb = False
files = projects[proj]['files']
for lib in projects[proj]['libs']:
    files += glob.glob(f'Particle_{lib}/firmware/*')

if usb:
    pre = os.listdir()
    proc = subprocess.run(['particle', 'compile', 'photon', *files], stdout = subprocess.PIPE)
    fw = proc.stdout.decode().split('/')[-1].strip()
    subprocess.run(['particle', 'flash', '--usb', fw])
    os.remove(fw)
else:
    subprocess.run(['particle', 'flash', DEVICE, *files])

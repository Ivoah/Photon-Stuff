#!/usr/local/bin/python3

import os
import sys
import glob
import getopt
import subprocess

device = 'Joda'

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
    },
    'CPU': {
        'files': ['CPU.ino'],
        'libs': ['ILI9163']
    },
    'dunno': {
        'files': ['dunno.ino'],
        'libs': ['ILI9163']
    },
    'Moe': {
        'files': ['Moe.ino'],
        'libs': ['ILI9163']
    },
    'IDK': {
        'files': ['IDK.ino'],
        'libs': ['ILI9163']
    }
}

usb = False

opts, args = getopt.getopt(sys.argv[1:], 'ud:', ['usb', 'device='])
for opt, arg in opts:
    if opt in ['-u', '--usb']:
        usb = True
    elif opt in ['-d', '--device']:
        device = arg

proj = args[0]

files = projects[proj]['files']
for lib in projects[proj]['libs']:
    files += glob.glob(f'Particle_{lib}/firmware/*')

if usb:
    #pre = os.listdir()
    proc = subprocess.run(['particle', 'compile', 'photon', *files], stdout = subprocess.PIPE)
    if proc.returncode:
        print(proc.stdout.decode())
        sys.exit(proc.returncode)
    fw = proc.stdout.decode().split('/')[-1].strip()
    subprocess.run(['particle', 'flash', '--usb', fw])
    os.remove(fw)
else:
    subprocess.run(['particle', 'flash', device, *files])

.phony: iTunes Joystick Noise

iTunes:
	particle flash Joda iTunes.ino Particle_ILI9163/firmware/*

Joystick:
	particle flash Joda Joystick.ino Particle_ILI9163/firmware/*

Noise:
	particle flash Joda Noise.ino Particle_ILI9163/firmware/* Particle_PerlinNoise/firmware/*

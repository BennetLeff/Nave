# Nave
A concatenative synthesizer

Concatenative synthesis is a fascinating form of sound synthesis based on sampling small granules of sound from a variety of sources.
Granules are concatenated together to form larger audio waveforms. This is similar to granular synthesis but draws samples from a 
variety of sources as opposed to just one in granular synthesis. An important part of concatenative synthesis is picking samples to 
string together. This is done by some target function which looks at tagged audio files and picks optimal samples to create
the desired sound.

The first milestone of this project is to build a granular synthesizer. At the moment the code defines a very basic sampler.
I'll be souping up the sampler, to include:
* arbitrary file plackback
* sample start and stop heads can be at arbitrary points rather than at the beginning and end of a file
* looping by time and bpm units
* GUI display of the file as it's being played

Naturally, one can build a granular synthesizer on top of a sampler
through some reasonable modifications. Once I have a working granular synthesizer I will further evaluate how to 
morph it into the concatenative synthesizer.

At the moment the project resembles a basic sampler. One can load any WAV file and play it on a midi keyboard. There's even a thumbnail that shows what the waveform looks like.

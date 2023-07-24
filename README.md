# VLF-SDR

A Software-Defined VLF (Very Low Frequency) Radio.

## What is this about?

As a licenced radio amateur I have access to a range of bands and techniques, but my interests lie at the far ends of the radio spectrum.  One challenge I found was the lack of equipment (either commercial or DIY) for the 2,200m band (137KHz).  Since this is such a low frequency, it lends itself in many ways to building your own equipment.  I figured I would have a try at this project.

Rather than build a conventional VFO (Variable Frequency Oscillator) I decided I would use the readily-available AD9850 DDS module.  These are quite easy to use, stable and relatively inexpensive.  This repo contains the code for driving one such module.

## The code

### ad9850test1

A simple test generator.  Once running, you will see a 137.5KHz signal on the output pins of your module.

### ad9850test2

A better test generator.  Uses an additional I/O line to change the freqency by 455KHz.  Why?  Ultimately this will be a transmitter and superhet receiver combo, so we need to adjust the receive frequency.

## Assembling your own radio

Watch this space!

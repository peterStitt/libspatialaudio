# Object Panning

If you are not interested in the theory you can read about how to use `ObjectPanner` [here](#objectpanner).

## Theory and Implementation Details

Object-based panning is the process of placing a mono signal at an arbitrary position in a loudspeaker layout by distributing its energy across the available loudspeakers.  
`ObjectPanner` implements Vector Base Amplitude Panning (VBAP) following the algorithm described in the Audio Definition Model (ADM) renderer specification [[1](#ref1), [2](#ref2)].

`ObjectPanner` can be used to pan a mono sound source as a point source on a given loudspeaker array.
More advanced Object rendering is possible with the `Renderer` class that allows for metadata to specify additional parameters, such as object extent.

### VBAP Algorithm

Classic VBAP works by selecting a subset of loudspeakers (a base) that encloses the desired source direction and calculating gain factors such that the vector sum of the loudspeaker positions matches the target direction. The gains are normalized to preserve energy.

For a source at direction vector $`\mathbf{d}`$ and a loudspeaker base matrix $`\mathbf{L}`$ containing unit vectors in the loudspeaker directions, the gains $`\mathbf{g}`$ are obtained by solving:

```math
\mathbf{L} \mathbf{g} = \mathbf{d}
```

with the constraint that all elements of g are non-negative. The resulting gains $`g'`$ are then normalized:

```math
\mathbf{g}' = \frac{\mathbf{g}}{\|\mathbf{g}\|}
```

### Gain Interpolation

To avoid audible artefacts when the source position changes (clicks or "zipper noise"), `ObjectPanner` uses an internal `GainInterp` object. When the position is updated, the loudspeaker gain vector is interpolated over a user-specified fade time (typically 10 ms). This ensures smooth transitions.

## ObjectPanner

The `ObjectPanner` class is used to distribute a mono signal across a loudspeaker layout using VBAP. It supports smooth position updates to reduce real-time "zipper" effects for moving sources.

### Configuration

Before calling any other functions the object must first be configured by calling `Configure()` with the appropriate values. If the values are supported then it will return `true` and the object can now be used.

The configuration parameters are:

- **layout**: An `OutputLayout` to select a support output layout.  
- **sampleRate**: The sample rate of the audio being used (e.g. 44100 Hz, 48000 Hz). Must be an integer > 0.  
- **fadeTimeMilliSec**: The time in milliseconds to fade from an old set of panning gains to another. Lower values reduce latency but may introduce artefacts; higher values increase smoothness at the cost of responsiveness.  

### Set Panning Position

The panning position is set as a polar direction in radians using the `SetPosition()` function. It takes a `PolarPosition<double>` as input.

### Panning a Signal

An array of floats can be panned using either the `Process()` or `ProcessAccumul()` functions. These two functions process the input signal in the same way. The only difference is that `ProcessAccumul()` will add the newly panned signal to the output with an optional gain, whereas `Process()` will replace the destination signal with the panned signal.

The inputs are:

- **pfSrc**: A pointer to the mono input signal.  
- **nSamples**: The length of the input signal in samples.  
- **ppDst**: A pointer to the destination buffer containing one channel per loudspeaker.  
- **nSamplesOut**: The number of samples in the output buffer. Must be at least `nSamplesIn` + `nOffset` in value.
- **nOffset**: Optional offset position when writing to the output.  
- (`ProcessAccumul()` only) **fGain**: Optional gain to apply before accumulation.  

---

### Code Example

This example shows how to pan a mono sine wave into a 5.1 loudspeaker layout using `ObjectPanner`.

```c++
#include "ObjectPanner.h"

using namespace spaudio;

const unsigned int sampleRate = 48000;
const int nBlockLength = 512;

auto layout = OutputLayout::FivePointOne;

ObjectPanner objPanner;
objPanner.Configure(layout, sampleRate, 1000.f * (float)nBlockLength / (float)sampleRate);
unsigned nLdspk = (unsigned)objPanner.GetNumSpeakers();

// Generate a block of sine wave samples
std::vector<float> sinewave(nBlockLength);
for (int i = 0; i < nBlockLength; ++i)
    sinewave[i] = (float)std::sin((float)M_PI * 2.f * 440.f * (float)i / (float)sampleRate);

// Prepare the output stream (5.1 layout)
float** renderStream = new float* [nLdspk];
for (unsigned int i = 0; i < nLdspk; ++i)
{
    renderStream[i] = new float[nBlockLength];
    std::fill(renderStream[i], renderStream[i] + nBlockLength, 0.f);
}

for (float az = 0.f; az < 360.f; az += 1.f)
{
    auto position = PolarPosition<double>{ az, 0.f, 1.f };
    objPanner.SetPosition(position);
    objPanner.Process(sinewave.data(), nBlockLength, renderStream, nBlockLength);
}

for (unsigned iLdspk = 0; iLdspk < nLdspk; ++iLdspk)
    delete[] renderStream[iLdspk];
delete[] renderStream;
```

## References

<a name="ref1">[1]</a> Ville Pulkki. Virtual sound source positioning using vector base amplitude panning. J. Audio Eng. Soc 45(6):456-466, 1997.

<a name="ref2">[2]</a> International Telecommunication Union. Audio Definition Model renderer for advanced sound systems. International Telecommunication Union, Geneva, Switzerland, Recommendation ITU-R BS.2127-1 edition, 2023. URL <https://www.itu.int/rec/r-rec-bs.2127/en>.

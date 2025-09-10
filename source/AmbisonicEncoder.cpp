/*############################################################################*/
/*#                                                                          #*/
/*#  Ambisonic C++ Library                                                   #*/
/*#  AmbisonicEncoder - Ambisonic Encoder                                   #*/
/*#  Copyright © 2007 Aristotel Digenis                                      #*/
/*#                                                                          #*/
/*#  Filename:      AmbisonicEncoder.cpp                                     #*/
/*#  Version:       0.1                                                      #*/
/*#  Date:          19/05/2007                                               #*/
/*#  Author(s):     Aristotel Digenis                                        #*/
/*#  Licence:       MIT                                                      #*/
/*#                                                                          #*/
/*############################################################################*/


#include "AmbisonicEncoder.h"
#include <assert.h>
#include <cmath>

namespace spaudio {

    AmbisonicEncoder::AmbisonicEncoder() : m_coeffInterp(0)
    {
    }

    AmbisonicEncoder::~AmbisonicEncoder()
    {
    }

    bool AmbisonicEncoder::Configure(unsigned nOrder, bool b3D, unsigned sampleRate, float fadeTimeMilliSec)
    {
        bool success = AmbisonicSource::Configure(nOrder, b3D, sampleRate);
        if (!success || fadeTimeMilliSec < 0.f)
            return false;

        m_pfCoeffCurrent.resize(m_nChannelCount);
        m_coeffInterp = GainInterp<float>(m_nChannelCount);

        m_fadingTimeMilliSec = fadeTimeMilliSec;
        m_fadingSamples = (unsigned)std::round(0.001f * m_fadingTimeMilliSec * (float)sampleRate);

        return true;
    }

    void AmbisonicEncoder::Refresh()
    {
        AmbisonicSource::Refresh();
    }

    void AmbisonicEncoder::Reset()
    {
        AmbisonicSource::Reset();
        m_coeffInterp.Reset();
    }

    void AmbisonicEncoder::SetPosition(PolarPoint polPosition)
    {
        // Update the coefficients
        AmbisonicSource::SetPosition(polPosition);
        AmbisonicSource::Refresh();
        AmbisonicSource::GetCoefficients(m_pfCoeffCurrent);
        m_coeffInterp.SetGainVector(m_pfCoeffCurrent, m_fadingSamples);
    }

    void AmbisonicEncoder::Process(float* pfSrc, unsigned nSamples, BFormat* pfDst, unsigned int nOffset)
    {
        assert(nSamples + nOffset <= pfDst->GetSampleCount()); // Cannot write beyond the of the destination buffers!

        m_coeffInterp.Process(pfSrc, pfDst->m_ppfChannels.get(), nSamples, nOffset);
    }

    void AmbisonicEncoder::ProcessAccumul(float* pfSrc, unsigned nSamples, BFormat* pfDst, unsigned int nOffset, float fGain)
    {
        assert(nSamples + nOffset <= pfDst->GetSampleCount()); // Cannot write beyond the of the destination buffers!

        m_coeffInterp.ProcessAccumul(pfSrc, pfDst->m_ppfChannels.get(), nSamples, nOffset, fGain);
    }

} // namespace spaudio

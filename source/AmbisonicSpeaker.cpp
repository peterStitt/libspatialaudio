/*############################################################################*/
/*#                                                                          #*/
/*#  Ambisonic C++ Library                                                   #*/
/*#  AmbisonicSpeaker - Ambisonic Speaker                                   #*/
/*#  Copyright © 2007 Aristotel Digenis                                      #*/
/*#  Copyright © 2017 Videolabs                                              #*/
/*#                                                                          #*/
/*#  Filename:      AmbisonicSpeaker.cpp                                     #*/
/*#  Version:       0.2                                                      #*/
/*#  Date:          19/05/2007                                               #*/
/*#  Author(s):     Aristotel Digenis, Peter Stitt                           #*/
/*#  Licence:       LGPL (+ Proprietary)                                     #*/
/*#                                                                          #*/
/*############################################################################*/


#include "AmbisonicSpeaker.h"

namespace spaudio {

    AmbisonicSpeaker::AmbisonicSpeaker()
    {
    }

    AmbisonicSpeaker::~AmbisonicSpeaker()
    {
    }

    bool AmbisonicSpeaker::Configure(unsigned nOrder, bool b3D, unsigned nMisc)
    {
        bool success = AmbisonicSource::Configure(nOrder, b3D, nMisc);
        if (!success)
            return false;

        return true;
    }

    void AmbisonicSpeaker::Refresh()
    {
        AmbisonicSource::Refresh();
    }

    void AmbisonicSpeaker::Process(BFormat* pBFSrc, unsigned nSamples, float* pfDst)
    {
        unsigned niChannel = 0;
        unsigned niSample = 0;
        memset(pfDst, 0, nSamples * sizeof(float));
        for (niChannel = 0; niChannel < m_nChannelCount; niChannel++)
        {
            float* in = pBFSrc->m_ppfChannels[niChannel];
            float* out = pfDst;

            const float coeff = m_pfCoeff[niChannel];
            for (niSample = 0; niSample < nSamples; niSample++)
                *out++ += (*in++) * coeff;
        }
    }

} // namespace spaudio

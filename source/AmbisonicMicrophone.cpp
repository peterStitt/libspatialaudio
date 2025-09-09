/*############################################################################*/
/*#                                                                          #*/
/*#  Ambisonic C++ Library                                                   #*/
/*#  AmbisonicMicrophone - Ambisonic Microphone                             #*/
/*#  Copyright © 2007 Aristotel Digenis                                      #*/
/*#                                                                          #*/
/*#  Filename:      AmbisonicMicrophone.cpp                                  #*/
/*#  Version:       0.1                                                      #*/
/*#  Date:          19/05/2007                                               #*/
/*#  Author(s):     Aristotel Digenis                                        #*/
/*#  Licence:       MIT                                                      #*/
/*#                                                                          #*/
/*############################################################################*/


#include "AmbisonicMicrophone.h"

AmbisonicMicrophone::AmbisonicMicrophone()
{
    m_fDirectivity = 1.f;
}

AmbisonicMicrophone::~AmbisonicMicrophone()
{ }

void AmbisonicMicrophone::Refresh()
{
    AmbisonicSource::Refresh();

    m_pfCoeff[0] *= (2.f - m_fDirectivity) * sqrtf(2.f);
}

void AmbisonicMicrophone::Process(BFormat* pBFSrc, unsigned nSamples, float* pfDst)
{
    unsigned niChannel = 0;
    unsigned niSample = 0;
    float fTempA = 0;
    float fTempB = 0;
    for(niSample = 0; niSample < nSamples; niSample++)
    {
        fTempA = pBFSrc->m_ppfChannels[0][niSample] * m_pfCoeff[0];
        fTempB = 0;
        for(niChannel = 1; niChannel < m_nChannelCount; niChannel++)
        {
            fTempB += pBFSrc->m_ppfChannels[niChannel][niSample] * m_pfCoeff[niChannel];
        }
        pfDst[niSample] = 0.5f * (fTempA + fTempB * m_fDirectivity);
    }
}

void AmbisonicMicrophone::SetDirectivity(float fDirectivity)
{
    m_fDirectivity = fDirectivity;
}

float AmbisonicMicrophone::GetDirectivity()
{
    return m_fDirectivity;
}

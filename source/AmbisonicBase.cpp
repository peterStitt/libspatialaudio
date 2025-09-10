/*############################################################################*/
/*#                                                                          #*/
/*#  Ambisonic C++ Library                                                   #*/
/*#  AmbisonicBase - Ambisonic Base                                         #*/
/*#  Copyright © 2007 Aristotel Digenis                                      #*/
/*#                                                                          #*/
/*#  Filename:      AmbisonicBase.cpp                                        #*/
/*#  Version:       0.1                                                      #*/
/*#  Date:          19/05/2007                                               #*/
/*#  Author(s):     Aristotel Digenis                                        #*/
/*#  Licence:       MIT                                                      #*/
/*#                                                                          #*/
/*############################################################################*/


#include "AmbisonicBase.h"
#include <assert.h>

namespace spaudio {

    AmbisonicBase::AmbisonicBase()
        : m_nOrder(0)
        , m_b3D(0)
        , m_nChannelCount(0)
    {
    }

    unsigned AmbisonicBase::GetOrder()
    {
        return m_nOrder;
    }

    bool AmbisonicBase::GetHeight()
    {
        return m_b3D;
    }

    unsigned AmbisonicBase::GetChannelCount()
    {
        return m_nChannelCount;
    }

    bool AmbisonicBase::Configure(unsigned nOrder, bool b3D, unsigned /*nMisc*/)
    {
        assert(nOrder <= 3); // Only supports up to 3rd order
        if (nOrder > 3)
            return false;
        m_nOrder = nOrder;
        m_b3D = b3D;
        m_nChannelCount = OrderToComponents(m_nOrder, m_b3D);

        return true;
    }

} // namespace spaudio

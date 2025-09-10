/*############################################################################*/
/*#                                                                          #*/
/*#  Screen scaling and screen edge lock handling                            #*/
/*#                                                                          #*/
/*#                                                                          #*/
/*#  Filename:      Screen.cpp                                               #*/
/*#  Version:       0.1                                                      #*/
/*#  Date:          30/10/2020                                               #*/
/*#  Author(s):     Peter Stitt                                              #*/
/*#  Licence:       LGPL + proprietary                                       #*/
/*#                                                                          #*/
/*############################################################################*/

#include "Screen.h"

namespace spaudio {

    ScreenScaleHandler::ScreenScaleHandler(const Optional<Screen>& reproductionScreen, const Layout& layout) : m_layout(layout)
    {
        m_repScreen = reproductionScreen;
        if (m_repScreen.hasValue())
            m_repPolarEdges.fromScreen(m_repScreen.value());
    }

    ScreenScaleHandler::~ScreenScaleHandler()
    {

    }

    CartesianPosition ScreenScaleHandler::handle(CartesianPosition position, bool screenRef, const Screen& referenceScreen, bool cartesian)
    {
        if (screenRef && m_repScreen.hasValue())
        {
            m_refPolarEdges.fromScreen(referenceScreen);

            if (cartesian)
            {
                auto polarPosition = adm::PointCartToPolar(position);
                auto AzEl_s = ScaleAzEl(polarPosition.azimuth, polarPosition.elevation);
                auto AzEl_sc = adm::CompensatePosition(AzEl_s.first, AzEl_s.second, m_layout.channelNames());
                return adm::PointPolarToCart({ AzEl_sc.first,AzEl_sc.second,polarPosition.distance });
            }
            else
                return ScalePosition(position);
        }
        else
            return position; // return unmodified
    }

    CartesianPosition ScreenScaleHandler::ScalePosition(CartesianPosition position)
    {
        PolarPosition polarPosition = CartesianToPolar(position);
        auto AzEl_s = ScaleAzEl(polarPosition.azimuth, polarPosition.elevation);
        return PolarToCartesian(PolarPosition{ AzEl_s.first,AzEl_s.second,polarPosition.distance });
    }

    std::pair<double, double> ScreenScaleHandler::ScaleAzEl(double az, double el)
    {
        double azScaled = interp(az, { -180., m_refPolarEdges.rightAzimuth,m_refPolarEdges.leftAzimuth, 180. },
            { -180., m_repPolarEdges.rightAzimuth,m_repPolarEdges.leftAzimuth, 180. });
        double elScaled = interp(el, { -90., m_refPolarEdges.bottomElevation,m_refPolarEdges.topElevation, 90. },
            { -90., m_repPolarEdges.bottomElevation,m_repPolarEdges.topElevation, 90. });

        return { azScaled,elScaled };
    }


    // ScreenEdgeLockHandler ==========================================================================================================
    ScreenEdgeLockHandler::ScreenEdgeLockHandler(const Optional<Screen>& reproductionScreen, const Layout& layout) : m_layout(layout)
    {
        m_reproductionScreen = reproductionScreen;
        if (m_reproductionScreen.hasValue())
            m_repPolarEdges.fromScreen(m_reproductionScreen.value());
    }

    ScreenEdgeLockHandler::~ScreenEdgeLockHandler()
    {

    }

    CartesianPosition ScreenEdgeLockHandler::HandleVector(CartesianPosition position, ScreenEdgeLock screenEdgeLock, bool cartesian)
    {
        if (m_reproductionScreen.hasValue())
        {
            if (cartesian)
            {
                auto polarPosition = adm::PointCartToPolar(position);
                auto AzEl_s = HandleAzEl(polarPosition.azimuth, polarPosition.elevation, screenEdgeLock);
                auto AzEl_sc = adm::CompensatePosition(AzEl_s.first, AzEl_s.second, m_layout.channelNames());
                return adm::PointPolarToCart({ AzEl_sc.first,AzEl_sc.second,polarPosition.distance });
            }
            else
            {
                PolarPosition polarPosition = CartesianToPolar(position);
                auto AzEl_s = HandleAzEl(polarPosition.azimuth, polarPosition.elevation, screenEdgeLock);
                return PolarToCartesian(PolarPosition{ AzEl_s.first,AzEl_s.second,polarPosition.distance });
            }
        }
        else
            return position;
    }

    std::pair<double, double> ScreenEdgeLockHandler::HandleAzEl(double az, double el, ScreenEdgeLock screenEdgeLock)
    {
        if (m_reproductionScreen.hasValue())
        {
            if (screenEdgeLock.horizontal == ScreenEdgeLock::LEFT)
                az = m_repPolarEdges.leftAzimuth;
            if (screenEdgeLock.horizontal == ScreenEdgeLock::RIGHT)
                az = m_repPolarEdges.rightAzimuth;

            if (screenEdgeLock.vertical == ScreenEdgeLock::TOP)
                el = m_repPolarEdges.topElevation;
            if (screenEdgeLock.vertical == ScreenEdgeLock::BOTTOM)
                el = m_repPolarEdges.bottomElevation;
        }

        return { az, el };
    }

} // namespace spaudio

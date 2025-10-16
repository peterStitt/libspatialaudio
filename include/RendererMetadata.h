/*############################################################################*/
/*#                                                                          #*/
/*#  Metadata structures for the Renderer class.                             #*/
/*#                                                                          #*/
/*#  Copyright © 2020 Peter Stitt                                            #*/
/*#                                                                          #*/
/*#  Filename:      RendererMetadata.h                                       #*/
/*#  Version:       0.1                                                      #*/
/*#  Date:          23/06/2020                                               #*/
/*#  Author(s):     Peter Stitt                                              #*/
/*#  Licence:       LGPL + proprietary                                       #*/
/*#                                                                          #*/
/*############################################################################*/

#pragma once

#include <vector>
#include <string>
#include <map>
#include "Coordinates.h"
#include "ScreenCommon.h"

namespace spaudio {

    /** The different audio types expected from the ADM metadata. Rec. See ITU-R BS.2127-0 pg. 8 */
    enum class TypeDefinition {
        DirectSpeakers = 1,
        Matrix,
        Objects,
        HOA,
        Binaural
    };

    /** A simple class similar to c++ 17's std::optional indicate that the metadata is optional. */
    template<typename T>
    class Optional
    {
    public:
        Optional() : m_data(), m_hasValue(false)
        {
        };
        Optional(const T& value) : m_hasValue(true)
        {
            m_data = value;
        }
        Optional(const Optional<T>& optional) : m_data(), m_hasValue(optional.m_hasValue)
        {
            if (optional.m_hasValue)
                m_data = optional.m_data;
        }
        ~Optional() {};

        bool hasValue() const { return m_hasValue; };

        T& value() { assert(m_hasValue); return m_data; };
        const T& value() const { assert(m_hasValue); return m_data; };

        T* operator->() { assert(m_hasValue); return &m_data; };
        const T* operator->() const { assert(m_hasValue); return reinterpret_cast<const T*>(&m_data); };

        void reset()
        {
            if (m_hasValue)
            {
                m_data = T();
                m_hasValue = false;
            }
        };

        Optional& operator=(const Optional& other)
        {
            if (other.m_hasValue)
            {
                m_data = other.m_data;
                m_hasValue = true;
            }
            else
            {
                reset();
            }

            return *this;
        };

        bool operator==(const Optional& other) const
        {
            if (m_hasValue != other.hasValue())
                return false;
            if (!m_hasValue)
                return true;
            return value() == other.value();
        };

    private:
        T m_data;
        bool m_hasValue;
    };

    // Shared structures (Rec.ITU - R BS.2127-0 section 11.1.1) ============================================================

    /** Frequency data for the channel. */
    struct Frequency {
        Optional<double> lowPass;
        Optional<double> highPass;
    };
    inline bool operator==(const Frequency& lhs, const Frequency& rhs)
    {
        return lhs.lowPass == rhs.lowPass && lhs.highPass == rhs.highPass;
    }

    struct ChannelLock
    {
        // If the distance is set <0 then no channel locking is applied
        Optional<double> maxDistance;
    };
    inline bool operator==(const ChannelLock& lhs, const ChannelLock& rhs)
    {
        return lhs.maxDistance == rhs.maxDistance;
    }
    struct ObjectDivergence
    {
        double value = 0.0;
        Optional<double> azimuthRange;
        Optional<double> positionRange;
    };
    inline bool operator==(const ObjectDivergence& lhs, const ObjectDivergence& rhs)
    {
        return lhs.value == rhs.value && lhs.azimuthRange == rhs.azimuthRange;
    }

    struct ScreenEdgeLock {
        enum Horizontal { NO_HOR, LEFT, RIGHT };
        enum Vertical { NO_VERT, BOTTOM, TOP };
        Horizontal horizontal = NO_HOR;
        Vertical vertical = NO_VERT;
    };
    inline bool operator==(const ScreenEdgeLock& lhs, const ScreenEdgeLock& rhs)
    {
        return lhs.horizontal == rhs.horizontal && lhs.vertical == rhs.vertical;
    }

    // Structures to hold positions of objects
    struct PolarBounds
    {
        double minAzimuth;
        double maxAzimuth;
        double minElevation;
        double maxElevation;
        double minDistance;
        double maxDistance;
    };
    inline bool operator==(const PolarBounds& lhs, const PolarBounds& rhs)
    {
        return lhs.minAzimuth == rhs.minAzimuth && lhs.maxAzimuth == rhs.maxAzimuth
            && lhs.minElevation == rhs.minElevation && lhs.maxElevation == rhs.maxElevation
            && lhs.minDistance == rhs.minDistance && lhs.maxDistance == rhs.maxDistance;
    }
    struct CartesianBounds
    {
        double minX;
        double maxX;
        double minY;
        double maxY;
        double minZ;
        double maxZ;
    };
    inline bool operator==(const CartesianBounds& lhs, const CartesianBounds& rhs)
    {
        return lhs.minX == rhs.minX && lhs.maxX == rhs.maxX
            && lhs.minY == rhs.minY && lhs.maxY == rhs.maxY
            && lhs.minZ == rhs.minZ && lhs.maxZ == rhs.maxZ;
    }

    struct JumpPosition
    {
        bool flag = false;
        // This is the duration of the interpolation in samples
        Optional<int> interpolationLength = 0;
    };
    inline bool operator==(const JumpPosition& lhs, const JumpPosition& rhs)
    {
        return lhs.flag == rhs.flag && lhs.interpolationLength == rhs.interpolationLength;
    }

    struct DirectSpeakerPolarPosition
    {
        double azimuth = 0.0;
        double elevation = 0.0;
        double distance = 1.f;
        // Bounds for speaker used in DirectSpeaker gain calculation
        Optional<PolarBounds> bounds;
    };
    inline bool operator==(const DirectSpeakerPolarPosition& lhs, const DirectSpeakerPolarPosition& rhs)
    {
        return lhs.azimuth == rhs.azimuth && lhs.elevation == rhs.elevation
            && lhs.distance == rhs.distance && lhs.bounds == rhs.bounds;
    }
    struct DirectSpeakerCartesianPosition
    {
        double x = 1.0;
        double y = 0.0;
        double z = 0.0;
        // Bounds for speaker used in DirectSpeaker gain calculation
        Optional<CartesianBounds> bounds;
    };
    inline bool operator==(const DirectSpeakerCartesianPosition& lhs, const DirectSpeakerCartesianPosition& rhs)
    {
        return lhs.x == rhs.x && lhs.y == rhs.y
            && lhs.z == rhs.z && lhs.bounds == rhs.bounds;
    }

    struct CartesianExclusionZone {
        float minX;
        float minY;
        float minZ;
        float maxX;
        float maxY;
        float maxZ;
    };
    inline bool operator==(const CartesianExclusionZone& lhs, const CartesianExclusionZone& rhs)
    {
        return lhs.minX == rhs.minX && lhs.maxX == rhs.maxX
            && lhs.minY == rhs.minY && lhs.maxY == rhs.maxY
            && lhs.minZ == rhs.minZ && lhs.maxZ == rhs.maxZ;
    }

    struct PolarExclusionZone {
        float minElevation;
        float maxElevation;
        float minAzimuth;
        float maxAzimuth;
    };
    inline bool operator==(const PolarExclusionZone& lhs, const PolarExclusionZone& rhs)
    {
        return lhs.minAzimuth == rhs.minAzimuth && lhs.maxAzimuth == rhs.maxAzimuth
            && lhs.minElevation == rhs.minElevation && lhs.maxElevation == rhs.maxElevation;
    }

    class ExclusionZone
    {
    public:
        ExclusionZone() = default;

        ExclusionZone(const PolarExclusionZone& polarExZone)
            : m_polarZone(polarExZone), m_cartesianZone()
        {
        }

        ExclusionZone(const CartesianExclusionZone& cartesianExZone)
            : m_polarZone(), m_cartesianZone(cartesianExZone)
        {
        }

        PolarExclusionZone& polarZone()
        {
            // On accessing make sure that there is a PolarExclusionZone to modify
            if (!m_polarZone.hasValue())
                m_polarZone = PolarExclusionZone();
            // Reset the CartesianPolarZone since only one can be set at a time
            m_cartesianZone.reset();
            return m_polarZone.value();
        }

        const PolarExclusionZone& polarZone() const { return m_polarZone.value(); }

        CartesianExclusionZone& cartesianZone()
        {
            // On accessing make sure that there is a CartesianPolarZone to modify
            if (!m_cartesianZone.hasValue())
                m_cartesianZone = CartesianExclusionZone();
            // Reset the PolarExclusionZone since only one can be set at a time
            m_polarZone.reset();
            return m_cartesianZone.value();
        }

        const CartesianExclusionZone& cartesianZone() const { return m_cartesianZone.value(); }

        bool isPolarZone() const
        {
            assert(!(m_polarZone.hasValue() && m_cartesianZone.hasValue())); // These should never both have values
            return m_polarZone.hasValue();
        }

        ExclusionZone& operator=(const PolarExclusionZone& polarExZone)
        {
            m_polarZone = polarExZone;
            m_cartesianZone.reset();
            return *this;
        }

        ExclusionZone& operator=(const CartesianExclusionZone& cartesianExZone)
        {
            m_cartesianZone = cartesianExZone;
            m_polarZone.reset();
            return *this;
        }

        inline bool operator== (const ExclusionZone& rhs) const
        {
            return m_polarZone == rhs.m_polarZone && m_cartesianZone == rhs.m_cartesianZone;
        }

    private:
        Optional<PolarExclusionZone> m_polarZone;
        Optional<CartesianExclusionZone> m_cartesianZone;
    };

    class ObjectPosition
    {
    public:
        ObjectPosition() = default;

        ObjectPosition(const PolarPosition<double>& polarPos)
            : m_polarPosition(polarPos), m_cartesianPosition()
        {
        }

        ObjectPosition(const CartesianPosition<double>& cartesianPos)
            : m_polarPosition(), m_cartesianPosition(cartesianPos)
        {
        }

        PolarPosition<double>& polarPosition()
        {
            // On accessing make sure that there is a PolarPosition to modify
            if (!m_polarPosition.hasValue())
                m_polarPosition = PolarPosition<double>();
            // Reset the CartesianPosition<double> since only one can be set at a time
            m_cartesianPosition.reset();
            return m_polarPosition.value();
        }

        const PolarPosition<double>& polarPosition() const { return m_polarPosition.value(); }

        CartesianPosition<double>& cartesianPosition()
        {
            // On accessing make sure that there is a CartesianPosition<double> to modify
            if (!m_cartesianPosition.hasValue())
                m_cartesianPosition = CartesianPosition<double>();
            // Reset the PolarPosition since only one can be set at a time
            m_polarPosition.reset();
            return m_cartesianPosition.value();
        }

        const CartesianPosition<double>& cartesianPosition() const { return m_cartesianPosition.value(); }

        bool isPolar() const
        {
            assert(!(m_polarPosition.hasValue() && m_cartesianPosition.hasValue())); // These should never both have values
            return m_polarPosition.hasValue();
        }

        ObjectPosition& operator=(const PolarPosition<double>& polarPos)
        {
            m_polarPosition = polarPos;
            m_cartesianPosition.reset();
            return *this;
        }

        ObjectPosition& operator=(const CartesianPosition<double>& cartesianPos)
        {
            m_cartesianPosition = cartesianPos;
            m_polarPosition.reset();
            return *this;
        }

        inline bool operator==(const ObjectPosition& rhs) const
        {
            return m_polarPosition == rhs.m_polarPosition && m_cartesianPosition == rhs.m_cartesianPosition;
        }

    private:
        Optional<PolarPosition<double>> m_polarPosition;
        Optional<CartesianPosition<double>> m_cartesianPosition;
    };

    // Metadata for different objects. See Rec. ITU-R BS.2127-0 page 86.

    /** The metadata for ObjectType */
    struct ObjectMetadata
    {
        ObjectPosition position;
        // Gain of the Object metadata
        double gain = 1.0;
        // Diffuseness parameter
        double diffuse = 0.0;
        // Channel lock distance. values < 0 mean no processing is applied
        Optional<ChannelLock> channelLock;
        // Object divergence parameters
        Optional<ObjectDivergence> objectDivergence;
        // Flag if cartesian position coordinates
        bool cartesian = false;
        // Extent parameters
        double width = 0.0;
        double height = 0.0;
        double depth = 0.0;
        // Jump position to determine how the gains are interpolated
        JumpPosition jumpPosition;
        // The track index (starting from 0)
        unsigned int trackInd = 0;
        std::vector<ExclusionZone> zoneExclusion;
        // Screen reference for screen scaling
        bool screenRef = false;
        // Screen lock
        ScreenEdgeLock screenEdgeLock;
        // The length of the block in samples
        unsigned int blockLength = 0;
        // The reference screen
        Screen referenceScreen;
    };
    inline bool operator==(const ObjectMetadata& lhs, const ObjectMetadata& rhs)
    {
        return lhs.position == rhs.position
            && lhs.gain == rhs.gain && lhs.diffuse == rhs.diffuse
            && lhs.channelLock == rhs.channelLock && lhs.objectDivergence == rhs.objectDivergence
            && lhs.width == rhs.width && lhs.height == rhs.height && lhs.depth == rhs.depth
            && lhs.cartesian == rhs.cartesian && lhs.jumpPosition == rhs.jumpPosition
            && lhs.trackInd == rhs.trackInd && lhs.zoneExclusion == rhs.zoneExclusion
            && lhs.screenEdgeLock == rhs.screenEdgeLock && lhs.screenRef == rhs.screenRef
            && lhs.blockLength == rhs.blockLength;
    }

    /** The metadata for HoaType */
    struct HoaMetadata
    {
        // A vector containing the HOA orders of each of the channels
        std::vector<int> orders;
        // The degree of each channel where -order <= degree <= +order
        std::vector<int> degrees;
        // The normalization scheme of the HOA signal
        std::string normalization = "SN3D";
        std::vector<unsigned int> trackInds;
    };
    inline bool operator==(const HoaMetadata& lhs, const HoaMetadata& rhs)
    {
        return lhs.orders == rhs.orders && lhs.degrees == rhs.degrees
            && lhs.normalization == rhs.normalization && lhs.trackInds == rhs.trackInds;
    }

    /** The metadata for DirectSpeaker. See See Rec. ITU-R BS.2127-0 page 63. */
    struct DirectSpeakerMetadata
    {
        // The speaker labels from the stream metadata
        std::string speakerLabel = {};
        // The position of the loudspeaker
        DirectSpeakerPolarPosition polarPosition;
        // The track index (starting from 0)
        unsigned int trackInd = 0;
        // audioPackFormatID
        Optional<std::string> audioPackFormatID;
        // Channel frequency information
        Frequency channelFrequency;
        // Screen edge lock
        ScreenEdgeLock screenEdgeLock;
    };
    inline bool operator==(const DirectSpeakerMetadata& lhs, const DirectSpeakerMetadata& rhs)
    {
        return lhs.speakerLabel == rhs.speakerLabel && lhs.polarPosition == rhs.polarPosition
            && lhs.trackInd == rhs.trackInd && lhs.audioPackFormatID == rhs.audioPackFormatID
            && lhs.channelFrequency == rhs.channelFrequency;
    }

    /** Information about all of the channels in the stream.Contains the type of each track and the number of channels */
    struct StreamInformation
    {
        std::vector<TypeDefinition> typeDefinition;
        unsigned int nChannels = 0;
    };

    /** Mapping from common definitions audioPackFormatID to layout name. Rec.ITU-R BS.2127-0 Table 15 */
    const std::map<std::string, std::string> ituPackNames = { {"AP_00010001", "0+1+0"},
    {"AP_00010002", "0+2+0"},
    {"AP_0001000c", "0+5+0"},
    {"AP_00010003", "0+5+0"},
    {"AP_00010004", "2+5+0"},
    {"AP_00010005", "4+5+0"},
    {"AP_00010010", "4+5+1"},
    {"AP_00010007", "3+7+0"},
    {"AP_00010008", "4+9+0"},
    {"AP_00010009", "9+10+3"},
    {"AP_0001000f", "0+7+0"},
    {"AP_00010017", "4+7+0"},
    };

} // namespace spaudio

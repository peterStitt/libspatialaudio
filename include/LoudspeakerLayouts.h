/*############################################################################*/
/*#                                                                          #*/
/*#  Loudspeaker layouts.		                                             #*/
/*#								                                             #*/
/*#  Copyright © 2020 Peter Stitt                                            #*/
/*#                                                                          #*/
/*#  Filename:      AdmLayout.h	                                             #*/
/*#  Version:       0.1                                                      #*/
/*#  Date:          23/06/2020                                               #*/
/*#  Author(s):     Peter Stitt                                              #*/
/*#  Licence:       LGPL + proprietary                                       #*/
/*#                                                                          #*/
/*############################################################################*/


#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "AdmMetadata.h"
#include "Coordinates.h"
#include "ScreenCommon.h"
#include "AdmConversions.h"

namespace admrender {

	/** The different output layouts supported by Renderer */
	enum class OutputLayout
	{
		Stereo = 0, // 2.0 - ITU-R BS.2051-3 System A (0+2+0)
		Quad, // Channel order: FrontLeft, FrontRight, BackLeftMid, BackRightMid
		FivePointOne, // 5.1 - ITU-R BS.2051-3 System B (0+5+0)
		FivePointOnePointTwo, // 5.1.2 - ITU-R BS.2051-3 System C (2+5+0)
		FivePointOnePointFour, // 5.1.4 - ITU-R BS.2051-3 System D (4+5+0)
		FivePointOnePointFourPlusLow, // ITU-R BS.2051-3 System E (4+5+1)
		SevenPointOnePointThree, // 7.1.3 - ITU-R BS.2051-3 System F (3+7+0)
		ThirteenPointOne, // 13.1 (or 7.1.4+screens) - ITU-R BS.2051-3 System G (4+9+0)
		TwentyTwoPointTwo, // 22.2 - ITU-R BS.2051-3 System H (9+10+3)
		SevenPointOne, // 7.1 - ITU-R BS.2051-3 System I (0+7+0)
		SevenPointOnePointFour, // 7.1.4 - ITU-R BS.2051-3 System J (4+7+0)
		BEAR_9_10_5, // BEAR layout. 9+10+3 with 2 extra lower speakers
		SevenPointOnePointTwo, // 7.1.2 layout specified in IAMF v1.0.0
		ThreePointOnePointTwo, // 3.1.2 layout specified in IAMF v1.0.0
		Binaural
	};

	/** The different channel types from Table 1A in Rec. ITU-R  BS.2094-2 */
	enum class ChannelTypes
	{
		Custom = -1, // A channel not defined in Rec. ITU-R  BS.2094-2
		FrontLeft = 0, // M+030
		FrontRight, // M-030
		FrontCentre, // M+000
		LFE,
		SurroundLeft, // M+110
		SurroundRight, // M-110
		FrontLeftOfCentre, // M+022
		FrontRightOfCentre, // M-022
		BackCentre, // M+180
		SideLeft, // M+090
		SideRight, // M-090
		TopCentre, // T+000
		TopFrontLeft, // U+030
		TopFrontCentre, // U+000
		TopFrontRight, // U-030
		TopSurroundLeft, // U+110
		TopBackCentre, // U+180
		TopSurroundRight, // U-110
		TopSideLeft, // U+090
		TopSideRight, // U-090
		BottomFrontCentre, // B+000
		BottonFrontLeftMid, // B+045
		BottomFrontRightMid, // B-045
		FrontLeftWide, // M+060
		FrontRightWide, // M-060
		BackLeftMid, // M+135
		BackRightMid, // M-135
		TopBackLeftMid, // U+135
		TopBackRightMid, // U-135
		LFE1,
		LFE2,
		TopFrontLeftMid, // U+045
		TopFrontRightMid, // U-045
		FrontLeftScreen, // M+SC
		FrontRightScreen, // M-SC
		FrontLeftMid, // M+045
		FrontRightMid, // M-045
		UpperTopBackCentre, // UH+180
		BackLeft, // M+150
		BackRight, // M-150
		BottomFrontLeft, // B+030
		BottomFrontRight, // B-030
		BottomBackLeft, // B+135 (Not in Rec. ITU-R  BS.2094-2. Used for BEAR layout)
		BottomBackRight, // B-135 (Not in Rec. ITU-R  BS.2094-2. Used for BEAR layout)
		ACN0, ACN1, ACN2, ACN3, ACN4, ACN5, ACN6, ACN7, ACN8,
		ACN9, ACN10, ACN11, ACN12,ACN13, ACN14, ACN15, ACN16
	};

	namespace bs2094 {
		/** List of labels for audio channels from Rec. ITU-R BS.2094-2 Table 1A. Includes */
		static const std::vector<std::string> channelLabels = {
			"M+030", "M-030", "M+000", "LFE", "M+110", "M-110",
			"M+022", "M-022", "M+180", "M+090", "M-090", "T+000",
			"U+030", "U+000", "U-030", "U+110", "U+180", "U-110",
			"U+090", "U-090", "B+000", "B+045", "B-045", "M+060", "M-060",
			"M+135", "M-135", "U+135", "U-135", "LFE1", "LFE2",
			"U+045", "U-045", "M+SC", "M-SC", "M+045", "M-045",
			"UH+180", "M+150", "M-150", "B+030", "B-030",
			"B+135", // Not in Rec. ITU-R BS.2094-1. Used in BEAR 9+10+3 plus 2 layout.
			"B-135", // Not in Rec. ITU-R BS.2094-1. Used in BEAR 9+10+3 plus 2 layout.
			"ACN0", "ACN1", "ACN2", "ACN3", "ACN4", "ACN5", "ACN6", "ACN7", "ACN8",
			"ACN9", "ACN10", "ACN11", "ACN12","ACN13", "ACN14", "ACN15", "ACN16",
			"" /* empty to indicate no appropriate channel name */
		};

		/** Directions of audio channels from Rec. ITU-R BS.2094-2 Table 1A. */
		static const std::vector<PolarPosition> positions = {
			PolarPosition{30.,0.,1.}, // FrontLeft - M+030
			PolarPosition{-30.,0.,1.}, // FrontRight - M-030
			PolarPosition{0.,0.,1.}, // FrontCentre - M+000
			PolarPosition{0.,-30.,1.}, // LFE - LFE
			PolarPosition{110.,0.,1.}, // SurroundLeft - M+110
			PolarPosition{-110.,0.,1.}, // SurroundRight - M-110
			PolarPosition{22.5,0.,1.}, // FrontLeftOfCentre - M+022
			PolarPosition{-22.5,0.,1.}, // FrontRightOfCentre - M-022
			PolarPosition{180.,0.,1.}, // BackCentre - M+180
			PolarPosition{90.,0.,1.}, // SideLeft - M+090
			PolarPosition{-90.,0.,1.}, // SideRight - M-090
			PolarPosition{0.,90.,1.}, // TopCentre - T+000
			PolarPosition{30.,30.,1.}, // TopFrontLeft - U+030
			PolarPosition{0.,30.,1.}, // TopFrontCentre - U+000
			PolarPosition{-30.,30.,1.}, // TopFrontRight - U-030
			PolarPosition{110.,30.,1.}, // TopSurroundLeft - U+110
			PolarPosition{180.,30.,1.}, // TopBackCentre - U+180
			PolarPosition{-110.,30.,1.}, // TopSurroundRight - U-110
			PolarPosition{90.,30.,1.}, // TopSideLeft - U+090
			PolarPosition{-90.,30.,1.}, // TopSideRight - U-090
			PolarPosition{0.,-30.,1.}, // BottomFrontCentre - B+000
			PolarPosition{45.,-30.,1.}, // BottonFrontLeftMid - B+045
			PolarPosition{-45.,-30.,1.}, // BottomFrontRightMid - B-045
			PolarPosition{60.,0.,1.}, // FrontLeftWide - M+060
			PolarPosition{-60.,0.,1.}, // FrontRightWide - M-060
			PolarPosition{135.,0.,1.}, // BackLeftMid - M+135
			PolarPosition{-135.,0.,1.}, // BackRightMid - M-135
			PolarPosition{135.,30.,1.}, // TopBackLeftMid - U+135
			PolarPosition{-135.,30.,1.}, // TopBackRightMid - U-135
			PolarPosition{45.,-30.,1.}, // LFE1 - LFE1
			PolarPosition{-45.,-30.,1.}, // LFE2 - LFE2
			PolarPosition{45.,30.,1.}, // TopFrontLeftMid - U+045
			PolarPosition{-45.,30.,1.}, // TopFrontRightMid - U-045
			PolarPosition{25.,0.,1.}, // FrontLeftScreen - M+SC
			PolarPosition{-25.,0.,1.}, // FrontRightScreen - M-SC
			PolarPosition{45.,0.,1.}, // FrontLeftMid - M+045
			PolarPosition{-45.,0.,1.}, // FrontRightMid - M-045
			PolarPosition{180.,45.,1.}, // UpperTopBackCentre - UH+180
			PolarPosition{150.,0.,1.}, // BackLeft - M+150
			PolarPosition{-150.,0.,1.}, // BackRight - M-150
			PolarPosition{30.,-30.,1.}, // BottomFrontLeft - B+030
			PolarPosition{-30.,-30.,1.}, // BottomFrontRight - B-030
			PolarPosition{135.,-30.,1.}, // BottomBackLeft - B+135
			PolarPosition{-135.,-30.,1.} // BottomBackRight - B-135
		};
	} // namespace bs2094
}

/** If the the speaker label is in the format "urn:itu:bs:2051:[0-9]:speaker:X+YYY then
 *  return the X+YYY portion. Otherwise, returns the original input.
 * @param label		String containing the label.
 * @return			String of the nominal speaker label (X+YYY portion of input).
 */
static inline const std::string& GetNominalSpeakerLabel(const std::string& label)
{
	auto& channelLabels = admrender::bs2094::channelLabels;
	for (size_t i = 0; i < channelLabels.size(); ++i)
		if (stringContains(label, channelLabels[i]) && channelLabels[i] != "LFE")
			return channelLabels[i];

	// Rename the LFE channels, if requried.
	// See Rec. ITU-R BS.2127-0 sec 8.3
	if (stringContains(label, "LFE") || stringContains(label,"LFEL"))
		return channelLabels[31];
	else if (stringContains(label, "LFER"))
		return channelLabels[32];

	return channelLabels.back();
}

// Information about a speaker channel (as opposed to an audio channel, which could be Object, HOA, etc.)
class Channel
{
public:
	/** Default constructor. Creates a FrontCentre channel */
	Channel();

	/** Constructor for a custom channel type */
	Channel(std::string& channelName, PolarPosition position, PolarPosition positionNominal, bool channelLfe);

	/** Constructor to initialise from a specific channel type */
	Channel(admrender::ChannelTypes channelType);

	/** Constructor to initialise from a one of the channel names in bs2094::channelLabels.
	 * If an unknown string is passed then the channel will be set to a Custom type and its position
	 * to 0deg azimuth and 0deg elevation (i.e. directly to the front).
	 */
	Channel(const std::string& channelName);
	~Channel();

	std::string name;
	admrender::ChannelTypes channelType;
	// Real loudspeaker position
	PolarPosition polarPosition;
	// Nominal loudspeaker position from ITU-R BS.2051-2
	PolarPosition polarPositionNominal;
	bool isLFE = false;

private:
	bool isChannelLFE();
};

// Class used to store the layout information
class Layout
{
public:
	Layout();
	Layout(std::string layoutName, std::vector<Channel> layoutChannels, bool layoutHasLfe, bool layoutIsHoa = false, unsigned int layoutOrder = 0);
	Layout(admrender::OutputLayout layoutType);
	Layout(std::string& layoutName);

	std::string name;
	std::vector<Channel> channels;
	bool hasLFE = false;

	bool isHoa = false;
	unsigned int hoaOrder = 0;

	admrender::Optional<Screen> reproductionScreen = admrender::Optional<Screen>();

	/** If the channel name matches one of the channels in the Layout then return
	 *  its index. If not, return -1.
	 * @param channelName	String containing the name of the channel.
	 * @return				The index of the channel in the layout. If it is not present, returns -1.
	 */
	int getMatchingChannelIndex(const std::string& channelName);

	/** Returns a list of the channel names in order. */
	std::vector<std::string> channelNames() const;

	/** Returns true if the layout contains the specified channel.
	 * @param channelName Name of the channel to check for.
	 * @return Returns true if channelName is present in the layout
	 */
	bool containsChannel(const std::string& channelName) const;

	/** Returns a version of the input layout without any LFE channels.
	 * @param layout	Input layout.
	 * @return			Copy of the input layout with any LFE channels removed.
	 */
	static Layout getLayoutWithoutLFE(Layout layout);

	static const std::vector<Layout>& getSpeakerLayouts();

	/** Get the speakerLayout that matches the given name. If no match then returns empty.
	 * @param layoutName	String containing the name of the desired speaker layout.
	 * @return				The speaker layout matching the name. Returns an empty layout if none is found.
	 */
	static Layout getMatchingLayout(std::string layoutName);

private:
};

/** Check if the input DirectSpeakerMetadata is for an LFE channel.
* @param metadata	Metadata to check.
* @return			Returns true if the input metadata refers to an LFE channel.
*/
static inline bool isLFE(const admrender::DirectSpeakerMetadata& metadata)
{
	// See Rec. ITU-R BS.2127-1 sec. 6.3
	if (metadata.channelFrequency.lowPass.hasValue())
		if (metadata.channelFrequency.lowPass.value() <= 120.)
			return true;

	const std::string& nominalLabel = GetNominalSpeakerLabel(metadata.speakerLabel);
	if (stringContains(nominalLabel, "LFE1") || stringContains(nominalLabel, "LFE2"))
	{
		return true;
	}
	return false;
}

/** Check the loudspeaker positions are within the valid ranges. See Rec. ITU-R BS.2127-1 Sec. 3.1.
 * @param layout Loudspeaker layout to check.
 * @return Returns true if all loudspeakers are in the valid range.
 */
static inline bool checkLayoutAngles(const Layout& layout)
{
	/** Angular ranges for each loudspeaker for all loudspeaker layouts.
	Where possible, ranges are taken from Rec. ITU-R BS.2051-3. If the layout is not
	in Rec. ITU-R BS.2051-3 then the nearest equivalent is used.
	*/
	struct ChannelRanges
	{
		std::pair<double, double> azRange;
		std::pair<double, double> elRange;
	};
	static const std::map<std::string, std::map<std::string, ChannelRanges>> speakerRanges = {
		// Stereo - BS.2051-3 System A 0+2+0
		{"0+2+0",{ {"M+030",{{30., 30.}, {0.,0.}}},
		{"M-030",{{-30., -30.}, {0.,0.}} }}},
		// Quad
		{"0+4+0",{ {"M+045",{{45., 45.}, {0.,0.}}},
		{"M-045",{{-45., -45.}, {0.,0.}}},
		{"M+135",{{135., 135.}, {0.,0.}}},
		{"M-135",{{-135., -135.}, {0.,0.}}}
		}},
		// 5.1 - BS.2051-3 System B 0+5+0
		{"0+5+0",{ {"M+030",{{30., 30.}, {0.,0.}}},
		{"M-030",{{-30., -30.}, {0.,0.}}},
		{"M+000",{{0., 0.}, {0.,0.}}},
		{"LFE1",{{-180., 180.}, {-90.,90.}}},
		{"M+110",{{100., 120.}, {0.,15.}}},
		{"M-110",{{-120., -100.}, {0.,15.}}}
		}},
		// 5.1.2 - BS.2051-3 System C 2+5+0
		{"2+5+0",{ {"M+030",{{30., 30.}, {0.,0.}}},
		{"M-030",{{-30., -30.}, {0.,0.}}},
		{"M+000",{{0., 0.}, {0.,0.}}},
		{"LFE1",{{-180., 180.}, {-90.,90.}}},
		{"M+110",{{100., 120.}, {0.,15.}}},
		{"M-110",{{-120., -100.}, {0.,15.}}},
		{"U+030",{{30., 45.}, {30.,55.}}},
		{"U-030",{{-45., -30.}, {30.,55.}}}
		}},
		// 5.1.4 - BS.2051-3 System D 4+5+0
		{"4+5+0",{ {"M+030",{{30., 30.}, {0.,0.}}},
		{"M-030",{{-30., -30.}, {0.,0.}}},
		{"M+000",{{0., 0.}, {0.,0.}}},
		{"LFE1",{{-180., 180.}, {-90.,90.}}},
		{"M+110",{{100., 120.}, {0.,15.}}},
		{"M-110",{{-120., -100.}, {0.,15.}}},
		{"U+030",{{30., 45.}, {30.,55.}}},
		{"U-030",{{-45., -30.}, {30.,55.}}},
		{"U+110",{{110., 135.}, {30.,55.}}},
		{"U-110",{{-135., -110.}, {30.,55.}}}
		}},
		// BS.2051-3 System E 4+5+1
		{"4+5+1",{ {"M+030",{{30., 30.}, {0.,0.}}},
		{"M-030",{{-30., -30.}, {0.,0.}}},
		{"M+000",{{0., 0.}, {0.,0.}}},
		{"LFE1",{{-180., 180.}, {-90.,90.}}},
		{"M+110",{{100., 120.}, {0.,15.}}},
		{"M-110",{{-120., -100.}, {0.,15.}}},
		{"U+030",{{30., 45.}, {30.,55.}}},
		{"U-030",{{-45., -30.}, {30.,55.}}},
		{"U+110",{{110., 135.}, {30.,55.}}},
		{"U-110",{{-135., -110.}, {30.,55.}}},
		{"B+000",{{0., 0.}, {-30., -15.}}}
		}},
		// BS.2051-3 System F 3+7+0
		{"3+7+0",{ {"M+000",{{0.,0.}, {0.,0.}}},
		{"M+030",{{30., 30.}, {0.,0.}}},
		{"M-030",{{-30., -30.}, {0.,0.}}},
		{"U+045",{{30., 45.}, {30.,45.}}},
		{"U-045",{{-45., -30.}, {30.,45.}}},
		{"M+090",{{60., 150.}, {0.,0.}}},
		{"M-090",{{-150., -60.}, {0.,0.}}},
		{"M+135",{{60., 150.}, {0.,0.}}},
		{"M-135",{{-150., -60.}, {0.,0.}}},
		{"UH+180",{{180., 180}, {45.,90.}}},
		{"LFE1",{{-180., 180.}, {-90.,90.}}},
		{"LFE2",{{-180., 180.}, {-90.,90.}}}
		}},
		// BS.2051-3 System G 4+9+0 - Note: doesn't include the screen speakers because these are defined as in Rec. ITU-R BS.2127-1 Sec. 7.3.9
		{"4+9+0",{ {"M+030",{{30., 45.}, {0.,0.}}},
		{"M-030",{{-45., -30.}, {0.,0.}}},
		{"M+000",{{0., 0.}, {0.,0.}}},
		{"LFE1",{{-180., 180.}, {-90.,90.}}},
		{"M+090",{{85., 110.}, {0.,0.}}},
		{"M-090",{{-110., -85.}, {0.,0.}}},
		{"M+135",{{120., 150.}, {0.,0.}}},
		{"M-135",{{-150., -120.}, {0.,0.}}},
		{"U+045",{{30., 45.}, {30.,55.}}},
		{"U-045",{{-45., -30.}, {30.,55.}}},
		{"U+135",{{100., 150.}, {30.,55.}}},
		{"U-135",{{-150., -100.}, {30.,55.}}},
		{"M+SC",{{5., 25.}, {0.,0.}} },
		{"M-SC",{{-25., -5.}, {0.,0.}}}
		}},
		// BS.2051-3 System H 9+10+3
		{"9+10+3",{ {"M+060",{{45., 60.}, {0.,5.}}},
		{"M-060",{{-60., -45.}, {0.,5.}}},
		{"M+000",{{0., 0.}, {0.,5.}}},
		{"LFE1",{{30., 90.}, {-30.,-15.}}},
		{"M+135",{{110., 135.}, {0.,15.}}},
		{"M-135",{{-135., -110.}, {0.,15.}}},
		{"M+030",{{22.5, 30.}, {0.,5.}}},
		{"M-030",{{-30., -22.5}, {0.,5.}}},
		{"M+180",{{180., 180.}, {0.,15.}}},
		{"LFE2",{{-90., -30.}, {-30.,-15.}}},
		{"M+090",{{90., 90.}, {0.,15.}}},
		{"M-090",{{-90., -90.}, {0.,15.}}},
		{"U+045",{{45., 60.}, {30.,45.}}},
		{"U-045",{{-60., -45.}, {30.,45.}}},
		{"U+000",{{0., 0.}, {30.,45.}}},
		{"T+000",{{-180., 180.}, {90.,90.}}},
		{"U+135",{{110., 135.}, {30.,45.}}},
		{"U-135",{{-135., -110.}, {30.,45.}}},
		{"U+090",{{90., 90.}, {30.,45.}}},
		{"U-090",{{-90., -90.}, {30.,45.}}},
		{"U+180",{{180., 180.}, {30.,45.}}},
		{"B+000",{{0., 0.}, {-30.,-15.}}},
		{"B+045",{{45., 60.}, {-30.,-15.}}},
		{"B-045",{{-60., -45.}, {-30.,-15.}}}
		}},
		// 7.1 - BS.2051-3 System I 0+7+0
		{ "0+7+0",{ {"M+030",{{30., 45.}, {0.,0.}}},
		{"M-030",{{-45., -30.}, {0.,0.}}},
		{"M+000",{{0., 0.}, {0.,0.}}},
		{"LFE1",{{-180., 180.}, {-90.,90.}}},
		{"M+090",{{85., 110.}, {0.,0.}}},
		{"M-090",{{-110., -85.}, {0.,0.}}},
		{"M+135",{{120., 150.}, {0.,0.}}},
		{"M-135",{{-150., -120.}, {0.,0.}}}
		}},
		// 7.1.4 - BS.2051-3 System J 4+7+0
		{ "4+7+0",{ {"M+030",{{30., 45.}, {0.,0.}}},
		{"M-030",{{-45., -30.}, {0.,0.}}},
		{"M+000",{{0., 0.}, {0.,0.}}},
		{"LFE1",{{-180., 180.}, {-90.,90.}}},
		{"M+090",{{85., 110.}, {0.,0.}}},
		{"M-090",{{-110., -85.}, {0.,0.}}},
		{"M+135",{{120., 150.}, {0.,0.}}},
		{"M-135",{{-150., -120.}, {0.,0.}}},
		{"U+045",{{30., 45.}, {30.,55.}}},
		{"U-045",{{-45., -30.}, {30.,55.}}},
		{"U+135",{{100., 150.}, {30.,55.}}},
		{"U-135",{{-150., -100.}, {30.,55.}}}
		}},
		// 7.1.2 - IAMF v1.0
		{ "2+7+0",{ {"M+030",{{30., 45.}, {0.,0.}}},
		{"M-030",{{-45., -30.}, {0.,0.}}},
		{"M+000",{{0., 0.}, {0.,0.}}},
		{"LFE1",{{-180., 180.}, {-90.,90.}}},
		{"M+090",{{85., 110.}, {0.,0.}}},
		{"M-090",{{-110., -85.}, {0.,0.}}},
		{"M+135",{{120., 150.}, {0.,0.}}},
		{"M-135",{{-150., -120.}, {0.,0.}}},
		{"U+045",{{30., 45.}, {30.,55.}}},
		{"U-045",{{-45., -30.}, {30.,55.}}}
		}},
		// 3.1.2 - IAMF v1.0
		{ "2+3+0",{ {"M+030",{{30., 45.}, {0.,0.}}},
		{"M-030",{{-45., -30.}, {0.,0.}}},
		{"M+000",{{0., 0.}, {0.,0.}}},
		{"LFE1",{{-180., 180.}, {-90.,90.}}},
		{"U+045",{{30., 45.}, {30.,55.}}},
		{"U-045",{{-45., -30.}, {30.,55.}}},
		}},
		// BEAR: 9+10+3 plus 2 bottom layer speakers
		{ "9+10+5",{ {"M+060",{{45., 60.}, {0.,5.}}},
		{"M-060",{{-60., -45.}, {0.,5.}}},
		{"M+000",{{0., 0.}, {0.,5.}}},
		{"M+135",{{110., 135.}, {0.,15.}}},
		{"M-135",{{-135., -110.}, {0.,15.}}},
		{"M+030",{{22.5, 30.}, {0.,5.}}},
		{"M-030",{{-22.5,-30.}, {0.,5.}}},
		{"M+180",{{180., 180.}, {0.,15.}}},
		{"M+090",{{90., 90.}, {0.,15.}}},
		{"M-090",{{-90., -90.}, {0.,15.}}},
		{"U+045",{{45., 60.}, {30.,45.}}},
		{"U-045",{{-60., -45.}, {30.,45.}}},
		{"U+000",{{0., 0.}, {30.,45.}}},
		{"T+000",{{-180., 180.}, {90.,90.}}},
		{"U+135",{{110., 135.}, {30.,45.}}},
		{"U-135",{{-135., -110.}, {30.,45.}}},
		{"U+090",{{90., 90.}, {30.,45.}}},
		{"U-090",{{-90., -90.}, {30.,45.}}},
		{"U+180",{{180., 180.}, {30.,45.}}},
		{"B+000",{{0., 0.}, {-30.,-15.}}},
		{"B+045",{{45., 60.}, {-30.,-15.}}},
		{"B-045",{{-60., -45.}, {-30.,-15.}}},
		{"B+135",{{110., 135.}, {-30.,-15.}}},
		{"B-135",{{-135., -110.}, {-30.,-15.}}} }}
	};

	auto it = speakerRanges.find(layout.name);
	double tol = 1e-6;
	if (it != speakerRanges.end())
	{
		const auto& layoutRanges = it->second;

		for (auto& channel : layout.channels)
		{
			std::pair<double, double> azRange, elRange;
			auto& nominalLabel = GetNominalSpeakerLabel(channel.name);
			auto ranges = layoutRanges.find(nominalLabel);
			if (ranges != layoutRanges.end())
			{
				elRange = ranges->second.elRange;

				if (channel.name == "M+SC" || channel.name == "M-SC")
				{
					// "the absolute azimuth of both M+SC and M-SC loudspeakers must either be between 5 deg and 25 deg or
					// between 35° or 60°"
					std::pair<double, double> azRange1 = ranges->second.azRange;
					std::pair<double, double> azRange2 = { 35., 60. };
					elRange = { 0., 0. };
					double absAz = std::abs(channel.polarPosition.azimuth);
					if (!(insideAngleRange(channel.polarPosition.azimuth, azRange1.first, azRange1.second, tol) || insideAngleRange(absAz, azRange2.first, azRange2.second, tol)
						|| !insideAngleRange(channel.polarPosition.elevation, elRange.first, elRange.second)))
						return false;
				}
				else
				{
					azRange = ranges->second.azRange;
					if (!insideAngleRange(channel.polarPosition.azimuth, azRange.first, azRange.second, tol)
						|| !insideAngleRange(channel.polarPosition.elevation, elRange.first, elRange.second, tol))
						return false;
				}
			}
			else
				return false; // Input layout contains a speaker not in the predefined layout
		}
		return true;
	}
	return false;
}

/*
	Presets of supported output layouts used in the point source panner
*/

// 0 = FL, 1 = FR, 2 = BL, 3 = BR
const std::vector<std::vector<unsigned int>> HULL_0_4_0 = {
{4, 12, 6},{6, 12, 7},{8, 10, 13},{10, 11, 13},
{4, 5, 12},{5, 7, 12},{8, 13, 9},{9, 13, 11},
{0, 4, 6, 2},{2, 6, 7, 3},{0, 2, 10, 8},{2, 3, 11, 10},
{0, 1, 5, 4},{1, 3, 7, 5},{0, 8, 9, 1},{1, 9, 11, 3} };

const std::vector<std::vector<unsigned int>> HULL_0_5_0 = {
{5, 15, 8},{8, 15, 9},{5, 7, 15},{10, 13, 16},
{11, 16, 14},{13, 14, 16},{10, 16, 12},{11, 12, 16},
{6, 9, 15},{6, 15, 7},{3, 8, 9, 4},{0, 2, 7, 5},
{0, 5, 8, 3},{0, 3, 13, 10},{3, 4, 14, 13},{0, 10, 12, 2},
{1, 11, 14, 4},{1, 4, 9, 6},{1, 2, 12, 11},{1, 6, 7, 2} };

const std::vector<std::vector<unsigned int>> HULL_2_5_0 = {
{2, 5, 6},{5, 15, 6},{7, 14, 10},{10, 14, 11},
{7, 9, 14},{5, 12, 15},{0, 5, 2},{6, 15, 13},
{12, 13, 15},{8, 11, 14},{8, 14, 9},{1, 2, 6},
{3, 10, 11, 4},{0, 2, 9, 7},{0, 7, 10, 3},{0, 3, 12, 5},
{3, 4, 13, 12},{1, 6, 13, 4},{1, 4, 11, 8},{1, 8, 9, 2} };

const std::vector<std::vector<unsigned int>> HULL_4_5_0 = {
{2, 5, 6},{5, 15, 6},{9, 14, 12},{12, 14, 13},
{9, 11, 14},{5, 7, 15},{0, 5, 2},{6, 15, 8},
{7, 8, 15},{10, 13, 14},{10, 14, 11},{1, 2, 6},
{3, 12, 13, 4},{0, 2, 11, 9},{0, 9, 12, 3},{0, 3, 7, 5},
{3, 4, 8, 7},{1, 6, 8, 4},{1, 4, 13, 10},{1, 10, 11, 2} };

const std::vector<std::vector<unsigned int>> HULL_4_5_1 = {
{0, 10, 3},{10, 12, 11},{0, 2, 9},{9, 11, 12},
{9, 12, 10},{0, 9, 10},{6, 13, 8},{7, 8, 13},
{5, 13, 6},{5, 7, 13},{0, 5, 2},{2, 5, 6},
{1, 11, 9},{1, 4, 11},{1, 2, 6},{1, 9, 2},
{3, 10, 11, 4},{3, 4, 8, 7},{0, 3, 7, 5},{1, 6, 8, 4} };

const std::vector<std::vector<unsigned int>> HULL_3_7_0 = {
{4, 9, 6},{0, 3, 4},{3, 5, 9},{3, 9, 4},
{6, 9, 8},{15, 17, 16},{2, 4, 6},{0, 4, 2},
{14, 16, 17},{12, 14, 17},{10, 12, 17},{10, 17, 11},
{1, 5, 3},{0, 1, 3},{11, 17, 13},{13, 17, 15},
{5, 7, 9},{7, 8, 9},{6, 8, 16, 14},{2, 6, 14, 12},
{0, 2, 12, 10},{0, 10, 11, 1},{1, 11, 13, 5},{5, 13, 15, 7},
{7, 15, 16, 8} };

const std::vector<std::vector<unsigned int>> HULL_4_9_0 = {
{16, 22, 18},{18, 22, 19},{2, 7, 8},{7, 23, 8},
{7, 9, 23},{4, 8, 10},{8, 23, 10},{9, 10, 23},
{14, 17, 22},{17, 19, 22},{13, 22, 16},{1, 8, 4},
{3, 9, 7},{3, 5, 9},{4, 10, 6},{0, 3, 7},
{2, 8, 12},{1, 12, 8},{14, 22, 21},{15, 21, 22},
{2, 11, 7},{0, 7, 11},{13, 20, 22},{15, 22, 20},
{1, 4, 17, 14},{3, 16, 18, 5},{4, 6, 19, 17},{5, 18, 19, 6},
{5, 6, 10, 9},{0, 13, 16, 3},{1, 14, 21, 12},{2, 12, 21, 15},
{0, 11, 20, 13},{2, 15, 20, 11} };

const std::vector<std::vector<unsigned int>> HULL_4_9_0_wide = {
{16, 22, 18},{18, 22, 19},{4, 8, 10},{8, 23, 10},
{9, 10, 23},{17, 19, 22},{17, 22, 21},{4, 12, 8},
{16, 20, 22},{13, 22, 20},{13, 15, 22},{4, 10, 6},
{3, 5, 9},{1, 2, 8},{1, 8, 12},{2, 7, 8},
{7, 23, 8},{3, 7, 11},{7, 9, 23},{3, 9, 7},
{14, 21, 22},{14, 22, 15},{0, 7, 2},{0, 11, 7},
{4, 17, 21, 12},{4, 6, 19, 17},{5, 18, 19, 6},{5, 6, 10, 9},
{3, 16, 18, 5},{3, 11, 20, 16},{1, 12, 21, 14},{1, 14, 15, 2},
{0, 2, 15, 13},{0, 13, 20, 11} };

const std::vector<std::vector<unsigned int>> HULL_4_9_0_wideL = {
{16, 22, 18},{18, 22, 19},{2, 7, 8},{7, 23, 8},
{7, 9, 23},{4, 8, 10},{8, 23, 10},{9, 10, 23},
{14, 17, 22},{17, 19, 22},{1, 8, 4},{3, 9, 7},
{3, 5, 9},{4, 10, 6},{13, 15, 22},{0, 7, 2},
{2, 8, 12},{1, 12, 8},{14, 22, 21},{15, 21, 22},
{3, 7, 11},{0, 11, 7},{16, 20, 22},{13, 22, 20},
{1, 4, 17, 14},{3, 16, 18, 5},{4, 6, 19, 17},{5, 18, 19, 6},
{5, 6, 10, 9},{0, 2, 15, 13},{1, 14, 21, 12},{2, 12, 21, 15},
{3, 11, 20, 16},{0, 13, 20, 11} };

const std::vector<std::vector<unsigned int>> HULL_4_9_0_wideR = {
{16, 22, 18},{18, 22, 19},{13, 22, 16},{4, 8, 10},
{8, 23, 10},{9, 10, 23},{17, 19, 22},{17, 22, 21},
{4, 12, 8},{2, 7, 8},{7, 23, 8},{7, 9, 23},
{4, 10, 6},{13, 20, 22},{15, 22, 20},{14, 21, 22},
{14, 22, 15},{3, 9, 7},{3, 5, 9},{0, 3, 7},
{1, 2, 8},{1, 8, 12},{2, 11, 7},{0, 7, 11},
{4, 17, 21, 12},{4, 6, 19, 17},{5, 18, 19, 6},{5, 6, 10, 9},
{3, 16, 18, 5},{0, 13, 16, 3},{1, 14, 15, 2},{1, 12, 21, 14},
{2, 15, 20, 11},{0, 11, 20, 13} };

const std::vector<std::vector<unsigned int>> HULL_9_10_3 = {
{23, 24, 27},{19, 21, 27},{23, 27, 26},{21, 26, 27},
{13, 18, 15},{11, 12, 13},{2, 6, 19},{6, 21, 19},
{2, 12, 6},{6, 12, 11},{13, 15, 17},{11, 13, 17},
{22, 27, 24},{19, 27, 20},{13, 14, 18},{10, 13, 12},
{1, 21, 6},{1, 9, 26},{1, 26, 21},{1, 6, 11},
{1, 17, 9},{1, 11, 17},{22, 25, 27},{20, 27, 25},
{13, 16, 14},{10, 16, 13},{0, 25, 8},{0, 20, 25},
{0, 8, 16},{0, 16, 10},{0, 5, 20},{2, 19, 5},
{5, 19, 20},{0, 10, 5},{2, 5, 12},{5, 10, 12},
{4, 7, 24, 23},{4, 15, 18, 7},{4, 23, 26, 9},{4, 9, 17, 15},
{3, 8, 25, 22},{3, 22, 24, 7},{3, 7, 18, 14},{3, 14, 16, 8} };

const std::vector<std::vector<unsigned int>> HULL_0_7_0 = {
{10, 21, 12},{12, 21, 13},{17, 19, 22},{19, 20, 22},
{8, 11, 21},{11, 13, 21},{7, 21, 10},{15, 16, 22},
{15, 22, 18},{18, 22, 20},{8, 21, 9},{7, 9, 21},
{14, 17, 22},{14, 22, 16},{1, 4, 11, 8},{1, 2, 16, 15},
{4, 6, 13, 11},{5, 12, 13, 6},{5, 6, 20, 19},{1, 15, 18, 4},
{4, 18, 20, 6},{1, 8, 9, 2},{3, 10, 12, 5},{3, 5, 19, 17},
{0, 7, 10, 3},{0, 2, 9, 7},{0, 3, 17, 14},{0, 14, 16, 2} };

const std::vector<std::vector<unsigned int>> HULL_4_7_0 = {
{14, 18, 16},{16, 18, 17},{2, 7, 8},{7, 19, 8},
{7, 9, 19},{4, 8, 10},{8, 19, 10},{9, 10, 19},
{12, 15, 18},{15, 17, 18},{11, 18, 14},{1, 2, 8},
{1, 8, 4},{3, 9, 7},{3, 5, 9},{4, 10, 6},
{12, 18, 13},{11, 13, 18},{0, 7, 2},{0, 3, 7},
{1, 4, 15, 12},{3, 14, 16, 5},{4, 6, 17, 15},{5, 16, 17, 6},
{5, 6, 10, 9},{1, 12, 13, 2},{0, 2, 13, 11},{0, 11, 14, 3} };

const std::vector<std::vector<unsigned int>> HULL_2_7_0 = {
{12, 20, 14},{14, 20, 15},{2, 7, 8},{7, 21, 8},
{18, 19, 21},{10, 13, 20},{13, 15, 20},{9, 20, 12},
{1, 2, 8},{1, 8, 4},{8, 21, 17},{17, 21, 19},
{4, 8, 17},{10, 20, 11},{9, 11, 20},{0, 7, 2},
{0, 3, 7},{3, 16, 7},{16, 18, 21},{7, 16, 21},
{1, 4, 13, 10},{3, 12, 14, 5},{4, 6, 15, 13},{5, 14, 15, 6},
{5, 6, 19, 18},{4, 17, 19, 6},{1, 10, 11, 2},{0, 2, 11, 9},
{0, 9, 12, 3},{3, 5, 18, 16} };

const std::vector<std::vector<unsigned int>> HULL_9_10_5 = {
{9, 23, 21},{21, 23, 25},{23, 24, 25},{19, 21, 25},
{13, 18, 15},{4, 23, 9},{11, 12, 13},{2, 6, 19},
{6, 21, 19},{2, 12, 6},{6, 12, 11},{13, 15, 17},
{11, 13, 17},{22, 25, 24},{19, 25, 20},{20, 25, 22},
{8, 20, 22},{13, 14, 18},{10, 13, 12},{1, 9, 21},
{1, 21, 6},{1, 6, 11},{1, 17, 9},{1, 11, 17},
{3, 8, 22},{13, 16, 14},{10, 16, 13},{0, 20, 8},
{0, 8, 16},{0, 16, 10},{0, 5, 20},{2, 19, 5},
{5, 19, 20},{0, 10, 5},{2, 5, 12},{5, 10, 12},
{4, 7, 24, 23},{4, 15, 18, 7},{4, 9, 17, 15},{3, 22, 24, 7},
{3, 7, 18, 14},{3, 14, 16, 8} };

/**
	Cartesian speaker coordinates defined in Rec. ITU-R BS.2127-1 Sec. 11.2
*/

namespace admrender {

	const std::map<std::string, std::map<std::string, CartesianPosition>> alloPositions = {
		// Stereo - BS.2051-3 System A 0+2+0
		{"0+2+0",{ {"M+030",{-1.,1.,0.}},
		{"M-030",{1.,1.,0.}} }},
		// 5.1 - BS.2051-3 System B 0+5+0
		{"0+5+0",{ {"M+030",{-1.,1.,0.}},
		{"M-030",{1.,1.,0.}},
		{"M+000",{0.,1.,0.}},
		{"M+110",{-1.,-1.,0.}},
		{"M-110",{1.,-1.,0.}},
		{"LFE1",{-1.,1.,-1.}}
		}},
		// 5.1.2 - BS.2051-3 System C 2+5+0
		{"2+5+0",{ {"M+030",{-1.,1.,0.}},
		{"M-030",{1.,1.,0.}},
		{"M+000",{0.,1.,0.}},
		{"M+110",{-1.,-1.,0.}},
		{"M-110",{1.,-1.,0.}},
		{"U+030",{-1.,1.,1.}},
		{"U-030",{1.,1.,1.}},
		{"LFE1",{-1.,1.,-1.}}
		}},
		// 5.1.4 - BS.2051-3 System D 4+5+0
		{"4+5+0",{ {"M+030",{-1.,1.,0.}},
		{"M-030",{1.,1.,0.}},
		{"M+000",{0.,1.,0.}},
		{"M+110",{-1.,-1.,0.}},
		{"M-110",{1.,-1.,0.}},
		{"U+030",{-1.,1.,1.}},
		{"U-030",{1.,1.,1.}},
		{"U+110",{-1.,-1.,1.}},
		{"U-110",{1.,-1.,1.}},
		{"LFE1",{-1.,1.,-1.}}
		}},
		// BS.2051-3 System E 4+5+1
		{"4+5+1",{ {"M+030",{-1.,1.,0.}},
		{"M-030",{1.,1.,0.}},
		{"M+000",{0.,1.,0.}},
		{"M+110",{-1.,-1.,0.}},
		{"M-110",{1.,-1.,0.}},
		{"U+030",{-1.,1.,1.}},
		{"U-030",{1.,1.,1.}},
		{"U+110",{-1.,-1.,1.}},
		{"U-110",{1.,-1.,1.}},
		{"B+000",{0.,1.,-1.}},
		{"LFE1",{-1.,1.,-1.}}
		}},
		// BS.2051-3 System F 3+7+0
		{"3+7+0",{ {"M+000",{0.,1.,0.}},
		{"M+030",{-1.,1.,0.}},
		{"M-030",{1.,1.,0.}},
		{"U+045",{-1.,1.,1.}},
		{"U-045",{1.,1.,1.}},
		{"M+090",{-1.,0.,0.}},
		{"M-090",{1.,0.,0.}},
		{"M+135",{-1.,-1.,0.}},
		{"M-135",{1.,-1.,0.}},
		{"UH+180",{0.,-1.,1.}},
		{"LFE1",{-1.,1.,-1.}},
		{"LFE2",{1.,1.,-1.}}
		}},
		// BS.2051-3 System G 4+9+0 - Note: doesn't include the screen speakers because these are defined as in Rec. ITU-R BS.2127-1 Sec. 7.3.9
		{"4+9+0",{ {"M+030",{-1.,1.,0.}},
		{"M-030",{1.,1.,0.}},
		{"M+000",{0.,1.,0.}},
		{"M+090",{-1.,0.,0.}},
		{"M-090",{1.,0.,0.}},
		{"M+135",{-1.,-1.,0.}},
		{"M-135",{1.,-1.,0.}},
		{"U+045",{-1.,1.,1.}},
		{"U-045",{1.,1.,1.}},
		{"U+135",{-1.,-1.,1.}},
		{"U-135",{1.,-1.,1.}},
		{"LFE1",{-1.,1.,-1.}},
		{"LFE2",{1.,1.,-1.}}
		}},
		// BS.2051-3 System H 9+10+3
		{"9+10+3",{ {"M+060",{-1.,0.414214,0.}},
		{"M-060",{1.,0.414214,0.}},
		{"M+000",{0.,1.,0.}},
		{"M+135",{-1.,-1.,0.}},
		{"M-135",{1.,-1.,0.}},
		{"M+030",{-1.,1.,0.}},
		{"M-030",{1.,1.,0.}},
		{"M+180",{0.,-1.,0.}},
		{"M+090",{-1.,0.,0.}},
		{"M-090",{1.,0.,0.}},
		{"U+045",{-1.,1.,1.}},
		{"U-045",{1.,1.,1.}},
		{"U+000",{0.,1.,1.}},
		{"T+000",{0.,0.,1.}},
		{"U+135",{-1.,-1.,1.}},
		{"U-135",{1.,-1.,1.}},
		{"U+090",{-1.,0.,1.}},
		{"U-090",{1.,0.,1.}},
		{"U+180",{0.,-1.,1.}},
		{"B+000",{0.,1.,-1.}},
		{"B+045",{-1.,1.,-1.}},
		{"B-045",{1.,1.,-1.}},
		{"LFE1",{-1.,1.,-1.}},
		{"LFE2",{1.,1.,-1.}}
		}},
		// 7.1 - BS.2051-3 System I 0+7+0
		{ "0+7+0",{ {"M+030",{-1.,1.,0.}},
		{"M-030",{1.,1.,0.}},
		{"M+000",{0.,1.,0.}},
		{"M+090",{-1.,0.,0.}},
		{"M-090",{1.,0.,0.}},
		{"M+135",{-1.,-1.,0.}},
		{"M-135",{1.,-1.,0.}},
		{"LFE1",{-1.,1.,-1.}}
		}},
		// 7.1.4 - BS.2051-3 System J 4+7+0
		{ "4+7+0",{ {"M+030",{-1.,1.,0.}},
		{"M-030",{1.,1.,0.}},
		{"M+000",{0.,1.,0.}},
		{"M+090",{-1.,0.,0.}},
		{"M-090",{1.,0.,0.}},
		{"M+135",{-1.,-1.,0.}},
		{"M-135",{1.,-1.,0.}},
		{"U+045",{-1.,1.,1.}},
		{"U-045",{1.,1.,1.}},
		{"U+135",{-1.,-1.,1.}},
		{"U-135",{1.,-1.,1.}},
		{"LFE1",{-1.,1.,-1.}}
		}},
		// BEAR 9+10+5
		{"9+10+5", { {"M+060",{-1.,0.414214,0.}},
		{"M-060",{1.,0.414214,0.}},
		{"M+000",{0.,1.,0.}},
		{"M+135",{-1.,-1.,0.}},
		{"M-135",{1.,-1.,0.}},
		{"M+030",{-1.,1.,0.}},
		{"M-030",{1.,1.,0.}},
		{"M+180",{0.,-1.,0.}},
		{"M+090",{-1.,0.,0.}},
		{"M-090",{1.,0.,0.}},
		{"U+045",{-1.,1.,1.}},
		{"U-045",{1.,1.,1.}},
		{"U+000",{0.,1.,1.}},
		{"T+000",{0.,0.,1.}},
		{"U+135",{-1.,-1.,1.}},
		{"U-135",{1.,-1.,1.}},
		{"U+090",{-1.,0.,1.}},
		{"U-090",{1.,0.,1.}},
		{"U+180",{0.,-1.,1.}},
		{"B+000",{0.,1.,-1.}},
		{"B+045",{-1.,1.,-1.}},
		{"B-045",{1.,1.,-1.}},
		{"B+135",{-1.,-1.,-1.}},
		{"B-135",{1.,-1.,-1.}},
		{"LFE1",{-1.,1.,-1.}},
		{"LFE2",{1.,1.,-1.}} }}
	};

	/** Returns the cartesian/allocentric positions of specified layout as specified in Rec. ITU-R BS.2127-1 Sec. 7.3.9
	 * @param layout Layout of the speaker array
	 * @return Positions of the speakers in cartesian/allocentric format. If the layout is not supported by (i.e. not defined
	 * in the tables in section 11.2 then an empty vector is returned.
	 */
	static inline std::vector<CartesianPosition> positionsForLayout(const Layout& layout)
	{
		std::vector<CartesianPosition> layoutPositions;

		auto it = alloPositions.find(layout.name);
		if (it != alloPositions.end())
		{
			const auto& positions = it->second;

			for (auto& channel : layout.channels)
				if (channel.name == "M+SC" || channel.name == "M-SC")
					layoutPositions.push_back(PointPolarToCart(channel.polarPosition));
				else
					layoutPositions.push_back(positions.at(channel.name));
		}
		return layoutPositions;
	}
}//namespace admrender

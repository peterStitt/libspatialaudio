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

#include "LoudspeakerLayouts.h"

Channel::Channel() : Channel(admrender::ChannelTypes::FrontCentre)
{
}

Channel::Channel(std::string& channelName, PolarPosition position, PolarPosition positionNominal, bool channelLfe)
	: name(channelName), channelType(admrender::ChannelTypes::Custom), polarPosition(position), polarPositionNominal(positionNominal), isLFE(channelLfe)
{
}

Channel::Channel(admrender::ChannelTypes channelType)
{
	int index = static_cast<int>(channelType);
	if (index > -1 && index < admrender::bs2094::channelLabels.size())
	{
		name = admrender::bs2094::channelLabels[index];
		this->channelType = channelType;
		if (index < admrender::bs2094::positions.size())
		{
			polarPosition = admrender::bs2094::positions[index];
			polarPositionNominal = admrender::bs2094::positions[index];
		}
		else // 
		{

		}
		isLFE = isChannelLFE();
	}
	else
	{
		// Fallback to a channel front centre
		name = "Unknown";
		this->channelType = admrender::ChannelTypes::FrontCentre;
		polarPosition = admrender::bs2094::positions[static_cast<int>(admrender::ChannelTypes::FrontCentre)];
		polarPositionNominal = polarPosition;
		isLFE = false;
		throw std::runtime_error("Channel type not complete. Please add label and position");
	}
}

Channel::Channel(const std::string& channelName)
{
	name = channelName;

	if (stringContains(channelName, "ACN")) // HOA channel so set position to default
	{
		polarPosition = PolarPosition{ 0., 0., 1. };
		polarPositionNominal = PolarPosition{ 0., 0., 1. };
		isLFE = false;
	}
	else
	{
		// Find the channel in the labels vector
		auto it = std::find(admrender::bs2094::channelLabels.begin(),
			admrender::bs2094::channelLabels.end(),
			channelName);

		if (it != admrender::bs2094::channelLabels.end())
		{
			auto index = std::distance(admrender::bs2094::channelLabels.begin(), it);
			channelType = static_cast<admrender::ChannelTypes>(index);
			polarPosition = admrender::bs2094::positions[index];
			polarPositionNominal = admrender::bs2094::positions[index];
			isLFE = isChannelLFE();
		}
		else
		{
			// Fallback to a Custom channel at the front
			channelType = admrender::ChannelTypes::Custom;
			int fallbackIndex = static_cast<int>(admrender::ChannelTypes::FrontCentre);
			polarPosition = admrender::bs2094::positions[fallbackIndex];
			polarPositionNominal = admrender::bs2094::positions[fallbackIndex];
			isLFE = false;
			throw std::runtime_error("Unknown channel: " + channelName);
		}
	}
}
Channel::~Channel() {};

bool Channel::isChannelLFE()
{
	const std::string& nominalLabel = GetNominalSpeakerLabel(name);
	return (stringContains(nominalLabel, "LFE1") || stringContains(nominalLabel, "LFE2"));
};


Layout::Layout()
	{
	}

Layout::Layout(std::string layoutName, std::vector<Channel> layoutChannels, bool layoutHasLfe, bool layoutIsHoa, unsigned int layoutOrder)
		: name(layoutName), channels(layoutChannels), hasLFE(layoutHasLfe), isHoa(layoutIsHoa), hoaOrder(layoutOrder)
	{
	}

Layout::Layout(admrender::OutputLayout layoutType)
	{
		std::string layoutName;
		switch (layoutType)
		{
		case admrender::OutputLayout::Stereo:
			layoutName = "0+2+0";
			break;
		case admrender::OutputLayout::Quad:
			layoutName = "0+4+0";
			break;
		case admrender::OutputLayout::FivePointOne:
			layoutName = "0+5+0";
			break;
		case admrender::OutputLayout::FivePointOnePointTwo:
			layoutName = "2+5+0";
			break;
		case admrender::OutputLayout::FivePointOnePointFour:
			layoutName = "4+5+0";
			break;
		case admrender::OutputLayout::FivePointOnePointFourPlusLow:
			layoutName = "4+5+1";
			break;
		case admrender::OutputLayout::SevenPointOnePointThree:
			layoutName = "3+7+0";
			break;
		case admrender::OutputLayout::ThirteenPointOne:
			layoutName = "4+9+0";
			break;
		case admrender::OutputLayout::TwentyTwoPointTwo:
			layoutName = "9+10+3";
			break;
		case admrender::OutputLayout::SevenPointOne:
			layoutName = "0+7+0";
			break;
		case admrender::OutputLayout::SevenPointOnePointFour:
			layoutName = "4+7+0";
			break;
		case admrender::OutputLayout::BEAR_9_10_5:
			layoutName = "9+10+5";
			break;
		case admrender::OutputLayout::SevenPointOnePointTwo:
			layoutName = "2+7+0";
			break;
		case admrender::OutputLayout::ThreePointOnePointTwo:
			layoutName = "2+3+0";
			break;
		case admrender::OutputLayout::Binaural:
			throw std::runtime_error("Binaural is not a supported loudspeaker layout. Using stereo instead.");
			layoutName = "0+2+0";
			break;
		default:
			throw std::runtime_error("Layout is not in the list of presets! Please add it. Using stereo for now.");
			layoutName = "0+2+0";
			break;
		}
		*this = getMatchingLayout(layoutName);
	}

Layout::Layout(std::string& layoutName)
	{
		*this = getMatchingLayout(layoutName);
	}

int Layout::getMatchingChannelIndex(const std::string& channelName)
{
	unsigned int nChannels = (unsigned int)channels.size();
	for (unsigned int iCh = 0; iCh < nChannels; ++iCh)
	{
		if (channelName.compare(channels[iCh].name) == 0)
			return iCh;
	}
	return -1; // if no matching channel names are found
}

std::vector<std::string> Layout::channelNames() const
{
	std::vector<std::string> channelNames;
	for (unsigned int iCh = 0; iCh < channels.size(); ++iCh)
		channelNames.push_back(channels[iCh].name);

	return channelNames;
}

bool Layout::containsChannel(const std::string& channelName) const
{
	for (unsigned int iCh = 0; iCh < channels.size(); ++iCh)
		if (channels[iCh].name == channelName)
			return true;
	return false;
}

Layout Layout::getLayoutWithoutLFE(Layout layout)
{
	Layout layoutNoLFE = layout;
	unsigned int nCh = (unsigned int)layout.channels.size();
	layoutNoLFE.channels.resize(0);
	for (unsigned int iCh = 0; iCh < nCh; ++iCh)
		if (!layout.channels[iCh].isLFE)
			layoutNoLFE.channels.push_back(layout.channels[iCh]);
	layoutNoLFE.hasLFE = false;

	return layoutNoLFE;
}

const std::vector<Layout>& Layout::getSpeakerLayouts()
{
	/** Predefined speaker layouts. */
	static const std::vector<Layout> speakerLayouts = {
		// Stereo - BS.2051-3 System A 0+2+0
		Layout{ "0+2+0",std::vector<Channel>{ Channel{"M+030"}, Channel{"M-030"} }, false},
		// Quad - note: Not defined in ITU-R BS.2051-3
		Layout{ "0+4+0",std::vector<Channel>{ Channel("M+045"), Channel("M-045"), Channel("M+135"), Channel("M-135")}, false},
		// 5.1 - BS.2051-3 System B 0+5+0
		Layout{ "0+5+0",std::vector<Channel>{ Channel("M+030"), Channel("M-030"), Channel("M+000"), Channel("LFE1"), Channel("M+110"), Channel("M-110")}, true},
		// 5.1.2 - BS.2051-3 System C 2+5+0
		Layout{ "2+5+0",std::vector<Channel>{ Channel("M+030"), Channel("M-030"), Channel("M+000"), Channel("LFE1"),Channel("M+110"), Channel("M-110"),
		Channel("U+030"), Channel("U-030")}, true},
		// 5.1.4 - BS.2051-3 System D 4+5+0
		Layout{ "4+5+0",std::vector<Channel>{ Channel("M+030"), Channel("M-030"), Channel("M+000"), Channel("LFE1"), Channel("M+110"), Channel("M-110"),
		Channel("U+030"), Channel("U-030"), Channel("U+110"), Channel("U-110")}, true},
		// 5.1.4+top - BS.2051-3 System E 4+5+1
		Layout{"4+5+1",std::vector<Channel>{ Channel("M+030"),Channel("M-030"),Channel("M+000"),Channel("LFE1"),Channel("M+110"),Channel("M-110"),
		Channel("U+030"),Channel("U-030"),Channel("U+110"),Channel("U-110"),Channel("B+000")}, true},
		// 7.1.3 - BS.2051-3 System F 3+7+0
		Layout{"3+7+0",std::vector<Channel>{ Channel("M+000"),Channel("M+030"),Channel("M-030"),Channel("U+045"),Channel("U-045"),Channel("M+090"),Channel("M-090"),
		Channel("M+135"),Channel("M-135"),Channel("UH+180"),Channel("LFE1"),Channel("LFE2")}, true},
		// 13.1 - BS.2051-3 System G 4+9+0
		Layout{"4+9+0",std::vector<Channel>{ Channel("M+030"),Channel("M-030"),Channel("M+000"),Channel("LFE1"),Channel("M+090"),Channel("M-090"),Channel("M+135"),
		Channel("M-135"),Channel("U+045"),Channel("U-045"),Channel("U+135"),Channel("U-135"), Channel("M+SC"), Channel("M-SC")}, true},
		// 22.2 BS.2051-3 System H 9+10+3
		Layout{"9+10+3",std::vector<Channel>{ Channel("M+060"),Channel("M-060"),Channel("M+000"),Channel("LFE1"),Channel("M+135"),Channel("M-135"),Channel("M+030"),Channel("M-030"),
		Channel("M+180"),Channel("LFE2"),Channel("M+090"),Channel("M-090"),Channel("U+045"),Channel("U-045"),Channel("U+000"),Channel("T+000"),Channel("U+135"),Channel("U-135"),
		Channel("U+090"),Channel("U-090"),Channel("U+180"),Channel("B+000"),Channel("B+045"),Channel("B-045")}, true},
		// 7.1 - BS.2051-3 System I 0+7+0
		Layout{"0+7+0",std::vector<Channel>{ Channel("M+030"),Channel("M-030"),Channel("M+000"),Channel("LFE1"),Channel("M+090"),Channel("M-090"),Channel("M+135"),Channel("M-135")}, true},
		// 7.1.4 - BS.2051-3 System J 4+7+0
		Layout{"4+7+0",std::vector<Channel>{ Channel("M+030"),Channel("M-030"),Channel("M+000"),Channel("LFE1"),Channel("M+090"),Channel("M-090"),
		Channel("M+135"),Channel("M-135"),Channel("U+045"),Channel("U-045"),Channel("U+135"),Channel("U-135")}, true},
		// 7.1.2 - IAMF v1.0.0-errata
		Layout{"2+7+0",std::vector<Channel>{ Channel("M+030"),Channel("M-030"),Channel("M+000"),Channel("LFE1"),Channel("M+090"),Channel("M-090"),Channel("M+135"),Channel("M-135"),
		Channel("U+045"),Channel("U-045")}, true },
		// 3.1.2 - IAMF v1.0.0-errata
		Layout{"2+3+0",std::vector<Channel>{ Channel("M+030"),Channel("M-030"),Channel("M+000"),Channel("LFE1"),Channel("U+045"),Channel("U-045")}, true },
		// EBU Tech 3369 (BEAR) 9+10+5 - 9+10+3 with LFE1 & LFE2 removed and B+135 & B-135 added
		Layout{"9+10+5",std::vector<Channel>{ Channel("M+060"),Channel("M-060"),Channel("M+000"),Channel("M+135"),Channel("M-135"),Channel("M+030"),Channel("M-030"),Channel("M+180"),
		Channel("M+090"),Channel("M-090"),Channel("U+045"),Channel("U-045"),Channel("U+000"),Channel("T+000"),Channel("U+135"),Channel("U-135"),Channel("U+090"),Channel("U-090"),
		Channel("U+180"),Channel("B+000"),Channel("B+045"),Channel("B-045"),Channel("B+135"),Channel("B-135")}, true },
		// First order Ambisonics (AmbiX). Directions are meaningless so all set to front
		Layout{"1OA",std::vector<Channel>{ Channel("ACN0"),Channel("ACN1"),Channel("ACN2"),Channel("ACN3")}, false, true, 1},
		// Second order Ambisonics (AmbiX). Directions are meaningless so all set to front
		Layout{"2OA",std::vector<Channel>{ Channel("ACN0"),Channel("ACN1"),Channel("ACN2"),Channel("ACN3"),Channel("ACN4"),Channel("ACN5"),Channel("ACN6"),Channel("ACN7"),Channel("ACN8")}, false, true, 2},
		// Third order Ambisonics (AmbiX). Directions are meaningless so all set to front
		Layout{"3OA",std::vector<Channel>{ Channel("ACN0"),Channel("ACN1"),Channel("ACN2"),Channel("ACN3"),Channel("ACN4"),Channel("ACN5"),Channel("ACN6"),Channel("ACN7"),
		Channel("ACN8"),Channel("ACN9"),Channel("ACN10"),Channel("ACN11"),Channel("ACN12"),Channel("ACN13"),Channel("ACN14"),Channel("ACN15")}, false, true, 3},
	};
	return speakerLayouts;
}

Layout Layout::getMatchingLayout(std::string layoutName)
{
	auto& speakerLayouts = getSpeakerLayouts();

	unsigned int nLayouts = (unsigned int)speakerLayouts.size();
	for (unsigned int iLayout = 0; iLayout < nLayouts; ++iLayout)
	{
		if (layoutName.compare(speakerLayouts[iLayout].name) == 0)
			return speakerLayouts[iLayout];
	}
	return {}; // if no matching channel names are found
}

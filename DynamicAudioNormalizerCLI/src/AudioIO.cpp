//////////////////////////////////////////////////////////////////////////////////
// Dynamic Audio Normalizer - Audio Processing Utility
// Copyright (c) 2014-2017 LoRd_MuldeR <mulder2@gmx.de>. Some rights reserved.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
// http://www.gnu.org/licenses/gpl-2.0.txt
//////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AudioIO.h"
#include "AudioIO_SndFile.h"
#include "AudioIO_Mpg123.h"

///////////////////////////////////////////////////////////////////////////////
// Helper Functions
///////////////////////////////////////////////////////////////////////////////

//AudioIO libraries
static const uint8_t AUDIO_LIB_NULL       = 0x00u;
static const uint8_t AUDIO_LIB_LIBSNDFILE = 0x01u;
static const uint8_t AUDIO_LIB_LIBMPG123  = 0x02u;

//AudioIO library name to id mappings
static const struct
{
	const CHR name[32];
	const uint8_t id;
}
g_audioIO_mapping[] =
{
	{ TXT("libsndfile"), AUDIO_LIB_LIBSNDFILE },
	{ TXT("libmpg123"),  AUDIO_LIB_LIBMPG123  },
	{ TXT(""),           AUDIO_LIB_NULL       }
};

//Get id from AudioIO library name
static uint8_t parseName(const CHR *const name)
{
	if (name && name[0])
	{
		for (size_t i = 0; g_audioIO_mapping[i].id; ++i)
		{
			if (STRCASECMP(name, g_audioIO_mapping[i].name) == 0)
			{
				return g_audioIO_mapping[i].id;
			}
		}
		return AUDIO_LIB_NULL;
	}
	else
	{
		return AUDIO_LIB_LIBSNDFILE; /*default*/
	}
}

///////////////////////////////////////////////////////////////////////////////
// AudioIO Factory
///////////////////////////////////////////////////////////////////////////////

const CHR *const *AudioIO::getSupportedLibraries(const CHR **const list, const uint32_t maxLen)
{
	if (list && (maxLen > 0))
	{
		uint32_t nextPos = 0;
		for (size_t i = 0; g_audioIO_mapping[i].id; ++i)
		{
			if (g_audioIO_mapping[i].name[0] && (nextPos < maxLen))
			{
				list[nextPos++] = &g_audioIO_mapping[i].name[0];
			}
		}
		list[(nextPos < maxLen) ? nextPos : (maxLen - 1)] = NULL;
	}
	return list;
}

AudioIO *AudioIO::createInstance(const CHR *const name)
{
	switch (parseName(name))
	{
	case AUDIO_LIB_LIBSNDFILE:
		return new AudioIO_SndFile();
	case AUDIO_LIB_LIBMPG123:
		return new AudioIO_Mpg123();
	default:
		throw "Unsupported audio I/O library!";
	}
}

const CHR *const *AudioIO::getSupportedFormats(const CHR **const list, const uint32_t maxLen, const CHR *const name)
{
	switch (parseName(name))
	{
	case AUDIO_LIB_LIBSNDFILE:
		return AudioIO_SndFile::supportedFormats(list, maxLen);
	case AUDIO_LIB_LIBMPG123:
		return AudioIO_Mpg123::supportedFormats(list, maxLen);
	default:
		throw "Unsupported audio I/O library!";
	}
}

const CHR *AudioIO::getLibraryVersion(const CHR *const name)
{
	switch (parseName(name))
	{
	case AUDIO_LIB_LIBSNDFILE:
		return AudioIO_SndFile::libraryVersion();
	case AUDIO_LIB_LIBMPG123:
		return AudioIO_Mpg123::libraryVersion();
	default:
		throw "Unsupported audio I/O library!";
	}
}

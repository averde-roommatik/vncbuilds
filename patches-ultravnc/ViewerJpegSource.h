// Stand-in for UltraVNC's vncviewer/ViewerJpegSource.h.
//
// Upstream commit 6c44cfc ("harden code against buffer overruns") includes this header
// from ClientConnectionUltra2.cpp and ClientConnectionTight.cpp but never committed it, so
// upstream main does not compile. This is a reconstruction inferred from the call sites,
// NOT upstream's file. The build only copies it in when upstream doesn't ship one; delete
// it once upstream does.
//
// libjpeg source-manager callbacks over an in-memory buffer. The caller points
// cinfo.client_data at a bool that is set when the input is truncated.
#pragma once

#include <stdio.h>
#ifdef _VCPKG
#include <jpeglib.h>
#else
#include "libjpeg-turbo-win/jpeglib.h"
#endif

namespace ViewerJpeg
{
	inline void setError(j_decompress_ptr cinfo, bool value)
	{
		if (cinfo->client_data)
			*static_cast<bool*>(cinfo->client_data) = value;
	}

	// On truncated input, flag it and feed a fake EOI so libjpeg stops instead of reading
	// past the end of the buffer.
	inline void feedEndOfImage(j_decompress_ptr cinfo)
	{
		static const JOCTET eoi[2] = { 0xFF, 0xD9 };
		setError(cinfo, true);
		cinfo->src->next_input_byte = eoi;
		cinfo->src->bytes_in_buffer = sizeof(eoi);
	}

	inline void initSource(j_decompress_ptr cinfo)
	{
		setError(cinfo, false);
	}

	inline boolean fillInputBuffer(j_decompress_ptr cinfo)
	{
		feedEndOfImage(cinfo);
		return TRUE;
	}

	inline void skipInputData(j_decompress_ptr cinfo, long num_bytes)
	{
		if (num_bytes <= 0)
			return;
		if (static_cast<size_t>(num_bytes) > cinfo->src->bytes_in_buffer)
		{
			feedEndOfImage(cinfo);
			return;
		}
		cinfo->src->next_input_byte += static_cast<size_t>(num_bytes);
		cinfo->src->bytes_in_buffer -= static_cast<size_t>(num_bytes);
	}
}

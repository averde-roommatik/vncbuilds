// Stand-in for UltraVNC's vncviewer/ViewerBounds.h.
//
// Upstream commit 6c44cfc ("harden code against buffer overruns") includes this header
// from ClientConnectionUltra2.cpp, ClientConnectionCursor.cpp and FileTransfer.cpp but
// never committed it, so upstream main does not compile. This is a reconstruction
// inferred from the call sites, NOT upstream's file. The build only copies it in when
// upstream doesn't ship one; delete it once upstream does.
#pragma once

#include <stdint.h>

namespace ViewerBounds
{
	// Same cap the call sites already use for decoded pixel buffers (100 MiB).
	const int64_t kMaxPayload = 104857600;

	// True if w*h pixels of bytesPerPixel bytes each fit in limit bytes (overflow-safe).
	inline bool pixels(int64_t w, int64_t h, int64_t bytesPerPixel, int64_t limit)
	{
		if (w <= 0 || h <= 0 || bytesPerPixel <= 0 || limit <= 0)
			return false;
		return w <= limit / bytesPerPixel / h;
	}

	// True if a length announced by the peer is within the accepted size.
	inline bool payload(int64_t len)
	{
		return len >= 0 && len <= kMaxPayload;
	}

	// True if [index, index + size) lies within [0, len).
	inline bool range(int64_t index, int64_t size, int64_t len)
	{
		return index >= 0 && size >= 0 && len >= 0 && index <= len && size <= len - index;
	}

	// Clips [pos, pos + len) to [0, limit). offset receives how much was cut off the start.
	inline void clipAxis(int& pos, int& len, int limit, int& offset)
	{
		int64_t p = pos, l = len;
		offset = 0;
		if (p < 0)
		{
			offset = (int)-p;
			l += p;
			p = 0;
		}
		if (p > limit)
			p = limit;
		if (l > limit - p)
			l = limit - p;
		if (l < 0)
			l = 0;
		pos = (int)p;
		len = (int)l;
	}
}

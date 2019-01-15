#include <Engine/Conversions/MemConversions.h>

namespace afex {
void BytesToReadableString(char* textBuffer, usize textBufferSize, u64 byteCount) {
	AFEX_ASSERT_TRUE(nullptr != textBuffer && 0 != textBufferSize);
	memset(textBuffer, 0, textBufferSize);

	char buff[256] = { 0 };
	char *suf[] = { "B", "KiB", "MiB", "GiB", "TiB" };
	usize sufNum = sizeof(suf) / sizeof(*suf);

	double bytesCountDouble = static_cast<double>(byteCount);

	usize sufIdx = 0;
	for (; bytesCountDouble >= 1024.0 && sufIdx < sufNum; bytesCountDouble /= 1024, ++sufIdx);
	// here: writeSize == strlen(buff)
	usize writeSize = numeric_cast<usize>(sprintf(buff, "%.02lf", bytesCountDouble));

	char* buffEnd = static_cast<char*>(buff) + numeric_cast<iptr>(writeSize)-1;
	while ('0' == *buffEnd && '\0' != *buffEnd) {
		*buffEnd = '\0';
		--buffEnd;
		if ('.' == *buffEnd) {
			*buffEnd = '\0';
			break;
		}
	}

	// here: writeSize == size of the buffer we're using (including null term)
	writeSize = numeric_cast<usize>(sprintf(buff, "%s %s", buff, suf[sufIdx])) + 1;
	
	if (AFEX_ENSURE_FALSE(writeSize > textBufferSize)) {
		return;
	}
	memcpy(textBuffer, buff, writeSize);
}
}
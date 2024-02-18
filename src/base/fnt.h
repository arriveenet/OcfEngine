#pragma once
#include <stdint.h>

enum class FntBlockType : uint8_t {
	Info = 1,
	Common = 2,
	Pages = 3,
	Chars = 4,
	KerningPairs = 5,
	Unknown = 6
};

#pragma pack(1)
struct FntBlock {
	FntBlockType type;
	int32_t size;
};

struct FntInfo {
	short fontSize;
	unsigned char bitField;
	unsigned char charSet;
	unsigned short stretchH;
	unsigned char aa;
	unsigned char paddingUp;
	unsigned char paddingRight;
	unsigned char paddingDown;
	unsigned char paddingLeft;
	unsigned char spacingHoriz;
	unsigned char spasingVert;
	unsigned char outline;
};

struct FntCommon {
	unsigned short lineHeight;
	unsigned short base;
	unsigned short scaleW;
	unsigned short scaleH;
	unsigned short pages;
	unsigned char bitField;
	unsigned char alphaChnl;
	unsigned char redChnl;
	unsigned char greenChnl;
	unsigned char blueChnl;
};

struct FntChars {
	unsigned int id;		// The character id
	unsigned short x;		// The left position of the character image in the texture
	unsigned short y;		// The top position of the character image in the texture
	unsigned short width;	// The width of the character image in the texture
	unsigned short height;	// The height of the character image in the texture
	short xoffset;			// How much the current position should be offset when copying the image from the texture to the screen
	short yoffset;			// How much the current position should be offset when copying the image from the texture to the screen
	short xadvance;			// How much the current position should be advanced after drawing the character.
	unsigned char page;		// The texture page where the character image is found
	unsigned char chnl;		// The texture channel where the character image is found (1 = blue, 2 = green, 4 = red, 8 = alpha, 15 = all channels)
};

struct FntKerningPairs {
	unsigned int first;
	unsigned int second;
	short amount;
};

#pragma pack()

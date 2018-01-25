#pragma once

class TexFont
{
public:
	TexFont();
	~TexFont();

	bool IsLoaded() const
	{
		return mTexture != 0;
	}

	GLuint GetTexture() const
	{
		return mTexture;
	}

	bool Load(lcContext* Context);
	void Release();

	void PrintText(lcContext* Context, float Left, float Top, float Z, const char* Text) const;
	void GetGlyphTriangles(float Left, float Top, float Z, int Glyph, float* Buffer) const;
	void GetStringDimensions(int* cx, int* cy, const char* Text) const;

protected:
	struct
	{
		unsigned char width;
		float left, right, top, bottom;
	} mGlyphs[256];

	GLuint mTexture;
	int mTextureWidth;
	int mTextureHeight;
	int mFontHeight;
	int mRefCount;
};

extern TexFont gTexFont;


/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org
Copyright (C) 2008      Apple Inc. All Rights Reserved.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __CCTEXTURE2D_H__
#define __CCTEXTURE2D_H__

#include <string>
#include "CCObject.h"
#include "CCGeometry.h"
#include "ccTypes.h"

#include "CCConfiguration.h"

#include <vector>

NS_CC_BEGIN
class CCImage;

//CONSTANTS:

/** @typedef CCTexture2DPixelFormat
Possible texture pixel formats
*/
typedef enum {
	kCCTexture2DPixelFormat_Automatic = 0,
	//! 32-bit texture: RGBA8888
	kCCTexture2DPixelFormat_RGBA8888,
	//! 24-bit texture: RGBA888
	kCCTexture2DPixelFormat_RGB888,
	//! 16-bit texture without Alpha channel
	kCCTexture2DPixelFormat_RGB565,
	//! 8-bit textures used as masks
	kCCTexture2DPixelFormat_A8,
	//! 8-bit intensity texture
	kCCTexture2DPixelFormat_I8,
	//! 16-bit textures used as masks
	kCCTexture2DPixelFormat_AI88,
	//! 16-bit textures: RGBA4444
	kCCTexture2DPixelFormat_RGBA4444,
	//! 16-bit textures: RGB5A1
	kCCTexture2DPixelFormat_RGB5A1,	
	//! 4-bit PVRTC-compressed texture: PVRTC4
	kCCTexture2DPixelFormat_PVRTC4,
	//! 2-bit PVRTC-compressed texture: PVRTC2
	kCCTexture2DPixelFormat_PVRTC2,

	//! Default texture format: RGBA8888
	kCCTexture2DPixelFormat_Default = kCCTexture2DPixelFormat_RGBA8888,

	// backward compatibility stuff
	kTexture2DPixelFormat_Automatic = kCCTexture2DPixelFormat_Automatic,
	kTexture2DPixelFormat_RGBA8888 = kCCTexture2DPixelFormat_RGBA8888,
	kTexture2DPixelFormat_RGB888 = kCCTexture2DPixelFormat_RGB888,
	kTexture2DPixelFormat_RGB565 = kCCTexture2DPixelFormat_RGB565,
	kTexture2DPixelFormat_A8 = kCCTexture2DPixelFormat_A8,
	kTexture2DPixelFormat_RGBA4444 = kCCTexture2DPixelFormat_RGBA4444,
	kTexture2DPixelFormat_RGB5A1 = kCCTexture2DPixelFormat_RGB5A1,
	kTexture2DPixelFormat_Default = kCCTexture2DPixelFormat_Default

} CCTexture2DPixelFormat;

/**
Extension to set the Min / Mag filter
*/
typedef struct _ccTexParams {
	CCuint	minFilter;
	CCuint	magFilter;
	CCuint	wrapS;
	CCuint	wrapT;
} ccTexParams;

//CLASS INTERFACES:

/** @brief CCTexture2D class.
* This class allows to easily create OpenGL 2D textures from images, text or raw data.
* The created CCTexture2D object will always have power-of-two dimensions. 
* Depending on how you create the CCTexture2D object, the actual image area of the texture might be smaller than the texture dimensions i.e. "contentSize" != (pixelsWide, pixelsHigh) and (maxS, maxT) != (1.0, 1.0).
* Be aware that the content of the generated textures will be upside-down!
*/
class CC_DLL CCTexture2D : public CCObject
{
	/** pixel format of the texture */
	CC_PROPERTY_READONLY(CCTexture2DPixelFormat, m_ePixelFormat, PixelFormat)
	/** width in pixels */
	CC_PROPERTY_READONLY(unsigned int, m_uPixelsWide, PixelsWide)
	/** hight in pixels */
	CC_PROPERTY_READONLY(unsigned int, m_uPixelsHigh, PixelsHigh)

	/** texture name */
	CC_PROPERTY_READONLY(CCuint, m_uName, Name)

	/** content size */
	CC_PROPERTY_READONLY_PASS_BY_REF(CCSize, m_tContentSize, ContentSizeInPixels)
	/** texture max S */
	CC_PROPERTY(CCfloat, m_fMaxS, MaxS)
	/** texture max T */
	CC_PROPERTY(CCfloat, m_fMaxT, MaxT)
	/** whether or not the texture has their Alpha premultiplied */
	CC_PROPERTY_READONLY(bool, m_bHasPremultipliedAlpha, HasPremultipliedAlpha);
	CC_PROPERTY(ccResolutionType, m_eResolutionType, ResolutionType);
public:

	ID3D11ShaderResourceView* getTextureResource();
	ID3D11SamplerState** GetSamplerState();

	CCTexture2D();
	virtual ~CCTexture2D();

	char * description(void);

	/** These functions are needed to create mutable textures */
	void releaseData(void *data);
	void* keepData(void *data, unsigned int length);

	/** Intializes with a texture2d with data */
	bool initWithData(const void* data, CCTexture2DPixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const CCSize& contentSize);

	/**
	Drawing extensions to make it easy to draw basic quads using a CCTexture2D object.
	These functions require CC_TEXTURE_2D and both CC_VERTEX_ARRAY and CC_TEXTURE_COORD_ARRAY client states to be enabled.
	*/
	/** draws a texture at a given point */
	void drawAtPoint(const CCPoint& point);
	/** draws a texture inside a rect */
	void drawInRect(const CCRect& rect);

	/**
	Extensions to make it easy to create a CCTexture2D object from an image file.
	Note that RGBA type textures will have their alpha premultiplied - use the blending mode (CC_ONE, CC_ONE_MINUS_SRC_ALPHA).
	*/
	/** Initializes a texture from a UIImage object */
	bool initWithImage(CCImage * uiImage);

	bool initWithImage(CCImage *uiImage, ccResolutionType resolution);

	/**
	Extensions to make it easy to create a CCTexture2D object from a string of text.
	Note that the generated textures are of type A8 - use the blending mode (CC_SRC_ALPHA, CC_ONE_MINUS_SRC_ALPHA).
	*/
	/** Initializes a texture from a string with dimensions, alignment, font name and font size */
	bool initWithString(const char *text, const CCSize& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize);
	/** Initializes a texture from a string with font name and font size */
	bool initWithString(const char *text, const char *fontName, float fontSize);

	/** returns the content size of the texture in points */
	CCSize getContentSize(void);

#ifdef CC_SUPPORT_PVRTC	
	/**
	Extensions to make it easy to create a CCTexture2D object from a PVRTC file
	Note that the generated textures don't have their alpha premultiplied - use the blending mode (CC_SRC_ALPHA, CC_ONE_MINUS_SRC_ALPHA).
	*/
	/** Initializes a texture from a PVRTC buffer */
    bool initWithPVRTCData(const void *data, int level, int bpp, bool hasAlpha, int length, CCTexture2DPixelFormat pixelFormat);
#endif // CC_SUPPORT_PVRTC
    
    /** Initializes a texture from a PVR file */
	bool initWithPVRFile(const char* file);

	/** sets the min filter, mag filter, wrap s and wrap t texture parameters.
	If the texture size is NPOT (non power of 2), then in can only use CC_CLAMP_TO_EDGE in CC_TEXTURE_WRAP_{S,T}.
	@since v0.8
	*/
	void setTexParameters(ccTexParams* texParams);

	/** sets antialias texture parameters:
	- CC_TEXTURE_MIN_FILTER = CC_LINEAR
	- CC_TEXTURE_MAG_FILTER = CC_LINEAR

	@since v0.8
	*/
	void setAntiAliasTexParameters();

	/** sets alias texture parameters:
	- CC_TEXTURE_MIN_FILTER = CC_NEAREST
	- CC_TEXTURE_MAG_FILTER = CC_NEAREST

	@since v0.8
	*/
	void setAliasTexParameters();


	/** Generates mipmap images for the texture.
	It only works if the texture size is POT (power of 2).
	@since v0.99.0
	*/
	void generateMipmap();

	/** returns the bits-per-pixel of the in-memory OpenGL texture
	@since v1.0
	*/
    unsigned int bitsPerPixelForFormat();  
    
	/** sets the default pixel format for UIImagescontains alpha channel.
	If the UIImage contains alpha channel, then the options are:
	- generate 32-bit textures: kCCTexture2DPixelFormat_RGBA8888 (default one)
	- generate 24-bit textures: kCCTexture2DPixelFormat_RGB888
	- generate 16-bit textures: kCCTexture2DPixelFormat_RGBA4444
	- generate 16-bit textures: kCCTexture2DPixelFormat_RGB5A1
	- generate 16-bit textures: kCCTexture2DPixelFormat_RGB565
	- generate 8-bit textures: kCCTexture2DPixelFormat_A8 (only use it if you use just 1 color)

	How does it work ?
	- If the image is an RGBA (with Alpha) then the default pixel format will be used (it can be a 8-bit, 16-bit or 32-bit texture)
	- If the image is an RGB (without Alpha) then an RGB565 or RGB888 texture will be used (16-bit texture)

	@since v0.8
	*/
	static void setDefaultAlphaPixelFormat(CCTexture2DPixelFormat format);

	/** returns the alpha pixel format
	@since v0.8
	*/
	static CCTexture2DPixelFormat defaultAlphaPixelFormat();

	/** treats (or not) PVR files as if they have alpha premultiplied.
	 Since it is impossible to know at runtime if the PVR images have the alpha channel premultiplied, it is
	 possible load them as if they have (or not) the alpha channel premultiplied.
	 
	 By default it is disabled.
	 
	 @since v0.99.5
	 */
	static void PVRImagesHavePremultipliedAlpha(bool haveAlphaPremultiplied);

private:
	bool initPremultipliedATextureWithImage(CCImage * image, unsigned int pixelsWide, unsigned int pixelsHigh);
    
    // By default PVR images are treated as if they don't have the alpha channel premultiplied
    bool m_bPVRHaveAlphaPremultiplied;





	/** d3d texture resource */
	ID3D11ShaderResourceView* m_pTextureResource;

	ID3D11SamplerState* m_sampleState;
	/*
	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	void initVertexBuffer();
	void RenderVertexBuffer(ID3D11DeviceContext* deviceContext);

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* fxFilename);
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, 
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);
	void RenderShader(ID3D11DeviceContext* deviceContext);
	void Render();
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};
	*/
};
NS_CC_END 

#endif //__CCTEXTURE2D_H__


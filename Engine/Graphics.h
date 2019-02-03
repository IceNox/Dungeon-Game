/******************************************************************************************
*    Chili DirectX Framework Version 16.07.20                                              *
*    Graphics.h                                                                              *
*    Copyright 2016 PlanetChili <http://www.planetchili.net>                                  *
*                                                                                          *
*    This file is part of The Chili DirectX Framework.                                      *
*                                                                                          *
*    The Chili DirectX Framework is free software: you can redistribute it and/or modify      *
*    it under the terms of the GNU General Public License as published by                  *
*    the Free Software Foundation, either version 3 of the License, or                      *
*    (at your option) any later version.                                                      *
*                                                                                          *
*    The Chili DirectX Framework is distributed in the hope that it will be useful,          *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of                          *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                          *
*    GNU General Public License for more details.                                          *
*                                                                                          *
*    You should have received a copy of the GNU General Public License                      *
*    along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "ChiliException.h"
#include "Colors.h"

#include "Navigation.h"

#include "Sprite.h"

#include <vector>

// Data structures for calling functions
struct SpriteDrawData
{
    RECT irect = { 0, 0, 0, 0 }; bool ira = false; // Input RECT active
    RECT orect = { 0, 0, 0, 0 }; bool ora = false; // Output RECT active
    float transparency = 1.0f;
    float brightness = 1.0f;

    Color color = { 255, 255, 255 };
    float blendp = 0.0f;
};

// Graphics class
class Graphics
{
public:
    class Exception : public ChiliException
    {
    public:
        Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line );
        std::wstring GetErrorName() const;
        std::wstring GetErrorDescription() const;
        virtual std::wstring GetFullMessage() const override;
        virtual std::wstring GetExceptionType() const override;
    private:
        HRESULT hr;
    };
private:
    // vertex format for the framebuffer fullscreen textured quad
    struct FSQVertex
    {
        float x,y,z;        // position
        float u,v;            // texcoords
    };
public:
    // Graphics.cpp
    Graphics() {};
    Graphics( const Graphics& ) = delete;
    ~Graphics();
    Graphics& operator=( const Graphics& ) = delete;
    void Initialize(class HWNDKey& key, bool f, int sw, int sh);
    void EndFrame();
    void BeginFrame();
    void PutPixel( int x,int y,int r,int g,int b )
    {
        PutPixel( x,y,{ unsigned char( r ),unsigned char( g ),unsigned char( b ) } );
    }
    void PutPixel( int x,int y,Color c );
    Color GetPixel( int x,int y );
    Color GetPixel( int i );

    //void DrawSprite(Pos2D pos, Sprite s, float brightness, float transparency = 1.0f);
    //void DrawSpriteTransparent(int x, int y, const Sprite& s, float brightness, float transparency, Color chroma = Colors::Magenta);
    //void DrawSpriteNonChroma(int x, int y, const Sprite& s, float brightness);
    void DrawSprite(Pos2D pos, const Sprite& s, SpriteDrawData sdd = SpriteDrawData());
    //void DrawSpriteTransparentRECT(int x, int y, const Sprite& s, float brightness, float transparency, RECT r, Color chroma = Colors::Magenta);
    //void DrawSpriteNonChromaRECT(int x, int y, const Sprite& s, float brightness, RECT r);

    // Graphics_screen.cpp
    Sprite CaptureScreenshot();
    void ChangeBrightness(float brightness);
    void ChangeBrightness(float brightness, RECT r);

    // Graphics_level.cpp
    void DrawFloorEdgeShadows(int sx, int sy, std::string side);
    void ApplyShadeCircle(int sx, int sy, int width, int height, float brightness);
private:
    Microsoft::WRL::ComPtr<IDXGISwapChain>                pSwapChain;
    Microsoft::WRL::ComPtr<ID3D11Device>                pDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>            pImmediateContext;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>        pRenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D>                pSysBufferTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    pSysBufferTextureView;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>            pPixelShader;
    Microsoft::WRL::ComPtr<ID3D11VertexShader>            pVertexShader;
    Microsoft::WRL::ComPtr<ID3D11Buffer>                pVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>            pInputLayout;
    Microsoft::WRL::ComPtr<ID3D11SamplerState>            pSamplerState;
    D3D11_MAPPED_SUBRESOURCE                            mappedSysBufferTexture;
public:
    Color* pSysBuffer = nullptr;

    int ScreenWidth = 1280;
    int ScreenHeight = 720;
};

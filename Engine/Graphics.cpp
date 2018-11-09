/******************************************************************************************
*    Chili DirectX Framework Version 16.07.20                                              *
*    Graphics.cpp                                                                          *
*    Copyright 2016 PlanetChili.net <http://www.planetchili.net>                              *
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
#include "MainWindow.h"
#include "Graphics.h"
#include "DXErr.h"
#include "ChiliException.h"

#include "UserData.h"
#include "Functions.h"

#include <assert.h>
#include <string>
#include <array>
#include <cmath>
#include <chrono>
#include <fstream>
#include <sstream>

// Ignore the intellisense error "cannot open source file" for .shh files.
// They will be created during the build sequence before the preprocessor runs.
namespace FramebufferShaders
{
#include "FramebufferPS.shh"
#include "FramebufferVS.shh"
}

#pragma comment( lib,"d3d11.lib" )

#define CHILI_GFX_EXCEPTION( hr,note ) Graphics::Exception( hr,note,_CRT_WIDE(__FILE__),__LINE__ )

using Microsoft::WRL::ComPtr;

void Graphics::Initialize( HWNDKey& key,bool f,int sw,int sh )
{
    assert( key.hWnd != nullptr );

    ScreenWidth = sw;
    ScreenHeight = sh;

    //////////////////////////////////////////////////////
    // create device and swap chain/get render target view
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = Graphics::ScreenWidth;
    sd.BufferDesc.Height = Graphics::ScreenHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 1;
    sd.BufferDesc.RefreshRate.Denominator = 60;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = key.hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = !f;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    HRESULT                hr;
    UINT                createFlags = 0u;
#ifdef CHILI_USE_D3D_DEBUG_LAYER
#ifdef _DEBUG
    createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif
    
    // create device and front/back buffers
    if( FAILED( hr = D3D11CreateDeviceAndSwapChain( 
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &pSwapChain,
        &pDevice,
        nullptr,
        &pImmediateContext ) ) )
    {
        throw CHILI_GFX_EXCEPTION( hr,L"Creating device and swap chain" );
    }

    // get handle to backbuffer
    ComPtr<ID3D11Resource> pBackBuffer;
    if( FAILED( hr = pSwapChain->GetBuffer(
        0,
        __uuidof( ID3D11Texture2D ),
        (LPVOID*)&pBackBuffer ) ) )
    {
        throw CHILI_GFX_EXCEPTION( hr,L"Getting back buffer" );
    }

    // create a view on backbuffer that we can render to
    if( FAILED( hr = pDevice->CreateRenderTargetView( 
        pBackBuffer.Get(),
        nullptr,
        &pRenderTargetView ) ) )
    {
        throw CHILI_GFX_EXCEPTION( hr,L"Creating render target view on backbuffer" );
    }


    // set backbuffer as the render target using created view
    pImmediateContext->OMSetRenderTargets( 1,pRenderTargetView.GetAddressOf(),nullptr );


    // set viewport dimensions
    D3D11_VIEWPORT vp;
    vp.Width = float( Graphics::ScreenWidth );
    vp.Height = float( Graphics::ScreenHeight );
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    pImmediateContext->RSSetViewports( 1,&vp );


    ///////////////////////////////////////
    // create texture for cpu render target
    D3D11_TEXTURE2D_DESC sysTexDesc;
    sysTexDesc.Width = Graphics::ScreenWidth;
    sysTexDesc.Height = Graphics::ScreenHeight;
    sysTexDesc.MipLevels = 1;
    sysTexDesc.ArraySize = 1;
    sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sysTexDesc.SampleDesc.Count = 1;
    sysTexDesc.SampleDesc.Quality = 0;
    sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
    sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    sysTexDesc.MiscFlags = 0;
    // create the texture
    if( FAILED( hr = pDevice->CreateTexture2D( &sysTexDesc,nullptr,&pSysBufferTexture ) ) )
    {
        throw CHILI_GFX_EXCEPTION( hr,L"Creating sysbuffer texture" );
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = sysTexDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    // create the resource view on the texture
    if( FAILED( hr = pDevice->CreateShaderResourceView( pSysBufferTexture.Get(),
        &srvDesc,&pSysBufferTextureView ) ) )
    {
        throw CHILI_GFX_EXCEPTION( hr,L"Creating view on sysBuffer texture" );
    }


    ////////////////////////////////////////////////
    // create pixel shader for framebuffer
    // Ignore the intellisense error "namespace has no member"
    if( FAILED( hr = pDevice->CreatePixelShader(
        FramebufferShaders::FramebufferPSBytecode,
        sizeof( FramebufferShaders::FramebufferPSBytecode ),
        nullptr,
        &pPixelShader ) ) )
    {
        throw CHILI_GFX_EXCEPTION( hr,L"Creating pixel shader" );
    }
    

    /////////////////////////////////////////////////
    // create vertex shader for framebuffer
    // Ignore the intellisense error "namespace has no member"
    if( FAILED( hr = pDevice->CreateVertexShader(
        FramebufferShaders::FramebufferVSBytecode,
        sizeof( FramebufferShaders::FramebufferVSBytecode ),
        nullptr,
        &pVertexShader ) ) )
    {
        throw CHILI_GFX_EXCEPTION( hr,L"Creating vertex shader" );
    }
    

    //////////////////////////////////////////////////////////////
    // create and fill vertex buffer with quad for rendering frame
    const FSQVertex vertices[] =
    {
        { -1.0f,1.0f,0.5f,0.0f,0.0f },
        { 1.0f,1.0f,0.5f,1.0f,0.0f },
        { 1.0f,-1.0f,0.5f,1.0f,1.0f },
        { -1.0f,1.0f,0.5f,0.0f,0.0f },
        { 1.0f,-1.0f,0.5f,1.0f,1.0f },
        { -1.0f,-1.0f,0.5f,0.0f,1.0f },
    };
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( FSQVertex ) * 6;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0u;
    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;
    if( FAILED( hr = pDevice->CreateBuffer( &bd,&initData,&pVertexBuffer ) ) )
    {
        throw CHILI_GFX_EXCEPTION( hr,L"Creating vertex buffer" );
    }

    
    //////////////////////////////////////////
    // create input layout for fullscreen quad
    const D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
        { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
    };

    // Ignore the intellisense error "namespace has no member"
    if( FAILED( hr = pDevice->CreateInputLayout( ied,2,
        FramebufferShaders::FramebufferVSBytecode,
        sizeof( FramebufferShaders::FramebufferVSBytecode ),
        &pInputLayout ) ) )
    {
        throw CHILI_GFX_EXCEPTION( hr,L"Creating input layout" );
    }


    ////////////////////////////////////////////////////
    // Create sampler state for fullscreen textured quad
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    if( FAILED( hr = pDevice->CreateSamplerState( &sampDesc,&pSamplerState ) ) )
    {
        throw CHILI_GFX_EXCEPTION( hr,L"Creating sampler state" );
    }

    // allocate memory for sysbuffer (16-byte aligned for faster access)
    pSysBuffer = reinterpret_cast<Color*>( 
        _aligned_malloc( sizeof( Color ) * Graphics::ScreenWidth * Graphics::ScreenHeight,16u ) );
}

Graphics::~Graphics()
{
    //pSwapChain->SetFullscreenState(FALSE, NULL);

    // free sysbuffer memory (aligned free)
    if( pSysBuffer )
    {
        _aligned_free( pSysBuffer );
        pSysBuffer = nullptr;
    }
    // clear the state of the device context before destruction
    if( pImmediateContext ) pImmediateContext->ClearState();
}

void Graphics::EndFrame()
{
    HRESULT hr;

    // lock and map the adapter memory for copying over the sysbuffer
    if( FAILED( hr = pImmediateContext->Map( pSysBufferTexture.Get(),0u,
        D3D11_MAP_WRITE_DISCARD,0u,&mappedSysBufferTexture ) ) )
    {
        throw CHILI_GFX_EXCEPTION( hr,L"Mapping sysbuffer" );
    }
    // setup parameters for copy operation
    Color* pDst = reinterpret_cast<Color*>(mappedSysBufferTexture.pData );
    const size_t dstPitch = mappedSysBufferTexture.RowPitch / sizeof( Color );
    const size_t srcPitch = Graphics::ScreenWidth;
    const size_t rowBytes = srcPitch * sizeof( Color );
    // perform the copy line-by-line
    for( size_t y = 0u; y < Graphics::ScreenHeight; y++ )
    {
        memcpy( &pDst[ y * dstPitch ],&pSysBuffer[y * srcPitch],rowBytes );
    }
    // release the adapter memory
    pImmediateContext->Unmap( pSysBufferTexture.Get(),0u );

    // render offscreen scene texture to back buffer
    pImmediateContext->IASetInputLayout( pInputLayout.Get() );
    pImmediateContext->VSSetShader( pVertexShader.Get(),nullptr,0u );
    pImmediateContext->PSSetShader( pPixelShader.Get(),nullptr,0u );
    pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    const UINT stride = sizeof( FSQVertex );
    const UINT offset = 0u;
    pImmediateContext->IASetVertexBuffers( 0u,1u,pVertexBuffer.GetAddressOf(),&stride,&offset );
    pImmediateContext->PSSetShaderResources( 0u,1u,pSysBufferTextureView.GetAddressOf() );
    pImmediateContext->PSSetSamplers( 0u,1u,pSamplerState.GetAddressOf() );
    pImmediateContext->Draw( 6u,0u );

    // flip back/front buffers
    if( FAILED( hr = pSwapChain->Present( 1u,0u ) ) )
    {
        if( hr == DXGI_ERROR_DEVICE_REMOVED )
        {
            throw CHILI_GFX_EXCEPTION( pDevice->GetDeviceRemovedReason(),L"Presenting back buffer [device removed]" );
        }
        else
        {
            throw CHILI_GFX_EXCEPTION( hr,L"Presenting back buffer" );
        }
    }
}

void Graphics::BeginFrame()
{
    // clear the sysbuffer
    memset( pSysBuffer,0u,sizeof( Color ) * Graphics::ScreenHeight * Graphics::ScreenWidth );
}

void Graphics::PutPixel( int x,int y,Color c )
{
    assert( x >= 0 );
    assert( x < int( Graphics::ScreenWidth ) );
    assert( y >= 0 );
    assert( y < int( Graphics::ScreenHeight ) );
    pSysBuffer[Graphics::ScreenWidth * y + x] = c;
}

Color Graphics::GetPixel( int x,int y )
{
    assert(x >= 0);
    assert(x < int(Graphics::ScreenWidth));
    assert(y >= 0);
    assert(y < int(Graphics::ScreenHeight));
    return pSysBuffer[Graphics::ScreenWidth * y + x];
}

Color Graphics::GetPixel( int i )
{
    assert(i >= 0);
    assert(i < int(Graphics::ScreenWidth) * int(Graphics::ScreenHeight));
    return pSysBuffer[i];
}

void Graphics::DrawSprite(Pos2D pos, Sprite s, float brightness, float transparency)
{
    DrawSpriteRECT(pos, s, brightness, {0, 0, s.GetWidth(), s.GetHeight()}, {0, 0, ScreenWidth, ScreenHeight}, transparency);
}
/*
void Graphics::DrawSpriteTransparent(int x, int y, const Sprite& s, float brightness, float transparency, Color chroma)
{
    if (transparency > 1.0f) transparency = 1.0f;
    if (transparency < 0.0f) transparency = 0.0f;

    const int height = s.GetHeight();
    const int width = s.GetWidth();

    int ox = 0;
    int oy = 0;

    if (x < 0) ox = abs(x);
    if (y < 0) oy = abs(y);

    int tx = width;
    int ty = height;

    if (tx + x > ScreenWidth) tx = ScreenWidth - x;
    if (ty + y > ScreenHeight) ty = ScreenHeight - y;

    for (int sy = oy; sy < ty; sy++) {
        for (int sx = ox; sx < tx; sx++) {
            if (s.GetPixel(sx, sy) != chroma) {
                Color tColor = s.GetPixel(sx, sy, brightness);
                Color cColor = pSysBuffer[ScreenWidth * (y + sy) + x + sx];

                Color fColor;
                fColor.SetR((cColor.GetR() - tColor.GetR()) * transparency + tColor.GetR());
                fColor.SetG((cColor.GetG() - tColor.GetG()) * transparency + tColor.GetG());
                fColor.SetB((cColor.GetB() - tColor.GetB()) * transparency + tColor.GetB());

                PutPixel(x + sx, y + sy, fColor);
            }
        }
    }
}

void Graphics::DrawSpriteNonChroma(int x, int y, const Sprite& s, float brightness)
{
    const int height = s.GetHeight();
    const int width = s.GetWidth();

    int ox = 0;
    int oy = 0;

    if (x < 0) ox = abs(x);
    if (y < 0) oy = abs(y);

    int tx = width;
    int ty = height;

    if (tx + x > ScreenWidth) tx = ScreenWidth - x;
    if (ty + y > ScreenHeight) ty = ScreenHeight - y;

    for (int sy = oy; sy < ty; sy++) {
        for (int sx = ox; sx < tx; sx++) {
            PutPixel(x + sx, y + sy, s.GetPixel(sx, sy, brightness));
        }
    }
}
*/
void Graphics::DrawSpriteRECT(Pos2D pos, Sprite s, float brightness, RECT ri, RECT ro, float transparency)
{
    // Set transparency bounds
    add_bounds(transparency, 0.0f, 1.0f);

    // Apply transparency to the sprite
    if (transparency < 1.0f) {
        s.ApplyTransparency(transparency);
    }

    // Get sprite dimensions
    const int height = s.GetHeight();
    const int width  = s.GetWidth();

    // Adjust input bounds
    if (ri.left < 0) ri.left = 0;
    if (ri.top  < 0) ri.top  = 0;

    if (ri.right  > width ) ri.right  = width;
    if (ri.bottom > height) ri.bottom = height;

    // Create offset variables
    int ox = 0;
    int oy = 0;

    int tx = width;
    int ty = height;

    // Evaluate selected sprite region
    pos.x -= ri.left;
    pos.y -= ri.top;

    if (ox < ri.left) ox = ri.left;
    if (oy < ri.top ) oy = ri.top;

    if (tx > ri.right)  tx = ri.right;
    if (ty > ri.bottom) ty = ri.bottom;

    // Evaluate bounds if object is offbound
    if (pos.x < ro.left) ox = abs(pos.x - ro.left);
    if (pos.y < ro.top)  oy = abs(pos.y - ro.top);

    if (tx + pos.x > ro.right)  tx = ro.right  - pos.x;
    if (ty + pos.y > ro.bottom) ty = ro.bottom - pos.y;

    // Draw to screen
    for (int sy = oy; sy < ty; sy++) {
        for (int sx = ox; sx < tx; sx++) {
            // Skip if pixel is out of selected sprite region
            if (sx < ri.left || sx > ri.right || sy < ri.top || sy > ri.bottom)
                continue;

            // Skip transparent pixels
            if (s.GetPixel(sx, sy).GetA() == 0)
                continue;

            // Draw the pixel
            if (s.GetPixel(sx, sy).GetA() == 255) {
                PutPixel(pos.x + sx, pos.y + sy, s.GetPixel(sx, sy, brightness));
            }
            else {
                float ratio = s.GetPixel(sx, sy).GetA() / 255.0f;

                int nr = GetPixel(pos.x + sx, pos.y + sy).GetR() + (s.GetPixel(sx, sy, brightness).GetR() - GetPixel(pos.x + sx, pos.y + sy).GetR()) * ratio;
                int ng = GetPixel(pos.x + sx, pos.y + sy).GetG() + (s.GetPixel(sx, sy, brightness).GetG() - GetPixel(pos.x + sx, pos.y + sy).GetG()) * ratio;
                int nb = GetPixel(pos.x + sx, pos.y + sy).GetB() + (s.GetPixel(sx, sy, brightness).GetB() - GetPixel(pos.x + sx, pos.y + sy).GetB()) * ratio;

                PutPixel(pos.x + sx, pos.y + sy, Color(nr, ng, nb));
            }
        }
    }
}
/*
void Graphics::DrawSpriteTransparentRECT(int x, int y, const Sprite& s, float brightness, float transparency, RECT r, Color chroma)
{
    if (transparency > 1.0f) transparency = 1.0f;
    if (transparency < 0.0f) transparency = 0.0f;

    for (int sy = r.top; sy < r.bottom; sy++) {
        for (int sx = r.left; sx < r.right; sx++) {
            if (s.GetPixel(sx, sy) != chroma) {

                int fx = x + sx - r.left;
                int fy = y + sy - r.top;

                if (fx < 0) continue;
                if (fy < 0) continue;

                if (fx > ScreenWidth) continue;
                if (fy > ScreenHeight) continue;

                Color tColor = s.GetPixel(sx, sy, brightness);
                Color cColor = pSysBuffer[ScreenWidth * (y + sy) + x + sx];

                Color fColor;
                fColor.SetR((cColor.GetR() - tColor.GetR()) * transparency + tColor.GetR());
                fColor.SetG((cColor.GetG() - tColor.GetG()) * transparency + tColor.GetG());
                fColor.SetB((cColor.GetB() - tColor.GetB()) * transparency + tColor.GetB());

                PutPixel(x + sx, y + sy, fColor);
            }
        }
    }
}

void Graphics::DrawSpriteNonChromaRECT(int x, int y, const Sprite& s, float brightness, RECT r)
{
    for (int sy = r.top; sy < r.bottom; sy++) {
        for (int sx = r.left; sx < r.right; sx++) {

            int fx = x + sx - r.left;
            int fy = y + sy - r.top;

            if (fx < 0) continue;
            if (fy < 0) continue;

            if (fx > ScreenWidth) continue;
            if (fy > ScreenHeight) continue;

            PutPixel(x + sx, y + sy, s.GetPixel(sx, sy, brightness));
        }
    }
}
*/

void Graphics::DrawFloorEdgeShadows(int sx, int sy, std::string side)
{
    if (side == "up") {
        for (int y = 0; y < 4; y++) {
             for (int x = 0; x < 96; x++) {
                if (sx + x >= 0 && sy + y >= 0 && sx + x < ScreenWidth && sy + y < ScreenHeight) {
                    Color pixel = pSysBuffer[(sy + y) * ScreenWidth + sx + x];

                    float bm;
                    if (y < 2) bm = 0.2f;
                    else bm = 0.6f;

                    int newR = pixel.GetR();
                    int newG = pixel.GetG();
                    int newB = pixel.GetB();

                    newR *= bm;
                    newG *= bm;
                    newB *= bm;

                    pixel.SetR(newR);
                    pixel.SetG(newG);
                    pixel.SetB(newB);

                    pSysBuffer[(sy + y) * ScreenWidth + sx + x] = pixel;
                }
            }
        }
    }
    else if (side == "down") {
        for (int y = 92; y < 96; y++) {
            for (int x = 0; x < 96; x++) {
                if (sx + x >= 0 && sy + y >= 0 && sx + x < ScreenWidth && sy + y < ScreenHeight) {
                    Color pixel = pSysBuffer[(sy + y) * ScreenWidth + sx + x];

                    float bm;
                    if (y > 93) bm = 0.2f;
                    else bm = 0.6f;

                    int newR = pixel.GetR();
                    int newG = pixel.GetG();
                    int newB = pixel.GetB();

                    newR *= bm;
                    newG *= bm;
                    newB *= bm;

                    pixel.SetR(newR);
                    pixel.SetG(newG);
                    pixel.SetB(newB);

                    pSysBuffer[(sy + y) * ScreenWidth + sx + x] = pixel;
                }
            }
        }
    }
    else if (side == "left") {
        for (int y = 0; y < 96; y++) {
            for (int x = 0; x < 4; x++) {
                if (sx + x >= 0 && sy + y >= 0 && sx + x < ScreenWidth && sy + y < ScreenHeight) {
                    Color pixel = pSysBuffer[(sy + y) * ScreenWidth + sx + x];

                    float bm;
                    if (x < 2) bm = 0.2f;
                    else bm = 0.6f;

                    int newR = pixel.GetR();
                    int newG = pixel.GetG();
                    int newB = pixel.GetB();

                    newR *= bm;
                    newG *= bm;
                    newB *= bm;

                    pixel.SetR(newR);
                    pixel.SetG(newG);
                    pixel.SetB(newB);

                    pSysBuffer[(sy + y) * ScreenWidth + sx + x] = pixel;
                }
            }
        }
    }
    else if (side == "right") {
        for (int y = 0; y < 96; y++) {
            for (int x = 92; x < 96; x++) {
                if (sx + x >= 0 && sy + y >= 0 && sx + x < ScreenWidth && sy + y < ScreenHeight) {
                    Color pixel = pSysBuffer[(sy + y) * ScreenWidth + sx + x];

                    float bm;
                    if (x > 93) bm = 0.2f;
                    else bm = 0.6f;

                    int newR = pixel.GetR();
                    int newG = pixel.GetG();
                    int newB = pixel.GetB();

                    newR *= bm;
                    newG *= bm;
                    newB *= bm;

                    pixel.SetR(newR);
                    pixel.SetG(newG);
                    pixel.SetB(newB);

                    pSysBuffer[(sy + y) * ScreenWidth + sx + x] = pixel;
                }
            }
        }
    }
}

void Graphics::ApplyShadeCircle(int sx, int sy, int width, int height, float brightness)
{
    width += 5;
    height += 5;

    int hh = height * height;
    int ww = width * width;
    int hhww = hh * ww;
    int x0 = width;
    int dx = 0;

    // do the horizontal diameter
    for (int x = -width; x <= width; x++) {
        if (sx + x >= 0 && sy >= 0 && sx + x < ScreenWidth && sy < ScreenHeight) {
            Color pixel = pSysBuffer[ScreenWidth * sy + (sx + x)];

            int pixelR = pixel.GetR() * brightness;
            int pixelG = pixel.GetG() * brightness;
            int pixelB = pixel.GetB() * brightness;

            pixel.SetR(pixelR);
            pixel.SetG(pixelG);
            pixel.SetB(pixelB);

            pSysBuffer[ScreenWidth * sy + (sx + x)] = pixel;
        }
    }

    // now do both halves at the same time, away from the diameter
    for (int y = 1; y <= height; y++)
    {
        int x1 = x0 - (dx - 1);  // try slopes of dx - 1 or more
        for (; x1 > 0; x1--)
            if (x1*x1*hh + y*y*ww <= hhww)
                break;
        dx = x0 - x1;  // current approximation of the slope
        x0 = x1;

        for (int x = -x0; x <= x0; x++)
        {
            if (sx + x >= 0 && sy - y >= 0 && sx + x < ScreenWidth && sy - y < ScreenHeight) {
                Color pixel = pSysBuffer[ScreenWidth * (sy - y) + (sx + x)];

                int pixelR = pixel.GetR() * brightness;
                int pixelG = pixel.GetG() * brightness;
                int pixelB = pixel.GetB() * brightness;

                pixel.SetR(pixelR);
                pixel.SetG(pixelG);
                pixel.SetB(pixelB);

                pSysBuffer[ScreenWidth * (sy - y) + (sx + x)] = pixel;
            }

            if (sx + x >= 0 && sy + y >= 0 && sx + x < ScreenWidth && sy + y < ScreenHeight) {
                Color pixel = pSysBuffer[ScreenWidth * (sy + y) + (sx + x)];

                int pixelR = pixel.GetR() * brightness;
                int pixelG = pixel.GetG() * brightness;
                int pixelB = pixel.GetB() * brightness;

                pixel.SetR(pixelR);
                pixel.SetG(pixelG);
                pixel.SetB(pixelB);

                pSysBuffer[ScreenWidth * (sy + y) + (sx + x)] = pixel;
            }
        }
    }
}

//////////////////////////////////////////////////
//           Graphics Exception
Graphics::Exception::Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line )
    :
    ChiliException( file,line,note ),
    hr( hr )
{}

std::wstring Graphics::Exception::GetFullMessage() const
{
    const std::wstring empty = L"";
    const std::wstring errorName = GetErrorName();
    const std::wstring errorDesc = GetErrorDescription();
    const std::wstring& note = GetNote();
    const std::wstring location = GetLocation();
    return    (!errorName.empty() ? std::wstring( L"Error: " ) + errorName + L"\n"
        : empty)
        + (!errorDesc.empty() ? std::wstring( L"Description: " ) + errorDesc + L"\n"
            : empty)
        + (!note.empty() ? std::wstring( L"Note: " ) + note + L"\n"
            : empty)
        + (!location.empty() ? std::wstring( L"Location: " ) + location
            : empty);
}

std::wstring Graphics::Exception::GetErrorName() const
{
    return DXGetErrorString( hr );
}

std::wstring Graphics::Exception::GetErrorDescription() const
{
    std::array<wchar_t,512> wideDescription;
    DXGetErrorDescription( hr,wideDescription.data(),wideDescription.size() );
    return wideDescription.data();
}

std::wstring Graphics::Exception::GetExceptionType() const
{
    return L"Chili Graphics Exception";
}
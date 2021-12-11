#include "pch.h"
#include "TextureFactory.h"

namespace Dynamo
{
    Texture TextureFactory::CreateTexture(const Vec2ui& aSize, DXGI_FORMAT aFormat)
    {
        HRESULT result;
        D3D11_TEXTURE2D_DESC desc = { 0 };
        desc.Width = aSize.x;
        desc.Height = aSize.y;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = aFormat;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        ID3D11Texture2D* texture;
        result = Main::GetDevice()->CreateTexture2D(&desc, nullptr, &texture);
        assert(SUCCEEDED(result));

        Texture textureResult = CreateTexture(texture);
        ID3D11ShaderResourceView* srv;

        result = Main::GetDevice()->CreateShaderResourceView(texture, nullptr, &srv);
        assert(SUCCEEDED(result));

        textureResult.mySRV = srv;
        return textureResult;
    }

    Texture TextureFactory::CreateTexture(ID3D11Texture2D* aTexture)
    {
        HRESULT result;

        ID3D11RenderTargetView* rtv;
        result = Main::GetDevice()->CreateRenderTargetView(aTexture, nullptr, &rtv);
        assert(SUCCEEDED(result));

        D3D11_VIEWPORT* viewport = nullptr;

        if (aTexture)
        {
            D3D11_TEXTURE2D_DESC desc;
            aTexture->GetDesc(&desc);
            viewport = new D3D11_VIEWPORT({ 0, 0, (float)desc.Width, (float)desc.Height, 0, 1 });
        }

        Texture textureResult;
        textureResult.myTexture = aTexture;
        textureResult.myRenderTarget = rtv;
        textureResult.myViewport = viewport;
        return textureResult;
    }

    Texture TextureFactory::CreateDepth(const Vec2ui& aSize, DXGI_FORMAT aFormat)
    {
        HRESULT result;
        D3D11_TEXTURE2D_DESC desc = { 0 };
        desc.Width = aSize.x;
        desc.Height = aSize.y;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = aFormat;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        ID3D11Texture2D* texture;
        result = Main::GetDevice()->CreateTexture2D(&desc, nullptr, &texture);
        assert(SUCCEEDED(result));

        ID3D11DepthStencilView* dsv;
        result = Main::GetDevice()->CreateDepthStencilView(texture, nullptr, &dsv);
        assert(SUCCEEDED(result));

        D3D11_VIEWPORT* viewport = new D3D11_VIEWPORT({ 0, 0, (float)aSize.x, (float)aSize.y, 0, 1 });
        Texture texResult;
        texResult.myTexture = texture;
        texResult.myDepth = dsv;
        texResult.myViewport = viewport;
        return texResult;
    }

    GBuffer TextureFactory::CreateGBuffer(const Vec2ui& aSize)
    {
        HRESULT result;

        std::array<DXGI_FORMAT, GBuffer::COUNT> textureFormats =
        {
            DXGI_FORMAT_R8G8B8A8_UNORM,         // ALBEDO
            DXGI_FORMAT_R16G16B16A16_SNORM,     // NORMAL
            DXGI_FORMAT_R8G8B8A8_UNORM,         // MATERIAL
            DXGI_FORMAT_R32G32B32A32_FLOAT,     // POSITION
            DXGI_FORMAT_R16G16B16A16_FLOAT,     // VERTEXNORMAL
            DXGI_FORMAT_R8_UNORM,               // AMBIENTOCCLUSION
            DXGI_FORMAT_R16_UNORM,              // DEPTH,
            DXGI_FORMAT_R8G8B8A8_UNORM          // NORMAL TEXTURE
        };

        GBuffer returnBuffer = {};

        D3D11_TEXTURE2D_DESC desc = { 0 };
        desc.Width = aSize.x;
        desc.Height = aSize.y;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        for (int i = 0; i < GBuffer::COUNT; ++i)
        {
            desc.Format = textureFormats[i];
            result = Main::GetDevice()->CreateTexture2D(&desc, nullptr, &returnBuffer.myTextures[i]);
            assert(SUCCEEDED(result));

            result = Main::GetDevice()->CreateRenderTargetView(returnBuffer.myTextures[i], nullptr, &returnBuffer.myRTVs[i]);
            assert(SUCCEEDED(result));

            result = Main::GetDevice()->CreateShaderResourceView(returnBuffer.myTextures[i], nullptr, &returnBuffer.mySRVs[i]);
            assert(SUCCEEDED(result));
        }

        returnBuffer.myViewport = new D3D11_VIEWPORT({ 0, 0, (float)desc.Width, (float)desc.Height, 0, 1 });

        return returnBuffer;
    }
}

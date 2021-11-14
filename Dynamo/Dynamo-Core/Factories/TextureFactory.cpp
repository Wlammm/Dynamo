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
}

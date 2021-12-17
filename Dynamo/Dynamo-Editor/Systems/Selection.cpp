#include "EditorPch.h"
#include "Selection.h"

#include "Managers/EditorManager.h"
#include "Windows/Viewport.h"

namespace Editor
{
	Selection::Selection()
	{
        D3D11_TEXTURE2D_DESC desc = { 0 };
        desc.Width = 1;
        desc.Height = 1;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = static_cast<DXGI_FORMAT>(DXGI_FORMAT_R32_UINT);
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_STAGING;
        desc.BindFlags = 0;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        desc.MiscFlags = 0;

        HRESULT result = Dyn::Main::GetDevice()->CreateTexture2D(&desc, nullptr, &myTexture);
        assert(SUCCEEDED(result));
	}

	void Selection::Update()
	{
        if (Input::IsKeyDown(MouseButton::Left) && !ImGuizmo::IsUsing())
        {
            Dyn::Main::GetRenderManager().SetSelectionCallback(std::bind(&Selection::SelectionCallback, this));
        }
	}

    void Selection::SelectionCallback()
    {
        Viewport* viewport = Main::GetEditorManager()->GetWindow<Viewport>();
        if (!viewport)
            return;

        const Vec2ui& res = Dyn::Main::GetWindowResolution();
        const Vec2f& viewportMousePos = viewport->GetMousePosWindowNormalized();

        if (viewportMousePos == Vec2f{ -1, -1 })
            return;

        Vec2ui texturePos;
        texturePos.x = (uint) (viewportMousePos.x * (res.x - 1));
        texturePos.y = (uint) (viewportMousePos.y * (res.y - 1));

        uint id = GetIDAtPos(texturePos);

        Dyn::Scene* scene = Dyn::Main::GetScene();
        if (!scene)
            return;

        if (id == 0)
            Main::SetSelectedGameObject(nullptr);
        else
            Main::SetSelectedGameObject(scene->GetAllGameObjects()[id]);
    }

    uint Selection::GetIDAtPos(const Vec2ui aPos)
    {
        auto& selectionTexture = Dyn::Main::GetRenderManager().GetSelectionTexture();
        
        D3D11_BOX box;
        box.left = aPos.x;
        box.right = aPos.y + 1;
        box.top = aPos.y;
        box.bottom = aPos.y + 1;
        box.front = 0;
        box.back = 1;

        Dyn::Main::GetContext()->CopySubresourceRegion(myTexture, 0, 0, 0, 0, selectionTexture.GetTexture(), 0, &box);

        D3D11_MAPPED_SUBRESOURCE resource{ 0 };
        ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

        HRESULT result = Dyn::Main::GetContext()->Map(myTexture, 0, D3D11_MAP_READ, 0, &resource);
        DYN_ASSERT(SUCCEEDED(result));

        uint id = *reinterpret_cast<uint*>(resource.pData);
        Dyn::Main::GetContext()->Unmap(myTexture, 0);

        return id;
    }
}

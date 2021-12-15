#include "pch.h"
#include "SRV.h"

namespace Dynamo
{
	ID3D11ShaderResourceView* SRV::Get()
	{
		return mySRV;
	}

	ID3D11ShaderResourceView* const* SRV::GetConst() const
	{
		return &mySRV;
	}

	const std::filesystem::path& SRV::GetPath() const
	{
		return myPath;
	}

	void SRV::Bind(const int aSlot) const
	{
		Main::GetContext()->PSSetShaderResources(aSlot, 1, &mySRV);
		Main::GetContext()->VSSetShaderResources(aSlot, 1, &mySRV);
		Main::GetContext()->GSSetShaderResources(aSlot, 1, &mySRV);
	}
	void SRV::Release()
	{
		mySRV->Release();
	}
}

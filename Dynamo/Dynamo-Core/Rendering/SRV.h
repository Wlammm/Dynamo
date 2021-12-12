#pragma once

namespace Dynamo
{
	class SRV
	{
	public:
		ID3D11ShaderResourceView* Get();
		ID3D11ShaderResourceView* const* GetConst() const;

		const std::filesystem::path& GetPath() const;

		void Bind(const int aSlot) const;

		void Release();

	private:
		SRV() = default;

	private:
		friend class ResourceFactory;

		ID3D11ShaderResourceView* mySRV = nullptr;
		std::filesystem::path myPath = "";
	};
}
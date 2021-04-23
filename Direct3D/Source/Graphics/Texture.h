#pragma once

#include "Math/Vector.h"

////////////////////////////////
// TEXTURE 2D //////////////////
////////////////////////////////

class Texture2D
{
public:
	Texture2D(ID3D11Device* device, const std::string& filename);
	Texture2D(ID3D11Device* device, uint32_t width, uint32_t height, uint8_t* pixels, uint32_t channels = 4);

	bool operator==(const Texture2D& other);
	bool operator!=(const Texture2D& other);

	uint32_t GetWidth() const;
	uint32_t GetHeight() const;
	uint32_t GetChannels() const;
	ID3D11ShaderResourceView* GetShaderResourceView() const;

	void Bind(ID3D11DeviceContext* device_context, uint32_t slot = 0);
private:
	int m_Width = 0, m_Height = 0, m_Channels = 0;
	wrl::ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView = nullptr;
};

////////////////////////////////
// Sub - Texture 2D ////////////
////////////////////////////////

class SubTexture2D
{
public:
	static Ref<SubTexture2D> CreateFromCoord(const Ref<Texture2D>& texture, const Vec2& coord, const Vec2& size);

	SubTexture2D(const Ref<Texture2D>& texture, const Vec2& min, const Vec2& max);

	const Ref<Texture2D>& GetTexture() const;
	const Vec2* GetTexCoords() const;
private:
	Ref<Texture2D> m_Texture;
	Vec2 m_TexCoords[4] = {};
};

////////////////////////////////
// SAMPLER /////////////////////
////////////////////////////////

class Sampler
{
public:
	Sampler(ID3D11Device* device);

	void Bind(ID3D11DeviceContext* device_context, uint32_t slot = 0);
private:
	wrl::ComPtr<ID3D11SamplerState> m_SamplerState = nullptr;
};
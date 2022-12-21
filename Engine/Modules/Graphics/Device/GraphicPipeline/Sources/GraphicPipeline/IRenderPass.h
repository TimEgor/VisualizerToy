#pragma once

#include "Core/HashFunctions/StdHashCombine.h"
#include "GraphicResourceCommon/Format.h"

#include <vector>

namespace VT
{
	enum class RenderPassAttachmentType
	{
		RENDER_TARGET,
		DEPTH_STENCIL
	};

	enum class RenderPassAttachmentLoadingOperation
	{
		LOADING,
		CLEARING,
		DONT_CARE
	};

	enum class RenderPassAttachmentStoringOperation
	{
		STORING,
		DONT_CARE
	};

	enum class ResourceLayout
	{
		UNDEFINED,
		COLOR,
		PRESENT,
		TRANSFER
	};

	using RenderPassHash = uint32_t;

	struct RenderPassAttachment final
	{
		Format m_format = Format::UNDEFINED;
		RenderPassAttachmentType m_type = RenderPassAttachmentType::RENDER_TARGET;
		RenderPassAttachmentLoadingOperation m_loadingOperation = RenderPassAttachmentLoadingOperation::LOADING;
		RenderPassAttachmentStoringOperation m_storingOperation = RenderPassAttachmentStoringOperation::STORING;
		ResourceLayout m_initialLayout = ResourceLayout::UNDEFINED;
		ResourceLayout m_targetLayout = ResourceLayout::UNDEFINED;

		RenderPassHash getHash() const
		{
			RenderPassHash result = 0;

			hashCombine(result, m_type);
			hashCombine(result, m_loadingOperation);
			hashCombine(result, m_storingOperation);
			hashCombine(result, m_initialLayout);
			hashCombine(result, m_targetLayout);

			return result;
		}
	};

	struct RenderPassInfo final
	{
		using AttachmentsContainer = std::vector<RenderPassAttachment>;

		AttachmentsContainer m_attachments;

		RenderPassHash getHash() const
		{
			RenderPassHash result = 0;
			for (const auto& attachment : m_attachments)
			{
				hashCombine(result, attachment.getHash());
			}

			return result;
		}
	};

	class IRenderPass
	{
	public:
		IRenderPass() = default;
		virtual ~IRenderPass() = default;

		virtual RenderPassHash getHash() const = 0;
	};
}
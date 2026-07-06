#include <VKRHI/VKPipeline.h>

#include <VKRHI/VKDevice.h>

#include <VKRHI/VKShaderModule.h>

#include <vector>

void VKPipeline::CreatePipeline(IDevice* _device)
{
    const VkDevice ldevice = _device->CastToVK()->GetLogicalDevice();

    const VkPipelineLayoutCreateInfo pipelineLayoutCI{
        .sType                      =   VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount             =   0,
        .pSetLayouts                =   VK_NULL_HANDLE,
        .pushConstantRangeCount     =   0,
        .pPushConstantRanges        =   VK_NULL_HANDLE
    };

    vkCreatePipelineLayout(ldevice, &pipelineLayoutCI, nullptr, &m_PipelineLayout);

    const VkVertexInputBindingDescription vertexBinding{
        .binding    =   0,
        .stride     =   3 * sizeof(float),
        .inputRate  =   VK_VERTEX_INPUT_RATE_VERTEX
    };

    const std::vector<VkVertexInputAttributeDescription> vertexAttributes{
        //{ .location = 0, .binding = 0, .format = VK_FORMAT_R32G32B32_SFLOAT },
        //{ .location = 1, .binding = 0, .format = VK_FORMAT_R32G32B32_SFLOAT, .offset = 3 * sizeof(float) },
        //{ .location = 2, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = 6 * sizeof(float) }
    };

    const VkPipelineVertexInputStateCreateInfo vertexInputStateCI{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = 1,
        .pVertexBindingDescriptions = &vertexBinding,
        .vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributes.size()),
        .pVertexAttributeDescriptions = vertexAttributes.data()
    };

    const VkPipelineInputAssemblyStateCreateInfo imputAssemblyCI{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
    };

    VKShaderModule vertModule = VKShaderModule();
    VKShaderModule fragModule = VKShaderModule();

    vertModule.CreateShaderModule(_device, IShaderModule::GetShaderData("Shaders/HelloTriangle.vert.spv"));
    fragModule.CreateShaderModule(_device, IShaderModule::GetShaderData("Shaders/HelloTriangle.frag.spv"));

    const std::vector<VkPipelineShaderStageCreateInfo> shaderStagesCI{
        {   .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = vertModule.CastToVK()->GetRaw(),
            .pName = "main"
        },
        {   .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = fragModule.CastToVK()->GetRaw(),
            .pName = "main"
        },
    };

    const VkPipelineViewportStateCreateInfo viewportState{
        .sType          =   VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount  =   1,
        .scissorCount   =   1
    };

    const std::vector<VkDynamicState> dynamicStates{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    const VkPipelineDynamicStateCreateInfo dynamicState{
        .sType              =   VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount  =   2,
        .pDynamicStates     =   dynamicStates.data()
    };

    const VkFormat imageFormat{ VK_FORMAT_B8G8R8A8_SRGB };

    const VkPipelineRenderingCreateInfo renderingCI{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
        .colorAttachmentCount = 1,
        .pColorAttachmentFormats = &imageFormat
    };

    const VkPipelineColorBlendAttachmentState blendAttachment{
        .colorWriteMask = 0xF
    };

    const VkPipelineColorBlendStateCreateInfo colorBlendStateCI{
        .sType              =   VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .attachmentCount    =   1,
        .pAttachments       =   &blendAttachment
    };

    const VkPipelineRasterizationStateCreateInfo rasterizationStateCI{
        .sType      =   VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .lineWidth  =   1.f
    };

    const VkPipelineMultisampleStateCreateInfo multisampleStateCI{
        .sType                  =   VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples   =   VK_SAMPLE_COUNT_1_BIT
    };

    const VkGraphicsPipelineCreateInfo gPipelineCI{
        .sType                  =    VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext                  =    &renderingCI,
        .stageCount             =    2,
        .pStages                =    shaderStagesCI.data(),
        .pVertexInputState      =    &vertexInputStateCI,
        .pInputAssemblyState    =    &imputAssemblyCI,
        .pViewportState         =    &viewportState,
        .pRasterizationState    =    &rasterizationStateCI,
        .pMultisampleState      =    &multisampleStateCI,
        .pDepthStencilState     =    VK_NULL_HANDLE,
        .pColorBlendState       =    &colorBlendStateCI,
        .pDynamicState          =    &dynamicState,
        .layout                 =    m_PipelineLayout
    };

    vkCreateGraphicsPipelines(ldevice, VK_NULL_HANDLE, 1, &gPipelineCI, nullptr, &m_Pipeline);

    fragModule.DestroyShaderModule(_device);
    vertModule.DestroyShaderModule(_device);
}

void VKPipeline::DestroyPipeline(IDevice* _device)
{
    const VkDevice ldevice = _device->CastToVK()->GetLogicalDevice();
    vkDestroyPipelineLayout(ldevice, m_PipelineLayout, nullptr);
    vkDestroyPipeline(ldevice, m_Pipeline, nullptr);
}
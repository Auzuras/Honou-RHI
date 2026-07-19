#include <VKRHI/VKPipeline.h>

#include <VKRHI/VKDevice.h>

#include <VKRHI/VKShaderModule.h>

#include <vector>
#include <Helpers/Vertex.h>

void VKPipeline::CreatePipeline(IDevice* _device, const GraphicsPipelineDescription& _pipelineDescription)
{
    const VkDevice ldevice = _device->CastToVK()->GetLogicalDevice();

    const VkPushConstantRange range{
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .offset = 0,
        .size = 128
    };

    std::vector<VkDescriptorSetLayoutBinding> bindings(2);
    bindings[0].binding = 0,
    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
    bindings[0].descriptorCount = 1;
    bindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    bindings[0].pImmutableSamplers = nullptr;
    bindings[1].binding = 1,
    bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    bindings[1].descriptorCount = 1;
    bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    bindings[1].pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCI{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = static_cast<uint32_t>(bindings.size()),
        .pBindings = bindings.data(),
    };

    vkCreateDescriptorSetLayout(ldevice, &descriptorSetLayoutCI, nullptr, &m_DescriptorSetLayout);

    const VkPipelineLayoutCreateInfo pipelineLayoutCI{
        .sType                      =   VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount             =   1,
        .pSetLayouts                =   &m_DescriptorSetLayout,
        .pushConstantRangeCount     =   1,
        .pPushConstantRanges        =   &range
    };

    vkCreatePipelineLayout(ldevice, &pipelineLayoutCI, nullptr, &m_PipelineLayout);

    const VkVertexInputBindingDescription vertexBinding{
        .binding    =   0,
        .stride     =   sizeof(Vertex),
        .inputRate  =   VK_VERTEX_INPUT_RATE_VERTEX
    };

    const std::vector<VkVertexInputAttributeDescription> vertexAttributes{
        { .location = 0, .binding = 0, .format = VK_FORMAT_R32G32B32_SFLOAT },
        { .location = 1, .binding = 0, .format = VK_FORMAT_R32G32B32_SFLOAT, .offset = 3 * sizeof(float) },
        { .location = 2, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = 6 * sizeof(float) }
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

    const VkShaderModule vertexShaderModule = _pipelineDescription.vertexShader.shader->CastToVK()->GetRaw();
    const VkShaderModule fragmentShaderModule = _pipelineDescription.fragmentShader.shader->CastToVK()->GetRaw();

    const std::vector<VkPipelineShaderStageCreateInfo> shaderStagesCI{
        {   .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = vertexShaderModule,
            .pName = _pipelineDescription.vertexShader.entryPoint
        },
        {   .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = fragmentShaderModule,
            .pName = _pipelineDescription.fragmentShader.entryPoint
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

    const VkFormat imageFormat{ VK_FORMAT_B8G8R8A8_UNORM };
    const VkFormat depthFormat{ VK_FORMAT_D32_SFLOAT_S8_UINT };

    const VkPipelineRenderingCreateInfo renderingCI{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
        .colorAttachmentCount = 1,
        .pColorAttachmentFormats = &imageFormat,
        .depthAttachmentFormat = depthFormat
    };

    const VkPipelineColorBlendAttachmentState blendAttachment{
        .colorWriteMask = 0xF
    };

    const VkPipelineColorBlendStateCreateInfo colorBlendStateCI{
        .sType              =   VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .attachmentCount    =   1,
        .pAttachments       =   &blendAttachment
    };

    const VkPipelineDepthStencilStateCreateInfo depthStencilCI{
        .sType              =   VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .depthTestEnable    =   VK_TRUE,
        .depthWriteEnable   =   VK_TRUE,
        .depthCompareOp     =   VK_COMPARE_OP_LESS_OR_EQUAL
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
        .pDepthStencilState     =    &depthStencilCI,
        .pColorBlendState       =    &colorBlendStateCI,
        .pDynamicState          =    &dynamicState,
        .layout                 =    m_PipelineLayout
    };

    vkCreateGraphicsPipelines(ldevice, VK_NULL_HANDLE, 1, &gPipelineCI, nullptr, &m_Pipeline);
}

void VKPipeline::CreatePipeline(IDevice* _device, const ComputePipelineDescription& _pipelineDescription)
{
    const VkDevice ldevice = _device->CastToVK()->GetLogicalDevice();
    const VkShaderModule computeShaderModule = _pipelineDescription.computeShader.shader->CastToVK()->GetRaw();

    const VkPipelineShaderStageCreateInfo computeShaderStageCI{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_COMPUTE_BIT,
        .module = computeShaderModule,
        .pName = _pipelineDescription.computeShader.entryPoint
    };

    std::vector<VkDescriptorSetLayoutBinding> bindings(1);
    bindings[0].binding = 0,
    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    bindings[0].descriptorCount = 1;
    bindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
    bindings[0].pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCI{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = static_cast<uint32_t>(bindings.size()),
        .pBindings = bindings.data(),
    };

    vkCreateDescriptorSetLayout(ldevice, &descriptorSetLayoutCI, nullptr, &m_DescriptorSetLayout);

    const VkPipelineLayoutCreateInfo pipelineLayoutCI{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 1,
        .pSetLayouts = &m_DescriptorSetLayout,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = VK_NULL_HANDLE
    };

    vkCreatePipelineLayout(ldevice, &pipelineLayoutCI, nullptr, &m_PipelineLayout);

    const VkComputePipelineCreateInfo cPipelineCI{
        .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
        .stage = computeShaderStageCI,
        .layout = m_PipelineLayout
    };

    vkCreateComputePipelines(ldevice, VK_NULL_HANDLE, 1, &cPipelineCI, nullptr, &m_Pipeline);
}

void VKPipeline::DestroyPipeline(IDevice* _device)
{
    const VkDevice ldevice = _device->CastToVK()->GetLogicalDevice();
    vkDestroyDescriptorSetLayout(ldevice, m_DescriptorSetLayout, nullptr);
    vkDestroyPipelineLayout(ldevice, m_PipelineLayout, nullptr);
    vkDestroyPipeline(ldevice, m_Pipeline, nullptr);
}
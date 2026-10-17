"dxc.exe" -T vs_6_0 -E main Unlit/DX12/Unlit_VERT.hlsl -Fo Unlit/DX12/Unlit_VERT.cso
"dxc.exe" -T ps_6_0 -E main Unlit/DX12/Unlit_PIX.hlsl -Fo Unlit/DX12/Unlit_PIX.cso
pause
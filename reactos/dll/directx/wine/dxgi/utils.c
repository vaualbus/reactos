/*
 * Copyright 2008 Henri Verbeet for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 *
 */

#include "dxgi_private.h"

#define WINE_DXGI_TO_STR(x) case x: return #x

const char *debug_dxgi_format(DXGI_FORMAT format)
{
    switch(format)
    {
        WINE_DXGI_TO_STR(DXGI_FORMAT_UNKNOWN);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R32G32B32A32_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R32G32B32A32_FLOAT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R32G32B32A32_UINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R32G32B32A32_SINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R32G32B32_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R32G32B32_FLOAT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R32G32B32_UINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R32G32B32_SINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R16G16B16A16_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R16G16B16A16_FLOAT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R16G16B16A16_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R16G16B16A16_UINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R16G16B16A16_SNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R16G16B16A16_SINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R32G32_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R32G32_FLOAT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R32G32_UINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R32G32_SINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R32G8X24_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_D32_FLOAT_S8X24_UINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_X32_TYPELESS_G8X24_UINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R10G10B10A2_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R10G10B10A2_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R10G10B10A2_UINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R11G11B10_FLOAT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R8G8B8A8_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R8G8B8A8_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R8G8B8A8_UINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R8G8B8A8_SNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R8G8B8A8_SINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R16G16_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R16G16_FLOAT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R16G16_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R16G16_UINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R16G16_SNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R16G16_SINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R32_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_D32_FLOAT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R32_FLOAT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R32_UINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R32_SINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R24G8_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_D24_UNORM_S8_UINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R24_UNORM_X8_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_X24_TYPELESS_G8_UINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R8G8_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R8G8_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R8G8_UINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R8G8_SNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R8G8_SINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R16_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R16_FLOAT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_D16_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R16_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R16_UINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R16_SNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R16_SINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R8_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R8_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R8_UINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R8_SNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R8_SINT);
        WINE_DXGI_TO_STR(DXGI_FORMAT_A8_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R1_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R9G9B9E5_SHAREDEXP);
        WINE_DXGI_TO_STR(DXGI_FORMAT_R8G8_B8G8_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_G8R8_G8B8_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_BC1_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_BC1_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_BC1_UNORM_SRGB);
        WINE_DXGI_TO_STR(DXGI_FORMAT_BC2_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_BC2_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_BC2_UNORM_SRGB);
        WINE_DXGI_TO_STR(DXGI_FORMAT_BC3_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_BC3_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_BC3_UNORM_SRGB);
        WINE_DXGI_TO_STR(DXGI_FORMAT_BC4_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_BC4_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_BC4_SNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_BC5_TYPELESS);
        WINE_DXGI_TO_STR(DXGI_FORMAT_BC5_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_BC5_SNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_B5G6R5_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_B5G5R5A1_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_B8G8R8A8_UNORM);
        WINE_DXGI_TO_STR(DXGI_FORMAT_B8G8R8X8_UNORM);
        default:
            FIXME("Unrecognized DXGI_FORMAT %#x\n", format);
            return "unrecognized";
    }
}

#undef WINE_DXGI_TO_STR

DXGI_FORMAT dxgi_format_from_wined3dformat(enum wined3d_format_id format)
{
    switch(format)
    {
        case WINED3DFMT_UNKNOWN: return DXGI_FORMAT_UNKNOWN;
        case WINED3DFMT_R32G32B32A32_TYPELESS: return DXGI_FORMAT_R32G32B32A32_TYPELESS;
        case WINED3DFMT_R32G32B32A32_FLOAT: return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case WINED3DFMT_R32G32B32A32_UINT: return DXGI_FORMAT_R32G32B32A32_UINT;
        case WINED3DFMT_R32G32B32A32_SINT: return DXGI_FORMAT_R32G32B32A32_SINT;
        case WINED3DFMT_R32G32B32_TYPELESS: return DXGI_FORMAT_R32G32B32_TYPELESS;
        case WINED3DFMT_R32G32B32_FLOAT: return DXGI_FORMAT_R32G32B32_FLOAT;
        case WINED3DFMT_R32G32B32_UINT: return DXGI_FORMAT_R32G32B32_UINT;
        case WINED3DFMT_R32G32B32_SINT: return DXGI_FORMAT_R32G32B32_SINT;
        case WINED3DFMT_R16G16B16A16_TYPELESS: return DXGI_FORMAT_R16G16B16A16_TYPELESS;
        case WINED3DFMT_R16G16B16A16_FLOAT: return DXGI_FORMAT_R16G16B16A16_FLOAT;
        case WINED3DFMT_R16G16B16A16_UNORM: return DXGI_FORMAT_R16G16B16A16_UNORM;
        case WINED3DFMT_R16G16B16A16_UINT: return DXGI_FORMAT_R16G16B16A16_UINT;
        case WINED3DFMT_R16G16B16A16_SNORM: return DXGI_FORMAT_R16G16B16A16_SNORM;
        case WINED3DFMT_R16G16B16A16_SINT: return DXGI_FORMAT_R16G16B16A16_SINT;
        case WINED3DFMT_R32G32_TYPELESS: return DXGI_FORMAT_R32G32_TYPELESS;
        case WINED3DFMT_R32G32_FLOAT: return DXGI_FORMAT_R32G32_FLOAT;
        case WINED3DFMT_R32G32_UINT: return DXGI_FORMAT_R32G32_UINT;
        case WINED3DFMT_R32G32_SINT: return DXGI_FORMAT_R32G32_SINT;
        case WINED3DFMT_R32G8X24_TYPELESS: return DXGI_FORMAT_R32G8X24_TYPELESS;
        case WINED3DFMT_D32_FLOAT_S8X24_UINT: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        case WINED3DFMT_R32_FLOAT_X8X24_TYPELESS: return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
        case WINED3DFMT_X32_TYPELESS_G8X24_UINT: return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
        case WINED3DFMT_R10G10B10A2_TYPELESS: return DXGI_FORMAT_R10G10B10A2_TYPELESS;
        case WINED3DFMT_R10G10B10A2_UNORM: return DXGI_FORMAT_R10G10B10A2_UNORM;
        case WINED3DFMT_R10G10B10A2_UINT: return DXGI_FORMAT_R10G10B10A2_UINT;
        case WINED3DFMT_R11G11B10_FLOAT: return DXGI_FORMAT_R11G11B10_FLOAT;
        case WINED3DFMT_R8G8B8A8_TYPELESS: return DXGI_FORMAT_R8G8B8A8_TYPELESS;
        case WINED3DFMT_R8G8B8A8_UNORM: return DXGI_FORMAT_R8G8B8A8_UNORM;
        case WINED3DFMT_R8G8B8A8_UNORM_SRGB: return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        case WINED3DFMT_R8G8B8A8_UINT: return DXGI_FORMAT_R8G8B8A8_UINT;
        case WINED3DFMT_R8G8B8A8_SNORM: return DXGI_FORMAT_R8G8B8A8_SNORM;
        case WINED3DFMT_R8G8B8A8_SINT: return DXGI_FORMAT_R8G8B8A8_SINT;
        case WINED3DFMT_R16G16_TYPELESS: return DXGI_FORMAT_R16G16_TYPELESS;
        case WINED3DFMT_R16G16_FLOAT: return DXGI_FORMAT_R16G16_FLOAT;
        case WINED3DFMT_R16G16_UNORM: return DXGI_FORMAT_R16G16_UNORM;
        case WINED3DFMT_R16G16_UINT: return DXGI_FORMAT_R16G16_UINT;
        case WINED3DFMT_R16G16_SNORM: return DXGI_FORMAT_R16G16_SNORM;
        case WINED3DFMT_R16G16_SINT: return DXGI_FORMAT_R16G16_SINT;
        case WINED3DFMT_R32_TYPELESS: return DXGI_FORMAT_R32_TYPELESS;
        case WINED3DFMT_D32_FLOAT: return DXGI_FORMAT_D32_FLOAT;
        case WINED3DFMT_R32_FLOAT: return DXGI_FORMAT_R32_FLOAT;
        case WINED3DFMT_R32_UINT: return DXGI_FORMAT_R32_UINT;
        case WINED3DFMT_R32_SINT: return DXGI_FORMAT_R32_SINT;
        case WINED3DFMT_R24G8_TYPELESS: return DXGI_FORMAT_R24G8_TYPELESS;
        case WINED3DFMT_D24_UNORM_S8_UINT: return DXGI_FORMAT_D24_UNORM_S8_UINT;
        case WINED3DFMT_R24_UNORM_X8_TYPELESS: return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        case WINED3DFMT_X24_TYPELESS_G8_UINT: return DXGI_FORMAT_X24_TYPELESS_G8_UINT;
        case WINED3DFMT_R8G8_TYPELESS: return DXGI_FORMAT_R8G8_TYPELESS;
        case WINED3DFMT_R8G8_UNORM: return DXGI_FORMAT_R8G8_UNORM;
        case WINED3DFMT_R8G8_UINT: return DXGI_FORMAT_R8G8_UINT;
        case WINED3DFMT_R8G8_SNORM: return DXGI_FORMAT_R8G8_SNORM;
        case WINED3DFMT_R8G8_SINT: return DXGI_FORMAT_R8G8_SINT;
        case WINED3DFMT_R16_TYPELESS: return DXGI_FORMAT_R16_TYPELESS;
        case WINED3DFMT_R16_FLOAT: return DXGI_FORMAT_R16_FLOAT;
        case WINED3DFMT_D16_UNORM: return DXGI_FORMAT_D16_UNORM;
        case WINED3DFMT_R16_UNORM: return DXGI_FORMAT_R16_UNORM;
        case WINED3DFMT_R16_UINT: return DXGI_FORMAT_R16_UINT;
        case WINED3DFMT_R16_SNORM: return DXGI_FORMAT_R16_SNORM;
        case WINED3DFMT_R16_SINT: return DXGI_FORMAT_R16_SINT;
        case WINED3DFMT_R8_TYPELESS: return DXGI_FORMAT_R8_TYPELESS;
        case WINED3DFMT_R8_UNORM: return DXGI_FORMAT_R8_UNORM;
        case WINED3DFMT_R8_UINT: return DXGI_FORMAT_R8_UINT;
        case WINED3DFMT_R8_SNORM: return DXGI_FORMAT_R8_SNORM;
        case WINED3DFMT_R8_SINT: return DXGI_FORMAT_R8_SINT;
        case WINED3DFMT_A8_UNORM: return DXGI_FORMAT_A8_UNORM;
        case WINED3DFMT_R1_UNORM: return DXGI_FORMAT_R1_UNORM;
        case WINED3DFMT_R9G9B9E5_SHAREDEXP: return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
        case WINED3DFMT_R8G8_B8G8_UNORM: return DXGI_FORMAT_R8G8_B8G8_UNORM;
        case WINED3DFMT_G8R8_G8B8_UNORM: return DXGI_FORMAT_G8R8_G8B8_UNORM;
        case WINED3DFMT_BC1_TYPELESS: return DXGI_FORMAT_BC1_TYPELESS;
        case WINED3DFMT_BC1_UNORM: return DXGI_FORMAT_BC1_UNORM;
        case WINED3DFMT_BC1_UNORM_SRGB: return DXGI_FORMAT_BC1_UNORM_SRGB;
        case WINED3DFMT_BC2_TYPELESS: return DXGI_FORMAT_BC2_TYPELESS;
        case WINED3DFMT_BC2_UNORM: return DXGI_FORMAT_BC2_UNORM;
        case WINED3DFMT_BC2_UNORM_SRGB: return DXGI_FORMAT_BC2_UNORM_SRGB;
        case WINED3DFMT_BC3_TYPELESS: return DXGI_FORMAT_BC3_TYPELESS;
        case WINED3DFMT_BC3_UNORM: return DXGI_FORMAT_BC3_UNORM;
        case WINED3DFMT_BC3_UNORM_SRGB: return DXGI_FORMAT_BC3_UNORM_SRGB;
        case WINED3DFMT_BC4_TYPELESS: return DXGI_FORMAT_BC4_TYPELESS;
        case WINED3DFMT_BC4_UNORM: return DXGI_FORMAT_BC4_UNORM;
        case WINED3DFMT_BC4_SNORM: return DXGI_FORMAT_BC4_SNORM;
        case WINED3DFMT_BC5_TYPELESS: return DXGI_FORMAT_BC5_TYPELESS;
        case WINED3DFMT_BC5_UNORM: return DXGI_FORMAT_BC5_UNORM;
        case WINED3DFMT_BC5_SNORM: return DXGI_FORMAT_BC5_SNORM;
        case WINED3DFMT_B5G6R5_UNORM: return DXGI_FORMAT_B5G6R5_UNORM;
        case WINED3DFMT_B5G5R5A1_UNORM: return DXGI_FORMAT_B5G5R5A1_UNORM;
        case WINED3DFMT_B8G8R8A8_UNORM: return DXGI_FORMAT_B8G8R8A8_UNORM;
        case WINED3DFMT_B8G8R8X8_UNORM: return DXGI_FORMAT_B8G8R8X8_UNORM;
        default:
            FIXME("Unhandled wined3d format %#x.\n", format);
            return DXGI_FORMAT_UNKNOWN;
    }
}

enum wined3d_format_id wined3dformat_from_dxgi_format(DXGI_FORMAT format)
{
    switch(format)
    {
        case DXGI_FORMAT_UNKNOWN: return WINED3DFMT_UNKNOWN;
        case DXGI_FORMAT_R32G32B32A32_TYPELESS: return WINED3DFMT_R32G32B32A32_TYPELESS;
        case DXGI_FORMAT_R32G32B32A32_FLOAT: return WINED3DFMT_R32G32B32A32_FLOAT;
        case DXGI_FORMAT_R32G32B32A32_UINT: return WINED3DFMT_R32G32B32A32_UINT;
        case DXGI_FORMAT_R32G32B32A32_SINT: return WINED3DFMT_R32G32B32A32_SINT;
        case DXGI_FORMAT_R32G32B32_TYPELESS: return WINED3DFMT_R32G32B32_TYPELESS;
        case DXGI_FORMAT_R32G32B32_FLOAT: return WINED3DFMT_R32G32B32_FLOAT;
        case DXGI_FORMAT_R32G32B32_UINT: return WINED3DFMT_R32G32B32_UINT;
        case DXGI_FORMAT_R32G32B32_SINT: return WINED3DFMT_R32G32B32_SINT;
        case DXGI_FORMAT_R16G16B16A16_TYPELESS: return WINED3DFMT_R16G16B16A16_TYPELESS;
        case DXGI_FORMAT_R16G16B16A16_FLOAT: return WINED3DFMT_R16G16B16A16_FLOAT;
        case DXGI_FORMAT_R16G16B16A16_UNORM: return WINED3DFMT_R16G16B16A16_UNORM;
        case DXGI_FORMAT_R16G16B16A16_UINT: return WINED3DFMT_R16G16B16A16_UINT;
        case DXGI_FORMAT_R16G16B16A16_SNORM: return WINED3DFMT_R16G16B16A16_SNORM;
        case DXGI_FORMAT_R16G16B16A16_SINT: return WINED3DFMT_R16G16B16A16_SINT;
        case DXGI_FORMAT_R32G32_TYPELESS: return WINED3DFMT_R32G32_TYPELESS;
        case DXGI_FORMAT_R32G32_FLOAT: return WINED3DFMT_R32G32_FLOAT;
        case DXGI_FORMAT_R32G32_UINT: return WINED3DFMT_R32G32_UINT;
        case DXGI_FORMAT_R32G32_SINT: return WINED3DFMT_R32G32_SINT;
        case DXGI_FORMAT_R32G8X24_TYPELESS: return WINED3DFMT_R32G8X24_TYPELESS;
        case DXGI_FORMAT_D32_FLOAT_S8X24_UINT: return WINED3DFMT_D32_FLOAT_S8X24_UINT;
        case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS: return WINED3DFMT_R32_FLOAT_X8X24_TYPELESS;
        case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT: return WINED3DFMT_X32_TYPELESS_G8X24_UINT;
        case DXGI_FORMAT_R10G10B10A2_TYPELESS: return WINED3DFMT_R10G10B10A2_TYPELESS;
        case DXGI_FORMAT_R10G10B10A2_UNORM: return WINED3DFMT_R10G10B10A2_UNORM;
        case DXGI_FORMAT_R10G10B10A2_UINT: return WINED3DFMT_R10G10B10A2_UINT;
        case DXGI_FORMAT_R11G11B10_FLOAT: return WINED3DFMT_R11G11B10_FLOAT;
        case DXGI_FORMAT_R8G8B8A8_TYPELESS: return WINED3DFMT_R8G8B8A8_TYPELESS;
        case DXGI_FORMAT_R8G8B8A8_UNORM: return WINED3DFMT_R8G8B8A8_UNORM;
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB: return WINED3DFMT_R8G8B8A8_UNORM_SRGB;
        case DXGI_FORMAT_R8G8B8A8_UINT: return WINED3DFMT_R8G8B8A8_UINT;
        case DXGI_FORMAT_R8G8B8A8_SNORM: return WINED3DFMT_R8G8B8A8_SNORM;
        case DXGI_FORMAT_R8G8B8A8_SINT: return WINED3DFMT_R8G8B8A8_SINT;
        case DXGI_FORMAT_R16G16_TYPELESS: return WINED3DFMT_R16G16_TYPELESS;
        case DXGI_FORMAT_R16G16_FLOAT: return WINED3DFMT_R16G16_FLOAT;
        case DXGI_FORMAT_R16G16_UNORM: return WINED3DFMT_R16G16_UNORM;
        case DXGI_FORMAT_R16G16_UINT: return WINED3DFMT_R16G16_UINT;
        case DXGI_FORMAT_R16G16_SNORM: return WINED3DFMT_R16G16_SNORM;
        case DXGI_FORMAT_R16G16_SINT: return WINED3DFMT_R16G16_SINT;
        case DXGI_FORMAT_R32_TYPELESS: return WINED3DFMT_R32_TYPELESS;
        case DXGI_FORMAT_D32_FLOAT: return WINED3DFMT_D32_FLOAT;
        case DXGI_FORMAT_R32_FLOAT: return WINED3DFMT_R32_FLOAT;
        case DXGI_FORMAT_R32_UINT: return WINED3DFMT_R32_UINT;
        case DXGI_FORMAT_R32_SINT: return WINED3DFMT_R32_SINT;
        case DXGI_FORMAT_R24G8_TYPELESS: return WINED3DFMT_R24G8_TYPELESS;
        case DXGI_FORMAT_D24_UNORM_S8_UINT: return WINED3DFMT_D24_UNORM_S8_UINT;
        case DXGI_FORMAT_R24_UNORM_X8_TYPELESS: return WINED3DFMT_R24_UNORM_X8_TYPELESS;
        case DXGI_FORMAT_X24_TYPELESS_G8_UINT: return WINED3DFMT_X24_TYPELESS_G8_UINT;
        case DXGI_FORMAT_R8G8_TYPELESS: return WINED3DFMT_R8G8_TYPELESS;
        case DXGI_FORMAT_R8G8_UNORM: return WINED3DFMT_R8G8_UNORM;
        case DXGI_FORMAT_R8G8_UINT: return WINED3DFMT_R8G8_UINT;
        case DXGI_FORMAT_R8G8_SNORM: return WINED3DFMT_R8G8_SNORM;
        case DXGI_FORMAT_R8G8_SINT: return WINED3DFMT_R8G8_SINT;
        case DXGI_FORMAT_R16_TYPELESS: return WINED3DFMT_R16_TYPELESS;
        case DXGI_FORMAT_R16_FLOAT: return WINED3DFMT_R16_FLOAT;
        case DXGI_FORMAT_D16_UNORM: return WINED3DFMT_D16_UNORM;
        case DXGI_FORMAT_R16_UNORM: return WINED3DFMT_R16_UNORM;
        case DXGI_FORMAT_R16_UINT: return WINED3DFMT_R16_UINT;
        case DXGI_FORMAT_R16_SNORM: return WINED3DFMT_R16_SNORM;
        case DXGI_FORMAT_R16_SINT: return WINED3DFMT_R16_SINT;
        case DXGI_FORMAT_R8_TYPELESS: return WINED3DFMT_R8_TYPELESS;
        case DXGI_FORMAT_R8_UNORM: return WINED3DFMT_R8_UNORM;
        case DXGI_FORMAT_R8_UINT: return WINED3DFMT_R8_UINT;
        case DXGI_FORMAT_R8_SNORM: return WINED3DFMT_R8_SNORM;
        case DXGI_FORMAT_R8_SINT: return WINED3DFMT_R8_SINT;
        case DXGI_FORMAT_A8_UNORM: return WINED3DFMT_A8_UNORM;
        case DXGI_FORMAT_R1_UNORM: return WINED3DFMT_R1_UNORM;
        case DXGI_FORMAT_R9G9B9E5_SHAREDEXP: return WINED3DFMT_R9G9B9E5_SHAREDEXP;
        case DXGI_FORMAT_R8G8_B8G8_UNORM: return WINED3DFMT_R8G8_B8G8_UNORM;
        case DXGI_FORMAT_G8R8_G8B8_UNORM: return WINED3DFMT_G8R8_G8B8_UNORM;
        case DXGI_FORMAT_BC1_TYPELESS: return WINED3DFMT_BC1_TYPELESS;
        case DXGI_FORMAT_BC1_UNORM: return WINED3DFMT_BC1_UNORM;
        case DXGI_FORMAT_BC1_UNORM_SRGB: return WINED3DFMT_BC1_UNORM_SRGB;
        case DXGI_FORMAT_BC2_TYPELESS: return WINED3DFMT_BC2_TYPELESS;
        case DXGI_FORMAT_BC2_UNORM: return WINED3DFMT_BC2_UNORM;
        case DXGI_FORMAT_BC2_UNORM_SRGB: return WINED3DFMT_BC2_UNORM_SRGB;
        case DXGI_FORMAT_BC3_TYPELESS: return WINED3DFMT_BC3_TYPELESS;
        case DXGI_FORMAT_BC3_UNORM: return WINED3DFMT_BC3_UNORM;
        case DXGI_FORMAT_BC3_UNORM_SRGB: return WINED3DFMT_BC3_UNORM_SRGB;
        case DXGI_FORMAT_BC4_TYPELESS: return WINED3DFMT_BC4_TYPELESS;
        case DXGI_FORMAT_BC4_UNORM: return WINED3DFMT_BC4_UNORM;
        case DXGI_FORMAT_BC4_SNORM: return WINED3DFMT_BC4_SNORM;
        case DXGI_FORMAT_BC5_TYPELESS: return WINED3DFMT_BC5_TYPELESS;
        case DXGI_FORMAT_BC5_UNORM: return WINED3DFMT_BC5_UNORM;
        case DXGI_FORMAT_BC5_SNORM: return WINED3DFMT_BC5_SNORM;
        case DXGI_FORMAT_B5G6R5_UNORM: return WINED3DFMT_B5G6R5_UNORM;
        case DXGI_FORMAT_B5G5R5A1_UNORM: return WINED3DFMT_B5G5R5A1_UNORM;
        case DXGI_FORMAT_B8G8R8A8_UNORM: return WINED3DFMT_B8G8R8A8_UNORM;
        case DXGI_FORMAT_B8G8R8X8_UNORM: return WINED3DFMT_B8G8R8X8_UNORM;
        default:
            FIXME("Unhandled DXGI_FORMAT %#x\n", format);
            return WINED3DFMT_UNKNOWN;
    }
}

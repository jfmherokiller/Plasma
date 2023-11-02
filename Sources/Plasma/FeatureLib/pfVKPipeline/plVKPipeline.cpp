/*==LICENSE==*

CyanWorlds.com Engine - MMOG client, server and tools
Copyright (C) 2011  Cyan Worlds, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Additional permissions under GNU GPL version 3 section 7

If you modify this Program, or any covered work, by linking or
combining it with any of RAD Game Tools Bink SDK, Autodesk 3ds Max SDK,
NVIDIA PhysX SDK, Microsoft DirectX SDK, OpenSSL library, Independent
JPEG Group JPEG library, Microsoft Windows Media SDK, or Apple QuickTime SDK
(or a modified version of those libraries),
containing parts covered by the terms of the Bink SDK EULA, 3ds Max EULA,
PhysX SDK EULA, DirectX SDK EULA, OpenSSL and SSLeay licenses, IJG
JPEG Library README, Windows Media SDK EULA, or QuickTime SDK EULA, the
licensors of this Program grant you additional
permission to convey the resulting work. Corresponding Source for a
non-source form of such a combination shall include the source code for
the parts of OpenSSL and IJG JPEG Library used as well as that of the covered
work.

You can contact Cyan Worlds, Inc. by email legal@cyan.com
 or by snail mail at:
      Cyan Worlds, Inc.
      14617 N Newport Hwy
      Mead, WA   99021

*==LICENSE==*/
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  plVKPipeline Class Functions                                             //
//  plPipeline derivative for OpenGL                                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "HeadSpin.h"
#include "hsWindows.h"

#include <string_theory/string>

#include "plPipeline/hsWinRef.h"

#include "plVKPipeline.h"
#include "plVKPlateManager.h"

#ifdef HS_SIMD_INCLUDE
#  include HS_SIMD_INCLUDE
#endif

plVKEnumerate plVKPipeline::enumerator;

plVKPipeline::plVKPipeline(hsWindowHndl display, hsWindowHndl window, const hsG3DDeviceModeRecord *devMode)
    : pl3DPipeline(devMode)
{
    fPlateMgr = new plVKPlateManager(this);
}

bool plVKPipeline::PreRender(plDrawable* drawable, std::vector<int16_t>& visList, plVisMgr* visMgr)
{
    return false;
}

bool plVKPipeline::PrepForRender(plDrawable* drawable, std::vector<int16_t>& visList, plVisMgr* visMgr)
{
    return false;
}

void plVKPipeline::Render(plDrawable* d, const std::vector<int16_t>& visList)
{}

plTextFont* plVKPipeline::MakeTextFont(ST::string face, uint16_t size)
{
    return nullptr;
}

void plVKPipeline::CheckVertexBufferRef(plGBufferGroup* owner, uint32_t idx)
{}

void plVKPipeline::CheckIndexBufferRef(plGBufferGroup* owner, uint32_t idx)
{}

bool plVKPipeline::OpenAccess(plAccessSpan& dst, plDrawableSpans* d, const plVertexSpan* span, bool readOnly)
{
    return false;
}

bool plVKPipeline::CloseAccess(plAccessSpan& acc)
{
    return false;
}

void plVKPipeline::CheckTextureRef(plLayerInterface* lay)
{}

void plVKPipeline::PushRenderRequest(plRenderRequest* req)
{}

void plVKPipeline::PopRenderRequest(plRenderRequest* req)
{}

void plVKPipeline::ClearRenderTarget(plDrawable* d)
{}

void plVKPipeline::ClearRenderTarget(const hsColorRGBA* col, const float* depth)
{}

hsGDeviceRef* plVKPipeline::MakeRenderTargetRef(plRenderTarget* owner)
{
    return nullptr;
}

void plVKPipeline::PushRenderTarget(plRenderTarget* target)
{}

plRenderTarget* plVKPipeline::PopRenderTarget()
{
    return nullptr;
}

bool plVKPipeline::BeginRender()
{
    return false;
}

bool plVKPipeline::EndRender()
{
    return false;
}

void plVKPipeline::RenderScreenElements()
{}

bool plVKPipeline::IsFullScreen() const
{
    return false;
}

uint32_t plVKPipeline::ColorDepth() const
{
    return 0;
}

void plVKPipeline::Resize(uint32_t width, uint32_t height)
{}

bool plVKPipeline::CheckResources()
{
    return false;
}

void plVKPipeline::LoadResources()
{}

void plVKPipeline::SetZBiasScale(float scale)
{}

float plVKPipeline::GetZBiasScale() const
{
    return 0.0f;
}

void plVKPipeline::SetWorldToCamera(const hsMatrix44& w2c, const hsMatrix44& c2w)
{}

void plVKPipeline::RefreshScreenMatrices()
{}

void plVKPipeline::SubmitClothingOutfit(plClothingOutfit* co)
{}

bool plVKPipeline::SetGamma(float eR, float eG, float eB)
{
    return false;
}

bool plVKPipeline::SetGamma(const uint16_t* const tabR, const uint16_t* const tabG, const uint16_t* const tabB)
{
    return false;
}

bool plVKPipeline::CaptureScreen(plMipmap* dest, bool flipVertical, uint16_t desiredWidth, uint16_t desiredHeight)
{
    return false;
}

plMipmap* plVKPipeline::ExtractMipMap(plRenderTarget* targ)
{
    return nullptr;
}

void plVKPipeline::GetSupportedDisplayModes(std::vector<plDisplayMode> *res, int ColorDepth)
{}

int plVKPipeline::GetMaxAnisotropicSamples()
{
    return 0;
}

int plVKPipeline::GetMaxAntiAlias(int Width, int Height, int ColorDepth)
{
    return 0;
}

void plVKPipeline::ResetDisplayDevice(int Width, int Height, int ColorDepth, bool Windowed, int NumAASamples, int MaxAnisotropicSamples, bool vSync)
{}

void plVKPipeline::RenderSpans(plDrawableSpans* ice, const std::vector<int16_t>& visList)
{}

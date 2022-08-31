#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>
#include <bx/allocator.h>
#include <bx/timer.h>
#include <bx/math.h>
#include <dear-imgui/imgui.h>
#include <dear-imgui/imgui_internal.h>

#include "DearImgui.h"

#include "vs_ocornut_imgui.bin.h"
#include "fs_ocornut_imgui.bin.h"
#include "vs_imgui_image.bin.h"
#include "fs_imgui_image.bin.h"

#include "roboto_regular.ttf.h"
#include "robotomono_regular.ttf.h"
#include "icons_kenney.ttf.h"
#include "icons_font_awesome.ttf.h"

static void *memAlloc(size_t _size, void *_userData);
static void memFree(void *_ptr, void *_userData);

static const bgfx::EmbeddedShader sEmbeddedShaders[] =
    {
        BGFX_EMBEDDED_SHADER(vs_ocornut_imgui),
        BGFX_EMBEDDED_SHADER(fs_ocornut_imgui),
        BGFX_EMBEDDED_SHADER(vs_imgui_image),
        BGFX_EMBEDDED_SHADER(fs_imgui_image),

        BGFX_EMBEDDED_SHADER_END()};

struct FontRangeMerge
{
    const void *data;
    size_t size;
    ImWchar ranges[3];
};

static FontRangeMerge sFontRangeMerge[] =
    {
        {s_iconsKenneyTtf, sizeof(s_iconsKenneyTtf), {ICON_MIN_KI, ICON_MAX_KI, 0}},
        {s_iconsFontAwesomeTtf, sizeof(s_iconsFontAwesomeTtf), {ICON_MIN_FA, ICON_MAX_FA, 0}},
};

struct DearImguiContext
{
    ImGuiContext *mImgui;
    bx::AllocatorI *mAllocator;

    bgfx::VertexLayout mLayout;
    bgfx::ProgramHandle mProgram;
    bgfx::ProgramHandle mImageProgram;
    bgfx::TextureHandle mTexture;
    bgfx::UniformHandle mSTex;
    bgfx::UniformHandle mUimageLodEnabled;

    ImFont *mFont[ImGui::Font::Count];

    int32_t mLastScroll;
    int64_t mLastTime;
    bgfx::ViewId mViewId;
};
static DearImguiContext sContext;

static void *memAlloc(size_t _size, void *_userData)
{
    BX_UNUSED(_userData);
    return BX_ALLOC(sContext.mAllocator, _size);
}

static void memFree(void *_ptr, void *_userData)
{
    BX_UNUSED(_userData);
    BX_FREE(sContext.mAllocator, _ptr);
}

void CreateGUI()
{
    sContext.mViewId = 255;
    // sContext.mLastScroll
    static bx::DefaultAllocator allocator;
    sContext.mAllocator = &allocator;
    ImGui::SetAllocatorFunctions(memAlloc, memFree, NULL);

    sContext.mImgui = ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();

    io.DisplaySize = ImVec2(1280.0f, 720.0f);
    io.DeltaTime = 1.0f / 60.0f;
    io.IniFilename = NULL;
    // Doug Binks' darl color scheme
    // https://gist.github.com/dougbinks/8089b4bbaccaaf6fa204236978d165a9
    ImGuiStyle &style = ImGui::GetStyle();

    ImGui::StyleColorsDark(&style);

    style.FrameRounding = 4.0f;
    style.WindowBorderSize = 0.0f;
    
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

    bgfx::RendererType::Enum type = bgfx::getRendererType();
    sContext.mProgram = bgfx::createProgram(bgfx::createEmbeddedShader(sEmbeddedShaders, type, "vs_ocornut_imgui"),
                                            bgfx::createEmbeddedShader(sEmbeddedShaders, type, "fs_ocornut_imgui"),
                                            true);

    sContext.mUimageLodEnabled = bgfx::createUniform("u_imageLodEnabled", bgfx::UniformType::Vec4);
    sContext.mImageProgram = bgfx::createProgram(bgfx::createEmbeddedShader(sEmbeddedShaders, type, "vs_imgui_image"),
                                                 bgfx::createEmbeddedShader(sEmbeddedShaders, type, "fs_imgui_image"), true);
    sContext.mLayout.begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();
    sContext.mSTex = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);

    // Build and load the texture atlas into a texture
    // (In the examples/ app this is usually done within the ImGui_ImplXXX_Init() function from one of the demo Renderer)
    uint8_t *data;
    int32_t width, height;
    {
        ImFontConfig config;
        config.FontDataOwnedByAtlas = false;
        config.MergeMode = false;

        const ImWchar *ranges = io.Fonts->GetGlyphRangesCyrillic();
        sContext.mFont[ImGui::Font::Regular] = io.Fonts->AddFontFromMemoryTTF((void *)s_robotoRegularTtf, sizeof(s_robotoRegularTtf), 18.0f, &config, ranges);
        sContext.mFont[ImGui::Font::Mono] = io.Fonts->AddFontFromMemoryTTF((void *)s_robotoMonoRegularTtf, sizeof(s_robotoMonoRegularTtf), 15.0f, &config, ranges);

        config.MergeMode = true;
        config.DstFont = sContext.mFont[ImGui::Font::Regular];

        for (uint32_t ii = 0; ii < BX_COUNTOF(sFontRangeMerge); ++ii)
        {
            const FontRangeMerge &frm = sFontRangeMerge[ii];

            io.Fonts->AddFontFromMemoryTTF((void *)frm.data, (int)frm.size, 15.0f, &config, frm.ranges);
        }
    }
    io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);

    sContext.mTexture = bgfx::createTexture2D(
        (uint16_t)width, (uint16_t)height, false, 1, bgfx::TextureFormat::BGRA8, 0, bgfx::copy(data, width * height * 4));
    ImGui::InitDockContext();
}

void BeginGUIFrame(int32_t mouseX, int32_t mouseY, uint8_t button, int32_t scroll, uint16_t width, uint16_t height, int inputChar, bgfx::ViewId viewId)
{
    // Setup low-level inputs, e.g. on Win32: calling GetKeyboardState(), or write to those fields from your Windows message handlers, etc.
    // (In the examples/ app this is usually done within the ImGui_ImplXXX_NewFrame() function from one of the demo Platform Backends)
    ImGuiIO &io = ImGui::GetIO();

    // set the time elapsed since the previous frame (in seconds)
    const int64_t now = bx::getHPCounter();
    const int64_t frameTime = now - sContext.mLastTime;
    sContext.mLastTime = now;
    const double freq = double(bx::getHPFrequency());
    io.DeltaTime = float(frameTime / freq);

    // set the current display width& height here
    io.DisplaySize = ImVec2((float)width, (float)height);

    // set the mouse position and mouse button states
    io.AddMousePosEvent((float)mouseX, (float)mouseY);
    io.AddMouseButtonEvent(ImGuiMouseButton_Left, 0 != (button & IMGUI_MBUT_LEFT));
    io.AddMouseButtonEvent(ImGuiMouseButton_Right, 0 != (button & IMGUI_MBUT_RIGHT));
    io.AddMouseButtonEvent(ImGuiMouseButton_Middle, 0 != (button & IMGUI_MBUT_MIDDLE));
    io.AddMouseWheelEvent(0.0f, (float)(scroll - sContext.mLastScroll));

    sContext.mLastScroll = scroll;

    // Call NewFrame(), after this point you can use ImGui::* functions anytime
    // (So you want to try calling NewFrame() as early as you can in your main loop to be able to use Dear ImGui everywhere)
    ImGui::NewFrame();
}

void EndGUIFrame()
{
    ImGui::Render();

    ImDrawData *drawData = ImGui::GetDrawData();

    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    int fb_width = (int)(drawData->DisplaySize.x * drawData->FramebufferScale.x);
    int fb_height = (int)(drawData->DisplaySize.y * drawData->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0)
        return;

    bgfx::setViewName(sContext.mViewId, "ImGui");
    bgfx::setViewMode(sContext.mViewId, bgfx::ViewMode::Sequential);

    const bgfx::Caps *caps = bgfx::getCaps();
    {
        float ortho[16];
        float x = drawData->DisplayPos.x;
        float y = drawData->DisplayPos.y;
        float width = drawData->DisplaySize.x;
        float height = drawData->DisplaySize.y;

        bx::mtxOrtho(ortho, x, x + width, y + height, y, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
        bgfx::setViewTransform(sContext.mViewId, NULL, ortho);
        bgfx::setViewRect(sContext.mViewId, 0, 0, uint16_t(width), uint16_t(height));
    }
    const ImVec2 clipPos = drawData->DisplayPos;         // (0,0) unless using multi-viewports
    const ImVec2 clipScale = drawData->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

    // Render command lists
    for (int32_t ii = 0, num = drawData->CmdListsCount; ii < num; ++ii)
    {
        bgfx::TransientVertexBuffer tvb;
        bgfx::TransientIndexBuffer tib;

        const ImDrawList *drawList = drawData->CmdLists[ii];
        uint32_t numVertices = (uint32_t)drawList->VtxBuffer.size();
        uint32_t numIndices = (uint32_t)drawList->IdxBuffer.size();

        // if (!checkAvailTransientBuffers(numVertices, m_layout, numIndices))
        // {
        //     // not enough space in transient buffer just quit drawing the rest...
        //     break;
        // }

        bgfx::allocTransientVertexBuffer(&tvb, numVertices, sContext.mLayout);
        bgfx::allocTransientIndexBuffer(&tib, numIndices, sizeof(ImDrawIdx) == 4);

        ImDrawVert *verts = (ImDrawVert *)tvb.data;
        bx::memCopy(verts, drawList->VtxBuffer.begin(), numVertices * sizeof(ImDrawVert));

        ImDrawIdx *indices = (ImDrawIdx *)tib.data;
        bx::memCopy(indices, drawList->IdxBuffer.begin(), numIndices * sizeof(ImDrawIdx));

        bgfx::Encoder *encoder = bgfx::begin();

        for (const ImDrawCmd *cmd = drawList->CmdBuffer.begin(), *cmdEnd = drawList->CmdBuffer.end(); cmd != cmdEnd; ++cmd)
        {
            if (cmd->UserCallback)
            {
                cmd->UserCallback(drawList, cmd);
            }
            else if (0 != cmd->ElemCount)
            {
                uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA;

                bgfx::TextureHandle th = sContext.mTexture;
                bgfx::ProgramHandle program = sContext.mProgram;

                if (NULL != cmd->TextureId)
                {
                    union
                    {
                        ImTextureID ptr;
                        struct
                        {
                            bgfx::TextureHandle handle;
                            uint8_t flags;
                            uint8_t mip;
                        } s;
                    } texture = {cmd->TextureId};
                    state |= 0 != (IMGUI_FLAGS_ALPHA_BLEND & texture.s.flags)
                                 ? BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
                                 : BGFX_STATE_NONE;
                    th = texture.s.handle;
                    if (0 != texture.s.mip)
                    {
                        const float lodEnabled[4] = {float(texture.s.mip), 1.0f, 0.0f, 0.0f};
                        bgfx::setUniform(sContext.mUimageLodEnabled, lodEnabled);
                        program = sContext.mProgram;
                    }
                }
                else
                {
                    state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
                }

                // Project scissor/clipping rectangles into framebuffer space
                ImVec4 clipRect;
                clipRect.x = (cmd->ClipRect.x - clipPos.x) * clipScale.x;
                clipRect.y = (cmd->ClipRect.y - clipPos.y) * clipScale.y;
                clipRect.z = (cmd->ClipRect.z - clipPos.x) * clipScale.x;
                clipRect.w = (cmd->ClipRect.w - clipPos.y) * clipScale.y;

                if (clipRect.x < fb_width && clipRect.y < fb_height && clipRect.z >= 0.0f && clipRect.w >= 0.0f)
                {
                    const uint16_t xx = uint16_t(bx::max(clipRect.x, 0.0f));
                    const uint16_t yy = uint16_t(bx::max(clipRect.y, 0.0f));
                    encoder->setScissor(xx, yy, uint16_t(bx::min(clipRect.z, 65535.0f) - xx), uint16_t(bx::min(clipRect.w, 65535.0f) - yy));

                    encoder->setState(state);
                    encoder->setTexture(0, sContext.mSTex, th);
                    encoder->setVertexBuffer(0, &tvb, cmd->VtxOffset, numVertices);
                    encoder->setIndexBuffer(&tib, cmd->IdxOffset, cmd->ElemCount);
                    encoder->submit(sContext.mViewId, program);
                }
            }
        }

        bgfx::end(encoder);
    }
}

void ShutdownGUI()
{
    ImGui::ShutdownDockContext();
    ImGui::DestroyContext(sContext.mImgui);
}

namespace ImGui
{
    void PushFont(Font::Enum _font)
    {
        PushFont(sContext.mFont[_font]);
    }

    void PushEnabled(bool _enabled)
    {
        extern void PushItemFlag(int option, bool enabled);
        PushItemFlag(ImGuiItemFlags_Disabled, !_enabled);
        PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * (_enabled ? 1.0f : 0.5f));
    }

    void PopEnabled()
    {
        extern void PopItemFlag();
        PopItemFlag();
        PopStyleVar();
    }

} // namespace ImGui

BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4505);                     // error C4505: '' : unreferenced local function has been removed
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wunused-function"); // warning: 'int rect_width_compare(const void*, const void*)' defined but not used
BX_PRAGMA_DIAGNOSTIC_PUSH();
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG("-Wunknown-pragmas")
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wtype-limits"); // warning: comparison is always true due to limited range of data type
#define STBTT_malloc(_size, _userData) memAlloc(_size, _userData)
#define STBTT_free(_ptr, _userData) memFree(_ptr, _userData)
#define STB_RECT_PACK_IMPLEMENTATION
#include <stb/stb_rect_pack.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>
BX_PRAGMA_DIAGNOSTIC_POP();
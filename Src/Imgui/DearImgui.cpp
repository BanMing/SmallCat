#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>
#include <bx/allocator.h>
#include <dear-imgui/imgui.h>
#include <dear-imgui/imgui_internal.h>

#include "DearImgui.h"

// #include "vs_ocornut_imgui.bin.h"
// #include "fs_ocornut_imgui.bin.h"
// #include "vs_imgui_image.bin.h"
// #include "fs_imgui_image.bin.h"

// static const bgfx::EmbeddedShader s_embeddedShaders[] =
//     {
//         BGFX_EMBEDDED_SHADER(vs_ocornut_imgui),
//         BGFX_EMBEDDED_SHADER(fs_ocornut_imgui),
//         BGFX_EMBEDDED_SHADER(vs_imgui_image),
//         BGFX_EMBEDDED_SHADER(fs_imgui_image),

//         BGFX_EMBEDDED_SHADER_END()};
bx::AllocatorI *m_allocator;

void CreateGUI()
{
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();

    ImGui::StyleColorsLight();

    // bgfx::RendererType::Enum type = bgfx::getRendererType();
    // bgfx::ProgramHandle m_program = bgfx::createProgram(bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_ocornut_imgui"),
    //                                                     bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_ocornut_imgui"),
    //                                                     true);

    // bgfx::UniformHandle u_imageLodEnabled = bgfx::createUniform("u_imageLodEnabled", bgfx::UniformType::Vec4);
    // bgfx::ProgramHandle m_imageProgram = bgfx::createProgram(bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_imgui_image"),
    //                                                          bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_imgui_image"), true);
    // bgfx::VertexLayout m_layout;
    // m_layout
    //     .begin()
    //     .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
    //     .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
    //     .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
    //     .end();
    // bgfx::UniformHandle s_tex = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);

    ImGui::InitDockContext();
}

void BeginGUIFrame(const char *name) {}

void EndGUIFrame() {}

void ShutdownGUI()
{
    ImGui::ShutdownDockContext();
    ImGui::DestroyContext();
}

namespace ImGui
{
    void PushFont(Font::Enum _font)
    {
        // PushFont(s_ctx.m_font[_font]);
    }

    void PushEnabled(bool _enabled)
    {
        extern void PushItemFlag(int option, bool enabled);
        // PushItemFlag(ImGuiItemFlags_Disabled, !_enabled);
        PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * (_enabled ? 1.0f : 0.5f));
    }

    void PopEnabled()
    {
        extern void PopItemFlag();
        PopItemFlag();
        PopStyleVar();
    }

} // namespace ImGui

static void *memAlloc(size_t _size, void *_userData)
{
    BX_UNUSED(_userData);
    return BX_ALLOC(m_allocator, _size);
}

static void memFree(void *_ptr, void *_userData)
{
    BX_UNUSED(_userData);
    BX_FREE(m_allocator, _ptr);
}
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
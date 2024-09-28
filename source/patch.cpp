#include "stdafx.h"

float AspectRatio43 = 4.0f / 3.0f;
float AspectRatio34 = 3.0f / 4.0f;
struct Screen
{
    int Width = 800;
    int Height = 600;
    float AspectRatioX = 4.0f / 3.0f;
    float AspectRatioY = 3.0f / 4.0f;
} Screen;
struct Frustum
{
    float Left;
    float Right;
    float Bottom;
    float Top;
};
struct Frustum Frustum = { -0.64f, 0.64f, -0.48f, 0.48f };
// struct Frustum ShrinkHUD = { -0.7f, 0.7f, -0.525f, 0.525f };
struct MovieRect
{
    int vX = 0;
    int vY = 0;
    int vW = 640;
    int vH = 480;
} MovieRect;
bool AspectRatioFix;
// uint8_t FrameInterval;

bool Borderless;

std::string GameSpyHost;
struct GameSpy
{
    char AvailableCheck[127] = "%s.available."; // Master - Available Check
    char ServerBrowser[127] = "%s.ms%d.";       // Master - Server Browser
    char QueryReport[127] = "%s.master.";       // Master - Query Report 2
    char ConnectionManager[127] = "gpcm.";      // Presence - Connection Manager
    char SearchPlayer[127] = "gpsp.";           // Presence - Search Player
    char GameStats[127] = "gamestats.";         // GameStats
    char PeerChat[127] = "peerchat.";           // Chat
} GS;

// float FE_Textbox_Back_Scale = 900.0f;
float EFMV_Border_Width = 1200.0f;

DWORD* WormsFortsApp__c_pTheInstance = (DWORD*)0x73156C;
DWORD AspectRatioCodeCaveExit = 0x55CA98;
DWORD FrustumCodeCaveExit = 0x55CB7B;
DWORD CameraLogicEntityCodeCaveExit = 0x59C7C9;
DWORD SetFromSceneCameraCodeCaveExit = 0x5E90D7;
DWORD MoviePlayerPCCodeCaveExit = 0x5672B7;

void __declspec(naked) AspectRatioCodeCave()
{
    __asm {
        mov     ecx, dword ptr ds : [0x73156C]
        mov     ecx, [ecx + 0x28]
        mov     Screen.Width, ecx
        mov     ecx, dword ptr ds : [0x73156C]
        mov     ecx, [ecx + 0x2C]
        mov     Screen.Height, ecx
        fild    dword ptr ds : [Screen.Height]
        fidiv   dword ptr ds : [Screen.Width]
        fld     dword ptr ds : [AspectRatio34]
        fcomip  st(0), st(1)
        fstp    st(0)
        jne     AspectRatioX
        jmp     AspectRatioCodeCaveExit

    AspectRatioX :
        // arX = W / H
        // fL = arX * fB
        // fR = arX * fT
        jb      AspectRatioY
        fild    dword ptr ds : [Screen.Width]
        fidiv   dword ptr ds : [Screen.Height]
        fst     dword ptr ds : [Screen.AspectRatioX]
        fld     st(0)
        // fld     st(0)
        fmul    dword ptr ds : [Frustum.Bottom]
        fstp    dword ptr ds : [Frustum.Left]
        fmul    dword ptr ds : [Frustum.Top]
        fstp    dword ptr ds : [Frustum.Right]
        // fmul    dword ptr ds : [ShrinkHUD.Top]
        // fstp    dword ptr ds : [ShrinkHUD.Right]
        jmp     AspectRatioCodeCaveExit

    AspectRatioY :
        // arY = H / W
        // fB = arY * fL
        // fT = arY * fR
        fild    dword ptr ds : [Screen.Height]
        fidiv   dword ptr ds : [Screen.Width]
        fst     dword ptr ds : [Screen.AspectRatioY]
        fld     st(0)
        // fld     st(0)
        fmul    dword ptr ds : [Frustum.Left]
        fstp    dword ptr ds : [Frustum.Bottom]
        fmul    dword ptr ds : [Frustum.Right]
        fstp    dword ptr ds : [Frustum.Top]
        // fmul    dword ptr ds : [ShrinkHUD.Right]
        // fstp    dword ptr ds : [ShrinkHUD.Top]
        jmp     AspectRatioCodeCaveExit
    }
}

void __declspec(naked) FrustumCodeCave()
{
    __asm {
        fld     dword ptr ds : [Frustum.Left]
        fstp    dword ptr ds : [esp + 0x28]
        fld     dword ptr ds : [Frustum.Right]
        fstp    dword ptr ds : [esp + 0x2C]
        fld     dword ptr ds : [Frustum.Bottom]
        fstp    dword ptr ds : [esp + 0x30]
        fld     dword ptr ds : [Frustum.Top]
        fstp    dword ptr ds : [esp + 0x34]
        jmp     FrustumCodeCaveExit
    }
}

void __declspec(naked) CameraLogicEntityCodeCave()
{
    __asm {
        fchs
        fst     dword ptr ss : [esp + 0x40] // Frustum.Left
        fchs
        fst     dword ptr ss : [esp + 0x44] // Frustum.Rigth
        fmul    dword ptr ds : [Screen.AspectRatioY]
        fchs
        fst     dword ptr ss : [esp + 0x48] // Frustum.Bottom
        fchs
        fstp    dword ptr ss : [esp + 0x4C] // Frustum.Top

        fld     dword ptr ds : [Screen.AspectRatioX]
        fld     dword ptr ds : [AspectRatio43]
        fcomip  st(0), st(1)
        fstp    st(0)
        jne     AspectRatioX
        jmp     CameraLogicEntityCodeCaveExit

    AspectRatioX :
        fld     dword ptr ss : [esp + 0x48]
        fmul    dword ptr ds : [Screen.AspectRatioX]
        fst     dword ptr ss : [esp + 0x40] // Frustum.Left
        fchs
        fstp    dword ptr ss : [esp + 0x44] // Frustum.Rigth
        jmp     CameraLogicEntityCodeCaveExit
    }
}

void __declspec(naked) SetFromSceneCameraCodeCave()
{
    __asm {
        fld     dword ptr ds : [eax + 0x30]
        fmul    dword ptr ds : [Screen.AspectRatioX]
        fmul    st(0), st(1)
        fstp    dword ptr ds : [ebp + 0xC]
        fld     dword ptr ds : [eax + 0x2C]
        fmul    dword ptr ds : [Screen.AspectRatioY]
        push    edi
        jmp     SetFromSceneCameraCodeCaveExit
    }
}

void __declspec(naked) MoviePlayerPCCodeCave()
{
    __asm {
        mov     edx, dword ptr ss : [esp + 0x18] // Right
        mov     dword ptr ds : [MovieRect.vW] , edx
        mov     edx, dword ptr ss : [esp + 0x1C] // Bottom
        mov     dword ptr ds : [MovieRect.vH] , edx
        fild    dword ptr ds : [MovieRect.vH]
        fidiv   dword ptr ds : [MovieRect.vW]
        fld     dword ptr ds : [AspectRatio34]
        fcomip  st(0), st(1)
        fstp    st(0)
        jne     AspectRatioX
        jmp     MoviePlayerEnd

    AspectRatioX :
        // vW = B * (4 / 3)     vH = B
        // vX = (R - vW) / 2    vY = 0
        jb      AspectRatioY
        fild    dword ptr ss : [esp + 0x18] // Width
        fild    dword ptr ss : [esp + 0x1C] // Height
        fld     dword ptr ds : [AspectRatio43]
        fmul    st(0), st(1)
        fistp   dword ptr ds : [MovieRect.vW]
        fistp   dword ptr ds : [MovieRect.vH]
        fisub   dword ptr ds : [MovieRect.vW]
        fld1
        fld1
        fadd
        fdivp   st(1), st(0)
        fistp   dword ptr ds : [MovieRect.vX]
        jmp     MoviePlayerEnd

    AspectRatioY :
        // vW = R    vH = R * (3 / 4)
        // vX = 0    vY = (B - vH) / 2
        fild    dword ptr ss : [esp + 0x1C] // Height
        fild    dword ptr ss : [esp + 0x18] // Width
        fld     dword ptr ds : [AspectRatio34]
        fmul    st(0), st(1)
        fistp   dword ptr ds : [MovieRect.vH]
        fistp   dword ptr ds : [MovieRect.vW]
        fisub   dword ptr ds : [MovieRect.vH]
        fld1
        fld1
        fadd
        fdivp   st(1), st(0)
        fistp   dword ptr ds : [MovieRect.vY]
        jmp     MoviePlayerEnd

    MoviePlayerEnd :
        mov     edx, dword ptr ds : [MovieRect.vH]
        push    edx
        mov     edx, dword ptr ds : [MovieRect.vW]
        push    edx
        mov     edx, dword ptr ds : [MovieRect.vY]
        push    edx
        mov     edx, dword ptr ds : [MovieRect.vX]
        push    edx
        jmp     MoviePlayerPCCodeCaveExit
    }
}

void Init()
{
    CIniReader iniReader("");

    AspectRatioFix = iniReader.ReadInteger("Main", "AspectRatioFix", 0) == 1;
    // FrameInterval = iniReader.ReadInteger("Main", "FrameInterval", 16);
    Borderless = iniReader.ReadInteger("Window", "Borderless", 0) == 1;
    GameSpyHost = iniReader.ReadString("GameSpy", "Host", "gamespy.com");

    if (AspectRatioFix)
    {
        // WormsFortsApp::Initialize
        injector::MakeJMP(0x55CA8F, AspectRatioCodeCave);
        injector::MakeJMP(0x55CB5B, FrustumCodeCave);
        injector::WriteMemory<FLOAT*>(0x55CCAC, &Frustum.Right, true); // HUD
        injector::WriteMemory<FLOAT*>(0x55CCCE, &Frustum.Top, true);   // HUD
        // injector::WriteMemory<FLOAT*>(0x55CCBC, &ShrinkHUD.Right, true);
        // injector::WriteMemory<FLOAT*>(0x55CCDA, &ShrinkHUD.Top, true);

        // zCameraLogicEntity
        injector::MakeJMP(0x59C7A7, CameraLogicEntityCodeCave); // In Game

        // XCamera::SetFromSceneCamera
        injector::MakeJMP(0x5E90CB, SetFromSceneCameraCodeCave); // Menu Background

        // PopUpEntity::Initialize
        // injector::WriteMemory<FLOAT>(0x4CFCFC, FE_Textbox_Back_Scale, true);
        // injector::WriteMemory<FLOAT>(0x4CFD04, FE_Textbox_Back_Scale, true);

        // EfmvBorderEntity::EfmvBorderEntity
        injector::WriteMemory<FLOAT*>(0x4616BD, &EFMV_Border_Width, true);

        // MoviePlayerPC::Open
        injector::MakeJMP(0x5672A7, MoviePlayerPCCodeCave);
    }

    if (Borderless)
    {
        // XOpenGLRenderManager::OpenDisplay
        injector::WriteMemory<BYTE>(0x66A0AA, 0, true); // Pos Y
        injector::WriteMemory<BYTE>(0x66A0AC, 0, true); // Pos X

        // XOglWindow
        injector::WriteMemory<DWORD>(0x69003B, 0x96030000, true); // dwStyle
        injector::WriteMemory<DWORD>(0x690045, 0x96030000, true); // dwStyle
        injector::MakeNOP(0x69003A, 10);
        injector::MakeNOP(0x690049, 6);
        injector::WriteMemory<BYTE>(0x690049, 0x5D, true); // pop ebp
    }

    if (GameSpyHost != "gamespy.com")
    {
        strcat_s(GS.AvailableCheck, GameSpyHost.c_str());
        strcat_s(GS.ServerBrowser, GameSpyHost.c_str());
        strcat_s(GS.QueryReport, GameSpyHost.c_str());
        strcat_s(GS.ConnectionManager, GameSpyHost.c_str());
        strcat_s(GS.SearchPlayer, GameSpyHost.c_str());
        strcat_s(GS.GameStats, GameSpyHost.c_str());
        strcat_s(GS.PeerChat, GameSpyHost.c_str());

        injector::WriteMemory(0x507169, &GS.AvailableCheck, true);
        injector::WriteMemory(0x51D3C7, &GS.ServerBrowser, true);
        injector::WriteMemory(0x510259, &GS.QueryReport, true);
        // injector::WriteMemory(0x0, &GS.ConnectionManager, true);
        injector::WriteMemory(0x52040E, &GS.SearchPlayer, true);
        injector::WriteMemory(0x511CC2, &GS.GameStats, true);
        injector::WriteMemory(0x511D04, &GS.GameStats, true);
        injector::WriteMemory(0x511D42, &GS.GameStats, true);
        injector::WriteMemory(0x50C59C, &GS.PeerChat, true);
        injector::WriteMemory(0x50C5FB, &GS.PeerChat, true);
        injector::WriteMemory(0x50C64D, &GS.PeerChat, true);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        if (strcmp((char*)0x6B55B8, "WormsForts") == 0)
        {
            Init();
        }
    }
    return TRUE;
}

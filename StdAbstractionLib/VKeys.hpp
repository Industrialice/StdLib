#ifndef __VKEYS_HPP__
#define __VKEYS_HPP__

#include <CVector.hpp>

namespace StdLib {

namespace VKeys
{
    CONSTS( vkey_t,
        MButton0, MButton1, MButton2, MButton3, MButton4,
        LShift, RShift, Shift,
        LControl, RControl, Control,
        LAlt, RAlt, Alt,
        Enter,
        Space,
        Escape,
        Tab,
        PageDown, PageUp, Home, End,
        Insert,
        Delete,
        CapsLock, ScrollLock, NumLock,
        Pause,
        PrintScreen,
        Tilda,
        Backspace,
        Up, Down, Left, Right,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
        NPad0, NPad1, NPad2, NPad3, NPad4, NPad5, NPad6, NPad7, NPad8, NPad9,
        NPadAdd, NPadSub, NPadMul, NPadDiv,
        OEM1,         //  ';:' for US
        OEM2,         //  '/?' for US
        OEM3,         //  '`~' for US
        OEM4,         //  '[{' for US
        OEM5,         //  '\|' for US
        OEM6,         //  ']}' for US
        OEM7,         //  ''"' for US
        OEMAdd,       //  '+' any country
        OEMComma,     //  ',' any country
        OEMSub,       //  '-' any country
        OEMDot,       //  '.' any country
        Undefined,
        /*  A-Z = 'A'-'Z', 0-9 = '0'-'9'  */
		_size
	);

	CRefVec < const ui8 > GetPlatformMappingStruct();
} }

#endif __VKEYS_HPP__
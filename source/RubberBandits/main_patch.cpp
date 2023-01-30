#include "skyline/inlinehook/And64InlineHook.hpp"
#include "skyline/utils/cpputils.hpp"
#include "skyline/inlinehook/memcpy_controlled.hpp"
#define Unicode std::char_traits<char16_t>

uintptr_t TextRegionOffset = 0;

ptrdiff_t returnInstructionOffset(uintptr_t LR) {
	return LR - TextRegionOffset;
}

struct System_String {
	void* _class;
	void* _monitor;
	uint32_t string_size;
	char16_t lang_string[12];
	char reserved[2];
};

struct LocalizationISO {
	System_String* isoCode;
};

namespace Flashbulb_Platform_Device_Switch_SwitchDevice {
	namespace GetLocalizationISO {

		LocalizationISO* (*original)(void* _this, void* isoCode, void* MethodInfo);
		LocalizationISO* hook(void* _this, void* isoCode, void* MethodInfo) {
			LocalizationISO* ret_data = original(_this, isoCode, MethodInfo);
			const char16_t* language = u"pl-PL";
			
			ret_data -> isoCode -> string_size = Unicode::length(language);
			Unicode::copy(ret_data -> isoCode -> lang_string, language, Unicode::length(language) + 1);
			return ret_data;
		}

	}
}

void RB_main()
{
	TextRegionOffset = (uintptr_t)skyline::utils::getRegionAddress(skyline::utils::region::Text);

	//Offset recovered with il2cppdumper, named as Flashbulb.Platform.Device.Switch.SwitchDevice$$GetLocalizationISO
	A64HookFunction((void*)(TextRegionOffset + 0x4F58280), reinterpret_cast<void*>(Flashbulb_Platform_Device_Switch_SwitchDevice::GetLocalizationISO::hook), (void**)&Flashbulb_Platform_Device_Switch_SwitchDevice::GetLocalizationISO::original);

}

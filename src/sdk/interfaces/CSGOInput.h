#pragma once

#include <Windows.h>
#include <stdint.h>
#include "../math/Vector.h"

#define MULTIPLAYER_BACKUP 150

enum ECommandButtons {
	IN_NONE = 0x0,
	IN_ALL = 0xffffffffffffffff,
	IN_ATTACK = 0x1,
	IN_JUMP = 0x2,
	IN_DUCK = 0x4,
	IN_FORWARD = 0x8,
	IN_BACK = 0x10,
	IN_USE = 0x20,
	IN_TURNLEFT = 0x80,
	IN_TURNRIGHT = 0x100,
	IN_MOVELEFT = 0x200,
	IN_MOVERIGHT = 0x400,
	IN_ATTACK2 = 0x800,
	IN_RELOAD = 0x2000,
	IN_SPEED = 0x10000,
	IN_JOYAUTOSPRINT = 0x20000,
	IN_FIRST_MOD_SPECIFIC_BIT = 0x100000000,
	IN_USEORRELOAD = 0x100000000,
	IN_SCORE = 0x200000000,
	IN_ZOOM = 0x400000000,
	IN_JUMP_THROW_RELEASE = 0x800000000,
};

class CMsgQAngle
{
	void* __vfptr; //0x0000
	char pad_0008[16]; //0x0008
public:
	float pitch; //0x0018
	float yaw; //0x001C
	float roll; //0x0020
private:
	char pad_0024[28]; //0x0024
}; //Size: 0x0040


class CInButtonStatePB
{
	void* __vfptr; //0x0000
	char pad_0008[16]; //0x0008
public:
	uint64_t buttonstate1; //0x0018
	uint64_t buttonstate2; //0x0020
	uint64_t buttonstate3; //0x0028
}; //Size: 0x0030

class CSGOInputHistory
{
public:
	void* __vfptr; //0x0000
	char pad_0008[16]; //0x0008
	CMsgQAngle* viewangles; //0x0018
	char pad_0020[32]; //0x0020
	void* cl_interp; //0x0040
	void* sv_interp0; //0x0048
	void* sv_interp1; //0x0050
	void* player_interp; //0x0058
	int render_tick_count; //0x0060
	float render_tick_fraction; //0x0064
	int player_tick_count; //0x0068
	float player_tick_fraction; //0x006C
	char pad_0070[8]; //0x0070
}; //Size: 0x0078

class CSubtickMoveStep {
	void* __vfptr;
	char pad_0008[16]; //0x0008
public:
	uint32_t button;
	bool pressed;
	float when;
};

template <class T, int MAX_CAPACITY>
class RepFieldContainer {
	int count_;
	int max_;
	struct InnerContainer {
		int count_; //0x0000
		int max_; //0x0004
		T* elements_[MAX_CAPACITY]; //0x0008
	}*elements_;
public:
	inline int size() const {
		return count_;
	}

	inline int max_size() const {
		return max_;
	}

	inline T* operator[](int i) {
		if (i >= max_size())
			return nullptr;

		return elements_->elements_[i];
	}

	inline operator bool() {
		return elements_ != nullptr;
	}
}; //Size: 0x0028

class CBaseUserCmd
{
	void* __vfptr; //0x0000
	char pad_0008[16]; //0x0008
	uint64_t N000000CB; //0x0018

public:
	RepFieldContainer<CSubtickMoveStep, 12> subticks; //0x0028
	int8_t* move_crc; //0x0030
	CInButtonStatePB* buttons_pb; //0x0038
	CMsgQAngle* viewangles; //0x0040
	int command_number; //0x0048
	int tickcount; //0x004C
	float forwardmove; //0x0050 from -1 to 1
	float leftmove; //0x0054 from -1 to 1
	float upmove; //0x0058 from -1 to 1
	int impulse; //0x005C
	int weapon_select; //0x0060
	int random_seed; //0x0064
	int mousedx; //0x0068
	int mousedy; //0x006C
	bool hasbeenpredicted; //0x0070
	uint32_t consumed_server_angle_changes; //0x0071
	int cmd_flags; //0x0075
	uint32_t pawn_entity_handle; //0x0079
	char pad_007D[3]; //0x007D
}; //Size: 0x0078



class CUserCmd
{
public:
	void* __vfptr; //0x0000
	char pad_0008[8]; //0x0008
	uint32_t cm_flag; //0x0010 some creamove flag, always 1
	char pad_0014[12]; //0x0014
	RepFieldContainer<CSGOInputHistory, 4> input_history; //0x0028
	CBaseUserCmd* base_cmd; //0x0030
	char pad_0038[24]; //0x0038
	void* button_state; //0x0050
	uint32_t buttons; //0x0058
	char pad_005C[68]; //0x005C
}; //Size: 0x00A0


class CSGOInput {
	void** __vfptr;
	char pad_0008[24584]; //0x0008
public:

	bool bBlockShoot; //0x6010
	bool bInThirdperson; //0x6011
	char pad_6012[2]; //0x6012
	bool bOrthographicCamera; //0x6014
	char pad_6015[3]; //0x6015
	Vector vecThirdpersonAngles; //0x6018
	char pad_6024[16]; //0x6024
	int nSequenceNumber; //0x6034
	char pad_6038[16]; //0x6038
	uint32_t nButtons; //0x6048
	char pad_604C[20]; //0x604C
	uint32_t nButtons2; //0x6060
	char pad_6064[4]; //0x6064
	float forwardmove; //0x6068
	float leftmove; //0x606C
	float upmove; //0x6070
	int mousedx; //0x6074
	int mousedy; //0x6078
	int subtick_movesteps_count; //0x607C
	uint32_t nLastButtonPressed; //0x6080
	char pad_6084[188]; //0x6084
	Vector vecViewAngles; //0x6140
	char pad_614C[200]; //0x614C
	int N00000BF8; //0x6214
	bool N00001078; //0x6218
	char pad_6219[30]; //0x6219
	bool bMouseInputEnabled; //0x6237
	bool bHoldingLeftButton; //0x6238
	bool bHoldingRightButton; //0x6239
	char pad_623A[86]; //0x623A

	CUserCmd* GetUserCmd(int seq = -1)
	{
		if (seq == -1)
			seq = nSequenceNumber;

		CUserCmd* cmd_arr = reinterpret_cast<CUserCmd*>(reinterpret_cast<uintptr_t>(this) + 0x250);

		return &cmd_arr[seq % MULTIPLAYER_BACKUP];
	}

	static CSGOInput* get()
	{
		static auto addr = modules::client.pattern_scanner.scan("E8 ? ? ? ? 48 8B 56 60").add(0x1).abs().as<void*>();

		const auto& get_input = reinterpret_cast<CSGOInput * (__thiscall*)()>(addr);

		if (get_input)
			return get_input();

		return nullptr;
	}
};
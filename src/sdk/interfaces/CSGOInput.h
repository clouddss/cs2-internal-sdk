#pragma once

#include <Windows.h>
#include <stdint.h>
#include "../math/Vector.h"
#include "../helpers/modules.h"

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

template <typename T>
struct repeated_ptr_field_t
{
	struct repeat_t
	{
		int m_alloc_size;
		T* m_elements[(INT_MAX - 2 * sizeof(int)) / sizeof(void*)];
	};

	void* m_arena;
	int m_current_size;
	int m_total_size;
	repeat_t* m_repeat;
};

class c_msg_qangle
{
public:
	std::uint8_t pad_1[ 24 ]; //0x0000
	Vector m_view_angles; // 0x18
};
static_assert( sizeof( c_msg_qangle ) == 0x24 );

class c_csgo_interpolation_info
{
public:
	char pad_0000[ 24 ]; //0x0000
	float m_fraction; // 0x18
	int m_src_tick; // 0x1C
	int m_dst_tick; // 0x20
	char pad_0024[ 12 ]; //0x0024
	c_msg_qangle entiy_view_angles; //0x0030
};
static_assert( sizeof( c_csgo_interpolation_info ) == 0x54 );

class c_csgo_input_history_entry_pb
{
public:
	std::uint8_t padding_1[0x18]{ };
	c_msg_qangle* m_view_cmd; // 0x18
	char pad_0020[ 32 ]; //0x0020
	c_csgo_interpolation_info* cl_interp; // 0x40
	c_csgo_interpolation_info* sv_interp0; // 0x48
	c_csgo_interpolation_info* sv_interp1; // 0x50	
	c_csgo_interpolation_info* m_player_interp; // 0x58
	int m_render_tick_count; // 0x60
	float m_render_tick_fraction; // 0x64
	int m_player_tick_count; // 0x68
	float m_player_tick_fraction; // 0x6C
	int m_frame_number; // 0x70
	int m_target_ent_index; // 0x74
	int32_t idk; //0x0078
	int32_t idk1; //0x007C
};
static_assert( sizeof( c_csgo_input_history_entry_pb ) == 0x80 );

struct c_subtick_move_step
{
	std::uint8_t padding_1[0x18]{ };
	uint64_t m_button;
	bool m_pressed;
	float m_when;
};

struct c_in_button_state_pb
{
	char pad_0000[ 24 ]; //0x0000
	std::uint64_t m_hold;
	std::uint64_t m_pressed;
	std::uint64_t m_scrolled;
};
static_assert( sizeof( c_in_button_state_pb ) == 0x30 );

class c_valve_proto
{
public:
	void* __vfptr;
	uint64_t m_has_bits;
	uint64_t m_cached_bits;
};

class c_base_usercmd : c_valve_proto
{
public:
	repeated_ptr_field_t<c_subtick_move_step> m_subtick_moves_field;
	const char* m_move_crc;
	c_in_button_state_pb* m_in_button_state;
	c_msg_qangle* m_view;
	int32_t m_command_number;
	int32_t m_tick_count;
	float m_forward_move;
	float m_side_move;
	float m_up_move;
	int32_t m_impulse;
	int32_t m_weapon_select;
	int32_t m_random_seed;
	int32_t m_moused_x;
	int32_t m_moused_y;
	uint32_t m_consumed_server_angle_changes;
	int32_t m_cmd_flags;
	uint32_t m_pawn_entity_handle;
};

struct c_in_button_state
{
	char pad_0000[ 8 ]; //0x0000
	std::uint64_t m_hold;
	std::uint64_t m_pressed;
	std::uint64_t m_scrolled;
};

class c_sub_tick_container
{
public:
	std::int32_t tick_count;
	char pad_0004[ 4 ]; //0x0004
	std::uintptr_t tick_pointer;

	c_csgo_input_history_entry_pb* get_tick(std::int32_t i)
	{
		if (i < this->tick_count)
		{
			c_csgo_input_history_entry_pb** tick_list = reinterpret_cast<c_csgo_input_history_entry_pb**>(this->tick_pointer + 0x8);
			return tick_list[i];
		}

		return nullptr;
	}
};

class c_usercmd
{
public:
	uint32_t m_has_bits;
	uint64_t m_cached_size;
	repeated_ptr_field_t<c_csgo_input_history_entry_pb> m_input_history_field;
	c_base_usercmd* m_base_cmd;
	int32_t m_weapon_decision;
	int32_t m_weapon_decision_weapon;
	int32_t m_attack3_start_history_index;
	int32_t m_attack1_start_history_index;
	int32_t m_attack2_start_history_index;
	c_in_button_state m_button_state;
	char pad_0068[ 8 ]; //0x0068
	double some_time; //0x0070
	char pad_0078[ 16 ]; //0x0078

	c_sub_tick_container get_sub_tick_container()
	{
		return *reinterpret_cast<c_sub_tick_container*>(reinterpret_cast<std::uintptr_t>(this) + 0x20);
	}

	void set_sub_tick_angles(Vector& angles)
	{
		c_sub_tick_container container = this->get_sub_tick_container();
		for (std::int32_t i = 0; i < container.tick_count; i++)
		{
			c_csgo_input_history_entry_pb* tick = container.get_tick(i);

			if (tick && tick->m_view_cmd)
			{
				tick->m_view_cmd->m_view_angles = angles;
			}
		}
	}
};

class c_csgo_input {
public:
	char pad_0000[ 592 ]; //0x0000
	c_usercmd m_commands[150]; //0x3040
	char pad_5200[ 1 ]; //0x5200
	bool m_in_thirdperson; //0x8E01
	char pad_5202[ 34 ]; //0x5202
	int32_t m_seq_number; //0x8E24
	int32_t m_prev_seq_number; //0x8E28
	char pad_522C[ 56 ]; //0x522C
	int32_t m_mousedx; //0x5264
	int32_t m_mousedy; //0x5268
	char pad_526C[ 292 ]; //0x526C
	Vector m_viewangles; //0x5390

	c_usercmd* get_usercmd()
	{
		return &m_commands[m_seq_number % 150];
	}

	c_usercmd* get_prev_cmd()
	{
		return &m_commands[m_prev_seq_number % 150];
	}

	static c_csgo_input* get()
	{
		static auto addr = modules::client.pattern_scanner.scan( "E8 ? ? ? ? 48 8B 56 60" ).add( 0x1 ).abs( ).as<void*>( );

		const auto& get_input = reinterpret_cast<c_csgo_input * (__thiscall*)()>(addr);

		if (get_input)
			return get_input();

		return nullptr;
	}
};

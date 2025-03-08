#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END, DIM_LB_Release};
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum class PROTOTYPE { TYPE_GAMEOBJECT, TYPE_COMPONENT };

	enum EVENT_FRAME_TYPE	
	{
		EVENT_SOUND,	
		EVENT_EFFECT,
		EVENT_COLLIDER,	
		EVENT_STATE,	
		EVENT_END,	
	};


	enum GROUP_TYPE
	{
		PLAYER = 1,				  /* 0x0000001 */
		PLAYER_WEAPON = 1 << 1,   /* 0x0000010 */
		MONSTER = 1 << 2,		  /* 0x0000100 */
		MONSTER_WEAPON = 1 << 3,  /* 0x0001000 */
		ENEMY = 1 << 4,		  /* 0x0010000 */
		OBJECT = 1 << 5,		  /* 0x0100000 */
		GROUP_END = 1 << 6,		  /* 0x1000000 */
	};

	enum class EVENT_TYPE{
		EVENT_1,	
		EVENT_2,	
		EVENT_3,	
		EVENT_4,		
		EVENT_5,		
	};

	enum CHANNELID
	{
		SOUND_BGM,
		SOUND_ACTION,
		SOUND_EFFECT,
		SOUND_MOREEFFECT,
		SOUND_SURROUNDING,
		SOUND_NATURAL,
		SOUND_ElectricEel,
		SOUND_ElectricEel_Collision,
		SOUND_RHINO,
		SOUND_STONE,
		MAXCHANNEL = 32,
	};
	enum UIOBJTYPE
	{
		UI_BUTTON,
		UI_BUTTONPLAYER,
		UI_IMAGE,
		UI_TEXT,
		UI_TEXTPLAYER,
		UI_END
	};

	enum class BINARY {
		BINARY_SAVE,
		BINARY_LOAD,
		BINARY_END,
	};
	
	enum class COLLIDER_TYPE	
	{
		COLLIDER_BOX,
		COLLIDER_CAPSULE,
		COLLIDER_SPHERE,
		COLLIDER_Field,
	};

	enum class FRUSTUM_TYPE
	{
		FRUSTUM_OBJECT,
		FRUSTUM_MONSTER,
		FRUSTUM_END,
		
	};


	enum Player_Hitted_State
	{
		PLAYER_HURT_KNOCKDOWN,			// ĳ���� �˴ٿ�
		PLAYER_HURT_HURTMFL,			// ���� �ڷ� �̵��ϸ鼭 ��û 
		PLAYER_HURT_HURTSF,				// ���� ���� �ڷ� �̵��ϸ鼭 ��û 
		PLAYER_HURT_HURTSL,				// ���� ���� �ڷ� �̵��ϸ鼭 ��û 
		PLAYER_HURT_HURTLF,				// ���� ��� �ڷ� �̵��ϸ鼭 ��û �ϸ鼭 ���� ����	
		PLAYER_HURT_HURXXLF,			// ���� ��� �ڷ� �̵��ϸ鼭 ��û �ϸ鼭 ���� �ݴ� �ð� ���� �� 
		PLAYER_HURT_KnockBackF,			// ��� �ڷ� �и��鼭 �Ѽ����� ��¤�� �Ͼ
		PLAYER_HURT_FallDown,			// ���߿� ������鼭 ����
		PLAYER_HURT_END
	};

}
#endif // Engine_Enum_h__

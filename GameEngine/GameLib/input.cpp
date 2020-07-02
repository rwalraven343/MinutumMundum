//Code by Rogier Walraven. Do not distribute.

#include <SDL.h>

#include "input.h"

static SDL_Event sdl_event;

static bool input_keyboard_backspace;
static bool input_keyboard_tab;
static bool input_keyboard_enter;
static bool input_keyboard_escape;
static bool input_keyboard_space;
static bool input_keyboard_exclamation;
static bool input_keyboard_quotation;
static bool input_keyboard_hash;
static bool input_keyboard_dollar;
static bool input_keyboard_percent;
static bool input_keyboard_ampersand;
static bool input_keyboard_apostrophe;
static bool input_keyboard_open_parentheses;
static bool input_keyboard_close_parentheses;
static bool input_keyboard_asterisk;
static bool input_keyboard_plus;
static bool input_keyboard_comma;
static bool input_keyboard_minus;
static bool input_keyboard_period;
static bool input_keyboard_slash;
static bool input_keyboard_zero;
static bool input_keyboard_one;
static bool input_keyboard_two;
static bool input_keyboard_three;
static bool input_keyboard_four;
static bool input_keyboard_five;
static bool input_keyboard_six;
static bool input_keyboard_seven;
static bool input_keyboard_eight;
static bool input_keyboard_nine;
static bool input_keyboard_colon;
static bool input_keyboard_semicolon;
static bool input_keyboard_less_than;
static bool input_keyboard_equals;
static bool input_keyboard_greater_than;
static bool input_keyboard_question;
static bool input_keyboard_at;
static bool input_keyboard_A;
static bool input_keyboard_B;
static bool input_keyboard_C;
static bool input_keyboard_D;
static bool input_keyboard_E;
static bool input_keyboard_F;
static bool input_keyboard_G;
static bool input_keyboard_H;
static bool input_keyboard_I;
static bool input_keyboard_J;
static bool input_keyboard_K;
static bool input_keyboard_L;
static bool input_keyboard_M;
static bool input_keyboard_N;
static bool input_keyboard_O;
static bool input_keyboard_P;
static bool input_keyboard_Q;
static bool input_keyboard_R;
static bool input_keyboard_S;
static bool input_keyboard_T;
static bool input_keyboard_U;
static bool input_keyboard_V;
static bool input_keyboard_W;
static bool input_keyboard_X;
static bool input_keyboard_Y;
static bool input_keyboard_Z;
static bool input_keyboard_open_square_brackets;
static bool input_keyboard_backslash;
static bool input_keyboard_close_square_brackets;
static bool input_keyboard_caret;
static bool input_keyboard_underscore;
static bool input_keyboard_grave_accent;
static bool input_keyboard_a;
static bool input_keyboard_b;
static bool input_keyboard_c;
static bool input_keyboard_d;
static bool input_keyboard_e;
static bool input_keyboard_f;
static bool input_keyboard_g;
static bool input_keyboard_h;
static bool input_keyboard_i;
static bool input_keyboard_j;
static bool input_keyboard_k;
static bool input_keyboard_l;
static bool input_keyboard_m;
static bool input_keyboard_n;
static bool input_keyboard_o;
static bool input_keyboard_p;
static bool input_keyboard_q;
static bool input_keyboard_r;
static bool input_keyboard_s;
static bool input_keyboard_t;
static bool input_keyboard_u;
static bool input_keyboard_v;
static bool input_keyboard_w;
static bool input_keyboard_x;
static bool input_keyboard_y;
static bool input_keyboard_z;
static bool input_keyboard_open_curly_brackets;
static bool input_keyboard_vertical_bar;
static bool input_keyboard_close_curly_brackets;
static bool input_keyboard_tilde;
static bool input_keyboard_del;
static bool input_keyboard_insert;
static bool input_keyboard_home;
static bool input_keyboard_end;
static bool input_keyboard_pageup;
static bool input_keyboard_pagedown;
static bool input_keyboard_left;
static bool input_keyboard_right;
static bool input_keyboard_up;
static bool input_keyboard_down;

static int  input_mouse_x_rel;
static int  input_mouse_y_rel;
static bool input_mouse_left_button;
static bool input_mouse_middle_button;
static bool input_mouse_right_button;
static int  input_mouse_wheel_steps;

static bool          input_gamepad1_available;
static SDL_Joystick *input_gamepad1_device;

static double input_gamepad1_axis_left_x;
static double input_gamepad1_axis_left_y;
static double input_gamepad1_axis_right_x;
static double input_gamepad1_axis_right_y;
static bool   input_gamepad1_hat_left;
static bool   input_gamepad1_hat_right;
static bool   input_gamepad1_hat_up;
static bool   input_gamepad1_hat_down;
static bool   input_gamepad1_start;
static bool   input_gamepad1_exit;
static bool   input_gamepad1_button1;
static bool   input_gamepad1_button2;
static bool   input_gamepad1_button3;
static bool   input_gamepad1_button4;
static bool   input_gamepad1_digital_trigger_left;
static bool   input_gamepad1_digital_trigger_right;

static bool          input_gamepad2_available;
static SDL_Joystick *input_gamepad2_device;

static double input_gamepad2_axis_left_x;
static double input_gamepad2_axis_left_y;
static double input_gamepad2_axis_right_x;
static double input_gamepad2_axis_right_y;
static bool   input_gamepad2_hat_left;
static bool   input_gamepad2_hat_right;
static bool   input_gamepad2_hat_up;
static bool   input_gamepad2_hat_down;
static bool   input_gamepad2_start;
static bool   input_gamepad2_exit;
static bool   input_gamepad2_button1;
static bool   input_gamepad2_button2;
static bool   input_gamepad2_button3;
static bool   input_gamepad2_button4;
static bool   input_gamepad2_digital_trigger_left;
static bool   input_gamepad2_digital_trigger_right;

void INPUT::init()
{
	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_EnableKeyRepeat(0,0);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_JoystickEventState(SDL_ENABLE);

	input_gamepad1_available=false;
	input_gamepad2_available=false;

	for (int i=0;i<SDL_NumJoysticks();i++)
	{
		switch (i)
		{
		case 0:
			input_gamepad1_device=SDL_JoystickOpen(0);
			input_gamepad1_available=input_gamepad1_device!=0;
		break;
		case 1:
			input_gamepad2_device=SDL_JoystickOpen(1);
			input_gamepad2_available=input_gamepad2_device!=0;
		break;
		}
	}

	clear();
}

void INPUT::quit()
{
	if (input_gamepad1_available)
	{
		SDL_JoystickClose(input_gamepad1_device);
	}

	if (input_gamepad2_available)
	{
		SDL_JoystickClose(input_gamepad2_device);
	}
}

void INPUT::clear()
{
	input_keyboard_backspace            =false;
	input_keyboard_tab                  =false;
	input_keyboard_enter                =false;
	input_keyboard_escape               =false;
	input_keyboard_space                =false;
	input_keyboard_exclamation          =false;
	input_keyboard_quotation            =false;
	input_keyboard_hash                 =false;
	input_keyboard_dollar               =false;
	input_keyboard_percent              =false;
	input_keyboard_ampersand            =false;
	input_keyboard_apostrophe           =false;
	input_keyboard_open_parentheses     =false;
	input_keyboard_close_parentheses    =false;
	input_keyboard_asterisk             =false;
	input_keyboard_plus                 =false;
	input_keyboard_comma                =false;
	input_keyboard_minus                =false;
	input_keyboard_period               =false;
	input_keyboard_slash                =false;
	input_keyboard_zero                 =false;
	input_keyboard_one                  =false;
	input_keyboard_two                  =false;
	input_keyboard_three                =false;
	input_keyboard_four                 =false;
	input_keyboard_five                 =false;
	input_keyboard_six                  =false;
	input_keyboard_seven                =false;
	input_keyboard_eight                =false;
	input_keyboard_nine                 =false;
	input_keyboard_colon                =false;
	input_keyboard_semicolon            =false;
	input_keyboard_less_than            =false;
	input_keyboard_equals               =false;
	input_keyboard_greater_than         =false;
	input_keyboard_question             =false;
	input_keyboard_at                   =false;
	input_keyboard_A                    =false;
	input_keyboard_B                    =false;
	input_keyboard_C                    =false;
	input_keyboard_D                    =false;
	input_keyboard_E                    =false;
	input_keyboard_F                    =false;
	input_keyboard_G                    =false;
	input_keyboard_H                    =false;
	input_keyboard_I                    =false;
	input_keyboard_J                    =false;
	input_keyboard_K                    =false;
	input_keyboard_L                    =false;
	input_keyboard_M                    =false;
	input_keyboard_N                    =false;
	input_keyboard_O                    =false;
	input_keyboard_P                    =false;
	input_keyboard_Q                    =false;
	input_keyboard_R                    =false;
	input_keyboard_S                    =false;
	input_keyboard_T                    =false;
	input_keyboard_U                    =false;
	input_keyboard_V                    =false;
	input_keyboard_W                    =false;
	input_keyboard_X                    =false;
	input_keyboard_Y                    =false;
	input_keyboard_Z                    =false;
	input_keyboard_open_square_brackets =false;
	input_keyboard_backslash            =false;
	input_keyboard_close_square_brackets=false;
	input_keyboard_caret                =false;
	input_keyboard_underscore           =false;
	input_keyboard_grave_accent         =false;
	input_keyboard_a                    =false;
	input_keyboard_b                    =false;
	input_keyboard_c                    =false;
	input_keyboard_d                    =false;
	input_keyboard_e                    =false;
	input_keyboard_f                    =false;
	input_keyboard_g                    =false;
	input_keyboard_h                    =false;
	input_keyboard_i                    =false;
	input_keyboard_j                    =false;
	input_keyboard_k                    =false;
	input_keyboard_l                    =false;
	input_keyboard_m                    =false;
	input_keyboard_n                    =false;
	input_keyboard_o                    =false;
	input_keyboard_p                    =false;
	input_keyboard_q                    =false;
	input_keyboard_r                    =false;
	input_keyboard_s                    =false;
	input_keyboard_t                    =false;
	input_keyboard_u                    =false;
	input_keyboard_v                    =false;
	input_keyboard_w                    =false;
	input_keyboard_x                    =false;
	input_keyboard_y                    =false;
	input_keyboard_z                    =false;
	input_keyboard_open_curly_brackets  =false;
	input_keyboard_vertical_bar         =false;
	input_keyboard_close_curly_brackets =false;
	input_keyboard_tilde                =false;
	input_keyboard_del                  =false;
	input_keyboard_insert               =false;
	input_keyboard_home                 =false;
	input_keyboard_end                  =false;
	input_keyboard_pageup               =false;
	input_keyboard_pagedown             =false;
	input_keyboard_left                 =false;
	input_keyboard_right                =false;
	input_keyboard_up                   =false;
	input_keyboard_down                 =false;

	input_mouse_x_rel        =0;
	input_mouse_y_rel        =0;
	input_mouse_left_button  =false;
	input_mouse_middle_button=false;
	input_mouse_right_button =false;
	input_mouse_wheel_steps  =0;

	input_gamepad1_axis_left_x          =0;
	input_gamepad1_axis_left_y          =0;
	input_gamepad1_axis_right_x         =0;
	input_gamepad1_axis_right_y         =0;
	input_gamepad1_hat_left             =false;
	input_gamepad1_hat_right            =false;
	input_gamepad1_hat_up               =false;
	input_gamepad1_hat_down             =false;
	input_gamepad1_start                =false;
	input_gamepad1_exit                 =false;
	input_gamepad1_button1              =false;
	input_gamepad1_button2              =false;
	input_gamepad1_button3              =false;
	input_gamepad1_button4              =false;
	input_gamepad1_digital_trigger_left =false;
	input_gamepad1_digital_trigger_right=false;

	input_gamepad2_axis_left_x          =0;
	input_gamepad2_axis_left_y          =0;
	input_gamepad2_axis_right_x         =0;
	input_gamepad2_axis_right_y         =0;
	input_gamepad2_hat_left             =false;
	input_gamepad2_hat_right            =false;
	input_gamepad2_hat_up               =false;
	input_gamepad2_hat_down             =false;
	input_gamepad2_start                =false;
	input_gamepad2_exit                 =false;
	input_gamepad2_button1              =false;
	input_gamepad2_button2              =false;
	input_gamepad2_button3              =false;
	input_gamepad2_button4              =false;
	input_gamepad2_digital_trigger_left =false;
	input_gamepad2_digital_trigger_right=false;

	while (SDL_PollEvent(&sdl_event)) {}
}

void INPUT::update()
{
	Uint8 appstate=SDL_GetAppState();
	if (!((appstate&SDL_APPACTIVE)&&(appstate&SDL_APPINPUTFOCUS)&&(appstate&SDL_APPMOUSEFOCUS)))
	{
		SDL_ShowCursor(SDL_ENABLE);
		while (!((appstate&SDL_APPACTIVE)&&(appstate&SDL_APPINPUTFOCUS)&&(appstate&SDL_APPMOUSEFOCUS)))
		{
			SDL_Delay(100);

			SDL_PumpEvents();
			appstate=SDL_GetAppState();
		}
		SDL_ShowCursor(SDL_DISABLE);

		clear();
	}

	input_mouse_x_rel=0;
	input_mouse_y_rel=0;
	input_mouse_wheel_steps=0;

	SDLMod mod;
	while (SDL_PollEvent(&sdl_event))
	{
		switch (sdl_event.type)
		{
		case SDL_KEYDOWN:
			mod=sdl_event.key.keysym.mod;

			switch (sdl_event.key.keysym.sym)
			{
			case SDLK_BACKSPACE:
				input_keyboard_backspace=true;
			break;
			case SDLK_TAB:
				input_keyboard_tab=true;
			break;
			case SDLK_RETURN:
				input_keyboard_enter=true;
			break;
			case SDLK_ESCAPE:
				input_keyboard_escape=true;
			break;
			case SDLK_SPACE:
				input_keyboard_space=true;
			break;
			case SDLK_QUOTE:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_apostrophe=true;
				}
				else
				{
					input_keyboard_quotation=true;
				}
			break;
			case SDLK_COMMA:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_comma=true;
				}
				else
				{
					input_keyboard_less_than=true;
				}
			break;
			case SDLK_MINUS:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_minus=true;
				}
				else
				{
					input_keyboard_underscore=true;
				}
			break;
			case SDLK_PERIOD:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_period=true;
				}
				else
				{
					input_keyboard_greater_than=true;
				}
			break;
			case SDLK_SLASH:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_slash=true;
				}
				else
				{
					input_keyboard_question=true;
				}
			break;
			case SDLK_0:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_zero=true;
				}
				else
				{
					input_keyboard_close_parentheses=true;
				}
			break;
			case SDLK_1:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_one=true;
				}
				else
				{
					input_keyboard_exclamation=true;
				}
			break;
			case SDLK_2:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_two=true;
				}
				else
				{
					input_keyboard_at=true;
				}
			break;
			case SDLK_3:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_three=true;
				}
				else
				{
					input_keyboard_hash=true;
				}
			break;
			case SDLK_4:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_four=true;
				}
				else
				{
					input_keyboard_dollar=true;
				}
			break;
			case SDLK_5:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_five=true;
				}
				else
				{
					input_keyboard_percent=true;
				}
			break;
			case SDLK_6:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_six=true;
				}
				else
				{
					input_keyboard_caret=true;
				}
			break;
			case SDLK_7:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_seven=true;
				}
				else
				{
					input_keyboard_ampersand=true;
				}
			break;
			case SDLK_8:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_eight=true;
				}
				else
				{
					input_keyboard_asterisk=true;
				}
			break;
			case SDLK_9:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_nine=true;
				}
				else
				{
					input_keyboard_open_parentheses=true;
				}
			break;
			case SDLK_SEMICOLON:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_semicolon=true;
				}
				else
				{
					input_keyboard_colon=true;
				}
			break;
			case SDLK_EQUALS:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_equals=true;
				}
				else
				{
					input_keyboard_plus=true;
				}
			break;
			case SDLK_LEFTBRACKET:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_open_square_brackets=true;
				}
				else
				{
					input_keyboard_open_curly_brackets=true;
				}
			break;
			case SDLK_BACKSLASH:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_backslash=true;
				}
				else
				{
					input_keyboard_vertical_bar=true;
				}
			break;
			case SDLK_RIGHTBRACKET:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_close_square_brackets=true;
				}
				else
				{
					input_keyboard_close_curly_brackets=true;
				}
			break;
			case SDLK_BACKQUOTE:
				if (!(mod&KMOD_SHIFT))
				{
					input_keyboard_grave_accent=true;
				}
				else
				{
					input_keyboard_tilde=true;
				}
			break;
			case SDLK_a:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_a=true;
				}
				else
				{
					input_keyboard_A=true;
				}
			break;
			case SDLK_b:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_b=true;
				}
				else
				{
					input_keyboard_B=true;
				}
			break;
			case SDLK_c:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_c=true;
				}
				else
				{
					input_keyboard_C=true;
				}
			break;
			case SDLK_d:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_d=true;
				}
				else
				{
					input_keyboard_D=true;
				}
			break;
			case SDLK_e:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_e=true;
				}
				else
				{
					input_keyboard_E=true;
				}
			break;
			case SDLK_f:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_f=true;
				}
				else
				{
					input_keyboard_F=true;
				}
			break;
			case SDLK_g:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_g=true;
				}
				else
				{
					input_keyboard_G=true;
				}
			break;
			case SDLK_h:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_h=true;
				}
				else
				{
					input_keyboard_H=true;
				}
			break;
			case SDLK_i:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_i=true;
				}
				else
				{
					input_keyboard_I=true;
				}
			break;
			case SDLK_j:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_j=true;
				}
				else
				{
					input_keyboard_J=true;
				}
			break;
			case SDLK_k:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_k=true;
				}
				else
				{
					input_keyboard_K=true;
				}
			break;
			case SDLK_l:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_l=true;
				}
				else
				{
					input_keyboard_L=true;
				}
			break;
			case SDLK_m:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_m=true;
				}
				else
				{
					input_keyboard_M=true;
				}
			break;
			case SDLK_n:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_n=true;
				}
				else
				{
					input_keyboard_N=true;
				}
			break;
			case SDLK_o:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_o=true;
				}
				else
				{
					input_keyboard_O=true;
				}
			break;
			case SDLK_p:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_p=true;
				}
				else
				{
					input_keyboard_P=true;
				}
			break;
			case SDLK_q:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_q=true;
				}
				else
				{
					input_keyboard_Q=true;
				}
			break;
			case SDLK_r:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_r=true;
				}
				else
				{
					input_keyboard_R=true;
				}
			break;
			case SDLK_s:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_s=true;
				}
				else
				{
					input_keyboard_S=true;
				}
			break;
			case SDLK_t:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_t=true;
				}
				else
				{
					input_keyboard_T=true;
				}
			break;
			case SDLK_u:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_u=true;
				}
				else
				{
					input_keyboard_U=true;
				}
			break;
			case SDLK_v:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_v=true;
				}
				else
				{
					input_keyboard_V=true;
				}
			break;
			case SDLK_w:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_w=true;
				}
				else
				{
					input_keyboard_W=true;
				}
			break;
			case SDLK_x:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_x=true;
				}
				else
				{
					input_keyboard_X=true;
				}
			break;
			case SDLK_y:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_y=true;
				}
				else
				{
					input_keyboard_Y=true;
				}
			break;
			case SDLK_z:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_CAPS))
				{
					input_keyboard_z=true;
				}
				else
				{
					input_keyboard_Z=true;
				}
			break;
			case SDLK_DELETE:
				input_keyboard_del=true;
			break;
			case SDLK_INSERT:
				input_keyboard_insert=true;
			break;
			case SDLK_HOME:
				input_keyboard_home=true;
			break;
			case SDLK_END:
				input_keyboard_end=true;
			break;
			case SDLK_PAGEUP:
				input_keyboard_pageup=true;
			break;
			case SDLK_PAGEDOWN:
				input_keyboard_pagedown=true;
			break;
			case SDLK_LEFT:
				input_keyboard_left=true;
			break;
			case SDLK_RIGHT:
				input_keyboard_right=true;
			break;
			case SDLK_UP:
				input_keyboard_up=true;
			break;
			case SDLK_DOWN:
				input_keyboard_down=true;
			break;
			case SDLK_KP0:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_NUM))
				{
					input_keyboard_insert=true;
				}
				else
				{
					input_keyboard_zero=true;
				}
			break;
			case SDLK_KP1:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_NUM))
				{
					input_keyboard_end=true;
				}
				else
				{
					input_keyboard_one=true;
				}
			break;
			case SDLK_KP2:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_NUM))
				{
					input_keyboard_down=true;
				}
				else
				{
					input_keyboard_two=true;
				}
			break;
			case SDLK_KP3:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_NUM))
				{
					input_keyboard_pagedown=true;
				}
				else
				{
					input_keyboard_three=true;
				}
			break;
			case SDLK_KP4:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_NUM))
				{
					input_keyboard_left=true;
				}
				else
				{
					input_keyboard_four=true;
				}
			break;
			case SDLK_KP5:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_NUM))
				{
				}
				else
				{
					input_keyboard_five=true;
				}
			break;
			case SDLK_KP6:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_NUM))
				{
					input_keyboard_right=true;
				}
				else
				{
					input_keyboard_six=true;
				}
			break;
			case SDLK_KP7:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_NUM))
				{
					input_keyboard_home=true;
				}
				else
				{
					input_keyboard_seven=true;
				}
			break;
			case SDLK_KP8:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_NUM))
				{
					input_keyboard_up=true;
				}
				else
				{
					input_keyboard_eight=true;
				}
			break;
			case SDLK_KP9:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_NUM))
				{
					input_keyboard_pageup=true;
				}
				else
				{
					input_keyboard_nine=true;
				}
			break;
			case SDLK_KP_PERIOD:
				if (!(mod&KMOD_SHIFT) && !(mod&KMOD_NUM))
				{
					input_keyboard_del=true;
				}
				else
				{
					input_keyboard_period=true;
				}
			break;
			case SDLK_KP_PLUS:
				input_keyboard_plus=true;
			break;
			case SDLK_KP_MINUS:
				input_keyboard_minus=true;
			break;
			case SDLK_KP_MULTIPLY:
				input_keyboard_asterisk=true;
			break;
			case SDLK_KP_DIVIDE:
				input_keyboard_slash=true;
			break;
			case SDLK_KP_ENTER:
				input_keyboard_enter=true;
			break;
			}
		break;
		case SDL_KEYUP:
			switch (sdl_event.key.keysym.sym)
			{
			case SDLK_BACKSPACE:
				input_keyboard_backspace=false;
			break;
			case SDLK_TAB:
				input_keyboard_tab=false;
			break;
			case SDLK_RETURN:
				input_keyboard_enter=false;
			break;
			case SDLK_ESCAPE:
				input_keyboard_escape=false;
			break;
			case SDLK_SPACE:
				input_keyboard_space=false;
			break;
			case SDLK_QUOTE:
				input_keyboard_apostrophe=false;
				input_keyboard_quotation=false;
			break;
			case SDLK_COMMA:
				input_keyboard_comma=false;
				input_keyboard_less_than=false;
			break;
			case SDLK_MINUS:
				input_keyboard_minus=false;
				input_keyboard_underscore=false;
			break;
			case SDLK_PERIOD:
				input_keyboard_period=false;
				input_keyboard_greater_than=false;
			break;
			case SDLK_SLASH:
				input_keyboard_slash=false;
				input_keyboard_question=false;
			break;
			case SDLK_0:
				input_keyboard_zero=false;
				input_keyboard_close_parentheses=false;
			break;
			case SDLK_1:
				input_keyboard_one=false;
				input_keyboard_exclamation=false;
			break;
			case SDLK_2:
				input_keyboard_two=false;
				input_keyboard_at=false;
			break;
			case SDLK_3:
				input_keyboard_three=false;
				input_keyboard_hash=false;
			break;
			case SDLK_4:
				input_keyboard_four=false;
				input_keyboard_dollar=false;
			break;
			case SDLK_5:
				input_keyboard_five=false;
				input_keyboard_percent=false;
			break;
			case SDLK_6:
				input_keyboard_six=false;
				input_keyboard_caret=false;
			break;
			case SDLK_7:
				input_keyboard_seven=false;
				input_keyboard_ampersand=false;
			break;
			case SDLK_8:
				input_keyboard_eight=false;
				input_keyboard_asterisk=false;
			break;
			case SDLK_9:
				input_keyboard_nine=false;
				input_keyboard_open_parentheses=false;
			break;
			case SDLK_SEMICOLON:
				input_keyboard_semicolon=false;
				input_keyboard_colon=false;
			break;
			case SDLK_EQUALS:
				input_keyboard_equals=false;
				input_keyboard_plus=false;
			break;
			case SDLK_LEFTBRACKET:
				input_keyboard_open_square_brackets=false;
				input_keyboard_open_curly_brackets=false;
			break;
			case SDLK_BACKSLASH:
				input_keyboard_backslash=false;
				input_keyboard_vertical_bar=false;
			break;
			case SDLK_RIGHTBRACKET:
				input_keyboard_close_square_brackets=false;
				input_keyboard_close_curly_brackets=false;
			break;
			case SDLK_BACKQUOTE:
				input_keyboard_grave_accent=false;
				input_keyboard_tilde=false;
			break;
			case SDLK_a:
				input_keyboard_a=false;
				input_keyboard_A=false;
			break;
			case SDLK_b:
				input_keyboard_b=false;
				input_keyboard_B=false;
			break;
			case SDLK_c:
				input_keyboard_c=false;
				input_keyboard_C=false;
			break;
			case SDLK_d:
				input_keyboard_d=false;
				input_keyboard_D=false;
			break;
			case SDLK_e:
				input_keyboard_e=false;
				input_keyboard_E=false;
			break;
			case SDLK_f:
				input_keyboard_f=false;
				input_keyboard_F=false;
			break;
			case SDLK_g:
				input_keyboard_g=false;
				input_keyboard_G=false;
			break;
			case SDLK_h:
				input_keyboard_h=false;
				input_keyboard_H=false;
			break;
			case SDLK_i:
				input_keyboard_i=false;
				input_keyboard_I=false;
			break;
			case SDLK_j:
				input_keyboard_j=false;
				input_keyboard_J=false;
			break;
			case SDLK_k:
				input_keyboard_k=false;
				input_keyboard_K=false;
			break;
			case SDLK_l:
				input_keyboard_l=false;
				input_keyboard_L=false;
			break;
			case SDLK_m:
				input_keyboard_m=false;
				input_keyboard_M=false;
			break;
			case SDLK_n:
				input_keyboard_n=false;
				input_keyboard_N=false;
			break;
			case SDLK_o:
				input_keyboard_o=false;
				input_keyboard_O=false;
			break;
			case SDLK_p:
				input_keyboard_p=false;
				input_keyboard_P=false;
			break;
			case SDLK_q:
				input_keyboard_q=false;
				input_keyboard_Q=false;
			break;
			case SDLK_r:
				input_keyboard_r=false;
				input_keyboard_R=false;
			break;
			case SDLK_s:
				input_keyboard_s=false;
				input_keyboard_S=false;
			break;
			case SDLK_t:
				input_keyboard_t=false;
				input_keyboard_T=false;
			break;
			case SDLK_u:
				input_keyboard_u=false;
				input_keyboard_U=false;
			break;
			case SDLK_v:
				input_keyboard_v=false;
				input_keyboard_V=false;
			break;
			case SDLK_w:
				input_keyboard_w=false;
				input_keyboard_W=false;
			break;
			case SDLK_x:
				input_keyboard_x=false;
				input_keyboard_X=false;
			break;
			case SDLK_y:
				input_keyboard_y=false;
				input_keyboard_Y=false;
			break;
			case SDLK_z:
				input_keyboard_z=false;
				input_keyboard_Z=false;
			break;
			case SDLK_DELETE:
				input_keyboard_del=false;
			break;
			case SDLK_INSERT:
				input_keyboard_insert=false;
			break;
			case SDLK_HOME:
				input_keyboard_home=false;
			break;
			case SDLK_END:
				input_keyboard_end=false;
			break;
			case SDLK_PAGEUP:
				input_keyboard_pageup=false;
			break;
			case SDLK_PAGEDOWN:
				input_keyboard_pagedown=false;
			break;
			case SDLK_LEFT:
				input_keyboard_left=false;
			break;
			case SDLK_RIGHT:
				input_keyboard_right=false;
			break;
			case SDLK_UP:
				input_keyboard_up=false;
			break;
			case SDLK_DOWN:
				input_keyboard_down=false;
			break;
			case SDLK_KP0:
				input_keyboard_insert=false;
				input_keyboard_zero=false;
			break;
			case SDLK_KP1:
				input_keyboard_end=false;
				input_keyboard_one=false;
			break;
			case SDLK_KP2:
				input_keyboard_down=false;
				input_keyboard_two=false;
			break;
			case SDLK_KP3:
				input_keyboard_pagedown=false;
				input_keyboard_three=false;
			break;
			case SDLK_KP4:
				input_keyboard_left=false;
				input_keyboard_four=false;
			break;
			case SDLK_KP5:
				input_keyboard_five=false;
			break;
			case SDLK_KP6:
				input_keyboard_right=false;
				input_keyboard_six=false;
			break;
			case SDLK_KP7:
				input_keyboard_home=false;
				input_keyboard_seven=false;
			break;
			case SDLK_KP8:
				input_keyboard_up=false;
				input_keyboard_eight=false;
			break;
			case SDLK_KP9:
				input_keyboard_pageup=false;
				input_keyboard_nine=false;
			break;
			case SDLK_KP_PERIOD:
				input_keyboard_del=false;
				input_keyboard_period=false;
			break;
			case SDLK_KP_PLUS:
				input_keyboard_plus=false;
			break;
			case SDLK_KP_MINUS:
				input_keyboard_minus=false;
			break;
			case SDLK_KP_MULTIPLY:
				input_keyboard_asterisk=false;
			break;
			case SDLK_KP_DIVIDE:
				input_keyboard_slash=false;
			break;
			case SDLK_KP_ENTER:
				input_keyboard_enter=false;
			break;
			}
		break;
		case SDL_MOUSEMOTION:
			input_mouse_x_rel+=sdl_event.motion.xrel;
			input_mouse_y_rel+=sdl_event.motion.yrel;
		break;
		case SDL_MOUSEBUTTONDOWN:
			switch (sdl_event.button.button)
			{
			case SDL_BUTTON_LEFT:
				input_mouse_left_button=true;
			break;
			case SDL_BUTTON_MIDDLE:
				input_mouse_middle_button=true;
			break;
			case SDL_BUTTON_RIGHT:
				input_mouse_right_button=true;
			break;
			}
		break;
		case SDL_MOUSEBUTTONUP:
			switch (sdl_event.button.button)
			{
			case SDL_BUTTON_LEFT:
				input_mouse_left_button=false;
			break;
			case SDL_BUTTON_MIDDLE:
				input_mouse_middle_button=false;
			break;
			case SDL_BUTTON_RIGHT:
				input_mouse_right_button=false;
			break;
			case SDL_BUTTON_WHEELUP:
				input_mouse_wheel_steps++;
			break;
			case SDL_BUTTON_WHEELDOWN:
				input_mouse_wheel_steps--;
			break;
			}
		break;
		case SDL_JOYAXISMOTION:
			switch (sdl_event.jaxis.which)
			{
			case 0:
				switch (sdl_event.jaxis.axis)
				{
				case 0:
					input_gamepad1_axis_left_x=(double)sdl_event.jaxis.value/32768;
				break;
				case 1:
					input_gamepad1_axis_left_y=(double)sdl_event.jaxis.value/32768;
				break;
				case 3:
					input_gamepad1_axis_right_x=(double)sdl_event.jaxis.value/32768;
				break;
				case 2:
					input_gamepad1_axis_right_y=(double)sdl_event.jaxis.value/32768;
				break;
				}
			break;
			case 1:
				switch (sdl_event.jaxis.axis)
				{
				case 0:
					input_gamepad2_axis_left_x=(double)sdl_event.jaxis.value/32768;
				break;
				case 1:
					input_gamepad2_axis_left_y=(double)sdl_event.jaxis.value/32768;
				break;
				case 3:
					input_gamepad2_axis_right_x=(double)sdl_event.jaxis.value/32768;
				break;
				case 2:
					input_gamepad2_axis_right_y=(double)sdl_event.jaxis.value/32768;
				break;
				}
			break;
			}
		break;
		case SDL_JOYHATMOTION:
			switch (sdl_event.jhat.which)
			{
			case 0:
				switch (sdl_event.jhat.value)
				{
				case SDL_HAT_LEFT:
					input_gamepad1_hat_left =true;
					input_gamepad1_hat_right=false;
					input_gamepad1_hat_up   =false;
					input_gamepad1_hat_down =false;
				break;
				case SDL_HAT_RIGHT:
					input_gamepad1_hat_left =false;
					input_gamepad1_hat_right=true;
					input_gamepad1_hat_up   =false;
					input_gamepad1_hat_down =false;
				break;
				case SDL_HAT_UP:
					input_gamepad1_hat_left =false;
					input_gamepad1_hat_right=false;
					input_gamepad1_hat_up   =true;
					input_gamepad1_hat_down =false;
				break;
				case SDL_HAT_DOWN:
					input_gamepad1_hat_left =false;
					input_gamepad1_hat_right=false;
					input_gamepad1_hat_up   =false;
					input_gamepad1_hat_down =true;
				break;
				case SDL_HAT_LEFTUP:
					input_gamepad1_hat_left =true;
					input_gamepad1_hat_right=false;
					input_gamepad1_hat_up   =true;
					input_gamepad1_hat_down =false;
				break;
				case SDL_HAT_LEFTDOWN:
					input_gamepad1_hat_left =true;
					input_gamepad1_hat_right=false;
					input_gamepad1_hat_up   =false;
					input_gamepad1_hat_down =true;
				break;
				case SDL_HAT_RIGHTUP:
					input_gamepad1_hat_left =false;
					input_gamepad1_hat_right=true;
					input_gamepad1_hat_up   =true;
					input_gamepad1_hat_down =false;
				break;
				case SDL_HAT_RIGHTDOWN:
					input_gamepad1_hat_left =false;
					input_gamepad1_hat_right=true;
					input_gamepad1_hat_up   =false;
					input_gamepad1_hat_down =true;
				break;
				case SDL_HAT_CENTERED:
					input_gamepad1_hat_left =false;
					input_gamepad1_hat_right=false;
					input_gamepad1_hat_up   =false;
					input_gamepad1_hat_down =false;
				break;
				}
			break;
			case 1:
				switch (sdl_event.jhat.value)
				{
				case SDL_HAT_LEFT:
					input_gamepad2_hat_left =true;
					input_gamepad2_hat_right=false;
					input_gamepad2_hat_up   =false;
					input_gamepad2_hat_down =false;
				break;
				case SDL_HAT_RIGHT:
					input_gamepad2_hat_left =false;
					input_gamepad2_hat_right=true;
					input_gamepad2_hat_up   =false;
					input_gamepad2_hat_down =false;
				break;
				case SDL_HAT_UP:
					input_gamepad2_hat_left =false;
					input_gamepad2_hat_right=false;
					input_gamepad2_hat_up   =true;
					input_gamepad2_hat_down =false;
				break;
				case SDL_HAT_DOWN:
					input_gamepad2_hat_left =false;
					input_gamepad2_hat_right=false;
					input_gamepad2_hat_up   =false;
					input_gamepad2_hat_down =true;
				break;
				case SDL_HAT_LEFTUP:
					input_gamepad2_hat_left =true;
					input_gamepad2_hat_right=false;
					input_gamepad2_hat_up   =true;
					input_gamepad2_hat_down =false;
				break;
				case SDL_HAT_LEFTDOWN:
					input_gamepad2_hat_left =true;
					input_gamepad2_hat_right=false;
					input_gamepad2_hat_up   =false;
					input_gamepad2_hat_down =true;
				break;
				case SDL_HAT_RIGHTUP:
					input_gamepad2_hat_left =false;
					input_gamepad2_hat_right=true;
					input_gamepad2_hat_up   =true;
					input_gamepad2_hat_down =false;
				break;
				case SDL_HAT_RIGHTDOWN:
					input_gamepad2_hat_left =false;
					input_gamepad2_hat_right=true;
					input_gamepad2_hat_up   =false;
					input_gamepad2_hat_down =true;
				break;
				case SDL_HAT_CENTERED:
					input_gamepad2_hat_left =false;
					input_gamepad2_hat_right=false;
					input_gamepad2_hat_up   =false;
					input_gamepad2_hat_down =false;
				break;
				}
			break;
			}
		break;
		case SDL_JOYBUTTONDOWN:
			switch (sdl_event.jbutton.which)
			{
			case 0:
				switch (sdl_event.jbutton.button)
				{
				case 5:
					input_gamepad1_start=true;
				break;
				case 4:
					input_gamepad1_exit=true;
				break;
				case 0:
					input_gamepad1_button1=true;
				break;
				case 1:
					input_gamepad1_button2=true;
				break;
				case 2:
					input_gamepad1_button3=true;
				break;
				case 3:
					input_gamepad1_button4=true;
				break;
				case 6:
					input_gamepad1_digital_trigger_left=true;
				break;
				case 7:
					input_gamepad1_digital_trigger_right=true;
				break;
				}
			break;
			case 1:
				switch (sdl_event.jbutton.button)
				{
				case 5:
					input_gamepad2_start=true;
				break;
				case 4:
					input_gamepad2_exit=true;
				break;
				case 0:
					input_gamepad2_button1=true;
				break;
				case 1:
					input_gamepad2_button2=true;
				break;
				case 2:
					input_gamepad2_button3=true;
				break;
				case 3:
					input_gamepad2_button4=true;
				break;
				case 6:
					input_gamepad2_digital_trigger_left=true;
				break;
				case 7:
					input_gamepad2_digital_trigger_right=true;
				break;
				}
			break;
			}
		break;
		case SDL_JOYBUTTONUP:
			switch (sdl_event.jbutton.which)
			{
			case 0:
				switch (sdl_event.jbutton.button)
				{
				case 5:
					input_gamepad1_start=false;
				break;
				case 4:
					input_gamepad1_exit=false;
				break;
				case 0:
					input_gamepad1_button1=false;
				break;
				case 1:
					input_gamepad1_button2=false;
				break;
				case 2:
					input_gamepad1_button3=false;
				break;
				case 3:
					input_gamepad1_button4=false;
				break;
				case 6:
					input_gamepad1_digital_trigger_left=false;
				break;
				case 7:
					input_gamepad1_digital_trigger_right=false;
				break;
				}
			break;
			case 1:
				switch (sdl_event.jbutton.button)
				{
				case 5:
					input_gamepad2_start=false;
				break;
				case 4:
					input_gamepad2_exit=false;
				break;
				case 0:
					input_gamepad2_button1=false;
				break;
				case 1:
					input_gamepad2_button2=false;
				break;
				case 2:
					input_gamepad2_button3=false;
				break;
				case 3:
					input_gamepad2_button4=false;
				break;
				case 6:
					input_gamepad2_digital_trigger_left=false;
				break;
				case 7:
					input_gamepad2_digital_trigger_right=false;
				break;
				}
			break;
			}
		break;
		case SDL_ACTIVEEVENT:
			clear();
		break;
		}
	}
}

bool INPUT::KEYBOARD::available()
{
	return(true);
}

bool INPUT::KEYBOARD::backspace()
{
	return(input_keyboard_backspace);
}

bool INPUT::KEYBOARD::tab()
{
	return(input_keyboard_tab);
}

bool INPUT::KEYBOARD::enter()
{
	return(input_keyboard_enter);
}

bool INPUT::KEYBOARD::escape()
{
	return(input_keyboard_escape);
}

bool INPUT::KEYBOARD::space()
{
	return(input_keyboard_space);
}

bool INPUT::KEYBOARD::exclamation()
{
	return(input_keyboard_exclamation);
}

bool INPUT::KEYBOARD::quotation()
{
	return(input_keyboard_quotation);
}

bool INPUT::KEYBOARD::hash()
{
	return(input_keyboard_hash);
}

bool INPUT::KEYBOARD::dollar()
{
	return(input_keyboard_dollar);
}

bool INPUT::KEYBOARD::percent()
{
	return(input_keyboard_percent);
}

bool INPUT::KEYBOARD::ampersand()
{
	return(input_keyboard_ampersand);
}

bool INPUT::KEYBOARD::apostrophe()
{
	return(input_keyboard_apostrophe);
}

bool INPUT::KEYBOARD::open_parentheses()
{
	return(input_keyboard_open_parentheses);
}

bool INPUT::KEYBOARD::close_parentheses()
{
	return(input_keyboard_close_parentheses);
}

bool INPUT::KEYBOARD::asterisk()
{
	return(input_keyboard_asterisk);
}

bool INPUT::KEYBOARD::plus()
{
	return(input_keyboard_plus);
}

bool INPUT::KEYBOARD::comma()
{
	return(input_keyboard_comma);
}

bool INPUT::KEYBOARD::minus()
{
	return(input_keyboard_minus);
}

bool INPUT::KEYBOARD::period()
{
	return(input_keyboard_period);
}

bool INPUT::KEYBOARD::slash()
{
	return(input_keyboard_slash);
}

bool INPUT::KEYBOARD::zero()
{
	return(input_keyboard_zero);
}

bool INPUT::KEYBOARD::one()
{
	return(input_keyboard_one);
}

bool INPUT::KEYBOARD::two()
{
	return(input_keyboard_two);
}

bool INPUT::KEYBOARD::three()
{
	return(input_keyboard_three);
}

bool INPUT::KEYBOARD::four()
{
	return(input_keyboard_four);
}

bool INPUT::KEYBOARD::five()
{
	return(input_keyboard_five);
}

bool INPUT::KEYBOARD::six()
{
	return(input_keyboard_six);
}

bool INPUT::KEYBOARD::seven()
{
	return(input_keyboard_seven);
}

bool INPUT::KEYBOARD::eight()
{
	return(input_keyboard_eight);
}

bool INPUT::KEYBOARD::nine()
{
	return(input_keyboard_nine);
}

bool INPUT::KEYBOARD::colon()
{
	return(input_keyboard_colon);
}

bool INPUT::KEYBOARD::semicolon()
{
	return(input_keyboard_semicolon);
}

bool INPUT::KEYBOARD::less_than()
{
	return(input_keyboard_less_than);
}

bool INPUT::KEYBOARD::equals()
{
	return(input_keyboard_equals);
}

bool INPUT::KEYBOARD::greater_than()
{
	return(input_keyboard_greater_than);
}

bool INPUT::KEYBOARD::question()
{
	return(input_keyboard_question);
}

bool INPUT::KEYBOARD::at()
{
	return(input_keyboard_at);
}

bool INPUT::KEYBOARD::A()
{
	return(input_keyboard_A);
}

bool INPUT::KEYBOARD::B()
{
	return(input_keyboard_B);
}

bool INPUT::KEYBOARD::C()
{
	return(input_keyboard_C);
}

bool INPUT::KEYBOARD::D()
{
	return(input_keyboard_D);
}

bool INPUT::KEYBOARD::E()
{
	return(input_keyboard_E);
}

bool INPUT::KEYBOARD::F()
{
	return(input_keyboard_F);
}

bool INPUT::KEYBOARD::G()
{
	return(input_keyboard_G);
}

bool INPUT::KEYBOARD::H()
{
	return(input_keyboard_H);
}

bool INPUT::KEYBOARD::I()
{
	return(input_keyboard_I);
}

bool INPUT::KEYBOARD::J()
{
	return(input_keyboard_J);
}

bool INPUT::KEYBOARD::K()
{
	return(input_keyboard_K);
}

bool INPUT::KEYBOARD::L()
{
	return(input_keyboard_L);
}

bool INPUT::KEYBOARD::M()
{
	return(input_keyboard_M);
}

bool INPUT::KEYBOARD::N()
{
	return(input_keyboard_N);
}

bool INPUT::KEYBOARD::O()
{
	return(input_keyboard_O);
}

bool INPUT::KEYBOARD::P()
{
	return(input_keyboard_P);
}

bool INPUT::KEYBOARD::Q()
{
	return(input_keyboard_Q);
}

bool INPUT::KEYBOARD::R()
{
	return(input_keyboard_R);
}

bool INPUT::KEYBOARD::S()
{
	return(input_keyboard_S);
}

bool INPUT::KEYBOARD::T()
{
	return(input_keyboard_T);
}

bool INPUT::KEYBOARD::U()
{
	return(input_keyboard_U);
}

bool INPUT::KEYBOARD::V()
{
	return(input_keyboard_V);
}

bool INPUT::KEYBOARD::W()
{
	return(input_keyboard_W);
}

bool INPUT::KEYBOARD::X()
{
	return(input_keyboard_X);
}

bool INPUT::KEYBOARD::Y()
{
	return(input_keyboard_Y);
}

bool INPUT::KEYBOARD::Z()
{
	return(input_keyboard_Z);
}

bool INPUT::KEYBOARD::open_square_brackets()
{
	return(input_keyboard_open_square_brackets);
}

bool INPUT::KEYBOARD::backslash()
{
	return(input_keyboard_backslash);
}

bool INPUT::KEYBOARD::close_square_brackets()
{
	return(input_keyboard_close_square_brackets);
}

bool INPUT::KEYBOARD::caret()
{
	return(input_keyboard_caret);
}

bool INPUT::KEYBOARD::underscore()
{
	return(input_keyboard_underscore);
}

bool INPUT::KEYBOARD::grave_accent()
{
	return(input_keyboard_grave_accent);
}

bool INPUT::KEYBOARD::a()
{
	return(input_keyboard_a);
}

bool INPUT::KEYBOARD::b()
{
	return(input_keyboard_b);
}

bool INPUT::KEYBOARD::c()
{
	return(input_keyboard_c);
}

bool INPUT::KEYBOARD::d()
{
	return(input_keyboard_d);
}

bool INPUT::KEYBOARD::e()
{
	return(input_keyboard_e);
}

bool INPUT::KEYBOARD::f()
{
	return(input_keyboard_f);
}

bool INPUT::KEYBOARD::g()
{
	return(input_keyboard_g);
}

bool INPUT::KEYBOARD::h()
{
	return(input_keyboard_h);
}

bool INPUT::KEYBOARD::i()
{
	return(input_keyboard_i);
}

bool INPUT::KEYBOARD::j()
{
	return(input_keyboard_j);
}

bool INPUT::KEYBOARD::k()
{
	return(input_keyboard_k);
}

bool INPUT::KEYBOARD::l()
{
	return(input_keyboard_l);
}

bool INPUT::KEYBOARD::m()
{
	return(input_keyboard_m);
}

bool INPUT::KEYBOARD::n()
{
	return(input_keyboard_n);
}

bool INPUT::KEYBOARD::o()
{
	return(input_keyboard_o);
}

bool INPUT::KEYBOARD::p()
{
	return(input_keyboard_p);
}

bool INPUT::KEYBOARD::q()
{
	return(input_keyboard_q);
}

bool INPUT::KEYBOARD::r()
{
	return(input_keyboard_r);
}

bool INPUT::KEYBOARD::s()
{
	return(input_keyboard_s);
}

bool INPUT::KEYBOARD::t()
{
	return(input_keyboard_t);
}

bool INPUT::KEYBOARD::u()
{
	return(input_keyboard_u);
}

bool INPUT::KEYBOARD::v()
{
	return(input_keyboard_v);
}

bool INPUT::KEYBOARD::w()
{
	return(input_keyboard_w);
}

bool INPUT::KEYBOARD::x()
{
	return(input_keyboard_x);
}

bool INPUT::KEYBOARD::y()
{
	return(input_keyboard_y);
}

bool INPUT::KEYBOARD::z()
{
	return(input_keyboard_z);
}

bool INPUT::KEYBOARD::open_curly_brackets()
{
	return(input_keyboard_open_curly_brackets);
}

bool INPUT::KEYBOARD::vertical_bar()
{
	return(input_keyboard_vertical_bar);
}

bool INPUT::KEYBOARD::close_curly_brackets()
{
	return(input_keyboard_close_curly_brackets);
}

bool INPUT::KEYBOARD::tilde()
{
	return(input_keyboard_tilde);
}

bool INPUT::KEYBOARD::del()
{
	return(input_keyboard_del);
}

bool INPUT::KEYBOARD::insert()
{
	return(input_keyboard_insert);
}

bool INPUT::KEYBOARD::home()
{
	return(input_keyboard_home);
}

bool INPUT::KEYBOARD::end()
{
	return(input_keyboard_end);
}

bool INPUT::KEYBOARD::pageup()
{
	return(input_keyboard_pageup);
}

bool INPUT::KEYBOARD::pagedown()
{
	return(input_keyboard_pagedown);
}

bool INPUT::KEYBOARD::left()
{
	return(input_keyboard_left);
}

bool INPUT::KEYBOARD::right()
{
	return(input_keyboard_right);
}

bool INPUT::KEYBOARD::up()
{
	return(input_keyboard_up);
}

bool INPUT::KEYBOARD::down()
{
	return(input_keyboard_down);
}

bool INPUT::MOUSE::available()
{
	return(true);
}

double INPUT::MOUSE::x_rel()
{
	return((double)input_mouse_x_rel);
}

double INPUT::MOUSE::y_rel()
{
	return((double)input_mouse_y_rel);
}

bool INPUT::MOUSE::left_button()
{
	return(input_mouse_left_button);
}

bool INPUT::MOUSE::middle_button()
{
	return(input_mouse_middle_button);
}

bool INPUT::MOUSE::right_button()
{
	return(input_mouse_right_button);
}

int INPUT::MOUSE::wheel_steps()
{
	return(input_mouse_wheel_steps);
}

bool INPUT::GAMEPAD1::available()
{
	return(input_gamepad1_available);
}

double INPUT::GAMEPAD1::axis_left_x()
{
	return(input_gamepad1_axis_left_x);
}

double INPUT::GAMEPAD1::axis_left_y()
{
	return(input_gamepad1_axis_left_y);
}

double INPUT::GAMEPAD1::axis_right_x()
{
	return(input_gamepad1_axis_right_x);
}

double INPUT::GAMEPAD1::axis_right_y()
{
	return(input_gamepad1_axis_right_y);
}

bool INPUT::GAMEPAD1::hat_left()
{
	return(input_gamepad1_hat_left);
}

bool INPUT::GAMEPAD1::hat_right()
{
	return(input_gamepad1_hat_right);
}

bool INPUT::GAMEPAD1::hat_up()
{
	return(input_gamepad1_hat_up);
}

bool INPUT::GAMEPAD1::hat_down()
{
	return(input_gamepad1_hat_down);
}

bool INPUT::GAMEPAD1::start()
{
	return(input_gamepad1_start);
}

bool INPUT::GAMEPAD1::exit()
{
	return(input_gamepad1_exit);
}

bool INPUT::GAMEPAD1::button1()
{
	return(input_gamepad1_button1);
}

bool INPUT::GAMEPAD1::button2()
{
	return(input_gamepad1_button2);
}

bool INPUT::GAMEPAD1::button3()
{
	return(input_gamepad1_button3);
}

bool INPUT::GAMEPAD1::button4()
{
	return(input_gamepad1_button4);
}

bool INPUT::GAMEPAD1::digital_trigger_left()
{
	return(input_gamepad1_digital_trigger_left);
}

bool INPUT::GAMEPAD1::digital_trigger_right()
{
	return(input_gamepad1_digital_trigger_right);
}

bool INPUT::GAMEPAD2::available()
{
	return(input_gamepad2_available);
}

double INPUT::GAMEPAD2::axis_left_x()
{
	return(input_gamepad2_axis_left_x);
}

double INPUT::GAMEPAD2::axis_left_y()
{
	return(input_gamepad2_axis_left_y);
}

double INPUT::GAMEPAD2::axis_right_x()
{
	return(input_gamepad2_axis_right_x);
}

double INPUT::GAMEPAD2::axis_right_y()
{
	return(input_gamepad2_axis_right_y);
}

bool INPUT::GAMEPAD2::hat_left()
{
	return(input_gamepad2_hat_left);
}

bool INPUT::GAMEPAD2::hat_right()
{
	return(input_gamepad2_hat_right);
}

bool INPUT::GAMEPAD2::hat_up()
{
	return(input_gamepad2_hat_up);
}

bool INPUT::GAMEPAD2::hat_down()
{
	return(input_gamepad2_hat_down);
}

bool INPUT::GAMEPAD2::start()
{
	return(input_gamepad2_start);
}

bool INPUT::GAMEPAD2::exit()
{
	return(input_gamepad2_exit);
}

bool INPUT::GAMEPAD2::button1()
{
	return(input_gamepad2_button1);
}

bool INPUT::GAMEPAD2::button2()
{
	return(input_gamepad2_button2);
}

bool INPUT::GAMEPAD2::button3()
{
	return(input_gamepad2_button3);
}

bool INPUT::GAMEPAD2::button4()
{
	return(input_gamepad2_button4);
}

bool INPUT::GAMEPAD2::digital_trigger_left()
{
	return(input_gamepad2_digital_trigger_left);
}

bool INPUT::GAMEPAD2::digital_trigger_right()
{
	return(input_gamepad2_digital_trigger_right);
}

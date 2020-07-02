//Code by Rogier Walraven. Do not distribute.

namespace INPUT
{
    void init();
    void quit();

    void clear();
    void update();

    namespace KEYBOARD
    {
		bool available();

		bool backspace();
		bool tab();
		bool enter();
		bool escape();

		bool space();
		bool exclamation();
		bool quotation();
		bool hash();
		bool dollar();
		bool percent();
		bool ampersand();
		bool apostrophe();
		bool open_parentheses();
		bool close_parentheses();
		bool asterisk();
		bool plus();
		bool comma();
		bool minus();
		bool period();
		bool slash();

		bool zero();
		bool one();
		bool two();
		bool three();
		bool four();
		bool five();
		bool six();
		bool seven();
		bool eight();
		bool nine();

		bool colon();
		bool semicolon();
		bool less_than();
		bool equals();
		bool greater_than();
		bool question();
		bool at();

		bool A();
		bool B();
		bool C();
		bool D();
		bool E();
		bool F();
		bool G();
		bool H();
		bool I();
		bool J();
		bool K();
		bool L();
		bool M();
		bool N();
		bool O();
		bool P();
		bool Q();
		bool R();
		bool S();
		bool T();
		bool U();
		bool V();
		bool W();
		bool X();
		bool Y();
		bool Z();

		bool open_square_brackets();
		bool backslash();
		bool close_square_brackets();
		bool caret();
		bool underscore();
		bool grave_accent();

		bool a();
		bool b();
		bool c();
		bool d();
		bool e();
		bool f();
		bool g();
		bool h();
		bool i();
		bool j();
		bool k();
		bool l();
		bool m();
		bool n();
		bool o();
		bool p();
		bool q();
		bool r();
		bool s();
		bool t();
		bool u();
		bool v();
		bool w();
		bool x();
		bool y();
		bool z();

		bool open_curly_brackets();
		bool vertical_bar();
		bool close_curly_brackets();
		bool tilde();
		bool del();

		bool insert();
		bool home();
		bool end();
		bool pageup();
		bool pagedown();

		bool left();
		bool right();
		bool up();
		bool down();
    }

	namespace MOUSE
	{
		bool available();

		double x_rel();
		double y_rel();

		bool left_button();
		bool middle_button();
		bool right_button();

		int wheel_steps();
	}

	namespace GAMEPAD1
	{
		bool available();

		double axis_left_x();
		double axis_left_y();

		double axis_right_x();
		double axis_right_y();

		bool hat_left();
		bool hat_right();
		bool hat_up();
		bool hat_down();

		bool start();
		bool exit();

		bool button1();
		bool button2();
		bool button3();
		bool button4();

		bool digital_trigger_left();
		bool digital_trigger_right();
	}

	namespace GAMEPAD2
	{
		bool available();

		double axis_left_x();
		double axis_left_y();

		double axis_right_x();
		double axis_right_y();

		bool hat_left();
		bool hat_right();
		bool hat_up();
		bool hat_down();

		bool start();
		bool exit();

		bool button1();
		bool button2();
		bool button3();
		bool button4();

		bool digital_trigger_left();
		bool digital_trigger_right();
	}
}

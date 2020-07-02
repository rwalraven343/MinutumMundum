//Code by Rogier Walraven. Do not distribute.

namespace SYSTEM
{
	typedef unsigned char      BYTE;
	typedef unsigned short     WORD;
	typedef unsigned int       DWORD;
	typedef unsigned long long QWORD;

	typedef signed   int       S_DWORD;

    void init();
    void quit();

	double seconds_since_startup();

	struct RFILE
	{
		RFILE(const char *filename);
		~RFILE();

		bool isopen() const;

		BYTE  readbyte() const;
		WORD  readword() const;
		DWORD readdword() const;
		QWORD readqword() const;

	  private:

		bool open;
		void *stream;

		RFILE(const RFILE &other) {}
		RFILE& operator=(const RFILE &other) {}
	};

	struct WFILE
	{
		WFILE(const char *filename);
		~WFILE();

		bool isopen() const;

		WFILE& writebyte(BYTE byte);
		WFILE& writeword(WORD word);
		WFILE& writedword(DWORD dword);
		WFILE& writeqword(QWORD qword);

	  private:

		bool open;
		void *stream;

		WFILE(const WFILE &other) {}
		WFILE& operator=(const WFILE &other) {}
	};
}

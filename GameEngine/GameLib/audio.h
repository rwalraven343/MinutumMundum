//Code by Rogier Walraven. Do not distribute.

namespace AUDIO
{
	void init();
	void quit();

	double getmastervolume();
	void   setmastervolume(double vol);

	double getmastervolume_sfx();
	void   setmastervolume_sfx(double vol);

	double getmastervolume_music();
	void   setmastervolume_music(double vol);

	void setspatial_scale      (double sc);
	void setspatial_scale_fixed(double sc);

	void listener_topdown  (double xpos,double ypos,double xvel,double yvel,double angle);
	void listener_platform (double xpos,double ypos,double xvel,double yvel);

	struct SOURCE
	{
		SOURCE(const char *filename,double loudness,bool fixed=false);
		~SOURCE();

		bool initial() const;
		bool playing() const;
		bool stopped() const;

		void rewind() const;
		void play(double xpos,double ypos,double xvel,double yvel,double pitch,double volume) const;
		void stop() const;

	  private:

		bool fixed;
		double loudness;
		void *sample;
		mutable int channel;

		SOURCE(const SOURCE &other) {}
		SOURCE& operator=(const SOURCE &other) {}
	};

	namespace MUSIC
	{
		void init(const char *filename);
		void quit();

		bool initial();
		bool playing();
		bool stopped();

		void reload(const char *filename);
		void start();
		void stop();

		void update();
	}
}

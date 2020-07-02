//Code by Rogier Walraven. Do not distribute.

template<class W>
struct FEATURE
{
	FEATURE() {}

	virtual void main_init(const VECTOR &pos,double angle,const VECTOR &vel,double angular_vel,W &world)=0;
	virtual void main_reset(W &world)=0;

	virtual bool main_wasinit() const=0;
	virtual bool main_exit() const=0;

	virtual void main_update(W &world)=0;

  private:

	FEATURE(const FEATURE<W> &other) {}
	FEATURE<W>& operator=(const FEATURE<W> &other) {}
};

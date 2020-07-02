//Code by Rogier Walraven. Do not distribute.

enum ELLIPSE_AXIS
{
	EA_X,
	EA_Y,
	EA_Z
};

VECTOR<3> *generate_ellipse_points(ELLIPSE_AXIS axis,const QUATERNION &rot,
                                   double radiusx,double radiusy,int numsteps);

VECTOR<3> *generate_ellipses_points(int numapp,int numorb,
                                    ELLIPSE_AXIS axis,const QUATERNION &rot,
                                    double radiusx,double radiusy,int numsteps);

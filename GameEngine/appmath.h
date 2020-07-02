//Code by Rogier Walraven. Do not distribute.

extern const double math_pi;
extern const double math_epsilon;

double sine      (double x);
double cosine    (double x);
double invsine   (double x);
double invcosine (double x);
double power     (double x,double y);
double square    (double x);
double squareroot(double x);
double absolute  (double x);
double sign      (double x);
double modulus   (double x,double m);
int    appround  (double x);
bool   equalszero(double x);

double compact_gaussian     (double x,double interval,bool normalized=true);
double compact_gaussian_diff(double x,double interval,bool normalized=true);

double sph_kernel         (double x,double y,double px,double py,double radius);
double sph_kernel_grad    (double x,double y,double px,double py,double radius);
double sph_kernel_div_grad(double x,double y,double px,double py,double radius);

//Code generated by MapCompiler.
//Ballzz pinball table.

namespace BALLZZ_TABLE_MAP
{
	namespace SHAPES
	{
		namespace SHAPE1
		{
			static const TRIANGLE triangle1(VECTOR(0.000000000000,1.000000000000),VECTOR(1.000000000000,0.000000000000),VECTOR(1.000000000000,1.000000000000),0.750000000000,0.250000000000,0.250000000000,8.000000000000);

			static const TRIANGLE triangles[1]={triangle1};
		}

		static const MAP_PROPERTIES::SHAPE shape1={1,0,SHAPE1::triangles,0};

		namespace SHAPE2
		{
			static const TRIANGLE triangle1(VECTOR(0.000000000000,0.000000000000),VECTOR(1.000000000000,1.000000000000),VECTOR(0.000000000000,1.000000000000),0.750000000000,0.250000000000,0.250000000000,8.000000000000);

			static const TRIANGLE triangles[1]={triangle1};
		}

		static const MAP_PROPERTIES::SHAPE shape2={1,0,SHAPE2::triangles,0};

		namespace SHAPE3
		{
			static const TRIANGLE triangle1(VECTOR(0.000000000000,0.000000000000),VECTOR(1.000000000000,0.000000000000),VECTOR(1.000000000000,1.000000000000),0.750000000000,0.250000000000,0.250000000000,8.000000000000);

			static const TRIANGLE triangles[1]={triangle1};
		}

		static const MAP_PROPERTIES::SHAPE shape3={1,0,SHAPE3::triangles,0};

		namespace SHAPE4
		{
			static const TRIANGLE triangle1(VECTOR(0.000000000000,0.000000000000),VECTOR(1.000000000000,0.000000000000),VECTOR(0.000000000000,1.000000000000),0.750000000000,0.250000000000,0.250000000000,8.000000000000);

			static const TRIANGLE triangles[1]={triangle1};
		}

		static const MAP_PROPERTIES::SHAPE shape4={1,0,SHAPE4::triangles,0};

		namespace SHAPE5
		{
			static const QUAD     quad1    (VECTOR(0.000000000000,0.000000000000),VECTOR(1.000000000000,0.000000000000),VECTOR(1.000000000000,1.000000000000),VECTOR(0.000000000000,1.000000000000),0.750000000000,0.250000000000,0.250000000000,8.000000000000);

			static const QUAD     quads    [1]={quad1};
		}

		static const MAP_PROPERTIES::SHAPE shape5={0,1,0,SHAPE5::quads};

		namespace SHAPE6
		{
			static const TRIANGLE triangle1(VECTOR(0.000000000000,0.000000000000),VECTOR(0.500000000000,1.000000000000),VECTOR(0.000000000000,1.000000000000),0.750000000000,0.250000000000,0.250000000000,8.000000000000);
			static const TRIANGLE triangle2(VECTOR(0.500000000000,1.000000000000),VECTOR(1.000000000000,0.000000000000),VECTOR(1.000000000000,1.000000000000),0.750000000000,0.250000000000,0.250000000000,8.000000000000);

			static const TRIANGLE triangles[2]={triangle1,triangle2};
		}

		static const MAP_PROPERTIES::SHAPE shape6={2,0,SHAPE6::triangles,0};
	}

	static const int numshapes=6;

	static const MAP_PROPERTIES::SHAPE shapes[6]={SHAPES::shape1,SHAPES::shape2,SHAPES::shape3,SHAPES::shape4,SHAPES::shape5,SHAPES::shape6};

	namespace AMBIENT_SOUNDS
	{
		static const MAP_PROPERTIES::AMBIENT_SOUND ambient_sound1={"Data\\Samples\\metallic_knocking.wav",VECTOR(8192.000000000000,16384.000000000000),16384.000000000000,128.000000000000,1.000000000000,0.500000000000,0.500000000000};
		static const MAP_PROPERTIES::AMBIENT_SOUND ambient_sound2={"Data\\Samples\\metallic_swing.wav",VECTOR(8192.000000000000,16384.000000000000),16384.000000000000,128.000000000000,1.000000000000,0.500000000000,0.500000000000};
		static const MAP_PROPERTIES::AMBIENT_SOUND ambient_sound3={"Data\\Samples\\metallic_drag.wav",VECTOR(8192.000000000000,16384.000000000000),16384.000000000000,128.000000000000,1.000000000000,0.500000000000,0.500000000000};
		static const MAP_PROPERTIES::AMBIENT_SOUND ambient_sound4={"Data\\Samples\\metallic_knocking.wav",VECTOR(8192.000000000000,16384.000000000000),16384.000000000000,128.000000000000,1.000000000000,0.500000000000,0.500000000000};
		static const MAP_PROPERTIES::AMBIENT_SOUND ambient_sound5={"Data\\Samples\\metallic_swing.wav",VECTOR(8192.000000000000,16384.000000000000),16384.000000000000,128.000000000000,1.000000000000,0.500000000000,0.500000000000};
		static const MAP_PROPERTIES::AMBIENT_SOUND ambient_sound6={"Data\\Samples\\metallic_drag.wav",VECTOR(8192.000000000000,16384.000000000000),16384.000000000000,128.000000000000,1.000000000000,0.500000000000,0.500000000000};
	}

	static const int numambient_sounds=6;

	static const MAP_PROPERTIES::AMBIENT_SOUND ambient_sounds[6]={AMBIENT_SOUNDS::ambient_sound1,AMBIENT_SOUNDS::ambient_sound2,AMBIENT_SOUNDS::ambient_sound3,AMBIENT_SOUNDS::ambient_sound4,AMBIENT_SOUNDS::ambient_sound5,AMBIENT_SOUNDS::ambient_sound6};

	namespace PRIMARY_LAYER
	{
		static const int numlinear_motions =0;
		static const int numangular_motions=0;

		static const MAP_PROPERTIES::LINEAR_MOTION *linear_motions=0;
		static const MAP_PROPERTIES::ANGULAR_MOTION *angular_motions=0;
	}

	namespace SECONDARY_LAYER
	{
		static const int numlinear_motions =0;
		static const int numangular_motions=0;

		static const MAP_PROPERTIES::LINEAR_MOTION *linear_motions=0;
		static const MAP_PROPERTIES::ANGULAR_MOTION *angular_motions=0;
	}

	namespace TERTIARY_LAYER
	{
		static const int numlinear_motions =0;
		static const int numangular_motions=0;

		static const MAP_PROPERTIES::LINEAR_MOTION *linear_motions=0;
		static const MAP_PROPERTIES::ANGULAR_MOTION *angular_motions=0;
	}

	namespace PRIMARY_LAYER
	{
		static const int numbeams=0;

		static const MAP_PROPERTIES::BEAM *beams=0;
	}

	namespace SECONDARY_LAYER
	{
		static const int numbeams=0;

		static const MAP_PROPERTIES::BEAM *beams=0;
	}

	namespace TERTIARY_LAYER
	{
		static const int numbeams=0;

		static const MAP_PROPERTIES::BEAM *beams=0;
	}

	namespace PRIMARY_LAYER
	{
		static const int numpointlights=0;

		static const MAP_PROPERTIES::POINTLIGHT *pointlights=0;
	}

	namespace SECONDARY_LAYER
	{
		static const int numpointlights=0;

		static const MAP_PROPERTIES::POINTLIGHT *pointlights=0;
	}

	namespace TERTIARY_LAYER
	{
		static const int numpointlights=0;

		static const MAP_PROPERTIES::POINTLIGHT *pointlights=0;
	}

	namespace FOREGROUND_LAYER
	{
		static const int numpointlights=0;

		static const MAP_PROPERTIES::POINTLIGHT *pointlights=0;
	}

	namespace BACKGROUND_LAYER
	{
		static const int numpointlights=0;

		static const MAP_PROPERTIES::POINTLIGHT *pointlights=0;
	}

	namespace PRIMARY_LAYER
	{
		static const int texture_index[384]=
		{
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,5,0,0,0,
			0,-1,-1,-1,-1,-1,-1,-1,-1,6,7,-1,-1,5,0,0,
			0,-1,-1,-1,-1,-1,-1,-1,-1,0,0,7,-1,-1,5,0,
			0,-1,-1,-1,-1,6,7,-1,-1,-1,5,0,-1,-1,-1,0,
			0,-1,-1,-1,-1,5,2,-1,-1,-1,-1,0,-1,-1,6,0,
			0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,-1,0,0,
			0,-1,6,7,-1,-1,-1,-1,-1,-1,-1,0,7,-1,5,0,
			0,-1,5,2,-1,-1,-1,6,7,-1,-1,5,0,7,-1,0,
			0,-1,-1,-1,-1,-1,6,0,0,7,-1,-1,5,0,-1,0,
			0,-1,-1,-1,-1,6,0,0,0,0,7,-1,-1,0,-1,0,
			0,-1,-1,-1,-1,5,0,0,0,0,2,-1,-1,0,-1,0,
			0,-1,-1,-1,-1,-1,5,0,0,2,-1,-1,-1,0,-1,0,
			0,-1,-1,-1,-1,-1,-1,5,2,-1,6,7,-1,0,-1,0,
			0,-1,-1,-1,-1,-1,-1,-1,-1,-1,5,2,-1,0,-1,0,
			0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,0,
			0,-1,-1,6,7,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,0,
			0,-1,-1,5,2,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,0,
			0,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,0,
			0,0,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,0,
			0,0,0,7,-1,-1,-1,-1,-1,-1,-1,-1,6,0,-1,0,
			0,0,0,0,7,-1,-1,-1,-1,-1,-1,6,0,0,-1,0,
			0,0,0,0,0,7,-1,-1,-1,-1,6,0,0,0,-1,0,
			0,0,0,0,0,0,-1,-1,-1,-1,0,0,0,0,0,0
		};

		static const int shape_index[384]=
		{
			4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
			4,3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,2,4,4,4,
			4,-1,-1,-1,-1,-1,-1,-1,-1,0,1,-1,-1,2,4,4,
			4,-1,-1,-1,-1,-1,-1,-1,-1,4,4,1,-1,-1,2,4,
			4,-1,-1,-1,-1,0,1,-1,-1,-1,2,4,-1,-1,-1,4,
			4,-1,-1,-1,-1,2,3,-1,-1,-1,-1,4,-1,-1,0,4,
			4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,4,-1,-1,4,4,
			4,-1,0,1,-1,-1,-1,-1,-1,-1,-1,4,1,-1,2,4,
			4,-1,2,3,-1,-1,-1,0,1,-1,-1,2,4,1,-1,4,
			4,-1,-1,-1,-1,-1,0,4,4,1,-1,-1,2,4,-1,4,
			4,-1,-1,-1,-1,0,4,4,4,4,1,-1,-1,4,-1,4,
			4,-1,-1,-1,-1,2,4,4,4,4,3,-1,-1,4,-1,4,
			4,-1,-1,-1,-1,-1,2,4,4,3,-1,-1,-1,4,-1,4,
			4,-1,-1,-1,-1,-1,-1,2,3,-1,0,1,-1,4,-1,4,
			4,-1,-1,-1,-1,-1,-1,-1,-1,-1,2,3,-1,4,-1,4,
			4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,4,-1,4,
			4,-1,-1,0,1,-1,-1,-1,-1,-1,-1,-1,-1,4,-1,4,
			4,-1,-1,2,3,-1,-1,-1,-1,-1,-1,-1,-1,4,-1,4,
			4,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,4,-1,4,
			4,4,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,4,-1,4,
			4,4,4,1,-1,-1,-1,-1,-1,-1,-1,-1,0,4,-1,4,
			4,4,4,4,1,-1,-1,-1,-1,-1,-1,0,4,4,-1,4,
			4,4,4,4,4,1,-1,-1,-1,-1,0,4,4,4,-1,4,
			4,4,4,4,4,4,-1,-1,-1,-1,4,4,4,4,4,4
		};
	}

	namespace SECONDARY_LAYER
	{
		static const int texture_index[384]=
		{
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
		};

		static const int shape_index[384]=
		{
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
		};
	}

	namespace TERTIARY_LAYER
	{
		static const int texture_index[384]=
		{
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
		};

		static const int shape_index[384]=
		{
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
		};
	}

	namespace FOREGROUND_LAYER
	{
		static const int texture_index[384]=
		{
			73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,
			73,73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,
			73,73,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,73,73,
			73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73
		};
	}

	namespace BACKGROUND_LAYER
	{
		static const int texture_index[384]=
		{
			-1,-1,-1,74,73,73,74,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,74,73,73,74,-1,-1,-1,-1,-1,-1,-1,74,74,-1,-1,
			73,73,74,-1,-1,-1,-1,-1,-1,-1,-1,-1,74,74,-1,-1,
			-1,74,73,73,74,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,74,73,73,74,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,74,73,73,74,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,74,73,73,74,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,74,73,73,74,-1,-1,-1,
			-1,-1,-1,-1,74,74,-1,-1,-1,-1,-1,74,73,73,74,-1,
			-1,-1,-1,-1,74,74,-1,-1,-1,-1,-1,-1,-1,74,73,73,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,74,73,73,74,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,74,73,73,74,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,74,73,73,74,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,74,73,73,74,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,74,73,73,74,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,74,73,73,74,-1,-1,-1,-1,-1,-1,-1,74,74,-1,-1,
			73,73,74,-1,-1,-1,-1,-1,-1,-1,-1,-1,74,74,-1,-1,
			-1,74,73,73,74,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,74,73,73,74,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,74,73,73,74,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,74,73,73,74,-1,-1,-1,-1,-1,
			-1,-1,74,74,-1,-1,-1,-1,-1,74,73,73,74,-1,-1,-1,
			-1,-1,74,74,-1,-1,-1,-1,-1,-1,-1,74,73,73,74,74,
			-1,-1,-1,-1,-1,-1,74,74,-1,-1,-1,-1,-1,74,73,73
		};
	}
}

static const MAP_PROPERTIES ballzz_table_map_properties=
{
	0.125000000000,
	0.125000000000,
	1953.125000000000,
	16777216.000000000000,
	56.000000000000,
	0.250000000000,
	"Data\\Tilesets\\sci_fi_tiles.bmp",
	"Data\\Textures\\Skyplanes\\clouds.bmp",
	256,
	15104,
	128,
	128,
	120,
	155,
	236,
	BALLZZ_TABLE_MAP::numshapes,
	BALLZZ_TABLE_MAP::shapes,
	BALLZZ_TABLE_MAP::numambient_sounds,
	BALLZZ_TABLE_MAP::ambient_sounds,
	4.000000000000,
	4.000000000000,
	16,
	24,
	false,
	VECTOR(0.000000000000,8.000000000000),
	0.250000000000,
	1.000000000000,
	{
		BALLZZ_TABLE_MAP::PRIMARY_LAYER::texture_index,
		BALLZZ_TABLE_MAP::PRIMARY_LAYER::shape_index,
		false,
		VECTOR(0.000000000000,0.000000000000),
		0.000000000000,
		0,
		0,
		0,
		0,
		0,
		0,
		BALLZZ_TABLE_MAP::PRIMARY_LAYER::numlinear_motions,
		BALLZZ_TABLE_MAP::PRIMARY_LAYER::numangular_motions,
		BALLZZ_TABLE_MAP::PRIMARY_LAYER::linear_motions,
		BALLZZ_TABLE_MAP::PRIMARY_LAYER::angular_motions,
		false,
		{0,{0,0,0,0,0,0,0,0,0},{0,0,0},{0,0,0},0,0},
		BALLZZ_TABLE_MAP::PRIMARY_LAYER::numbeams,
		BALLZZ_TABLE_MAP::PRIMARY_LAYER::beams,
		BALLZZ_TABLE_MAP::PRIMARY_LAYER::numpointlights,
		BALLZZ_TABLE_MAP::PRIMARY_LAYER::pointlights
	},
	{
		BALLZZ_TABLE_MAP::SECONDARY_LAYER::texture_index,
		BALLZZ_TABLE_MAP::SECONDARY_LAYER::shape_index,
		false,
		VECTOR(0.000000000000,0.000000000000),
		0.000000000000,
		0,
		0,
		0,
		0,
		0,
		0,
		BALLZZ_TABLE_MAP::SECONDARY_LAYER::numlinear_motions,
		BALLZZ_TABLE_MAP::SECONDARY_LAYER::numangular_motions,
		BALLZZ_TABLE_MAP::SECONDARY_LAYER::linear_motions,
		BALLZZ_TABLE_MAP::SECONDARY_LAYER::angular_motions,
		false,
		{0,{0,0,0,0,0,0,0,0,0},{0,0,0},{0,0,0},0,0},
		BALLZZ_TABLE_MAP::SECONDARY_LAYER::numbeams,
		BALLZZ_TABLE_MAP::SECONDARY_LAYER::beams,
		BALLZZ_TABLE_MAP::SECONDARY_LAYER::numpointlights,
		BALLZZ_TABLE_MAP::SECONDARY_LAYER::pointlights
	},
	{
		BALLZZ_TABLE_MAP::TERTIARY_LAYER::texture_index,
		BALLZZ_TABLE_MAP::TERTIARY_LAYER::shape_index,
		false,
		VECTOR(0.000000000000,0.000000000000),
		0.000000000000,
		0,
		0,
		0,
		0,
		0,
		0,
		BALLZZ_TABLE_MAP::TERTIARY_LAYER::numlinear_motions,
		BALLZZ_TABLE_MAP::TERTIARY_LAYER::numangular_motions,
		BALLZZ_TABLE_MAP::TERTIARY_LAYER::linear_motions,
		BALLZZ_TABLE_MAP::TERTIARY_LAYER::angular_motions,
		false,
		{0,{0,0,0,0,0,0,0,0,0},{0,0,0},{0,0,0},0,0},
		BALLZZ_TABLE_MAP::TERTIARY_LAYER::numbeams,
		BALLZZ_TABLE_MAP::TERTIARY_LAYER::beams,
		BALLZZ_TABLE_MAP::TERTIARY_LAYER::numpointlights,
		BALLZZ_TABLE_MAP::TERTIARY_LAYER::pointlights
	},
	{
		BALLZZ_TABLE_MAP::FOREGROUND_LAYER::texture_index,
		0,
		true,
		VECTOR(0,0),
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		false,
		{0,{0,0,0,0,0,0,0,0,0},{0,0,0},{0,0,0},0,0},
		0,
		0,
		BALLZZ_TABLE_MAP::FOREGROUND_LAYER::numpointlights,
		BALLZZ_TABLE_MAP::FOREGROUND_LAYER::pointlights
	},
	{
		BALLZZ_TABLE_MAP::BACKGROUND_LAYER::texture_index,
		0,
		true,
		VECTOR(0,0),
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		false,
		{0,{0,0,0,0,0,0,0,0,0},{0,0,0},{0,0,0},0,0},
		0,
		0,
		BALLZZ_TABLE_MAP::BACKGROUND_LAYER::numpointlights,
		BALLZZ_TABLE_MAP::BACKGROUND_LAYER::pointlights
	}
};

namespace BALLZZ_TABLE_MAP
{
	namespace OBJECTS1
	{
	}
}

// T3DLIB4.H - Header file for T3DLIB4.CPP game engine library

// watch for multiple inclusions
#ifndef T3DLIB4
#define T3DLIB4

// DEFINES & CONSTANTS /////////////////////////////////////

// defines for small numbers
#define EPSILON_E3 (float)(1E-3) 
#define EPSILON_E4 (float)(1E-4) 
#define EPSILON_E5 (float)(1E-5)
#define EPSILON_E6 (float)(1E-6)

// defines for parametric line intersections
#define PARM_LINE_NO_INTERSECT          0
#define PARM_LINE_INTERSECT_IN_SEGMENT  1
#define PARM_LINE_INTERSECT_OUT_SEGMENT 2
#define PARM_LINE_INTERSECT_EVERYWHERE  3

// TYPES //////////////////////////////////////////////////

// 2x2 matrix /////////////////////////////////////////////
typedef struct MATRIX2X2_TYP
{
	union
	{
		float M[2][2]; // array indexed data storage

		// storage in row major form with explicit names
		struct
		{
			float M00, M01;
			float M10, M11;
		}; // end explicit names

	}; // end union
} MATRIX2X2, *MATRIX2X2_PTR;

// 1x4 matrix /////////////////////////////////////////////
typedef struct MATRIX1X4_TYP
{
	union
	{
		float M[4]; // array indexed data storage

		// storage in row major form with explicit names
		struct
		{
			float M00, M01, M02, M03;
		}; // end explicit names

	}; // end union
} MATRIX1X4, *MATRIX1X4_PTR;

// 4x4 matrix /////////////////////////////////////////////
typedef struct MATRIX4X4_TYP
{
	union
	{
		float M[4][4]; // array indexed data storage

		// storage in row major form with explicit names
		struct
		{
			float M00, M01, M02, M03;
			float M10, M11, M12, M13;
			float M20, M21, M22, M23;
			float M30, M31, M32, M33;
		}; // end explicit names

	}; // end union

} MATRIX4X4, *MATRIX4X4_PTR;

// 4x3 matrix /////////////////////////////////////////////
typedef struct MATRIX4X3_TYP
{
	union
	{
		float M[4][3]; // array indexed data storage

		// storage in row major form with explicit names
		struct
		{
			float M00, M01, M02;
			float M10, M11, M12;
			float M20, M21, M22;
			float M30, M31, M32;
		}; // end explicit names

	}; // end union

} MATRIX4X3, *MATRIX4X3_PTR;

// vector types ///////////////////////////////////////////

// 2D vector, point without the w ////////////////////////
typedef struct VECTOR2D_TYP
{
	union
	{
		float M[2]; // array indexed storage

		// explicit names
		struct
		{
			float x, y;
		}; // end struct

	}; // end union

} VECTOR2D, POINT2D, *VECTOR2D_PTR, *POINT2D_PTR;

// 3D vector, point without the w ////////////////////////
typedef struct VECTOR3D_TYP
{
	union
	{
		float M[3]; // array indexed storage

		// explicit names
		struct
		{
			float x, y, z;
		}; // end struct

	}; // end union

} VECTOR3D, POINT3D, *VECTOR3D_PTR, *POINT3D_PTR;

// 4D homogenous vector, point with w ////////////////////
typedef struct VECTOR4D_TYP
{
	union
	{
		float M[4]; // array indexed storage

		// explicit names
		struct
		{
			float x, y, z, w;
		}; // end struct
	}; // end union

} VECTOR4D, POINT4D, *VECTOR4D_PTR, *POINT4D_PTR;

// 2D parametric line /////////////////////////////////////////
typedef struct PARMLINE2D_TYP
{
	POINT2D  p0; // start point of parametric line
	POINT2D  p1; // end point of parametric line
	VECTOR2D v;  // direction vector of line segment
	// |v|=|p0->p1|
} PARMLINE2D, *PARMLINE2D_PTR;

// 3D parametric line /////////////////////////////////////////
typedef struct PARMLINE3D_TYP
{
	POINT3D  p0; // start point of parametric line
	POINT3D  p1; // end point of parametric line
	VECTOR3D v;  // direction vector of line segment
	// |v|=|p0->p1|
} PARMLINE3D, *PARMLINE3D_PTR;

// 3D plane ///////////////////////////////////////////////////
typedef struct PLANE3D_TYP
{
	POINT3D p0; // point on the plane
	VECTOR3D n; // normal to the plane (not necessarily a unit vector)
} PLANE3D, *PLANE3D_PTR;

// 2D polar coordinates ///////////////////////////////////////
typedef struct POLAR2D_TYP
{
	float r;     // the radi of the point
	float theta; // the angle in rads
} POLAR2D, *POLAR2D_PTR;

// 3D cylindrical coordinates ////////////////////////////////
typedef struct CYLINDRICAL3D_TYP
{
	float r;     // the radi of the point
	float theta; // the angle in degrees about the z axis
	float z;     // the z-height of the point
} CYLINDRICAL3D, *CYLINDRICAL3D_PTR;

// 3D spherical coordinates //////////////////////////////////
typedef struct SPHERICAL3D_TYP
{
	float p;      // rho, the distance to the point from the origin
	float theta;  // the angle from the z-axis and the line segment o->p
	float phi;    // the angle from the projection if o->p onto the x-y 
	// plane and the x-axis
} SPHERICAL3D, *SPHERICAL3D_PTR;

// 4d quaternion ////////////////////////////////////////////
// note the union gives us a number of ways to work with
// the components of the quaternion
typedef struct QUAT_TYP
{
	union
	{
		float M[4]; // array indexed storage w,x,y,z order

		// vector part, real part format
		struct
		{
			float    q0;  // the real part
			VECTOR3D qv;  // the imaginary part xi+yj+zk
		};
		struct
		{
			float w, x, y, z;
		};
	}; // end union

} QUAT, *QUAT_PTR;

// fixed point types //////////////////////////////////////////

typedef int FIXP16;
typedef int *FIXP16_PTR;

///////////////////////////////////////////////////////////////

// FORWARD REF DEFINES & CONSTANTS ////////////////////////////

// identity matrices

// 4x4 identity matrix
const MATRIX4X4 IMAT_4X4 = { 1,0,0,0,
							0,1,0,0,
							0,0,1,0,
							0,0,0,1 };

// 4x3 identity matrix (note this is not correct mathematically)
// but later we may use 4x3 matrices with the assumption that 
// the last column is always [0 0 0 1]t
const MATRIX4X3 IMAT_4X3 = { 1,0,0,
							0,1,0,
							0,0,1,
							0,0,0, };


// 3x3 identity matrix
const MATRIX3X3 IMAT_3X3 = { 1,0,0,
							0,1,0,
							0,0,1 };

// 2x2 identity matrix
const MATRIX2X2 IMAT_2X2 = { 1,0,
							0,1 };

// MACROS, SMALL INLINE FUNCS /////////////////////////////////

// matrix macros

// macros to clear out matrices
#define MAT_ZERO_2X2(m) {memset((void *)(m), 0, sizeof(MATRIX2X2));}
#define MAT_ZERO_3X3(m) {memset((void *)(m), 0, sizeof(MATRIX3X3));}
#define MAT_ZERO_4X4(m) {memset((void *)(m), 0, sizeof(MATRIX4X4));}
#define MAT_ZERO_4X3(m) {memset((void *)(m), 0, sizeof(MATRIX4X3));}

// macros to set the identity matrix
#define MAT_IDENTITY_2X2(m) {memcpy((void *)(m), (void *)&IMAT_2X2, sizeof(MATRIX2X2));}
#define MAT_IDENTITY_3X3(m) {memcpy((void *)(m), (void *)&IMAT_3X3, sizeof(MATRIX3X3));}
#define MAT_IDENTITY_4X4(m) {memcpy((void *)(m), (void *)&IMAT_4X4, sizeof(MATRIX4X4));}
#define MAT_IDENTITY_4X3(m) {memcpy((void *)(m), (void *)&IMAT_4X3, sizeof(MATRIX4X3));}

// matrix copying macros
#define MAT_COPY_2X2(src_mat, dest_mat) {memcpy((void *)(dest_mat), (void *)(src_mat), sizeof(MATRIX2X2) ); }
#define MAT_COPY_3X3(src_mat, dest_mat) {memcpy((void *)(dest_mat), (void *)(src_mat), sizeof(MATRIX3X3) ); }
#define MAT_COPY_4X4(src_mat, dest_mat) {memcpy((void *)(dest_mat), (void *)(src_mat), sizeof(MATRIX4X4) ); }
#define MAT_COPY_4X3(src_mat, dest_mat) {memcpy((void *)(dest_mat), (void *)(src_mat), sizeof(MATRIX4X3) ); }

// matrix transposing macros
inline void MAT_TRANSPOSE_3X3(MATRIX3X3_PTR m)
{
	MATRIX3X3 mt;
	mt.M00 = m->M00; mt.M01 = m->M10; mt.M02 = m->M20;
	mt.M10 = m->M01; mt.M11 = m->M11; mt.M12 = m->M21;
	mt.M20 = m->M02; mt.M21 = m->M12; mt.M22 = m->M22;
	memcpy((void *)m, (void *)&mt, sizeof(MATRIX3X3));
}

inline void MAT_TRANSPOSE_4X4(MATRIX4X4_PTR m)
{
	MATRIX4X4 mt;
	mt.M00 = m->M00; mt.M01 = m->M10; mt.M02 = m->M20; mt.M03 = m->M30;
	mt.M10 = m->M01; mt.M11 = m->M11; mt.M12 = m->M21; mt.M13 = m->M31;
	mt.M20 = m->M02; mt.M21 = m->M12; mt.M22 = m->M22; mt.M23 = m->M32;
	mt.M30 = m->M03; mt.M31 = m->M13; mt.M32 = m->M22; mt.M33 = m->M33;
	memcpy((void *)m, (void *)&mt, sizeof(MATRIX4X4));
}

inline void MAT_TRANSPOSE_3X3(MATRIX3X3_PTR m, MATRIX3X3_PTR mt)
{
	mt->M00 = m->M00; mt->M01 = m->M10; mt->M02 = m->M20;
	mt->M10 = m->M01; mt->M11 = m->M11; mt->M12 = m->M21;
	mt->M20 = m->M02; mt->M21 = m->M12; mt->M22 = m->M22;
}

inline void MAT_TRANSPOSE_4X4(MATRIX4X4_PTR m, MATRIX4X4_PTR mt)
{
	mt->M00 = m->M00; mt->M01 = m->M10; mt->M02 = m->M20; mt->M03 = m->M30;
	mt->M10 = m->M01; mt->M11 = m->M11; mt->M12 = m->M21; mt->M13 = m->M31;
	mt->M20 = m->M02; mt->M21 = m->M12; mt->M22 = m->M22; mt->M23 = m->M32;
	mt->M30 = m->M03; mt->M31 = m->M13; mt->M32 = m->M22; mt->M33 = m->M33;
}

// small inline functions that could be re-written as macros, but would
// be less robust

// matrix and vector column swaping macros
inline void MAT_COLUMN_SWAP_2X2(MATRIX2X2_PTR m, int c, MATRIX1X2_PTR v)
{
	m->M[0][c] = v->M[0]; m->M[1][c] = v->M[1];
}

inline void MAT_COLUMN_SWAP_3X3(MATRIX3X3_PTR m, int c, MATRIX1X3_PTR v)
{
	m->M[0][c] = v->M[0]; m->M[1][c] = v->M[1]; m->M[2][c] = v->M[2];
}

inline void MAT_COLUMN_SWAP_4X4(MATRIX4X4_PTR m, int c, MATRIX1X4_PTR v)
{
	m->M[0][c] = v->M[0]; m->M[1][c] = v->M[1]; m->M[2][c] = v->M[2]; m->M[3][c] = v->M[3];
}

inline void MAT_COLUMN_SWAP_4X3(MATRIX4X3_PTR m, int c, MATRIX1X4_PTR v)
{
	m->M[0][c] = v->M[0]; m->M[1][c] = v->M[1]; m->M[2][c] = v->M[2]; m->M[3][c] = v->M[3];
}

// vector macros, note the 4D vector sets w=1
// vector zeroing macros
inline void VECTOR2D_ZERO(VECTOR2D_PTR v)
{
	(v)->x = (v)->y = 0.0;
}

inline void VECTOR3D_ZERO(VECTOR3D_PTR v)
{
	(v)->x = (v)->y = (v)->z = 0.0;
}

inline void VECTOR4D_ZERO(VECTOR4D_PTR v)
{
	(v)->x = (v)->y = (v)->z = 0.0; (v)->w = 1.0;
}

// macros to initialize vectors with explicit components
inline void VECTOR2D_INITXY(VECTOR2D_PTR v, float x, float y)
{
	(v)->x = (x); (v)->y = (y);
}

inline void VECTOR3D_INITXYZ(VECTOR3D_PTR v, float x, float y, float z)
{
	(v)->x = (x); (v)->y = (y); (v)->z = (z);
}

inline void VECTOR4D_INITXYZ(VECTOR4D_PTR v, float x, float y, float z)
{
	(v)->x = (x); (v)->y = (y); (v)->z = (z); (v)->w = 1.0;
}

// used to convert from 4D homogenous to 4D non-homogenous
inline void VECTOR4D_DIV_BY_W(VECTOR4D_PTR v)
{
	(v)->x /= (v)->w; (v)->y /= (v)->w; (v)->z /= (v)->w;
}

inline void VECTOR4D_DIV_BY_W_VECTOR3D(VECTOR4D_PTR v4, VECTOR3D_PTR v3)
{
	(v3)->x = (v4)->x / (v4)->w; (v3)->y = (v4)->y / (v4)->w; (v3)->z = (v4)->z / (v4)->w;
}

// vector intialization macros to initialize with other vectors
inline void VECTOR2D_INIT(VECTOR2D_PTR vdst, VECTOR2D_PTR vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;
}

inline void VECTOR3D_INIT(VECTOR3D_PTR vdst, VECTOR3D_PTR vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;  (vdst)->z = (vsrc)->z;
}

inline void VECTOR4D_INIT(VECTOR4D_PTR vdst, VECTOR4D_PTR vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;
	(vdst)->z = (vsrc)->z; (vdst)->w = (vsrc)->w;
}


// vector copying macros
inline void VECTOR2D_COPY(VECTOR2D_PTR vdst, VECTOR2D_PTR vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;
}


inline void VECTOR3D_COPY(VECTOR3D_PTR vdst, VECTOR3D_PTR vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;  (vdst)->z = (vsrc)->z;
}

inline void VECTOR4D_COPY(VECTOR4D_PTR vdst, VECTOR4D_PTR vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;
	(vdst)->z = (vsrc)->z; (vdst)->w = (vsrc)->w;
}


// point initialization macros
inline void POINT2D_INIT(POINT2D_PTR vdst, POINT2D_PTR vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;
}

inline void POINT3D_INIT(POINT3D_PTR vdst, POINT3D_PTR vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;  (vdst)->z = (vsrc)->z;
}

inline void POINT4D_INIT(POINT4D_PTR vdst, POINT4D_PTR vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;
	(vdst)->z = (vsrc)->z; (vdst)->w = (vsrc)->w;
}

// point copying macros
inline void POINT2D_COPY(POINT2D_PTR vdst, POINT2D_PTR vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;
}

inline void POINT3D_COPY(POINT3D_PTR vdst, POINT3D_PTR vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;  (vdst)->z = (vsrc)->z;
}

inline void POINT4D_COPY(POINT4D_PTR vdst, POINT4D_PTR vsrc)
{
	(vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;
	(vdst)->z = (vsrc)->z; (vdst)->w = (vsrc)->w;
}

// quaternion macros
inline void QUAT_ZERO(QUAT_PTR q)
{
	(q)->x = (q)->y = (q)->z = (q)->w = 0.0;
}

inline void QUAT_INITWXYZ(QUAT_PTR q, float w, float x, float y, float z)
{
	(q)->w = (w); (q)->x = (x); (q)->y = (y); (q)->z = (z);
}

inline void QUAT_INIT_VECTOR3D(QUAT_PTR q, VECTOR3D_PTR v)
{
	(q)->w = 0; (q)->x = (v->x); (q)->y = (v->y); (q)->z = (v->z);
}

inline void QUAT_INIT(QUAT_PTR qdst, QUAT_PTR qsrc)
{
	(qdst)->w = (qsrc)->w;  (qdst)->x = (qsrc)->x;
	(qdst)->y = (qsrc)->y;  (qdst)->z = (qsrc)->z;
}

inline void QUAT_COPY(QUAT_PTR qdst, QUAT_PTR qsrc)
{
	(qdst)->x = (qsrc)->x; (qdst)->y = (qsrc)->y;
	(qdst)->z = (qsrc)->z; (qdst)->w = (qsrc)->w;
}

// convert integer and float to fixed point 16.16
#define INT_TO_FIXP16(i) ((i) <<  FIXP16_SHIFT)
#define FLOAT_TO_FIXP16(f) (((float)(f) * (float)FIXP16_MAG+0.5))

// convert fixed point to float
#define FIXP16_TO_FLOAT(fp) ( ((float)fp)/FIXP16_MAG)

// extract the whole part and decimal part from a fixed point 16.16
#define FIXP16_WP(fp) ((fp) >> FIXP16_SHIFT)
#define FIXP16_DP(fp) ((fp) && FIXP16_DP_MASK)

// FUNCTIONS //////////////////////////////////////////////

FIXP16 FIXP16_MUL(FIXP16 fp1, FIXP16 fp2)
{
	// this function computes the product fp_prod = fp1*fp2
	// using 64 bit math, so as not to loose precission

	return fp1 * fp2;

} // end FIXP16_MUL

///////////////////////////////////////////////////////////////

FIXP16 FIXP16_DIV(FIXP16 fp1, FIXP16 fp2)
{
	// this function computes the quotient fp1/fp2 using
	// 64 bit math, so as not to loose precision

	return fp1 / fp2;

} // end FIXP16_DIV

///////////////////////////////////////////////////////////////

void FIXP16_Print(FIXP16 fp)
{
	// this function prints out a fixed point number
	Write_Error("\nfp=%f", (float)(fp) / FIXP16_MAG);

} // end FIXP16_Print

///////////////////////////////////////////////////////////////

void QUAT_Add(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR qsum)
{
	// this function adds two quaternions
	qsum->x = q1->x + q2->x;
	qsum->y = q1->y + q2->y;
	qsum->z = q1->z + q2->z;
	qsum->w = q1->w + q2->w;

} // end QUAT_Add

///////////////////////////////////////////////////////////////

void QUAT_Sub(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR qdiff)
{
	// this function subtracts two quaternions, q1-q2
	qdiff->x = q1->x - q2->x;
	qdiff->y = q1->y - q2->y;
	qdiff->z = q1->z - q2->z;
	qdiff->w = q1->w - q2->w;

} // end QUAT_Sub

///////////////////////////////////////////////////////////////

void QUAT_Conjugate(QUAT_PTR q, QUAT_PTR qconj)
{
	// this function computes the conjugate of a quaternion
	qconj->x = -q->x;
	qconj->y = -q->y;
	qconj->z = -q->z;
	qconj->w = q->w;
} // end QUAT_Conjugate

///////////////////////////////////////////////////////////////

void QUAT_Scale(QUAT_PTR q, float scale, QUAT_PTR qs)
{
	// this function scales a quaternion and returns it 
	qs->x = scale * q->x;
	qs->y = scale * q->y;
	qs->z = scale * q->z;
	qs->w = scale * q->w;

} // end QUAT_Scale

///////////////////////////////////////////////////////////////

void QUAT_Scale(QUAT_PTR q, float scale)
{
	// this function scales a quaternion in place
	q->x *= scale;
	q->y *= scale;
	q->z *= scale;
	q->w *= scale;

} // end QUAT_Scale

//////////////////////////////////////////////////////////////

float QUAT_Norm(QUAT_PTR q)
{
	// returns the length or norm of a quaterion
	return(sqrtf(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z));

} // end QUAT_Norm

//////////////////////////////////////////////////////////////

float QUAT_Norm2(QUAT_PTR q)
{
	// returns the length or norm of a quaterion squared
	return(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);

} // end QUAT_Norm2

/////////////////////////////////////////////////////////////

void QUAT_Normalize(QUAT_PTR q, QUAT_PTR qn)
{
	// this functions normalizes the sent quaternion and 
	// returns it

	// compute 1/length
	float qlength_inv = 1.0 / (sqrtf(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z));

	// now normalize
	qn->w = q->w * qlength_inv;
	qn->x = q->x * qlength_inv;
	qn->y = q->y * qlength_inv;
	qn->z = q->z * qlength_inv;

} // end QUAT_Normalize

/////////////////////////////////////////////////////////////

void QUAT_Normalize(QUAT_PTR q)
{
	// this functions normalizes the sent quaternion in place

	// compute length
	float qlength_inv = 1.0 / (sqrtf(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z));

	// now normalize
	q->w *= qlength_inv;
	q->x *= qlength_inv;
	q->y *= qlength_inv;
	q->z *= qlength_inv;

} // end QUAT_Normalize

//////////////////////////////////////////////////////////////

void QUAT_Unit_Inverse(QUAT_PTR q, QUAT_PTR qi)
{
	// this function computes the inverse of a unit quaternion
	// and returns the result
	// the inverse of a unit quaternion is the conjugate :)
	qi->w = q->w;
	qi->x = -q->x;
	qi->y = -q->y;
	qi->z = -q->z;
} // end QUAT_Unit_Inverse

//////////////////////////////////////////////////////////////

void QUAT_Unit_Inverse(QUAT_PTR q)
{
	// this function computes the inverse of a unit quaternion
	// in place
	// the inverse of a unit quaternion is the conjugate :)
	q->x = -q->x;
	q->y = -q->y;
	q->z = -q->z;
} // end QUAT_Unit_Inverse

/////////////////////////////////////////////////////////////

void QUAT_Inverse(QUAT_PTR q, QUAT_PTR qi)
{
	// this function computes the inverse of a general quaternion
	// and returns result
	// in general, q-1 = *q/|q|2
	// compute norm squared
	float norm2_inv = 1.0 / (q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);

	// and plug in
	qi->w = q->w * norm2_inv;
	qi->x = -q->x * norm2_inv;
	qi->y = -q->y * norm2_inv;
	qi->z = -q->z * norm2_inv;

} // end QUAT_Inverse

/////////////////////////////////////////////////////////////

void QUAT_Inverse(QUAT_PTR q)
{
	// this function computes the inverse of a general quaternion
	// in place
	// in general, q-1 = *q/|q|2
	// compute norm squared
	float norm2_inv = 1.0 / (q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);

	// and plug in
	q->w = q->w * norm2_inv;
	q->x = -q->x * norm2_inv;
	q->y = -q->y * norm2_inv;
	q->z = -q->z * norm2_inv;

} // end QUAT_Inverse

/////////////////////////////////////////////////////////////

void QUAT_Mul(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR qprod)
{
	// this function multiplies two quaternions

	// this is the brute force method
	//qprod->w = q1->w*q2->w - q1->x*q2->x - q1->y*q2->y - q1->z*q2->z;
	//qprod->x = q1->w*q2->x + q1->x*q2->w + q1->y*q2->z - q1->z*q2->y;
	//qprod->y = q1->w*q2->y - q1->x*q2->z + q1->y*q2->w - q1->z*q2->x;
	//qprod->z = q1->w*q2->z + q1->x*q2->y - q1->y*q2->x + q1->z*q2->w;

	// this method was arrived at basically by trying to factor the above
	// expression to reduce the # of multiplies

	float prd_0 = (q1->z - q1->y) * (q2->y - q2->z);
	float prd_1 = (q1->w + q1->x) * (q2->w + q2->x);
	float prd_2 = (q1->w - q1->x) * (q2->y + q2->z);
	float prd_3 = (q1->y + q1->z) * (q2->w - q2->x);
	float prd_4 = (q1->z - q1->x) * (q2->x - q2->y);
	float prd_5 = (q1->z + q1->x) * (q2->x + q2->y);
	float prd_6 = (q1->w + q1->y) * (q2->w - q2->z);
	float prd_7 = (q1->w - q1->y) * (q2->w + q2->z);

	float prd_8 = prd_5 + prd_6 + prd_7;
	float prd_9 = 0.5 * (prd_4 + prd_8);

	// and finally build up the result with the temporary products

	qprod->w = prd_0 + prd_9 - prd_5;
	qprod->x = prd_1 + prd_9 - prd_8;
	qprod->y = prd_2 + prd_9 - prd_7;
	qprod->z = prd_3 + prd_9 - prd_6;

} // end QUAT_Mul

///////////////////////////////////////////////////////////

void QUAT_Triple_Product(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR q3,
	QUAT_PTR qprod)
{
	// this function computes q1*q2*q3 in that order and returns 
	// the results in qprod

	QUAT qtmp;
	QUAT_Mul(q1, q2, &qtmp);
	QUAT_Mul(&qtmp, q3, qprod);

} // end QUAT_Triple_Product

///////////////////////////////////////////////////////////

void VECTOR3D_Theta_To_QUAT(QUAT_PTR q, VECTOR3D_PTR v, float theta)
{
	// initializes a quaternion based on a 3d direction vector and angle
	// note the direction vector must be a unit vector
	// and the angle is in rads

	float theta_div_2 = (0.5) * theta; // compute theta/2

	// compute the quaterion, note this is from chapter 4
	// pre-compute to save time
	float sinf_theta = sinf(theta_div_2);

	q->x = sinf_theta * v->x;
	q->y = sinf_theta * v->y;
	q->z = sinf_theta * v->z;
	q->w = cosf(theta_div_2);

} // end VECTOR3D_Theta_To_QUAT

///////////////////////////////////////////////////////////////

void VECTOR4D_Theta_To_QUAT(QUAT_PTR q, VECTOR4D_PTR v, float theta)
{
	// initializes a quaternion based on a 4d direction vector and angle
	// note the direction vector must be a unit vector
	// and the angle is in rads

	float theta_div_2 = (0.5) * theta; // compute theta/2

	// compute the quaterion, note this is from chapter 4

	// pre-compute to save time
	float sinf_theta = sinf(theta_div_2);

	q->x = sinf_theta * v->x;
	q->y = sinf_theta * v->y;
	q->z = sinf_theta * v->z;
	q->w = cosf(theta_div_2);

} // end VECTOR4D_Theta_to_QUAT

///////////////////////////////////////////////////////////////

void EulerZYX_To_QUAT(QUAT_PTR q, float theta_z, float theta_y, float theta_x)
{
	// this function intializes a quaternion based on the zyx
	// multiplication order of the angles that are parallel to the
	// zyx axis respectively, note there are 11 other possibilities
	// this is just one, later we may make a general version of the
	// the function

	// precompute values
	float cos_z_2 = 0.5 * cosf(theta_z);
	float cos_y_2 = 0.5 * cosf(theta_y);
	float cos_x_2 = 0.5 * cosf(theta_x);

	float sin_z_2 = 0.5 * sinf(theta_z);
	float sin_y_2 = 0.5 * sinf(theta_y);
	float sin_x_2 = 0.5 * sinf(theta_x);

	// and now compute quaternion
	q->w = cos_z_2 * cos_y_2 * cos_x_2 + sin_z_2 * sin_y_2 * sin_x_2;
	q->x = cos_z_2 * cos_y_2 * sin_x_2 - sin_z_2 * sin_y_2 * cos_x_2;
	q->y = cos_z_2 * sin_y_2 * cos_x_2 + sin_z_2 * cos_y_2 * sin_x_2;
	q->z = sin_z_2 * cos_y_2 * cos_x_2 - cos_z_2 * sin_y_2 * sin_x_2;

} // EulerZYX_To_QUAT

///////////////////////////////////////////////////////////////

void QUAT_To_VECTOR3D_Theta(QUAT_PTR q, VECTOR3D_PTR v, float *theta)
{
	// this function converts a unit quaternion into a unit direction
	// vector and rotation angle about that vector

	// extract theta
	*theta = acosf(q->w);

	// pre-compute to save time
	float sinf_theta_inv = 1.0 / sinf(*theta);

	// now the vector
	v->x = q->x * sinf_theta_inv;
	v->y = q->y * sinf_theta_inv;
	v->z = q->z * sinf_theta_inv;

	// multiply by 2
	*theta *= 2;

} // end QUAT_To_VECTOR3D_Theta

////////////////////////////////////////////////////////////

void QUAT_Print(QUAT_PTR q, const char *name = "q")
{
	// this function prints out a quaternion
	Write_Error("\n%s=%f+[(%f)i + (%f)j + (%f)k]", name, q->w, q->x, q->y, q->z);
} // end QUAT_Print

///////////////////////////////////////////////////////////

float Fast_Sin(float theta)
{
	// this function uses the sin_look[] lookup table, but
	// has logic to handle negative angles as well as fractional
	// angles via interpolation, use this for a more robust
	// sin computation that the blind lookup, but with with
	// a slight hit in speed

	// convert angle to 0-359
	theta = fmodf(theta, 360);

	// make angle positive
	if (theta < 0) theta += 360.0;

	// compute floor of theta and fractional part to interpolate
	int theta_int = (int)theta;
	float theta_frac = theta - theta_int;

	// now compute the value of sin(angle) using the lookup tables
	// and interpolating the fractional part, note that if theta_int
	// is equal to 359 then theta_int+1=360, but this is fine since the
	// table was made with the entries 0-360 inclusive
	return(sin_look[theta_int] +
		theta_frac * (sin_look[theta_int + 1] - sin_look[theta_int]));

} // end Fast_Sin

///////////////////////////////////////////////////////////////

float Fast_Cos(float theta)
{
	// this function uses the cos_look[] lookup table, but
	// has logic to handle negative angles as well as fractional
	// angles via interpolation, use this for a more robust
	// cos computation that the blind lookup, but with with
	// a slight hit in speed

	// convert angle to 0-359
	theta = fmodf(theta, 360);

	// make angle positive
	if (theta < 0) theta += 360.0;

	// compute floor of theta and fractional part to interpolate
	int theta_int = (int)theta;
	float theta_frac = theta - theta_int;

	// now compute the value of sin(angle) using the lookup tables
	// and interpolating the fractional part, note that if theta_int
	// is equal to 359 then theta_int+1=360, but this is fine since the
	// table was made with the entries 0-360 inclusive
	return(cos_look[theta_int] +
		theta_frac * (cos_look[theta_int + 1] - cos_look[theta_int]));

} // end Fast_Cos

///////////////////////////////////////////////////////////////

void POLAR2D_To_POINT2D(POLAR2D_PTR polar, POINT2D_PTR rect)
{
	// convert polar to rectangular
	rect->x = polar->r * cosf(polar->theta);
	rect->y = polar->r * sinf(polar->theta);

} // end POLAR2D_To_POINT2D

////////////////////////////////////////////////////////////////

void POLAR2D_To_RectXY(POLAR2D_PTR polar, float *x, float *y)
{
	// convert polar to rectangular
	*x = polar->r * cosf(polar->theta);
	*y = polar->r * sinf(polar->theta);

} // end POLAR2D_To_RectXY

///////////////////////////////////////////////////////////////

void POINT2D_To_POLAR2D(POINT2D_PTR rect, POLAR2D_PTR polar)
{
	// convert rectangular to polar
	polar->r = sqrtf((rect->x * rect->x) + (rect->y * rect->y));
	polar->theta = atanf(rect->y / rect->x);

} // end POINT2D_To_POLAR2D

////////////////////////////////////////////////////////////

void POINT2D_To_PolarRTh(POINT2D_PTR rect, float *r, float *theta)
{
	// convert rectangular to polar
	*r = sqrtf((rect->x * rect->x) + (rect->y * rect->y));
	*theta = atanf(rect->y / rect->x);

} // end POINT2D_To_PolarRTh

///////////////////////////////////////////////////////////////

void CYLINDRICAL3D_To_POINT3D(CYLINDRICAL3D_PTR cyl, POINT3D_PTR rect)
{
	// convert cylindrical to rectangular
	rect->x = cyl->r * cosf(cyl->theta);
	rect->y = cyl->r * sinf(cyl->theta);
	rect->z = cyl->z;

} // end CYLINDRICAL3D_To_POINT3D

////////////////////////////////////////////////////////////////

void CYLINDRICAL3D_To_RectXYZ(CYLINDRICAL3D_PTR cyl,
	float *x, float *y, float *z)
{
	// convert cylindrical to rectangular
	*x = cyl->r * cosf(cyl->theta);
	*y = cyl->r * sinf(cyl->theta);
	*z = cyl->z;

} // end CYLINDRICAL3D_To_RectXYZ

///////////////////////////////////////////////////////////////

void POINT3D_To_CYLINDRICAL3D(POINT3D_PTR rect,
	CYLINDRICAL3D_PTR cyl)
{
	// convert rectangular to cylindrical
	cyl->r = sqrtf((rect->x * rect->x) + (rect->y * rect->y));
	cyl->theta = atanf(rect->y / rect->x);
	cyl->z = rect->z;

} // end POINT3D_To_CYLINDRICAL3D

///////////////////////////////////////////////////////////////

void POINT3D_To_CylindricalRThZ(POINT3D_PTR rect,
	float *r, float *theta, float *z)
{
	// convert rectangular to cylindrical
	*r = sqrtf((rect->x * rect->x) + (rect->y * rect->y));
	*theta = atanf(rect->y / rect->x);
	*z = rect->z;

} // end POINT3D_To_CylindricalRThZ

///////////////////////////////////////////////////////////////

void SPHERICAL3D_To_POINT3D(SPHERICAL3D_PTR sph, POINT3D_PTR rect)
{
	// convert spherical to rectangular
	float r;

	// pre-compute r, and z
	r = sph->p * sinf(sph->phi);
	rect->z = sph->p * cosf(sph->phi);

	// use r to simplify computation of x,y
	rect->x = r * cosf(sph->theta);
	rect->y = r * sinf(sph->theta);

} // end SPHERICAL3D_To_POINT3D

////////////////////////////////////////////////////////////////

void SPHERICAL3D_To_RectXYZ(SPHERICAL3D_PTR sph,
	float *x, float *y, float *z)
{
	// convert spherical to rectangular

	float r;

	// pre-compute r, and z
	r = sph->p * sinf(sph->phi);
	*z = sph->p * cosf(sph->phi);

	// use r to simplify computation of x,y
	*x = r * cosf(sph->theta);
	*y = r * sinf(sph->theta);

} // end SPHERICAL3D_To_RectXYZ

///////////////////////////////////////////////////////////

void POINT3D_To_SPHERICAL3D(POINT3D_PTR rect, SPHERICAL3D_PTR sph)
{
	// convert rectangular to spherical
	sph->p = sqrtf((rect->x * rect->x) + (rect->y * rect->y) + (rect->z * rect->z));

	sph->theta = atanf(rect->y / rect->x);

	// we need r to compute phi
	float r = sph->p * sinf(sph->phi);

	sph->phi = asinf(r / sph->p);

} // end POINT3D_To_CYLINDRICAL3D

////////////////////////////////////////////////////////////

void POINT3D_To_SphericalPThPh(POINT3D_PTR rect,
	float *p, float *theta, float *phi)
{
	// convert rectangular to spherical
	*p = sqrtf((rect->x * rect->x) + (rect->y * rect->y) + (rect->z * rect->z));
	*theta = atanf(rect->y / rect->x);

	// we need r to compute phi
	float r = sqrtf((rect->x * rect->x) + (rect->y * rect->y));
	*phi = asinf(r / (*p));

} // end POINT3D_To_SphericalPThPh

/////////////////////////////////////////////////////////////

void VECTOR2D_Add(VECTOR2D_PTR va,
	VECTOR2D_PTR vb,
	VECTOR2D_PTR vsum)
{
	// this function adds va+vb and return it in vsum
	vsum->x = va->x + vb->x;
	vsum->y = va->y + vb->y;

} // end VECTOR2D_Add

////////////////////////////////////////////////////////////

VECTOR2D VECTOR2D_Add(VECTOR2D_PTR va,
	VECTOR2D_PTR vb)
{
	// this function adds va+vb and returns the result on 
	// the stack
	VECTOR2D vsum;

	vsum.x = va->x + vb->x;
	vsum.y = va->y + vb->y;

	// return result
	return(vsum);

} // end VECTOR2D_Add

////////////////////////////////////////////////////////////

void VECTOR2D_Sub(VECTOR2D_PTR va,
	VECTOR2D_PTR vb,
	VECTOR2D_PTR vdiff)
{
	// this function subtracts va-vb and return it in vdiff
	// the stack
	vdiff->x = va->x - vb->x;
	vdiff->y = va->y - vb->y;

} // end VECTOR2D_Sub

////////////////////////////////////////////////////////////

VECTOR2D VECTOR2D_Sub(VECTOR2D_PTR va,
	VECTOR2D_PTR vb)
{
	// this function subtracts va-vb and returns the result on 
	// the stack
	VECTOR2D vdiff;

	vdiff.x = va->x - vb->x;
	vdiff.y = va->y - vb->y;

	// return result
	return(vdiff);

} // end VECTOR2D_Sub

////////////////////////////////////////////////////////////

void VECTOR2D_Scale(float k,
	VECTOR2D_PTR va,
	VECTOR2D_PTR vscaled)
{
	// this function scales a vector by the constant k,
	// leaves the original unchanged, and returns the result
	// in vscaled

	// multiply each component by scaling factor
	vscaled->x = k * va->x;
	vscaled->y = k * va->y;

} // end VECTOR2D_Scale

/////////////////////////////////////////////////////////////

void VECTOR2D_Scale(float k, VECTOR2D_PTR va)
{
	// this function scales a vector by the constant k,
	// by scaling the original

	// multiply each component by scaling factor
	va->x *= k;
	va->y *= k;

} // end VECTOR2D_Scale

//////////////////////////////////////////////////////////////

float VECTOR2D_Dot(VECTOR2D_PTR va, VECTOR2D_PTR vb)
{
	// computes the dot product between va and vb
	return((va->x * vb->x) + (va->y * vb->y));
} // end VECTOR2D_Dot

///////////////////////////////////////////////////////////////

float VECTOR2D_Length(VECTOR2D_PTR va)
{
	// computes the magnitude of a vector, slow

	return(sqrtf(va->x * va->x + va->y * va->y));

} // end VECTOR2D_Length

///////////////////////////////////////////////////////////////

float VECTOR2D_Length_Fast(VECTOR2D_PTR va)
{
	// computes the magnitude of a vector using an approximation
	// very fast
	return((float)Fast_Distance_2D(va->x, va->y));

} // end VECTOR2D_Length_Fast

///////////////////////////////////////////////////////////////

void VECTOR2D_Normalize(VECTOR2D_PTR va)
{
	// normalizes the sent vector in place

	// compute length
	float length = sqrtf(va->x * va->x + va->y * va->y);

	// test for zero length vector
	// if found return zero vector
	if (length < EPSILON_E5)
		return;

	float length_inv = 1 / length;

	// compute normalized version of vector
	va->x = va->x * length_inv;
	va->y = va->y * length_inv;

} // end VECTOR2D_Normalize

///////////////////////////////////////////////////////////////

void VECTOR2D_Normalize(VECTOR2D_PTR va, VECTOR2D_PTR vn)
{
	// normalizes the sent vector and returns the result in vn

	VECTOR2D_ZERO(vn);

	// compute length
	float length = (float)sqrtf(va->x * va->x + va->y * va->y);

	// test for zero length vector
	// if found return zero vector
	if (length < EPSILON_E5)
		return;

	float length_inv = 1 / length;

	// compute normalized version of vector
	vn->x = va->x * length_inv;
	vn->y = va->y * length_inv;

} // end VECTOR2D_Normalize

///////////////////////////////////////////////////////////////

void VECTOR2D_Build(VECTOR2D_PTR init,
	VECTOR2D_PTR term,
	VECTOR2D_PTR result)
{
	// this function creates a vector from two vectors (or points)
	//  in 3D space

	result->x = term->x - init->x;
	result->y = term->y - init->y;

} // end VECTOR2D_Build

///////////////////////////////////////////////////////////////

float VECTOR2D_CosTh(VECTOR2D_PTR va, VECTOR2D_PTR vb)
{
	// this function returns the cosine of the angle between
	// two vectors. Note, we could compute the actual angle,
	// many many times, in further calcs we will want ultimately
	// compute cos of the angle, so why not just leave it!
	return(VECTOR2D_Dot(va, vb) / (VECTOR2D_Length(va) * VECTOR2D_Length(vb)));

} // end VECTOR2D_CosTh

//////////////////////////////////////////////////////////////

void VECTOR2D_Print(VECTOR2D_PTR va, const char *name = "v")
{
	// this function prints out a VECTOR2D

	Write_Error("\n%s=[", name);
	for (int index = 0; index < 2; index++)
		Write_Error("%f, ", va->M[index]);
	Write_Error("]");
} // end VECTOR2D_Print

/////////////////////////////////////////////////////////////

void VECTOR3D_Add(VECTOR3D_PTR va,
	VECTOR3D_PTR vb,
	VECTOR3D_PTR vsum)
{
	// this function adds va+vb and return it in vsum
	vsum->x = va->x + vb->x;
	vsum->y = va->y + vb->y;
	vsum->z = va->z + vb->z;

} // end VECTOR3D_Add

////////////////////////////////////////////////////////////

VECTOR3D VECTOR3D_Add(VECTOR3D_PTR va,
	VECTOR3D_PTR vb)
{
	// this function adds va+vb and returns the result on 
	// the stack
	VECTOR3D vsum;

	vsum.x = va->x + vb->x;
	vsum.y = va->y + vb->y;
	vsum.z = va->z + vb->z;

	// return result
	return(vsum);

} // end VECTOR3D_Add

////////////////////////////////////////////////////////////

void VECTOR3D_Sub(VECTOR3D_PTR va,
	VECTOR3D_PTR vb,
	VECTOR3D_PTR vdiff)
{
	// this function subtracts va-vb and return it in vdiff
	// the stack
	vdiff->x = va->x - vb->x;
	vdiff->y = va->y - vb->y;
	vdiff->z = va->z - vb->z;

} // end VECTOR3D_Sub

////////////////////////////////////////////////////////////

VECTOR3D VECTOR3D_Sub(VECTOR3D_PTR va, VECTOR3D_PTR vb)
{
	// this function subtracts va-vb and returns the result on 
	// the stack
	VECTOR3D vdiff;

	vdiff.x = va->x - vb->x;
	vdiff.y = va->y - vb->y;
	vdiff.z = va->z - vb->z;

	// return result
	return(vdiff);

} // end VECTOR3D_Sub

////////////////////////////////////////////////////////////

void VECTOR3D_Scale(float k, VECTOR3D_PTR va)
{
	// this function scales a vector by the constant k,
	// and modifies the original

	// multiply each component by scaling factor
	va->x *= k;
	va->y *= k;
	va->z *= k;

} // end VECTOR3D_Scale

/////////////////////////////////////////////////////////////

void VECTOR3D_Scale(float k, VECTOR3D_PTR va, VECTOR3D_PTR vscaled)
{
	// this function scales a vector by the constant k,
	// leaves the original unchanged, and returns the result
	// in vres as well as on the stack

	// multiply each component by scaling factor
	vscaled->x = k * va->x;
	vscaled->y = k * va->y;
	vscaled->z = k * va->z;

} // end VECTOR3D_Scale

//////////////////////////////////////////////////////////////

float VECTOR3D_Dot(VECTOR3D_PTR va, VECTOR3D_PTR vb)
{
	// computes the dot product between va and vb
	return((va->x * vb->x) + (va->y * vb->y) + (va->z * vb->z));
} // end VECTOR3D_DOT
/////////////////////////////////////////////////////////////

void VECTOR3D_Cross(VECTOR3D_PTR va, VECTOR3D_PTR vb, VECTOR3D_PTR vn)
{
	// this function computes the cross product between va and vb
	// and returns the vector that is perpendicular to each in vn

	vn->x = ((va->y * vb->z) - (va->z * vb->y));
	vn->y = -((va->x * vb->z) - (va->z * vb->x));
	vn->z = ((va->x * vb->y) - (va->y * vb->x));

} // end VECTOR3D_Cross

/////////////////////////////////////////////////////////////

VECTOR3D VECTOR3D_Cross(VECTOR3D_PTR va, VECTOR3D_PTR vb)
{
	// this function computes the cross product between va and vb
	// and returns the vector that is perpendicular to each

	VECTOR3D vn;

	vn.x = ((va->y * vb->z) - (va->z * vb->y));
	vn.y = -((va->x * vb->z) - (va->z * vb->x));
	vn.z = ((va->x * vb->y) - (va->y * vb->x));

	// return result
	return(vn);

} // end VECTOR3D_Cross

//////////////////////////////////////////////////////////////

float VECTOR3D_Length(VECTOR3D_PTR va)
{
	// computes the magnitude of a vector, slow

	return((float)sqrtf(va->x * va->x + va->y * va->y + va->z * va->z));

} // end VECTOR3D_Length

///////////////////////////////////////////////////////////////

float VECTOR3D_Length_Fast(VECTOR3D_PTR va)
{
	// computes the magnitude of a vector using an approximation
	// very fast
	return(Fast_Distance_3D(va->x, va->y, va->z));

} // end VECTOR3D_Length_Fast

///////////////////////////////////////////////////////////////

void VECTOR3D_Normalize(VECTOR3D_PTR va)
{
	// normalizes the sent vector in placew

	// compute length
	float length = sqrtf(va->x * va->x + va->y * va->y + va->z * va->z);

	// test for zero length vector
	// if found return zero vector
	if (length < EPSILON_E5)
		return;

	float length_inv = 1 / length;

	// compute normalized version of vector
	va->x *= length_inv;
	va->y *= length_inv;
	va->z *= length_inv;

} // end VECTOR3D_Normalize

///////////////////////////////////////////////////////////////

void VECTOR3D_Normalize(VECTOR3D_PTR va, VECTOR3D_PTR vn)
{
	// normalizes the sent vector and returns the result in vn

	VECTOR3D_ZERO(vn);

	// compute length
	float length = VECTOR3D_Length(va);

	// test for zero length vector
	// if found return zero vector
	if (length < EPSILON_E5)
		return;

	float length_inv = 1.0 / length;

	// compute normalized version of vector
	vn->x = va->x * length_inv;
	vn->y = va->y * length_inv;
	vn->z = va->z * length_inv;

} // end VECTOR3D_Normalize

///////////////////////////////////////////////////////////////

void VECTOR3D_Build(VECTOR3D_PTR init, VECTOR3D_PTR term, VECTOR3D_PTR result)
{
	// this function creates a vector from two vectors (or points)
	//  in 3D space
	result->x = term->x - init->x;
	result->y = term->y - init->y;
	result->z = term->z - init->z;

} // end VECTOR3D_Build

/////////////////////////////////////////////////////////////

float VECTOR3D_CosTh(VECTOR3D_PTR va, VECTOR3D_PTR vb)
{
	// this function returns the cosine of the angle between
	// two vectors. Note, we could compute the actual angle,
	// many many times, in further calcs we will want ultimately
	// compute cos of the angle, so why not just leave it!
	return(VECTOR3D_Dot(va, vb) / (VECTOR3D_Length(va) * VECTOR3D_Length(vb)));

} // end VECTOR3D_CosTh

///////////////////////////////////////////////////////////////

void VECTOR3D_Print(VECTOR3D_PTR va, const char *name = "v")
{
	// this function prints out a VECTOR3D

	Write_Error("\n%s=[", name);
	for (int index = 0; index < 3; index++)
		Write_Error("%f, ", va->M[index]);
	Write_Error("]");

} // end VECTOR3D_Print

////////////////////////////////////////////////////////////////

// these are the 4D version of the vector functions, they
// assume that the vectors are 3D with a w, so w is left
// out of all the operations

void VECTOR4D_Build(VECTOR4D_PTR init, VECTOR4D_PTR term, VECTOR4D_PTR result)
{
	// build a 4d vector
	result->x = term->x - init->x;
	result->y = term->y - init->y;
	result->z = term->z - init->z;
	result->w = 1;

} // end VECTOR4D_Build

////////////////////////////////////////////////////////////////

void VECTOR4D_Add(VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vsum)
{
	// this function adds va+vb and return it in vsum
	vsum->x = va->x + vb->x;
	vsum->y = va->y + vb->y;
	vsum->z = va->z + vb->z;
	vsum->w = 1;

} // end VECTOR4D_Add

////////////////////////////////////////////////////////////

VECTOR4D VECTOR4D_Add(VECTOR4D_PTR va, VECTOR4D_PTR vb)
{
	// this function adds va+vb and returns the result on 
	// the stack
	VECTOR4D vsum;

	vsum.x = va->x + vb->x;
	vsum.y = va->y + vb->y;
	vsum.z = va->z + vb->z;
	vsum.w = 1;

	// return result
	return(vsum);

} // end VECTOR4D_Add

////////////////////////////////////////////////////////////

void VECTOR4D_Sub(VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vdiff)
{
	// this function subtracts va-vb and return it in vdiff
	// the stack
	vdiff->x = va->x - vb->x;
	vdiff->y = va->y - vb->y;
	vdiff->z = va->z - vb->z;
	vdiff->w = 1;

} // end VECTOR4D_Sub

////////////////////////////////////////////////////////////

VECTOR4D VECTOR4D_Sub(VECTOR4D_PTR va, VECTOR4D_PTR vb)
{
	// this function subtracts va-vb and returns the result on 
	// the stack
	VECTOR4D vdiff;

	vdiff.x = va->x - vb->x;
	vdiff.y = va->y - vb->y;
	vdiff.z = va->z - vb->z;
	vdiff.w = 1;

	// return result
	return(vdiff);

} // end VECTOR4D_Sub

////////////////////////////////////////////////////////////

void VECTOR4D_Scale(float k, VECTOR4D_PTR va)
{
	// this function scales a vector by the constant k,
	// in place , note w is left unchanged

	// multiply each component by scaling factor
	va->x *= k;
	va->y *= k;
	va->z *= k;
	va->w = 1;

} // end VECTOR4D_Scale

/////////////////////////////////////////////////////////////

void VECTOR4D_Scale(float k, VECTOR4D_PTR va, VECTOR4D_PTR vscaled)
{
	// this function scales a vector by the constant k,
	// leaves the original unchanged, and returns the result
	// in vres as well as on the stack

	// multiply each component by scaling factor
	vscaled->x = k * va->x;
	vscaled->y = k * va->y;
	vscaled->z = k * va->z;
	vscaled->w = 1;

} // end VECTOR4D_Scale

//////////////////////////////////////////////////////////////

float VECTOR4D_Dot(VECTOR4D_PTR va, VECTOR4D_PTR vb)
{
	// computes the dot product between va and vb
	return((va->x * vb->x) + (va->y * vb->y) + (va->z * vb->z));
} // end VECTOR4D_DOT

/////////////////////////////////////////////////////////////

void VECTOR4D_Cross(VECTOR4D_PTR va,
	VECTOR4D_PTR vb,
	VECTOR4D_PTR vn)
{
	// this function computes the cross product between va and vb
	// and returns the vector that is perpendicular to each in vn

	vn->x = ((va->y * vb->z) - (va->z * vb->y));
	vn->y = -((va->x * vb->z) - (va->z * vb->x));
	vn->z = ((va->x * vb->y) - (va->y * vb->x));
	vn->w = 1;

} // end VECTOR4D_Cross

/////////////////////////////////////////////////////////////

VECTOR4D VECTOR4D_Cross(VECTOR4D_PTR va, VECTOR4D_PTR vb)
{
	// this function computes the cross product between va and vb
	// and returns the vector that is perpendicular to each

	VECTOR4D vn;

	vn.x = ((va->y * vb->z) - (va->z * vb->y));
	vn.y = -((va->x * vb->z) - (va->z * vb->x));
	vn.z = ((va->x * vb->y) - (va->y * vb->x));
	vn.w = 1;

	// return result
	return(vn);

} // end VECTOR4D_Cross

//////////////////////////////////////////////////////////////

float VECTOR4D_Length(VECTOR4D_PTR va)
{
	// computes the magnitude of a vector, slow

	return(sqrtf(va->x * va->x + va->y * va->y + va->z * va->z));

} // end VECTOR4D_Length

///////////////////////////////////////////////////////////////

float VECTOR4D_Length_Fast(VECTOR4D_PTR va)
{
	// computes the magnitude of a vector using an approximation
	// very fast
	return(Fast_Distance_3D(va->x, va->y, va->z));

} // end VECTOR4D_Length_Fast

///////////////////////////////////////////////////////////////

void VECTOR4D_Normalize(VECTOR4D_PTR va)
{
	// normalizes the sent vector and returns the result

	// compute length
	float length = sqrtf(va->x * va->x + va->y * va->y + va->z * va->z);

	// test for zero length vector
	// if found return zero vector
	if (length < EPSILON_E5)
		return;

	float length_inv = 1.0 / length;

	// compute normalized version of vector
	va->x *= length_inv;
	va->y *= length_inv;
	va->z *= length_inv;
	va->w = 1;

} // end VECTOR4D_Normalize

///////////////////////////////////////////////////////////////

void VECTOR4D_Normalize(VECTOR4D_PTR va, VECTOR4D_PTR vn)
{
	// normalizes the sent vector and returns the result in vn

	VECTOR4D_ZERO(vn);

	// compute length
	float length = sqrt(va->x * va->x + va->y * va->y + va->z * va->z);

	// test for zero length vector
	// if found return zero vector
	if (length < EPSILON_E5)
		return;

	float length_inv = 1.0 / length;

	// compute normalized version of vector
	vn->x = va->x * length_inv;
	vn->y = va->y * length_inv;
	vn->z = va->z * length_inv;
	vn->w = 1;

} // end VECTOR4D_Normalize

///////////////////////////////////////////////////////////////

float VECTOR4D_CosTh(VECTOR4D_PTR va, VECTOR4D_PTR vb)
{
	// this function returns the cosine of the angle between
	// two vectors. Note, we could compute the actual angle,
	// many many times, in further calcs we will want ultimately
	// compute cos of the angle, so why not just leave it!
	return(VECTOR4D_Dot(va, vb) / (VECTOR4D_Length(va) * VECTOR4D_Length(vb)));

} // end VECTOR4D_CosTh

////////////////////////////////////////////////////////////

void VECTOR4D_Print(VECTOR4D_PTR va, const char *name = "v")
{
	// this function prints out a VECTOR4D

	Write_Error("\n%s[", name);
	for (int index = 0; index < 4; index++)
		Write_Error("%f, ", va->M[index]);
	Write_Error("]");

} // end VECTOR4D_Print

////////////////////////////////////////////////////////////////

void Mat_Init_2X2(MATRIX2X2_PTR ma,
	float m00, float m01,
	float m10, float m11)

{
	// this function fills a 2x2 matrix with the sent data in 
	// row major form
	ma->M00 = m00; ma->M01 = m01;
	ma->M10 = m10; ma->M11 = m11;

} // end Mat_Init_2X2

/////////////////////////////////////////////////////////////////

void Mat_Add_2X2(MATRIX2X2_PTR ma, MATRIX2X2_PTR mb, MATRIX2X2_PTR msum)
{
	// this function adds two 2x2 matrices together and stores
	// the result in msum
	msum->M00 = ma->M00 + mb->M00;
	msum->M01 = ma->M01 + mb->M01;
	msum->M10 = ma->M10 + mb->M10;
	msum->M11 = ma->M11 + mb->M11;

} // end Mat_Add_2X2

/////////////////////////////////////////////////////////////////

void Mat_Mul_2X2(MATRIX2X2_PTR ma, MATRIX2X2_PTR mb, MATRIX2X2_PTR mprod)
{
	// this function multiplies two 2x2 matrices together and 
	// and stores the result in mprod
	mprod->M00 = ma->M00 * mb->M00 + ma->M01 * mb->M10;
	mprod->M01 = ma->M00 * mb->M01 + ma->M01 * mb->M11;

	mprod->M10 = ma->M10 * mb->M00 + ma->M11 * mb->M10;
	mprod->M11 = ma->M10 * mb->M01 + ma->M11 * mb->M11;

} // end Mat_Mul_2X2

/////////////////////////////////////////////////////////////////

int Mat_Inverse_2X2(MATRIX2X2_PTR m, MATRIX2X2_PTR mi)
{
	// this function computes the inverse of a 2x2 matrix
	// and stores the result in mi

	// compute determinate
	float det = (m->M00 * m->M11 - m->M01 * m->M10);

	// if determinate is 0 then inverse doesn't exist
	if (fabs(det) < EPSILON_E5)
		return(0);

	float det_inv = 1.0 / det;

	// fill in inverse by formula
	mi->M00 = m->M11 * det_inv;
	mi->M01 = -m->M01 * det_inv;
	mi->M10 = -m->M10 * det_inv;
	mi->M11 = m->M00 * det_inv;

	// return sucess
	return(1);

} // end Mat_Inverse_2X2

/////////////////////////////////////////////////////////////////

void Print_Mat_2X2(MATRIX2X2_PTR ma, const char *name = "M")
{
	// prints out a 3x3 matrix
	Write_Error("\n%s=\n", name);

	for (int r = 0; r < 2; r++, Write_Error("\n"))
		for (int c = 0; c < 2; c++)
			Write_Error("%f ", ma->M[r][c]);

} // end Print_Mat_2X2

//////////////////////////////////////////////////////////////////

float Mat_Det_2X2(MATRIX2X2_PTR m)
{
	// computes the determinate of a 2x2 matrix

	return(m->M00 * m->M11 - m->M01 * m->M10);
} // end Mat_Det_2X2

///////////////////////////////////////////////////////////////

int Solve_2X2_System(MATRIX2X2_PTR A, MATRIX1X2_PTR X, MATRIX1X2_PTR B)
{
	// solves the system AX=B and computes X=A(-1)*B
	// by using cramers rule and determinates

	// step 1: compute determinate of A
	float det_A = Mat_Det_2X2(A);

	// test if det(a) is zero, if so then there is no solution
	if (fabs(det_A) < EPSILON_E5)
		return(0);

	// step 2: create x,y numerator matrices by taking A and
	// replacing each column of it with B(transpose) and solve
	MATRIX2X2 work_mat; // working matrix

	// solve for x /////////////////

	// copy A into working matrix
	MAT_COPY_2X2(A, &work_mat);

	// swap out column 0 (x column)
	MAT_COLUMN_SWAP_2X2(&work_mat, 0, B);

	// compute determinate of A with B swapped into x column
	float det_ABx = Mat_Det_2X2(&work_mat);

	// now solve for X00
	X->M00 = det_ABx / det_A;

	// solve for y /////////////////

	// copy A into working matrix
	MAT_COPY_2X2(A, &work_mat);

	// swap out column 1 (y column)
	MAT_COLUMN_SWAP_2X2(&work_mat, 1, B);

	// compute determinate of A with B swapped into y column
	float det_ABy = Mat_Det_2X2(&work_mat);

	// now solve for X01
	X->M01 = det_ABy / det_A;

	// return success
	return(1);

} // end Solve_2X2_System

///////////////////////////////////////////////////////////////

void Mat_Add_3X3(MATRIX3X3_PTR ma,
	MATRIX3X3_PTR mb,
	MATRIX3X3_PTR msum)
{
	// this function adds two 3x3 matrices together and 
	// and stores the result

	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			// insert resulting row,col element
			msum->M[row][col] = ma->M[row][col] + mb->M[row][col];
		} // end for col

	} // end for row

} // end Mat_Add_3X3

////////////////////////////////////////////////////////////////////

void Mat_Mul_VECTOR3D_3X3(VECTOR3D_PTR  va,
	MATRIX3X3_PTR mb,
	VECTOR3D_PTR  vprod)
{
	// this function multiplies a VECTOR3D against a 
	// 3x3 matrix - ma*mb and stores the result in mprod

	for (int col = 0; col < 3; col++) {
		// compute dot product from row of ma 
		// and column of mb
		float sum = 0; // used to hold result

		for (int row = 0; row < 3; row++) {
			// add in next product pair
			sum += (va->M[row] * mb->M[row][col]);
		} // end for index

		// insert resulting col element
		vprod->M[col] = sum;
	} // end for col
} // end Mat_Mul_VECTOR3D_3X3

////////////////////////////////////////////////////////////////////

void Mat_Init_3X3(MATRIX3X3_PTR ma,
	float m00, float m01, float m02,
	float m10, float m11, float m12,
	float m20, float m21, float m22)
{
	// this function fills a 3x3 matrix with the sent data in row major form

	ma->M00 = m00; ma->M01 = m01; ma->M02 = m02;
	ma->M10 = m10; ma->M11 = m11; ma->M12 = m12;
	ma->M20 = m20; ma->M21 = m21; ma->M22 = m22;

} // end Mat_Init_3X3

/////////////////////////////////////////////////////////////////

int Mat_Inverse_3X3(MATRIX3X3_PTR m, MATRIX3X3_PTR mi)
{
	// this function computes the inverse of a 3x3

	// first compute the determinate to see if there is 
	// an inverse
	float det = m->M00 * (m->M11 * m->M22 - m->M21 * m->M12) -
		m->M01 * (m->M10 * m->M22 - m->M20 * m->M12) +
		m->M02 * (m->M10 * m->M21 - m->M20 * m->M11);

	if (fabs(det) < EPSILON_E5)
		return(0);

	// compute inverse to save divides
	float det_inv = 1.0 / det;

	// compute inverse using m-1 = adjoint(m)/det(m)
	mi->M00 = det_inv * (m->M11 * m->M22 - m->M21 * m->M12);
	mi->M10 = -det_inv * (m->M10 * m->M22 - m->M20 * m->M12);
	mi->M20 = det_inv * (m->M10 * m->M21 - m->M20 * m->M11);

	mi->M01 = -det_inv * (m->M01 * m->M22 - m->M21 * m->M02);
	mi->M11 = det_inv * (m->M00 * m->M22 - m->M20 * m->M02);
	mi->M21 = -det_inv * (m->M00 * m->M21 - m->M20 * m->M01);

	mi->M02 = det_inv * (m->M01 * m->M12 - m->M11 * m->M02);
	mi->M12 = -det_inv * (m->M00 * m->M12 - m->M10 * m->M02);
	mi->M22 = det_inv * (m->M00 * m->M11 - m->M10 * m->M01);

	// return success
	return(1);

} // end Mat_Inverse_3X3

/////////////////////////////////////////////////////////////////

void Print_Mat_3X3(MATRIX3X3_PTR ma, const char *name = "M")
{
	// prints out a 3x3 matrix
	Write_Error("\n%s=\n", name);

	for (int r = 0; r < 3; r++, Write_Error("\n"))
		for (int c = 0; c < 3; c++)
			Write_Error("%f ", ma->M[r][c]);

} // end Print_Mat_3X3

/////////////////////////////////////////////////////////////////

float Mat_Det_3X3(MATRIX3X3_PTR m)
{
	// computes the determinate of a 3x3 matrix using co-factor
	// expansion

	return(m->M00 * (m->M11 * m->M22 - m->M21 * m->M12) -
		m->M01 * (m->M10 * m->M22 - m->M20 * m->M12) +
		m->M02 * (m->M10 * m->M21 - m->M20 * m->M11));

} // end Mat_Det_3X3

///////////////////////////////////////////////////////////////

int Solve_3X3_System(MATRIX3X3_PTR A, MATRIX1X3_PTR X, MATRIX1X3_PTR B)
{
	// solves the system AX=B and computes X=A(-1)*B
	// by using cramers rule and determinates

	// step 1: compute determinate of A
	float det_A = Mat_Det_3X3(A);

	// test if det(a) is zero, if so then there is no solution
	if (fabs(det_A) < EPSILON_E5)
		return(0);

	// step 2: create x,y,z numerator matrices by taking A and
	// replacing each column of it with B(transpose) and solve
	MATRIX3X3 work_mat; // working matrix

	// solve for x /////////////////

	// copy A into working matrix
	MAT_COPY_3X3(A, &work_mat);

	// swap out column 0 (x column)
	MAT_COLUMN_SWAP_3X3(&work_mat, 0, B);

	// compute determinate of A with B swapped into x column
	float det_ABx = Mat_Det_3X3(&work_mat);

	// now solve for X00
	X->M00 = det_ABx / det_A;

	// solve for y /////////////////

	// copy A into working matrix
	MAT_COPY_3X3(A, &work_mat);

	// swap out column 1 (y column)
	MAT_COLUMN_SWAP_3X3(&work_mat, 1, B);

	// compute determinate of A with B swapped into y column
	float det_ABy = Mat_Det_3X3(&work_mat);

	// now solve for X01
	X->M01 = det_ABy / det_A;

	// solve for z /////////////////

	// copy A into working matrix
	MAT_COPY_3X3(A, &work_mat);

	// swap out column 2 (z column)
	MAT_COLUMN_SWAP_3X3(&work_mat, 2, B);

	// compute determinate of A with B swapped into z column
	float det_ABz = Mat_Det_3X3(&work_mat);

	// now solve for X02
	X->M02 = det_ABz / det_A;

	// return success
	return(1);

} // end Solve_3X3_System

///////////////////////////////////////////////////////////////

void Mat_Add_4X4(MATRIX4X4_PTR ma,
	MATRIX4X4_PTR mb,
	MATRIX4X4_PTR msum)
{
	// this function adds two 4x4 matrices together and 
	// and stores the result

	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			// insert resulting row,col element
			msum->M[row][col] = ma->M[row][col] + mb->M[row][col];
		} // end for col

	} // end for row

} // end Mat_Add_4X4

///////////////////////////////////////////////////////////////

void Mat_Mul_4X4(MATRIX4X4_PTR ma,
	MATRIX4X4_PTR mb,
	MATRIX4X4_PTR mprod)
{
	// this function multiplies two 4x4 matrices together and 
	// and stores the result in mprod
	// note later we will take advantage of the fact that we know
	// that w=1 always, and that the last column of a 4x4 is
	// always 0

	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			// compute dot product from row of ma 
			// and column of mb

			float sum = 0; // used to hold result

			for (int index = 0; index < 4; index++) {
				// add in next product pair
				sum += (ma->M[row][index] * mb->M[index][col]);
			} // end for index

			// insert resulting row,col element
			mprod->M[row][col] = sum;

		} // end for col

	} // end for row

} // end Mat_Mul_4X4

////////////////////////////////////////////////////////////////

void Mat_Mul_1X4_4X4(MATRIX1X4_PTR ma,
	MATRIX4X4_PTR mb,
	MATRIX1X4_PTR mprod)
{
	// this function multiplies a 1x4 matrix against a 
	// 4x4 matrix - ma*mb and stores the result
	// no tricks or assumptions here, just a straight multiply

	for (int col = 0; col < 4; col++) {
		// compute dot product from row of ma 
		// and column of mb
		float sum = 0; // used to hold result

		for (int row = 0; row < 4; row++) {
			// add in next product pair
			sum += (ma->M[row] * mb->M[row][col]);
		} // end for index

		// insert resulting col element
		mprod->M[col] = sum;

	} // end for col

} // end Mat_Mul_1X4_4X4

////////////////////////////////////////////////////////////////////

void Mat_Mul_VECTOR3D_4X4(VECTOR3D_PTR  va,
	MATRIX4X4_PTR mb,
	VECTOR3D_PTR  vprod)
{
	// this function multiplies a VECTOR3D against a 
	// 4x4 matrix - ma*mb and stores the result in mprod
	// the function assumes that the vector refers to a 
	// 4D homogenous vector, thus the function assumes that
	// w=1 to carry out the multiply, also the function
	// does not carry out the last column multiply since
	// we are assuming w=1, there is no point

	for (int col = 0; col < 3; col++) {
		// compute dot product from row of ma 
		// and column of mb
		float sum = 0; // used to hold result

		int row;           // looping var

		for (row = 0; row < 3; row++) {
			// add in next product pair
			sum += (va->M[row] * mb->M[row][col]);
		} // end for index

		// add in last element in column or w*mb[3][col]
		sum += mb->M[row][col];

		// insert resulting col element
		vprod->M[col] = sum;

	} // end for col

} // end Mat_Mul_VECTOR3D_4X4

///////////////////////////////////////////////////////////////

void Mat_Mul_VECTOR3D_4X3(VECTOR3D_PTR  va,
	MATRIX4X3_PTR mb,
	VECTOR3D_PTR  vprod)
{
	// this function multiplies a VECTOR3D against a 
	// 4x3 matrix - ma*mb and stores the result in mprod
	// the function assumes that the vector refers to a 
	// 4D homogenous vector, thus the function assumes that
	// w=1 to carry out the multiply, also the function
	// does not carry out the last column multiply since
	// we are assuming w=1, there is no point

	for (int col = 0; col < 3; col++) {
		// compute dot product from row of ma 
		// and column of mb
		float sum = 0; // used to hold result

		int row;           // looping var

		for (row = 0; row < 3; row++) {
			// add in next product pair
			sum += (va->M[row] * mb->M[row][col]);
		} // end for index

		// add in last element in column or w*mb[3][col]
		sum += mb->M[row][col];

		// insert resulting col element
		vprod->M[col] = sum;

	} // end for col

} // end Mat_Mul_VECTOR3D_4X3

////////////////////////////////////////////////////////////////////

void Mat_Mul_VECTOR4D_4X4(VECTOR4D_PTR  va,
	MATRIX4X4_PTR mb,
	VECTOR4D_PTR  vprod)
{
	// this function multiplies a VECTOR4D against a 
	// 4x4 matrix - ma*mb and stores the result in mprod
	// the function makes no assumptions

	for (int col = 0; col < 4; col++) {
		// compute dot product from row of ma 
		// and column of mb
		float sum = 0; // used to hold result

		for (int row = 0; row < 4; row++) {
			// add in next product pair
			sum += (va->M[row] * mb->M[row][col]);
		} // end for index

		// insert resulting col element
		vprod->M[col] = sum;

	} // end for col

} // end Mat_Mul_VECTOR4D_4X4

////////////////////////////////////////////////////////////////////

void Mat_Mul_VECTOR4D_4X3(VECTOR4D_PTR  va,
	MATRIX4X4_PTR mb,
	VECTOR4D_PTR  vprod)
{
	// this function multiplies a VECTOR4D against a 
	// 4x3 matrix - ma*mb and stores the result in mprod
	// the function assumes that the last column of
	// mb is [0 0 0 1]t , thus w just gets replicated
	// from the vector [x y z w]

	for (int col = 0; col < 3; col++) {
		// compute dot product from row of ma 
		// and column of mb
		float sum = 0; // used to hold result

		for (int row = 0; row < 4; row++) {
			// add in next product pair
			sum += (va->M[row] * mb->M[row][col]);
		} // end for index

		// insert resulting col element
		vprod->M[col] = sum;

	} // end for col

	// copy back w element
	vprod->M[3] = va->M[3];

} // end Mat_Mul_VECTOR4D_4X3

///////////////////////////////////////////////////////////////

void Mat_Init_4X4(MATRIX4X4_PTR ma,
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)

{
	// this function fills a 4x4 matrix with the sent data in 
	// row major form
	ma->M00 = m00; ma->M01 = m01; ma->M02 = m02; ma->M03 = m03;
	ma->M10 = m10; ma->M11 = m11; ma->M12 = m12; ma->M13 = m13;
	ma->M20 = m20; ma->M21 = m21; ma->M22 = m22; ma->M23 = m23;
	ma->M30 = m30; ma->M31 = m31; ma->M32 = m32; ma->M33 = m33;

} // end Mat_Init_4X4

////////////////////////////////////////////////////////////////

int Mat_Inverse_4X4(MATRIX4X4_PTR m, MATRIX4X4_PTR mi)
{
	// computes the inverse of a 4x4, assumes that the last
	// column is [0 0 0 1]t

	float det = (m->M00 * (m->M11 * m->M22 - m->M12 * m->M21) -
		m->M01 * (m->M10 * m->M22 - m->M12 * m->M20) +
		m->M02 * (m->M10 * m->M21 - m->M11 * m->M20));

	// test determinate to see if it's 0
	if (fabs(det) < EPSILON_E5)
		return(0);

	float det_inv = 1.0f / det;

	mi->M00 = det_inv * (m->M11 * m->M22 - m->M12 * m->M21);
	mi->M01 = -det_inv * (m->M01 * m->M22 - m->M02 * m->M21);
	mi->M02 = det_inv * (m->M01 * m->M12 - m->M02 * m->M11);
	mi->M03 = 0.0f; // always 0

	mi->M10 = -det_inv * (m->M10 * m->M22 - m->M12 * m->M20);
	mi->M11 = det_inv * (m->M00 * m->M22 - m->M02 * m->M20);
	mi->M12 = -det_inv * (m->M00 * m->M12 - m->M02 * m->M10);
	mi->M13 = 0.0f; // always 0

	mi->M20 = det_inv * (m->M10 * m->M21 - m->M11 * m->M20);
	mi->M21 = -det_inv * (m->M00 * m->M21 - m->M01 * m->M20);
	mi->M22 = det_inv * (m->M00 * m->M11 - m->M01 * m->M10);
	mi->M23 = 0.0f; // always 0

	mi->M30 = -(m->M30 * mi->M00 + m->M31 * mi->M10 + m->M32 * mi->M20);
	mi->M31 = -(m->M30 * mi->M01 + m->M31 * mi->M11 + m->M32 * mi->M21);
	mi->M32 = -(m->M30 * mi->M02 + m->M31 * mi->M12 + m->M32 * mi->M22);
	mi->M33 = 1.0f; // always 0

	// return success 
	return(1);

} // end Mat_Inverse_4X4

/////////////////////////////////////////////////////////////////

void Print_Mat_4X4(MATRIX4X4_PTR ma, const char *name = "M")
{
	// prints out a 4x4 matrix
	Write_Error("\n%s=\n", name);
	for (int r = 0; r < 4; r++, Write_Error("\n"))
		for (int c = 0; c < 4; c++)
			Write_Error("%f ", ma->M[r][c]);

} // end Print_Mat_4X4

/////////////////////////////////////////////////////////////////

void Init_Parm_Line2D(POINT2D_PTR p_init,
	POINT2D_PTR p_term, PARMLINE2D_PTR p)
{
	// this initializes a parametric 2d line, note that the function
	// computes v=p_pend - p_init, thus when t=0 the line p=p0+v*t = p0
	// and whne t=1, p=p1, this way the segement is traced out from
	// p0 to p1 via 0<= t <= 1

	// start point
	VECTOR2D_INIT(&(p->p0), p_init);

	// end point
	VECTOR2D_INIT(&(p->p1), p_term);

	// now compute direction vector from p0->p1
	VECTOR2D_Build(p_init, p_term, &(p->v));

} // end Init_Parm_Line2D

/////////////////////////////////////////////////////////////////

void Init_Parm_Line3D(POINT3D_PTR p_init,
	POINT3D_PTR p_term, PARMLINE3D_PTR p)
{
	// this initializes a parametric 3d line, note that the function
	// computes v=p_pend - p_init, thus when t=0 the line p=p0+v*t = p0
	// and whne t=1, p=p1, this way the segement is traced out from
	// p0 to p1 via 0<= t <= 1

	// start point
	VECTOR3D_INIT(&(p->p0), p_init);

	// end point
	VECTOR3D_INIT(&(p->p1), p_term);

	// now compute direction vector from p0->p1
	VECTOR3D_Build(p_init, p_term, &(p->v));

} // end Init_Parm_Line3D

/////////////////////////////////////////////////////////////////

void Compute_Parm_Line2D(PARMLINE2D_PTR p, float t, POINT2D_PTR pt)
{
	// this function computes the value of the sent parametric 
	// line at the value of t

	pt->x = p->p0.x + p->v.x * t;
	pt->y = p->p0.y + p->v.y * t;

} // end Compute_Parm_Line2D

///////////////////////////////////////////////////////////////

void Compute_Parm_Line3D(PARMLINE3D_PTR p, float t, POINT3D_PTR pt)
{
	// this function computes the value of the sent parametric 
	// line at the value of t

	pt->x = p->p0.x + p->v.x * t;
	pt->y = p->p0.y + p->v.y * t;
	pt->z = p->p0.z + p->v.z * t;

} // end Compute_Parm_Line3D

///////////////////////////////////////////////////////////////////

int Intersect_Parm_Lines2D(PARMLINE2D_PTR p1, PARMLINE2D_PTR p2,
	float *t1, float *t2)
{
	// this function computes the interesection of the two parametric 
	// line segments the function returns true if the segments 
	// interesect and sets the values of t1 and t2 to the t values that 
	// the intersection occurs on the lines p1 and p2 respectively, 
	// however, the function may send back t value not in the range [0,1]
	// this means that the segments don't intersect, but the lines that 
	// they represent do, thus a retun of 0 means no intersection, a 
	// 1 means intersection on the segments and a 2 means the lines 
	// intersect, but not necessarily the segments and 3 means that 
	// the lines are the same, thus they intersect everywhere

	// basically we have a system of 2d linear equations, we need
	// to solve for t1, t2 when x,y are both equal (if that's possible)

	// step 1: test for parallel lines, if the direction vectors are 
	// scalar multiples then the lines are parallel and can't possible
	// intersect unless the lines overlap

	float det_p1p2 = (p1->v.x * p2->v.y - p1->v.y * p2->v.x);
	if (fabs(det_p1p2) <= EPSILON_E5) {
		// at this point, the lines either don't intersect at all
		// or they are the same lines, in which case they may intersect
		// at one or many points along the segments, at this point we 
		// will assume that the lines don't intersect at all, but later
		// we may need to rewrite this function and take into 
		// consideration the overlap and singular point exceptions
		return(PARM_LINE_NO_INTERSECT);

	} // end if

	// step 2: now compute the t1, t2 values for intersection
	// we have two lines of the form
	// p    = p0    +  v*t, specifically
	// p1   = p10   + v1*t1

	// p1.x = p10.x + v1.x*t1
	// p1.y = p10.y + v1.y*t1

	// p2 = p20 + v2*t2
	// p2   = p20   + v2*t2

	// p2.x = p20.x + v2.x*t2
	// p2.y = p20.y + v2.y*t2
	// solve the system when x1 = x2 and y1 = y2
	// explained in chapter 4
	*t1 = (p2->v.x * (p1->p0.y - p2->p0.y) - p2->v.y * (p1->p0.x - p2->p0.x))
		/ det_p1p2;

	*t2 = (p1->v.x * (p1->p0.y - p2->p0.y) - p1->v.y * (p1->p0.x - p2->p0.x))
		/ det_p1p2;

	// test if the lines intersect on the segments
	if ((*t1 >= 0) && (*t1 <= 1) && (*t2 >= 0) && (*t2 <= 1))
		return(PARM_LINE_INTERSECT_IN_SEGMENT);
	else
		return(PARM_LINE_INTERSECT_OUT_SEGMENT);

} // end Intersect_Parm_Lines2D

///////////////////////////////////////////////////////////////

int Intersect_Parm_Lines2D(PARMLINE2D_PTR p1, PARMLINE2D_PTR p2, POINT2D_PTR pt)
{
	// this function computes the interesection of the two 
	// parametric line segments the function returns true if 
	// the segments interesect and sets the values of pt to the 
	// intersection point, however, the function may send back a 
	// value not in the range [0,1] on the segments this means 
	// that the segments don't intersect, but the lines that 
	// they represent do, thus a retun of 0 means no intersection, 
	// a 1 means intersection on the segments and a 2 means 
	// the lines intersect, but not necessarily the segments

	// basically we have a system of 2d linear equations, we need
	// to solve for t1, t2 when x,y are both equal (if that's possible)

	// step 1: test for parallel lines, if the direction vectors are 
	// scalar multiples then the lines are parallel and can't possible
	// intersect

	float t1, t2, det_p1p2 = (p1->v.x * p2->v.y - p1->v.y * p2->v.x);

	if (fabs(det_p1p2) <= EPSILON_E5) {
		// at this point, the lines either don't intersect at all
		// or they are the same lines, in which case they may intersect
		// at one or many points along the segments, at this point we 
		// will assume that the lines don't intersect at all, but later
		// we may need to rewrite this function and take into 
		// consideration the overlap and singular point exceptions
		return(PARM_LINE_NO_INTERSECT);

	} // end if

	// step 2: now compute the t1, t2 values for intersection
	// we have two lines of the form
	// p    = p0    +  v*t, specifically
	// p1   = p10   + v1*t1

	// p1.x = p10.x + v1.x*t1
	// p1.y = p10.y + v1.y*t1

	// p2 = p20 + v2*t2
	// p2   = p20   + v2*t2

	// p2.x = p20.x + v2.x*t2
	// p2.y = p20.y + v2.y*t2
	// solve the system when x1 = x2 and y1 = y2
	// explained in chapter 4
	t1 = (p2->v.x * (p1->p0.y - p2->p0.y) - p2->v.y * (p1->p0.x - p2->p0.x))
		/ det_p1p2;

	t2 = (p1->v.x * (p1->p0.y - p2->p0.y) - p1->v.y * (p1->p0.x - p2->p0.x))
		/ det_p1p2;

	// now compute point of intersection
	pt->x = p1->p0.x + p1->v.x * t1;
	pt->y = p1->p0.y + p1->v.y * t1;

	// test if the lines intersect on the segments
	if ((t1 >= 0) && (t1 <= 1) && (t2 >= 0) && (t2 <= 1))
		return(PARM_LINE_INTERSECT_IN_SEGMENT);
	else
		return(PARM_LINE_INTERSECT_OUT_SEGMENT);

} // end Intersect_Parm_Lines2D

///////////////////////////////////////////////////////////////

void PLANE3D_Init(PLANE3D_PTR plane, POINT3D_PTR p0,
	VECTOR3D_PTR normal, int normalize = 0)
{
	// this function initializes a 3d plane

	// copy the point
	POINT3D_COPY(&plane->p0, p0);

	// if normalize is 1 then the normal is made into a unit vector
	if (!normalize)
		VECTOR3D_COPY(&plane->n, normal);
	else {
		// make normal into unit vector
		VECTOR3D_Normalize(normal, &plane->n);
	} // end else

} // end PLANE3D_Init

//////////////////////////////////////////////////////////////

float Compute_Point_In_Plane3D(POINT3D_PTR pt, PLANE3D_PTR plane)
{
	// test if the point in on the plane, in the positive halfspace
	// or negative halfspace
	float hs = plane->n.x * (pt->x - plane->p0.x) +
		plane->n.y * (pt->y - plane->p0.y) +
		plane->n.z * (pt->z - plane->p0.z);

	// return half space value
	return(hs);

} // end Compute_Point_In_Plane3D

///////////////////////////////////////////////////////////////

int Intersect_Parm_Line3D_Plane3D(PARMLINE3D_PTR pline,
	PLANE3D_PTR plane,
	float *t, POINT3D_PTR pt)
{
	// this function determines where the sent parametric line 
	// intersects the plane the function will project the line 
	// infinitely in both directions, to compute the intersection, 
	// but the line segment defined by p intersected the plane iff t e [0,1]
	// also the function returns 0 for no intersection, 1 for 
	// intersection of the segment and the plane and 2 for intersection 
	// of the line along the segment and the plane 3, the line lies 
	// in the plane

	// first test of the line and the plane are parallel, if so 
	// then they can't intersect unless the line lies in the plane!

	float plane_dot_line = VECTOR3D_Dot(&pline->v, &plane->n);

	if (fabs(plane_dot_line) <= EPSILON_E5) {
		// the line and plane are co-planer, does the line lie 
		// in the plane?
		if (fabs(Compute_Point_In_Plane3D(&pline->p0, plane)) <= EPSILON_E5)
			return(PARM_LINE_INTERSECT_EVERYWHERE);
		else
			return(PARM_LINE_NO_INTERSECT);
	} // end if

	// from chapter 4 we know that we can solve for the t where 
	// intersection occurs by
	// a*(x0+vx*t) + b*(y0+vy*t) + c*(z0+vz*t) + d =0
	// t = -(a*x0 + b*y0 + c*z0 + d)/(a*vx + b*vy + c*vz)
	// x0,y0,z0, vx,vy,vz, define the line
	// d = (-a*xp0 - b*yp0 - c*zp0), xp0, yp0, zp0, define the point on the plane 

	*t = -(plane->n.x * pline->p0.x +
		plane->n.y * pline->p0.y +
		plane->n.z * pline->p0.z -
		plane->n.x * plane->p0.x -
		plane->n.y * plane->p0.y -
		plane->n.z * plane->p0.z) / (plane_dot_line);

	// now plug t into the parametric line and solve for x,y,z
	pt->x = pline->p0.x + pline->v.x * (*t);
	pt->y = pline->p0.y + pline->v.y * (*t);
	pt->z = pline->p0.z + pline->v.z * (*t);

	// test interval of t
	if (*t >= 0.0 && *t <= 1.0)
		return(PARM_LINE_INTERSECT_IN_SEGMENT);
	else
		return(PARM_LINE_INTERSECT_OUT_SEGMENT);

} // end Intersect_Parm_Line3D_Plane3D

/////////////////////////////////////////////////////////////////

#endif

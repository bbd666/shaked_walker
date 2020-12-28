#include "pch.h"
#include "NewtonUtil.h"

dVector Rel2AbsPoint(const NewtonBody* const Body, dFloat*  PointRel) {
	dMatrix M;
	dVector P;
	NewtonBodyGetMatrix(Body, &M[0][0]);
	P = dVector(PointRel[0], PointRel[1], PointRel[2], 1.0f);
	return M.TransformVector(P);
}

dVector Rel2AbsVector(const NewtonBody* const Body, dFloat*  VectorRel) {
	dMatrix M;
	dVector P;
	NewtonBodyGetMatrix(Body, &M[0][0]);
	P = dVector(VectorRel[0], VectorRel[1], VectorRel[2], 0.0f);
	return M.TransformVector(P);
}

void AddForceAtPos(const NewtonBody* const Body, dFloat* const Force, dFloat* const Point) {
	dVector R, Torque, CoM;
	NewtonBodyGetCentreOfMass(Body, &CoM[0]);
	Torque=Rel2AbsPoint(Body, &CoM[0]);
	R = Point - &Torque.m_x;
	Torque = R.CrossProduct(Force);
	NewtonBodyAddForce(Body, Force);
	NewtonBodyAddTorque(Body, &Torque.m_x);
}

void AddForceAtRelPos(const NewtonBody* const Body, dFloat* const Force, dFloat* const Point) {
	dVector Pos;
	Pos = Rel2AbsPoint(Body, &Point[0]);
	AddForceAtPos(Body, Force,&Pos.m_x);
}

void AddRelForceAtRelPos(const NewtonBody* const Body, dFloat* const Force, dFloat* const Point) {
	dVector Pos, ForceGlob;
	Pos = Rel2AbsPoint(Body, &Point[0]);
	ForceGlob = Rel2AbsVector(Body, &Force[0]);
	AddForceAtPos(Body, &ForceGlob.m_x, &Pos.m_x);
}

void AddRelForceAtPos(const NewtonBody* const Body, dFloat* const Force, dFloat* const Point) {
	dVector ForceGlob;
	ForceGlob = Rel2AbsVector(Body, &Force[0]);
	AddForceAtPos(Body, &ForceGlob.m_x, Point);
}

dVector AbsVelatRelPos(const NewtonBody* const Body, dFloat* const Point) {
	dVector Omega,CenterVel,CoM,V;
	NewtonBodyGetCentreOfMass(Body, &CoM[0]);
	NewtonBodyGetVelocity(Body, &CenterVel[0]);
	NewtonBodyGetOmega(Body, &Omega[0]);
	V = Rel2AbsPoint(Body, &Point[0]) - Rel2AbsPoint(Body, &CoM[0]);
	V = Omega.CrossProduct(V);
	V = CenterVel + V;
	return V;
}

dVector AbsVelatPos(const NewtonBody* const Body, dFloat* const Point) {
	dVector Omega, CenterVel, CoM, V;
	NewtonBodyGetCentreOfMass(Body, &CoM[0]);
	NewtonBodyGetVelocity(Body, &CenterVel[0]);
	NewtonBodyGetOmega(Body, &Omega[0]);
	V = Rel2AbsPoint(Body, &CoM[0]);
	V = Point - &V[0];
	V = Omega.CrossProduct(V);
	V = CenterVel + V;
	return V;
}

dVector AbsAccatRelPos(const NewtonBody* const Body, dFloat* const Point) {
	dMatrix M,M0;
	dVector P,Omega,CoM,ForceGlob,Torque,Alpha,V,W;
	float InvMass, InvIxx, InvIyy, InvIzz;
	NewtonBodyGetMatrix(Body, &M[0][0]);
	NewtonBodyGetInvMass(Body, &InvMass, &InvIxx, &InvIyy, &InvIzz);
	M0 = M;
	M.Inverse();
	NewtonBodyGetForce(Body, &ForceGlob[0]);
	NewtonBodyGetTorque(Body, &Torque[0]);
	P = Torque;
	P.m_w = 0.0f;
	Alpha=M.TransformVector(P);
	Alpha.m_x = Alpha.m_x * InvIxx;
	Alpha.m_y = Alpha.m_y * InvIyy;
	Alpha.m_z = Alpha.m_z * InvIzz;
	Alpha.m_w = 0.f;
	Alpha = M0.TransformVector(P);
	ForceGlob = ForceGlob.Scale(InvMass);
	NewtonBodyGetOmega(Body, &Omega[0]);
	NewtonBodyGetCentreOfMass(Body, &CoM[0]);
	V = Rel2AbsPoint(Body, &Point[0]) - Rel2AbsPoint(Body, &CoM[0]);
	V = Omega.CrossProduct(V);
	V = Omega.CrossProduct(V);
	W = Rel2AbsPoint(Body, &Point[0]) - Rel2AbsPoint(Body, &CoM[0]);
	W = Alpha.CrossProduct(W);
	W = ForceGlob + W;
	return V + W;
}
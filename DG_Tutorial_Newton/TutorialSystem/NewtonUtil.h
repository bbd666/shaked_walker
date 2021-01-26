#ifndef NEWTONUTIL_H
#define NEWTONUTIL_H

#pragma once
#include "pch.h"

void AddForceAtPos(const NewtonBody* const Body, dFloat* const Force, dFloat* const Point);
void AddForceAtRelPos(const NewtonBody* const Body, dFloat* const Force, dFloat* const Point);
void AddRelForceAtRelPos(const NewtonBody* const Body, dFloat* const Force, dFloat* const Point);
void AddRelForceAtPos(const NewtonBody* const Body, dFloat* const Force, dFloat* const Point);
dVector Rel2AbsPoint(const NewtonBody* const Body, dFloat* const PointRel) ;
dVector Rel2AbsVector(const NewtonBody* const Body, dFloat* const VectorRel) ;
dVector AbsVelatRelPos(const NewtonBody* const Body, dFloat* const Point) ;
dVector AbsVelatPos(const NewtonBody* const Body, dFloat* const Point) ;
dVector AbsAccatRelPos(const NewtonBody* const Body, dFloat* const Point) ;

#endif //NEWTONUTIL_H
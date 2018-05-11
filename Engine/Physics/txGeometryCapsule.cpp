#include "txGeometryCapsule.h"
#include "txCamera.h"
#include "txRigidBody.h"
#include "txPhysicsUtility.h"
#include "Utility.h"
#include "txEngineRoot.h"
#include "txRenderUtility.h"

txGeometryCapsule::txGeometryCapsule(txRigidBody* body, dSpaceID space, const GEOM_TYPE& type)
:
txGeometry(body, space, type),
mCylinderHeight(0.0f),
mRadius(0.0f)
{
	;
}

void txGeometryCapsule::create(const float& cylinderHeight, const float& radius, const float& mass, const VECTOR3& pos, const MATRIX3& rot)
{
	mCylinderHeight = cylinderHeight;
	mRadius = radius;

	// ��ʼ�����ɵ��ӵ���״
	startCreateGeometry(pos, rot);
	// ������װ�Ľ�����״
	dGeomID tempGeom = dCreateCapsule(NULL, mRadius, mCylinderHeight + 2 * mRadius);
	// ���������ɵ��ӵ���״
	endCreateGeometry(tempGeom, mass);
}

void txGeometryCapsule::render(txCamera* camera)
{
	txRenderUtility::renderCapsule(camera, mCylinderHeight + 2 * mRadius, mRadius, mRigidBody->getWorldTransform() * mRelativeTransform, mColour, mWireframe);
}

void txGeometryCapsule::setMass(const float& mass)
{
	dMass capsuleMass;
	dMassSetCapsuleTotal(&capsuleMass, mass, 3, mRadius, mCylinderHeight + 2 * mRadius);
	// �任����
	transformMass(capsuleMass);
	// ˢ�¸�������
	mRigidBody->refreshMass();
}

void txGeometryCapsule::setCylinderHeight(const float& cylinderHeight)
{
	setCapsuleParams(cylinderHeight, mRadius);
}

void txGeometryCapsule::setRadius(const float& radius)
{
	setCapsuleParams(mCylinderHeight, radius);
}

void txGeometryCapsule::setCapsuleParams(const float& cylinderHeight, const float& radius)
{
	mCylinderHeight = cylinderHeight;
	mRadius = radius;
	dGeomID geom = mGeom;
	if (dGeomGetClass(geom) == dGeomTransformClass)
	{
		geom = dGeomTransformGetGeom(geom);
	}
	dGeomCapsuleSetParams(geom, mRadius, mCylinderHeight + 2 * mRadius);
	// �޸Ĳ�����,��Ҫˢ������
	setMass(mMass.mass);
}
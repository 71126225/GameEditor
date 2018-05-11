#include "txGeometrySphere.h"
#include "txCamera.h"
#include "txPhysicsUtility.h"
#include "txRigidBody.h"
#include "txRenderUtility.h"

txGeometrySphere::txGeometrySphere(txRigidBody* body, dSpaceID space, const GEOM_TYPE& type)
:
txGeometry(body, space, type),
mRadius(0.0f)
{
	;
}

void txGeometrySphere::create(const float& radius, const float& mass, const VECTOR3& pos, const MATRIX3& rot)
{
	mRadius = radius;

	// ��ʼ�����ɵ��ӵ���״
	startCreateGeometry(pos, rot);
	// �������任��������״
	dGeomID tempGeom = dCreateSphere(NULL, mRadius);
	// ���������ɵ��ӵ���״
	endCreateGeometry(tempGeom, mass);
}

void txGeometrySphere::render(txCamera* camera)
{
	txRenderUtility::renderSphere(camera, mRadius, mRigidBody->getWorldTransform() * mRelativeTransform, mColour, mWireframe);
}

void txGeometrySphere::setMass(const float& mass)
{
	dMass sphereMass;
	dMassSetSphereTotal(&sphereMass, mass, mRadius);
	// �任����
	transformMass(sphereMass);
	// ˢ�¸�������
	mRigidBody->refreshMass();
}

void txGeometrySphere::setRadius(const float& radius)
{
	mRadius = radius;
	dGeomID geom = mGeom;
	if (dGeomGetClass(geom) == dGeomTransformClass)
	{
		geom = dGeomTransformGetGeom(geom);
	}
	dGeomSphereSetRadius(geom, mRadius);
	// �޸Ĳ�����,��Ҫˢ������
	setMass(mMass.mass);
}
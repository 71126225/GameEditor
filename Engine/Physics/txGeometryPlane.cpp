#include "txGeometryPlane.h"
#include "txCamera.h"
#include "txPhysicsUtility.h"
#include "txRigidBody.h"
#include "txRenderUtility.h"

txGeometryPlane::txGeometryPlane(txRigidBody* body, dSpaceID space, const GEOM_TYPE& type)
:
txGeometry(body, space, type),
mX(0.0f),
mZ(0.0f)
{
	;
}

void txGeometryPlane::create(const float& x, const float& z, const float& mass, const VECTOR3& pos, const MATRIX3& rot)
{
	mX = x;
	mZ = z;

	// ��ʼ�����ɵ��ӵ���״
	startCreateGeometry(pos, rot);

	// �������任��������״
	dGeomID tempGeom = createPlane();

	// ���������ɵ��ӵ���״
	endCreateGeometry(tempGeom, mass);
}

dGeomID txGeometryPlane::createPlane()
{
	// ���������ι�4������
	mVertexCount = 4;
	mVertices = TRACE_NEW_ARRAY(float, mVertexCount * 3, mVertices);
	fillPlaneVertices(mX, mZ);

	// ���������ι�6������
	mIndexCount = 2 * 3;
	mIndices = TRACE_NEW_ARRAY(dTriIndex, mIndexCount, mIndices);
	int triangleIndex = 0;
	mIndices[triangleIndex * 3 + 0] = 0;
	mIndices[triangleIndex * 3 + 1] = 1;
	mIndices[triangleIndex * 3 + 2] = 2;
	++triangleIndex;

	mIndices[triangleIndex * 3 + 0] = 0;
	mIndices[triangleIndex * 3 + 1] = 2;
	mIndices[triangleIndex * 3 + 2] = 3;
	++triangleIndex;

	// ���ݶ�����������ݴ���ƽ��ģ��
	dTriMeshDataID triMeshData = dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuildSingle(triMeshData, mVertices, 3 * sizeof(float), mVertexCount, mIndices, mIndexCount, 3 * sizeof(dTriIndex));
	dGeomID geom = dCreateTriMesh(NULL, triMeshData, NULL, NULL, NULL);
	return geom;
}

void txGeometryPlane::fillPlaneVertices(const float& x, const float& z)
{
	int vertexIndex = 0;
	mVertices[vertexIndex * 3 + 0] = x / 2.0f;
	mVertices[vertexIndex * 3 + 1] = 0.0f;
	mVertices[vertexIndex * 3 + 2] = -z / 2.0f;
	++vertexIndex;

	mVertices[vertexIndex * 3 + 0] = -x / 2.0f;
	mVertices[vertexIndex * 3 + 1] = 0.0f;
	mVertices[vertexIndex * 3 + 2] = -z / 2.0f;
	++vertexIndex;

	mVertices[vertexIndex * 3 + 0] = -x / 2.0f;
	mVertices[vertexIndex * 3 + 1] = 0.0f;
	mVertices[vertexIndex * 3 + 2] = z / 2.0f;
	++vertexIndex;

	mVertices[vertexIndex * 3 + 0] = x / 2.0f;
	mVertices[vertexIndex * 3 + 1] = 0.0f;
	mVertices[vertexIndex * 3 + 2] = z / 2.0f;
	++vertexIndex;
}

void txGeometryPlane::render(txCamera* camera)
{
	txRenderUtility::renderPlane(camera, mX, mZ, mRigidBody->getWorldTransform() * mRelativeTransform, mColour, mWireframe);
}

void txGeometryPlane::setMass(const float& mass)
{
	// ʹ�ú��ӵ�����������
	dMass planeMass;
	dMassSetBoxTotal(&planeMass, mass, mX, 0.001f, mZ);
	// �任����
	transformMass(planeMass);
	// ˢ�¸�������
	mRigidBody->refreshMass();
}

void txGeometryPlane::setWidthX(const float& x)
{
	setSide(x, mZ);
}

void txGeometryPlane::setLengthZ(const float& z)
{
	setSide(mX, z);
}

void txGeometryPlane::setSide(const float& x, const float& z)
{
	mX = x;
	mZ = z;
	dGeomID geom = mGeom;
	if (dGeomGetClass(geom) == dGeomTransformClass)
	{
		geom = dGeomTransformGetGeom(geom);
	}
	fillPlaneVertices(mX, mZ);
	// �޸Ĳ�����,��Ҫˢ������
	setMass(mMass.mass);
}
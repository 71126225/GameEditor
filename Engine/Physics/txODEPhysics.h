#ifndef _TX_ODE_PHYSICS_H_
#define _TX_ODE_PHYSICS_H_

#include "ode.h"
#include "txEngineDefine.h"

class txCamera;
class txRigidBody;
class txODEPhysics
{
public:
	txODEPhysics();
	virtual ~txODEPhysics();
	virtual void init();
	virtual void update(float elapsedTime);
	virtual void render(txCamera* camera);
	virtual void destroy();
	const dWorldID& getPhysicsWorld() { return mPhysicsWorld; }
	const dSpaceID& getPhysicsSpace() { return mPhysicsSpace; }
	const dJointGroupID& getContactGroup() { return mContactGroup; }
	// ����һ���յĸ���,û������,û����״
	txRigidBody* createBody(const bool& showCollideShape = true);
	// ����һ������,����ͬ�����е�������״Ҳһ������
	void destroyBody(txRigidBody* body);
protected:
	dWorldID mPhysicsWorld;									// ��������
	dSpaceID mPhysicsSpace;									// ����ռ�
	dJointGroupID mContactGroup;							// �Ӵ���ļ���
	dThreadingImplementationID mThreadingImplementation;	// �߳�ʵ��
	dThreadingThreadPoolID mThreadingPool;					// �̳߳�
	txSet<txRigidBody*> mBodyList;
};

#endif
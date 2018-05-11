#include "txODEPhysics.h"
#include "txEngineLog.h"
#include "txEngineRoot.h"
#include "txRigidBody.h"
#include "Utility.h"

txODEPhysics::txODEPhysics()
:
mPhysicsWorld(NULL),
mPhysicsSpace(NULL),
mContactGroup(NULL),
mThreadingPool(NULL),
mThreadingImplementation(NULL)
{
	;
}

txODEPhysics::~txODEPhysics()
{
	destroy();
}

void txODEPhysics::init()
{
	if (dInitODE2(0) == 0)
	{
		ENGINE_ERROR("error : init ode failed!");
		return;
	}
	mPhysicsWorld = dWorldCreate();

	mPhysicsSpace = dSimpleSpaceCreate(0);
	mContactGroup = dJointGroupCreate(0);
	dWorldSetGravity(mPhysicsWorld, 0.0f, -9.81f, 0.0f);
	// ����ȫ�ֻ��Լ����ֵ
	dWorldSetCFM(mPhysicsWorld, 1e-5f);
	// ����ȫ���ݴ�ϵ��,ʵ����0.2����Ĭ��ֵ
	dWorldSetERP(mPhysicsWorld, 0.2f);

	mThreadingImplementation = dThreadingAllocateMultiThreadedImplementation();
	mThreadingPool = dThreadingAllocateThreadPool(4, 0, dAllocateFlagBasicData, NULL);
	dThreadingThreadPoolServeMultiThreadedImplementation(mThreadingPool, mThreadingImplementation);
	dWorldSetStepThreadingImplementation(mPhysicsWorld, dThreadingImplementationGetFunctions(mThreadingImplementation), mThreadingImplementation);

	dAllocateODEDataForThread(dAllocateMaskAll);
}

void txODEPhysics::update(float elapsedTime)
{
	// ������ײ���ص�����
	dSpaceCollide(mPhysicsSpace, NULL, txRigidBody::nearCallback);
	dWorldStep(mPhysicsWorld, 0.01f);
	dJointGroupEmpty(mContactGroup);

	auto iterBody = mBodyList.begin();
	auto iterBodyEnd = mBodyList.end();
	FOR_STL(mBodyList, ; iterBody != iterBodyEnd; ++iterBody)
	{
		txRigidBody* rigidBody = *iterBody;
		rigidBody->update();
	}
	END_FOR_STL(mBodyList);
}

void txODEPhysics::render(txCamera* camera)
{
	auto iterBody = mBodyList.begin();
	auto iterBodyEnd = mBodyList.end();
	FOR_STL(mBodyList, ; iterBody != iterBodyEnd; ++iterBody)
	{
		txRigidBody* rigidBody = *iterBody;
		if (rigidBody->getShowCollideShape())
		{
			rigidBody->render(camera);
		}
	}
	END_FOR_STL(mBodyList);
}

void txODEPhysics::destroy()
{
	// �������еĸ���
	auto iterBody = mBodyList.begin();
	auto iterBodyEnd = mBodyList.end();
	FOR_STL(mBodyList, ; iterBody != iterBodyEnd; ++iterBody)
	{
		txRigidBody* rigidBody = *iterBody;
		TRACE_DELETE(rigidBody);
	}
	END_FOR_STL(mBodyList);
	mBodyList.clear();

	// ������������
	dThreadingImplementationShutdownProcessing(mThreadingImplementation);
	dThreadingFreeThreadPool(mThreadingPool);
	dWorldSetStepThreadingImplementation(mPhysicsWorld, NULL, NULL);
	dThreadingFreeImplementation(mThreadingImplementation);

	dJointGroupDestroy(mContactGroup);
	dSpaceDestroy(mPhysicsSpace);
	dWorldDestroy(mPhysicsWorld);
	dCloseODE();
	mContactGroup = NULL;
	mPhysicsSpace = NULL;
	mPhysicsWorld = NULL;
	mThreadingImplementation = NULL;
	mThreadingPool = NULL;
}

txRigidBody* txODEPhysics::createBody(const bool& showCollideShape)
{
	txRigidBody* rigidbody = TRACE_NEW(txRigidBody, rigidbody, showCollideShape);
	rigidbody->init();
	mBodyList.insert(rigidbody);
	return rigidbody;
}

void txODEPhysics::destroyBody(txRigidBody* body)
{
	auto iterBody = mBodyList.find(body);
	if (iterBody == mBodyList.end())
	{
		ENGINE_ERROR("error : can not find body! destroyBody");
		return;
	}
	TRACE_DELETE(body);
	mBodyList.erase(iterBody);
}
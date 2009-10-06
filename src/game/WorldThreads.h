#ifndef __WORLD_THREADS_H
#define __WORLD_THREADS_H

#include <ace/OS_NS_signal.h>

ACE_Based::Thread* CLIUp = NULL;
ACE_Based::Thread* BGUpDate = NULL;
ACE_Based::Thread* OPvPUpdate = NULL;

class CLIUpdater : public ACE_Based::Runnable
{
public:
	void run()
	{	
		
		while (!World::IsStopped())
		{ 
			sWorld.ProcessCliCommands();
			ACE_Based::Thread::Sleep (sWorld.CLIThreadTimer);
		}
	}
};

class BGUpdater : public ACE_Based::Runnable
{
public:
	void run()
	{		
		while (!World::IsStopped())
		{
			sBattleGroundMgr.Update(sWorld.DiffTime);
			sWorld.RecordTimeDiff("UpdateBattleGroundMgr");
			ACE_Based::Thread::Sleep (sWorld.DiffTime);
		}
	}
};

class OPvPUpdater : public ACE_Based::Runnable
{
public:
	void run()
	{
		while (!World::IsStopped())
		{				
			sOutdoorPvPMgr.Update(sWorld.DiffTime);
			sWorld.RecordTimeDiff("UpdateOutdoorPvPMgr");
			ACE_Based::Thread::Sleep (sWorld.DiffTime);
		}
	}
};

void World::ThreadStarter()
{
	CLIThreadTimer = 500;
	CLIUp = new ACE_Based::Thread (new CLIUpdater);
	BGUpDate = new ACE_Based::Thread (new BGUpdater);
	OPvPUpdate = new ACE_Based::Thread (new OPvPUpdater);
}

void World::ThreadEnd()
{
	CLIUp->destroy ();
	BGUpDate->destroy ();
	OPvPUpdate->destroy ();
}

#endif
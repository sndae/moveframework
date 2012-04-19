#include "MoveManager.h"
#include "MoveDevice.h"
#include "MoveRawCalibration.h"
#include "MoveLock.h"

namespace Move
{
	MoveManager* MoveManager::instance=0;

	MoveManager::MoveManager()
	{
		FPS=0;
		eye=0;
		moveCount=-1;
	}


	MoveManager::~MoveManager()
	{
		for (int i=0;i<moveCount;++i)
		{
			delete moves[i];
		}
		if (eye)
		{
			delete eye;
		}
		MoveLock::deinit();
		MoveDevice::CloseMoves();
	}

	int MoveManager::initMoves()
	{
		//if already initialized or there is no moves found, return -1
		if (moveCount!=-1)
		{
			return moveCount;
		}
		if (!MoveDevice::OpenMoves())
		{
			return 0;
		}
		moveCount = MoveDevice::GetMoveCount();

		MoveLock::init(moveCount);

		moveData.resize(moveCount);

		for (int i=0;i<moveCount;++i)
		{
			MoveController* ctrl=new MoveController(i);
			moves.push_back(ctrl);
		}

		return moveCount;
	}

	void MoveManager::closeMoves(void)
	{
	
	}

	bool MoveManager::initCamera(int numMoves)
	{
		eye=new EyeController();
		if (!eye->initCamera(numMoves))
		{
			delete eye;
			eye=0;
			return false;
		}
		return true;
	}

	void MoveManager::closeCamera()
	{

	}

	int MoveManager::getNumUsedMoves()
	{
		return moveCount;
	}

	int MoveManager::getNumAllMoves()
	{
		return 0;
	}

	void MoveManager::notify(int moveId, MoveMessage message)
	{
		std::list<IMoveObserver*>::iterator it;
		for ( it=observers.begin() ; it != observers.end(); it++ )
		{
			(*it)->moveNotify(moveId, message);
		}
	}

	MoveData& MoveManager::getMoveDataEx(int moveId)
	{
		return moveData[moveId];
	}

	int MoveManager::pairMoves()
	{
		return MoveDevice::PairMoves();
	}

	IMoveController* MoveManager::getMove(int moveId)
	{
		return moves[moveId];
	}

	IEyeController* MoveManager::getEye()
	{
		return eye;
	}

	void MoveManager::subsribe(IMoveObserver* observer)
	{
		observers.push_back(observer);
	}
	void MoveManager::unsubsribe(IMoveObserver* observer)
	{
		observers.remove(observer);
	}

	void MoveManager::moveUpdated(int moveId)
	{
		std::list<IMoveObserver*>::iterator it;
		for ( it=observers.begin() ; it != observers.end(); it++ )
		{
			(*it)->moveUpdated(moveId, moveData[moveId]);
		}
	}
	void MoveManager::moveKeyPressed(int moveId, MoveButton button)
	{
		std::list<IMoveObserver*>::iterator it;
		for ( it=observers.begin() ; it != observers.end(); it++ )
		{
			(*it)->moveKeyPressed(moveId, button);
		}
	}
	void MoveManager::moveKeyReleased(int moveId, MoveButton button)
	{
		std::list<IMoveObserver*>::iterator it;
		for ( it=observers.begin() ; it != observers.end(); it++ )
		{
			(*it)->moveKeyReleased(moveId, button);
		}
	}
}


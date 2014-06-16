
/******************************************************************************/

#include "stdafx.h"
#include "SPlayerFactory.h"

/******************************************************************************/

namespace MG
{

    //--------------------------------------------------------------------------------------
    SPlayerFactory::SPlayerFactory()
    {
    }

    //--------------------------------------------------------------------------------------
    SPlayerFactory::~SPlayerFactory()
    {
    }

    //-----------------------------------------------------------------------------
    void SPlayerFactory::createSPlayerPtr( SPlayerPtr& ptr,  AccountIdType accountId, I32 client_id, Str16& accName )
    {
        SPlayer* player = new SPlayer();
        //mSPlayerPoolCs.lock();
        {
            //player = MG_POOL_NEW_ONCE_CONSTRUCTION(mSPlayerPool, SPlayer, ());
        }
        //mSPlayerPoolCs.unlock();

        player->clear();
        player->initialize( accountId, client_id, accName );

        ptr.bind( player );

        ptr.mFactory = this;
    }

    //-----------------------------------------------------------------------------
    void SPlayerFactory::destroySPlayerPtr( SPlayerPtr& ptr )
    {
        SPlayer* player = ptr.getPointer();

        if ( player )
        {
            player->clear();
            player->unInitialize();

            mSPlayerPoolCs.lock();
            {
                MG_POOL_DELETE_ONCE_DESTRUCTOR(mSPlayerPool, player);
            }
            mSPlayerPoolCs.unlock();
        }else
        {
            DYNAMIC_EEXCEPT_LOG("player pointer is null!");
        }
    }


}